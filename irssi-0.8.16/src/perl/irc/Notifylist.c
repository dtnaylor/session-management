/*
 * This file was generated automatically by ExtUtils::ParseXS version 2.2210 from the
 * contents of Notifylist.xs. Do not edit this file, edit Notifylist.xs instead.
 *
 *	ANY CHANGES MADE HERE WILL BE LOST! 
 *
 */

#line 1 "Notifylist.xs"
#include "module.h"

#line 13 "Notifylist.c"
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

#line 65 "Notifylist.c"

XS(XS_Irssi__Irc_notifies); /* prototype to pass -Wmissing-prototypes */
XS(XS_Irssi__Irc_notifies)
{
#ifdef dVAR
    dVAR; dXSARGS;
#else
    dXSARGS;
#endif
    if (items != 0)
       croak_xs_usage(cv,  "");
    PERL_UNUSED_VAR(ax); /* -Wall */
    SP -= items;
    {
#line 9 "Notifylist.xs"
	GSList *tmp;
#line 82 "Notifylist.c"
#line 11 "Notifylist.xs"
	for (tmp = notifies; tmp != NULL; tmp = tmp->next) {
		XPUSHs(sv_2mortal(plain_bless(tmp->data, "Irssi::Irc::Notifylist")));
	}
#line 87 "Notifylist.c"
	PUTBACK;
	return;
    }
}


XS(XS_Irssi__Irc_notifylist_add); /* prototype to pass -Wmissing-prototypes */
XS(XS_Irssi__Irc_notifylist_add)
{
#ifdef dVAR
    dVAR; dXSARGS;
#else
    dXSARGS;
#endif
    if (items != 4)
       croak_xs_usage(cv,  "mask, ircnets, away_check, idle_check_time");
    {
	char *	mask = (char *)SvPV_nolen(ST(0));
	char *	ircnets = (char *)SvPV_nolen(ST(1));
	int	away_check = (int)SvIV(ST(2));
	int	idle_check_time = (int)SvIV(ST(3));
	Irssi__Irc__Notifylist	RETVAL;
#line 22 "Notifylist.xs"
	if (idle_check_time != 0)
		croak("Notify -idle has been removed");
	RETVAL = notifylist_add(mask, ircnets, away_check);
#line 114 "Notifylist.c"
	ST(0) = plain_bless(RETVAL, "Irssi::Irc::Notifylist");

	sv_2mortal(ST(0));
    }
    XSRETURN(1);
}


XS(XS_Irssi__Irc_notifylist_remove); /* prototype to pass -Wmissing-prototypes */
XS(XS_Irssi__Irc_notifylist_remove)
{
#ifdef dVAR
    dVAR; dXSARGS;
#else
    dXSARGS;
#endif
    if (items != 1)
       croak_xs_usage(cv,  "mask");
    {
	char *	mask = (char *)SvPV_nolen(ST(0));

	notifylist_remove(mask);
    }
    XSRETURN_EMPTY;
}


XS(XS_Irssi__Irc_notifylist_ison); /* prototype to pass -Wmissing-prototypes */
XS(XS_Irssi__Irc_notifylist_ison)
{
#ifdef dVAR
    dVAR; dXSARGS;
#else
    dXSARGS;
#endif
    if (items != 2)
       croak_xs_usage(cv,  "nick, serverlist");
    {
	char *	nick = (char *)SvPV_nolen(ST(0));
	char *	serverlist = (char *)SvPV_nolen(ST(1));
	Irssi__Irc__Server	RETVAL;

	RETVAL = notifylist_ison(nick, serverlist);
	ST(0) = iobject_bless((SERVER_REC *)RETVAL);

	sv_2mortal(ST(0));
    }
    XSRETURN(1);
}


XS(XS_Irssi__Irc_notifylist_find); /* prototype to pass -Wmissing-prototypes */
XS(XS_Irssi__Irc_notifylist_find)
{
#ifdef dVAR
    dVAR; dXSARGS;
#else
    dXSARGS;
#endif
    if (items != 2)
       croak_xs_usage(cv,  "mask, ircnet");
    {
	char *	mask = (char *)SvPV_nolen(ST(0));
	char *	ircnet = (char *)SvPV_nolen(ST(1));
	Irssi__Irc__Notifylist	RETVAL;

	RETVAL = notifylist_find(mask, ircnet);
	ST(0) = plain_bless(RETVAL, "Irssi::Irc::Notifylist");

	sv_2mortal(ST(0));
    }
    XSRETURN(1);
}


XS(XS_Irssi__Irc__Server_notifylist_ison_server); /* prototype to pass -Wmissing-prototypes */
XS(XS_Irssi__Irc__Server_notifylist_ison_server)
{
#ifdef dVAR
    dVAR; dXSARGS;
#else
    dXSARGS;
#endif
    if (items != 2)
       croak_xs_usage(cv,  "server, nick");
    {
	Irssi__Irc__Server	server = irssi_ref_object(ST(0));
	char *	nick = (char *)SvPV_nolen(ST(1));
	int	RETVAL;
	dXSTARG;

	RETVAL = notifylist_ison_server(server, nick);
	XSprePUSH; PUSHi((IV)RETVAL);
    }
    XSRETURN(1);
}


XS(XS_Irssi__Irc__Notifylist_ircnets_match); /* prototype to pass -Wmissing-prototypes */
XS(XS_Irssi__Irc__Notifylist_ircnets_match)
{
#ifdef dVAR
    dVAR; dXSARGS;
#else
    dXSARGS;
#endif
    if (items != 2)
       croak_xs_usage(cv,  "rec, ircnet");
    {
	Irssi__Irc__Notifylist	rec = irssi_ref_object(ST(0));
	char *	ircnet = (char *)SvPV_nolen(ST(1));
	int	RETVAL;
	dXSTARG;

	RETVAL = notifylist_ircnets_match(rec, ircnet);
	XSprePUSH; PUSHi((IV)RETVAL);
    }
    XSRETURN(1);
}

#ifdef __cplusplus
extern "C"
#endif
XS(boot_Irssi__Irc__Notifylist); /* prototype to pass -Wmissing-prototypes */
XS(boot_Irssi__Irc__Notifylist)
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

        (void)newXSproto_portable("Irssi::Irc::notifies", XS_Irssi__Irc_notifies, file, "");
        (void)newXSproto_portable("Irssi::Irc::notifylist_add", XS_Irssi__Irc_notifylist_add, file, "$$$$");
        (void)newXSproto_portable("Irssi::Irc::notifylist_remove", XS_Irssi__Irc_notifylist_remove, file, "$");
        (void)newXSproto_portable("Irssi::Irc::notifylist_ison", XS_Irssi__Irc_notifylist_ison, file, "$$");
        (void)newXSproto_portable("Irssi::Irc::notifylist_find", XS_Irssi__Irc_notifylist_find, file, "$$");
        (void)newXSproto_portable("Irssi::Irc::Server::notifylist_ison_server", XS_Irssi__Irc__Server_notifylist_ison_server, file, "$$");
        (void)newXSproto_portable("Irssi::Irc::Notifylist::ircnets_match", XS_Irssi__Irc__Notifylist_ircnets_match, file, "$$");
#if (PERL_REVISION == 5 && PERL_VERSION >= 9)
  if (PL_unitcheckav)
       call_list(PL_scopestack_ix, PL_unitcheckav);
#endif
    XSRETURN_YES;
}
