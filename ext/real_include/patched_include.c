#include <ruby.h>

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

/* patched to work well with real_include */
static VALUE
include_class_new(VALUE module, VALUE super)
{
  VALUE klass = class_alloc(T_ICLASS, rb_cClass);

  if (BUILTIN_TYPE(module) == T_ICLASS) {

    /* correct for real_include'd modules */
    if (!NIL_P(rb_iv_get(module, "__module__")))
      module = rb_iv_get(module, "__module__");
    else
      module = RBASIC(module)->klass;
  }
  if (!RCLASS_IV_TBL(module)) {
    RCLASS_IV_TBL(module) = st_init_numtable();
  }
  RCLASS_IV_TBL(klass) = RCLASS_IV_TBL(module);
  RCLASS_M_TBL(klass) = RCLASS_M_TBL(module);
  RCLASS_SUPER(klass) = super;
  if (TYPE(module) == T_ICLASS) {
    RBASIC(klass)->klass = RBASIC(module)->klass;
  }
  else {
    RBASIC(klass)->klass = module;
  }
  OBJ_INFECT(klass, module);
  OBJ_INFECT(klass, super);

  return (VALUE)klass;
}

static void
rb_patched_include_module(VALUE klass, VALUE module)
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
    c = RCLASS_SUPER(c) = include_class_new(module, RCLASS_SUPER(c));
    if (RMODULE_M_TBL(module) && RMODULE_M_TBL(module)->num_entries)
      changed = 1;
  skip:
    module = RCLASS_SUPER(module);
  }
  if (changed) rb_clear_cache();
}

static VALUE
rb_patched_mod_append_features(VALUE module, VALUE include)
{
  switch (TYPE(include)) {
  case T_CLASS:
  case T_MODULE:
    break;
  default:
    Check_Type(include, T_CLASS);
    break;
  }
  rb_patched_include_module(include, module);

  return module;
}

void
Init_patched_include() {
  rb_define_method(rb_cModule, "include", rb_patched_mod_append_features, 1);
}
