
#pragma once

#include "demhandlers.h"
#include "demofile/demotypes.h"
#include "netmessages/netcontants.h"

namespace DemMsg
{
    struct Dem_Packet
    {
        democmdinfo_t cmdInfo;
        int32_t sequenceNum1;
        int32_t sequenceNum2;
        //uint32_t dataLengthInBytes;
        //uint8_t data[NET_MAX_PAYLOAD];
    };
}

DECLARE_DEM_HANDLERS(Dem_Packet);
