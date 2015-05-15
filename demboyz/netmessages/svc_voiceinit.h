
#pragma once

#include "nethandlers.h"

namespace NetMsg
{
    struct SVC_VoiceInit
    {
        char voiceCodec[MAX_OSPATH];    // used voice codec .dll
        uint8_t quality;                // custom quality setting
    };
}

DECLARE_NET_HANDLERS(SVC_VoiceInit);
