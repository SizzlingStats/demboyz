
#include "svc_classinfo.h"
#include "sourcesdk/bitbuf.h"
#include "netmath.h"

using class_t = NetMsg::SVC_ClassInfo::class_t;

namespace NetHandlers
{
    bool SVC_ClassInfo_BitRead_Internal(BitRead& bitbuf, SourceGameContext& context, NetMsg::SVC_ClassInfo* data)
    {
        const int16_t numServerClasses = bitbuf.ReadShort();
        const bool createOnClient = bitbuf.ReadOneBit() != 0;
        data->numServerClasses = numServerClasses;
        data->createOnClient = createOnClient;
        if (!createOnClient)
        {
            const int numServerClassBits = math::log2(numServerClasses) + 1;
            data->serverClasses.resize(numServerClasses);
            for (class_t& serverClass : data->serverClasses)
            {
                serverClass.classID = bitbuf.ReadUBitLong(numServerClassBits);
                bitbuf.ReadString(serverClass.className, sizeof(serverClass.className));
                bitbuf.ReadString(serverClass.dataTableName, sizeof(serverClass.dataTableName));
            }
        }
        return !bitbuf.IsOverflowed();
    }

    bool SVC_ClassInfo_BitWrite_Internal(BitWrite& bitbuf, const SourceGameContext& context, NetMsg::SVC_ClassInfo* data)
    {
        bitbuf.WriteShort(data->numServerClasses);
        bitbuf.WriteOneBit(data->createOnClient);
        if (!data->createOnClient)
        {
            const int numServerClassBits = math::log2(data->numServerClasses) + 1;
            for (class_t& serverClass : data->serverClasses)
            {
                bitbuf.WriteUBitLong(serverClass.classID, numServerClassBits);
                bitbuf.WriteString(serverClass.className);
                bitbuf.WriteString(serverClass.dataTableName);
            }
        }
        return !bitbuf.IsOverflowed();
    }

    bool SVC_ClassInfo_JsonRead_Internal(JsonRead& jsonbuf, SourceGameContext& context, NetMsg::SVC_ClassInfo* data)
    {
        return true;
    }

    bool SVC_ClassInfo_JsonWrite_Internal(JsonWrite& jsonbuf, const SourceGameContext& context, NetMsg::SVC_ClassInfo* data)
    {
        return true;
    }

    void SVC_ClassInfo_ToString_Internal(std::ostringstream& out, NetMsg::SVC_ClassInfo* data)
    {
        out << "svc_ClassInfo: num " << data->numServerClasses
            << ", " << (data->createOnClient ? "use client classes" : "full update");
    }
}
