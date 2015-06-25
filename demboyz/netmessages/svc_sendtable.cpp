
#include "svc_sendtable.h"
#include "base/bitfile.h"
#include "base/jsonfile.h"
#include "netmath.h"

namespace NetHandlers
{
    bool SVC_SendTable_BitRead_Internal(BitRead& bitbuf, SourceGameContext& context, NetMsg::SVC_SendTable* data)
    {
        data->needsDecoder = bitbuf.ReadOneBit() != 0;
        data->dataLengthInBits = bitbuf.ReadShort();
        data->data.reset(new uint8_t[math::BitsToBytes(data->dataLengthInBits)]);
        bitbuf.ReadBits(data->data.get(), data->dataLengthInBits);
        return !bitbuf.IsOverflowed();
    }

    bool SVC_SendTable_BitWrite_Internal(BitWrite& bitbuf, const SourceGameContext& context, NetMsg::SVC_SendTable* data)
    {
        bitbuf.WriteOneBit(data->needsDecoder);
        bitbuf.WriteShort(data->dataLengthInBits);
        bitbuf.WriteBits(data->data.get(), data->dataLengthInBits);
        return !bitbuf.IsOverflowed();
    }

    bool SVC_SendTable_JsonRead_Internal(JsonRead& jsonbuf, SourceGameContext& context, NetMsg::SVC_SendTable* data)
    {
        return true;
    }

    bool SVC_SendTable_JsonWrite_Internal(JsonWrite& jsonbuf, const SourceGameContext& context, NetMsg::SVC_SendTable* data)
    {
        jsonbuf.StartObject("svc_sendtable");
        jsonbuf.WriteBool("needsDecoder", data->needsDecoder);
        jsonbuf.WriteUInt32("dataLengthInBits", data->dataLengthInBits);
        jsonbuf.WriteBits("data", data->data.get(), data->dataLengthInBits);
        jsonbuf.EndObject();
        return true;
    }

    void SVC_SendTable_ToString_Internal(std::ostringstream& out, NetMsg::SVC_SendTable* data)
    {
        out << "svc_SendTable: needs Decoder " << (data->needsDecoder ? "yes" : "no")
            << ",bytes " << math::BitsToBytes(data->dataLengthInBits);
    }
}
