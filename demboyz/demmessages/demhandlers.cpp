
#include "demhandlers.h"
#include "demmessages.h"

#define DECLARE_DEM_HANDLER_ARRAY(funcname) \
    { \
        &DemHandlers::Dem_Unknown_##funcname, \
        &DemHandlers::Dem_Packet_##funcname, \
        &DemHandlers::Dem_Packet_##funcname, \
        &DemHandlers::Dem_SyncTick_##funcname, \
        &DemHandlers::Dem_ConsoleCmd_##funcname, \
        &DemHandlers::Dem_UserCmd_##funcname, \
        &DemHandlers::Dem_DataTables_##funcname, \
        &DemHandlers::Dem_Stop_##funcname, \
        &DemHandlers::Dem_StringTables_##funcname \
    }

typedef bool (*DemMsgFileReadFn)(DemoFileReader& demofile, void* data);
typedef bool (*DemMsgFileWriteFn)(DemoFileWriter& demofile, void* data);
typedef bool (*DemMsgJsonReadFn)(JsonRead& jsonbuf, void* data);
typedef bool (*DemMsgJsonWriteFn)(JsonWrite& jsonbuf, void* data);

bool DemHandlers::DemMsg_FileRead(uint32_t type, DemoFileReader& demofile, void* data)
{
    static const DemMsgFileReadFn demHandlers[] = DECLARE_DEM_HANDLER_ARRAY(FileRead);
    return demHandlers[type](demofile, data);
}

bool DemHandlers::DemMsg_FileWrite(uint32_t type, DemoFileWriter& demofile, void* data)
{
    static const DemMsgFileWriteFn demHandlers[] = DECLARE_DEM_HANDLER_ARRAY(FileWrite);
    return demHandlers[type](demofile, data);
}

bool DemHandlers::DemMsg_JsonRead(uint32_t type, JsonRead& jsonbuf, void* data)
{
    static const DemMsgJsonReadFn demHandlers[] = DECLARE_DEM_HANDLER_ARRAY(JsonRead);
    return demHandlers[type](jsonbuf, data);
}

bool DemHandlers::DemMsg_JsonWrite(uint32_t type, JsonWrite& jsonbuf, void* data)
{
    static const DemMsgJsonWriteFn demHandlers[] = DECLARE_DEM_HANDLER_ARRAY(JsonWrite);
    return demHandlers[type](jsonbuf, data);
}
