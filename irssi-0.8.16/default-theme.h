const char *default_theme =
"# When testing changes, the easiest way to reload the theme is with /RELOAD.\n"
"# This reloads the configuration file too, so if you did any changes remember\n"
"# to /SAVE it first. Remember also that /SAVE overwrites the theme file with\n"
"# old data so keep backups :)\n"
"\n"
"# TEMPLATES:\n"
"\n"
"# The real text formats that irssi uses are the ones you can find with\n"
"# /FORMAT command. Back in the old days all the colors and texts were mixed\n"
"# up in those formats, and it was really hard to change the colors since you\n"
"# might have had to change them in tens of different places. So, then came\n"
"# this templating system.\n"
"\n"
"# Now the /FORMATs don't have any colors in them, and they also have very\n"
"# little other styling. Most of the stuff you need to change is in this\n"
"# theme file. If you can't change something here, you can always go back\n"
"# to change the /FORMATs directly, they're also saved in these .theme files.\n"
"\n"
"# So .. the templates. They're those {blahblah} parts you see all over the\n"
"# /FORMATs and here. Their usage is simply {name parameter1 parameter2}.\n"
"# When irssi sees this kind of text, it goes to find \"name\" from abstracts\n"
"# block below and sets \"parameter1\" into $0 and \"parameter2\" into $1 (you\n"
"# can have more parameters of course). Templates can have subtemplates.\n"
"# Here's a small example:\n"
"#   /FORMAT format hello {colorify {underline world}}\n"
"#   abstracts = { colorify = \"%G$0-%n\"; underline = \"%U$0-%U\"; }\n"
"# When irssi expands the templates in \"format\", the final string would be:\n"
"#   hello %G%Uworld%U%n\n"
"# ie. underlined bright green \"world\" text.\n"
"# and why \"$0-\", why not \"$0\"? $0 would only mean the first parameter,\n"
"# $0- means all the parameters. With {underline hello world} you'd really\n"
"# want to underline both of the words, not just the hello (and world would\n"
"# actually be removed entirely).\n"
"\n"
"# COLORS:\n"
"\n"
"# You can find definitions for the color format codes in docs/formats.txt.\n"
"\n"
"# There's one difference here though. %n format. Normally it means the\n"
"# default color of the terminal (white mostly), but here it means the\n"
"# \"reset color back to the one it was in higher template\". For example\n"
"# if there was /FORMAT test %g{foo}bar, and foo = \"%Y$0%n\", irssi would\n"
"# print yellow \"foo\" (as set with %Y) but \"bar\" would be green, which was\n"
"# set at the beginning before the {foo} template. If there wasn't the %g\n"
"# at start, the normal behaviour of %n would occur. If you _really_ want\n"
"# to use the terminal's default color, use %N.\n"
"\n"
"#############################################################################\n"
"\n"
"# default foreground color (%N) - -1 is the \"default terminal color\"\n"
"default_color = \"-1\";\n"
"\n"
"# print timestamp/servertag at the end of line, not at beginning\n"
"info_eol = \"false\";\n"
"\n"
"# these characters are automatically replaced with specified color\n"
"# (dark grey by default)\n"
"replaces = { \"[]=\" = \"%K$*%n\"; };\n"
"\n"
"abstracts = {\n"
"  ##\n"
"  ## generic\n"
"  ##\n"
"\n"
"  # text to insert at the beginning of each non-message line\n"
"  line_start = \"%B-%n!%B-%n \";\n"
"\n"
"  # timestamp styling, nothing by default\n"
"  timestamp = \"$*\";\n"
"\n"
"  # any kind of text that needs hilighting, default is to bold\n"
"  hilight = \"%_$*%_\";\n"
"\n"
"  # any kind of error message, default is bright red\n"
"  error = \"%R$*%n\";\n"
"\n"
"  # channel name is printed\n"
"  channel = \"%_$*%_\";\n"
"\n"
"  # nick is printed\n"
"  nick = \"%_$*%_\";\n"
"\n"
"  # nick host is printed\n"
"  nickhost = \"[$*]\";\n"
"\n"
"  # server name is printed\n"
"  server = \"%_$*%_\";\n"
"\n"
"  # some kind of comment is printed\n"
"  comment = \"[$*]\";\n"
"\n"
"  # reason for something is printed (part, quit, kick, ..)\n"
"  reason = \"{comment $*}\";\n"
"\n"
"  # mode change is printed ([+o nick])\n"
"  mode = \"{comment $*}\";\n"
"\n"
"  ##\n"
"  ## channel specific messages\n"
"  ##\n"
"\n"
"  # highlighted nick/host is printed (joins)\n"
"  channick_hilight = \"%C$*%n\";\n"
"  chanhost_hilight = \"{nickhost %c$*%n}\";\n"
"\n"
"  # nick/host is printed (parts, quits, etc.)\n"
"  channick = \"%c$*%n\";\n"
"  chanhost = \"{nickhost $*}\";\n"
"\n"
"  # highlighted channel name is printed\n"
"  channelhilight = \"%c$*%n\";\n"
"\n"
"  # ban/ban exception/invite list mask is printed\n"
"  ban = \"%c$*%n\";\n"
"\n"
"  ##\n"
"  ## messages\n"
"  ##\n"
"\n"
"  # the basic styling of how to print message, $0 = nick mode, $1 = nick\n"
"  msgnick = \"%K<%n$0$1-%K>%n %|\";\n"
"\n"
"  # message from you is printed. \"msgownnick\" specifies the styling of the\n"
"  # nick ($0 part in msgnick) and \"ownmsgnick\" specifies the styling of the\n"
"  # whole line.\n"
"\n"
"  # Example1: You want the message text to be green:\n"
"  #  ownmsgnick = \"{msgnick $0 $1-}%g\";\n"
"  # Example2.1: You want < and > chars to be yellow:\n"
"  #  ownmsgnick = \"%Y{msgnick $0 $1-%Y}%n\";\n"
"  #  (you'll also have to remove <> from replaces list above)\n"
"  # Example2.2: But you still want to keep <> grey for other messages:\n"
"  #  pubmsgnick = \"%K{msgnick $0 $1-%K}%n\";\n"
"  #  pubmsgmenick = \"%K{msgnick $0 $1-%K}%n\";\n"
"  #  pubmsghinick = \"%K{msgnick $1 $0$2-%n%K}%n\";\n"
"  #  ownprivmsgnick = \"%K{msgnick  $*%K}%n\";\n"
"  #  privmsgnick = \"%K{msgnick  %R$*%K}%n\";\n"
"\n"
"  # $0 = nick mode, $1 = nick\n"
"  ownmsgnick = \"{msgnick $0 $1-}\";\n"
"  ownnick = \"%_$*%n\";\n"
"\n"
"  # public message in channel, $0 = nick mode, $1 = nick\n"
"  pubmsgnick = \"{msgnick $0 $1-}\";\n"
"  pubnick = \"%N$*%n\";\n"
"\n"
"  # public message in channel meant for me, $0 = nick mode, $1 = nick\n"
"  pubmsgmenick = \"{msgnick $0 $1-}\";\n"
"  menick = \"%Y$*%n\";\n"
"\n"
"  # public highlighted message in channel\n"
"  # $0 = highlight color, $1 = nick mode, $2 = nick\n"
"  pubmsghinick = \"{msgnick $1 $0$2-%n}\";\n"
"\n"
"  # channel name is printed with message\n"
"  msgchannel = \"%K:%c$*%n\";\n"
"\n"
"  # private message, $0 = nick, $1 = host\n"
"  privmsg = \"[%R$0%K(%r$1-%K)%n] \";\n"
"\n"
"  # private message from you, $0 = \"msg\", $1 = target nick\n"
"  ownprivmsg = \"[%r$0%K(%R$1-%K)%n] \";\n"
"\n"
"  # own private message in query\n"
"  ownprivmsgnick = \"{msgnick  $*}\";\n"
"  ownprivnick = \"%_$*%n\";\n"
"\n"
"  # private message in query\n"
"  privmsgnick = \"{msgnick  %R$*%n}\";\n"
"\n"
"  ##\n"
"  ## Actions (/ME stuff)\n"
"  ##\n"
"\n"
"  # used internally by this theme\n"
"  action_core = \"%_ * $*%n\";\n"
"\n"
"  # generic one that's used by most actions\n"
"  action = \"{action_core $*} \";\n"
"\n"
"  # own action, both private/public\n"
"  ownaction = \"{action $*}\";\n"
"\n"
"  # own action with target, both private/public\n"
"  ownaction_target = \"{action_core $0}%K:%c$1%n \";\n"
"\n"
"  # private action sent by others\n"
"  pvtaction = \"%_ (*) $*%n \";\n"
"  pvtaction_query = \"{action $*}\";\n"
"\n"
"  # public action sent by others\n"
"  pubaction = \"{action $*}\";\n"
"\n"
"\n"
"  ##\n"
"  ## other IRC events\n"
"  ##\n"
"\n"
"  # whois\n"
"  whois = \"%# $[8]0 : $1-\";\n"
"\n"
"  # notices\n"
"  ownnotice = \"[%r$0%K(%R$1-%K)]%n \";\n"
"  notice = \"%K-%M$*%K-%n \";\n"
"  pubnotice_channel = \"%K:%m$*\";\n"
"  pvtnotice_host = \"%K(%m$*%K)\";\n"
"  servernotice = \"%g!$*%n \";\n"
"\n"
"  # CTCPs\n"
"  ownctcp = \"[%r$0%K(%R$1-%K)] \";\n"
"  ctcp = \"%g$*%n\";\n"
"\n"
"  # wallops\n"
"  wallop = \"%_$*%n: \";\n"
"  wallop_nick = \"%n$*\";\n"
"  wallop_action = \"%_ * $*%n \";\n"
"\n"
"  # netsplits\n"
"  netsplit = \"%R$*%n\";\n"
"  netjoin = \"%C$*%n\";\n"
"\n"
"  # /names list\n"
"  names_prefix = \"\";\n"
"  names_nick = \"[%_$0%_$1-] \";\n"
"  names_nick_op = \"{names_nick $*}\";\n"
"  names_nick_halfop = \"{names_nick $*}\";\n"
"  names_nick_voice = \"{names_nick $*}\";\n"
"  names_users = \"[%g$*%n]\";\n"
"  names_channel = \"%G$*%n\";\n"
"\n"
"  # DCC\n"
"  dcc = \"%g$*%n\";\n"
"  dccfile = \"%_$*%_\";\n"
"\n"
"  # DCC chat, own msg/action\n"
"  dccownmsg = \"[%r$0%K($1-%K)%n] \";\n"
"  dccownnick = \"%R$*%n\";\n"
"  dccownquerynick = \"%_$*%n\";\n"
"  dccownaction = \"{action $*}\";\n"
"  dccownaction_target = \"{action_core $0}%K:%c$1%n \";\n"
"\n"
"  # DCC chat, others\n"
"  dccmsg = \"[%G$1-%K(%g$0%K)%n] \";\n"
"  dccquerynick = \"%G$*%n\";\n"
"  dccaction = \"%_ (*dcc*) $*%n %|\";\n"
"\n"
"  ##\n"
"  ## statusbar\n"
"  ##\n"
"\n"
"  # default background for all statusbars. You can also give\n"
"  # the default foreground color for statusbar items.\n"
"  sb_background = \"%4%w\";\n"
"\n"
"  # default backround for \"default\" statusbar group\n"
"  #sb_default_bg = \"%4\";\n"
"  # background for prompt / input line\n"
"  sb_prompt_bg = \"%n\";\n"
"  # background for info statusbar\n"
"  sb_info_bg = \"%8\";\n"
"  # background for topicbar (same default)\n"
"  #sb_topic_bg = \"%4\";\n"
"\n"
"  # text at the beginning of statusbars. sb-item already puts\n"
"  # space there,so we don't use anything by default.\n"
"  sbstart = \"\";\n"
"  # text at the end of statusbars. Use space so that it's never\n"
"  # used for anything.\n"
"  sbend = \" \";\n"
"\n"
"  topicsbstart = \"{sbstart $*}\";\n"
"  topicsbend = \"{sbend $*}\";\n"
"\n"
"  prompt = \"[$*] \";\n"
"\n"
"  sb = \" %c[%n$*%c]%n\";\n"
"  sbmode = \"(%c+%n$*)\";\n"
"  sbaway = \" (%GzZzZ%n)\";\n"
"  sbservertag = \":$0 (change with ^X)\";\n"
"  sbnickmode = \"$0\";\n"
"\n"
"  # activity in statusbar\n"
"\n"
"  # ',' separator\n"
"  sb_act_sep = \"%c$*\";\n"
"  # normal text\n"
"  sb_act_text = \"%c$*\";\n"
"  # public message\n"
"  sb_act_msg = \"%W$*\";\n"
"  # hilight\n"
"  sb_act_hilight = \"%M$*\";\n"
"  # hilight with specified color, $0 = color, $1 = text\n"
"  sb_act_hilight_color = \"$0$1-%n\";\n"
"};\n"
;
