/* (c) 2010 John Mair (banisterfiend), MIT license */
/* */
/* include a module (and its singleton) into an inheritance chain */
/* only includes a single module, see real_include.rb for multi-module version */

#include <ruby.h>
#include "compat.h"

#ifdef RUBY_19
static VALUE
class_alloc(VALUE flags, VALUE klass)
{
  rb_classext_t *ext = ALLOC(rb_classext_t);
  NEWOBJ(obj, struct RClass);
  OBJSETUP(obj, klass, flags);
  obj->ptr = ext;
  RCLASS_IV_TBL(obj) = 0;
  RCLASS_M_TBL(obj) = 0;
  RCLASS_SUPER(obj) = 0;
  RCLASS_IV_INDEX_TBL(obj) = 0;
  return (VALUE)obj;
}
#endif


static VALUE
class_to_s(VALUE self)
{
  VALUE attached = rb_iv_get(self, "__attached__");
  
  if (attached) 
    return rb_mod_name(rb_iv_get(attached, "__module__"));
  else
    return rb_mod_name(rb_iv_get(self, "__module__"));
}

/* totally hacked up version of include_class_new() from class.c; brings in singletons into inheritance chain */
static VALUE
include_class_new(VALUE module, VALUE super)
{
  /* base case for recursion */
  if (module == rb_singleton_class(rb_cModule))
    return module;

  if (TYPE(module) == T_ICLASS) {

    /* real_include */
    if (!NIL_P(rb_iv_get(module, "__module__")))
      module = rb_iv_get(module, "__module__");

    /* ordinary Module#include */
    else
      module = KLASS_OF(module);
  }
    
  /* allocate iclass */
#ifdef RUBY_19
  VALUE klass = class_alloc(T_ICLASS, rb_singleton_class(rb_cModule));
#else
  NEWOBJ(klass, struct RClass);
  OBJSETUP(klass, rb_singleton_class(rb_cModule), T_ICLASS);
#endif

  /* if the module hasn't yet got an ivtbl, create it */
  if (!RCLASS_IV_TBL(module))
    RCLASS_IV_TBL(module) = st_init_numtable();

  /* we want to point to the original ivtbl for normal modules, so that we bring in constants */
  RCLASS_IV_TBL(klass) = RCLASS_IV_TBL(module);
  
  /* we want to point to the original module's mtbl */
  RCLASS_M_TBL(klass) = RCLASS_M_TBL(module);
  RCLASS_SUPER(klass) = super;

  if (TYPE(module) == T_MODULE ||  FL_TEST(module, FL_SINGLETON))
    rb_iv_set((VALUE)klass, "__module__", module);
    
  /* create IClass for module's singleton  */
  /* if super is 0 then we're including into a module (not a class), so treat as special case */
  VALUE meta = include_class_new(KLASS_OF(module), super ? KLASS_OF(super) : rb_cModule);

  /* don't mess with (Module) */
  if (meta != rb_singleton_class(rb_cModule)) {

    /* set it as a singleton */
    FL_SET(meta, FL_SINGLETON);

    /* we want a fresh ivtbl for singleton classes (so we can redefine __attached__) */
    RCLASS_IV_TBL(meta) = st_init_numtable();

    /* attach singleton to module */
    rb_iv_set(meta, "__attached__", (VALUE)klass);

    /* attach the #to_s method to the metaclass (so #ancestors doesn't look weird) */
    rb_define_singleton_method(meta, "to_s", class_to_s, 0);
  }
  /* assign the metaclass to module's klass */
  KLASS_OF(klass) = meta;

  OBJ_INFECT(klass, module);
  OBJ_INFECT(klass, super);

  return (VALUE)klass;
}

static VALUE
rb_real_include_module_one(VALUE klass, VALUE module)
{
  VALUE p, c;
  int changed = 0;

  rb_frozen_class_p(klass);
  if (!OBJ_UNTRUSTED(klass)) {
    rb_secure(4);
  }

  if (TYPE(module) != T_MODULE) {
    Check_Type(module, T_MODULE);
  }

  OBJ_INFECT(klass, module);
  c = klass;

  /* ensure singleton classes exist, both for includer and includee */
  rb_singleton_class(module);
  rb_singleton_class(klass);
  
  while (module) {
    int superclass_seen = FALSE;

    if (RCLASS_M_TBL(klass) == RCLASS_M_TBL(module))
      rb_raise(rb_eArgError, "cyclic include detected");
    /* ignore if the module included already in superclasses */
    for (p = RCLASS_SUPER(klass); p; p = RCLASS_SUPER(p)) {
      switch (BUILTIN_TYPE(p)) {
      case T_ICLASS:
        if (RCLASS_M_TBL(p) == RCLASS_M_TBL(module)) {
          if (!superclass_seen) {
            c = p;  /* move insertion point */
          }
          goto skip;
        }
        break;
      case T_CLASS:
        superclass_seen = TRUE;
        break;
      }
    }

    /* we're including the module, so create the iclass */
    VALUE imod = include_class_new(module, RCLASS_SUPER(c));

    /* module gets included directly above c, so set the super */
    RCLASS_SUPER(c) = imod;

    /* also do the same for parallel inheritance chain (singleton classes) */
    RCLASS_SUPER(KLASS_OF(c)) = KLASS_OF(imod);
    c = imod;
        
    if (RMODULE_M_TBL(module) && RMODULE_M_TBL(module)->num_entries)
      changed = 1;
  skip:
    module = RCLASS_SUPER(module);
  }
  if (changed) rb_clear_cache();

  return Qnil;
}

void
Init_real_include_one() {
  rb_define_method(rb_cModule, "real_include_one", rb_real_include_module_one, 1);
}
