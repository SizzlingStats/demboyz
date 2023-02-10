
#include "steamvoicecodecmanager.h"

#include "ivoicecodecmanager.h"
#include "steam/steam_api.h"
#include <assert.h>
#include <stdio.h>

class SteamVoiceCodec : public IVoiceCodec
{
public:
    SteamVoiceCodec(ISteamUser* steamUser, uint32 sampleRate);
    virtual ~SteamVoiceCodec();

    virtual bool Init();
    virtual void Destroy();

    virtual uint32_t Decompress(
        const uint8_t* compressedData,
        uint32_t compressedBytes,
        uint8_t* destBuffer,
        uint32_t destBufferSize
    );

private:
    ISteamUser* mSteamUser;
    uint32 mSampleRate;
};

SteamVoiceCodec::SteamVoiceCodec(ISteamUser* steamUser, uint32 sampleRate) :
    mSteamUser(steamUser),
    mSampleRate(sampleRate)
{
}

SteamVoiceCodec::~SteamVoiceCodec()
{
}

bool SteamVoiceCodec::Init()
{
    return true;
}

void SteamVoiceCodec::Destroy()
{
}

uint32_t SteamVoiceCodec::Decompress(
    const uint8_t* compressedData,
    uint32_t compressedBytes,
    uint8_t* destBuffer,
    uint32_t destBufferSize)
{
    uint32 numBytesWritten = 0;
    EVoiceResult result = mSteamUser->DecompressVoice(
        compressedData, compressedBytes, destBuffer, destBufferSize, &numBytesWritten, mSampleRate);
    return numBytesWritten;
}

class SteamCodecManager : public IVoiceCodecManager
{
public:
    SteamCodecManager();
    virtual ~SteamCodecManager();

    virtual bool Init(uint8_t quality, int32_t sampleRate);
    virtual void Destroy();
    virtual IVoiceCodec* CreateVoiceCodec();
    virtual int32_t GetSampleRate();

private:
    ISteamClient* mSteamClient;

    HSteamPipe mPipe;
    HSteamUser mLocalUser;
    ISteamUser* mSteamUser;
    uint32 mSampleRate;
};

IVoiceCodecManager* CreateSteamCodecManager()
{
    return new SteamCodecManager;
}

SteamCodecManager::SteamCodecManager() :
    mSteamClient(nullptr),
    mPipe(0),
    mLocalUser(0),
    mSteamUser(nullptr),
    mSampleRate(0)
{
}

SteamCodecManager::~SteamCodecManager()
{
}

bool SteamCodecManager::Init(uint8_t quality, int32_t sampleRate)
{
    //assert(quality == NetMsg::SVC_VoiceInit::QUALITY_HAS_SAMPLE_RATE);
    assert(sampleRate == 0);

    if (!SteamAPI_Init())
    {
        printf("SteamAPI_Init failed. Ensure Steam is running and logged in.\n");
        assert(false);
        return false;
    }

    mSteamClient = SteamClient();

    mPipe = mSteamClient->CreateSteamPipe();
    mLocalUser = mSteamClient->ConnectToGlobalUser(mPipe);
    mSteamUser = mSteamClient->GetISteamUser(mLocalUser, mPipe, STEAMUSER_INTERFACE_VERSION);
    assert(mSteamUser);

    mSampleRate = mSteamUser->GetVoiceOptimalSampleRate();

    return mSteamUser;
}

void SteamCodecManager::Destroy()
{
    mSteamClient->BReleaseSteamPipe(mPipe);
    
    mSteamClient = nullptr;
    mPipe = 0;
    mLocalUser = 0;
    mSteamUser = nullptr;

    SteamAPI_Shutdown();
}

IVoiceCodec* SteamCodecManager::CreateVoiceCodec()
{
    return new SteamVoiceCodec(mSteamUser, mSampleRate);
}

int32_t SteamCodecManager::GetSampleRate()
{
    return mSampleRate;
}
