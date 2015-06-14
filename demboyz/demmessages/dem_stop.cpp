
#include "dem_stop.h"

namespace DemHandlers
{
    bool Dem_Stop_FileRead_Internal(DemoFileReader& demofile, DemMsg::Dem_Stop* data)
    {
        return true;
    }

    bool Dem_Stop_FileWrite_Internal(DemoFileWriter& demofile, DemMsg::Dem_Stop* data)
    {
        return true;
    }

    bool Dem_Stop_JsonRead_Internal(JsonRead& jsonbuf, DemMsg::Dem_Stop* data)
    {
        return true;
    }

    bool Dem_Stop_JsonWrite_Internal(JsonWrite& jsonbuf, DemMsg::Dem_Stop* data)
    {
        return true;
    }
}
