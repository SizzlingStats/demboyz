
#include "svc_voiceinit.h"
#include "bitbuf.h"

namespace NetHandlers
{
    bool SVC_VoiceInit_BitRead_Internal(bf_read& bitbuf, SourceGameContext& context, NetMsg::SVC_VoiceInit* data)
    {
        bitbuf.ReadString(data->voiceCodec, sizeof(data->voiceCodec));
        data->quality = bitbuf.ReadByte();
        return !bitbuf.IsOverflowed();
    }

    bool SVC_VoiceInit_BitWrite_Internal(bf_write& bitbuf, SourceGameContext& context, NetMsg::SVC_VoiceInit* data)
    {
        bitbuf.WriteString(data->voiceCodec);
        bitbuf.WriteByte(data->quality);
        return !bitbuf.IsOverflowed();
    }

    bool SVC_VoiceInit_JsonRead_Internal(JsonRead& jsonbuf, SourceGameContext& context, NetMsg::SVC_VoiceInit* data)
    {
        return true;
    }

    bool SVC_VoiceInit_JsonWrite_Internal(JsonWrite& jsonbuf, SourceGameContext& context, NetMsg::SVC_VoiceInit* data)
    {
        return true;
    }

    void SVC_VoiceInit_ToString_Internal(std::ostringstream& out, NetMsg::SVC_VoiceInit* data)
    {
        out << "svc_VoiceInit: codec \"" << data->voiceCodec
            << "\", qualitty " << static_cast<uint32_t>(data->quality);
    }
}
