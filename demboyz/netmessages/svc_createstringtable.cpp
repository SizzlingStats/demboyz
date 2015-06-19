
#include "svc_createstringtable.h"
#include "sourcesdk/bitbuf.h"
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
            data->dataLengthInBits = bitbuf.ReadUBitLong(NET_MAX_PAYLOAD_BITS + 1);
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
            data->unk1 = bitbuf.ReadOneBit() != 0;
        }
        else
        {
            data->unk1 = false;
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
            bitbuf.WriteUBitLong(data->dataLengthInBits, NET_MAX_PAYLOAD_BITS + 1);
        }
        bitbuf.WriteOneBit(data->isUserDataFixedSize);
        if (data->isUserDataFixedSize)
        {
            bitbuf.WriteUBitLong(data->userDataSize, 12);
            bitbuf.WriteUBitLong(data->userDataSizeBits, 4);
        }
        if (context.protocol > 14)
        {
            bitbuf.WriteOneBit(data->unk1);
        }
        bitbuf.WriteBits(data->data.get(), data->dataLengthInBits);
        return !bitbuf.IsOverflowed();
    }

    bool SVC_CreateStringTable_JsonRead_Internal(JsonRead& jsonbuf, SourceGameContext& context, NetMsg::SVC_CreateStringTable* data)
    {
        return true;
    }

    bool SVC_CreateStringTable_JsonWrite_Internal(JsonWrite& jsonbuf, const SourceGameContext& context, NetMsg::SVC_CreateStringTable* data)
    {
        return true;
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
