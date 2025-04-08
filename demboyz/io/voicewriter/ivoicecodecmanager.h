
#pragma once

#include <cstdint>

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

enum class VoiceCodec : uint8_t
{
    Unknown = 0,
    Steam,
    Speex,
    Celt,
    Celt_High
};

class IVoiceCodecManager
{
public:
    virtual ~IVoiceCodecManager();

    virtual bool Init() = 0;
    virtual void Destroy() = 0;

    virtual IVoiceCodec* CreateVoiceCodec() = 0;
    virtual int32_t GetSampleRate() = 0;

public:
    static IVoiceCodecManager* Create(
        VoiceCodec codec,
        const uint8_t* compressedData,
        uint32_t compressedBytes);
};
