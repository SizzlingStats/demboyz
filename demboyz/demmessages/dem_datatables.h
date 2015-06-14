
#pragma once

#include "demhandlers.h"
#include "base/array.h"

namespace DemMsg
{
    struct Dem_DataTables
    {
        static const int DATA_MAX_LENGTH = 256 * 1024;
        Array<uint8_t> data;
    };
}

DECLARE_DEM_HANDLERS(Dem_DataTables);
