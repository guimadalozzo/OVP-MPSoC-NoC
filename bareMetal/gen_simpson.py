#!/usr/bin/env python2

######################################
# authors Thiago M. Lourenco
# date 14 November 2018
#
# Simpson's generation
######################################

import sys
import fileinput
import os
from shutil import copyfile

def createIntegrator():
	integratorFile.write("#include <stdio.h>\n")
	integratorFile.write("#include <stdlib.h>\n")
	integratorFile.write("#include <math.h>\n")
	integratorFile.write("#include \"MPIe.h\"\n")
	integratorFile.write("#include \"tasks_comm.h\"\n")
	integratorFile.write("\n#define THIS_ID "+str(num_tasks-1)+"\n")
	integratorFile.write("#define NUM_CPU "+str(num_tasks-1)+"\n")
	integratorFile.write("\n#define FIRST_SIMPSON msg.msg[0]\n")
	integratorFile.write("#define LAST_SIMPSON  msg.msg[1]\n")
	integratorFile.write("\n#define LL "+str(ll)+"\n")
	integratorFile.write("#define UL "+str(ul)+"\n")
	integratorFile.write("\nconst int simpsons[] = {")
	for i in range(num_tasks-2):
		integratorFile.write("simpson_"+str(i)+", ")
	integratorFile.write("simpson_"+str(i+1)+" };\n")
	integratorFile.write("\nint main(int argc, char **argv)\n")
	integratorFile.write("{\n")
	integratorFile.write("\tMemoryWrite(INITIALIZE_ROUTER, THIS_ID);\n")
	integratorFile.write("\tprintf(\"\\nIntegrator started\\n\\n\");\n")
	integratorFile.write("\n\tMessage msg;\n")
	integratorFile.write("\tmsg.length = 2;\n")
	integratorFile.write("\n\tfloat lol = LL;\n")
	integratorFile.write("\tfloat uol = UL;\n")
	integratorFile.write("\tfloat nn = NUM_CPU;\n")
	integratorFile.write("\n\tfloat h = (uol - lol) / nn;\n")
	integratorFile.write("\n\tprintf(\"\\nH VALUE : %f\\n\\n\", h);\n\n")
	integratorFile.write("\tuol = lol + h;\n\n")
	integratorFile.write("\tFIRST_SIMPSON = lol;\n")
	integratorFile.write("\tLAST_SIMPSON  = uol;\n")
	integratorFile.write("\tSend(msg, simpsons[0]);\n")
	integratorFile.write("\n\tfor(int i = 1; i < NUM_CPU; i++){\n")
	integratorFile.write("\t\tlol = uol;\n")
	integratorFile.write("\t\tuol += h;\n")
	integratorFile.write("\t\tFIRST_SIMPSON = lol;\n")
	integratorFile.write("\t\tLAST_SIMPSON  = uol;\n")
	integratorFile.write("\t\tSend(msg, simpsons[i]);\n")
	integratorFile.write("\t}\n")
	integratorFile.write("\n\tfloat sum = 0;\n")
	integratorFile.write("\n\tfor(int i = 0; i < NUM_CPU; i++){\n")
	integratorFile.write("\t\tmsg = Receive();\n")
	integratorFile.write("\t\tsum += *msg.msg;\n")
	integratorFile.write("\t\tprintf(\"\\nCurrent sum: %.2f\\n\", sum);\n")
	integratorFile.write("\t}\n")
	integratorFile.write("\n\tprintf(\"\\nFinal integration: %.2f\\n\\n\", sum);\n")
	integratorFile.write("\n\tMemoryWrite(END_SIM, THIS_ID);\n")
	integratorFile.write("\n}\n")

def createSimpsons(this_id):
	simpsonsFile.write("#include <stdio.h>\n")
	simpsonsFile.write("#include <stdlib.h>\n")
	simpsonsFile.write("#include <math.h>\n")
	simpsonsFile.write("#include \"MPIe.h\"\n")
	simpsonsFile.write("#include \"tasks_comm.h\"\n")
	simpsonsFile.write("\n#define THIS_ID "+str(this_id)+"\n")
	simpsonsFile.write("#define NUM_CPU "+str(num_tasks-1)+"\n")
	simpsonsFile.write("\n#define LL msg.msg[0]\n")
	simpsonsFile.write("#define UL msg.msg[1]\n")
	simpsonsFile.write("\nfloat x[NUM_CPU], fx[NUM_CPU];\n")
	simpsonsFile.write("\nfloat f(float x){\n")
	simpsonsFile.write("\treturn(log(x));\n")
	simpsonsFile.write("}\n")		
	simpsonsFile.write("\nint main(int argc, char **argv){\n")
	simpsonsFile.write("\tMemoryWrite(INITIALIZE_ROUTER, THIS_ID);\n")
	simpsonsFile.write("\n\tprintf(\"Simpson rule calculator %d started\\n\", THIS_ID);\n")
	simpsonsFile.write("\n\tMessage msg;\n")
	simpsonsFile.write("\n\tmsg = Receive();\n")
	simpsonsFile.write("\n\tfloat lol = LL;")
	simpsonsFile.write("\n\tfloat uol = UL;")
	simpsonsFile.write("\n\tfloat nn  = NUM_CPU;\n")
	simpsonsFile.write("\n\tprintf(\"Start of %d at %u\\n\", THIS_ID, LL);\n")
	simpsonsFile.write("\tprintf(\"End of %d at %u\\n\", THIS_ID, UL);\n")
	simpsonsFile.write("\n\tfor(int i = 0; i <= NUM_CPU; i++){\n")
	simpsonsFile.write("\t\tx[i] = lol + i*((uol-lol)/nn);\n")
	simpsonsFile.write("\t\tfx[i] = f(x[i]);\n")
	simpsonsFile.write("\t}\n")
	simpsonsFile.write("\n\tfloat res = 0;\n")
	simpsonsFile.write("\tfor (int i = 0; i <= NUM_CPU; i++) {\n")
	simpsonsFile.write("\t\tif (i == 0 || i == NUM_CPU)\n")
	simpsonsFile.write("\t\t\tres += fx[i];\n")
	simpsonsFile.write("\t\telse if (i % 2 != 0)\n")
	simpsonsFile.write("\t\t\tres += 4 * fx[i];\n")
	simpsonsFile.write("\t\telse\n")
	simpsonsFile.write("\t\t\tres += 2 * fx[i];\n")
	simpsonsFile.write("\t}\n")
	simpsonsFile.write("\tres *= (((uol - lol) / nn) / 3);\n")
	simpsonsFile.write("\tmsg.length = 1;\n")
	simpsonsFile.write("\n\t*msg.msg = res;\n")
	simpsonsFile.write("\n\tSend(msg, integrator);\n")
	simpsonsFile.write("\n\tprintf(\"Rule %d end\\n\\n\", THIS_ID);\n")
	simpsonsFile.write("\n\tMemoryWrite(END_SIM, THIS_ID);\n")
	simpsonsFile.write("}\n")

try:
	num_tasks = int(sys.argv[1])*int(sys.argv[2])
	ll = int(sys.argv[3])
	ul = int(sys.argv[4])
	foldername = "simpson"+str(sys.argv[1])+"x"+str(sys.argv[2])
	if not os.path.exists(foldername):
		os.mkdir(foldername)
	copyfile("Makefile", foldername+"/Makefile")
	copyfile("MPIe.h", foldername+"/MPIe.h")
	integratorFile = open(foldername+"/integrator.c", "w")
	createIntegrator();
	integratorFile.close()
	for IdTask in range(num_tasks-1):
		simpsonsFile = open(foldername+"/simpson_"+str(IdTask)+".c", "w")
		createSimpsons(IdTask)
		simpsonsFile.close()

except Exception, e:
	print "\n####################\n\tExecute: \n\t$ gen_simpson.py <X> <Y>\n####################\n"
	raise
else:
	print "\n\t### Generation successful ###\n"
	pass
finally:
	pass