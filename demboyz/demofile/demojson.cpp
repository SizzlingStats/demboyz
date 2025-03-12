
#include "demojson.h"
#include "demofile/demotypes.h"
#include <algorithm>

bool DemoJsonReader::ReadDemoHeader(base::JsonReaderObject& reader, demoheader_t& header)
{
    base::JsonReaderObject object = reader.ReadObject("demoheader");
    object.ReadString("demofilestamp", header.demofilestamp, sizeof(header.demofilestamp));
    header.demoprotocol = object.ReadInt32("demoprotocol");
    header.networkprotocol = object.ReadInt32("networkprotocol");
    object.ReadString("servername", header.servername, sizeof(header.servername));
    object.ReadString("clientname", header.clientname, sizeof(header.clientname));
    object.ReadString("mapname", header.mapname, sizeof(header.mapname));
    object.ReadString("gamedirectory", header.gamedirectory, sizeof(header.gamedirectory));
    header.playback_time = object.ReadFloat("playback_time");
    header.playback_ticks = object.ReadInt32("playback_ticks");
    header.playback_frames = object.ReadInt32("playback_frames");
    header.signonlength = object.ReadInt32("signonlength");
    return !reader.HasReadError() && !object.HasReadError();
}

bool DemoJsonReader::ReadSequenceInfo(base::JsonReaderObject& reader,
                                      int32_t& seqNum1, int32_t& seqNum2)
{
    seqNum1 = reader.ReadInt32("sequenceNum1");
    seqNum2 = reader.ReadInt32("sequenceNum2");
    return !reader.HasReadError();
}

bool DemoJsonReader::ReadCmdInfo(base::JsonReaderObject& reader, democmdinfo_t& info)
{
    democmdinfo_t::Split_t& split = info.u[0];
    base::JsonReaderObject object = reader.ReadObject("democmdinfo");
    split.flags = object.ReadInt32("flags");
    bool readError = ReadVector(object, "viewOrigin", split.viewOrigin);
    readError |= ReadAngle(object, "viewAngles", split.viewAngles);
    readError |= ReadAngle(object, "localViewAngles", split.localViewAngles);
    readError |= ReadVector(object, "viewOrigin2", split.viewOrigin2);
    readError |= ReadAngle(object, "viewAngles2", split.viewAngles2);
    readError |= ReadAngle(object, "localViewAngles2", split.localViewAngles2);
    return !readError && !reader.HasReadError() && !object.HasReadError();
}

bool DemoJsonReader::ReadCmdHeader(base::JsonReaderObject& reader, unsigned char& cmd, int32_t& tick)
{
    cmd = reader.ReadUInt32("cmd");
    tick = reader.ReadInt32("tick");
    return !reader.HasReadError();
}

bool DemoJsonReader::ReadUserCmd(base::JsonReaderObject& reader, int32_t& cmdNum,
                                 Array<uint8_t>& dest, int32_t maxLength)
{
    base::JsonReaderObject object = reader.ReadObject("usercmd");
    cmdNum = object.ReadInt32("cmd");

    const int32_t numBytes = object.ReadBytes("data", nullptr, 0);
    dest.reset(std::min(maxLength, numBytes));
    object.ReadBytes("data", dest.begin(), dest.length());
    return !object.HasReadError();
}

bool DemoJsonReader::ReadVector(base::JsonReaderObject& reader, const char* name, Vector& vec)
{
    base::JsonReaderObject object = reader.ReadObject(name);
    vec.x = object.ReadFloat("x");
    vec.y = object.ReadFloat("y");
    vec.z = object.ReadFloat("z");
    return !reader.HasReadError() && !object.HasReadError();
}

bool DemoJsonReader::ReadAngle(base::JsonReaderObject& reader, const char* name, QAngle& angles)
{
    base::JsonReaderObject object = reader.ReadObject(name);
    angles.x = object.ReadFloat("pitch");
    angles.y = object.ReadFloat("yaw");
    angles.z = object.ReadFloat("roll");
    return !reader.HasReadError() && !object.HasReadError();
}

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

void DemoJsonWriter::WriteCmdHeader(base::JsonWriterFile& writer, unsigned char cmd, int32_t tick)
{
    writer.WriteUInt32("cmd", cmd);
    writer.WriteInt32("tick", tick);
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
