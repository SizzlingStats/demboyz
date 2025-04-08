
#pragma once

#include <cstdint>

class IVoiceCodecManager;
IVoiceCodecManager* CreateSteamCodecManager();

bool IsValidSteamVoicePacket(
    const uint8_t* compressedData,
    uint32_t compressedBytes);
