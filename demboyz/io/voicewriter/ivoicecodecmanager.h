
#pragma once

#include <stdint.h>

class IVoiceCodec
{
public:
    virtual ~IVoiceCodec();

    virtual bool Init() = 0;
    virtual void Destroy() = 0;

    // Returns num bytes written to destBuffer
    virtual uint32_t Decompress(
        const uint8_t* compressedData,
        uint32_t compressedBytes,
        uint8_t* destBuffer,
        uint32_t destBufferSize
    ) = 0;
};

class IVoiceCodecManager
{
public:
    virtual ~IVoiceCodecManager();

    virtual bool Init(uint8_t quality, int32_t sampleRate) = 0;
    virtual void Destroy() = 0;
    virtual IVoiceCodec* CreateVoiceCodec() = 0;
    virtual int32_t GetSampleRate() = 0;

public:
    static IVoiceCodecManager* Create(const char* codec);
};
