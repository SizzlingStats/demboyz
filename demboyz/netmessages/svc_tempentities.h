
#pragma once

#include "nethandlers.h"
#include <memory>

namespace NetMsg
{
    struct SVC_TempEntities
    {
        std::unique_ptr<uint8_t[]> data;
        uint32_t dataLengthInBits;
        uint32_t numEntries;
    };
}

DECLARE_NET_HANDLERS(SVC_TempEntities);
