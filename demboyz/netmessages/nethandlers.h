
#pragma once

#include <cstdint>
#include <sstream>
#include <array>
#include "netmessages.h"

namespace base
{
    class JsonReaderFile;
    class JsonWriterFile;
    class BitFileRead;
    class BitFileWrite;
}

class bf_read;
class bf_write;

namespace NetHandlers
{
    using BitRead = bf_read;
    using BitWrite = bf_write;
    using JsonRead = base::JsonReaderFile;
    using JsonWrite = base::JsonWriterFile;
}

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
        bool msgname##_BitRead_Internal(BitRead& bitbuf, SourceGameContext& context, NetMsg::msgname* data); \
        bool msgname##_BitWrite_Internal(BitWrite& bitbuf, const SourceGameContext& context, NetMsg::msgname* data); \
        bool msgname##_JsonRead_Internal(JsonRead& jsonbuf, SourceGameContext& context, NetMsg::msgname* data); \
        bool msgname##_JsonWrite_Internal(JsonWrite& jsonbuf, const SourceGameContext& context, NetMsg::msgname* data); \
        void msgname##_ToString_Internal(std::ostringstream& out, NetMsg::msgname* data); \
        inline bool msgname##_BitRead(BitRead& bitbuf, SourceGameContext& context, void* data) \
        { \
            return msgname##_BitRead_Internal(bitbuf, context, reinterpret_cast<NetMsg::msgname*>(data)); \
        } \
        inline bool msgname##_BitWrite(BitWrite& bitbuf, const SourceGameContext& context, void* data) \
        { \
            return msgname##_BitWrite_Internal(bitbuf, context, reinterpret_cast<NetMsg::msgname*>(data)); \
        } \
        inline bool msgname##_JsonRead(JsonRead& jsonbuf, SourceGameContext& context, void* data) \
        { \
            return msgname##_JsonRead_Internal(jsonbuf, context, reinterpret_cast<NetMsg::msgname*>(data)); \
        } \
        inline bool msgname##_JsonWrite(JsonWrite& jsonbuf, const SourceGameContext& context, void* data) \
        { \
            return msgname##_JsonWrite_Internal(jsonbuf, context, reinterpret_cast<NetMsg::msgname*>(data)); \
        } \
        inline void msgname##_ToString(std::ostringstream& out, void* data) \
        { \
            msgname##_ToString_Internal(out, reinterpret_cast<NetMsg::msgname*>(data)); \
        } \
    }

namespace NetHandlers
{
    using NetDataStructArray = std::array<void*, NetMsg::SVC_LASTMSG + 1>;
    void CreateNetMsgStructs(NetDataStructArray& netDataStructs);
    void DestroyNetMsgStructs(NetDataStructArray& netDataStructs);

    bool NetMsg_BitRead(uint32_t type, BitRead& bitbuf, SourceGameContext& context, void* data);
    bool NetMsg_BitWrite(uint32_t type, BitWrite& bitbuf, const SourceGameContext& context, void* data);
    bool NetMsg_JsonRead(uint32_t type, JsonRead& jsonbuf, SourceGameContext& context, void* data);
    bool NetMsg_JsonWrite(uint32_t type, JsonWrite& jsonbuf, const SourceGameContext& context, void* data);
    void NetMsg_ToString(uint32_t type, std::ostringstream& out, void* data);
}
