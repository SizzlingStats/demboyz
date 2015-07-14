
#include "svc_usermessage.h"
#include "base/bitfile.h"
#include "base/jsonfile.h"
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
        base::JsonReaderObject reader = jsonbuf.ParseObject();
        assert(!reader.HasReadError());
        data->msgType = reader.ReadUInt32("msgType");
        data->dataLengthInBits = reader.ReadUInt32("dataLengthInBits");
        data->data.reset(new uint8_t[math::BitsToBytes(data->dataLengthInBits)]);
        reader.ReadBits("data", data->data.get(), data->dataLengthInBits);
        return !reader.HasReadError();
    }

    bool SVC_UserMessage_JsonWrite_Internal(JsonWrite& jsonbuf, const SourceGameContext& context, NetMsg::SVC_UserMessage* data)
    {
        jsonbuf.Reset();
        jsonbuf.StartObject();
        jsonbuf.WriteUInt32("msgType", data->msgType);
        jsonbuf.WriteUInt32("dataLengthInBits", data->dataLengthInBits);
        jsonbuf.WriteBits("data", data->data.get(), data->dataLengthInBits);
        jsonbuf.EndObject();
        return jsonbuf.IsComplete();
    }

    void SVC_UserMessage_ToString_Internal(std::ostringstream& out, NetMsg::SVC_UserMessage* data)
    {
        out << "svc_UserMessage: type " << static_cast<uint32_t>(data->msgType)
            << ", bytes " << math::BitsToBytes(data->dataLengthInBits);
    }
}
