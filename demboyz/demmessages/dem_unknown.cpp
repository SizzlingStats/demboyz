
#include "dem_unknown.h"

namespace DemHandlers
{
    bool Dem_Unknown_FileRead_Internal(DemoFileReader& demofile, DemMsg::Dem_Unknown* data)
    {
        return true;
    }

    bool Dem_Unknown_FileWrite_Internal(DemoFileWriter& demofile, DemMsg::Dem_Unknown* data)
    {
        return true;
    }

    bool Dem_Unknown_JsonRead_Internal(JsonRead& jsonbuf, DemMsg::Dem_Unknown* data)
    {
        return true;
    }

    bool Dem_Unknown_JsonWrite_Internal(JsonWrite& jsonbuf, DemMsg::Dem_Unknown* data)
    {
        return true;
    }
}
