
#include "ivoicecodecmanager.h"
#include "steamvoicecodecmanager.h"
#include "celtcodecmanager.h"
#include "vaudiocodecmanager.h"

#include <cstdio>

#define USE_VAUDIO_CELT 0 && _WIN32
#define USE_VAUDIO_SPEEX 1 && _WIN32

IVoiceCodec::~IVoiceCodec()
{
}

IVoiceCodecManager::~IVoiceCodecManager()
{
}

IVoiceCodecManager* IVoiceCodecManager::Create(
	VoiceCodec codec,
	const uint8_t* compressedData,
	uint32_t compressedBytes)
{
	if (codec != VoiceCodec::Steam)
	{
		if (IsValidSteamVoicePacket(compressedData, compressedBytes))
		{
			printf("VoiceWriter: overriding codec %i as Steam Voice.\n", codec);
			codec = VoiceCodec::Steam;
		}
	}
	else
	{
		printf("VoiceWriter: codec %i.\n", codec);
	}

	if (codec == VoiceCodec::Steam)
	{
		return CreateSteamCodecManager();
	}
	else if (codec == VoiceCodec::Speex)
	{
#if USE_VAUDIO_SPEEX
		return CreateVAudioSpeexCodecManager();
#else
		return nullptr;// CreateSpeexCodecManager();
#endif
	}
	else if (codec == VoiceCodec::Celt || codec == VoiceCodec::Celt_High)
	{
		const bool bHighQuality = (codec == VoiceCodec::Celt_High);
#if USE_VAUDIO_CELT
		return CreateVAudioCeltCodecManager(bHighQuality);
#else
		return CreateCeltCodecManager(bHighQuality);
#endif
	}
	return nullptr;
}
