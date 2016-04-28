
#pragma once

#include <rapidjson/filewritestream.h>
#include <rapidjson/filereadstream.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/document.h>
#include <cstdint>
#include <cstdio>
#include <cstddef>
#include <string>

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
        void WriteChar(const char* name, char value);
        void WriteInt32(const char* name, std::int32_t value);
        void WriteInt32(const char* name, std::int32_t value, bool writeCondition);
        void WriteInt64(const char* name, std::int64_t value);
        void WriteUInt32(const char* name, std::uint32_t value);
        void WriteUInt32(const char* name, std::uint32_t value, bool writeCondition);
        void WriteUint64(const char* name, std::uint64_t value);
        void WriteString(const char* name, const char* value);
        void WriteString(const char* name, const char* value, std::uint32_t length);
        void WriteString(const char* name, const std::string& value);
        void WriteFloat(const char* name, const double value);

        void WriteBits(const char* name, const unsigned char* data, std::size_t numBits);
        void WriteBytes(const char* name, const unsigned char* data, std::size_t numBytes);

    private:
        rapidjson::FileWriteStream m_fileStream;
        rapidjson::PrettyWriter<rapidjson::FileWriteStream, rapidjson::ASCII<>> m_writer;
        FILE* m_fp;
    };

    class JsonReaderObject
    {
    public:
        using JsonValue = rapidjson::GenericValue<rapidjson::ASCII<>>;

        class JsonReaderIterator
        {
        public:
            explicit JsonReaderIterator(JsonValue* value, bool& hasReadError);

            JsonReaderObject operator*() const;
            JsonReaderIterator& operator++();
            bool operator==(const JsonReaderIterator& other) const;
            bool operator!=(const JsonReaderIterator& other) const;

        private:
            JsonValue* m_value;
            bool& m_hasReadError;
        };

        class JsonReaderArray
        {
        public:
            explicit JsonReaderArray(JsonValue& value, bool& parseError);

            bool HasReadError() const;

            std::size_t size() const;
            JsonReaderIterator begin();
            JsonReaderIterator end();

            template<typename Container, typename Fn>
            void TransformTo(Container& c, Fn fn)
            {
                c.resize(m_value.Size());
                std::size_t index = 0;
                for (base::JsonReaderObject obj : *this)
                {
                    fn(obj, c[index++]);
                }
            }

        private:
            JsonValue& m_value;
            bool& m_hasReadError;
        };

    public:
        explicit JsonReaderObject(JsonValue& value, bool& parseError);

        bool HasReadError() const;

        JsonReaderObject ReadObject(const char* name) const;
        JsonReaderArray ReadArray(const char* name) const;

        bool            ReadBool(const char* name);
        char            ReadChar(const char* name);
        std::int32_t    ReadInt32(const char* name);
        std::int64_t    ReadInt64(const char* name);
        std::uint32_t   ReadUInt32(const char* name);
        std::uint64_t   ReadUint64(const char* name);
        std::uint32_t   ReadString(const char* name, char* dest, std::uint32_t maxLength);
        //std::string     ReadString(const char* name, std::uint32_t maxLength);
        float           ReadFloat(const char* name);

        std::size_t     ReadBits(const char* name, unsigned char* dest, std::size_t numBits);
        std::size_t     ReadBytes(const char* name, unsigned char* dest, std::size_t numBytes);

    private:
        JsonValue& m_value;
        bool& m_hasReadError;
    };

    using JsonReaderIterator = JsonReaderObject::JsonReaderIterator;
    using JsonReaderArray = JsonReaderObject::JsonReaderArray;

    class JsonReaderFile
    {
    public:
        JsonReaderFile(FILE* fp, char* buffer, std::size_t length);

        JsonReaderObject ParseObject();

        bool HasParseError() const;
        bool HasReadError() const;

    private:
        rapidjson::FileReadStream m_fileStream;
        rapidjson::GenericDocument<rapidjson::ASCII<>> m_document;
        bool m_hasParseError;
        bool m_hasReadError;
    };
}
