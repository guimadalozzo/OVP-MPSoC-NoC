
////////////////////////////////////////////////////////////////////////////////
//
//                W R I T T E N   B Y   I M P E R A S   I G E N
//
//                             Version 20140430.0
//                          Mon Jun 16 16:00:15 2014
//
////////////////////////////////////////////////////////////////////////////////


#ifdef _PSE_
#    include "peripheral/impTypes.h"
#    include "peripheral/bhm.h"
#    include "peripheral/ppm.h"
#else
#    include "hostapi/impTypes.h"
#endif


static ppmBusPort busPorts[] = {
    { 0 }
};

static PPM_BUS_PORT_FN(nextBusPort) {
    if(!busPort) {
        return busPorts;
    } else {
        busPort++;
    }
    return busPort->name ? busPort : 0;
}

static ppmNetPort netPorts[] = {
    {
        .name            = "router_address",
        .type            = PPM_INPUT_PORT,
        .mustBeConnected = 0,
        .description     = "Interrupt router_address"
    },
    {
        .name            = "end_sim",
        .type            = PPM_INPUT_PORT,
        .mustBeConnected = 0,
        .description     = "Interrupt end_sim"
    },
    {
        .name            = "read_data_0",
        .type            = PPM_INPUT_PORT,
        .mustBeConnected = 0,
        .description     = "Interrupt read_data_0"
    },
    {
        .name            = "read_data_1",
        .type            = PPM_INPUT_PORT,
        .mustBeConnected = 0,
        .description     = "Interrupt read_data_1"
    },
    {
        .name            = "read_data_2",
        .type            = PPM_INPUT_PORT,
        .mustBeConnected = 0,
        .description     = "Interrupt read_data_2"
    },
    {
        .name            = "read_data_3",
        .type            = PPM_INPUT_PORT,
        .mustBeConnected = 0,
        .description     = "Interrupt read_data_3"
    },
    {
        .name            = "read_data_4",
        .type            = PPM_INPUT_PORT,
        .mustBeConnected = 0,
        .description     = "Interrupt read_data_4"
    },
    {
        .name            = "data_read_0",
        .type            = PPM_INPUT_PORT,
        .mustBeConnected = 0,
        .description     = "Interrupt data_read_0"
    },
    {
        .name            = "data_read_1",
        .type            = PPM_INPUT_PORT,
        .mustBeConnected = 0,
        .description     = "Interrupt data_read_1"
    },
    {
        .name            = "data_read_2",
        .type            = PPM_INPUT_PORT,
        .mustBeConnected = 0,
        .description     = "Interrupt data_read_2"
    },
    {
        .name            = "data_read_3",
        .type            = PPM_INPUT_PORT,
        .mustBeConnected = 0,
        .description     = "Interrupt data_read_3"
    },
    {
        .name            = "data_read_4",
        .type            = PPM_INPUT_PORT,
        .mustBeConnected = 0,
        .description     = "Interrupt data_read_4"
    },
    {
        .name            = "send_data_0",
        .type            = PPM_OUTPUT_PORT,
        .mustBeConnected = 0,
        .description     = "Interrupt send_data_0"
    },
    {
        .name            = "send_data_1",
        .type            = PPM_OUTPUT_PORT,
        .mustBeConnected = 0,
        .description     = "Interrupt send_data_1"
    },
    {
        .name            = "send_data_2",
        .type            = PPM_OUTPUT_PORT,
        .mustBeConnected = 0,
        .description     = "Interrupt send_data_2"
    },
    {
        .name            = "send_data_3",
        .type            = PPM_OUTPUT_PORT,
        .mustBeConnected = 0,
        .description     = "Interrupt send_data_3"
    },
    {
        .name            = "send_data_4",
        .type            = PPM_OUTPUT_PORT,
        .mustBeConnected = 0,
        .description     = "Interrupt send_data_4"
    },
    {
        .name            = "data_write_0",
        .type            = PPM_OUTPUT_PORT,
        .mustBeConnected = 0,
        .description     = "Interrupt data_write_0"
    },
    {
        .name            = "data_write_1",
        .type            = PPM_OUTPUT_PORT,
        .mustBeConnected = 0,
        .description     = "Interrupt data_write_1"
    },
    {
        .name            = "data_write_2",
        .type            = PPM_OUTPUT_PORT,
        .mustBeConnected = 0,
        .description     = "Interrupt data_write_2"
    },
    {
        .name            = "data_write_3",
        .type            = PPM_OUTPUT_PORT,
        .mustBeConnected = 0,
        .description     = "Interrupt data_write_3"
    },
    {
        .name            = "data_write_4",
        .type            = PPM_OUTPUT_PORT,
        .mustBeConnected = 0,
        .description     = "Interrupt data_write_4"
    },
    { 0 }
};

static PPM_NET_PORT_FN(nextNetPort) {
    if(!netPort) {
        return netPorts;
    } else {
        netPort++;
    }
    return netPort->name ? netPort : 0;
}

static ppmParameter parameters[] = {
    { 0 }
};

static PPM_PARAMETER_FN(nextParameter) {
    if(!parameter) {
        return parameters;
    } else {
        parameter++;
    }
    return parameter->name ? parameter : 0;
}

ppmModelAttr modelAttrs = {

    .versionString = PPM_VERSION_STRING,
    .type          = PPM_MT_PERIPHERAL,

    .busPortsCB    = nextBusPort,  
    .netPortsCB    = nextNetPort,  
    .paramSpecCB   = nextParameter,

    .vlnv          = {
        .vendor  = "ovpworld.org",
        .library = "peripheral",
        .name    = "dmac",
        .version = "1.0"
    },

    .family    = "ovpworld.org",           

};
