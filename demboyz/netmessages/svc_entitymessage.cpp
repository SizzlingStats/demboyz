
#include "svc_entitymessage.h"
#include "base/bitfile.h"
#include "base/jsonfile.h"
#include "netcontants.h"
#include "netmath.h"

namespace NetHandlers
{
    bool SVC_EntityMessage_BitRead_Internal(BitRead& bitbuf, SourceGameContext& context, NetMsg::SVC_EntityMessage* data)
    {
        data->entIndex = bitbuf.ReadUBitLong(MAX_EDICT_BITS);
        data->classID = bitbuf.ReadUBitLong(MAX_SERVER_CLASS_BITS);
        data->dataLengthInBits = bitbuf.ReadUBitLong(11);
        data->data.reset(new uint8_t[math::BitsToBytes(data->dataLengthInBits)]);
        bitbuf.ReadBits(data->data.get(), data->dataLengthInBits);
        return !bitbuf.IsOverflowed();
    }

    bool SVC_EntityMessage_BitWrite_Internal(BitWrite& bitbuf, const SourceGameContext& context, NetMsg::SVC_EntityMessage* data)
    {
        bitbuf.WriteUBitLong(data->entIndex, MAX_EDICT_BITS);
        bitbuf.WriteUBitLong(data->classID, MAX_SERVER_CLASS_BITS);
        bitbuf.WriteUBitLong(data->dataLengthInBits, 11);
        bitbuf.WriteBits(data->data.get(), data->dataLengthInBits);
        return !bitbuf.IsOverflowed();
    }

    bool SVC_EntityMessage_JsonRead_Internal(JsonRead& jsonbuf, SourceGameContext& context, NetMsg::SVC_EntityMessage* data)
    {
        base::JsonReaderObject reader = jsonbuf.ParseObject();
        assert(!reader.HasReadError());
        data->entIndex = reader.ReadUInt32("entIndex");
        data->classID = reader.ReadUInt32("classId");
        data->dataLengthInBits = reader.ReadUInt32("dataLengthInBits");
        data->data.reset(new uint8_t[math::BitsToBytes(data->dataLengthInBits)]);
        reader.ReadBits("data", data->data.get(), data->dataLengthInBits);
        return !reader.HasReadError();
    }

    bool SVC_EntityMessage_JsonWrite_Internal(JsonWrite& jsonbuf, const SourceGameContext& context, NetMsg::SVC_EntityMessage* data)
    {
        jsonbuf.Reset();
        jsonbuf.StartObject();
        jsonbuf.WriteUInt32("entIndex", data->entIndex);
        jsonbuf.WriteUInt32("classId", data->classID);
        jsonbuf.WriteUInt32("dataLengthInBits", data->dataLengthInBits);
        jsonbuf.WriteBits("data", data->data.get(), data->dataLengthInBits);
        jsonbuf.EndObject();
        return jsonbuf.IsComplete();
    }

    void SVC_EntityMessage_ToString_Internal(std::ostringstream& out, NetMsg::SVC_EntityMessage* data)
    {
        out << "svc_EntityMessage: entity " << data->entIndex
            << ", class " << data->classID
            << ", bytes " << math::BitsToBytes(data->dataLengthInBits);
    }
}
