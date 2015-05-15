
#pragma once

#include "nethandlers.h"
#include <memory>

namespace NetMsg
{
    struct SVC_SendTable
    {
        bool needsDecoder;
        int16_t dataLengthInBits;
        std::unique_ptr<uint8_t[]> data;
    };
}

DECLARE_NET_HANDLERS(SVC_SendTable);
