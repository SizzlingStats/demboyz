
#include "svc_usermessage.h"
#include "sourcesdk/bitbuf.h"
#include "netmath.h"
#include "netcontants.h"
#include <cassert>

namespace NetHandlers
{
    bool SVC_UserMessage_BitRead_Internal(BitRead& bitbuf, SourceGameContext& context, NetMsg::SVC_UserMessage* data)
    {
        data->msgType = bitbuf.ReadByte();
        data->dataLengthInBits = bitbuf.ReadUBitLong(11);
        assert(math::BitsToBytes(data->dataLengthInBits) <= MAX_USER_MSG_DATA);
        data->data.reset(new uint8_t[math::BitsToBytes(data->dataLengthInBits)]);
        bitbuf.ReadBits(data->data.get(), data->dataLengthInBits);
        return !bitbuf.IsOverflowed();
    }

    bool SVC_UserMessage_BitWrite_Internal(BitWrite& bitbuf, const SourceGameContext& context, NetMsg::SVC_UserMessage* data)
    {
        bitbuf.WriteByte(data->msgType);
        bitbuf.WriteUBitLong(data->dataLengthInBits, 11);
        bitbuf.WriteBits(data->data.get(), data->dataLengthInBits);
        return !bitbuf.IsOverflowed();
    }

    bool SVC_UserMessage_JsonRead_Internal(JsonRead& jsonbuf, SourceGameContext& context, NetMsg::SVC_UserMessage* data)
    {
        return true;
    }

    bool SVC_UserMessage_JsonWrite_Internal(JsonWrite& jsonbuf, const SourceGameContext& context, NetMsg::SVC_UserMessage* data)
    {
        return true;
    }

    void SVC_UserMessage_ToString_Internal(std::ostringstream& out, NetMsg::SVC_UserMessage* data)
    {
        out << "svc_UserMessage: type " << static_cast<uint32_t>(data->msgType)
            << ", bytes " << math::BitsToBytes(data->dataLengthInBits);
    }
}
