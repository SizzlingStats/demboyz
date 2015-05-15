
#pragma once

#include "nethandlers.h"
#include <memory>

namespace NetMsg
{
    struct SVC_UpdateStringTable
    {
        uint32_t tableID;
        bool twoOrMoreChangedEntries;
        uint16_t numChangedEntries;
        uint32_t dataLengthInBits;
        std::unique_ptr<uint8_t[]> data;
    };
}

DECLARE_NET_HANDLERS(SVC_UpdateStringTable);
