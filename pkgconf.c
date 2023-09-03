
// guile-pkgconf
// Copyright © 2023 Zheng Junjie <873216071@qq.com>


#include <stdio.h>
#include <stdarg.h>
#include <libguile.h>
#include <pkgconf/libpkgconf/libpkgconf.h>
SCM type_table;
SCM_SNARF_INIT(
    type_table = scm_make_weak_value_hash_table(scm_from_int(3000)););

SCM make_foreign_object_n(SCM type, int num, ...) {
  va_list valist;
  void *vals[num];
  va_start(valist, num);
  for (int i = 0; i < num; i++) {
    vals[i]=va_arg(valist, void*);
  }
  va_end(valist);

  if (!vals[0]) {
    return SCM_BOOL_F;
  }
  SCM p = scm_from_pointer(vals[0], NULL);
  SCM d = scm_hash_ref(type_table, p, SCM_BOOL_F);
  if (scm_is_false(d)) {


    SCM obj = scm_make_foreign_object_n(type,num, vals);
    scm_hash_set_x(type_table, p, obj);
    return obj;
  }
  return d;
};

SCM make_foreign_object(SCM type, void *o) {
  return make_foreign_object_n(type, 1,o);
};

SCM make_foreign_object_type(char *cname, scm_t_struct_finalize finalizer) {
  SCM name, slots;
  name = scm_from_utf8_symbol(cname);
  slots = scm_list_1(scm_from_utf8_symbol("%data"));
  return scm_make_foreign_object_type(name, slots, finalizer);
}
#define define_forege_type(c_name, scheme_name, display_name, finalizer)       \
  SCM_SNARF_HERE(SCM c_name)                                                   \
  SCM_SNARF_INIT({                                                             \
    SCM _v = make_foreign_object_type(                                         \
        display_name ? display_name : scheme_name, finalizer);                 \
    c_name = _v;                                                               \
    scm_c_define(scheme_name, _v);                                             \
  })
#define define_enum(n)                                                         \
  SCM_SNARF_INIT(scm_c_define(#n, scm_from_uint32(n)); scm_c_export(#n, NULL));
#define foreign_object_ref(o) scm_foreign_object_ref(o, 0)

static void pkgconf_client_finalizer(SCM scm) {
  pkgconf_client_t *o = foreign_object_ref(scm);
  pkgconf_client_free(o);
}
define_forege_type(gpkgconf_client_t, "<%pc:client>", NULL,
                   pkgconf_client_finalizer);
static SCM set_pc_client_type() {
  gpkgconf_client_t=scm_c_private_ref("pkgconf", "<pc:client>");
  return SCM_UNSPECIFIED;
}
typedef struct pkgref {
  pkgconf_pkg_t *pkg;
  pkgconf_client_t *client;
} pkgref;
static void pkgconf_pkgref_finalizer(SCM scm) {
  pkgref *pkgref=foreign_object_ref(scm);
  pkgconf_pkg_t *pkg = pkgref->pkg;
  pkgconf_client_t *c = pkgref->client;
  pkgconf_pkg_free(c,pkg);
  free(pkgref);
}

define_forege_type(gpkgconf_pkgref_t, "<pc:package>", NULL,
                   pkgconf_pkgref_finalizer);

SCM_DEFINE(client_init, "%client-new", 3, 0, 0, (SCM handle, SCM p,SCM build_list), "") {
  pkgconf_cross_personality_t *personality = NULL;
  if (scm_is_true(p)) {
    personality = pkgconf_cross_personality_find(scm_to_utf8_string(p));
  }
  personality = personality ? personality : pkgconf_cross_personality_default();
  pkgconf_client_t *o = pkgconf_client_new(
      scm_to_pointer(handle), NULL,personality);
  if (scm_is_true(build_list)) {
    pkgconf_client_dir_list_build(o, personality);
  }
  return make_foreign_object(gpkgconf_client_t, o);
}

SCM_DEFINE(client_get_sysroot_dir, "%client-sysroot-dir", 1, 0, 0, (SCM c), "") {
  pkgconf_client_t *client=foreign_object_ref(c);
  const char* dir=pkgconf_client_get_sysroot_dir(client);
  scm_remember_upto_here_1(c);
  return dir ? scm_from_utf8_string(dir) : SCM_BOOL_F;
}

SCM_DEFINE(client_set_sysroot_dir, "%client-set-sysroot-dir", 2, 0, 0, (SCM c,SCM str), "") {
  pkgconf_client_t *client=foreign_object_ref(c);
  char* dir=NULL;
  scm_dynwind_begin (0);
  dir=scm_to_locale_string(str);

  pkgconf_client_set_sysroot_dir(client,dir);

  scm_dynwind_free (dir);
  scm_dynwind_end ();
  scm_remember_upto_here_2(c,str);
  return SCM_UNSPECIFIED;
}

SCM_DEFINE_PUBLIC(s_make_foregin, "%make-foreign-object", 2, 0, 0,
                  (SCM type, SCM p), "") {
  return make_foreign_object(type, scm_to_pointer(p));
}

SCM_DEFINE(gpkgconf_pkg_find, "%pc:package-find", 2, 0, 0, (SCM c, SCM name),
           "") {
  scm_assert_foreign_object_type(gpkgconf_client_t, c);
  pkgconf_client_t *client=foreign_object_ref(c);
  pkgconf_pkg_t *pkg=pkgconf_pkg_find(client ,scm_to_utf8_string(name));
  scm_remember_upto_here_2(c, name);
  if (pkg) {
    pkgref *pkgref=scm_malloc(sizeof(struct pkgref));
    pkgref->pkg=pkg;
    pkgref->client=client;
    return make_foreign_object(gpkgconf_pkgref_t, pkgref);
  }
  return SCM_BOOL_F;
}

void init_pkgconf() {
  scm_c_define_gsubr("%set-pc:client-type",0,0,0, (scm_t_subr) set_pc_client_type);
#ifndef SCM_MAGIC_SNARFER
#include "pkgconf.x"
#endif
}
