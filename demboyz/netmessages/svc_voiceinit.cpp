
#include "svc_voiceinit.h"
#include "base/bitfile.h"
#include "base/jsonfile.h"

namespace NetHandlers
{
    bool SVC_VoiceInit_BitRead_Internal(BitRead& bitbuf, SourceGameContext& context, NetMsg::SVC_VoiceInit* data)
    {
        bitbuf.ReadString(data->voiceCodec, sizeof(data->voiceCodec));
        data->quality = bitbuf.ReadByte();
        return !bitbuf.IsOverflowed();
    }

    bool SVC_VoiceInit_BitWrite_Internal(BitWrite& bitbuf, const SourceGameContext& context, NetMsg::SVC_VoiceInit* data)
    {
        bitbuf.WriteString(data->voiceCodec);
        bitbuf.WriteByte(data->quality);
        return !bitbuf.IsOverflowed();
    }

    bool SVC_VoiceInit_JsonRead_Internal(JsonRead& jsonbuf, SourceGameContext& context, NetMsg::SVC_VoiceInit* data)
    {
        return true;
    }

    bool SVC_VoiceInit_JsonWrite_Internal(JsonWrite& jsonbuf, const SourceGameContext& context, NetMsg::SVC_VoiceInit* data)
    {
        jsonbuf.StartObject("svc_voiceinit");
        jsonbuf.WriteString("voiceCodec", data->voiceCodec);
        jsonbuf.WriteUInt32("quality", data->quality);
        jsonbuf.EndObject();
        return true;
    }

    void SVC_VoiceInit_ToString_Internal(std::ostringstream& out, NetMsg::SVC_VoiceInit* data)
    {
        out << "svc_VoiceInit: codec \"" << data->voiceCodec
            << "\", qualitty " << static_cast<uint32_t>(data->quality);
    }
}
