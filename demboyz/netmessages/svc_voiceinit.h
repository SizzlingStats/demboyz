
#pragma once

#include "nethandlers.h"

namespace NetMsg
{
    struct SVC_VoiceInit
    {
        static const uint8_t QUALITY_HAS_SAMPLE_RATE = 255;

        char voiceCodec[MAX_OSPATH];    // used voice codec .dll
        uint8_t quality;                // custom quality setting
        int32_t sampleRate;             // Hz
    };
}

DECLARE_NET_HANDLERS(SVC_VoiceInit);
