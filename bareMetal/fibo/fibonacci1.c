#include <stdio.h>
#include <stdlib.h>
#include "MPIe.h"
#include "tasks_comm.h"

#define ITERATIONS  15

static int fib(int i) {
    return (i>1) ? fib(i-1) + fib(i-2) : i;
}

int main(int argc, char **argv) {
    MemoryWrite(INITIALIZE_ROUTER, 1);

    Message msg;
    msg.length = 1;
    int i=0;
    Message recebeu;
    printf("1 starting...\n");

    while(i<ITERATIONS) {
        recebeu = Receive();
        if(recebeu.msg[0]==ITERATIONS) break;
        i=recebeu.msg[0];
        printf("fib1(%d)=%d \n", i, fib(i));
        msg.msg[0] = i+1;
        Send(msg, fibonacci2);
    }
    msg.msg[0] = ITERATIONS;
    Send(msg, fibonacci2);

    printf("1 finishing ...\n");
    MemoryWrite(END_SIM, 1);
    return 0;
}
