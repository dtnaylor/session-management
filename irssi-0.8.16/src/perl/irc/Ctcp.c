/*
 * This file was generated automatically by ExtUtils::ParseXS version 2.2210 from the
 * contents of Ctcp.xs. Do not edit this file, edit Ctcp.xs instead.
 *
 *	ANY CHANGES MADE HERE WILL BE LOST! 
 *
 */

#line 1 "Ctcp.xs"
#include "module.h"
#include "ctcp.h"

#line 14 "Ctcp.c"
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

#line 66 "Ctcp.c"

XS(XS_Irssi_ctcp_register); /* prototype to pass -Wmissing-prototypes */
XS(XS_Irssi_ctcp_register)
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

	ctcp_register(name);
    }
    XSRETURN_EMPTY;
}


XS(XS_Irssi_ctcp_unregister); /* prototype to pass -Wmissing-prototypes */
XS(XS_Irssi_ctcp_unregister)
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

	ctcp_unregister(name);
    }
    XSRETURN_EMPTY;
}


XS(XS_Irssi__Irc__Server_ctcp_send_reply); /* prototype to pass -Wmissing-prototypes */
XS(XS_Irssi__Irc__Server_ctcp_send_reply)
{
#ifdef dVAR
    dVAR; dXSARGS;
#else
    dXSARGS;
#endif
    if (items != 2)
       croak_xs_usage(cv,  "server, data");
    {
	Irssi__Irc__Server	server = irssi_ref_object(ST(0));
	char *	data = (char *)SvPV_nolen(ST(1));

	ctcp_send_reply(server, data);
    }
    XSRETURN_EMPTY;
}

#ifdef __cplusplus
extern "C"
#endif
XS(boot_Irssi__Irc__Ctcp); /* prototype to pass -Wmissing-prototypes */
XS(boot_Irssi__Irc__Ctcp)
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

        (void)newXSproto_portable("Irssi::ctcp_register", XS_Irssi_ctcp_register, file, "$");
        (void)newXSproto_portable("Irssi::ctcp_unregister", XS_Irssi_ctcp_unregister, file, "$");
        (void)newXSproto_portable("Irssi::Irc::Server::ctcp_send_reply", XS_Irssi__Irc__Server_ctcp_send_reply, file, "$$");
#if (PERL_REVISION == 5 && PERL_VERSION >= 9)
  if (PL_unitcheckav)
       call_list(PL_scopestack_ix, PL_unitcheckav);
#endif
    XSRETURN_YES;
}

