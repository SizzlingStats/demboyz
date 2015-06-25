
#include "svc_crosshairangle.h"
#include "base/bitfile.h"
#include "base/jsonfile.h"
#include <iomanip>

namespace NetHandlers
{
    bool SVC_CrosshairAngle_BitRead_Internal(BitRead& bitbuf, SourceGameContext& context, NetMsg::SVC_CrosshairAngle* data)
    {
        data->x = bitbuf.ReadBitAngle(16);
        data->y = bitbuf.ReadBitAngle(16);
        data->z = bitbuf.ReadBitAngle(16);
        return !bitbuf.IsOverflowed();
    }

    bool SVC_CrosshairAngle_BitWrite_Internal(BitWrite& bitbuf, const SourceGameContext& context, NetMsg::SVC_CrosshairAngle* data)
    {
        bitbuf.WriteBitAngle(data->x, 16);
        bitbuf.WriteBitAngle(data->y, 16);
        bitbuf.WriteBitAngle(data->z, 16);
        return !bitbuf.IsOverflowed();
    }

    bool SVC_CrosshairAngle_JsonRead_Internal(JsonRead& jsonbuf, SourceGameContext& context, NetMsg::SVC_CrosshairAngle* data)
    {
        return true;
    }

    bool SVC_CrosshairAngle_JsonWrite_Internal(JsonWrite& jsonbuf, const SourceGameContext& context, NetMsg::SVC_CrosshairAngle* data)
    {
        jsonbuf.StartObject("svc_crosshairangle");
        jsonbuf.StartObject("angle");
        jsonbuf.WriteFloat("pitch", data->x);
        jsonbuf.WriteFloat("yaw", data->y);
        jsonbuf.WriteFloat("roll", data->z);
        jsonbuf.EndObject();
        jsonbuf.EndObject();
        return true;
    }

    void SVC_CrosshairAngle_ToString_Internal(std::ostringstream& out, NetMsg::SVC_CrosshairAngle* data)
    {
        const std::streamsize oldPrecision = out.precision();
        out << "svc_CrosshairAngle:"
            << std::setprecision(1) << std::fixed
            << " (" << data->x
            << " " << data->y
            << " " << data->z << ")"
            << std::setprecision(oldPrecision);
        out.unsetf(std::ios_base::floatfield);
    }
}
