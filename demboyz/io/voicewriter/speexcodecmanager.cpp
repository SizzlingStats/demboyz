
#include "speexcodecmanager.h"

#include "ivoicecodecmanager.h"
#include "speex/speex.h"
#include <assert.h>

class SpeexVoiceCodec : public IVoiceCodec
{
public:
    SpeexVoiceCodec();
    virtual ~SpeexVoiceCodec();

    virtual bool Init();
    virtual void Destroy();

    virtual uint32_t Decompress(
        const uint8_t* compressedData,
        uint32_t compressedBytes,
        uint8_t* destBuffer,
        uint32_t destBufferSize
    );

private:
    void DecodeFrame(const uint8_t* compressedData, uint8_t* destBuffer)
    {
        speex_bits_read_from(&mBits, (const char*)compressedData, mEncodedFrameSizeBytes);
        const int error = speex_decode_int(mDecoderState, &mBits, (spx_int16_t*)destBuffer);
        assert(error == 0);
    }

private:
    SpeexBits mBits;
    void* mDecoderState;

    uint32_t mFrameSizeSamples;
    uint32_t mFrameSizeBytes;
    uint32_t mEncodedFrameSizeBytes;
};

SpeexVoiceCodec::SpeexVoiceCodec() :
    mBits(),
    mDecoderState(nullptr),
    mFrameSizeSamples(160),
    mFrameSizeBytes(320),
    mEncodedFrameSizeBytes(28)
{
}

SpeexVoiceCodec::~SpeexVoiceCodec()
{
}

bool SpeexVoiceCodec::Init()
{
    speex_bits_init(&mBits);
    mDecoderState = speex_decoder_init(&speex_nb_mode);
    assert(mDecoderState);

    spx_int32_t frame_size = 0;
    speex_decoder_ctl(mDecoderState, SPEEX_GET_FRAME_SIZE, &frame_size);
    assert(frame_size == mFrameSizeSamples);
    assert((frame_size * 2) == mFrameSizeBytes);

    spx_int32_t sampling_rate = 0;
    speex_decoder_ctl(mDecoderState, SPEEX_GET_SAMPLING_RATE, &sampling_rate);
    assert(sampling_rate == 8000);

    spx_int32_t perceptual_enhancer = 1;
    speex_decoder_ctl(mDecoderState, SPEEX_SET_ENH, &perceptual_enhancer);

    return true;
}

void SpeexVoiceCodec::Destroy()
{
    speex_decoder_destroy(mDecoderState);
    mDecoderState = nullptr;

    speex_bits_destroy(&mBits);
}

uint32_t SpeexVoiceCodec::Decompress(
    const uint8_t* compressedData,
    uint32_t compressedBytes,
    uint8_t* destBuffer,
    uint32_t destBufferSize)
{
    int curCompressedByte = 0;
    int curDecompressedByte = 0;

    const uint32_t encodedframeSizeBytes = mEncodedFrameSizeBytes;
    const uint32_t frameSizeBytes = mFrameSizeBytes;
    while (
        ((compressedBytes - curCompressedByte) >= encodedframeSizeBytes) &&
        ((destBufferSize - curDecompressedByte) >= frameSizeBytes))
    {
        DecodeFrame(&compressedData[curCompressedByte], &destBuffer[curDecompressedByte]);
        curCompressedByte += encodedframeSizeBytes;
        curDecompressedByte += frameSizeBytes;
    }
    return curDecompressedByte;
}

class SpeexCodecManager : public IVoiceCodecManager
{
public:
    SpeexCodecManager();
    virtual ~SpeexCodecManager();

    virtual bool Init();
    virtual void Destroy();
    virtual IVoiceCodec* CreateVoiceCodec();
    virtual int32_t GetSampleRate();
};

IVoiceCodecManager* CreateSpeexCodecManager()
{
    return new SpeexCodecManager();
}

SpeexCodecManager::SpeexCodecManager()
{
}

SpeexCodecManager::~SpeexCodecManager()
{
}

bool SpeexCodecManager::Init()
{
    return true;
}

void SpeexCodecManager::Destroy()
{
}

IVoiceCodec* SpeexCodecManager::CreateVoiceCodec()
{
    return new SpeexVoiceCodec;
}

int32_t SpeexCodecManager::GetSampleRate()
{
    return 11025;
}
