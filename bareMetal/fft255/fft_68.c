#include <stdlib.h>
#include <stdio.h>
#include "MPIe.h"
#include "tasks_comm.h"
Message msg;
#define MINPI	314

unsigned int sin_(unsigned int x){
	unsigned int sin;

	//always wrap input angle to -PI..PI
	if (x < -314159265)
	    x += 628318531;
	else
	if (x >  314159265)
	    x -= 628318531;

	//compute sine
	if (x < 0)
	    sin = 127323954 * x + 405284735 * x * x;
	else
	    sin = 127323954 * x - 405284735 * x * x;

	return sin;
}

unsigned int cos_(unsigned int x){
	unsigned int cos;

	//always wrap input angle to -PI..PI
	if (x < -314159265)
		x += 628318531;
	else
	if (x >  314159265)
		x -= 628318531;

	//compute cosine: sin(x + PI/2) = cos(x)
	x += 157079632;
	if (x >  314159265)
	    x -= 628318531;

	if (x < 0)
	    cos = 127323954 * x + 405284735 * x * x;
	else
	    cos = 127323954 * x - 405284735 * x * x;

	return cos;
}

void FFT(unsigned int logN, unsigned int *real, unsigned int *im) // logN is base 2 log(N)
{
    unsigned int n=0, nspan, span, submatrix, node;
    unsigned int N = 1<<logN;
    unsigned int temp, primitive_root, angle, realtwiddle, imtwiddle;

    for(span=N>>1; span; span>>=1)      // loop over the FFT stages
    {
       primitive_root = MINPI/span;     // define MINPI in the header

       for(submatrix=0; submatrix<(N>>1)/span; submatrix++)
       {
          for(node=0; node<span; node++)
          {
            nspan = n+span;
            temp = real[n] + real[nspan];       // additions & subtractions
            real[nspan] = real[n]-real[nspan];
            real[n] = temp;
            temp = im[n] + im[nspan];
            im[nspan] = im[n] - im[nspan];
            im[n] = temp;

            angle = primitive_root * node;      // rotations
            realtwiddle = cos_(angle);
            imtwiddle = sin_(angle);
            temp = realtwiddle * real[nspan] - imtwiddle * im[nspan];
            im[nspan] = realtwiddle * im[nspan] + imtwiddle * real[nspan];
            real[nspan] = temp;

            n++;   // not forget to increment n

          } // end of loop over nodes

          n = (n+span) & (N-1);   // jump over the odd blocks

        } // end of loop over submatrices

     } // end of loop over FFT stages
} // end of FFT function

int main() {
	MemoryWrite(INITIALIZE_ROUTER, 137);
	unsigned int logN;
	unsigned int real[128], im[128];
	int i = 0;
	int k;

	while(1) {

		msg = Receive();
		if (msg.msg[0] == 0)
			break;

		printf("Inicio do processamento do frame_68: %d", i++);

		msg = Receive();
		logN = msg.msg[0];

		printf("Recebeu logN_68 %d", logN);

		//real part
		msg = Receive();
		for (k=0; k<128; k++)
			real[k] = msg.msg[k];

		printf("Recebeu real_68");

		//img part
		msg = Receive();
		for (k=0; k<128; k++)
			im[k] = msg.msg[k];

		printf("Recebeu imag_68");
		printf("Inicio da FFT_68");

		FFT(logN, real, im);

		printf("Fim da FFT_68");

		//Assuming that msg size is seted
		for (k=0; k<128; k++)
			msg.msg[k] = real[k];
		Send(msg, front_68);

		for (k=0; k<128; k++)
			msg.msg[k] = im[k];
		Send(msg, front_68);

		printf("Envio completo do Frame_68");
	}

	printf("Fim da Aplicacao FFT_68");
	MemoryWrite(END_SIM, 137);

	return 0;
}
