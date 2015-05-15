
#pragma once

#include "nethandlers.h"
#include <memory>

namespace NetMsg
{
    struct SVC_EntityMessage
    {
        std::unique_ptr<uint8_t[]> data;
        uint16_t dataLengthInBits;
        uint16_t entIndex;
        uint16_t classID;
    };
}

DECLARE_NET_HANDLERS(SVC_EntityMessage);
