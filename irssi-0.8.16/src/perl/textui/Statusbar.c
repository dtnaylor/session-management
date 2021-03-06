/*
 * This file was generated automatically by ExtUtils::ParseXS version 2.2210 from the
 * contents of Statusbar.xs. Do not edit this file, edit Statusbar.xs instead.
 *
 *	ANY CHANGES MADE HERE WILL BE LOST! 
 *
 */

#line 1 "Statusbar.xs"
#include "module.h"

static GHashTable *perl_sbar_defs;

static int check_sbar_destroy(char *key, char *value, char *script)
{
	if (strncmp(value, script, strlen(script)) == 0 &&
	    value[strlen(script)] == ':') {
                statusbar_item_unregister(key);
		g_free(key);
                g_free(value);
		return TRUE;
	}

        return FALSE;
}

static void script_unregister_statusbars(PERL_SCRIPT_REC *script)
{
	g_hash_table_foreach_remove(perl_sbar_defs,
				    (GHRFunc) check_sbar_destroy,
				    script->package);
}

void perl_statusbar_init(void)
{
	perl_sbar_defs = g_hash_table_new((GHashFunc) g_str_hash,
					  (GCompareFunc) g_str_equal);
	signal_add("script destroyed", (SIGNAL_FUNC) script_unregister_statusbars);
}

static void statusbar_item_def_destroy(void *key, void *value)
{
	statusbar_item_unregister(key);
	g_free(key);
        g_free(value);
}

void perl_statusbar_deinit(void)
{
	signal_remove("script destroyed", (SIGNAL_FUNC) script_unregister_statusbars);

	g_hash_table_foreach(perl_sbar_defs,
			     (GHFunc) statusbar_item_def_destroy, NULL);
	g_hash_table_destroy(perl_sbar_defs);
}

static void perl_statusbar_event(char *function, SBAR_ITEM_REC *item,
				 int get_size_only)
{
	dSP;
	int retcount;
	SV *item_sv, **sv;
        HV *hv;

	ENTER;
	SAVETMPS;

	PUSHMARK(SP);
        item_sv = plain_bless(item, "Irssi::TextUI::StatusbarItem");
	XPUSHs(sv_2mortal(item_sv));
	XPUSHs(sv_2mortal(newSViv(get_size_only)));
	PUTBACK;

	retcount = perl_call_pv(function, G_EVAL|G_DISCARD);
	SPAGAIN;

	if (SvTRUE(ERRSV)) {
                PERL_SCRIPT_REC *script;
                char *package;

                package = perl_function_get_package(function);
                script = perl_script_find_package(package);
                g_free(package);

		if (script != NULL) {
                        /* make sure we don't get back here */
			script_unregister_statusbars(script);
		}
		signal_emit("script error", 2, script, SvPV(ERRSV, PL_na));
	} else {
		/* min_size and max_size can be changed, move them to SBAR_ITEM_REC */
		hv = hvref(item_sv);
		if (hv != NULL) {
			sv = hv_fetch(hv, "min_size", 8, 0);
			if (sv != NULL) item->min_size = SvIV(*sv);
			sv = hv_fetch(hv, "max_size", 8, 0);
			if (sv != NULL) item->max_size = SvIV(*sv);
		}
	}

	PUTBACK;
	FREETMPS;
	LEAVE;
}


static void sig_perl_statusbar(SBAR_ITEM_REC *item, int get_size_only)
{
	char *function;

	function = g_hash_table_lookup(perl_sbar_defs, item->config->name);
	if (function != NULL)
		perl_statusbar_event(function, item, get_size_only);
	else {
		/* use default function - this shouldn't actually happen.. */
		statusbar_item_default_handler(item, get_size_only, NULL, "", TRUE);
	}
}

#line 121 "Statusbar.c"
#ifndef PERL_UNUSED_VAR
#  define PERL_UNUSED_VAR(var) if (0) var = var
#endif

#ifndef PERL_ARGS_ASSERT_CROAK_XS_USAGE
#define PERL_ARGS_ASSERT_CROAK_XS_USAGE assert(cv); assert(params)

/* prototype to pass -Wmissing-prototypes */
STATIC void
S_croak_xs_usage(pTHX_ const CV *const cv, const char *const params);

STATIC void
S_croak_xs_usage(pTHX_ const CV *const cv, const char *const params)
{
    const GV *const gv = CvGV(cv);

    PERL_ARGS_ASSERT_CROAK_XS_USAGE;

    if (gv) {
        const char *const gvname = GvNAME(gv);
        const HV *const stash = GvSTASH(gv);
        const char *const hvname = stash ? HvNAME(stash) : NULL;

        if (hvname)
            Perl_croak(aTHX_ "Usage: %s::%s(%s)", hvname, gvname, params);
        else
            Perl_croak(aTHX_ "Usage: %s(%s)", gvname, params);
    } else {
        /* Pants. I don't think that it should be possible to get here. */
        Perl_croak(aTHX_ "Usage: CODE(0x%"UVxf")(%s)", PTR2UV(cv), params);
    }
}
#undef  PERL_ARGS_ASSERT_CROAK_XS_USAGE

#ifdef PERL_IMPLICIT_CONTEXT
#define croak_xs_usage(a,b)	S_croak_xs_usage(aTHX_ a,b)
#else
#define croak_xs_usage		S_croak_xs_usage
#endif

#endif

/* NOTE: the prototype of newXSproto() is different in versions of perls,
 * so we define a portable version of newXSproto()
 */
#ifdef newXS_flags
#define newXSproto_portable(name, c_impl, file, proto) newXS_flags(name, c_impl, file, proto, 0)
#else
#define newXSproto_portable(name, c_impl, file, proto) (PL_Sv=(SV*)newXS(name, c_impl, file), sv_setpv(PL_Sv, proto), (CV*)PL_Sv)
#endif /* !defined(newXS_flags) */

#line 173 "Statusbar.c"

XS(XS_Irssi_statusbar_item_register); /* prototype to pass -Wmissing-prototypes */
XS(XS_Irssi_statusbar_item_register)
{
#ifdef dVAR
    dVAR; dXSARGS;
#else
    dXSARGS;
#endif
    if (items < 2 || items > 3)
       croak_xs_usage(cv,  "name, value, func = NULL");
    {
	char *	name = (char *)SvPV_nolen(ST(0));
	char *	value = (char *)SvPV_nolen(ST(1));
	char *	func;

	if (items < 3)
	    func = NULL;
	else {
	    func = (char *)SvPV_nolen(ST(2));
	}
#line 120 "Statusbar.xs"
	statusbar_item_register(name, value, func == NULL || *func == '\0' ? NULL : sig_perl_statusbar);
	if (func != NULL) {
		g_hash_table_insert(perl_sbar_defs, g_strdup(name),
				    g_strdup_printf("%s::%s", perl_get_package(), func));
	}
#line 201 "Statusbar.c"
    }
    XSRETURN_EMPTY;
}


XS(XS_Irssi_statusbar_item_unregister); /* prototype to pass -Wmissing-prototypes */
XS(XS_Irssi_statusbar_item_unregister)
{
#ifdef dVAR
    dVAR; dXSARGS;
#else
    dXSARGS;
#endif
    if (items != 1)
       croak_xs_usage(cv,  "name");
    {
	char *	name = (char *)SvPV_nolen(ST(0));
#line 130 "Statusbar.xs"
        gpointer key, value;
#line 221 "Statusbar.c"
#line 132 "Statusbar.xs"
	if (g_hash_table_lookup_extended(perl_sbar_defs, name, &key, &value)) {
                g_hash_table_remove(perl_sbar_defs, name);
		g_free(key);
                g_free(value);
	}
	statusbar_item_unregister(name);
#line 229 "Statusbar.c"
    }
    XSRETURN_EMPTY;
}


XS(XS_Irssi_statusbar_items_redraw); /* prototype to pass -Wmissing-prototypes */
XS(XS_Irssi_statusbar_items_redraw)
{
#ifdef dVAR
    dVAR; dXSARGS;
#else
    dXSARGS;
#endif
    if (items != 1)
       croak_xs_usage(cv,  "name");
    {
	char *	name = (char *)SvPV_nolen(ST(0));

	statusbar_items_redraw(name);
    }
    XSRETURN_EMPTY;
}


XS(XS_Irssi_statusbars_recreate_items); /* prototype to pass -Wmissing-prototypes */
XS(XS_Irssi_statusbars_recreate_items)
{
#ifdef dVAR
    dVAR; dXSARGS;
#else
    dXSARGS;
#endif
    if (items != 0)
       croak_xs_usage(cv,  "");
    {

	statusbars_recreate_items();
    }
    XSRETURN_EMPTY;
}


XS(XS_Irssi__TextUI__StatusbarItem_default_handler); /* prototype to pass -Wmissing-prototypes */
XS(XS_Irssi__TextUI__StatusbarItem_default_handler)
{
#ifdef dVAR
    dVAR; dXSARGS;
#else
    dXSARGS;
#endif
    if (items < 4 || items > 5)
       croak_xs_usage(cv,  "item, get_size_only, str, data, escape_vars = TRUE");
    {
	Irssi__TextUI__StatusbarItem	item = irssi_ref_object(ST(0));
	int	get_size_only = (int)SvIV(ST(1));
	char *	str = (char *)SvPV_nolen(ST(2));
	char *	data = (char *)SvPV_nolen(ST(3));
	int	escape_vars;
#line 158 "Statusbar.xs"
	HV *hv;
#line 290 "Statusbar.c"

	if (items < 5)
	    escape_vars = TRUE;
	else {
	    escape_vars = (int)SvIV(ST(4));
	}
#line 160 "Statusbar.xs"
	statusbar_item_default_handler(item, get_size_only,
				       *str == '\0' ? NULL : str,
				       data, escape_vars);
	hv = hvref(ST(0));
	hv_store(hv, "min_size", 8, newSViv(item->min_size), 0);
	hv_store(hv, "max_size", 8, newSViv(item->max_size), 0);
#line 304 "Statusbar.c"
    }
    XSRETURN_EMPTY;
}

#ifdef __cplusplus
extern "C"
#endif
XS(boot_Irssi__TextUI__Statusbar); /* prototype to pass -Wmissing-prototypes */
XS(boot_Irssi__TextUI__Statusbar)
{
#ifdef dVAR
    dVAR; dXSARGS;
#else
    dXSARGS;
#endif
#if (PERL_REVISION == 5 && PERL_VERSION < 9)
    char* file = __FILE__;
#else
    const char* file = __FILE__;
#endif

    PERL_UNUSED_VAR(cv); /* -W */
    PERL_UNUSED_VAR(items); /* -W */
#ifdef XS_APIVERSION_BOOTCHECK
    XS_APIVERSION_BOOTCHECK;
#endif
    XS_VERSION_BOOTCHECK ;

        (void)newXSproto_portable("Irssi::statusbar_item_register", XS_Irssi_statusbar_item_register, file, "$$;$");
        (void)newXSproto_portable("Irssi::statusbar_item_unregister", XS_Irssi_statusbar_item_unregister, file, "$");
        (void)newXSproto_portable("Irssi::statusbar_items_redraw", XS_Irssi_statusbar_items_redraw, file, "$");
        (void)newXSproto_portable("Irssi::statusbars_recreate_items", XS_Irssi_statusbars_recreate_items, file, "");
        (void)newXSproto_portable("Irssi::TextUI::StatusbarItem::default_handler", XS_Irssi__TextUI__StatusbarItem_default_handler, file, "$$$$;$");
#if (PERL_REVISION == 5 && PERL_VERSION >= 9)
  if (PL_unitcheckav)
       call_list(PL_scopestack_ix, PL_unitcheckav);
#endif
    XSRETURN_YES;
}

