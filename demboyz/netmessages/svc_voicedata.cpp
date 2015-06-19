
#include "svc_voicedata.h"
#include "sourcesdk/bitbuf.h"
#include "netmath.h"

namespace NetHandlers
{
    bool SVC_VoiceData_BitRead_Internal(BitRead& bitbuf, SourceGameContext& context, NetMsg::SVC_VoiceData* data)
    {
        data->fromClientIndex = bitbuf.ReadByte();
        data->proximity = !!bitbuf.ReadByte();
        data->dataLengthInBits = bitbuf.ReadWord();
        data->data.reset(new uint8_t[math::BitsToBytes(data->dataLengthInBits)]);
        bitbuf.ReadBits(data->data.get(), data->dataLengthInBits);
        return !bitbuf.IsOverflowed();
    }

    bool SVC_VoiceData_BitWrite_Internal(BitWrite& bitbuf, const SourceGameContext& context, NetMsg::SVC_VoiceData* data)
    {
        bitbuf.WriteByte(data->fromClientIndex);
        bitbuf.WriteByte(data->proximity);
        bitbuf.WriteWord(data->dataLengthInBits);
        bitbuf.WriteBits(data->data.get(), data->dataLengthInBits);
        return !bitbuf.IsOverflowed();
    }

    bool SVC_VoiceData_JsonRead_Internal(JsonRead& jsonbuf, SourceGameContext& context, NetMsg::SVC_VoiceData* data)
    {
        return true;
    }

    bool SVC_VoiceData_JsonWrite_Internal(JsonWrite& jsonbuf, const SourceGameContext& context, NetMsg::SVC_VoiceData* data)
    {
        return true;
    }

    void SVC_VoiceData_ToString_Internal(std::ostringstream& out, NetMsg::SVC_VoiceData* data)
    {
        out << "svc_VoiceData: client " << static_cast<uint32_t>(data->fromClientIndex)
            << ", bytes " << math::BitsToBytes(data->dataLengthInBits);
    }
}
