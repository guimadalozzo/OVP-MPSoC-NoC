#ifndef __API_COMM_H__
#define __API_COMM_H__

#define DMA_READY           0xf0000100
#define START_READ_NI       0xf0000180
#define READ_NI_READY       0xf0000190
#define DATA_IN             0xf0000200
#define WRITE_NI            0xf0000210
#define INITIALIZE_ROUTER   0xf0000230
#define END_SIM             0xf0000050

/* Memory Access */
#define MemoryRead(A)   (*(volatile unsigned int*)(A))
#define MemoryWrite(A,V) *(volatile unsigned int*)(A)=(V)
#define MSG_SIZE 128

typedef struct {
  int length;
  int msg[MSG_SIZE];
} Message;

Message Receive() {
  int size=0, i=0;

  MemoryWrite(START_READ_NI, 1);
  while(MemoryRead(READ_NI_READY));
  MemoryRead(DATA_IN);

  MemoryWrite(START_READ_NI, 1);
  while(MemoryRead(READ_NI_READY));
  size = MemoryRead(DATA_IN); //receive value size array

  Message msg;
  msg.length = size;
  for(i=0; i<size; i++) {
    MemoryWrite(START_READ_NI, 1);
    while(MemoryRead(READ_NI_READY));

    /* Receive VALUES */
    msg.msg[i] = MemoryRead(DATA_IN);
  }

	return msg;
}

void Send(Message msg, unsigned int target) {
  if(MemoryRead(DMA_READY))
    while(MemoryRead(DMA_READY));

  MemoryWrite(WRITE_NI, target);      //target
  MemoryWrite(WRITE_NI, msg.length);  //value size array

  int i;
  /* Send VALUES */
  for(i=0; i<msg.length; i++) {
    MemoryWrite(WRITE_NI, msg.msg[i]);
  }
}

#endif /*__API_COMM_H__*/
