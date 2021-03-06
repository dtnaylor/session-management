/*
 * This file was generated automatically by ExtUtils::ParseXS version 2.2210 from the
 * contents of Server.xs. Do not edit this file, edit Server.xs instead.
 *
 *	ANY CHANGES MADE HERE WILL BE LOST! 
 *
 */

#line 1 "Server.xs"
#include "module.h"

static GSList *register_hash2list(HV *hv)
{
	HE *he;
	GSList *list;

	if (hv == NULL)
		return NULL;

	list = NULL;
	hv_iterinit(hv);
	while ((he = hv_iternext(hv)) != NULL) {
		I32 len;
		char *key = hv_iterkey(he, &len);
		int value = (int)SvIV(HeVAL(he));

		list = g_slist_append(list, g_strdup(key));
		list = g_slist_append(list, GINT_TO_POINTER(value));
	}
	return list;
}

static GSList *event_hash2list(HV *hv)
{
	HE *he;
	GSList *list;

	if (hv == NULL)
		return NULL;

	list = NULL;
	hv_iterinit(hv);
	while ((he = hv_iternext(hv)) != NULL) {
		I32 len;
		char *key = hv_iterkey(he, &len);
		char *value = SvPV(HeVAL(he), PL_na);

		list = g_slist_append(list, g_strdup(key));
		list = g_slist_append(list, g_strdup(value));
	}
	return list;
}

#line 55 "Server.c"
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

#line 107 "Server.c"

XS(XS_Irssi__Irc__Server_get_channels); /* prototype to pass -Wmissing-prototypes */
XS(XS_Irssi__Irc__Server_get_channels)
{
#ifdef dVAR
    dVAR; dXSARGS;
#else
    dXSARGS;
#endif
    if (items != 1)
       croak_xs_usage(cv,  "server");
    PERL_UNUSED_VAR(ax); /* -Wall */
    SP -= items;
    {
	Irssi__Irc__Server	server = irssi_ref_object(ST(0));
#line 52 "Server.xs"
	char *ret;
#line 125 "Server.c"
#line 54 "Server.xs"
	ret = irc_server_get_channels(server);
	XPUSHs(sv_2mortal(new_pv(ret)));
	g_free(ret);
#line 130 "Server.c"
	PUTBACK;
	return;
    }
}


XS(XS_Irssi__Irc__Server_send_raw); /* prototype to pass -Wmissing-prototypes */
XS(XS_Irssi__Irc__Server_send_raw)
{
#ifdef dVAR
    dVAR; dXSARGS;
#else
    dXSARGS;
#endif
    if (items != 2)
       croak_xs_usage(cv,  "server, cmd");
    {
	Irssi__Irc__Server	server = irssi_ref_object(ST(0));
	char *	cmd = (char *)SvPV_nolen(ST(1));
#line 63 "Server.xs"
	irc_send_cmd(server, cmd);
#line 152 "Server.c"
    }
    XSRETURN_EMPTY;
}


XS(XS_Irssi__Irc__Server_send_raw_now); /* prototype to pass -Wmissing-prototypes */
XS(XS_Irssi__Irc__Server_send_raw_now)
{
#ifdef dVAR
    dVAR; dXSARGS;
#else
    dXSARGS;
#endif
    if (items != 2)
       croak_xs_usage(cv,  "server, cmd");
    {
	Irssi__Irc__Server	server = irssi_ref_object(ST(0));
	char *	cmd = (char *)SvPV_nolen(ST(1));
#line 70 "Server.xs"
	irc_send_cmd_now(server, cmd);
#line 173 "Server.c"
    }
    XSRETURN_EMPTY;
}


XS(XS_Irssi__Irc__Server_send_raw_first); /* prototype to pass -Wmissing-prototypes */
XS(XS_Irssi__Irc__Server_send_raw_first)
{
#ifdef dVAR
    dVAR; dXSARGS;
#else
    dXSARGS;
#endif
    if (items != 2)
       croak_xs_usage(cv,  "server, cmd");
    {
	Irssi__Irc__Server	server = irssi_ref_object(ST(0));
	char *	cmd = (char *)SvPV_nolen(ST(1));
#line 77 "Server.xs"
	irc_send_cmd_first(server, cmd);
#line 194 "Server.c"
    }
    XSRETURN_EMPTY;
}


XS(XS_Irssi__Irc__Server_send_raw_split); /* prototype to pass -Wmissing-prototypes */
XS(XS_Irssi__Irc__Server_send_raw_split)
{
#ifdef dVAR
    dVAR; dXSARGS;
#else
    dXSARGS;
#endif
    if (items != 4)
       croak_xs_usage(cv,  "server, cmd, nickarg, max_nicks");
    {
	Irssi__Irc__Server	server = irssi_ref_object(ST(0));
	char *	cmd = (char *)SvPV_nolen(ST(1));
	int	nickarg = (int)SvIV(ST(2));
	int	max_nicks = (int)SvIV(ST(3));
#line 86 "Server.xs"
	irc_send_cmd_split(server, cmd, nickarg, max_nicks);
#line 217 "Server.c"
    }
    XSRETURN_EMPTY;
}


XS(XS_Irssi__Irc__Server_redirect_register); /* prototype to pass -Wmissing-prototypes */
XS(XS_Irssi__Irc__Server_redirect_register)
{
#ifdef dVAR
    dVAR; dXSARGS;
#else
    dXSARGS;
#endif
    if (items != 6)
       croak_xs_usage(cv,  "command, remote, timeout, start, stop, opt");
    {
	char *	command = (char *)SvPV_nolen(ST(0));
	int	remote = (int)SvIV(ST(1));
	int	timeout = (int)SvIV(ST(2));
	SV *	start = ST(3);
	SV *	stop = ST(4);
	SV *	opt = ST(5);
#line 99 "Server.xs"
	server_redirect_register_list(command, remote, timeout, 
				      register_hash2list(hvref(start)),
				      register_hash2list(hvref(stop)),
				      register_hash2list(hvref(opt)));
#line 245 "Server.c"
    }
    XSRETURN_EMPTY;
}


XS(XS_Irssi__Irc__Server_redirect_event); /* prototype to pass -Wmissing-prototypes */
XS(XS_Irssi__Irc__Server_redirect_event)
{
#ifdef dVAR
    dVAR; dXSARGS;
#else
    dXSARGS;
#endif
    if (items != 7)
       croak_xs_usage(cv,  "server, command, count, arg, remote, failure_signal, signals");
    {
	Irssi__Irc__Server	server = irssi_ref_object(ST(0));
	char *	command = (char *)SvPV_nolen(ST(1));
	int	count = (int)SvIV(ST(2));
	char *	arg = (char *)SvPV_nolen(ST(3));
	int	remote = (int)SvIV(ST(4));
	char *	failure_signal = (char *)SvPV_nolen(ST(5));
	SV *	signals = ST(6);
#line 114 "Server.xs"
	server_redirect_event_list(server, command, count, *arg == '\0' ? NULL : arg, remote,
				   *failure_signal == '\0' ? NULL : failure_signal,
				   event_hash2list(hvref(signals)));
#line 273 "Server.c"
    }
    XSRETURN_EMPTY;
}


XS(XS_Irssi__Irc__Server_redirect_get_signal); /* prototype to pass -Wmissing-prototypes */
XS(XS_Irssi__Irc__Server_redirect_get_signal)
{
#ifdef dVAR
    dVAR; dXSARGS;
#else
    dXSARGS;
#endif
    if (items != 4)
       croak_xs_usage(cv,  "server, prefix, event, args");
    {
	Irssi__Irc__Server	server = irssi_ref_object(ST(0));
	char *	prefix = (char *)SvPV_nolen(ST(1));
	char *	event = (char *)SvPV_nolen(ST(2));
	char *	args = (char *)SvPV_nolen(ST(3));
	char *	RETVAL;
	dXSTARG;
#line 125 "Server.xs"
	RETVAL = (char *) server_redirect_get_signal(server, prefix, event, args);
#line 298 "Server.c"
	sv_setpv(TARG, RETVAL); XSprePUSH; PUSHTARG;
    }
    XSRETURN(1);
}


XS(XS_Irssi__Irc__Server_redirect_peek_signal); /* prototype to pass -Wmissing-prototypes */
XS(XS_Irssi__Irc__Server_redirect_peek_signal)
{
#ifdef dVAR
    dVAR; dXSARGS;
#else
    dXSARGS;
#endif
    if (items != 4)
       croak_xs_usage(cv,  "server, prefix, event, args");
    {
	Irssi__Irc__Server	server = irssi_ref_object(ST(0));
	char *	prefix = (char *)SvPV_nolen(ST(1));
	char *	event = (char *)SvPV_nolen(ST(2));
	char *	args = (char *)SvPV_nolen(ST(3));
#line 136 "Server.xs"
	int redirection;
#line 322 "Server.c"
	char *	RETVAL;
	dXSTARG;
#line 138 "Server.xs"
	RETVAL = (char *) server_redirect_peek_signal(server, prefix, event, args, &redirection);
#line 327 "Server.c"
	sv_setpv(TARG, RETVAL); XSprePUSH; PUSHTARG;
    }
    XSRETURN(1);
}


XS(XS_Irssi__Irc__Server_isupport); /* prototype to pass -Wmissing-prototypes */
XS(XS_Irssi__Irc__Server_isupport)
{
#ifdef dVAR
    dVAR; dXSARGS;
#else
    dXSARGS;
#endif
    if (items != 2)
       croak_xs_usage(cv,  "server, name");
    {
	Irssi__Irc__Server	server = irssi_ref_object(ST(0));
	char *	name = (char *)SvPV_nolen(ST(1));
	char *	RETVAL;
	dXSTARG;
#line 147 "Server.xs"
	RETVAL = (char *) g_hash_table_lookup(server->isupport, name);
#line 351 "Server.c"
	sv_setpv(TARG, RETVAL); XSprePUSH; PUSHTARG;
    }
    XSRETURN(1);
}

#ifdef __cplusplus
extern "C"
#endif
XS(boot_Irssi__Irc__Server); /* prototype to pass -Wmissing-prototypes */
XS(boot_Irssi__Irc__Server)
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

        (void)newXSproto_portable("Irssi::Irc::Server::get_channels", XS_Irssi__Irc__Server_get_channels, file, "$");
        (void)newXSproto_portable("Irssi::Irc::Server::send_raw", XS_Irssi__Irc__Server_send_raw, file, "$$");
        (void)newXSproto_portable("Irssi::Irc::Server::send_raw_now", XS_Irssi__Irc__Server_send_raw_now, file, "$$");
        (void)newXSproto_portable("Irssi::Irc::Server::send_raw_first", XS_Irssi__Irc__Server_send_raw_first, file, "$$");
        (void)newXSproto_portable("Irssi::Irc::Server::send_raw_split", XS_Irssi__Irc__Server_send_raw_split, file, "$$$$");
        (void)newXSproto_portable("Irssi::Irc::Server::redirect_register", XS_Irssi__Irc__Server_redirect_register, file, "$$$$$$");
        (void)newXSproto_portable("Irssi::Irc::Server::redirect_event", XS_Irssi__Irc__Server_redirect_event, file, "$$$$$$$");
        (void)newXSproto_portable("Irssi::Irc::Server::redirect_get_signal", XS_Irssi__Irc__Server_redirect_get_signal, file, "$$$$");
        (void)newXSproto_portable("Irssi::Irc::Server::redirect_peek_signal", XS_Irssi__Irc__Server_redirect_peek_signal, file, "$$$$");
        (void)newXSproto_portable("Irssi::Irc::Server::isupport", XS_Irssi__Irc__Server_isupport, file, "$$");
#if (PERL_REVISION == 5 && PERL_VERSION >= 9)
  if (PL_unitcheckav)
       call_list(PL_scopestack_ix, PL_unitcheckav);
#endif
    XSRETURN_YES;
}

