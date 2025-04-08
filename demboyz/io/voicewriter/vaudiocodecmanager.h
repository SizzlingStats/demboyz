
#pragma once

class IVoiceCodecManager;
IVoiceCodecManager* CreateVAudioCeltCodecManager(bool bHighQuality);
IVoiceCodecManager* CreateVAudioSpeexCodecManager();
