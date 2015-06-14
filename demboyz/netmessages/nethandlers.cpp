
#include "nethandlers.h"
#include "netmessages.h"

#define DECLARE_NET_HANDLER_ARRAY(funcname) \
    { \
        &NetHandlers::Net_NOP_##funcname, \
        &NetHandlers::Net_Disconnect_##funcname, \
        &NetHandlers::Net_File_##funcname, \
        &NetHandlers::Net_Tick_##funcname, \
        &NetHandlers::Net_StringCmd_##funcname, \
        &NetHandlers::Net_SetConVar_##funcname, \
        &NetHandlers::Net_SignonState_##funcname, \
        &NetHandlers::SVC_Print_##funcname, \
        &NetHandlers::SVC_ServerInfo_##funcname, \
        &NetHandlers::SVC_SendTable_##funcname, \
        &NetHandlers::SVC_ClassInfo_##funcname, \
        &NetHandlers::SVC_SetPause_##funcname, \
        &NetHandlers::SVC_CreateStringTable_##funcname, \
        &NetHandlers::SVC_UpdateStringTable_##funcname, \
        &NetHandlers::SVC_VoiceInit_##funcname, \
        &NetHandlers::SVC_VoiceData_##funcname, \
        &NetHandlers::SVC_HLTV_##funcname, \
        &NetHandlers::SVC_Sounds_##funcname, \
        &NetHandlers::SVC_SetView_##funcname, \
        &NetHandlers::SVC_FixAngle_##funcname, \
        &NetHandlers::SVC_CrosshairAngle_##funcname, \
        &NetHandlers::SVC_BSPDecal_##funcname, \
        &NetHandlers::SVC_TerrainMod_##funcname, \
        &NetHandlers::SVC_UserMessage_##funcname, \
        &NetHandlers::SVC_EntityMessage_##funcname, \
        &NetHandlers::SVC_GameEvent_##funcname, \
        &NetHandlers::SVC_PacketEntities_##funcname, \
        &NetHandlers::SVC_TempEntities_##funcname, \
        &NetHandlers::SVC_Prefetch_##funcname, \
        &NetHandlers::SVC_Menu_##funcname, \
        &NetHandlers::SVC_GameEventList_##funcname, \
        &NetHandlers::SVC_GetCvarValue_##funcname \
    }

typedef bool (*NetMsgBitReadFn)(bf_read& bitbuf, SourceGameContext& context, void* data);
typedef bool (*NetMsgBitWriteFn)(bf_write& bitbuf, const SourceGameContext& context, void* data);
typedef bool (*NetMsgJsonReadFn)(JsonRead& jsonbuf, SourceGameContext& context, void* data);
typedef bool (*NetMsgJsonWriteFn)(JsonWrite& jsonbuf, const SourceGameContext& context, void* data);
typedef void (*NetMsgToStringFn)(std::ostringstream& out, void* data);

bool NetHandlers::NetMsg_BitRead(uint32_t type, bf_read& bitbuf, SourceGameContext& context, void* data)
{
    static const NetMsgBitReadFn netHandlers[] = DECLARE_NET_HANDLER_ARRAY(BitRead);
    return netHandlers[type](bitbuf, context, data);
}

bool NetHandlers::NetMsg_BitWrite(uint32_t type, bf_write& bitbuf, const SourceGameContext& context, void* data)
{
    static const NetMsgBitWriteFn netHandlers[] = DECLARE_NET_HANDLER_ARRAY(BitWrite);
    return netHandlers[type](bitbuf, context, data);
}

bool NetHandlers::NetMsg_JsonRead(uint32_t type, JsonRead& jsonbuf, SourceGameContext& context, void* data)
{
    static const NetMsgJsonReadFn netHandlers[] = DECLARE_NET_HANDLER_ARRAY(JsonRead);
    return netHandlers[type](jsonbuf, context, data);
}

bool NetHandlers::NetMsg_JsonWrite(uint32_t type, JsonWrite& jsonbuf, const SourceGameContext& context, void* data)
{
    static const NetMsgJsonWriteFn netHandlers[] = DECLARE_NET_HANDLER_ARRAY(JsonWrite);
    return netHandlers[type](jsonbuf, context, data);
}

void NetHandlers::NetMsg_ToString(uint32_t type, std::ostringstream& out, void* data)
{
    static const NetMsgToStringFn netHandlers[] = DECLARE_NET_HANDLER_ARRAY(ToString);
    netHandlers[type](out, data);
}
