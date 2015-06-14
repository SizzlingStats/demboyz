
#pragma once

#include "demhandlers.h"
#include "base/array.h"

namespace DemMsg
{
    struct Dem_UserCmd
    {
        static const int COMMANDDATA_MAX_LENGTH = 256;
        int32_t commandNum;
        Array<uint8_t> commandData;
    };
}

DECLARE_DEM_HANDLERS(Dem_UserCmd);
