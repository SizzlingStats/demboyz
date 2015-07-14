
#include "demhandlers.h"
#include "demmessages.h"

void DemHandlers::CreateDemMsgStructs(void* demDataStructs[9])
{
    demDataStructs[0] = new DemMsg::Dem_Unknown();
    demDataStructs[1] = new DemMsg::Dem_Packet();
    demDataStructs[2] = new DemMsg::Dem_Packet();
    demDataStructs[3] = new DemMsg::Dem_SyncTick();
    demDataStructs[4] = new DemMsg::Dem_ConsoleCmd();
    demDataStructs[5] = new DemMsg::Dem_UserCmd();
    demDataStructs[6] = new DemMsg::Dem_DataTables();
    demDataStructs[7] = new DemMsg::Dem_Stop();
    demDataStructs[8] = new DemMsg::Dem_StringTables();
}

void DemHandlers::DestroyDemMsgStructs(void* demDataStructs[9])
{
    delete reinterpret_cast<DemMsg::Dem_Unknown*>(demDataStructs[0]);
    delete reinterpret_cast<DemMsg::Dem_Packet*>(demDataStructs[1]);
    delete reinterpret_cast<DemMsg::Dem_Packet*>(demDataStructs[2]);
    delete reinterpret_cast<DemMsg::Dem_SyncTick*>(demDataStructs[3]);
    delete reinterpret_cast<DemMsg::Dem_ConsoleCmd*>(demDataStructs[4]);
    delete reinterpret_cast<DemMsg::Dem_UserCmd*>(demDataStructs[5]);
    delete reinterpret_cast<DemMsg::Dem_DataTables*>(demDataStructs[6]);
    delete reinterpret_cast<DemMsg::Dem_Stop*>(demDataStructs[7]);
    delete reinterpret_cast<DemMsg::Dem_StringTables*>(demDataStructs[8]);
}

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

typedef bool (*DemMsgFileReadFn)(DemHandlers::FileRead& demofile, void* data);
typedef bool (*DemMsgFileWriteFn)(DemHandlers::FileWrite& demofile, void* data);
typedef bool (*DemMsgJsonReadFn)(DemHandlers::JsonRead& jsonbuf, void* data);
typedef bool (*DemMsgJsonWriteFn)(DemHandlers::JsonWrite& jsonbuf, void* data);

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
