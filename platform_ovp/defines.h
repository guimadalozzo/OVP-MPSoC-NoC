//define router positions
#define BL 0
#define BC 1
#define BR 2
#define CL 3
#define CC 4
#define CRX 5
#define TL 6
#define TC 7
#define TR 8

#define EAST 0
#define WEST 1
#define NORTH 2
#define SOUTH 3
#define LOCAL 4

#define NPORT 5

#define IRQ_MASK          	0xf0000010
#define IRQ_STATUS        	0xf0000020
#define COUNTER_REG       	0xf0000030
#define REPOSITORY		   	0xf0000040
#define END_SIM 		   	0xf0000050

/* DMA*/
#define DMA_SIZE		  	0xf0000060
#define DMA_ADDRESS		  	0xf0000070
#define DMA_OP			  	0xf0000080
#define DMA_START		  	0xf0000090
#define DMA_READY			0xf0000100

#define NEXT_PAGE			0xf0000110
#define PROC_ADDRESS		0xf0000120
#define TICK_COUNTER	  	0xf0000130
#define PAGE			  	0xf0000140

#define APP_SIZE		  	0xf0000150
#define REQ_APP		  		0xf0000160
#define ACK_APP		  		0xf0000170

#define START_READ_NI 0xf0000180
#define READ_NI_READY 0xf0000190 
#define DATA_IN 0xf0000200 
#define WRITE_NI 0xf0000210 

#define INITIALIZE_ROUTER 0xf0000230
#define SCHEDULER_CALLED 0xf0000240
#define SWAP_READ 0xf0000250
#define START_SYSTEM 0xf0000260

#define CPU_HALT 0xf0000270

//#define REPOSITORY_ADRESS 0xf0000300
//#define REPOSITORY_READ 0xf0000310

/* DMA operations */
#define READ	0
#define WRITE	1

/*********** Interrupt bits **************/
#define IRQ_UART_READ_AVAILABLE  0x01
#define IRQ_UART_WRITE_AVAILABLE 0x02
#define IRQ_COUNTER18_NOT        0x04
#define IRQ_COUNTER18            0x08
#define IRQ_DMA					 0x10
#define IRQ_NOC					 0x20
#define IRQ_SYS_CALL			 0x40
