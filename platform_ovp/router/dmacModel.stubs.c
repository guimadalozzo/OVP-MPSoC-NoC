#include "dmacModel.h"
#include <stdio.h>
#include <stdlib.h>

#define THREAD_STACK      (8*1024)

typedef struct {
    bhmThreadHandle       thread;
    bhmEventHandle        start;
    char                  stack[THREAD_STACK];
} th;

static th thArbiter0, thArbiter1, thArbiter2, thArbiter3, thArbiter4, thClock;

int routing[NPORT], numberPackets[NPORT];
//int finished[NPORT];
//int cont_in[NPORT];
//int cont_out[NPORT];

Uns32 swap_Uns32(Uns32 val)
{
    val = ((val << 8) & 0xFF00FF00 ) | ((val >> 8) & 0xFF00FF );
    return (val << 16) | (val >> 16);
}

PPM_CONSTRUCTOR_CB(constructor) {
    // YOUR CODE HERE (pre constructor)
    periphConstructor();
    userInit();
    // YOUR CODE HERE (post constructor)
}

PPM_DESTRUCTOR_CB(destructor) {
    // YOUR CODE HERE (destructor)
}

void updateRouterAddress(ppmNetValue new, void *userdata)
{
	routerAddress = ppmReadNet(handles.router_address);
	x_router = routerAddress >> 8;
	y_router = routerAddress & 0xFF;
	//printf("routerAddress %d %d %d \n", routerAddress, x_router, y_router);
	//if(routerAddress == 0x10) ppmWriteNet(handles.data_write[1], routerAddress);
}

void end_simulation(ppmNetValue new, void *userdata)
{
	int i;
	char* port = "0";

	//ppmWriteNet(handles.total_volume, totalVolume);
	//printf("Router %d\n", routerAddress);
	for(i=0;i<NPORT;i++)
	{
		if(i==NORTH) port = "NORTH";
		if(i==SOUTH) port = "SOUTH";
		if(i==EAST) port = "EAST";
		if(i==WEST) port = "WEST";
		if(flits_counter[i]!=0 && i!=LOCAL) printf("TRAFFIC IN ROUTER %d%d port %s = %d\n", x_router, y_router, port, flits_counter[i]);
	}
}

void Receive0(ppmNetValue new, void *userdata)
{
//	printf("receive 0 || %d\n\n", routerAddress);
		int data;

		data = ppmReadNet(handles.data_read[0]);
		if(first[0] == NULL)
		{
			first[0] = malloc( sizeof(struct node) );
			first[0]->x = data;
			first[0]->next = NULL;
			buffer[0] = first[0];
		}
		else
		{
			buffer[0]->next = malloc( sizeof(struct node) );
			buffer[0] = buffer[0]->next;
			buffer[0]->x = data;
			buffer[0]->next = NULL;
		}
		cont_receive[0]++;
		if(cont_receive[0]==1)
		{
			x_dest[0] = data >> 8;
			y_dest[0] = data & 0xFF;
			if(x_dest[0]>x_router)
			{
				routing[0] = EAST;
			}
			if(x_dest[0]<x_router)
			{
					routing[0] = WEST;
			}
			if(x_dest[0]==x_router)
			{
				if(y_dest[0]>y_router)
				{
					routing[0] = NORTH;
				}
				if(y_dest[0]<y_router)
				{
					routing[0] = SOUTH;
				}
				if(y_dest[0]==y_router)
				{
					routing[0] = LOCAL;
				}
			}

			if(first_send[0] == NULL)
			{
				first_send[0] = malloc( sizeof(struct node) );
				first_send[0]->x = routing[0];
				first_send[0]->next = NULL;
				buffer_send[0] = first_send[0];
			}
			else
			{
				buffer_send[0]->next = malloc( sizeof(struct node) );
				buffer_send[0] = buffer_send[0]->next;
				buffer_send[0]->x = routing[0];
				buffer_send[0]->next = NULL;
			}
			//printf("\n0-routerAddress %d %d to  %d  %d  ---> port %d\n", x_router, y_router, x_dest[0], y_dest[0], routing[0]);
		}
		if(cont_receive[0]==2)
		{
			pkt_size_receive[0]=data;
			flits_counter[0] = flits_counter[0] + data + 2;
		}
		if(cont_receive[0]>2)
		{
			pkt_size_receive[0]--;
			if(pkt_size_receive[0]==0)
			{
				numberPackets[routing[0]]++;
				cont_receive[0]=0;
			}
		}
}

void Receive1(ppmNetValue new, void *userdata)
{
		int data;
		
		data = ppmReadNet(handles.data_read[1]);
		if(first[1] == NULL) 
		{
			first[1] = malloc( sizeof(struct node) ); 
			first[1]->x = data;
			first[1]->next = NULL;
			buffer[1] = first[1];
		}
		else
		{
			buffer[1]->next = malloc( sizeof(struct node) ); 
			buffer[1] = buffer[1]->next;
			buffer[1]->x = data;
			buffer[1]->next = NULL;
		}
		cont_receive[1]++;
		if(cont_receive[1]==1)
		{
			x_dest[1] = data >> 8;
			y_dest[1] = data & 0xFF;
			if(x_dest[1]>x_router)
			{
				routing[1] = EAST;
			}
			if(x_dest[1]<x_router)
			{
					routing[1] = WEST;
			}
			if(x_dest[1]==x_router)
			{
				if(y_dest[1]>y_router)
				{
					routing[1] = NORTH;
				}
				if(y_dest[1]<y_router)
				{
					routing[1] = SOUTH;
				}
				if(y_dest[1]==y_router)
				{
					routing[1] = LOCAL;
				}
			}
			
			if(first_send[1] == NULL) 
			{
				first_send[1] = malloc( sizeof(struct node) ); 
				first_send[1]->x = routing[1];
				first_send[1]->next = NULL;
				buffer_send[1] = first_send[1];
			}
			else
			{
				buffer_send[1]->next = malloc( sizeof(struct node) ); 
				buffer_send[1] = buffer_send[1]->next;
				buffer_send[1]->x = routing[1];
				buffer_send[1]->next = NULL;
			}
		//	printf("\n1-routerAddress %d %d to  %d  %d  ---> port %d\n", x_router, y_router, x_dest[1], y_dest[1], routing[1]);
		}
		if(cont_receive[1]==2)
		{
			pkt_size_receive[1]=data;
			flits_counter[1] = flits_counter[1] + data + 2;
		}
		if(cont_receive[1]>2)
		{
			pkt_size_receive[1]--;
			if(pkt_size_receive[1]==0)
			{
				numberPackets[routing[1]]++;
				cont_receive[1]=0;
			}
		}
}

void Receive2(ppmNetValue new, void *userdata)
{
		int data;
		
		data = ppmReadNet(handles.data_read[2]);
		if(first[2] == NULL) 
		{
			first[2] = malloc( sizeof(struct node) ); 
			first[2]->x = data;
			first[2]->next = NULL;
			buffer[2] = first[2];
		}
		else
		{
			buffer[2]->next = malloc( sizeof(struct node) ); 
			buffer[2] = buffer[2]->next;
			buffer[2]->x = data;
			buffer[2]->next = NULL;
		}
		cont_receive[2]++;
		if(cont_receive[2]==1)
		{
			x_dest[2] = data >> 8;
			y_dest[2] = data & 0xFF;
			if(x_dest[2]>x_router)
			{
				routing[2] = EAST;
			}
			if(x_dest[2]<x_router)
			{
					routing[2] = WEST;
			}
			if(x_dest[2]==x_router)
			{
				if(y_dest[2]>y_router)
				{
					routing[2] = NORTH;
				}
				if(y_dest[2]<y_router)
				{
					routing[2] = SOUTH;
				}
				if(y_dest[2]==y_router)
				{
					routing[2] = LOCAL;
				}
			}
			
			if(first_send[2] == NULL) 
			{
				first_send[2] = malloc( sizeof(struct node) ); 
				first_send[2]->x = routing[2];
				first_send[2]->next = NULL;
				buffer_send[2] = first_send[2];
			}
			else
			{
				buffer_send[2]->next = malloc( sizeof(struct node) ); 
				buffer_send[2] = buffer_send[2]->next;
				buffer_send[2]->x = routing[2];
				buffer_send[2]->next = NULL;
			}
			//printf("\n2-routerAddress %d %d to  %d  %d  ---> port %d\n", x_router, y_router, x_dest[2], y_dest[2], routing[2]);
		}
		if(cont_receive[2]==2)
		{
			pkt_size_receive[2]=data;
			flits_counter[2] = flits_counter[2] + data + 2;
		}
		if(cont_receive[2]>2)
		{
			pkt_size_receive[2]--;
			if(pkt_size_receive[2]==0)
			{
				numberPackets[routing[2]]++;
				cont_receive[2]=0;
			}
		}
}

void Receive3(ppmNetValue new, void *userdata)
{
		int data;
		
		data = ppmReadNet(handles.data_read[3]);
		if(first[3] == NULL) 
		{
			first[3] = malloc( sizeof(struct node) ); 
			first[3]->x = data;
			first[3]->next = NULL;
			buffer[3] = first[3];
		}
		else
		{
			buffer[3]->next = malloc( sizeof(struct node) ); 
			buffer[3] = buffer[3]->next;
			buffer[3]->x = data;
			buffer[3]->next = NULL;
		}
		cont_receive[3]++;
		if(cont_receive[3]==1)
		{
			x_dest[3] = data >> 8;
			y_dest[3] = data & 0xFF;
			if(x_dest[3]>x_router)
			{
				routing[3] = EAST;
			}
			if(x_dest[3]<x_router)
			{
					routing[3] = WEST;
			}
			if(x_dest[3]==x_router)
			{
				if(y_dest[3]>y_router)
				{
					routing[3] = NORTH;
				}
				if(y_dest[3]<y_router)
				{
					routing[3] = SOUTH;
				}
				if(y_dest[3]==y_router)
				{
					routing[3] = LOCAL;
				}
			}
			
			if(first_send[3] == NULL) 
			{
				first_send[3] = malloc( sizeof(struct node) ); 
				first_send[3]->x = routing[3];
				first_send[3]->next = NULL;
				buffer_send[3] = first_send[3];
			}
			else
			{
				buffer_send[3]->next = malloc( sizeof(struct node) ); 
				buffer_send[3] = buffer_send[3]->next;
				buffer_send[3]->x = routing[3];
				buffer_send[3]->next = NULL;
			}
		//	printf("\n3-routerAddress %d %d to  %d  %d  ---> port %d\n", x_router, y_router, x_dest[3], y_dest[3], routing[3]);
		}
		if(cont_receive[3]==2)
		{
			pkt_size_receive[3]=data;
			flits_counter[3] = flits_counter[3] + data + 2;
		}
		if(cont_receive[3]>2)
		{
			pkt_size_receive[3]--;
			if(pkt_size_receive[3]==0)
			{
				numberPackets[routing[3]]++;
				cont_receive[3]=0;
			}
		}
}

void Receive4(ppmNetValue new, void *userdata)
{
		int data;
		
		data = ppmReadNet(handles.data_read[4]);
		if(first[4] == NULL) 
		{
			first[4] = malloc( sizeof(struct node) ); 
			first[4]->x = data;
			first[4]->next = NULL;
			buffer[4] = first[4];
		}
		else
		{
			buffer[4]->next = malloc( sizeof(struct node) ); 
			buffer[4] = buffer[4]->next;
			buffer[4]->x = data;
			buffer[4]->next = NULL;
		}
		cont_receive[4]++;
		if(cont_receive[4]==1)
		{

			x_dest[4] = data >> 8;
			y_dest[4] = data & 0xFF;
			if(x_dest[4]>x_router)
			{
				routing[4] = EAST;
			}
			if(x_dest[4]<x_router)
			{
					routing[4] = WEST;
			}
			if(x_dest[4]==x_router)
			{
				if(y_dest[4]>y_router)
				{
					routing[4] = NORTH;
				}
				if(y_dest[4]<y_router)
				{
					routing[4] = SOUTH;
				}
				if(y_dest[4]==y_router)
				{
					routing[4] = LOCAL;
				}
			}
			
			if(first_send[4] == NULL) 
			{
				first_send[4] = malloc( sizeof(struct node) ); 
				first_send[4]->x = routing[4];
				first_send[4]->next = NULL;
				buffer_send[4] = first_send[4];
			}
			else
			{
				buffer_send[4]->next = malloc( sizeof(struct node) ); 
				buffer_send[4] = buffer_send[4]->next;
				buffer_send[4]->x = routing[4];
				buffer_send[4]->next = NULL;
			}
			//printf("\n4-routerAddress %d %d to  %d  %d  ---> port %d\n", x_router, y_router, x_dest[4], y_dest[4], routing[4]);
		}
		if(cont_receive[4]==2)
		{
			pkt_size_receive[4]=data;
			flits_counter[4] = flits_counter[4] + data + 2;
		}
		if(cont_receive[4]>2)
		{
			pkt_size_receive[4]--;
			if(pkt_size_receive[4]==0)
			{
				numberPackets[routing[4]]++;
				cont_receive[4]=0;
			}
		}

}

void schedule(int p)
{
	switch(prox[p])
	{
		case LOCAL:
				if(first_send[EAST ]!=NULL && first_send[EAST ]->x==p) prox[p] = EAST;
				else if(first_send[WEST ]!=NULL && first_send[WEST ]->x==p) prox[p] = WEST;
				else if(first_send[NORTH ]!=NULL && first_send[NORTH ]->x==p) prox[p] = NORTH;
				else if(first_send[SOUTH ]!=NULL && first_send[SOUTH ]->x==p) prox[p] = SOUTH;
				else                 prox[p] = LOCAL;
		break;
		case EAST:
				if     (first_send[WEST ]!=NULL && first_send[WEST ]->x==p) prox[p] = WEST;
				else if(first_send[NORTH]!=NULL && first_send[NORTH]->x==p) prox[p] = NORTH;
				else if(first_send[SOUTH]!=NULL && first_send[SOUTH]->x==p) prox[p] = SOUTH;
				else if(first_send[LOCAL]!=NULL && first_send[LOCAL]->x==p) prox[p] = LOCAL;
				else                 prox[p] = EAST;
		break;
		case WEST:
				if     (first_send[NORTH]!=NULL && first_send[NORTH]->x==p) prox[p] = NORTH;
				else if(first_send[SOUTH]!=NULL && first_send[SOUTH]->x==p) prox[p] = SOUTH;
				else if(first_send[LOCAL]!=NULL && first_send[LOCAL]->x==p) prox[p] = LOCAL;
				else if(first_send[EAST ]!=NULL && first_send[EAST ]->x==p) prox[p] = EAST;
				else                 prox[p] = WEST;
		break;
		case NORTH:
				if     (first_send[SOUTH]!=NULL && first_send[SOUTH]->x==p) prox[p] = SOUTH;
				else if(first_send[LOCAL]!=NULL && first_send[LOCAL]->x==p) prox[p] = LOCAL;
				else if(first_send[EAST ]!=NULL && first_send[EAST ]->x==p) prox[p] = EAST;
				else if(first_send[WEST ]!=NULL && first_send[WEST ]->x==p) prox[p] = WEST;
				else                 prox[p] = NORTH;
		break;
		case SOUTH:
				if     (first_send[LOCAL]!=NULL && first_send[LOCAL]->x==p) prox[p] = LOCAL;
				else if(first_send[EAST ]!=NULL && first_send[EAST ]->x==p) prox[p] = EAST;
				else if(first_send[WEST ]!=NULL && first_send[WEST ]->x==p) prox[p] = WEST;
				else if(first_send[NORTH]!=NULL && first_send[NORTH]->x==p) prox[p] = NORTH;
				else                 prox[p] = SOUTH;
		break;
	}
}

static void arbiter0(void *user)
{
	int data;
	
	for(;;) 
	{
     	//bhmWaitDelay(0.01);
     	finish_arbiter[0] = 1;
     	bhmWaitEvent(thArbiter0.start);
		while(finish_arbiter[0])
		{
			bhmWaitDelay(0.01);
			if(cont_arbiter[0]==0)
			{
				schedule(0);
			}
			if(first[prox[0]]!=NULL && first_send[prox[0]]!=NULL && first_send[prox[0]]->x==0) 
			{
				if(first_read[prox[0]] == 1)
				{
					free(clean[prox[0]]);
				}
				else
				{
					first_read[prox[0]] = 1;
				}
				data = first[prox[0]]->x;
				ppmWriteNet(handles.data_write[0], data);
				//printf("Writing data 0x%08x from port %d to port %d: %d\n", data, prox[0], 0, routerAddress);
				clean[prox[0]] = first[prox[0]];
				first[prox[0]] = first[prox[0]]->next;
				cont_arbiter[0]++;
				/*if(cont_arbiter[0]==1) 
				{
					cont_out[0]++;
				}*/
				if(cont_arbiter[0]==2)
				{
					pkt_size_arbiter[0]=data;
					//if(routerAddress == 2048) printf("xxxxxxxxxxxxxxxxx pkt_size_arbiter %d : %d\n", routerAddress, pkt_size_arbiter[0]);
				}
				if(cont_arbiter[0]>2)
				{
					pkt_size_arbiter[0]--;
					//if(routerAddress == 2048) printf("pkt_size_arbiter[0] %d \n", pkt_size_arbiter[0]);
					if(pkt_size_arbiter[0]==0)
					{
						cont_arbiter[0]=0;
						first_send[prox[0]] = first_send[prox[0]]->next;
						numberPackets[0]--;
						finish_arbiter[0] = 0;
						//printf("pkt_size_arbiter[0] == 0\n");
						//finished[0] = 1;
						//cont_out[0]++;
						//printf("cont_out[%d] %d in %d\n", p, cont_out[0], routerAddress);
					}
				}
			}
		}
		//if(routerAddress == 2048) printf("FIM - arbiter0 - router %d \n", routerAddress);
	}
}

static void arbiter1(void *user)
{
	int data;
	
	for(;;) 
	{
     	//bhmWaitDelay(0.01);
     	finish_arbiter[1] = 1;
     	bhmWaitEvent(thArbiter1.start);
     	//if(routerAddress == 2048) printf("arbiter1 - router %d \n", routerAddress);
		while(finish_arbiter[1])
		{
			bhmWaitDelay(0.01);
			if(cont_arbiter[1]==0)
			{
				schedule(1);
			}
			
			if(first[prox[1]]!=NULL && first_send[prox[1]]!=NULL && first_send[prox[1]]->x==1) 
			{
				if(first_read[prox[1]] == 1)
				{
					free(clean[prox[1]]);
				}
				else
				{
					first_read[prox[1]] = 1;
				}
				data = first[prox[1]]->x;
				ppmWriteNet(handles.data_write[1], data);
				//printf("Writing data 0x%08x from port %d to port %d: %d\n", data, prox[1], 1, routerAddress);
				clean[prox[1]] = first[prox[1]];
				first[prox[1]] = first[prox[1]]->next;
				cont_arbiter[1]++;
				/*if(cont_arbiter[1]==1) 
				{
					cont_out[1]++;
				}*/
				if(cont_arbiter[1]==2)
				{
					pkt_size_arbiter[1]=data;
					//printf("xxxxxxxxxxxxxxxxx pkt_size_arbiter %d : %d\n", routerAddress, pkt_size_arbiter[1]);
				}
				if(cont_arbiter[1]>2)
				{
					pkt_size_arbiter[1]--;
					if(pkt_size_arbiter[1]==0)
					{
						cont_arbiter[1]=0;
						first_send[prox[1]] = first_send[prox[1]]->next;
						numberPackets[1]--;
						finish_arbiter[1] = 0;
						//printf("pkt_size_arbiter[1] == 0\n");
						//finished[1] = 1;
						//cont_out[1]++;
						//printf("cont_out[%d] %d in %d\n", p, cont_out[1], routerAddress);
					}
				}
			}
		}
		//if(routerAddress == 2048) printf("FIM - arbiter1 - router %d \n", routerAddress);
    }
}

static void arbiter2(void *user)
{
	int data;
	
	for(;;) 
	{
     	//bhmWaitDelay(0.01);
     	finish_arbiter[2] = 1;
     	bhmWaitEvent(thArbiter2.start);
     	//printf("\narbiter2 - router %d \n", routerAddress);
		while(finish_arbiter[2])
		{
			bhmWaitDelay(0.01);
			if(cont_arbiter[2]==0)
			{
				schedule(2);
			}
			
			if(first[prox[2]]!=NULL && first_send[prox[2]]!=NULL && first_send[prox[2]]->x==2) 
			{
				if(first_read[prox[2]] == 1)
				{
					free(clean[prox[2]]);
				}
				else
				{
					first_read[prox[2]] = 1;
				}
				data = first[prox[2]]->x;
				ppmWriteNet(handles.data_write[2], data);
				//printf("Writing data 0x%08x from port %d to port %d: %d\n", data, prox[2], 2, routerAddress);
				clean[prox[2]] = first[prox[2]];
				first[prox[2]] = first[prox[2]]->next;
				cont_arbiter[2]++;
				/*if(cont_arbiter[2]==1) 
				{
					cont_out[2]++;
				}*/
				if(cont_arbiter[2]==2)
				{
					pkt_size_arbiter[2]=data;
					//printf("xxxxxxxxxxxxxxxxx pkt_size_arbiter %d : %d\n", routerAddress, pkt_size_arbiter[2]);
				}
				if(cont_arbiter[2]>2)
				{
					pkt_size_arbiter[2]--;
					if(pkt_size_arbiter[2]==0)
					{
						cont_arbiter[2]=0;
						first_send[prox[2]] = first_send[prox[2]]->next;
						numberPackets[2]--;
						finish_arbiter[2] = 0;
						//printf("pkt_size_arbiter[2] == 0\n");
						//finished[2] = 1;
						//cont_out[2]++;
						//printf("cont_out[%d] %d in %d\n", p, cont_out[2], routerAddress);
					}
				}
			}
		}
		//if(routerAddress == 2048) printf("FIM - arbiter2 - router %d \n", routerAddress);
    }
}

static void arbiter3(void *user)
{
	int data;
	
	for(;;) 
	{
     	//bhmWaitDelay(0.01);
     	finish_arbiter[3] = 1;
     	bhmWaitEvent(thArbiter3.start);
		while(finish_arbiter[3])
		{
			bhmWaitDelay(0.01);
			if(cont_arbiter[3]==0)
			{
				schedule(3);
			}
			
			if(first[prox[3]]!=NULL && first_send[prox[3]]!=NULL && first_send[prox[3]]->x==3) 
			{
				if(first_read[prox[3]] == 1)
				{
					free(clean[prox[3]]);
				}
				else
				{
					first_read[prox[3]] = 1;
				}
				data = first[prox[3]]->x;
				ppmWriteNet(handles.data_write[3], data);
				//printf("Writing data 0x%08x from port %d to port %d: %d\n", data, prox[3], 3, routerAddress);
				clean[prox[3]] = first[prox[3]];
				first[prox[3]] = first[prox[3]]->next;
				cont_arbiter[3]++;
				/*if(cont_arbiter[3]==1) 
				{
					cont_out[3]++;
				}*/
				if(cont_arbiter[3]==2)
				{
					pkt_size_arbiter[3]=data;
					//printf("xxxxxxxxxxxxxxxxx pkt_size_arbiter %d : %d\n", routerAddress, pkt_size_arbiter[3]);
				}
				if(cont_arbiter[3]>2)
				{
					pkt_size_arbiter[3]--;
					if(pkt_size_arbiter[3]==0)
					{
						cont_arbiter[3]=0;
						first_send[prox[3]] = first_send[prox[3]]->next;
						numberPackets[3]--;
						finish_arbiter[3] = 0;
						//printf("pkt_size_arbiter[3] == 0\n");
						//finished[3] = 1;
						//cont_out[3]++;
						//printf("cont_out[%d] %d in %d\n", p, cont_out[3], routerAddress);
					}
				}
			}
		}
		//if(routerAddress == 2048) printf("FIM - arbiter3 - router %d \n", routerAddress);
    }
}

static void arbiter4(void *user)
{
	int data; 
	
	for(;;) 
	{
     	//bhmWaitDelay(0.01);
     	finish_arbiter[4] = 1;
     	//printf("arbiter4.1 - router %d \n", routerAddress);
     	bhmWaitEvent(thArbiter4.start);
     	//printf("arbiter4 - router %d \n", routerAddress);
		while(finish_arbiter[4])
		{
			bhmWaitDelay(0.01);
			if(cont_arbiter[4]==0)
			{
				schedule(4);
				//printf("\n\nschedule: \n\n");
			}
			
			if(first[prox[4]]!=NULL && first_send[prox[4]]!=NULL && first_send[prox[4]]->x==4) 
			{
				//printf("\n\nfirst: \n\n");
				if(first_read[prox[4]] == 1)
				{
					free(clean[prox[4]]);
				}
				else
				{
					first_read[prox[4]] = 1;
				}
				data = first[prox[4]]->x;
				ppmWriteNet(handles.data_write[4], data);
				//printf("Writing data 0x%08x from port 4 to port %d: %d\n", data, prox[4], routerAddress);
				clean[prox[4]] = first[prox[4]];
				first[prox[4]] = first[prox[4]]->next;
				cont_arbiter[4]++;
				/*if(cont_arbiter[4]==1) 
				{
					cont_out[4]++;
				}*/
				if(cont_arbiter[4]==2)
				{
					pkt_size_arbiter[4]=data;
					//printf("xxxxxxxxxxxxxxxxx pkt_size_arbiter %d : %d\n", routerAddress, pkt_size_arbiter[4]);
				}
				if(cont_arbiter[4]>2)
				{
					pkt_size_arbiter[4]--;
					if(pkt_size_arbiter[4]==0)
					{
						cont_arbiter[4]=0;
						first_send[prox[4]] = first_send[prox[4]]->next;
						numberPackets[4]--;
						finish_arbiter[4] = 0;
						//printf("pkt_size_arbiter[4] == 0\n");
						//finished[4] = 1;
						//cont_out[4]++;
						//printf("cont_out[%d] %d in %d\n", p, cont_out[4], routerAddress);
					}
				}
			}
		}
		//if(routerAddress == 2048) printf("FIM - arbiter4 - router %d \n", routerAddress);
    }
}

static void Clock(void *user)
{
	for(;;) 
	{
     	bhmWaitDelay(0.01);
     	if(numberPackets[0]>0 && pkt_size_arbiter[0]==0) bhmTriggerEvent(thArbiter0.start);
		if(numberPackets[1]>0 && pkt_size_arbiter[1]==0) bhmTriggerEvent(thArbiter1.start);
		if(numberPackets[2]>0 && pkt_size_arbiter[2]==0) bhmTriggerEvent(thArbiter2.start);
		if(numberPackets[3]>0 && pkt_size_arbiter[3]==0) bhmTriggerEvent(thArbiter3.start);
		if(numberPackets[4]>0 && pkt_size_arbiter[4]==0) bhmTriggerEvent(thArbiter4.start);
    }
}

void userInit(void)
{
	Uns32 oldValue = -1;
	int stepIndex;
	
	for (stepIndex=0; stepIndex < NPORT; stepIndex++)
	{
		cont_arbiter[stepIndex] = 0;
		cont_receive[stepIndex] = 0;
		pkt_size_arbiter[stepIndex] = 0;
		pkt_size_receive[stepIndex] = 0;
		prox[stepIndex] = stepIndex;
		x_dest[stepIndex] = 0;
		y_dest[stepIndex] = 0;
		first_read[stepIndex] = 0;
		routing[stepIndex] = 0;
		numberPackets[stepIndex] = 0;
		flits_counter[stepIndex] = 0;
		//cont_in[stepIndex] = 0;
		//cont_out[stepIndex] = 0;
	}
	
	//totalVolume = 0;
	//hops = 0;
	
	ppmInstallNetCallback(handles.router_address, updateRouterAddress, &oldValue);
	ppmInstallNetCallback(handles.end_sim, end_simulation, &oldValue);
	
	//ppmInstallNetCallback(handles.read_data[0], Receive0, &oldValue);
	ppmInstallNetCallback(handles.data_read[0], Receive0, &oldValue);
	//ppmInstallNetCallback(handles.read_data[1], Receive1, &oldValue);
	ppmInstallNetCallback(handles.data_read[1], Receive1, &oldValue);
	//ppmInstallNetCallback(handles.read_data[2], Receive2, &oldValue);
	ppmInstallNetCallback(handles.data_read[2], Receive2, &oldValue);
	//ppmInstallNetCallback(handles.read_data[3], Receive3, &oldValue);
	ppmInstallNetCallback(handles.data_read[3], Receive3, &oldValue);
	//ppmInstallNetCallback(handles.read_data[4], Receive4, &oldValue);
	ppmInstallNetCallback(handles.data_read[4], Receive4, &oldValue);
	//ppmInstallNetCallback(handles.send_data[stepIndex], updateNetValues, &oldValue);
	//ppmInstallNetCallback(handles.data_write[stepIndex], updateNetValues, &oldValue);
	
	thArbiter0.start = bhmCreateEvent();
	thArbiter0.thread = bhmCreateThread(
		arbiter0,
		NULL,
		"thArbiter0",
		&thArbiter0.stack[THREAD_STACK] // top of downward growing stack
	);
	
	thArbiter1.start = bhmCreateEvent();
	thArbiter1.thread = bhmCreateThread(
		arbiter1,
		NULL,
		"thArbiter1",
		&thArbiter1.stack[THREAD_STACK] // top of downward growing stack
	);
	
	thArbiter2.start = bhmCreateEvent();
	thArbiter2.thread = bhmCreateThread(
		arbiter2,
		NULL,
		"thArbiter2",
		&thArbiter2.stack[THREAD_STACK] // top of downward growing stack
	);
	
	thArbiter3.start = bhmCreateEvent();
	thArbiter3.thread = bhmCreateThread(
		arbiter3,
		NULL,
		"thArbiter3",
		&thArbiter3.stack[THREAD_STACK] // top of downward growing stack
	);
	
	thArbiter4.start = bhmCreateEvent();
	thArbiter4.thread = bhmCreateThread(
		arbiter4,
		NULL,
		"thArbiter4",
		&thArbiter4.stack[THREAD_STACK] // top of downward growing stack
	);
	
	thClock.start = bhmCreateEvent();
	thClock.thread = bhmCreateThread(
		Clock,
		NULL,
		"thClock",
		&thClock.stack[THREAD_STACK] // top of downward growing stack
	);
}

