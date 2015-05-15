
#pragma once

#include "nethandlers.h"
#include <memory>

namespace NetMsg
{
    struct SVC_VoiceData
    {
        uint8_t fromClientIndex;
        bool proximity;          // stored in a byte
        uint16_t dataLengthInBits;
        std::unique_ptr<uint8_t[]> data;
    };
}

DECLARE_NET_HANDLERS(SVC_VoiceData);
