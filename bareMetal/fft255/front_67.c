#include <stdlib.h>
#include <stdio.h>
#include "MPIe.h"
#include "tasks_comm.h"

Message msg;

int main() {

	MemoryWrite(INITIALIZE_ROUTER, 134);

	int real[128], img[128];

	int i, f, k;


	int NUM_FRAMES = 10;

	for(f=0; f<NUM_FRAMES;f++) {

		for(i=0; i<128; i++) {

			real[i] = 12;//rand(2, 4, 320);

			img[i] = 0;

		}

		msg.length = 1;

		msg.msg[0] = 1;

		Send(msg, fft_67);

		msg.length = 1;

		msg.msg[0] = 7;

		Send(msg, fft_67);

		msg.length = 128;

		for (k=0; k<128; k++)

			msg.msg[k] = real[k];

		Send(msg, fft_67);

		msg.length = 128;

		for (k=0; k<128; k++)

			msg.msg[k] = img[k];

		Send(msg, fft_67);

		printf("\nFront enviou frame %d", f);

		msg = Receive();

		for (k=0; k<128; k++)

			real[k] = msg.msg[k];

		msg = Receive();

		for (k=0; k<128; k++)

			img[k] = msg.msg[k];

		printf("\nFront recebeu o resultado da fft_67");

		}

		msg.length = 1;

		msg.msg[0] = 0;

		Send(msg, fft_67);

		MemoryWrite(END_SIM, 134);

		return 0;

}
