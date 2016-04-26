
#include "svc_createstringtable.h"
#include "base/bitfile.h"
#include "base/jsonfile.h"
#include "netmath.h"
#include "netcontants.h"

// #define WIP_STRINGTABLE

#ifdef WIP_STRINGTABLE
#include "sourcesdk/common.h"
#include <vector>

#define SUBSTRING_BITS 5
struct StringHistoryEntry
{
    char string[(1 << SUBSTRING_BITS)];
};

static void StringTable_BitRead(NetHandlers::BitRead& bitbuf, SourceGameContext& context, NetMsg::SVC_CreateStringTable* data)
{
    const size_t numEncodeBits = math::log2(data->maxEntries);
    std::vector<StringHistoryEntry> history;
    int entryIndex = -1;
    for (uint i = 0; i < data->numEntries; ++i)
    {
        entryIndex++;

        if (bitbuf.ReadOneBit() == 0)
        {
            entryIndex = bitbuf.ReadUBitLong(numEncodeBits);
        }

        const char *pEntry = NULL;
        char entry[1024];
        char substr[1024];
        if (bitbuf.ReadOneBit() != 0)
        {
            bool substringcheck = bitbuf.ReadOneBit() != 0;
            if (substringcheck)
            {
                int index = bitbuf.ReadUBitLong(5);
                int bytestocopy = bitbuf.ReadUBitLong(SUBSTRING_BITS);
                strncpy(entry, history.at(index).string, bytestocopy + 1);
                entry[bytestocopy + 1] = '\0';
                bitbuf.ReadString(substr, sizeof(substr));
                strncat(entry, substr, sizeof(entry));
            }
            else
            {
                bitbuf.ReadString(entry, sizeof(entry));
            }
            pEntry = entry;
            printf("%s\n", pEntry);
        }
        const int MAX_USERDATA_BITS = 14;
        unsigned char tempbuf[(1 << MAX_USERDATA_BITS)] = { 0 };
        const void *pUserData = NULL;
        if (bitbuf.ReadOneBit() != 0)
        {
            if (data->isUserDataFixedSize)
            {
                bitbuf.ReadBits(tempbuf, data->userDataSizeBits);
            }
            else
            {
                int nBytes = bitbuf.ReadUBitLong(MAX_USERDATA_BITS);
                bitbuf.ReadBytes(tempbuf, nBytes);
            }
            pUserData = tempbuf;
        }

        if (pEntry == NULL)
        {
            pEntry = "";
        }

        if (history.size() > 31)
        {
            history.erase(history.begin());
        }

        StringHistoryEntry she;
        strncpy(she.string, pEntry, sizeof(she.string));
        history.emplace_back(she);
    }
}

#endif // WIP_STRINGTABLE

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

        const size_t numEncodeBits = math::log2(data->maxEntries);
        data->numEntries = bitbuf.ReadUBitLong(numEncodeBits + 1);

        size_t dataLengthInBits;
        if (context.protocol > 23)
        {
            dataLengthInBits = bitbuf.ReadVarInt32();
            data->dataLengthInBits = dataLengthInBits;
        }
        else
        {
            dataLengthInBits = bitbuf.ReadUBitLong(NET_MAX_PAYLOAD_BITS_OLD + 3);
            data->dataLengthInBits = dataLengthInBits;
        }
        const size_t dataLengthInBytes = math::BitsToBytes(dataLengthInBits);

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
        data->data.reset(new uint8_t[dataLengthInBytes]);
        bitbuf.ReadBits(data->data.get(), dataLengthInBits);

#ifdef WIP_STRINGTABLE
        if (data->compressedData)
        {
            bf_read bitbuf2(data->data.get(), dataLengthInBytes, dataLengthInBits);
            const uint32_t decompressedNumBytes = bitbuf2.ReadUBitLong(32);
            const uint32_t compressedNumBytes = bitbuf2.ReadUBitLong(32);
            std::unique_ptr<uint8_t[]> compressedData(new uint8[compressedNumBytes]);
            std::unique_ptr<uint8_t[]> uncompressedData(new uint8[decompressedNumBytes]);
            bitbuf2.ReadBytes(compressedData.get(), compressedNumBytes);

            uint32_t numWritten = COM_BufferToBufferDecompress(uncompressedData.get(), decompressedNumBytes, compressedData.get(), compressedNumBytes);
            bitbuf2 = bf_read(uncompressedData.get(), decompressedNumBytes);
            StringTable_BitRead(bitbuf2, context, data);
        }
        else
        {
            bf_read bitbuf2(data->data.get(), dataLengthInBytes, dataLengthInBits);
            StringTable_BitRead(bitbuf2, context, data);
        }
#endif // WIP_STRINGTABLE

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
