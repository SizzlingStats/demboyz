
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
        bool msgname##_BitRead_Internal(bf_read& bitbuf, SourceGameContext& context, NetMsg::msgname* data); \
        bool msgname##_BitWrite_Internal(bf_write& bitbuf, const SourceGameContext& context, NetMsg::msgname* data); \
        bool msgname##_JsonRead_Internal(JsonRead& jsonbuf, SourceGameContext& context, NetMsg::msgname* data); \
        bool msgname##_JsonWrite_Internal(JsonWrite& jsonbuf, const SourceGameContext& context, NetMsg::msgname* data); \
        void msgname##_ToString_Internal(std::ostringstream& out, NetMsg::msgname* data); \
        inline bool msgname##_BitRead(bf_read& bitbuf, SourceGameContext& context, void* data) \
        { \
            return msgname##_BitRead_Internal(bitbuf, context, reinterpret_cast<NetMsg::msgname*>(data)); \
        } \
        inline bool msgname##_BitWrite(bf_write& bitbuf, const SourceGameContext& context, void* data) \
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
    bool NetMsg_BitRead(uint32_t type, bf_read& bitbuf, SourceGameContext& context, void* data);
    bool NetMsg_BitWrite(uint32_t type, bf_write& bitbuf, const SourceGameContext& context, void* data);
    bool NetMsg_JsonRead(uint32_t type, JsonRead& jsonbuf, SourceGameContext& context, void* data);
    bool NetMsg_JsonWrite(uint32_t type, JsonWrite& jsonbuf, const SourceGameContext& context, void* data);
    void NetMsg_ToString(uint32_t type, std::ostringstream& out, void* data);
}
