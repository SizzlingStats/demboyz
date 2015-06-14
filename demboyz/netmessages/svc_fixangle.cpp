
#include "svc_fixangle.h"
#include "sourcesdk/bitbuf.h"
#include <iomanip>

namespace NetHandlers
{
    bool SVC_FixAngle_BitRead_Internal(bf_read& bitbuf, SourceGameContext& context, NetMsg::SVC_FixAngle* data)
    {
        data->relative = bitbuf.ReadOneBit() != 0;
        data->x = bitbuf.ReadBitAngle(16);
        data->y = bitbuf.ReadBitAngle(16);
        data->z = bitbuf.ReadBitAngle(16);
        return !bitbuf.IsOverflowed();
    }

    bool SVC_FixAngle_BitWrite_Internal(bf_write& bitbuf, const SourceGameContext& context, NetMsg::SVC_FixAngle* data)
    {
        bitbuf.WriteOneBit(data->relative);
        bitbuf.WriteBitAngle(data->x, 16);
        bitbuf.WriteBitAngle(data->y, 16);
        bitbuf.WriteBitAngle(data->z, 16);
        return !bitbuf.IsOverflowed();
    }

    bool SVC_FixAngle_JsonRead_Internal(JsonRead& jsonbuf, SourceGameContext& context, NetMsg::SVC_FixAngle* data)
    {
        return true;
    }

    bool SVC_FixAngle_JsonWrite_Internal(JsonWrite& jsonbuf, const SourceGameContext& context, NetMsg::SVC_FixAngle* data)
    {
        return true;
    }

    void SVC_FixAngle_ToString_Internal(std::ostringstream& out, NetMsg::SVC_FixAngle* data)
    {
        const std::streamsize oldPrecision = out.precision();
        out << "svc_FixAngle: " << (data->relative ? "relative" : "absolute")
            << std::setprecision(1) << std::fixed
            << " " << data->x
            << " " << data->y
            << " " << data->z
            << std::setprecision(oldPrecision);
        out.unsetf(std::ios_base::floatfield);
    }
}
