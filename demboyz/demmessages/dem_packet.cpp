
#include "dem_packet.h"
#include "demofile/demofile.h"

namespace DemHandlers
{
    bool Dem_Packet_FileRead_Internal(FileRead& demofile, DemMsg::Dem_Packet* data)
    {
        demofile.ReadCmdInfo(data->cmdInfo);
        demofile.ReadSequenceInfo(data->sequenceNum1, data->sequenceNum2);
        //data->dataLengthInBytes = demofile.ReadRawData(data->data, sizeof(data->data));
        return demofile.IsOk();
    }

    bool Dem_Packet_FileWrite_Internal(FileWrite& demofile, DemMsg::Dem_Packet* data)
    {
        demofile.WriteCmdInfo(data->cmdInfo);
        demofile.WriteSequenceInfo(data->sequenceNum1, data->sequenceNum2);
        //demofile.WriteRawData(data->data, data->dataLengthInBytes);
        return demofile.IsOk();
    }

    bool Dem_Packet_JsonRead_Internal(JsonRead& jsonbuf, DemMsg::Dem_Packet* data)
    {
        return true;
    }

    bool Dem_Packet_JsonWrite_Internal(JsonWrite& jsonbuf, DemMsg::Dem_Packet* data)
    {
        return true;
    }
}
