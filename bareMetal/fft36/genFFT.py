#!/usr/bin/env python

######################################
# authors G.Madalozzo
# date 26 May 2014 16:38:17
#
# FFT's generation
######################################

import sys
import fileinput
import os

nomeFFT = 'fft'
nomeFront = 'front'

def createFront():
	frontFile.write("#include <stdlib.h>\n")
	frontFile.write("#include <stdio.h>\n")
	frontFile.write('#include "MPIe.h"\n')
	frontFile.write('#include "tasks_comm.h"\n')
	frontFile.write('\nMessage msg;\n')

	frontFile.write('\nint main() {\n')
	frontFile.write('\n\tMemoryWrite(INITIALIZE_ROUTER, '+str(router)+');\n')
	frontFile.write('\n\tint real[128], img[128];\n')
	frontFile.write('\n\tint i, f, k;\n')

	frontFile.write('\n\n\tint NUM_FRAMES = 10;\n')

	frontFile.write('\n\tfor(f=0; f<NUM_FRAMES;f++) {\n')
	frontFile.write('\n\t\tfor(i=0; i<128; i++) {\n')
	frontFile.write('\n\t\t\treal[i] = 12;//rand(2, 4, 320);\n')
	frontFile.write('\n\t\t\timg[i] = 0;\n')
	frontFile.write('\n\t\t}\n')

	frontFile.write('\n\t\tmsg.length = 1;\n')
	frontFile.write('\n\t\tmsg.msg[0] = 1;\n')
	frontFile.write('\n\t\tSend(msg, fft_'+str(app)+');\n')

	frontFile.write('\n\t\tmsg.length = 1;\n')
	frontFile.write('\n\t\tmsg.msg[0] = 7;\n')
	frontFile.write('\n\t\tSend(msg, fft_'+str(app)+');\n')

	frontFile.write('\n\t\tmsg.length = 128;\n')
	frontFile.write('\n\t\tfor (k=0; k<128; k++)\n')
	frontFile.write('\n\t\t\tmsg.msg[k] = real[k];\n')
	frontFile.write('\n\t\tSend(msg, fft_'+str(app)+');\n')

	frontFile.write('\n\t\tmsg.length = 128;\n')
	frontFile.write('\n\t\tfor (k=0; k<128; k++)\n')
	frontFile.write('\n\t\t\tmsg.msg[k] = img[k];\n')
	frontFile.write('\n\t\tSend(msg, fft_'+str(app)+');\n')

	frontFile.write('\n\t\tprintf("\\nFront enviou frame %d", f);\n')

	frontFile.write('\n\t\tmsg = Receive();\n')
	frontFile.write('\n\t\tfor (k=0; k<128; k++)\n')
	frontFile.write('\n\t\t\treal[k] = msg.msg[k];\n')
	frontFile.write('\n\t\tmsg = Receive();\n')
	frontFile.write('\n\t\tfor (k=0; k<128; k++)\n')
	frontFile.write('\n\t\t\timg[k] = msg.msg[k];\n')

	frontFile.write('\n\t\tprintf("\\nFront recebeu o resultado da fft_'+str(app)+'");\n')
	frontFile.write('\n\t\t}\n')

	frontFile.write('\n\t\tmsg.length = 1;\n')
	frontFile.write('\n\t\tmsg.msg[0] = 0;\n')
	frontFile.write('\n\t\tSend(msg, fft_'+str(app)+');\n')

	frontFile.write('\n\t\tMemoryWrite(END_SIM, '+str(router)+');\n')
	frontFile.write('\n\t\treturn 0;\n')
	frontFile.write('\n}\n')

def createFFT():
	fftFile.write("#include <stdlib.h>\n")
	fftFile.write("#include <stdio.h>\n")
	fftFile.write('#include "MPIe.h"\n')
	fftFile.write('#include "tasks_comm.h"\n')

	fftFile.write('Message msg;\n')

	fftFile.write('#define MINPI	314\n')
	fftFile.write('\n')
	fftFile.write('unsigned int sin_(unsigned int x){\n')
	fftFile.write('	unsigned int sin;\n')
	fftFile.write('\n')
	fftFile.write('	//always wrap input angle to -PI..PI\n')
	fftFile.write('	if (x < -314159265)\n')
	fftFile.write('	    x += 628318531;\n')
	fftFile.write('	else\n')
	fftFile.write('	if (x >  314159265)\n')
	fftFile.write('	    x -= 628318531;\n')
	fftFile.write('\n')
	fftFile.write('	//compute sine\n')
	fftFile.write('	if (x < 0)\n')
	fftFile.write('	    sin = 127323954 * x + 405284735 * x * x;\n')
	fftFile.write('	else\n')
	fftFile.write('	    sin = 127323954 * x - 405284735 * x * x;\n')
	fftFile.write('\n')
	fftFile.write('	return sin;\n')
	fftFile.write('}\n')
	fftFile.write('\n')
	fftFile.write('unsigned int cos_(unsigned int x){\n')
	fftFile.write('	unsigned int cos;\n')
	fftFile.write('\n')
	fftFile.write('	//always wrap input angle to -PI..PI\n')
	fftFile.write('	if (x < -314159265)\n')
	fftFile.write('		x += 628318531;\n')
	fftFile.write('	else\n')
	fftFile.write('	if (x >  314159265)\n')
	fftFile.write('		x -= 628318531;\n')
	fftFile.write('\n')
	fftFile.write('	//compute cosine: sin(x + PI/2) = cos(x)\n')
	fftFile.write('	x += 157079632;\n')
	fftFile.write('	if (x >  314159265)\n')
	fftFile.write('	    x -= 628318531;\n')
	fftFile.write('\n')
	fftFile.write('	if (x < 0)\n')
	fftFile.write('	    cos = 127323954 * x + 405284735 * x * x;\n')
	fftFile.write('	else\n')
	fftFile.write('	    cos = 127323954 * x - 405284735 * x * x;\n')
	fftFile.write('\n')
	fftFile.write('	return cos;\n')
	fftFile.write('}\n')
	fftFile.write('\n')
	fftFile.write('void FFT(unsigned int logN, unsigned int *real, unsigned int *im) // logN is base 2 log(N)\n')
	fftFile.write('{\n')
	fftFile.write('    unsigned int n=0, nspan, span, submatrix, node;\n')
	fftFile.write('    unsigned int N = 1<<logN;\n')
	fftFile.write('    unsigned int temp, primitive_root, angle, realtwiddle, imtwiddle;\n')
	fftFile.write('\n')
	fftFile.write('    for(span=N>>1; span; span>>=1)      // loop over the FFT stages\n')
	fftFile.write('    {\n')
	fftFile.write('       primitive_root = MINPI/span;     // define MINPI in the header\n')
	fftFile.write('\n')
	fftFile.write('       for(submatrix=0; submatrix<(N>>1)/span; submatrix++)\n')
	fftFile.write('       {\n')
	fftFile.write('          for(node=0; node<span; node++)\n')
	fftFile.write('          {\n')
	fftFile.write('            nspan = n+span;\n')
	fftFile.write('            temp = real[n] + real[nspan];       // additions & subtractions\n')
	fftFile.write('            real[nspan] = real[n]-real[nspan];\n')
	fftFile.write('            real[n] = temp;\n')
	fftFile.write('            temp = im[n] + im[nspan];\n')
	fftFile.write('            im[nspan] = im[n] - im[nspan];\n')
	fftFile.write('            im[n] = temp;\n')
	fftFile.write('\n')
	fftFile.write('            angle = primitive_root * node;      // rotations\n')
	fftFile.write('            realtwiddle = cos_(angle);\n')
	fftFile.write('            imtwiddle = sin_(angle);\n')
	fftFile.write('            temp = realtwiddle * real[nspan] - imtwiddle * im[nspan];\n')
	fftFile.write('            im[nspan] = realtwiddle * im[nspan] + imtwiddle * real[nspan];\n')
	fftFile.write('            real[nspan] = temp;\n')
	fftFile.write('\n')
	fftFile.write('            n++;   // not forget to increment n\n')
	fftFile.write('\n')
	fftFile.write('          } // end of loop over nodes\n')
	fftFile.write('\n')
	fftFile.write('          n = (n+span) & (N-1);   // jump over the odd blocks\n')
	fftFile.write('\n')
	fftFile.write('        } // end of loop over submatrices\n')
	fftFile.write('\n')
	fftFile.write('     } // end of loop over FFT stages\n')
	fftFile.write('} // end of FFT function\n')
	fftFile.write('\n')
	fftFile.write('int main() {\n')
	fftFile.write('	MemoryWrite(INITIALIZE_ROUTER, '+str(router)+');\n')
	fftFile.write('	unsigned int logN;\n')
	fftFile.write('	unsigned int real[128], im[128];\n')
	fftFile.write('	int i = 0;\n')
	fftFile.write('	int k;\n')
	fftFile.write('\n')
	fftFile.write('	while(1) {\n')
	fftFile.write('\n')
	fftFile.write('		msg = Receive();\n')
	fftFile.write('		if (msg.msg[0] == 0)\n')
	fftFile.write('			break;\n')
	fftFile.write('\n')
	fftFile.write('		printf("Inicio do processamento do frame_'+str(app)+': %d", i++);\n')
	fftFile.write('\n')
	fftFile.write('		msg = Receive();\n')
	fftFile.write('		logN = msg.msg[0];\n')
	fftFile.write('\n')
	fftFile.write('		printf("Recebeu logN_'+str(app)+' %d", logN);\n')
	fftFile.write('\n')
	fftFile.write('		//real part\n')
	fftFile.write('		msg = Receive();\n')
	fftFile.write('		for (k=0; k<128; k++)\n')
	fftFile.write('			real[k] = msg.msg[k];\n')
	fftFile.write('\n')
	fftFile.write('		printf("Recebeu real_'+str(app)+'");\n')
	fftFile.write('\n')
	fftFile.write('		//img part\n')
	fftFile.write('		msg = Receive();\n')
	fftFile.write('		for (k=0; k<128; k++)\n')
	fftFile.write('			im[k] = msg.msg[k];\n')
	fftFile.write('\n')
	fftFile.write('		printf("Recebeu imag_'+str(app)+'");\n')
	fftFile.write('		printf("Inicio da FFT_'+str(app)+'");\n')
	fftFile.write('\n')
	fftFile.write('		FFT(logN, real, im);\n')
	fftFile.write('\n')
	fftFile.write('		printf("Fim da FFT_'+str(app)+'");\n')
	fftFile.write('\n')
	fftFile.write('		//Assuming that msg size is seted\n')
	fftFile.write('		for (k=0; k<128; k++)\n')
	fftFile.write('			msg.msg[k] = real[k];\n')
	fftFile.write('		Send(msg, front_'+str(app)+');\n')
	fftFile.write('\n')
	fftFile.write('		for (k=0; k<128; k++)\n')
	fftFile.write('			msg.msg[k] = im[k];\n')
	fftFile.write('		Send(msg, front_'+str(app)+');\n')
	fftFile.write('\n')
	fftFile.write('		printf("Envio completo do Frame_'+str(app)+'");\n')
	fftFile.write('	}\n')
	fftFile.write('\n')
	fftFile.write('	printf("Fim da Aplicacao FFT_'+str(app)+'");\n')
	fftFile.write('	MemoryWrite(END_SIM, '+str(router)+');\n')
	fftFile.write('\n')
	fftFile.write('	return 0;\n')
	fftFile.write('}\n')

try:
	qtdApp = 18
	app = 0
	router = 0
	for x in range(qtdApp):
		frontFile = open(nomeFront+"_"+str(app)+".c", "w")
		createFront()
		frontFile.close()
		router += 1

		fftFile = open(nomeFFT+"_"+str(app)+".c", "w")
		createFFT()
		fftFile.close()
		app += 1

except Exception, e:
	print "\n####################\n\tExecute: \n\t$ gen_dijkstra.py <NUMBER_OF_TASKS>\n####################\n"
	raise
else:
	print "\n\t### Generation successful ###\n"
	pass
finally:
	pass

