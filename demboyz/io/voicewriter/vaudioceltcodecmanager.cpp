
#include "vaudioceltcodecmanager.h"

#ifdef _WIN32

#include "ivoicecodecmanager.h"

#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <assert.h>

static int32_t sSamplingRates[] =
{
    { 44100 },  // unused
    { 22050 },  // unused
    { 22050 },  // unused
    { 22050 },  // vaudio_celt
    { 44100 }   // vaudio_celt_high
};

static const int sQuality = 3;
typedef void* (CreateInterfaceFn)(const char* pName, int* pReturnCode);

// SDK Interface
class IVAudioVoiceCodec
{
protected:
    virtual ~IVAudioVoiceCodec() {}

public:
    // Initialize the object. The uncompressed format is always 8-bit signed mono.
    virtual bool Init(int quality) = 0;

    // Use this to delete the object.
    virtual void Release() = 0;

    // Compress the voice data.
    // pUncompressed        -   16-bit signed mono voice data.
    // maxCompressedBytes   -   The length of the pCompressed buffer. Don't exceed this.
    // bFinal               -   Set to true on the last call to Compress (the user stopped talking).
    //                          Some codecs like big block sizes and will hang onto data you give them in Compress calls.
    //                          When you call with bFinal, the codec will give you compressed data no matter what.
    // Return the number of bytes you filled into pCompressed.
    virtual int Compress(const char* pUncompressed, int nSamples, char* pCompressed, int maxCompressedBytes, bool bFinal) = 0;

    // Decompress voice data. pUncompressed is 16-bit signed mono.
    virtual int Decompress(const char* pCompressed, int compressedBytes, char* pUncompressed, int maxUncompressedBytes) = 0;

    // Some codecs maintain state between Compress and Decompress calls. This should clear that state.
    virtual bool ResetState() = 0;
};

class VAudioVoiceCodec : public IVoiceCodec
{
public:
    VAudioVoiceCodec(IVAudioVoiceCodec* codec);
    virtual ~VAudioVoiceCodec();

    virtual bool Init();
    virtual void Destroy();

    virtual uint32_t Decompress(
        const uint8_t* compressedData,
        uint32_t compressedBytes,
        uint8_t* destBuffer,
        uint32_t destBufferSize
    );

private:
    IVAudioVoiceCodec* mCodec;
};

VAudioVoiceCodec::VAudioVoiceCodec(IVAudioVoiceCodec* codec) :
    mCodec(codec)
{
}

VAudioVoiceCodec::~VAudioVoiceCodec()
{
}

bool VAudioVoiceCodec::Init()
{
    return mCodec->Init(sQuality);
}

void VAudioVoiceCodec::Destroy()
{
    mCodec->Release();
}

uint32_t VAudioVoiceCodec::Decompress(
    const uint8_t* compressedData,
    uint32_t compressedBytes,
    uint8_t* destBuffer,
    uint32_t destBufferSize)
{
    uint32_t numBytesWritten = mCodec->Decompress((const char*)compressedData, compressedBytes, (char*)destBuffer, destBufferSize);
    return numBytesWritten * sizeof(int16_t);
}

class VAudioCeltCodecManager : public IVoiceCodecManager
{
public:
    VAudioCeltCodecManager();
    virtual ~VAudioCeltCodecManager();

    virtual bool Init(uint8_t quality, int32_t sampleRate);
    virtual void Destroy();
    virtual IVoiceCodec* CreateVoiceCodec();
    virtual int32_t GetSampleRate();

private:
    HINSTANCE mVAudioCeltDll;
    CreateInterfaceFn* mIVAudioVoiceCodecCreateInterfaceFunc;
};

IVoiceCodecManager* CreateVAudioCeltCodecManager()
{
    return new VAudioCeltCodecManager;
}

VAudioCeltCodecManager::VAudioCeltCodecManager() :
    mVAudioCeltDll(nullptr),
    mIVAudioVoiceCodecCreateInterfaceFunc(nullptr)
{
}

VAudioCeltCodecManager::~VAudioCeltCodecManager()
{
}

bool VAudioCeltCodecManager::Init(uint8_t quality, int32_t sampleRate)
{
    mVAudioCeltDll = LoadLibrary(TEXT("vaudio_celt.dll"));
    assert(mVAudioCeltDll);

    mIVAudioVoiceCodecCreateInterfaceFunc = (CreateInterfaceFn*)GetProcAddress(mVAudioCeltDll, "CreateInterface");
    assert(mIVAudioVoiceCodecCreateInterfaceFunc);

    return true;
}

void VAudioCeltCodecManager::Destroy()
{
    mIVAudioVoiceCodecCreateInterfaceFunc = nullptr;
    FreeLibrary(mVAudioCeltDll);
    mVAudioCeltDll = nullptr;
}

IVoiceCodec* VAudioCeltCodecManager::CreateVoiceCodec()
{
    assert(mIVAudioVoiceCodecCreateInterfaceFunc);

    int ret = 0;
    IVAudioVoiceCodec* codec = (IVAudioVoiceCodec*)mIVAudioVoiceCodecCreateInterfaceFunc("vaudio_celt", &ret);
    assert(codec);

    return new VAudioVoiceCodec(codec);
}

int32_t VAudioCeltCodecManager::GetSampleRate()
{
    return sSamplingRates[sQuality];
}

#else // _WIN32

IVoiceCodecManager* CreateVAudioCeltCodecManager()
{
    return nullptr;
}

#endif
