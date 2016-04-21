
#include "svc_createstringtable.h"
#include "base/bitfile.h"
#include "base/jsonfile.h"
#include "netmath.h"
#include "netcontants.h"

namespace NetHandlers
{
    bool SVC_CreateStringTable_BitRead_Internal(BitRead& bitbuf, SourceGameContext& context, NetMsg::SVC_CreateStringTable* data)
    {
        if (bitbuf.PeekUBitLong(8) == ':')
        {
            data->isFileNames = true;
            bitbuf.ReadByte();
        }
        else
        {
            data->isFileNames = false;
        }
        bitbuf.ReadString(data->tableName, sizeof(data->tableName));
        data->maxEntries = bitbuf.ReadWord();
        data->numEntries = bitbuf.ReadUBitLong(math::log2(data->maxEntries) + 1);
        if (context.protocol > 23)
        {
            data->dataLengthInBits = bitbuf.ReadVarInt32();
        }
        else
        {
            data->dataLengthInBits = bitbuf.ReadUBitLong(NET_MAX_PAYLOAD_BITS_OLD + 3);
        }
        data->isUserDataFixedSize = bitbuf.ReadOneBit() != 0;
        if (data->isUserDataFixedSize)
        {
            data->userDataSize = bitbuf.ReadUBitLong(12);
            data->userDataSizeBits = bitbuf.ReadUBitLong(4);
        }
        else
        {
            data->userDataSize = 0;
            data->userDataSizeBits = 0;
        }
        if (context.protocol > 14)
        {
            data->compressedData = bitbuf.ReadOneBit() != 0;
        }
        data->data.reset(new uint8_t[math::BitsToBytes(data->dataLengthInBits)]);
        bitbuf.ReadBits(data->data.get(), data->dataLengthInBits);
        return !bitbuf.IsOverflowed();
    }

    bool SVC_CreateStringTable_BitWrite_Internal(BitWrite& bitbuf, const SourceGameContext& context, NetMsg::SVC_CreateStringTable* data)
    {
        if (data->isFileNames)
        {
            bitbuf.WriteByte(':');
        }
        bitbuf.WriteString(data->tableName);
        bitbuf.WriteWord(data->maxEntries);
        bitbuf.WriteUBitLong(data->numEntries, math::log2(data->maxEntries) + 1);
        if (context.protocol > 23)
        {
            bitbuf.WriteVarInt32(data->dataLengthInBits);
        }
        else
        {
            bitbuf.WriteUBitLong(data->dataLengthInBits, NET_MAX_PAYLOAD_BITS_OLD + 3);
        }
        bitbuf.WriteOneBit(data->isUserDataFixedSize);
        if (data->isUserDataFixedSize)
        {
            bitbuf.WriteUBitLong(data->userDataSize, 12);
            bitbuf.WriteUBitLong(data->userDataSizeBits, 4);
        }
        if (context.protocol > 14)
        {
            bitbuf.WriteOneBit(data->compressedData);
        }
        bitbuf.WriteBits(data->data.get(), data->dataLengthInBits);
        return !bitbuf.IsOverflowed();
    }

    bool SVC_CreateStringTable_JsonRead_Internal(JsonRead& jsonbuf, SourceGameContext& context, NetMsg::SVC_CreateStringTable* data)
    {
        base::JsonReaderObject reader = jsonbuf.ParseObject();
        assert(!reader.HasReadError());
        data->isFileNames = reader.ReadBool("isFilenames");
        reader.ReadString("tableName", data->tableName, sizeof(data->tableName));
        data->maxEntries = reader.ReadUInt32("maxEntries");
        data->numEntries = reader.ReadUInt32("numEntries");
        data->dataLengthInBits = reader.ReadInt32("dataLengthInBits");
        data->isUserDataFixedSize = reader.ReadBool("isUserDataFixedSize");
        data->userDataSize = reader.ReadUInt32("userDataSize");
        data->userDataSizeBits = reader.ReadUInt32("userDataSizeBits");
        if (context.protocol > 14)
        {
            data->compressedData = reader.ReadBool("compressedData");
        }
        data->data.reset(new uint8_t[math::BitsToBytes(data->dataLengthInBits)]);
        reader.ReadBits("data", data->data.get(), data->dataLengthInBits);
        return !reader.HasReadError();
    }

    bool SVC_CreateStringTable_JsonWrite_Internal(JsonWrite& jsonbuf, const SourceGameContext& context, NetMsg::SVC_CreateStringTable* data)
    {
        jsonbuf.Reset();
        jsonbuf.StartObject();
        jsonbuf.WriteBool("isFilenames", data->isFileNames);
        jsonbuf.WriteString("tableName", data->tableName);
        jsonbuf.WriteUInt32("maxEntries", data->maxEntries);
        jsonbuf.WriteUInt32("numEntries", data->numEntries);
        jsonbuf.WriteInt32("dataLengthInBits", data->dataLengthInBits);
        jsonbuf.WriteBool("isUserDataFixedSize", data->isUserDataFixedSize);
        jsonbuf.WriteUInt32("userDataSize", data->userDataSize);
        jsonbuf.WriteUInt32("userDataSizeBits", data->userDataSizeBits);
        if (context.protocol > 14)
        {
            jsonbuf.WriteBool("compressedData", data->compressedData);
        }
        jsonbuf.WriteBits("data", data->data.get(), data->dataLengthInBits);
        jsonbuf.EndObject();
        return jsonbuf.IsComplete();
    }

    void SVC_CreateStringTable_ToString_Internal(std::ostringstream& out, NetMsg::SVC_CreateStringTable* data)
    {
        out << "svc_CreateStringTable: table " << data->tableName
            << ", entries " << data->numEntries
            << ", bytes " << math::BitsToBytes(data->dataLengthInBits)
            << " userdatasize " << data->userDataSize
            << " userdatabits " << static_cast<uint32_t>(data->userDataSizeBits);
    }
}
