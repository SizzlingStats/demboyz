
#include "svc_tempentities.h"
#include "bitbuf.h"
#include "netcontants.h"
#include "netmath.h"

namespace NetHandlers
{
    bool SVC_TempEntities_BitRead_Internal(bf_read& bitbuf, SourceGameContext& context, NetMsg::SVC_TempEntities* data)
    {
        data->numEntries = bitbuf.ReadUBitLong(EVENT_INDEX_BITS);
        data->dataLengthInBits = bitbuf.ReadUBitLong(NET_MAX_PAYLOAD_BITS);
        data->data.reset(new uint8_t[math::BitsToBytes(data->dataLengthInBits)]);
        bitbuf.ReadBits(data->data.get(), data->dataLengthInBits);
        return !bitbuf.IsOverflowed();
    }

    bool SVC_TempEntities_BitWrite_Internal(bf_write& bitbuf, SourceGameContext& context, NetMsg::SVC_TempEntities* data)
    {
        bitbuf.WriteUBitLong(data->numEntries, EVENT_INDEX_BITS);
        bitbuf.WriteUBitLong(data->dataLengthInBits, NET_MAX_PAYLOAD_BITS);
        bitbuf.WriteBits(data->data.get(), data->dataLengthInBits);
        return !bitbuf.IsOverflowed();
    }

    bool SVC_TempEntities_JsonRead_Internal(JsonRead& jsonbuf, SourceGameContext& context, NetMsg::SVC_TempEntities* data)
    {
        return true;
    }

    bool SVC_TempEntities_JsonWrite_Internal(JsonWrite& jsonbuf, SourceGameContext& context, NetMsg::SVC_TempEntities* data)
    {
        return true;
    }

    void SVC_TempEntities_ToString_Internal(std::ostringstream& out, NetMsg::SVC_TempEntities* data)
    {
        out << "svc_TempEntities: number " << data->numEntries
            << ", bytes " << math::BitsToBytes(data->dataLengthInBits);
    }
}
