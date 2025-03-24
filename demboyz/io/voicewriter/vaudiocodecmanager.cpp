
#include "vaudiocodecmanager.h"

#ifdef _WIN32

#include "ivoicecodecmanager.h"

#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <assert.h>

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
    VAudioVoiceCodec(IVAudioVoiceCodec* codec, int quality);
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
    int mQuality;
};

VAudioVoiceCodec::VAudioVoiceCodec(IVAudioVoiceCodec* codec, int quality) :
    mCodec(codec),
    mQuality(quality)
{
}

VAudioVoiceCodec::~VAudioVoiceCodec()
{
}

bool VAudioVoiceCodec::Init()
{
    return mCodec->Init(mQuality);
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

class VAudioCodecManager : public IVoiceCodecManager
{
public:
    VAudioCodecManager(int32_t sampleRate, int quality) :
        mVAudioDll(nullptr),
        mIVAudioVoiceCodecCreateInterfaceFunc(nullptr),
        mSampleRate(sampleRate),
        mQuality(quality)
    {
    }

    virtual bool Init(uint8_t quality, int32_t sampleRate)
    {
        mVAudioDll = LoadLibraryA(GetCodecDll());
        assert(mVAudioDll);

        mIVAudioVoiceCodecCreateInterfaceFunc = (CreateInterfaceFn*)GetProcAddress(mVAudioDll, "CreateInterface");
        assert(mIVAudioVoiceCodecCreateInterfaceFunc);

        // Do not trust the sample rate or quality from the packets
        //mSampleRate = sampleRate;
        //mQuality = quality;

        return true;
    }

    virtual void Destroy()
    {
        mIVAudioVoiceCodecCreateInterfaceFunc = nullptr;
        FreeLibrary(mVAudioDll);
        mVAudioDll = nullptr;
    }

    virtual IVoiceCodec* CreateVoiceCodec()
    {
        assert(mIVAudioVoiceCodecCreateInterfaceFunc);

        int ret = 0;
        IVAudioVoiceCodec* codec = (IVAudioVoiceCodec*)mIVAudioVoiceCodecCreateInterfaceFunc(GetCodecName(), &ret);
        assert(codec);

        return new VAudioVoiceCodec(codec, mQuality);
    }

    virtual int32_t GetSampleRate()
    {
        return mSampleRate;
    }

protected:
    virtual const char* GetCodecDll() const = 0;
    virtual const char* GetCodecName() const = 0;

private:
    HINSTANCE mVAudioDll;
    CreateInterfaceFn* mIVAudioVoiceCodecCreateInterfaceFunc;

    int32_t mSampleRate;
    int mQuality;
};

class VAudioCeltCodecManager : public VAudioCodecManager
{
public:
    VAudioCeltCodecManager(int32_t sampleRate, int quality) : VAudioCodecManager(sampleRate, quality) {}
protected:
    virtual const char* GetCodecDll() const { return "vaudio_celt.dll"; }
    virtual const char* GetCodecName() const { return "vaudio_celt"; }
};

class VAudioSpeexCodecManager : public VAudioCodecManager
{
public:
    VAudioSpeexCodecManager(int32_t sampleRate, int quality) : VAudioCodecManager(sampleRate, quality) {}
protected:
    virtual const char* GetCodecDll() const { return "vaudio_speex.dll"; }
    virtual const char* GetCodecName() const { return "vaudio_speex"; }
};

IVoiceCodecManager* CreateVAudioCeltCodecManager()
{
    const int32_t sSamplingRates[] =
    {
        { 44100 },  // unused
        { 22050 },  // unused
        { 22050 },  // unused
        { 22050 },  // vaudio_celt
        { 44100 }   // vaudio_celt_high
    };
    const int sQuality = 3;

    return new VAudioCeltCodecManager(sSamplingRates[sQuality], sQuality);
}

IVoiceCodecManager* CreateVAudioSpeexCodecManager()
{
    const int32_t sSamplingRate = 11025;
    const int sQuality = 4;

    return new VAudioSpeexCodecManager(sSamplingRate, sQuality);
}

#else // _WIN32

IVoiceCodecManager* CreateVAudioCeltCodecManager()
{
    return nullptr;
}

IVoiceCodecManager* CreateVAudioSpeexCodecManager()
{
    return nullptr;
}

#endif
