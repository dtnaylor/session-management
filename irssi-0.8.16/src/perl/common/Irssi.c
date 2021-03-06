/*
 * This file was generated automatically by ExtUtils::ParseXS version 2.2210 from the
 * contents of Irssi.xs. Do not edit this file, edit Irssi.xs instead.
 *
 *	ANY CHANGES MADE HERE WILL BE LOST! 
 *
 */

#line 1 "Irssi.xs"
#include "module.h"

static int initialized = FALSE;

void perl_expando_init(void);
void perl_expando_deinit(void);

void perl_settings_init(void);
void perl_settings_deinit(void);

#line 21 "Irssi.c"
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

#line 73 "Irssi.c"

XS(XS_Irssi_init); /* prototype to pass -Wmissing-prototypes */
XS(XS_Irssi_init)
{
#ifdef dVAR
    dVAR; dXSARGS;
#else
    dXSARGS;
#endif
    if (items != 0)
       croak_xs_usage(cv,  "");
    {
#line 18 "Irssi.xs"
	if (initialized) return;
	perl_api_version_check("Irssi");
	initialized = TRUE;

        perl_settings_init();
	perl_expando_init();
#line 93 "Irssi.c"
    }
    XSRETURN_EMPTY;
}


XS(XS_Irssi_deinit); /* prototype to pass -Wmissing-prototypes */
XS(XS_Irssi_deinit)
{
#ifdef dVAR
    dVAR; dXSARGS;
#else
    dXSARGS;
#endif
    if (items != 0)
       croak_xs_usage(cv,  "");
    {
#line 28 "Irssi.xs"
	if (!initialized) return;
	perl_expando_deinit();
        perl_settings_deinit();
	initialized = FALSE;
#line 115 "Irssi.c"
    }
    XSRETURN_EMPTY;
}

#ifdef __cplusplus
extern "C"
#endif
XS(boot_Irssi); /* prototype to pass -Wmissing-prototypes */
XS(boot_Irssi)
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

        (void)newXSproto_portable("Irssi::init", XS_Irssi_init, file, "");
        (void)newXSproto_portable("Irssi::deinit", XS_Irssi_deinit, file, "");

    /* Initialisation Section */

#line 34 "Irssi.xs"
        irssi_boot(Channel);
	irssi_boot(Core);
	irssi_boot(Expando);
	irssi_boot(Ignore);
	irssi_boot(Log);
	irssi_boot(Masks);
	irssi_boot(Query);
	irssi_boot(Rawlog);
	irssi_boot(Server);
	irssi_boot(Settings);

#line 161 "Irssi.c"

    /* End of Initialisation Section */

#if (PERL_REVISION == 5 && PERL_VERSION >= 9)
  if (PL_unitcheckav)
       call_list(PL_scopestack_ix, PL_unitcheckav);
#endif
    XSRETURN_YES;
}

