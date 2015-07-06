
#pragma once

#include <rapidjson/filewritestream.h>
#include <rapidjson/writer.h>
#include <cstdint>
#include <cstdio>
#include <cstddef>

namespace base
{
    class JsonWriterFile
    {
    public:
        JsonWriterFile(FILE* fp, char* buffer, std::size_t length);
        ~JsonWriterFile();

        FILE* GetFp() const;
        void Flush();
        void Reset();
        bool IsComplete() const;

        void StartObject(const char* name = nullptr);
        void EndObject();

        void StartArray(const char* name = nullptr);
        void EndArray();

        void WriteNull(const char* name);
        void WriteBool(const char* name, bool value);
        void WriteInt32(const char* name, std::int32_t value);
        void WriteInt32(const char* name, std::int32_t value, bool writeCondition);
        void WriteInt64(const char* name, std::int64_t value);
        void WriteUInt32(const char* name, std::uint32_t value);
        void WriteUInt32(const char* name, std::uint32_t value, bool writeCondition);
        void WriteUint64(const char* name, std::uint64_t value);
        void WriteString(const char* name, const char* value);
        void WriteString(const char* name, const char* value, std::int32_t length);
        void WriteFloat(const char* name, const double value);

        void WriteBits(const char* name, const unsigned char* data, std::size_t numBits);
        void WriteBytes(const char* name, const unsigned char* data, std::size_t numBytes);

    private:
        rapidjson::FileWriteStream m_fileStream;
        rapidjson::Writer<rapidjson::FileWriteStream> m_writer;
        FILE* m_fp;
    };
}
