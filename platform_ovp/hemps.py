#!/usr/bin/env python2

######################################
# author G.Madalozzo
# date 9 Dec 2013 16:28:43
#
# boot's generation
######################################

import sys
import fileinput
import os
import subprocess
from subprocess import call

try:
	print '\033[93m\n\nHeMPS SysOV: NoC-based MPSoCs with SystemC, OVP and VHDL\033[0m\n\n\n'

	call("echo 'compiling bareMetal';", shell=True)
	call("cd ../bareMetal;make clean;make all;", shell=True)
	call("echo;echo;echo 'compiling platform';", shell=True)
	call("make clean;make all;", shell=True)
	call("sleep 5s;./platform.Linux32.exe", shell=True)

except Exception, e:
	print "\n\tERRO\n"
	raise
finally:
	pass
