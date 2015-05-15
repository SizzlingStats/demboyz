
#pragma once

#include "nethandlers.h"
#include <memory>

namespace NetMsg
{
    struct SVC_CreateStringTable
    {
        bool isFileNames;
        char tableName[256];
        uint16_t maxEntries;
        uint16_t numEntries;
        uint32_t dataLengthInBits;
        bool isUserDataFixedSize;
        uint16_t userDataSize;
        uint8_t userDataSizeBits;
        bool unk1;
        std::unique_ptr<uint8_t[]> data;
    };
}

DECLARE_NET_HANDLERS(SVC_CreateStringTable);
