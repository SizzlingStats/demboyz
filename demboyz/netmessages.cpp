
#include "netmessages.h"
#include "demofilebitbuf.h"
#include <cassert>

namespace math
{
    unsigned int log2(unsigned int value)
    {
        unsigned int res = 0;
        while (value >>= 1)
            ++res;
        return res;
    }

    unsigned int Bits2Bytes(unsigned int bits)
    {
        return ((bits + 7) >> 3);
    }
}

void Net_NOP(CBitRead& bitbuf)
{
    // nothing
}

void Net_Disconnect(CBitRead& bitbuf)
{
    char message[1024];
    bitbuf.ReadString(message, sizeof(message));
}

void Net_File(CBitRead& bitbuf)
{
    const unsigned int transferID = bitbuf.ReadUBitLong(32);
    char filename[1024];
    bitbuf.ReadString(filename, sizeof(filename));
    const bool isRequest = bitbuf.ReadOneBit() != 0;
}

// verified
void Net_Tick(CBitRead& bitbuf)
{
    static const float NET_TICK_SCALEUP = 100000.0f;

    const int tick = bitbuf.ReadLong();
    const float hostFrameTime = (float)bitbuf.ReadUBitLong(16) / NET_TICK_SCALEUP;
    const float hostFrameTimeStdDev = (float)bitbuf.ReadUBitLong(16) / NET_TICK_SCALEUP;
}

void Net_StringCmd(CBitRead& bitbuf)
{
    char commandBuffer[1024];
    bitbuf.ReadString(commandBuffer, sizeof(commandBuffer));
}

// verified
void Net_SetConVar(CBitRead& bitbuf)
{
    typedef struct cvar_s
    {
        char name[MAX_OSPATH];
        char value[MAX_OSPATH];
    } cvar_t;

    const int numVars = bitbuf.ReadByte();
    cvar_t cvar;
    for (int i = 0; i < numVars; ++i)
    {
        bitbuf.ReadString(cvar.name, sizeof(cvar.name));
        bitbuf.ReadString(cvar.value, sizeof(cvar.value));
    }
}

// verified
void Net_SignonState(CBitRead& bitbuf)
{
    const int signonState = bitbuf.ReadByte();
    assert(signonState >= SIGNONSTATE_NONE &&
           signonState <= SIGNONSTATE_CHANGELEVEL);
    const int spawnCount = bitbuf.ReadLong();
}

void SVC_Print(CBitRead& bitbuf)
{
    char textBuffer[2048];
    bitbuf.ReadString(textBuffer, sizeof(textBuffer));
}

// verified
void SVC_ServerInfo(CBitRead& bitbuf)
{
    const int protocol = bitbuf.ReadShort();            // protocol version
    const int serverCount = bitbuf.ReadLong();          // number of changelevels since server start
    const bool isHLTV = bitbuf.ReadOneBit() != 0;       // HLTV server ?
    const bool isDedicated = bitbuf.ReadOneBit() != 0;  // dedicated server ?
    const int clientCRC = bitbuf.ReadLong();            // client.dll CRC server is using
    const int maxClasses = bitbuf.ReadWord();           // max number of server classes

    if (protocol <= 17)
    {
        const int mapCRC = bitbuf.ReadLong();           // server map CRC
    }
    else
    {
        char unknown[16];
        bitbuf.ReadBytes(unknown, sizeof(unknown));
    }
    const int playerSlot = bitbuf.ReadByte();           // our client slot number
    const int maxClients = bitbuf.ReadByte();           // max number of clients on server
    const float tickInterval = bitbuf.ReadFloat();      // server tick interval
    const char os = bitbuf.ReadChar();                  // 'l' = linux, 'w' = Win32

    char gameDir[MAX_OSPATH];   // game directory eg "tf2"
    char mapName[MAX_OSPATH];   // name of current map 
    char skyName[MAX_OSPATH];   // name of current skybox 
    char hostName[MAX_OSPATH];  // host name
    bitbuf.ReadString(gameDir, sizeof(gameDir));
    bitbuf.ReadString(mapName, sizeof(mapName));
    bitbuf.ReadString(skyName, sizeof(skyName));
    bitbuf.ReadString(hostName, sizeof(hostName));
    
    // TODO:
    // if (protocol > 15)
    // {
    const bool unknown = bitbuf.ReadOneBit() != 0;
    // }
}

void SVC_SendTable(CBitRead& bitbuf)
{
    const bool needsDecoder = bitbuf.ReadOneBit() != 0;
    const int dataLengthBits = bitbuf.ReadShort();
    bitbuf.SeekRelative(dataLengthBits);
}

// verified
void SVC_ClassInfo(CBitRead& bitbuf)
{
    typedef struct class_s
    {
        int     classID;
        char    datatablename[256];
        char    classname[256];
    } class_t;

    const int numServerClasses = bitbuf.ReadShort();
    const int nServerClassBits = math::log2(numServerClasses) + 1;
    // if true, client creates own SendTables & classinfos from game.dll
    const bool createOnClient = bitbuf.ReadOneBit() != 0;
    if (!createOnClient)
    {
        class_t serverclass;
        for (int i = 0; i < numServerClasses; ++i)
        {
            serverclass.classID = bitbuf.ReadUBitLong(nServerClassBits);
            bitbuf.ReadString(serverclass.classname, sizeof(serverclass.classname));
            bitbuf.ReadString(serverclass.datatablename, sizeof(serverclass.datatablename));
        }
    }
}

void SVC_SetPause(CBitRead& bitbuf)
{
    const bool paused = bitbuf.ReadOneBit() != 0;
}

// verified
void SVC_CreateStringTable(CBitRead& bitbuf)
{
    if (bitbuf.PeekUBitLong(8) == ':')
    {
        const bool isFilenames = true;
        bitbuf.ReadByte();
    }
    else
    {
        const bool isFilenames = false;
    }

    char tableName[256];
    bitbuf.ReadString(tableName, sizeof(tableName));
    const int maxEntries = bitbuf.ReadWord();
    const int encodeBits = math::log2(maxEntries);
    const int numEntries = bitbuf.ReadUBitLong(encodeBits + 1);

    // TODO:
    // if (protocol > 23)
    // {
    const uint32 lengthInBits = bitbuf.ReadVarInt32();
    // }
    // else
    // {
    //     const int lengthInBits = bitbuf.ReadUBitLong(NET_MAX_PALYLOAD_BITS + 1);
    // }
    
    const bool userDataFixedSize = bitbuf.ReadOneBit() != 0;
    if (userDataFixedSize)
    {
        const int userDataSize = bitbuf.ReadUBitLong(12);
        const int userDataSizeBits = bitbuf.ReadUBitLong(4);
    }
    else
    {
        const int userDataSize = 0;
        const int userDataSizeBits = 0;
    }

    // TODO:
    // if (protocol > 14)
    // {
    const bool unk1 = bitbuf.ReadOneBit() != 0;
    // }
    // else
    // {
    //     const bool unk1 = false;
    // }
    bitbuf.SeekRelative(lengthInBits);
}

void SVC_UpdateStringTable(CBitRead& bitbuf)
{
    const int tableId = bitbuf.ReadUBitLong(math::log2(MAX_TABLES));
    const int numChangedEntries = (bitbuf.ReadOneBit() != 0) ? bitbuf.ReadWord() : 1;
    const int lengthInBits = bitbuf.ReadUBitLong(20);
    bitbuf.SeekRelative(lengthInBits);
}

// verified
void SVC_VoiceInit(CBitRead& bitbuf)
{
    char voiceCodec[MAX_OSPATH]; // used voice codec .dll
    bitbuf.ReadString(voiceCodec, sizeof(voiceCodec));
    const int quality = bitbuf.ReadByte(); // custom quality setting
}

void SVC_VoiceData(CBitRead& bitbuf)
{
    const int fromClientIndex = bitbuf.ReadByte();
    const bool proximity = !!bitbuf.ReadByte();
    const int lengthInBits = bitbuf.ReadWord();
    bitbuf.SeekRelative(lengthInBits);
}

void SVC_HLTV(CBitRead& bitbuf)
{
    //const int state = bitbuf.ReadByte();
    assert(false);
}

void SVC_Sounds(CBitRead& bitbuf)
{
    const bool reliableSound = bitbuf.ReadOneBit() != 0;
    int numSounds;
    int lengthInBits;
    if (reliableSound)
    {
        numSounds = 1;
        lengthInBits = bitbuf.ReadUBitLong(8);
    }
    else
    {
        numSounds = bitbuf.ReadUBitLong(8);
        lengthInBits = bitbuf.ReadUBitLong(16);
    }
    bitbuf.SeekRelative(lengthInBits);
}

void SVC_SetView(CBitRead& bitbuf)
{
    const int entIndex = bitbuf.ReadUBitLong(MAX_EDICT_BITS);
}

void SVC_FixAngle(CBitRead& bitbuf)
{
    const bool relative = bitbuf.ReadOneBit() != 0;
    const float x = bitbuf.ReadBitAngle(16);
    const float y = bitbuf.ReadBitAngle(16);
    const float z = bitbuf.ReadBitAngle(16);
}

void SVC_CrosshairAngle(CBitRead& bitbuf)
{
    const float x = bitbuf.ReadBitAngle(16);
    const float y = bitbuf.ReadBitAngle(16);
    const float z = bitbuf.ReadBitAngle(16);
}

void SVC_BSPDecal(CBitRead& bitbuf)
{
    Vector pos;
    bitbuf.ReadBitVec3Coord(pos);
    const int decalTextureIndex = bitbuf.ReadUBitLong(MAX_DECAL_INDEX_BITS);
    if (bitbuf.ReadOneBit() != 0)
    {
        const int entIndex = bitbuf.ReadUBitLong(MAX_EDICT_BITS);
        const int modelIndex = bitbuf.ReadUBitLong(SP_MODEL_INDEX_BITS);
    }
    else
    {
        const int entIndex = 0;
        const int modelIndex = 0;
    }
    const bool lowPriority = bitbuf.ReadOneBit() != 0;
}

void SVC_TerrainMod(CBitRead& bitbuf)
{
    assert(false);
}

void SVC_UserMessage(CBitRead& bitbuf)
{
    const int msgType = bitbuf.ReadByte();

    // max 256 * 8 bits, see MAX_USER_MSG_DATA
    const int lengthInBits = bitbuf.ReadUBitLong(11);
    bitbuf.SeekRelative(lengthInBits);
}

void SVC_EntityMessage(CBitRead& bitbuf)
{
    const int entIndex = bitbuf.ReadUBitLong(MAX_EDICT_BITS);
    const int classID = bitbuf.ReadUBitLong(MAX_SERVER_CLASS_BITS);

    // max 256 * 8 bits
    const int lengthInBits = bitbuf.ReadUBitLong(11);
    bitbuf.SeekRelative(lengthInBits);
}

void SVC_GameEvent(CBitRead& bitbuf)
{
    const int lengthInBits = bitbuf.ReadUBitLong(11);
    //uint32 eventId = bitbuf.ReadUBitLong(9);
    bitbuf.SeekRelative(lengthInBits);
}

void SVC_PacketEntities(CBitRead& bitbuf)
{
    const int maxEntries = bitbuf.ReadUBitLong(MAX_EDICT_BITS);
    const bool isDelta = bitbuf.ReadOneBit() != 0;
    if (isDelta)
    {
        const int deltaFrom = bitbuf.ReadLong();
    }
    else
    {
        const int deltaFrom = -1;
    }
    const int numBaseline = bitbuf.ReadUBitLong(1);
    const int numUpdatedEntries = bitbuf.ReadUBitLong(MAX_EDICT_BITS);
    const int lengthInBits = bitbuf.ReadUBitLong(DELTASIZE_BITS);
    const bool updateBaseline = bitbuf.ReadOneBit() != 0;
    bitbuf.SeekRelative(lengthInBits);
}

void SVC_TempEntities(CBitRead& bitbuf)
{
    const int numEntries = bitbuf.ReadUBitLong(EVENT_INDEX_BITS);
    const int lengthInBits = bitbuf.ReadUBitLong(NET_MAX_PALYLOAD_BITS);
    bitbuf.SeekRelative(lengthInBits);
}

void SVC_Prefetch(CBitRead& bitbuf)
{
    enum
    {
        SOUND = 0,
    };

    const short type = SOUND; // bitbuf.ReadUBitLong(1);
    const short soundIndex = bitbuf.ReadUBitLong(MAX_SOUND_INDEX_BITS);
}

void SVC_Menu(CBitRead& bitbuf)
{
    typedef enum
    {
        DIALOG_MSG = 0,		// just an on screen message
        DIALOG_MENU,		// an options menu
        DIALOG_TEXT,		// a richtext dialog
        DIALOG_ENTRY,		// an entry box
        DIALOG_ASKCONNECT	// Ask the client to connect to a specified IP address. Only the "time" and "title" keys are used.
    } DIALOG_TYPE;

    DIALOG_TYPE type = (DIALOG_TYPE)bitbuf.ReadShort();
    const int lengthInBytes = bitbuf.ReadWord();
    bitbuf.SeekRelative(lengthInBytes * 8);
}

void SVC_GameEventList(CBitRead& bitbuf)
{
    const int numEvents = bitbuf.ReadUBitLong(MAX_EVENT_BITS);
    for (int i = 0; i < numEvents; ++i)
    {
        const int id = bitbuf.ReadUBitLong(MAX_EVENT_BITS);
        char name[MAX_EVENT_NAME_LENGTH];
        bitbuf.ReadString(name, sizeof(name));
        printf("%s\n", name);
        assert(false);
        // gameeventmanager.cpp ParseEventList
    }
    const int lengthInBits = bitbuf.ReadUBitLong(20);
    bitbuf.SeekRelative(lengthInBits);
}

void SVC_GetCvarValue(CBitRead& bitbuf)
{
    typedef int QueryCvarCookie_t;
    QueryCvarCookie_t cookie = bitbuf.ReadSBitLong(32);
    char cvarName[256];
    bitbuf.ReadString(cvarName, sizeof(cvarName));
}

static const int NUM_MESSAGES = static_cast<uint8_t>(NetMsg::SVC_LASTMSG) + 1;

void ProcessNetMsg(const std::uint32_t msgType, CBitRead& bitbuf)
{
    static NetMsgFn netMsgHandler[NUM_MESSAGES] =
    {
        &Net_NOP,
        &Net_Disconnect,
        &Net_File,
        &Net_Tick,
        &Net_StringCmd,
        &Net_SetConVar,
        &Net_SignonState,

        &SVC_Print,
        &SVC_ServerInfo,
        &SVC_SendTable,
        &SVC_ClassInfo,
        &SVC_SetPause,
        &SVC_CreateStringTable,
        &SVC_UpdateStringTable,
        &SVC_VoiceInit,
        &SVC_VoiceData,
        &SVC_HLTV,
        &SVC_Sounds,
        &SVC_SetView,
        &SVC_FixAngle,
        &SVC_CrosshairAngle,
        &SVC_BSPDecal,
        &SVC_TerrainMod,
        &SVC_UserMessage,
        &SVC_EntityMessage,
        &SVC_GameEvent,
        &SVC_PacketEntities,
        &SVC_TempEntities,
        &SVC_Prefetch,
        &SVC_Menu,
        &SVC_GameEventList,
        &SVC_GetCvarValue
    };

    assert(msgType < NUM_MESSAGES);
    netMsgHandler[msgType](bitbuf);
}
