
#include "dem_packet.h"
#include "demofile/demofile.h"
#include "demofile/demojson.h"
#include "netmessages/nethandlers.h"

namespace DemHandlers
{
    bool Dem_Packet_FileRead_Internal(FileRead& demofile, DemMsg::Dem_Packet* data)
    {
        demofile.ReadCmdInfo(data->cmdInfo);
        demofile.ReadSequenceInfo(data->sequenceNum1, data->sequenceNum2);
        return demofile.IsOk();
    }

    bool Dem_Packet_FileWrite_Internal(FileWrite& demofile, DemMsg::Dem_Packet* data)
    {
        demofile.WriteCmdInfo(data->cmdInfo);
        demofile.WriteSequenceInfo(data->sequenceNum1, data->sequenceNum2);
        return demofile.IsOk();
    }

    bool Dem_Packet_JsonRead_Internal(JsonRead& jsonbuf, DemMsg::Dem_Packet* data)
    {
        base::JsonReaderObject reader = jsonbuf.ParseObject();
        assert(!reader.HasReadError());

        bool readError = DemoJsonReader::ReadCmdInfo(reader, data->cmdInfo);
        readError |= DemoJsonReader::ReadSequenceInfo(reader, data->sequenceNum1, data->sequenceNum2);
        return !readError && !reader.HasReadError();
    }

    bool Dem_Packet_JsonWrite_Internal(JsonWrite& jsonbuf, DemMsg::Dem_Packet* data)
    {
        jsonbuf.Reset();
        jsonbuf.StartObject();
        DemoJsonWriter::WriteCmdInfo(jsonbuf, data->cmdInfo);
        DemoJsonWriter::WriteSequenceInfo(jsonbuf, data->sequenceNum1, data->sequenceNum2);
        jsonbuf.EndObject();
        return jsonbuf.IsComplete();
    }
}
