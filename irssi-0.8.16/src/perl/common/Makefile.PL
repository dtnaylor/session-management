use ExtUtils::MakeMaker;

WriteMakefile('NAME' => 'Irssi',
              'LIBS' => '',
	      'OBJECT' => '$(O_FILES)',
              'INC' => '-I../../.. -I../../.. -I../../../src -I../../../src/core -pthread -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include  ',
	      'VERSION_FROM' => './Irssi.pm');
