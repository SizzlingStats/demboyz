
#include "demojson.h"
#include "demofile/demotypes.h"

void DemoJsonWriter::WriteDemoHeader(base::JsonWriterFile& writer, const demoheader_t& header)
{
    writer.StartObject("demoheader");
    writer.WriteString("demofilestamp", header.demofilestamp);
    writer.WriteInt32("demoprotocol", header.demoprotocol);
    writer.WriteInt32("networkprotocol", header.networkprotocol);
    writer.WriteString("servername", header.servername);
    writer.WriteString("clientname", header.clientname);
    writer.WriteString("mapname", header.mapname);
    writer.WriteString("gamedirectory", header.gamedirectory);
    writer.WriteFloat("playback_time", header.playback_time);
    writer.WriteInt32("playback_ticks", header.playback_ticks);
    writer.WriteInt32("playback_frames", header.playback_frames);
    writer.WriteInt32("signonlength", header.signonlength);
    writer.EndObject();
}

void DemoJsonWriter::WriteSequenceInfo(base::JsonWriterFile& writer,
                                       int32_t seqNum1, int32_t seqNum2)
{
    writer.WriteInt32("sequenceNum1", seqNum1);
    writer.WriteInt32("sequenceNum2", seqNum2);
}

void DemoJsonWriter::WriteCmdInfo(base::JsonWriterFile& writer,
                                  const democmdinfo_t& info)
{
    const democmdinfo_t::Split_t& split = info.u[0];
    writer.StartObject("democmdinfo");
    writer.WriteInt32("flags", split.flags);
    WriteVector(writer, "viewOrigin", split.viewOrigin);
    WriteAngle(writer, "viewAngles", split.viewAngles);
    WriteAngle(writer, "localViewAngles", split.localViewAngles);
    WriteVector(writer, "viewOrigin2", split.viewOrigin2);
    WriteAngle(writer, "viewAngles2", split.viewAngles2);
    WriteAngle(writer, "localViewAngles2", split.localViewAngles2);
    writer.EndObject();
}

void DemoJsonWriter::WriteUserCmd(base::JsonWriterFile& writer,
                                  int32_t cmdNum, const uint8_t* buffer, int32_t length)
{
    writer.StartObject("usercmd");
    writer.WriteInt32("cmd", cmdNum);
    writer.WriteBytes("data", buffer, length);
    writer.EndObject();
}

void DemoJsonWriter::WriteVector(base::JsonWriterFile& writer, const char* name, const Vector& vec)
{
    writer.StartObject(name);
    writer.WriteFloat("x", vec.x);
    writer.WriteFloat("y", vec.y);
    writer.WriteFloat("z", vec.z);
    writer.EndObject();
}

void DemoJsonWriter::WriteAngle(base::JsonWriterFile& writer, const char* name, const QAngle& angles)
{
    writer.StartObject(name);
    writer.WriteFloat("pitch", angles.x);
    writer.WriteFloat("yaw", angles.y);
    writer.WriteFloat("roll", angles.z);
    writer.EndObject();
}
