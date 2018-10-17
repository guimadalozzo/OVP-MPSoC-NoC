#include <stdio.h>
#include <stdlib.h>
#include "MPIe.h"
#include "tasks_comm.h"

#define ITERATIONS  28

static int fib(int i) {
    return (i>1) ? fib(i-1) + fib(i-2) : i;
}

int main(int argc, char **argv) {
    MemoryWrite(INITIALIZE_ROUTER, 2);

    Message msg;
    msg.length = 1;
    int i=0;
    int* recebeu;
    printf("2 starting...\n");

    while(i<ITERATIONS) {
        recebeu = Receive();
        if(recebeu[0]==ITERATIONS) break;
        i=recebeu[0];
        printf("fib2(%d)=%d \n", i, fib(i));
        msg.msg[0] = i+1;
        Send(fibonacci3, msg);
    }
    msg.msg[0] = ITERATIONS;
    Send(fibonacci3, msg);
    printf("2 finishing ...\n");
    MemoryWrite(END_SIM, 2);
    return 0;
}
