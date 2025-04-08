
#include "celtcodecmanager.h"

#include "ivoicecodecmanager.h"
#include "celt/celt.h"
#include <assert.h>

struct CeltConfig
{
    celt_int32 sampleRate;
    uint32_t frameSizeSamples;
    uint32_t encodedFrameSizeBytes;
};

static CeltConfig sCeltConfigs[] =
{
    { 44100, 256, 120 },    // unused
    { 22050, 120, 60 },     // unused
    { 22050, 256, 60 },     // unused
    { 22050, 512, 64 },     // vaudio_celt
    { 44100, 1024, 128 }    // vaudio_celt_high
};
static const int sCeltChannels = 1;

class CeltVoiceCodec : public IVoiceCodec
{
public:
    CeltVoiceCodec(CELTMode* celtMode, int quality);
    virtual ~CeltVoiceCodec();

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
        int error = celt_decode(mCeltDecoder, compressedData, mEncodedFrameSizeBytes, (celt_int16*)destBuffer, mFrameSizeSamples);
        assert(error >= CELT_OK);
    }

private:
    CELTMode* mCeltMode;
    CELTDecoder* mCeltDecoder;
    uint32_t mFrameSizeSamples;
    uint32_t mFrameSizeBytes;
    uint32_t mEncodedFrameSizeBytes;
    int mQuality;
};

CeltVoiceCodec::CeltVoiceCodec(CELTMode* celtMode, int quality) :
    mCeltMode(celtMode),
    mCeltDecoder(nullptr),
    mFrameSizeSamples(0),
    mFrameSizeBytes(0),
    mEncodedFrameSizeBytes(0),
    mQuality(quality)
{
}

CeltVoiceCodec::~CeltVoiceCodec()
{
}

bool CeltVoiceCodec::Init()
{
    const CeltConfig& config = sCeltConfigs[mQuality];
    mFrameSizeSamples = config.frameSizeSamples;
    mFrameSizeBytes = config.frameSizeSamples * sCeltChannels * sizeof(int16_t);
    mEncodedFrameSizeBytes = config.encodedFrameSizeBytes;

    int error = CELT_OK;
    CELTDecoder* celtDecoder = celt_decoder_create_custom(mCeltMode, sCeltChannels, &error);
    assert(error == CELT_OK);
    assert(celtDecoder);

    mCeltDecoder = celtDecoder;
    return true;
}

void CeltVoiceCodec::Destroy()
{
    celt_decoder_destroy(mCeltDecoder);
    mCeltDecoder = nullptr;
}

uint32_t CeltVoiceCodec::Decompress(
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

class CeltCodecManager : public IVoiceCodecManager
{
public:
    CeltCodecManager(bool bHighQuality);
    virtual ~CeltCodecManager();

    virtual bool Init();
    virtual void Destroy();
    virtual IVoiceCodec* CreateVoiceCodec();
    virtual int32_t GetSampleRate();

private:
    CELTMode* m_celtMode;
    int m_Quality;
};

IVoiceCodecManager* CreateCeltCodecManager(bool bHighQuality)
{
    return new CeltCodecManager(bHighQuality);
}

CeltCodecManager::CeltCodecManager(bool bHighQuality) :
    m_celtMode(nullptr),
    m_Quality(bHighQuality ? 4 : 3)
{
}

CeltCodecManager::~CeltCodecManager()
{
}

bool CeltCodecManager::Init()
{
    const CeltConfig& config = sCeltConfigs[m_Quality];

    int error = CELT_OK;
    m_celtMode = celt_mode_create(config.sampleRate, config.frameSizeSamples, &error);
    assert(error == CELT_OK);
    assert(m_celtMode);

    return true;
}

void CeltCodecManager::Destroy()
{
    if (m_celtMode)
    {
        celt_mode_destroy(m_celtMode);
        m_celtMode = nullptr;
    }
}

IVoiceCodec* CeltCodecManager::CreateVoiceCodec()
{
    return new CeltVoiceCodec(m_celtMode, m_Quality);
}

int32_t CeltCodecManager::GetSampleRate()
{
    return sCeltConfigs[m_Quality].sampleRate;
}
