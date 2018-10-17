#include <stdio.h>
#include <stdlib.h>
#include "MPIe.h"
#include "tasks_comm.h"

#define ITERATIONS  28

static int fib(int i) {
    return (i>1) ? fib(i-1) + fib(i-2) : i;
}

int main(int argc, char **argv) {
    MemoryWrite(INITIALIZE_ROUTER, 0);

    Message msg;
    msg.length = 1;
    int i=0;
    int *recebeu;
    printf("0 starting...\n");

    while(i<ITERATIONS) {
        printf("fib0(%d)=%d \n", i, fib(i));
        msg.msg[0] = i+1;
        Send(fibonacci1, msg);
        recebeu = Receive();
        i=recebeu[0];
    }
    msg.msg[0] = ITERATIONS;
    Send(fibonacci1, msg);

    printf("0 finishing ...\n");
    MemoryWrite(END_SIM, 0);
    return 0;
}
