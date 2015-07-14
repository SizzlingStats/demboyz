
#include "svc_packetentities.h"
#include "base/bitfile.h"
#include "base/jsonfile.h"
#include "netcontants.h"
#include "netmath.h"

namespace NetHandlers
{
    bool SVC_PacketEntities_BitRead_Internal(BitRead& bitbuf, SourceGameContext& context, NetMsg::SVC_PacketEntities* data)
    {
        data->maxEntries = bitbuf.ReadUBitLong(MAX_EDICT_BITS);
        data->isDelta = bitbuf.ReadOneBit() != 0;
        if (data->isDelta)
        {
            data->deltaFromTick = bitbuf.ReadLong();
        }
        else
        {
            data->deltaFromTick = -1;
        }
        data->baselineIndex = bitbuf.ReadUBitLong(1);
        data->numUpdatedEntries = bitbuf.ReadUBitLong(MAX_EDICT_BITS);
        data->dataLengthInBits = bitbuf.ReadUBitLong(DELTASIZE_BITS);
        data->updateBaseline = bitbuf.ReadOneBit() != 0;
        data->data.reset(new uint8_t[math::BitsToBytes(data->dataLengthInBits)]);
        bitbuf.ReadBits(data->data.get(), data->dataLengthInBits);
        return !bitbuf.IsOverflowed();
    }

    bool SVC_PacketEntities_BitWrite_Internal(BitWrite& bitbuf, const SourceGameContext& context, NetMsg::SVC_PacketEntities* data)
    {
        bitbuf.WriteUBitLong(data->maxEntries, MAX_EDICT_BITS);
        if (data->isDelta)
        {
            bitbuf.WriteOneBit(1);
            bitbuf.WriteLong(data->deltaFromTick);
        }
        else
        {
            bitbuf.WriteOneBit(0);
        }
        bitbuf.WriteUBitLong(data->baselineIndex, 1);
        bitbuf.WriteUBitLong(data->numUpdatedEntries, MAX_EDICT_BITS);
        bitbuf.WriteUBitLong(data->dataLengthInBits, DELTASIZE_BITS);
        bitbuf.WriteOneBit(data->updateBaseline);
        bitbuf.WriteBits(data->data.get(), data->dataLengthInBits);
        return !bitbuf.IsOverflowed();
    }

    bool SVC_PacketEntities_JsonRead_Internal(JsonRead& jsonbuf, SourceGameContext& context, NetMsg::SVC_PacketEntities* data)
    {
        base::JsonReaderObject reader = jsonbuf.ParseObject();
        assert(!reader.HasReadError());
        data->maxEntries = reader.ReadInt32("maxEntries");
        data->isDelta = reader.ReadBool("isDelta");
        data->deltaFromTick = reader.ReadInt32("deltaFromTick");
        data->baselineIndex = reader.ReadUInt32("baselineIndex");
        data->numUpdatedEntries = reader.ReadUInt32("numUpdatedEntries");
        data->dataLengthInBits = reader.ReadUInt32("dataLengthInBits");
        data->updateBaseline = reader.ReadBool("updateBaseline");
        data->data.reset(new uint8_t[math::BitsToBytes(data->dataLengthInBits)]);
        reader.ReadBits("data", data->data.get(), data->dataLengthInBits);
        return !reader.HasReadError();
    }

    bool SVC_PacketEntities_JsonWrite_Internal(JsonWrite& jsonbuf, const SourceGameContext& context, NetMsg::SVC_PacketEntities* data)
    {
        jsonbuf.Reset();
        jsonbuf.StartObject();
        jsonbuf.WriteInt32("maxEntries", data->maxEntries);
        jsonbuf.WriteBool("isDelta", data->isDelta);
        jsonbuf.WriteInt32("deltaFromTick", data->deltaFromTick);
        jsonbuf.WriteUInt32("baselineIndex", data->baselineIndex);
        jsonbuf.WriteUInt32("numUpdatedEntries", data->numUpdatedEntries);
        jsonbuf.WriteUInt32("dataLengthInBits", data->dataLengthInBits);
        jsonbuf.WriteBool("updateBaseline", data->updateBaseline);
        jsonbuf.WriteBits("data", data->data.get(), data->dataLengthInBits);
        jsonbuf.EndObject();
        return jsonbuf.IsComplete();
    }

    void SVC_PacketEntities_ToString_Internal(std::ostringstream& out, NetMsg::SVC_PacketEntities* data)
    {
        out << "svc_PacketEntities: delta " << data->deltaFromTick
            << ", max " << data->maxEntries
            << ", changed " << data->numUpdatedEntries
            << "," << (data->updateBaseline ? " BL update," : "")
            << " bytes " << math::BitsToBytes(data->dataLengthInBits);
    }
}
