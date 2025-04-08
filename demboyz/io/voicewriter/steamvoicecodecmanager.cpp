
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

// ----------------------------- crc32b --------------------------------

/* This is the basic CRC-32 calculation with some optimization but no
table lookup. The the byte reversal is avoided by shifting the crc reg
right instead of left and by using a reversed 32-bit word to represent
the polynomial.
   When compiled to Cyclops with GCC, this function executes in 8 + 72n
instructions, where n is the number of bytes in the input message. It
should be doable in 4 + 61n instructions.
   If the inner loop is strung out (approx. 5*8 = 40 instructions),
it would take about 6 + 46n instructions. */

static uint32_t crc32b(const uint8_t* data, uint32_t length)
{
    uint32_t crc = 0xFFFFFFFF;
    for (uint32_t i = 0; i < length; ++i)
    {
        uint32_t byte = data[i];
        crc = crc ^ byte;
        for (uint32_t j = 0; j < 8; ++j)
        {
            const uint32_t mask = -static_cast<int32_t>(crc & 1);
            crc = (crc >> 1) ^ (0xEDB88320 & mask);
        }
    }
    return ~crc;
}

bool IsValidSteamVoicePacket(const uint8_t* compressedData, uint32_t compressedBytes)
{
    if (compressedBytes < (sizeof(CSteamID) + sizeof(uint32_t)))
    {
        return false;
    }

    const uint8_t* data = compressedData;
    const uint32_t dataLength = compressedBytes - sizeof(uint32_t);

    const CSteamID* steamid = reinterpret_cast<const CSteamID*>(data);
    if (!steamid->IsValid())
    {
        return false;
    }

    const uint32_t crcPacket = *reinterpret_cast<const uint32_t*>(data + dataLength);
    const uint32_t crcCalc = crc32b(data, dataLength);
    if (crcPacket != crcCalc)
    {
        return false;
    }
    return true;
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

    virtual bool Init();
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

bool SteamCodecManager::Init()
{
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
