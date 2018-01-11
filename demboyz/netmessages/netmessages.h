
#pragma once

namespace NetMsg
{
    enum
    {
        net_NOP = 0,                // nop command used for padding
        net_Disconnect = 1,         // disconnect, last message in connection
        net_File = 2,               // file transmission message request/deny

        net_Tick = 3,               // send last world tick
        net_StringCmd = 4,          // a string command
        net_SetConVar = 5,          // sends one/multiple convar settings
        net_SignonState = 6,        // signals current signon state

        //
        // server to client
        //

        svc_Print = 7,              // print text to console
        svc_ServerInfo = 8,         // first message from server about game, map etc
        svc_SendTable = 9,          // sends a sendtable description for a game class
        svc_ClassInfo = 10,         // Info about classes (first byte is a CLASSINFO_ define).
        svc_SetPause = 11,          // tells client if server paused or unpaused


        svc_CreateStringTable = 12, // inits shared string tables
        svc_UpdateStringTable = 13, // updates a string table

        svc_VoiceInit = 14,         // inits used voice codecs & quality
        svc_VoiceData = 15,         // Voicestream data from the server

        //svc_HLTV = 16,            // HLTV control messages

        svc_Sounds = 17,            // starts playing sound

        svc_SetView = 18,           // sets entity as point of view
        svc_FixAngle = 19,          // sets/corrects players viewangle
        svc_CrosshairAngle = 20,    // adjusts crosshair in auto aim mode to lock on traget

        svc_BSPDecal = 21,          // add a static decal to the world BSP

        // NOTE: This is now unused!
        // svc_TerrainMod = 22,     // modification to the terrain/displacement

        // Message from server side to client side entity
        svc_UserMessage = 23,       // a game specific message 
        svc_EntityMessage = 24,     // a message for an entity
        svc_GameEvent = 25,         // global game event fired

        svc_PacketEntities = 26,    // non-delta compressed entities

        svc_TempEntities = 27,      // non-reliable event object

        svc_Prefetch = 28,          // only sound indices for now

        svc_Menu = 29,              // display a menu from a plugin

        svc_GameEventList = 30,     // list of known games events and fields

        svc_GetCvarValue = 31,      // Server wants to know the value of a cvar on the client.

        svc_CmdKeyValues = 32,
        svc_SetPauseTimed = 33,

        SVC_LASTMSG = 33            // last known server messages
    };
}
