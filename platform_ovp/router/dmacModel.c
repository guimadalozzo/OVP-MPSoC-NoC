#include "dmacModel.h"
#include <stdio.h>

/////////////////////////////// Port Declarations //////////////////////////////

handlesT handles;

//////////////////////////////// Bus Slave Ports ///////////////////////////////

static void installSlavePorts(void) {
	int stepIndex;

	handles.router_address = ppmOpenNetPort("router_address");
	handles.end_sim = ppmOpenNetPort("end_sim");
	//handles.total_volume = ppmOpenNetPort("total_volume");
	for (stepIndex=0; stepIndex < NPORT; stepIndex++)
    {
        char nameString[64]; // temporary string to create labels
		sprintf(nameString, "read_data_%d", stepIndex);
		handles.read_data[stepIndex]		= ppmOpenNetPort(nameString);
		sprintf(nameString, "data_read_%d", stepIndex);
		handles.data_read[stepIndex]		= ppmOpenNetPort(nameString);
		sprintf(nameString, "send_data_%d", stepIndex);
		handles.send_data[stepIndex]		= ppmOpenNetPort(nameString);
		sprintf(nameString, "data_write_%d", stepIndex);
		handles.data_write[stepIndex]		= ppmOpenNetPort(nameString);
	}
}

////////////////////////////////// Constructor /////////////////////////////////

PPM_CONSTRUCTOR_CB(periphConstructor) {
    installSlavePorts();
}

///////////////////////////////////// Main /////////////////////////////////////

int main(int argc, char *argv[]) {
    constructor();
	bhmWaitEvent(bhmGetSystemEvent(BHM_SE_END_OF_SIMULATION));
    destructor();
    return 0;
}

