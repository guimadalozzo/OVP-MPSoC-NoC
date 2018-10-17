#ifndef DMACMODEL_H
#define DMACMODEL_H
#include "peripheral/impTypes.h"
#include "peripheral/bhm.h"
#include "peripheral/ppm.h"
#include "../defines.h"

//int read_data_old[NPORT];
int routerAddress;
int x_router;
int y_router;
int cont_arbiter[NPORT];
int cont_receive[NPORT];
int pkt_size_arbiter[NPORT];
int pkt_size_receive[NPORT];
int x_dest[NPORT];
int y_dest[NPORT];
int prox[NPORT];
int first_read[NPORT];
int flits_counter[NPORT];
int finish_arbiter[NPORT];
//int pktSize[NPORT];
//int hops;
//int totalVolume;

struct node {
  int x;
  struct node *next;
};

struct node *buffer[NPORT];
struct node *first[NPORT];
struct node *clean[NPORT];

struct node *buffer_send[NPORT];
struct node *first_send[NPORT];
struct node *clean_send[NPORT];

///////////////////////////////// Port handles /////////////////////////////////

typedef struct handlesS {
	ppmNetHandle router_address;
	ppmNetHandle end_sim;
	//ppmNetHandle total_volume;
	ppmNetHandle read_data[NPORT];		
	ppmNetHandle data_read[NPORT];			
	ppmNetHandle send_data[NPORT];			
	ppmNetHandle data_write[NPORT];			
} handlesT, *handlesTP;

extern handlesT handles;

//! Byte swap unsigned int
Uns32 swap_Uns32(Uns32 val);
////////////////////////////// Callback prototypes /////////////////////////////

PPM_CONSTRUCTOR_CB(periphConstructor);
PPM_DESTRUCTOR_CB(periphDestructor);
PPM_CONSTRUCTOR_CB(constructor);
PPM_DESTRUCTOR_CB(destructor);
//static void ReadNI(void *user);
void userInit(void);
void updateRouterAddress(ppmNetValue new, void *userdata);
void updateNetValues(ppmNetValue new, void *userdata);

#endif
