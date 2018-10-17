#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "icm/icmCpuManager.hpp"
#include <sys/time.h>
#include "defines.h"

#define N_PE 		225
#define N_PE_X 	15
#define N_PE_Y 	15

#define SIM_ATTRS (ICM_ATTR_DEFAULT)

icmProcessorP processors[N_PE];
icmBusP bus[N_PE];
icmMemoryP mem[N_PE];
int dma_size[N_PE];
int dma_address[N_PE];
int dma_op[N_PE];
int dma_start[N_PE];
int header_dma_size[N_PE];
int data_in[N_PE];
int first_read[N_PE];
int page[N_PE];
int irq_mask[N_PE];
int irq_status[N_PE];
int counter_reg[N_PE];
int repo_write[N_PE];
int next_page[N_PE];
int first_address[N_PE];
int packets_in[N_PE];
int packets_out[N_PE];
int cpu_halt[N_PE];
icmPseP pse[N_PE];
icmPseP dynamicApps;
icmNetP router_address[N_PE];
icmNetP end_sim[N_PE];
icmNetP read_data[N_PE][NPORT];
icmNetP data_read[N_PE][NPORT];
icmNetP send_data[N_PE][NPORT];
icmNetP data_write[N_PE][NPORT];
icmNetP ni_intr[N_PE];
icmNetP timeslice_intr[N_PE];
icmNetP n_req_app;
icmNetP n_ack_app;
icmNetP n_app_size;
icmNetP n_finished_tasks;
int timeslice[N_PE];
int wait_scheduler[N_PE];
int scheduler[N_PE];
int ni_i[N_PE];
int timeslice_i[N_PE];
int req_app;
int ack_app;
int app_size;
float totalEnergy;
int totalVolume;
int volume[N_PE];
float energy[N_PE];
int router_cycles[N_PE];
int swap_read[N_PE];
int init_counter;
int mapped_tasks = 0;
int finished_tasks = 0;

struct timeval stop, start;
double max_instructions = 0;
double totalSimulated;
double elapsed;
double mips;

//function that converts big endian/little endian or little endian/big endian
Uns32 swap_Uns32(Uns32 val)
{
    val = ((val << 8) & 0xFF00FF00 ) | ((val >> 8) & 0xFF00FF );
    return (val << 16) | (val >> 16);
}

char nameString[64]; // temporary string to create labels

struct node {
  int x;
  struct node *next;
};

struct node *buff[N_PE];
struct node *first[N_PE];
struct node *clean[N_PE];
int size[N_PE];
int size_packet[N_PE];
int counter_packet[N_PE];
int counter[N_PE];
int counter_send[N_PE];
int size_send[N_PE];
int xo[N_PE];
int yo[N_PE];
int xd[N_PE];
int yd[N_PE];
int hops[N_PE];
int total_hops;


/** Functions - Trace**/
/** ------------------------------------------------------------------------------------- **/
int NormalAddress(int normal)
{
	int n_address, pos_x, pos_y;
	pos_x = normal >> 8;
	pos_y = normal & 0xFF;
	if(N_PE_X <= N_PE_Y)
	{
		n_address = (N_PE_Y * pos_y) + pos_x;
	}
	else
	{
		n_address = (N_PE_X * pos_y) + pos_x;
	}
	return n_address;
}

NET_WRITE_FN(req_app_update)
{
	req_app = value;
}

NET_WRITE_FN(app_size_update)
{
	app_size = value;
}

void bufferReceive(int p, int value)
{
     //   if(packets_in[p]!=packets_out[p])
		   // register_unfreeze(p, 0);
		counter[p]++;
		if(first[p] == NULL)
		{
			first[p] = malloc( sizeof(struct node) );
			first[p]->x = value;
			first[p]->next = NULL;
			buff[p] = first[p];
		}
		else
		{
			buff[p]->next = malloc( sizeof(struct node) );
			buff[p] = buff[p]->next;
			buff[p]->x = value;
			buff[p]->next = NULL;
		}
		if(counter[p]==2)
		{
			size[p] = value;
		}
		if(counter[p]>2)
		{
			size[p]--;
			if(size[p]==0)
			{
				counter[p] = 0;
				packets_in[p]++;
     //   if(packets_in[p]!=packets_out[p])
		   // register_unfreeze(p, 1);
			}
		}
}

NET_WRITE_FN(Receive)
{
	int processorNumber = (int) userData;
	bufferReceive(processorNumber, value);
}


int RouterPosition(int router)
{
	int pos;

	int column = router%N_PE_X;

	if(router>=(N_PE-N_PE_X)){ //TOP
		if(column==(N_PE_X-1)){ //RIGHT
			pos = TR;
		}
		else{
			if(column==0){//LEFT
				pos = TL;
			}
			else{//CENTER_X
				pos = TC;
			}
		}
	}
	else{
		if(router<N_PE_X){ //BOTTOM
			if(column==(N_PE_X-1)){ //RIGHT
				pos = BR;
			}
			else{
				if(column==0){//LEFT
					pos = BL;
				}
				else{//CENTER_X
					pos = BC;
				}
			}
		}
		else{//CENTER_Y
			if(column==(N_PE_X-1)){ //RIGHT
				pos = CRX;
			}
			else{
				if(column==0){//LEFT
					pos = CL;
				}
				else{//CENTER_X
					pos = CC;
				}
			}
		}
	}

	return pos;
}

int RouterPorts(int router)
{
	int ports;

	int column = router%N_PE_X;

	if(router>=(N_PE-N_PE_X)){ //TOP
		if(column==(N_PE_X-1)){ //RIGHT
			ports = 3;
		}
		else{
			if(column==0){//LEFT
				ports = 3;
			}
			else{//CENTER_X
				ports = 4;
			}
		}
	}
	else{
		if(router<N_PE_X){ //BOTTOM
			if(column==(N_PE_X-1)){ //RIGHT
				ports = 3;
			}
			else{
				if(column==0){//LEFT
					ports = 3;
				}
				else{//CENTER_X
					ports = 4;
				}
			}
		}
		else{//CENTER_Y
			if(column==(N_PE_X-1)){ //RIGHT
				ports = 4;
			}
			else{
				if(column==0){//LEFT
					ports = 4;
				}
				else{//CENTER_X
					ports = 5;
				}
			}
		}
	}

	return ports;
}

int RouterAddress(int router)
{
	int r_address, pos_x, pos_y;

	if(N_PE_X <= N_PE_Y)
	{
		pos_x = (router%N_PE_Y) << 8;
		pos_y = router/N_PE_Y;
	}
	else
	{
		pos_x = (router%N_PE_X) << 8;
		pos_y = router/N_PE_X;
	}

	r_address = pos_x | pos_y;
	return r_address;
}

void update_volume(int xi, int yi, int xf, int yf, int vol)
{
	int xx = xi;
	int yy = yi;
	while(1)
	{
		if(xf>xi)
		{
			xx++;
		}
		if(xf<xi)
		{
			xx--;
		}
		if(xf==xi)
		{
			if(yf>yi)
			{
				yy++;
			}
			if(yf<yi)
			{
				yy--;
			}
		}
				xi = xx;
				yi = yy;
				volume[NormalAddress(xi*256+yi)] = volume[NormalAddress(xi*256+yi)] + vol;
				router_cycles[NormalAddress(xi*256+yi)] = router_cycles[NormalAddress(xi*256+yi)] + vol+3;
				energy[NormalAddress(xi*256+yi)] = energy[NormalAddress(xi*256+yi)] + (vol*0.0035104);
				if((xi==xf) && (yi==yf)) break;
	}
}

void dmaProcess(int p)
{
	int data[N_PE], verify_data[N_PE];
	while(dma_size[p])
	{
		if(dma_op[p] == READ)
		{
			if(repo_write[p]==0)
			{
				icmReadProcessorMemory(processors[p], dma_address[p], &data[p], 4);
				dma_address[p] = dma_address[p] + 4;
				counter_send[p]++;
				if(counter_send[p]==1)
				{
					xd[p] = swap_Uns32(data[p]) >> 8;
					yd[p] = swap_Uns32(data[p]) & 0xFF;

					xo[p] = RouterAddress(p) >> 8;
					yo[p] = RouterAddress(p) & 0xFF;

					hops[p] = (abs(xd[p]-xo[p])+abs(yd[p]-yo[p]));
				}
				if(counter_send[p]==2)
				{
					size_send[p] = swap_Uns32(data[p]);
					totalVolume = totalVolume + size_send[p] + 2;
					totalEnergy = totalEnergy + hops[p]*(size_send[p] + 2)*0.0035104;
					volume[p] = volume[p] + size_send[p] + 2;
					energy[p] = energy[p] + (size_send[p] + 2)*0.0035104;
					router_cycles[p] = router_cycles[p] + (size_send[p] + 5);
					total_hops = total_hops + hops[p];
					update_volume(xo[p], yo[p], xd[p], yd[p], size_send[p]+2);
				}
				if(counter_send[p]==3 && swap_Uns32(data[p])==0x00000040)
				{
					mapped_tasks++;
				}
				if(counter_send[p]==3 && swap_Uns32(data[p])==0x00000070)
				{
					mapped_tasks--;
					finished_tasks++;
					icmWriteNet(n_finished_tasks, finished_tasks);
				}
				if(counter_send[p]>2)
				{
					size_send[p]--;
					if(size_send[p]==0)
					{
						counter_send[p] = 0;
					}
				}
			}
			else
			{
				//data[p] = repository[dma_address[p]];
				dma_address[p] = dma_address[p] + 1;
				counter_send[p]++;
				if(counter_send[p]==2)
				{
					size_send[p] = swap_Uns32(data[p]);
				}
				if(counter_send[p]>2)
				{
					size_send[p]--;
					if(size_send[p]==0)
					{
						counter_send[p] = 0;
					}
				}
			}
			icmWriteNet(data_read[p][LOCAL], swap_Uns32(data[p]));
			dma_size[p]--;
		}
		else
		{
			if(first[p] != NULL)
			{
				if(first_read[p] == 1)
				{
					free(clean[p]);
				}
				else
				{
					first_read[p] = 1;
				}
				data[p] = first[p]->x;
				if(page[p]!=0)
				{
					if(dma_address[p]==first_address[p])
					{
						data[p] = 0X3c1d0000 | (page[p]>>16);
						data[p] = swap_Uns32(data[p]);
					}
					if(dma_address[p]==(first_address[p]+4))
					{
						data[p] = 0X37bd0000 | ((page[p] & 0xFFFF)+0x4000);
						data[p] = swap_Uns32(data[p]);
					}
					if(dma_address[p]>(first_address[p]+4))
					{
						verify_data[p] = swap_Uns32(data[p]) >> 26;
						if(verify_data[p] == 3 || verify_data[p] == 2)
						{
							data[p] = swap_Uns32(data[p]);
							data[p] = data[p] | (page[p]>>2);
							data[p] = swap_Uns32(data[p]);
						}
					}
				}
				else data[p] = swap_Uns32(data[p]);
				icmWriteProcessorMemory(processors[p], dma_address[p], &data[p], 4);
				counter_packet[p]++;
				if(counter_packet[p] == 2)
				{
					size_packet[p] = data[p];
				}
				if(counter_packet[p] > 2)
				{
					size_packet[p]--;
					if(size_packet[p] == 0)
					{
						counter_packet[p]=0;
						packets_out[p]++;
					}
				}
				dma_address[p] = dma_address[p] + 4;
				dma_size[p]--;
				clean[p] = first[p];
				first[p] = first[p]->next;
			}
			else
			{
				first_read[p] = 0;
			}
		}
	}
	if(dma_size[p] == 0) dma_start[p] = 0;
}

//function acessed when a register is read from memory
int register_bank_read(int address, int p)
{
	int return_data;

	switch(address)
	{
		case DMA_READY:
			if(dma_start[p] == 1)
			{
				return_data = dma_size[p];
			}
			else
			{
				return_data = 0;
			}
		break;

		case PROC_ADDRESS:
			return_data = swap_Uns32(RouterAddress(p));
		break;

		case START_SYSTEM:
			if(init_counter!=N_PE)
			{
				return_data = 1;
			}
			else
			{
				return_data = 0;
			}
		break;

		case READ_NI_READY:
			if(first[p] != NULL)
			{
				if(first_read[p] == 1)
				{
					free(clean[p]);
				}
				else
				{
					first_read[p] = 1;
				}
				data_in[p] = swap_Uns32(first[p]->x);
				counter_packet[p]++;
				if(counter_packet[p] == 2)
				{
					size_packet[p] = first[p]->x;
				}
				if(counter_packet[p] > 2)
				{
					size_packet[p]--;
					if(size_packet[p] == 0)
					{
						counter_packet[p]=0;
						packets_out[p]++;
					}
				}
				header_dma_size[p]--;
				clean[p] = first[p];
				first[p] = first[p]->next;
			}
			else
			{
				first_read[p] = 0;
			}
			return_data = header_dma_size[p];
		break;

		case DATA_IN:
			if(swap_read[p]==0)
			{
				return_data = data_in[p];
			}
			else
			{

				return_data = swap_Uns32(data_in[p]);
			}


		break;

		case IRQ_STATUS:
			if(ni_i[p]==1 && timeslice_i[p]==1)
			{
				return swap_Uns32(IRQ_NOC|IRQ_COUNTER18);
			}
			else
			{
				if(ni_i[p]==1) return swap_Uns32(IRQ_NOC);
				if(timeslice_i[p] ==1) return swap_Uns32(IRQ_COUNTER18);
			}
		break;

		case REQ_APP:
			return_data = swap_Uns32(req_app);
		break;

		case APP_SIZE:
			return_data = swap_Uns32(app_size);
		break;

		default: break;
	}
	return return_data;
}

//function acessed when a register is written in memory
void register_bank_write(int address, int value, int p)
{
	int x, y;
	switch(address)
	{
		case DMA_SIZE:
			dma_size[p] = value;
		break;

		case DMA_ADDRESS:
			dma_address[p] = value;
		break;

		case DMA_OP:
			dma_op[p] = value;
		break;

		case DMA_START:
			first_address[p] = dma_address[p];
			dma_start[p] = 1;
			dmaProcess(p);
		break;

		case START_READ_NI:
			//printf("START_READ_NI\n");
			header_dma_size[p] = 1;
		break;

		case WRITE_NI:
			icmWriteNet(data_read[p][LOCAL], value);
			counter_send[p]++;
			if(counter_send[p]==1)
			{
				xd[p] = value >> 8;
				yd[p] = value & 0xFF;

				xo[p] = RouterAddress(p) >> 8;
				yo[p] = RouterAddress(p) & 0xFF;

				hops[p] = (abs(xd[p]-xo[p])+abs(yd[p]-yo[p]));
			}
			if(counter_send[p]==2)
			{
				size_send[p] = value;
				totalVolume = totalVolume + size_send[p] + 2;
				totalEnergy = totalEnergy + hops[p]*(size_send[p] + 2)*0.0035104;
				volume[p] = volume[p] + size_send[p] + 2;
				energy[p] = energy[p] + (size_send[p] + 2)*0.0035104;
				router_cycles[p] = router_cycles[p] + (size_send[p] + 5);
				total_hops = total_hops + hops[p];
				update_volume(xo[p], yo[p], xd[p], yd[p], size_send[p]+2);
			}
			if(counter_send[p]>2)
			{
				size_send[p]--;
				if(size_send[p]==0)
				{
					counter_send[p] = 0;
				}
			}
		break;

		case PAGE:
			page[p] = value;
		break;

		case IRQ_MASK:
			irq_mask[p] = value;
		break;

		case COUNTER_REG:
			counter_reg[p] = value;
		break;

		case REPOSITORY:
			repo_write[p] = value;
		break;

		case SWAP_READ:
			swap_read[p] = value;
		break;

		case NEXT_PAGE:
			counter_reg[p] = 0;
			next_page[p] = value;
		break;

		case SCHEDULER_CALLED:
			scheduler[p] = 1;
		break;

		case ACK_APP:
			icmWriteNet(n_ack_app, value);
		break;

		case INITIALIZE_ROUTER:
			icmWriteNet(router_address[value], RouterAddress(value));
		break;

		case END_SIM:
			printf("Total communication volume: f(%d)", value);
			for(y=(N_PE_Y-1); y>-1; y--)
			{
				printf("\n");
				for(x=0; x<N_PE_X; x++) printf("%d	", volume[NormalAddress(x*256+y)]);
			};
			printf("\n");
		break;

	default: break;
	}
}

ICM_MEM_WATCH_FN(sendRegData)
{
	int p = atoi(icmGetProcessorName(processor, "\n"));
	if(packets_in[p]!=packets_out[p])
	{
		ni_i[p] = 1;
		icmWriteNet(ni_intr[p], 1);
	}
	else
	{
		ni_i[p] = 0;
		icmWriteNet(ni_intr[p], 0);
	}

	if(next_page[p] != 0)// && kernel_type[p]==0)
	{
		if(wait_scheduler[p]==1)
		{
			if(scheduler[p]==1)
			{
				scheduler[p]=0;
				wait_scheduler[p]=0;
				timeslice_i[p] = 0;
				icmWriteNet(timeslice_intr[p], 0);
			}
			else
			{
				timeslice_i[p] = 1;
				icmWriteNet(timeslice_intr[p], 1);
			}

		}
		else
		{
			if(timeslice[p]>=16384)
			{
				wait_scheduler[p] = 1;
				timeslice[p] = 0;
			}
			else
			{
				timeslice[p]++;
			}
		}
	}
	else
	{
		timeslice[p] = 0;
	}
}

//register bank callback functions
ICM_MEM_READ_FN(regbank_rb)
{
	int p = atoi(icmGetProcessorName(processor, "\n"));
	int data = register_bank_read(address, p);
	*(Int32 *)value = data;
}

ICM_MEM_WRITE_FN(regbank_wb)
{
	int p = atoi(icmGetProcessorName(processor, "\n"));
	register_bank_write(address, swap_Uns32(*(Uns32*)value), p);
}

//pagination callback functions
ICM_MEM_READ_FN(mem_rb)
{
	int new_address, data;
	int p = atoi(icmGetProcessorName(processor, "\n"));

	if(next_page[p]!=0)
	{
		new_address = (Int32)address | next_page[p];
		icmReadProcessorMemory(processor, new_address, &data, 4);
		*(Int32 *)value = data;
	}
}

ICM_MEM_WRITE_FN(mem_wb)
{
	int new_address;
	int p = atoi(icmGetProcessorName(processor, "\n"));

	new_address = (Int32)address | next_page[p];
	icmWriteProcessorMemory(processor, new_address, value, 4);
}

//
// Main routine
//
int main(int argc, char **argv) {

	gettimeofday(&start, NULL);//----------------------------------------------

    // initialize OVPsim, enabling verbose mode to get statistics at end
    // of execution
    icmInit(ICM_VERBOSE|ICM_STOP_ON_CTRLC, 0, 0);

    const char *model      = icmGetVlnvString(NULL, "mips.ovpworld.org", "processor", "mips32", "1.0", "model");
	const char *semihosting = icmGetVlnvString(NULL, "mips.ovpworld.org", "semihosting", "mips32SDE", "1.0", "model");

	//create a user attribute object
	icmAttrListP userAttrs = icmNewAttrList();
	//add a double attribute to set mips to 1000
	icmAddDoubleAttr(userAttrs, "mips", 1000.0);
    // Create the processor instances
    int stepIndex;
    for (stepIndex=0; stepIndex < N_PE; stepIndex++)
	 {
		sprintf(nameString, "%d", stepIndex);
        processors[stepIndex] = icmNewProcessor(
            nameString,            // CPU name
            "mips32",           // CPU type
            stepIndex,          // CPU cpuId
            0,                  // CPU model flags
            32,                 // address bits
            model,        // model file
            "modelAttrs",       // morpher attributes
            SIM_ATTRS,          // attributes
            0,          // user-defined attributes
            semihosting,     // semi-hosting file
            "modelAttrs"        // semi-hosting attributes
        );



        // create the processor bus
        sprintf(nameString, "BUS-%d", stepIndex);
        bus[stepIndex] = icmNewBus(nameString, 32);

        // connect the processors onto the busses
        icmConnectProcessorBusses(processors[stepIndex], bus[stepIndex], bus[stepIndex]);

        // create memory
        sprintf(nameString, "MEM-%d", stepIndex);
        mem[stepIndex] = icmNewMemory(nameString, ICM_PRIV_RWX, 0xefffffff);

        // connect the memory onto the busses
        icmConnectMemoryToBus(bus[stepIndex], "mp1", mem[stepIndex], 0x00000000);
        sprintf(nameString, "extMemReg-%d", stepIndex);
        icmMapExternalMemory(bus[stepIndex], nameString, ICM_PRIV_RW, 0xf0000000, 0xffffffff, regbank_rb, regbank_wb, 0);

        sprintf(nameString, "router-%d", stepIndex);
        pse[stepIndex] = icmNewPSE(nameString, "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/platform_ovp/router/pse.pse", NULL, NULL, NULL);

        sprintf(nameString, "router_address-%d", stepIndex);
		router_address[stepIndex] = icmNewNet(nameString);
		icmConnectPSENet(pse[stepIndex], router_address[stepIndex], "router_address", ICM_INPUT);

        sprintf(nameString, "end_sim-%d", stepIndex);
		end_sim[stepIndex] = icmNewNet(nameString);
		icmConnectPSENet(pse[stepIndex], end_sim[stepIndex], "end_sim", ICM_INPUT);

        sprintf(nameString, "read_data-%d-local", stepIndex);
		read_data[stepIndex][LOCAL] = icmNewNet(nameString);
		icmConnectPSENet(pse[stepIndex], read_data[stepIndex][LOCAL], "read_data_4", ICM_INPUT);

		sprintf(nameString, "data_read-%d-local", stepIndex);
		data_read[stepIndex][LOCAL] = icmNewNet(nameString);
		icmConnectPSENet(pse[stepIndex], data_read[stepIndex][LOCAL], "data_read_4", ICM_INPUT);

		sprintf(nameString, "send_data-%d-local", stepIndex);
        send_data[stepIndex][LOCAL] = icmNewNet(nameString);
        icmConnectPSENet(pse[stepIndex], send_data[stepIndex][LOCAL], "send_data_4", ICM_OUTPUT);

        sprintf(nameString, "data_write-%d-local", stepIndex);
        data_write[stepIndex][LOCAL] = icmNewNet(nameString);
        icmConnectPSENet(pse[stepIndex], data_write[stepIndex][LOCAL], "data_write_4", ICM_OUTPUT);
	 }
    for (stepIndex=0; stepIndex < N_PE; stepIndex++)
    {
		printf("RouterAddress(%d) 0x%02x\n", stepIndex, RouterAddress(stepIndex));

		//EAST CONNECTION
		if(RouterPosition(stepIndex) != BR && RouterPosition(stepIndex) != CRX && RouterPosition(stepIndex) != TR)
		{
			//printf("EAST CONNECTION\n");
			sprintf(nameString, "data_read-%d-east", stepIndex);
			data_read[stepIndex][EAST] = icmNewNet(nameString);
			icmConnectPSENet(pse[stepIndex], data_read[stepIndex][EAST], "data_read_0", ICM_INPUT);
			icmConnectPSENet(pse[stepIndex+1], data_read[stepIndex][EAST], "data_write_1", ICM_OUTPUT);
		}

		//WEST CONNECTION
		if(RouterPosition(stepIndex) != BL && RouterPosition(stepIndex) != CL && RouterPosition(stepIndex) != TL)
		{
			//printf("WEST CONNECTION\n");
			sprintf(nameString, "data_read-%d-west", stepIndex);
			data_read[stepIndex][WEST] = icmNewNet(nameString);
			icmConnectPSENet(pse[stepIndex], data_read[stepIndex][WEST], "data_read_1", ICM_INPUT);
			icmConnectPSENet(pse[stepIndex-1], data_read[stepIndex][WEST], "data_write_0", ICM_OUTPUT);
		}

		//NORTH CONNECTION
		if(RouterPosition(stepIndex) != TL && RouterPosition(stepIndex) != TC && RouterPosition(stepIndex) != TR)
		{
			//printf("NORTH CONNECTION\n");
			sprintf(nameString, "data_read-%d-north", stepIndex);
			data_read[stepIndex][NORTH] = icmNewNet(nameString);
			icmConnectPSENet(pse[stepIndex], data_read[stepIndex][NORTH], "data_read_2", ICM_INPUT);
			icmConnectPSENet(pse[stepIndex+N_PE_X], data_read[stepIndex][NORTH], "data_write_3", ICM_OUTPUT);
		}

		//SOUTH CONNECTION
		if(RouterPosition(stepIndex) != BL && RouterPosition(stepIndex) != BC && RouterPosition(stepIndex) != BR)
		{
			//printf("SOUTH CONNECTION\n");
			sprintf(nameString, "data_read-%d-south", stepIndex);
			data_read[stepIndex][SOUTH] = icmNewNet(nameString);
			icmConnectPSENet(pse[stepIndex], data_read[stepIndex][SOUTH], "data_read_3", ICM_INPUT);
			icmConnectPSENet(pse[stepIndex-N_PE_X], data_read[stepIndex][SOUTH], "data_write_2", ICM_OUTPUT);
		}
		icmAddNetCallback(data_write[stepIndex][LOCAL], Receive, (void*) stepIndex);
	}

	icmLoadProcessorMemory(processors[0], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_107.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[1], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_71.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[2], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_19.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[3], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_46.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[4], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_93.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[5], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_22.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[6], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_30.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[7], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_45.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[8], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_67.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[9], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_33.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[10], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_15.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[11], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_47.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[12], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_64.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[13], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_70.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[14], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_73.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[15], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_53.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[16], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_90.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[17], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_102.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[18], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_100.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[19], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_102.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[20], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_49.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[21], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_28.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[22], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_82.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[23], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_66.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[24], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_33.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[25], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_59.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[26], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_54.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[27], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_29.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[28], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_111.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[29], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_87.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[30], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_34.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[31], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_38.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[32], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_39.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[33], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_72.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[34], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_77.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[35], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_95.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[36], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_92.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[37], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_23.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[38], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_17.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[39], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_26.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[40], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_34.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[41], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_109.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[42], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_24.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[43], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_36.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[44], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_45.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[45], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_32.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[46], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_97.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[47], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_29.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[48], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_5.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[49], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_60.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[50], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/idle.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[51], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_72.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[52], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_21.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[53], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_6.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[54], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_52.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[55], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_108.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[56], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_106.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[57], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_84.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[58], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_36.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[59], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_54.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[60], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_81.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[61], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_52.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[62], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_6.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[63], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_67.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[64], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_3.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[65], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_31.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[66], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_94.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[67], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_17.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[68], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_66.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[69], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_43.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[70], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_86.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[71], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_35.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[72], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_9.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[73], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_51.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[74], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_0.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[75], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_42.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[76], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_55.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[77], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_78.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[78], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_55.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[79], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_80.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[80], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_68.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[81], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_4.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[82], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_4.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[83], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_95.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[84], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_75.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[85], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_69.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[86], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_7.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[87], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_50.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[88], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_19.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[89], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_98.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[90], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_83.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[91], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_78.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[92], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_20.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[93], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_27.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[94], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_18.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[95], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_86.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[96], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_21.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[97], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_62.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[98], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_83.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[99], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_49.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[100], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_39.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[101], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_58.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[102], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_80.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[103], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_3.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[104], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_68.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[105], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_47.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[106], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_75.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[107], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_13.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[108], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_10.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[109], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_60.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[110], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_57.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[111], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_46.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[112], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_1.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[113], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_12.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[114], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_91.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[115], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_74.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[116], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_77.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[117], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_62.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[118], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_99.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[119], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_50.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[120], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_81.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[121], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_32.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[122], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_8.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[123], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_9.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[124], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_110.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[125], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_96.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[126], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_1.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[127], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_61.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[128], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_76.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[129], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_65.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[130], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_37.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[131], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_24.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[132], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_57.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[133], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_26.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[134], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_63.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[135], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_44.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[136], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_87.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[137], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_8.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[138], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_16.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[139], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_108.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[140], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_76.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[141], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_53.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[142], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_10.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[143], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_73.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[144], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_74.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[145], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_110.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[146], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_43.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[147], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_89.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[148], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_0.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[149], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_100.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[150], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_42.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[151], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_105.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[152], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_79.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[153], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_59.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[154], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_22.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[155], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_88.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[156], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_111.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[157], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_84.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[158], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_44.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[159], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_56.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[160], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_18.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[161], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_15.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[162], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_104.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[163], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_105.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[164], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_35.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[165], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_93.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[166], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_20.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[167], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_69.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[168], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_2.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[169], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_85.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[170], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_103.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[171], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_31.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[172], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_70.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[173], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_13.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[174], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_25.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[175], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_82.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[176], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_88.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[177], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_90.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[178], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_41.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[179], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_30.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[180], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_40.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[181], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_61.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[182], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_91.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[183], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_14.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[184], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_25.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[185], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_40.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[186], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_99.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[187], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_16.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[188], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_48.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[189], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_106.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[190], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_71.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[191], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_7.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[192], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_64.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[193], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_89.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[194], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_2.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[195], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_5.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[196], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_101.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[197], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_12.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[198], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_28.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[199], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_23.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[200], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_38.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[201], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_98.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[202], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_14.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[203], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_107.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[204], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_79.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[205], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_65.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[206], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_37.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[207], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_96.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[208], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_51.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[209], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_104.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[210], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_41.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[211], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_11.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[212], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_101.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[213], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_109.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[214], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_56.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[215], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_85.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[216], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_58.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[217], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_92.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[218], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_27.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[219], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_48.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[220], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_63.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[221], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/front_97.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[222], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_94.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[223], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_11.MIPS32.elf", 0, 0, 1);
	icmLoadProcessorMemory(processors[224], "/home/madalozzo/Dropbox/2-Doutorado/bare_iscas/bareMetal/fft225/fft_103.MIPS32.elf", 0, 0, 1);

    // simulate the platform
    icmProcessorP final = icmSimulatePlatform();

	// was simulation interrupted or did it complete
    if(final && (icmGetStopReason(final)==ICM_SR_INTERRUPT)) {
        icmPrintf("*** simulation interrupted\n");
    }

    icmTerminate();

    return 0;
}