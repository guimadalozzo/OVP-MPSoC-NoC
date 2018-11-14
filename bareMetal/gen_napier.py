#!/usr/bin/env python

######################################
# authors Angelo Elias Dalzotto
# date 07 November 2018
#
# Napier's generation
######################################

import sys
import fileinput
import os
from shutil import copyfile


# >33 causes underflow
NUM_SERIES = 33;

def createAccumulator():
	accumulatorFile.write("#include <stdio.h>\n")
	accumulatorFile.write("#include \"MPIe.h\"\n")
	accumulatorFile.write("#include \"tasks_comm.h\"\n")
	accumulatorFile.write("#include <math.h>\n")
	accumulatorFile.write("\n#define THIS_ID "+str(num_tasks-1)+"\n")
	accumulatorFile.write("\n#define FIRST_SERIES msg.msg[0]\n")
	accumulatorFile.write("#define LAST_SERIES msg.msg[1]\n")

	accumulatorFile.write("\n#define NUM_SERIES "+str(NUM_SERIES)+"\n")
	accumulatorFile.write("#define NUM_CPU "+str(num_tasks-1)+"\n")
	accumulatorFile.write("const int series[NUM_CPU] = {")
	for i in range(num_tasks-1):
		accumulatorFile.write("series_"+str(i)+", ")
	accumulatorFile.write("};\n")

	accumulatorFile.write("\nint main(int argc, char **argv)\n")
	accumulatorFile.write("{\n")
	accumulatorFile.write("\tMemoryWrite(INITIALIZE_ROUTER, THIS_ID);\n")
	accumulatorFile.write("\tprintf(\"Accumulator started\\n\");\n")
	
	accumulatorFile.write("\n\tMessage msg;\n")
	accumulatorFile.write("\tmsg.length = 2;\n")

	accumulatorFile.write("\n\tdouble sum = 0;\n")

	accumulatorFile.write("\n\tLAST_SERIES = 0;\n")

	accumulatorFile.write("\n\tfor(int i = 0; i < NUM_CPU; i++){\n")
	accumulatorFile.write("\t\tFIRST_SERIES = LAST_SERIES;\n")
	accumulatorFile.write("\t\tLAST_SERIES += ceil((double)NUM_SERIES / NUM_CPU);\n")
	accumulatorFile.write("\t\t//if(i == NUM_CPU) LAST_SERIES = NUM_SERIES;\n")
	accumulatorFile.write("\t\tSend(msg, series[i]);\n")
	accumulatorFile.write("\t}\n")

	accumulatorFile.write("\n\tprintf(\"Current sum: %.50lf\\n\", sum);\n")

	accumulatorFile.write("\n\tfor(int i = 0; i < NUM_CPU; i++){\n")
	accumulatorFile.write("\t\tmsg = Receive();\n")
	accumulatorFile.write("\t\tsum += *(double*)msg.msg;\n")
	accumulatorFile.write("\t\tprintf(\"Current sum: %.50lf\\n\", sum);\n")
	accumulatorFile.write("\t}\n")

	accumulatorFile.write("\n\tprintf(\"e value: %.50lf\\n\", sum);\n")

	accumulatorFile.write("\n\tMemoryWrite(END_SIM, THIS_ID);\n")

	accumulatorFile.write("\n}\n")

def createSeries(this_id):
	seriesFile.write("#include <stdio.h>\n")
	seriesFile.write("#include \"MPIe.h\"\n")
	seriesFile.write("#include \"tasks_comm.h\"\n")

	seriesFile.write("\n#define THIS_ID "+str(this_id)+"\n")

	seriesFile.write("\n#define FIRST_SERIES msg.msg[0]\n")
	seriesFile.write("#define LAST_SERIES  msg.msg[1]\n")

	seriesFile.write("\nunsigned int factorial(unsigned int n)\n")
	seriesFile.write("{\n")
	seriesFile.write("\tunsigned int retval = 1;\n")
	seriesFile.write("\n\tfor(int i = n; i > 1; --i)\n")
	seriesFile.write("\t\tretval *= i;\n")
	seriesFile.write("\n\treturn retval;\n")
	seriesFile.write("}\n")

	seriesFile.write("\nint main(int argc, char **argv)\n")
	seriesFile.write("{\n")
	seriesFile.write("\tMemoryWrite(INITIALIZE_ROUTER, THIS_ID);\n")

	seriesFile.write("\n\tprintf(\"Series calculator %d started\\n\", THIS_ID);\n")

	seriesFile.write("\n\tMessage msg;\n")
	seriesFile.write("\tdouble sum = 0;\n")

	seriesFile.write("\n\tmsg = Receive();\n")

	seriesFile.write("\n\tprintf(\"S%d start series: %u\\n\", THIS_ID, FIRST_SERIES);\n")
	seriesFile.write("\n\tprintf(\"S%d last series: %u\\n\", THIS_ID, LAST_SERIES);\n")

	seriesFile.write("\n\tfor(unsigned int i = FIRST_SERIES; i < LAST_SERIES; i++){\n")
	seriesFile.write("\t\tif(i > 33){\n")
	seriesFile.write("\t\t\tprintf(\"S%d avoiding underflow!\\n\", THIS_ID);\n")
	seriesFile.write("\t\t\tbreak;")
	seriesFile.write("\t\t}\n")
	seriesFile.write("\t\tsum += 1.0 / factorial(i);\n")
	seriesFile.write("\t\tprintf(\"S%d value: %.50lf\\n\", THIS_ID, sum);\n")
	seriesFile.write("\t}\n")

	seriesFile.write("\n\t*(double*)msg.msg = sum;\n")

	seriesFile.write("\n\tSend(msg, accumulator);\n")

	seriesFile.write("\n\tprintf(\"Series %d end\\n\", THIS_ID);\n")

	seriesFile.write("\n\tMemoryWrite(END_SIM, THIS_ID);\n")
	seriesFile.write("}\n")

try:
	num_tasks = int(sys.argv[1])*int(sys.argv[2])
	foldername = "napier"+str(sys.argv[1])+"x"+str(sys.argv[2])

	if not os.path.exists(foldername):
		os.mkdir(foldername)

	copyfile("Makefile", foldername+"/Makefile")
	copyfile("MPIe.h", foldername+"/MPIe.h")

	accumulatorFile = open(foldername+"/accumulator.c", "w")
	createAccumulator();
	accumulatorFile.close()

	for IdTask in range(num_tasks-1):
		seriesFile = open(foldername+"/series_"+str(IdTask)+".c", "w")
		createSeries(IdTask)
		seriesFile.close()

except Exception, e:
	print "\n####################\n\tExecute: \n\t$ gen_napier.py <X> <Y>\n####################\n"
	raise
else:
	print "\n\t### Generation successful ###\n"
	pass
finally:
	pass