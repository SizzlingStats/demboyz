
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

typedef bool (*DemMsgFileReadFn)(FileRead& demofile, void* data);
typedef bool (*DemMsgFileWriteFn)(FileWrite& demofile, void* data);
typedef bool (*DemMsgJsonReadFn)(JsonRead& jsonbuf, void* data);
typedef bool (*DemMsgJsonWriteFn)(JsonWrite& jsonbuf, void* data);

bool DemHandlers::DemMsg_FileRead(uint32_t type, FileRead& demofile, void* data)
{
    static const DemMsgFileReadFn demHandlers[] = DECLARE_DEM_HANDLER_ARRAY(FileRead);
    if (type >= (sizeof(demHandlers) / sizeof(DemMsgFileReadFn)))
    {
        return false;
    }
    return demHandlers[type](demofile, data);
}

bool DemHandlers::DemMsg_FileWrite(uint32_t type, FileWrite& demofile, void* data)
{
    static const DemMsgFileWriteFn demHandlers[] = DECLARE_DEM_HANDLER_ARRAY(FileWrite);
    if (type >= (sizeof(demHandlers) / sizeof(DemMsgFileWriteFn)))
    {
        return false;
    }
    return demHandlers[type](demofile, data);
}

bool DemHandlers::DemMsg_JsonRead(uint32_t type, JsonRead& jsonbuf, void* data)
{
    static const DemMsgJsonReadFn demHandlers[] = DECLARE_DEM_HANDLER_ARRAY(JsonRead);
    if (type >= (sizeof(demHandlers) / sizeof(DemMsgJsonReadFn)))
    {
        return false;
    }
    return demHandlers[type](jsonbuf, data);
}

bool DemHandlers::DemMsg_JsonWrite(uint32_t type, JsonWrite& jsonbuf, void* data)
{
    static const DemMsgJsonWriteFn demHandlers[] = DECLARE_DEM_HANDLER_ARRAY(JsonWrite);
    if (type >= (sizeof(demHandlers) / sizeof(DemMsgJsonWriteFn)))
    {
        return false;
    }
    return demHandlers[type](jsonbuf, data);
}
