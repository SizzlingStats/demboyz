
#pragma once

#include "nethandlers.h"

namespace NetMsg
{
    struct SVC_ServerInfo
    {
        int16_t protocol;           // protocol version
        uint32_t serverCount;       // number of changelevels since server start
        bool isHLTV;                // HLTV server ?
        bool isDedicated;           // dedicated server ?
        uint32_t clientCRC;         // client.dll CRC server is using
        uint16_t maxClasses;        // max number of server classes
        uint32_t mapCRC;            // server map CRC
        uint8_t unk1[16];
        uint8_t playerSlot;         // our client slot number
        uint8_t maxClients;         // max number of clients on server
        float tickInterval;         // server tick interval
        char os;                    // 'l' = linux, 'w' = Win32
        char gameDir[MAX_OSPATH];   // game directory eg "tf2"
        char mapName[MAX_OSPATH];   // name of current map
        char skyName[MAX_OSPATH];   // name of current skybox
        char hostName[MAX_OSPATH];  // host name
        bool unk2;
    };
}

DECLARE_NET_HANDLERS(SVC_ServerInfo);
