
#pragma once

#include "demhandlers.h"
#include <string>
#include "base/array.h"

namespace DemMsg
{
    struct Dem_StringTables
    {
        struct StringTableEntry
        {
            static const int ENTRYNAME_MAX_LENGTH = 4096;

            std::string entryName;
            Array<uint8_t, int16_t> data;
        };

        struct StringTable
        {
            static const int TABLENAME_MAX_LENGTH = 256;

            std::string tableName;
            Array<StringTableEntry> entries1;
            Array<StringTableEntry> entries2;
        };

        Array<StringTable> stringtables;
    };
}

DECLARE_DEM_HANDLERS(Dem_StringTables);
