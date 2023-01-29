
#include "steamvoicecodecmanager.h"

#include "ivoicecodecmanager.h"
#include "steam/steam_api.h"
#include <assert.h>
#include <stdio.h>

class SteamVoiceCodec : public IVoiceCodec
{
public:
    SteamVoiceCodec(ISteamUser* steamUser);
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
};

SteamVoiceCodec::SteamVoiceCodec(ISteamUser* steamUser) :
    mSteamUser(steamUser)
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
    volatile uint32 sampleRate = mSteamUser->GetVoiceOptimalSampleRate();
    EVoiceResult result = mSteamUser->DecompressVoice(
        compressedData, compressedBytes, destBuffer, destBufferSize, &numBytesWritten, 22050);
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
};

IVoiceCodecManager* CreateSteamCodecManager()
{
    return new SteamCodecManager;
}

SteamCodecManager::SteamCodecManager() :
    mSteamClient(nullptr),
    mPipe(0),
    mLocalUser(0),
    mSteamUser(nullptr)
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
    return new SteamVoiceCodec(mSteamUser);
}

int32_t SteamCodecManager::GetSampleRate()
{
    return 22050;
}
