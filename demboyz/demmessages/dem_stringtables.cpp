
#include "dem_stringtables.h"
#include "demofile/demofile.h"
#include "netmessages/netcontants.h"
#include "sourcesdk/bitbuf.h"
#include "base/jsonfile.h"
#include <memory>

static void StringTableEntry_BitRead(bf_read& bitbuf, DemMsg::Dem_StringTables::StringTableEntry* data)
{
    using StringTableEntry = DemMsg::Dem_StringTables::StringTableEntry;
    {
        char entryName[StringTableEntry::ENTRYNAME_MAX_LENGTH];
        bitbuf.ReadString(entryName, sizeof(entryName));
        data->entryName.assign(entryName);
    }

    if (bitbuf.ReadOneBit() != 0)
    {
        const int32_t numBytes = bitbuf.ReadWord();
        data->data.reset(numBytes);
        bitbuf.ReadBytes(data->data.begin(), numBytes);
    }
    else
    {
        data->data.reset(0);
    }
}

static void StringTableEntry_BitWrite(bf_write& bitbuf, DemMsg::Dem_StringTables::StringTableEntry* data)
{
    bitbuf.WriteString(data->entryName.c_str());
    const int32_t numDataBytes = data->data.length();

    bitbuf.WriteOneBit(numDataBytes > 0);
    if (numDataBytes > 0)
    {
        bitbuf.WriteWord(numDataBytes);
        bitbuf.WriteBytes(data->data.begin(), numDataBytes);
    }
}

static void StringTableEntry_JsonWrite(JsonWrite& jsonbuf, const DemMsg::Dem_StringTables::StringTableEntry* data)
{
    jsonbuf.StartObject(data->entryName.c_str());
    //jsonbuf.WriteUInt32("dataLengthInBytes", data->data.length());
    jsonbuf.WriteBytes("data", data->data.begin(), data->data.length());
    jsonbuf.EndObject();
}

static void StringTable_BitRead(bf_read& bitbuf, DemMsg::Dem_StringTables::StringTable* data)
{
    using StringTable = DemMsg::Dem_StringTables::StringTable;
    using StringTableEntry = DemMsg::Dem_StringTables::StringTableEntry;
    {
        char tableName[StringTable::TABLENAME_MAX_LENGTH];
        bitbuf.ReadString(tableName, sizeof(tableName));
        data->tableName.assign(tableName);
    }

    data->entries.reset(bitbuf.ReadWord());
    for (StringTableEntry& entry : data->entries)
    {
        StringTableEntry_BitRead(bitbuf, &entry);
    }
    if (bitbuf.ReadOneBit() != 0)
    {
        data->entriesClientSide.reset(bitbuf.ReadWord());
        for (StringTableEntry& entry : data->entriesClientSide)
        {
            StringTableEntry_BitRead(bitbuf, &entry);
        }
    }
    else
    {
        data->entriesClientSide.reset(0);
    }
}

static void StringTable_BitWrite(bf_write& bitbuf, DemMsg::Dem_StringTables::StringTable* data)
{
    using StringTableEntry = DemMsg::Dem_StringTables::StringTableEntry;
    bitbuf.WriteString(data->tableName.c_str());

    bitbuf.WriteWord(data->entries.length());
    for (StringTableEntry& entry : data->entries)
    {
        StringTableEntry_BitWrite(bitbuf, &entry);
    }

    const int32_t numEntriesClientSide = data->entriesClientSide.length();
    bitbuf.WriteOneBit(numEntriesClientSide > 0);
    if (numEntriesClientSide > 0)
    {
        bitbuf.WriteWord(numEntriesClientSide);
        for (StringTableEntry& entry : data->entriesClientSide)
        {
            StringTableEntry_BitWrite(bitbuf, &entry);
        }
    }
}

static void StringTable_JsonWrite(JsonWrite& jsonbuf, const DemMsg::Dem_StringTables::StringTable* data)
{
    using StringTableEntry = DemMsg::Dem_StringTables::StringTableEntry;
    jsonbuf.StartArray(data->tableName.c_str());
    for (const StringTableEntry& entry : data->entries)
    {
        jsonbuf.StartObject();
        StringTableEntry_JsonWrite(jsonbuf, &entry);
        jsonbuf.EndObject();
    }
    jsonbuf.EndArray();
}

namespace DemHandlers
{
    bool Dem_StringTables_FileRead_Internal(FileRead& demofile, DemMsg::Dem_StringTables* data)
    {
        using StringTable = DemMsg::Dem_StringTables::StringTable;

        Array<uint8_t> buffer = demofile.ReadRawData(MAX_STRINGTABLE_DATA);
        bf_read bitbuf(buffer.begin(), buffer.length());

        data->stringtables.reset(bitbuf.ReadByte());
        for (StringTable& table : data->stringtables)
        {
            StringTable_BitRead(bitbuf, &table);
        }

        // copy trailing bits for binary completeness
        const unsigned int numBitsLeft = bitbuf.GetNumBitsLeft();
        assert(numBitsLeft < 8);
        data->numTrailingBits = numBitsLeft;
        if (numBitsLeft > 0)
        {
            data->trailingBitsValue = bitbuf.ReadUBitLong(numBitsLeft);
        }
        return !bitbuf.IsOverflowed();
    }

    bool Dem_StringTables_FileWrite_Internal(FileWrite& demofile, DemMsg::Dem_StringTables* data)
    {
        using StringTable = DemMsg::Dem_StringTables::StringTable;

        std::unique_ptr<uint8_t[]> buffer(new uint8_t[MAX_STRINGTABLE_DATA]);
        bf_write bitbuf(buffer.get(), MAX_STRINGTABLE_DATA);

        bitbuf.WriteByte(data->stringtables.length());
        for (StringTable& table : data->stringtables)
        {
            StringTable_BitWrite(bitbuf, &table);
        }
        if (data->numTrailingBits > 0)
        {
            bitbuf.WriteUBitLong(data->trailingBitsValue, data->numTrailingBits);
        }
        demofile.WriteRawData(bitbuf.GetBasePointer(), bitbuf.GetNumBytesWritten());
        return !bitbuf.IsOverflowed();
    }

    bool Dem_StringTables_JsonRead_Internal(JsonRead& jsonbuf, DemMsg::Dem_StringTables* data)
    {
        return true;
    }

    bool Dem_StringTables_JsonWrite_Internal(JsonWrite& jsonbuf, DemMsg::Dem_StringTables* data)
    {
        using StringTable = DemMsg::Dem_StringTables::StringTable;
        jsonbuf.StartArray("tables");
        for (const StringTable& table : data->stringtables)
        {
            jsonbuf.StartObject();
            StringTable_JsonWrite(jsonbuf, &table);
            jsonbuf.EndObject();
        }
        jsonbuf.EndArray();
        jsonbuf.WriteUInt32("numTrailingBits", data->numTrailingBits);
        if (data->numTrailingBits > 0)
        {
            jsonbuf.WriteUInt32("trailingBitsValue", data->trailingBitsValue);
        }
        else
        {
            jsonbuf.WriteNull("trailingBitsValue");
        }
        return true;
    }
}
