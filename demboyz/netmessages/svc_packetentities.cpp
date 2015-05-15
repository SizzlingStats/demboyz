
#include "svc_packetentities.h"
#include "bitbuf.h"
#include "netcontants.h"
#include "netmath.h"

namespace NetHandlers
{
    bool SVC_PacketEntities_BitRead_Internal(bf_read& bitbuf, SourceGameContext& context, NetMsg::SVC_PacketEntities* data)
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

    bool SVC_PacketEntities_BitWrite_Internal(bf_write& bitbuf, SourceGameContext& context, NetMsg::SVC_PacketEntities* data)
    {
        bitbuf.WriteUBitLong(data->maxEntries, MAX_EDICT_BITS);
        if (data->isDelta)
        {
            bitbuf.WriteLong(data->deltaFromTick);
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
        return true;
    }

    bool SVC_PacketEntities_JsonWrite_Internal(JsonWrite& jsonbuf, SourceGameContext& context, NetMsg::SVC_PacketEntities* data)
    {
        return true;
    }

    void SVC_PacketEntities_ToString_Internal(std::ostringstream& out, NetMsg::SVC_PacketEntities* data)
    {
        out << "svc_PacketEntities: delta " << data->deltaFromTick
            << ", max " << data->maxEntries
            << ", changed " << data->numUpdatedEntries
            << "," << (data->updateBaseline ? " BL update," : "")
            << " bytes" << math::BitsToBytes(data->dataLengthInBits);
    }
}
