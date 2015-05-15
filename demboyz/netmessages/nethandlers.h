
#pragma once

#include <cstdint>
#include <sstream>

class bf_read;
class bf_write;
class JsonRead;
class JsonWrite;

struct SourceGameContext
{
    int16_t protocol;
};

#if !defined(MAX_OSPATH)
#define MAX_OSPATH 260 // max length of a filesystem pathname
#endif

#define DECLARE_NET_HANDLERS(msgname) \
    namespace NetHandlers \
    { \
        bool msgname##_BitRead_Internal(bf_read& bitbuf, SourceGameContext& context, NetMsg::##msgname* data); \
        bool msgname##_BitWrite_Internal(bf_write& bitbuf, SourceGameContext& context, NetMsg::##msgname* data); \
        bool msgname##_JsonRead_Internal(JsonRead& jsonbuf, SourceGameContext& context, NetMsg::##msgname* data); \
        bool msgname##_JsonWrite_Internal(JsonWrite& jsonbuf, SourceGameContext& context, NetMsg::##msgname* data); \
        void msgname##_ToString_Internal(std::ostringstream& out, NetMsg::##msgname* data); \
        inline bool msgname##_BitRead(bf_read& bitbuf, SourceGameContext& context, void* data) \
        { \
            return msgname##_BitRead_Internal(bitbuf, context, reinterpret_cast<NetMsg::##msgname*>(data)); \
        } \
        inline bool msgname##_BitWrite(bf_write& bitbuf, SourceGameContext& context, void* data) \
        { \
            return msgname##_BitWrite_Internal(bitbuf, context, reinterpret_cast<NetMsg::##msgname*>(data)); \
        } \
        inline bool msgname##_JsonRead(JsonRead& jsonbuf, SourceGameContext& context, void* data) \
        { \
            return msgname##_JsonRead_Internal(jsonbuf, context, reinterpret_cast<NetMsg::##msgname*>(data)); \
        } \
        inline bool msgname##_JsonWrite(JsonWrite& jsonbuf, SourceGameContext& context, void* data) \
        { \
            return msgname##_JsonWrite_Internal(jsonbuf, context, reinterpret_cast<NetMsg::##msgname*>(data)); \
        } \
        inline void msgname##_ToString(std::ostringstream& out, void* data) \
        { \
            msgname##_ToString_Internal(out, reinterpret_cast<NetMsg::##msgname*>(data)); \
        } \
    }

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
