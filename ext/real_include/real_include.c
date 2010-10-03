#include "compat.h"
#include "ruby.h"

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

static VALUE
include_class_new(VALUE module, VALUE super)
{
  if (module == rb_cModule)
    return module;
    
  VALUE klass = class_alloc(T_ICLASS, rb_cClass);

  if (BUILTIN_TYPE(module) == T_ICLASS) {
    module = RBASIC(module)->klass;
  }
  if (!RCLASS_IV_TBL(module)) {
    RCLASS_IV_TBL(module) = st_init_numtable();
  }
  RCLASS_IV_TBL(klass) = RCLASS_IV_TBL(module);
  RCLASS_M_TBL(klass) = RCLASS_M_TBL(module);
  RCLASS_SUPER(klass) = super;
  /*
    if (TYPE(module) == T_ICLASS) {
    RBASIC(klass)->klass = RBASIC(module)->klass;
    }
  */
  
  /* create IClass for module's singleton  */
  VALUE meta = include_class_new(KLASS_OF(module), KLASS_OF(super)); 
  FL_SET(meta, FL_SINGLETON);

  /* attach singleton to module */
  rb_singleton_class_attached(meta, klass);

  /* assign the metaclass to module's klass */
  KLASS_OF(klass) = meta;
        
  OBJ_INFECT(klass, module);
  OBJ_INFECT(klass, super);

  return (VALUE)klass;
}

void
rb_real_include_module(VALUE klass, VALUE module)
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

  /* ensure singleton class exists */
  rb_singleton_class(module);
  
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

    VALUE imod = include_class_new(module, RCLASS_SUPER(c));
    RCLASS_SUPER(c) = imod;
    RCLASS_SUPER(KLASS_OF(c)) = KLASS_OF(imod);
    c = imod;
        
    if (RMODULE_M_TBL(module) && RMODULE_M_TBL(module)->num_entries)
      changed = 1;
  skip:
    module = RCLASS_SUPER(module);
  }
  if (changed) rb_clear_cache();
}

void
Init_real_include() {
  rb_define_method(rb_cModule, "real_include", rb_real_include_module, 1);
}
