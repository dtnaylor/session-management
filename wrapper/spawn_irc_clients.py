#!/usr/bin/python

from subprocess import Popen

dn = open('/dev/null', 'w')
for i in xrange(3):
    Popen('irssi', stdout=dn)
while(1):
    pass

