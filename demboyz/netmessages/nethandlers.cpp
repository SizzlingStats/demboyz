
#include "nethandlers.h"
#include "netmessages.h"
#include <cassert>
#include <cstdint>

#if !defined( MAX_OSPATH )
#define MAX_OSPATH      260         // max length of a filesystem pathname
#endif

#include "net_nop.h"
#include "net_disconnect.h"
#include "net_file.h"
#include "net_tick.h"
#include "net_stringcmd.h"
#include "net_setconvar.h"
#include "net_signonstate.h"
#include "svc_print.h"
#include "svc_serverinfo.h"
#include "svc_sendtable.h"
#include "svc_classinfo.h"
#include "svc_setpause.h"
#include "svc_createstringtable.h"
#include "svc_updatestringtable.h"
#include "svc_voiceinit.h"
#include "svc_voicedata.h"
#include "svc_hltv.h"
#include "svc_sounds.h"
#include "svc_setview.h"
#include "svc_fixangle.h"
#include "svc_crosshairangle.h"
#include "svc_bspdecal.h"
#include "svc_terrainmod.h"
#include "svc_usermessage.h"
#include "svc_entitymessage.h"
#include "svc_gameevent.h"
#include "svc_packetentities.h"
#include "svc_tempentities.h"
#include "svc_prefetch.h"
#include "svc_menu.h"
#include "svc_gameeventlist.h"
#include "svc_getcvarvalue.h"

void NetHandlers::CreateNetMsgStructs(NetDataStructArray& netDataStructs)
{
    netDataStructs[0] = new NetMsg::Net_NOP();
    netDataStructs[1] = new NetMsg::Net_Disconnect();
    netDataStructs[2] = new NetMsg::Net_File();
    netDataStructs[3] = new NetMsg::Net_Tick();
    netDataStructs[4] = new NetMsg::Net_StringCmd();
    netDataStructs[5] = new NetMsg::Net_SetConVar();
    netDataStructs[6] = new NetMsg::Net_SignonState();
    netDataStructs[7] = new NetMsg::SVC_Print();
    netDataStructs[8] = new NetMsg::SVC_ServerInfo();
    netDataStructs[9] = new NetMsg::SVC_SendTable();
    netDataStructs[10] = new NetMsg::SVC_ClassInfo();
    netDataStructs[11] = new NetMsg::SVC_SetPause();
    netDataStructs[12] = new NetMsg::SVC_CreateStringTable();
    netDataStructs[13] = new NetMsg::SVC_UpdateStringTable();
    netDataStructs[14] = new NetMsg::SVC_VoiceInit();
    netDataStructs[15] = new NetMsg::SVC_VoiceData();
    netDataStructs[16] = new NetMsg::SVC_HLTV();
    netDataStructs[17] = new NetMsg::SVC_Sounds();
    netDataStructs[18] = new NetMsg::SVC_SetView();
    netDataStructs[19] = new NetMsg::SVC_FixAngle();
    netDataStructs[20] = new NetMsg::SVC_CrosshairAngle();
    netDataStructs[21] = new NetMsg::SVC_BSPDecal();
    netDataStructs[22] = new NetMsg::SVC_TerrainMod();
    netDataStructs[23] = new NetMsg::SVC_UserMessage();
    netDataStructs[24] = new NetMsg::SVC_EntityMessage();
    netDataStructs[25] = new NetMsg::SVC_GameEvent();
    netDataStructs[26] = new NetMsg::SVC_PacketEntities();
    netDataStructs[27] = new NetMsg::SVC_TempEntities();
    netDataStructs[28] = new NetMsg::SVC_Prefetch();
    netDataStructs[29] = new NetMsg::SVC_Menu();
    netDataStructs[30] = new NetMsg::SVC_GameEventList();
    netDataStructs[31] = new NetMsg::SVC_GetCvarValue();
}

void NetHandlers::DestroyNetMsgStructs(NetDataStructArray& netDataStructs)
{
    delete reinterpret_cast<NetMsg::Net_NOP*>(netDataStructs[0]);
    delete reinterpret_cast<NetMsg::Net_Disconnect*>(netDataStructs[1]);
    delete reinterpret_cast<NetMsg::Net_File*>(netDataStructs[2]);
    delete reinterpret_cast<NetMsg::Net_Tick*>(netDataStructs[3]);
    delete reinterpret_cast<NetMsg::Net_StringCmd*>(netDataStructs[4]);
    delete reinterpret_cast<NetMsg::Net_SetConVar*>(netDataStructs[5]);
    delete reinterpret_cast<NetMsg::Net_SignonState*>(netDataStructs[6]);
    delete reinterpret_cast<NetMsg::SVC_Print*>(netDataStructs[7]);
    delete reinterpret_cast<NetMsg::SVC_ServerInfo*>(netDataStructs[8]);
    delete reinterpret_cast<NetMsg::SVC_SendTable*>(netDataStructs[9]);
    delete reinterpret_cast<NetMsg::SVC_ClassInfo*>(netDataStructs[10]);
    delete reinterpret_cast<NetMsg::SVC_SetPause*>(netDataStructs[11]);
    delete reinterpret_cast<NetMsg::SVC_CreateStringTable*>(netDataStructs[12]);
    delete reinterpret_cast<NetMsg::SVC_UpdateStringTable*>(netDataStructs[13]);
    delete reinterpret_cast<NetMsg::SVC_VoiceInit*>(netDataStructs[14]);
    delete reinterpret_cast<NetMsg::SVC_VoiceData*>(netDataStructs[15]);
    delete reinterpret_cast<NetMsg::SVC_HLTV*>(netDataStructs[16]);
    delete reinterpret_cast<NetMsg::SVC_Sounds*>(netDataStructs[17]);
    delete reinterpret_cast<NetMsg::SVC_SetView*>(netDataStructs[18]);
    delete reinterpret_cast<NetMsg::SVC_FixAngle*>(netDataStructs[19]);
    delete reinterpret_cast<NetMsg::SVC_CrosshairAngle*>(netDataStructs[20]);
    delete reinterpret_cast<NetMsg::SVC_BSPDecal*>(netDataStructs[21]);
    delete reinterpret_cast<NetMsg::SVC_TerrainMod*>(netDataStructs[22]);
    delete reinterpret_cast<NetMsg::SVC_UserMessage*>(netDataStructs[23]);
    delete reinterpret_cast<NetMsg::SVC_EntityMessage*>(netDataStructs[24]);
    delete reinterpret_cast<NetMsg::SVC_GameEvent*>(netDataStructs[25]);
    delete reinterpret_cast<NetMsg::SVC_PacketEntities*>(netDataStructs[26]);
    delete reinterpret_cast<NetMsg::SVC_TempEntities*>(netDataStructs[27]);
    delete reinterpret_cast<NetMsg::SVC_Prefetch*>(netDataStructs[28]);
    delete reinterpret_cast<NetMsg::SVC_Menu*>(netDataStructs[29]);
    delete reinterpret_cast<NetMsg::SVC_GameEventList*>(netDataStructs[30]);
    delete reinterpret_cast<NetMsg::SVC_GetCvarValue*>(netDataStructs[31]);
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

typedef bool (*NetMsgBitReadFn)(NetHandlers::BitRead& bitbuf, SourceGameContext& context, void* data);
typedef bool (*NetMsgBitWriteFn)(NetHandlers::BitWrite& bitbuf, const SourceGameContext& context, void* data);
typedef bool (*NetMsgJsonReadFn)(NetHandlers::JsonRead& jsonbuf, SourceGameContext& context, void* data);
typedef bool (*NetMsgJsonWriteFn)(NetHandlers::JsonWrite& jsonbuf, const SourceGameContext& context, void* data);
typedef void (*NetMsgToStringFn)(std::ostringstream& out, void* data);

template<typename BufType, typename FnType, typename ContextType, size_t NumHandlers>
bool NetMsgFuncRunner(const FnType (&netHandlers)[NumHandlers], uint32_t type, BufType& buf, ContextType& context, void* data)
{
    if (type >= NumHandlers)
    {
        return false;
    }
    return netHandlers[type](buf, context, data);
}

bool NetHandlers::NetMsg_BitRead(uint32_t type, BitRead& bitbuf, SourceGameContext& context, void* data)
{
    static const NetMsgBitReadFn netHandlers[] = DECLARE_NET_HANDLER_ARRAY(BitRead);
    return NetMsgFuncRunner(netHandlers, type, bitbuf, context, data);
}

bool NetHandlers::NetMsg_BitWrite(uint32_t type, BitWrite& bitbuf, const SourceGameContext& context, void* data)
{
    static const NetMsgBitWriteFn netHandlers[] = DECLARE_NET_HANDLER_ARRAY(BitWrite);
    return NetMsgFuncRunner(netHandlers, type, bitbuf, context, data);
}

bool NetHandlers::NetMsg_JsonRead(uint32_t type, JsonRead& jsonbuf, SourceGameContext& context, void* data)
{
    static const NetMsgJsonReadFn netHandlers[] = DECLARE_NET_HANDLER_ARRAY(JsonRead);
    return NetMsgFuncRunner(netHandlers, type, jsonbuf, context, data);
}

bool NetHandlers::NetMsg_JsonWrite(uint32_t type, JsonWrite& jsonbuf, const SourceGameContext& context, void* data)
{
    static const NetMsgJsonWriteFn netHandlers[] = DECLARE_NET_HANDLER_ARRAY(JsonWrite);
    return NetMsgFuncRunner(netHandlers, type, jsonbuf, context, data);
}

void NetHandlers::NetMsg_ToString(uint32_t type, std::ostringstream& out, void* data)
{
    static const NetMsgToStringFn netHandlers[] = DECLARE_NET_HANDLER_ARRAY(ToString);
    if (type >= (sizeof(netHandlers) / sizeof(NetMsgToStringFn)))
    {
        return;
    }
    netHandlers[type](out, data);
}
