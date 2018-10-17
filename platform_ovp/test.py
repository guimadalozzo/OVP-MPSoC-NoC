#!/usr/bin/env python

import os
import re

results = []

# for folder in '/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal':
#     for f in os.listdir(folder):
#         if re.search('.c', f):
#             results += [f]
#results += [each
for each in os.listdir('/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal'):
	if each.endswith('.c'):
		print each[0:len(each)-2] + ' no proc: ' + str(1<<8)

#print results