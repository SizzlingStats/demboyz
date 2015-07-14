
#include "svc_updatestringtable.h"
#include "base/bitfile.h"
#include "base/jsonfile.h"
#include "netmath.h"
#include "netcontants.h"

namespace NetHandlers
{
    bool SVC_UpdateStringTable_BitRead_Internal(BitRead& bitbuf, SourceGameContext& context, NetMsg::SVC_UpdateStringTable* data)
    {
        data->tableID = bitbuf.ReadUBitLong(math::log2(MAX_TABLES));
        data->numChangedEntries = (bitbuf.ReadOneBit() != 0) ? bitbuf.ReadWord() : 1;
        data->dataLengthInBits = bitbuf.ReadUBitLong(20);
        data->data.reset(new uint8_t[math::BitsToBytes(data->dataLengthInBits)]);
        bitbuf.ReadBits(data->data.get(), data->dataLengthInBits);
        return !bitbuf.IsOverflowed();
    }

    bool SVC_UpdateStringTable_BitWrite_Internal(BitWrite& bitbuf, const SourceGameContext& context, NetMsg::SVC_UpdateStringTable* data)
    {
        bitbuf.WriteUBitLong(data->tableID, math::log2(MAX_TABLES));
        if (data->numChangedEntries != 1)
        {
            bitbuf.WriteOneBit(1);
            bitbuf.WriteWord(data->numChangedEntries);
        }
        else
        {
            bitbuf.WriteOneBit(0);
        }
        bitbuf.WriteUBitLong(data->dataLengthInBits, 20);
        bitbuf.WriteBits(data->data.get(), data->dataLengthInBits);
        return !bitbuf.IsOverflowed();
    }

    bool SVC_UpdateStringTable_JsonRead_Internal(JsonRead& jsonbuf, SourceGameContext& context, NetMsg::SVC_UpdateStringTable* data)
    {
        base::JsonReaderObject reader = jsonbuf.ParseObject();
        assert(!reader.HasReadError());
        data->tableID = reader.ReadUInt32("tableId");
        data->numChangedEntries = reader.ReadUInt32("numChangedEntries");
        data->dataLengthInBits = reader.ReadUInt32("dataLengthInBits");
        data->data.reset(new uint8_t[math::BitsToBytes(data->dataLengthInBits)]);
        reader.ReadBits("data", data->data.get(), data->dataLengthInBits);
        return !reader.HasReadError();
    }

    bool SVC_UpdateStringTable_JsonWrite_Internal(JsonWrite& jsonbuf, const SourceGameContext& context, NetMsg::SVC_UpdateStringTable* data)
    {
        jsonbuf.Reset();
        jsonbuf.StartObject();
        jsonbuf.WriteUInt32("tableId", data->tableID);
        jsonbuf.WriteUInt32("numChangedEntries", data->numChangedEntries);
        jsonbuf.WriteUInt32("dataLengthInBits", data->dataLengthInBits);
        jsonbuf.WriteBits("data", data->data.get(), data->dataLengthInBits);
        jsonbuf.EndObject();
        return jsonbuf.IsComplete();
    }

    void SVC_UpdateStringTable_ToString_Internal(std::ostringstream& out, NetMsg::SVC_UpdateStringTable* data)
    {
        out << "svc_UpdateStringTable: table " << data->tableID
            << ", changed " << data->numChangedEntries
            << ", bytes " << math::BitsToBytes(data->dataLengthInBits);
    }
}
