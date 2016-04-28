
#include "svc_tempentities.h"
#include "base/bitfile.h"
#include "base/jsonfile.h"
#include "game/sourcecontext.h"
#include "netcontants.h"
#include "netmath.h"

namespace NetHandlers
{
    bool SVC_TempEntities_BitRead_Internal(BitRead& bitbuf, SourceGameContext& context, NetMsg::SVC_TempEntities* data)
    {
        data->numEntries = bitbuf.ReadUBitLong(EVENT_INDEX_BITS);
        if (context.protocol > 23)
        {
            data->dataLengthInBits = bitbuf.ReadVarInt32();
        }
        else
        {
            data->dataLengthInBits = bitbuf.ReadUBitLong(NET_MAX_PAYLOAD_BITS_OLD);
        }
        data->data.reset(new uint8_t[math::BitsToBytes(data->dataLengthInBits)]);
        bitbuf.ReadBits(data->data.get(), data->dataLengthInBits);
        return !bitbuf.IsOverflowed();
    }

    bool SVC_TempEntities_BitWrite_Internal(BitWrite& bitbuf, const SourceGameContext& context, NetMsg::SVC_TempEntities* data)
    {
        bitbuf.WriteUBitLong(data->numEntries, EVENT_INDEX_BITS);
        if (context.protocol > 23)
        {
            bitbuf.WriteVarInt32(data->dataLengthInBits);
        }
        else
        {
            bitbuf.WriteUBitLong(data->dataLengthInBits, NET_MAX_PAYLOAD_BITS_OLD);
        }
        bitbuf.WriteBits(data->data.get(), data->dataLengthInBits);
        return !bitbuf.IsOverflowed();
    }

    bool SVC_TempEntities_JsonRead_Internal(JsonRead& jsonbuf, SourceGameContext& context, NetMsg::SVC_TempEntities* data)
    {
        base::JsonReaderObject reader = jsonbuf.ParseObject();
        assert(!reader.HasReadError());
        data->numEntries = reader.ReadUInt32("numEntries");
        data->dataLengthInBits = reader.ReadUInt32("dataLengthInBits");
        data->data.reset(new uint8_t[math::BitsToBytes(data->dataLengthInBits)]);
        reader.ReadBits("data", data->data.get(), data->dataLengthInBits);
        return !reader.HasReadError();
    }

    bool SVC_TempEntities_JsonWrite_Internal(JsonWrite& jsonbuf, const SourceGameContext& context, NetMsg::SVC_TempEntities* data)
    {
        jsonbuf.Reset();
        jsonbuf.StartObject();
        jsonbuf.WriteUInt32("numEntries", data->numEntries);
        jsonbuf.WriteUInt32("dataLengthInBits", data->dataLengthInBits);
        jsonbuf.WriteBits("data", data->data.get(), data->dataLengthInBits);
        jsonbuf.EndObject();
        return jsonbuf.IsComplete();
    }

    void SVC_TempEntities_ToString_Internal(std::ostringstream& out, NetMsg::SVC_TempEntities* data)
    {
        out << "svc_TempEntities: number " << data->numEntries
            << ", bytes " << math::BitsToBytes(data->dataLengthInBits);
    }
}
