
#include "dem_stringtables.h"
#include "demofile/demofile.h"
#include "netmessages/netcontants.h"
#include "sourcesdk/bitbuf.h"
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

static void StringTable_BitRead(bf_read& bitbuf, DemMsg::Dem_StringTables::StringTable* data)
{
    using StringTable = DemMsg::Dem_StringTables::StringTable;
    using StringTableEntry = DemMsg::Dem_StringTables::StringTableEntry;
    {
        char tableName[StringTable::TABLENAME_MAX_LENGTH];
        bitbuf.ReadString(tableName, sizeof(tableName));
        data->tableName.assign(tableName);
    }

    data->entries1.reset(bitbuf.ReadWord());
    for (StringTableEntry& entry : data->entries1)
    {
        StringTableEntry_BitRead(bitbuf, &entry);
    }
    if (bitbuf.ReadOneBit() != 0)
    {
        data->entries2.reset(bitbuf.ReadWord());
        for (StringTableEntry& entry : data->entries2)
        {
            StringTableEntry_BitRead(bitbuf, &entry);
        }
    }
    else
    {
        data->entries2.reset(0);
    }
}

static void StringTable_BitWrite(bf_write& bitbuf, DemMsg::Dem_StringTables::StringTable* data)
{
    using StringTableEntry = DemMsg::Dem_StringTables::StringTableEntry;
    bitbuf.WriteString(data->tableName.c_str());

    bitbuf.WriteWord(data->entries1.length());
    for (StringTableEntry& entry : data->entries1)
    {
        StringTableEntry_BitWrite(bitbuf, &entry);
    }

    const int32_t entries2Count = data->entries2.length();
    bitbuf.WriteOneBit(entries2Count > 0);
    if (entries2Count > 0)
    {
        bitbuf.WriteWord(entries2Count);
        for (StringTableEntry& entry : data->entries2)
        {
            StringTableEntry_BitWrite(bitbuf, &entry);
        }
    }
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
        demofile.WriteRawData(bitbuf.GetBasePointer(), bitbuf.GetNumBytesWritten());
        return !bitbuf.IsOverflowed();
    }

    bool Dem_StringTables_JsonRead_Internal(JsonRead& jsonbuf, DemMsg::Dem_StringTables* data)
    {
        return true;
    }

    bool Dem_StringTables_JsonWrite_Internal(JsonWrite& jsonbuf, DemMsg::Dem_StringTables* data)
    {
        return true;
    }
}
