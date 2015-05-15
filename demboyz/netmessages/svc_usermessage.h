
#pragma once

#include "nethandlers.h"
#include <memory>

namespace NetMsg
{
    struct SVC_UserMessage
    {
        std::unique_ptr<uint8_t[]> data;
        uint16_t dataLengthInBits;
        uint8_t msgType;
    };
}

DECLARE_NET_HANDLERS(SVC_UserMessage);
