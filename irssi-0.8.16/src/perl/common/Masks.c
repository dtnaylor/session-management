/*
 * This file was generated automatically by ExtUtils::ParseXS version 2.2210 from the
 * contents of Masks.xs. Do not edit this file, edit Masks.xs instead.
 *
 *	ANY CHANGES MADE HERE WILL BE LOST! 
 *
 */

#line 1 "Masks.xs"
#include "module.h"

#line 13 "Masks.c"
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

#line 65 "Masks.c"

XS(XS_Irssi_mask_match); /* prototype to pass -Wmissing-prototypes */
XS(XS_Irssi_mask_match)
{
#ifdef dVAR
    dVAR; dXSARGS;
#else
    dXSARGS;
#endif
    if (items != 4)
       croak_xs_usage(cv,  "mask, nick, user, host");
    {
	char *	mask = (char *)SvPV_nolen(ST(0));
	char *	nick = (char *)SvPV_nolen(ST(1));
	char *	user = (char *)SvPV_nolen(ST(2));
	char *	host = (char *)SvPV_nolen(ST(3));
	int	RETVAL;
	dXSTARG;
#line 13 "Masks.xs"
	RETVAL = mask_match(NULL, mask, nick, user, host);
#line 86 "Masks.c"
	XSprePUSH; PUSHi((IV)RETVAL);
    }
    XSRETURN(1);
}


XS(XS_Irssi_mask_match_address); /* prototype to pass -Wmissing-prototypes */
XS(XS_Irssi_mask_match_address)
{
#ifdef dVAR
    dVAR; dXSARGS;
#else
    dXSARGS;
#endif
    if (items != 3)
       croak_xs_usage(cv,  "mask, nick, address");
    {
	char *	mask = (char *)SvPV_nolen(ST(0));
	char *	nick = (char *)SvPV_nolen(ST(1));
	char *	address = (char *)SvPV_nolen(ST(2));
	int	RETVAL;
	dXSTARG;
#line 23 "Masks.xs"
	RETVAL = mask_match_address(NULL, mask, nick, address);
#line 111 "Masks.c"
	XSprePUSH; PUSHi((IV)RETVAL);
    }
    XSRETURN(1);
}


XS(XS_Irssi_masks_match); /* prototype to pass -Wmissing-prototypes */
XS(XS_Irssi_masks_match)
{
#ifdef dVAR
    dVAR; dXSARGS;
#else
    dXSARGS;
#endif
    if (items != 3)
       croak_xs_usage(cv,  "masks, nick, address");
    {
	char *	masks = (char *)SvPV_nolen(ST(0));
	char *	nick = (char *)SvPV_nolen(ST(1));
	char *	address = (char *)SvPV_nolen(ST(2));
	int	RETVAL;
	dXSTARG;
#line 33 "Masks.xs"
	RETVAL = masks_match(NULL, masks, nick, address);
#line 136 "Masks.c"
	XSprePUSH; PUSHi((IV)RETVAL);
    }
    XSRETURN(1);
}


XS(XS_Irssi__Server_mask_match); /* prototype to pass -Wmissing-prototypes */
XS(XS_Irssi__Server_mask_match)
{
#ifdef dVAR
    dVAR; dXSARGS;
#else
    dXSARGS;
#endif
    if (items != 5)
       croak_xs_usage(cv,  "server, mask, nick, user, host");
    {
	Irssi__Server	server = irssi_ref_object(ST(0));
	char *	mask = (char *)SvPV_nolen(ST(1));
	char *	nick = (char *)SvPV_nolen(ST(2));
	char *	user = (char *)SvPV_nolen(ST(3));
	char *	host = (char *)SvPV_nolen(ST(4));
	int	RETVAL;
	dXSTARG;

	RETVAL = mask_match(server, mask, nick, user, host);
	XSprePUSH; PUSHi((IV)RETVAL);
    }
    XSRETURN(1);
}


XS(XS_Irssi__Server_mask_match_address); /* prototype to pass -Wmissing-prototypes */
XS(XS_Irssi__Server_mask_match_address)
{
#ifdef dVAR
    dVAR; dXSARGS;
#else
    dXSARGS;
#endif
    if (items != 4)
       croak_xs_usage(cv,  "server, mask, nick, address");
    {
	Irssi__Server	server = irssi_ref_object(ST(0));
	char *	mask = (char *)SvPV_nolen(ST(1));
	char *	nick = (char *)SvPV_nolen(ST(2));
	char *	address = (char *)SvPV_nolen(ST(3));
	int	RETVAL;
	dXSTARG;

	RETVAL = mask_match_address(server, mask, nick, address);
	XSprePUSH; PUSHi((IV)RETVAL);
    }
    XSRETURN(1);
}


XS(XS_Irssi__Server_masks_match); /* prototype to pass -Wmissing-prototypes */
XS(XS_Irssi__Server_masks_match)
{
#ifdef dVAR
    dVAR; dXSARGS;
#else
    dXSARGS;
#endif
    if (items != 4)
       croak_xs_usage(cv,  "server, masks, nick, address");
    {
	Irssi__Server	server = irssi_ref_object(ST(0));
	char *	masks = (char *)SvPV_nolen(ST(1));
	char *	nick = (char *)SvPV_nolen(ST(2));
	char *	address = (char *)SvPV_nolen(ST(3));
	int	RETVAL;
	dXSTARG;

	RETVAL = masks_match(server, masks, nick, address);
	XSprePUSH; PUSHi((IV)RETVAL);
    }
    XSRETURN(1);
}

#ifdef __cplusplus
extern "C"
#endif
XS(boot_Irssi__Masks); /* prototype to pass -Wmissing-prototypes */
XS(boot_Irssi__Masks)
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

        (void)newXSproto_portable("Irssi::mask_match", XS_Irssi_mask_match, file, "$$$$");
        (void)newXSproto_portable("Irssi::mask_match_address", XS_Irssi_mask_match_address, file, "$$$");
        (void)newXSproto_portable("Irssi::masks_match", XS_Irssi_masks_match, file, "$$$");
        (void)newXSproto_portable("Irssi::Server::mask_match", XS_Irssi__Server_mask_match, file, "$$$$$");
        (void)newXSproto_portable("Irssi::Server::mask_match_address", XS_Irssi__Server_mask_match_address, file, "$$$$");
        (void)newXSproto_portable("Irssi::Server::masks_match", XS_Irssi__Server_masks_match, file, "$$$$");
#if (PERL_REVISION == 5 && PERL_VERSION >= 9)
  if (PL_unitcheckav)
       call_list(PL_scopestack_ix, PL_unitcheckav);
#endif
    XSRETURN_YES;
}

