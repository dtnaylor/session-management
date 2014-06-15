use ExtUtils::MakeMaker;

WriteMakefile('NAME' => 'Irssi::UI',
              'LIBS' => '',
	      'OBJECT' => '$(O_FILES)',
              'TYPEMAPS' => ['../common/typemap'],
              'INC' => '-I../../.. -I../../../src -I../../../src/core -I../../../src/fe-common/core -pthread -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include  ',
	      'VERSION_FROM' => './UI.pm');
