
#pragma once

#include "nethandlers.h"
#include <memory>

namespace NetMsg
{
    struct SVC_PacketEntities
    {
        std::unique_ptr<uint8_t[]> data;
        uint32_t dataLengthInBits;
        uint32_t deltaFromTick;
        uint16_t maxEntries;
        uint16_t numUpdatedEntries;
        uint8_t baselineIndex;
        bool updateBaseline;
        bool isDelta;
    };
}

DECLARE_NET_HANDLERS(SVC_PacketEntities);
