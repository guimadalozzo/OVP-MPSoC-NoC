#include <stdio.h>
#include <stdlib.h>
#include "MPIe.h"
#include "tasks_comm.h"

#define ITERATIONS  15

static int fib(int i) {
    return (i>1) ? fib(i-1) + fib(i-2) : i;
}

int main(int argc, char **argv) {
    MemoryWrite(INITIALIZE_ROUTER, 3);

    Message msg;
    msg.length = 1;
    int i=0;
    int* recebeu;
    printf("3 starting...\n");

    while(i<ITERATIONS) {
        recebeu = Receive();
        if(recebeu[0]==ITERATIONS) break;
        i=recebeu[0];
        printf("fib3(%d)=%d \n", i, fib(i));
        msg.msg[0] = i+1;
        Send(fibonacci0, msg);
    }

    printf("3 finishing ...\n");
    MemoryWrite(END_SIM, 3);
    return 0;
}
