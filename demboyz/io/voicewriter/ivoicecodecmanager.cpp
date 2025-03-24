
#include "ivoicecodecmanager.h"
#include "steamvoicecodecmanager.h"
#include "celtcodecmanager.h"
#include "vaudiocodecmanager.h"

#include <string.h>

#define USE_VAUDIO_CELT 0 && _WIN32
#define USE_VAUDIO_SPEEX 1 && _WIN32

IVoiceCodec::~IVoiceCodec()
{
}

IVoiceCodecManager::~IVoiceCodecManager()
{
}

IVoiceCodecManager* IVoiceCodecManager::Create(const char* codec)
{
	if (!strcmp(codec, "vaudio_speex"))
	{
#if USE_VAUDIO_SPEEX
		return CreateVAudioSpeexCodecManager();
#else
		return CreateSpeexCodecManager();
#endif
	}
	else if (!strcmp(codec, "vaudio_celt"))
	{
#if USE_VAUDIO_CELT
		return CreateVAudioCeltCodecManager();
#else
		return CreateCeltCodecManager();
#endif
	}
	else if (!strcmp(codec, "steam"))
	{
		return CreateSteamCodecManager();
	}
	return nullptr;
}
