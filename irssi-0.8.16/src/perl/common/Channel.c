/*
 * This file was generated automatically by ExtUtils::ParseXS version 2.2210 from the
 * contents of Channel.xs. Do not edit this file, edit Channel.xs instead.
 *
 *	ANY CHANGES MADE HERE WILL BE LOST! 
 *
 */

#line 1 "Channel.xs"
#include "module.h"

#line 13 "Channel.c"
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

#line 65 "Channel.c"

XS(XS_Irssi_channels); /* prototype to pass -Wmissing-prototypes */
XS(XS_Irssi_channels)
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
#line 9 "Channel.xs"
	GSList *tmp;
#line 82 "Channel.c"
#line 11 "Channel.xs"
	for (tmp = channels; tmp != NULL; tmp = tmp->next) {
		XPUSHs(sv_2mortal(iobject_bless((CHANNEL_REC *) tmp->data)));
	}
#line 87 "Channel.c"
	PUTBACK;
	return;
    }
}


XS(XS_Irssi_channel_find); /* prototype to pass -Wmissing-prototypes */
XS(XS_Irssi_channel_find)
{
#ifdef dVAR
    dVAR; dXSARGS;
#else
    dXSARGS;
#endif
    if (items != 1)
       croak_xs_usage(cv,  "channel");
    {
	char *	channel = (char *)SvPV_nolen(ST(0));
	Irssi__Channel	RETVAL;
#line 19 "Channel.xs"
	RETVAL = channel_find(NULL, channel);
#line 109 "Channel.c"
	ST(0) = iobject_bless((SERVER_REC *)RETVAL);

	sv_2mortal(ST(0));
    }
    XSRETURN(1);
}


XS(XS_Irssi__Server_channels); /* prototype to pass -Wmissing-prototypes */
XS(XS_Irssi__Server_channels)
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
	Irssi__Server	server = irssi_ref_object(ST(0));
#line 31 "Channel.xs"
	GSList *tmp;
#line 134 "Channel.c"
#line 33 "Channel.xs"
	for (tmp = server->channels; tmp != NULL; tmp = tmp->next) {
		XPUSHs(sv_2mortal(iobject_bless((CHANNEL_REC *) tmp->data)));
	}
#line 139 "Channel.c"
	PUTBACK;
	return;
    }
}


XS(XS_Irssi__Server_channels_join); /* prototype to pass -Wmissing-prototypes */
XS(XS_Irssi__Server_channels_join)
{
#ifdef dVAR
    dVAR; dXSARGS;
#else
    dXSARGS;
#endif
    if (items != 3)
       croak_xs_usage(cv,  "server, channels, automatic");
    {
	Irssi__Server	server = irssi_ref_object(ST(0));
	char *	channels = (char *)SvPV_nolen(ST(1));
	int	automatic = (int)SvIV(ST(2));
#line 43 "Channel.xs"
	server->channels_join(server, channels, automatic);
#line 162 "Channel.c"
    }
    XSRETURN_EMPTY;
}


XS(XS_Irssi__Server_channel_find); /* prototype to pass -Wmissing-prototypes */
XS(XS_Irssi__Server_channel_find)
{
#ifdef dVAR
    dVAR; dXSARGS;
#else
    dXSARGS;
#endif
    if (items != 2)
       croak_xs_usage(cv,  "server, name");
    {
	Irssi__Server	server = irssi_ref_object(ST(0));
	char *	name = (char *)SvPV_nolen(ST(1));
	Irssi__Channel	RETVAL;

	RETVAL = channel_find(server, name);
	ST(0) = iobject_bless((SERVER_REC *)RETVAL);

	sv_2mortal(ST(0));
    }
    XSRETURN(1);
}


XS(XS_Irssi__Server_nicks_get_same); /* prototype to pass -Wmissing-prototypes */
XS(XS_Irssi__Server_nicks_get_same)
{
#ifdef dVAR
    dVAR; dXSARGS;
#else
    dXSARGS;
#endif
    if (items != 2)
       croak_xs_usage(cv,  "server, nick");
    PERL_UNUSED_VAR(ax); /* -Wall */
    SP -= items;
    {
	Irssi__Server	server = irssi_ref_object(ST(0));
	char *	nick = (char *)SvPV_nolen(ST(1));
#line 55 "Channel.xs"
	GSList *list, *tmp;
#line 209 "Channel.c"
#line 57 "Channel.xs"
	list = nicklist_get_same(server, nick);

	for (tmp = list; tmp != NULL; tmp = tmp->next->next) {
		XPUSHs(sv_2mortal(iobject_bless((CHANNEL_REC *) tmp->data)));
		XPUSHs(sv_2mortal(iobject_bless((NICK_REC *) tmp->next->data)));
	}
	g_slist_free(list);
#line 218 "Channel.c"
	PUTBACK;
	return;
    }
}


XS(XS_Irssi__Channel_destroy); /* prototype to pass -Wmissing-prototypes */
XS(XS_Irssi__Channel_destroy)
{
#ifdef dVAR
    dVAR; dXSARGS;
#else
    dXSARGS;
#endif
    if (items != 1)
       croak_xs_usage(cv,  "channel");
    {
	Irssi__Channel	channel = irssi_ref_object(ST(0));

	channel_destroy(channel);
    }
    XSRETURN_EMPTY;
}


XS(XS_Irssi__Channel_nick_insert); /* prototype to pass -Wmissing-prototypes */
XS(XS_Irssi__Channel_nick_insert)
{
#ifdef dVAR
    dVAR; dXSARGS;
#else
    dXSARGS;
#endif
    if (items != 2)
       croak_xs_usage(cv,  "channel, nick");
    {
	Irssi__Channel	channel = irssi_ref_object(ST(0));
	Irssi__Nick	nick = irssi_ref_object(ST(1));
#line 78 "Channel.xs"
	nicklist_insert(channel, nick);
#line 259 "Channel.c"
    }
    XSRETURN_EMPTY;
}


XS(XS_Irssi__Channel_nick_remove); /* prototype to pass -Wmissing-prototypes */
XS(XS_Irssi__Channel_nick_remove)
{
#ifdef dVAR
    dVAR; dXSARGS;
#else
    dXSARGS;
#endif
    if (items != 2)
       croak_xs_usage(cv,  "channel, nick");
    {
	Irssi__Channel	channel = irssi_ref_object(ST(0));
	Irssi__Nick	nick = irssi_ref_object(ST(1));
#line 85 "Channel.xs"
	nicklist_remove(channel, nick);
#line 280 "Channel.c"
    }
    XSRETURN_EMPTY;
}


XS(XS_Irssi__Channel_nick_find); /* prototype to pass -Wmissing-prototypes */
XS(XS_Irssi__Channel_nick_find)
{
#ifdef dVAR
    dVAR; dXSARGS;
#else
    dXSARGS;
#endif
    if (items != 2)
       croak_xs_usage(cv,  "channel, nick");
    {
	Irssi__Channel	channel = irssi_ref_object(ST(0));
	char *	nick = (char *)SvPV_nolen(ST(1));
	Irssi__Nick	RETVAL;
#line 92 "Channel.xs"
	RETVAL = nicklist_find(channel, nick);
#line 302 "Channel.c"
	ST(0) = iobject_bless((SERVER_REC *)RETVAL);

	sv_2mortal(ST(0));
    }
    XSRETURN(1);
}


XS(XS_Irssi__Channel_nick_find_mask); /* prototype to pass -Wmissing-prototypes */
XS(XS_Irssi__Channel_nick_find_mask)
{
#ifdef dVAR
    dVAR; dXSARGS;
#else
    dXSARGS;
#endif
    if (items != 2)
       croak_xs_usage(cv,  "channel, mask");
    {
	Irssi__Channel	channel = irssi_ref_object(ST(0));
	char *	mask = (char *)SvPV_nolen(ST(1));
	Irssi__Nick	RETVAL;
#line 101 "Channel.xs"
	RETVAL = nicklist_find_mask(channel, mask);
#line 327 "Channel.c"
	ST(0) = iobject_bless((SERVER_REC *)RETVAL);

	sv_2mortal(ST(0));
    }
    XSRETURN(1);
}


XS(XS_Irssi__Channel_nicks); /* prototype to pass -Wmissing-prototypes */
XS(XS_Irssi__Channel_nicks)
{
#ifdef dVAR
    dVAR; dXSARGS;
#else
    dXSARGS;
#endif
    if (items != 1)
       croak_xs_usage(cv,  "channel");
    PERL_UNUSED_VAR(ax); /* -Wall */
    SP -= items;
    {
	Irssi__Channel	channel = irssi_ref_object(ST(0));
#line 109 "Channel.xs"
	GSList *list, *tmp;
#line 352 "Channel.c"
#line 111 "Channel.xs"
	list = nicklist_getnicks(channel);

	for (tmp = list; tmp != NULL; tmp = tmp->next) {
		XPUSHs(sv_2mortal(iobject_bless((NICK_REC *) tmp->data)));
	}
	g_slist_free(list);
#line 360 "Channel.c"
	PUTBACK;
	return;
    }
}

#ifdef __cplusplus
extern "C"
#endif
XS(boot_Irssi__Channel); /* prototype to pass -Wmissing-prototypes */
XS(boot_Irssi__Channel)
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

        (void)newXSproto_portable("Irssi::channels", XS_Irssi_channels, file, "");
        (void)newXSproto_portable("Irssi::channel_find", XS_Irssi_channel_find, file, "$");
        (void)newXSproto_portable("Irssi::Server::channels", XS_Irssi__Server_channels, file, "$");
        (void)newXSproto_portable("Irssi::Server::channels_join", XS_Irssi__Server_channels_join, file, "$$$");
        (void)newXSproto_portable("Irssi::Server::channel_find", XS_Irssi__Server_channel_find, file, "$$");
        (void)newXSproto_portable("Irssi::Server::nicks_get_same", XS_Irssi__Server_nicks_get_same, file, "$$");
        (void)newXSproto_portable("Irssi::Channel::destroy", XS_Irssi__Channel_destroy, file, "$");
        (void)newXSproto_portable("Irssi::Channel::nick_insert", XS_Irssi__Channel_nick_insert, file, "$$");
        (void)newXSproto_portable("Irssi::Channel::nick_remove", XS_Irssi__Channel_nick_remove, file, "$$");
        (void)newXSproto_portable("Irssi::Channel::nick_find", XS_Irssi__Channel_nick_find, file, "$$");
        (void)newXSproto_portable("Irssi::Channel::nick_find_mask", XS_Irssi__Channel_nick_find_mask, file, "$$");
        (void)newXSproto_portable("Irssi::Channel::nicks", XS_Irssi__Channel_nicks, file, "$");
#if (PERL_REVISION == 5 && PERL_VERSION >= 9)
  if (PL_unitcheckav)
       call_list(PL_scopestack_ix, PL_unitcheckav);
#endif
    XSRETURN_YES;
}

