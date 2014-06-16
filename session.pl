use strict;
use vars qw($VERSION %IRSSI);

use Irssi qw(command_bind signal_add);
use IO::File;
$VERSION = '0.20';
%IRSSI = (
	authors		=> 'Patrik Jansson',
	contact		=> 'gein@knivby.nu',
	name		=> '8-ball',
	description	=> 'Dont like to take decisions? Have the 8-ball do it for you instead.',
	license		=> 'GPL',
);

sub own_question {
	my ($server, $msg, $target) = @_;
	question($server, $msg, "", $target);
}

sub public_question {
	my ($server, $msg, $nick, $address, $target) = @_;
	question($server, $msg, $nick.": ", $target);
}
sub question($server, $msg, $nick, $target) {
	my ($server, $msg, $nick, $target) = @_;
	$_ = $msg;
	if (/^8-ball:.+\?$/i) {
		my $answer = "Merb";
        $a = 1;
        while( $a < 500 ) {
            $server->command('msg '.$target.' '.$nick.'8-ball: '.$a.$answer);
            $a = $a + 1;
        }
		return 0;
	} else {
        return 0;
	}
}

signal_add("message public", "public_question");
signal_add("message own_public", "own_question");
