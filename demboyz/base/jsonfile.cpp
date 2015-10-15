
#include "jsonfile.h"
#include <cstdlib>
#include <algorithm>

#define CBASE64_IMPLEMENTATION
#include "cbase64/cbase64.h"

#define RIGHT_TO_LEFT_BITS

namespace base
{
    JsonWriterFile::JsonWriterFile(FILE* fp, char* buffer, std::size_t length):
        m_fileStream(fp, buffer, length),
        m_writer(m_fileStream),
        m_fp(fp)
    {
    }

    JsonWriterFile::~JsonWriterFile()
    {
        Flush();
    }

    FILE* JsonWriterFile::GetFp() const
    {
        return m_fp;
    }

    void JsonWriterFile::Flush()
    {
        m_fileStream.Flush();
        fflush(m_fp);
    }

    void JsonWriterFile::Reset()
    {
        m_writer.Reset(m_fileStream);
    }

    bool JsonWriterFile::IsComplete() const
    {
        return m_writer.IsComplete();
    }

    void JsonWriterFile::StartObject(const char* name /*= nullptr*/)
    {
        auto& writer = m_writer;
        if (name)
        {
            writer.String(name);
        }
        writer.StartObject();
    }

    void JsonWriterFile::EndObject()
    {
        m_writer.EndObject();
    }

    void JsonWriterFile::StartArray(const char* name /*= nullptr*/)
    {
        auto& writer = m_writer;
        if (name)
        {
            writer.String(name);
        }
        writer.StartArray();
    }

    void JsonWriterFile::EndArray()
    {
        m_writer.EndArray();
    }

    void JsonWriterFile::WriteNull(const char* name)
    {
        auto& writer = m_writer;
        writer.String(name);
        writer.Null();
    }

    void JsonWriterFile::WriteBool(const char* name, bool value)
    {
        auto& writer = m_writer;
        writer.String(name);
        writer.Bool(value);
    }

    void JsonWriterFile::WriteChar(const char* name, char value)
    {
        auto& writer = m_writer;
        writer.String(name);

        char temp[2] = { value, '\0' };
        writer.String(temp);
    }

    void JsonWriterFile::WriteInt32(const char* name, std::int32_t value)
    {
        auto& writer = m_writer;
        writer.String(name);
        writer.Int(value);
    }

    void JsonWriterFile::WriteInt32(const char* name, std::int32_t value, bool writeCondition)
    {
        auto& writer = m_writer;
        writer.String(name);
        if (writeCondition)
        {
            writer.Int(value);
        }
        else
        {
            writer.Null();
        }
    }

    void JsonWriterFile::WriteInt64(const char* name, std::int64_t value)
    {
        auto& writer = m_writer;
        writer.String(name);
        writer.Int64(value);
    }

    void JsonWriterFile::WriteUInt32(const char* name, std::uint32_t value)
    {
        auto& writer = m_writer;
        writer.String(name);
        writer.Uint(value);
    }

    void JsonWriterFile::WriteUInt32(const char* name, std::uint32_t value, bool writeCondition)
    {
        auto& writer = m_writer;
        writer.String(name);
        if (writeCondition)
        {
            writer.Uint(value);
        }
        else
        {
            writer.Null();
        }
    }

    void JsonWriterFile::WriteUint64(const char* name, std::uint64_t value)
    {
        auto& writer = m_writer;
        writer.String(name);
        writer.Uint64(value);
    }

    void JsonWriterFile::WriteString(const char* name, const char* value)
    {
        auto& writer = m_writer;
        writer.String(name);
        writer.String(value);
    }

    void JsonWriterFile::WriteString(const char* name, const char* value, std::uint32_t length)
    {
        auto& writer = m_writer;
        writer.String(name);
        writer.String(value, length);
    }

    void JsonWriterFile::WriteString(const char* name, const std::string& value)
    {
        auto& writer = m_writer;
        writer.String(name);
        writer.String(value.c_str(), value.length());
    }

    void JsonWriterFile::WriteFloat(const char* name, const double value)
    {
        auto& writer = m_writer;
        writer.String(name);
        writer.Double(value);
    }

    void JsonWriterFile::WriteBits(const char* name, const unsigned char* data, std::size_t numBits)
    {
        auto& writer = m_writer;
        writer.String(name);

        cbase64_encodestate state;
        cbase64_init_encodestate(&state);

        const std::size_t numBytes = ((numBits + 7) >> 3);

        char* const encoded = (char*)malloc(cbase64_calc_encoded_length(numBytes));
        char* encodedCurOut = encoded;

        const std::size_t numBytesWithoutBits = (numBits >> 3);
        const std::size_t numTrailingBits = (numBits & 7);

        encodedCurOut += cbase64_encode_block(data, numBytesWithoutBits, encodedCurOut, &state);
        if (numTrailingBits > 0)
        {
#ifdef LEFT_TO_RIGHT_BITS
            const unsigned char lastByteClean = data[numBytesWithoutBits] & (0xFF >> numTrailingBits);
#else // RIGHT_TO_LEFT_BITS
            const unsigned char lastByteClean = data[numBytesWithoutBits] & ~(0xFF << numTrailingBits);
#endif
            encodedCurOut += cbase64_encode_block(&lastByteClean, 1, encodedCurOut, &state);
        }
        encodedCurOut += cbase64_encode_blockend(encodedCurOut, &state);

        writer.String(encoded, encodedCurOut - encoded);
        free(encoded);
        return;
    }

    void JsonWriterFile::WriteBytes(const char* name, const unsigned char* data, std::size_t numBytes)
    {
        JsonWriterFile::WriteBits(name, data, numBytes * 8);
    }

    JsonReaderObject::JsonReaderIterator::JsonReaderIterator(JsonValue* value, bool& hasReadError):
        m_value(value),
        m_hasReadError(hasReadError)
    {
    }

    JsonReaderObject JsonReaderIterator::operator*() const
    {
        return JsonReaderObject(*m_value, m_hasReadError);
    }

    JsonReaderIterator& JsonReaderIterator::operator++()
    {
        ++m_value;
        return *this;
    }

    bool JsonReaderIterator::operator==(const JsonReaderIterator& other) const
    {
        return m_value == other.m_value;
    }

    bool JsonReaderIterator::operator!=(const JsonReaderIterator& other) const
    {
        return m_value != other.m_value;
    }

    JsonReaderObject::JsonReaderArray::JsonReaderArray(JsonValue& value, bool& parseError):
        m_value(value),
        m_hasReadError(parseError)
    {
    }

    bool JsonReaderArray::HasReadError() const
    {
        return m_hasReadError;
    }

    std::size_t JsonReaderArray::size() const
    {
        return m_value.Size();
    }

    JsonReaderIterator JsonReaderArray::begin()
    {
        return JsonReaderIterator(m_value.Begin(), m_hasReadError);
    }

    JsonReaderIterator JsonReaderArray::end()
    {
        return JsonReaderIterator(m_value.End(), m_hasReadError);
    }

    JsonReaderObject::JsonReaderObject(JsonValue& value, bool& parseError):
        m_value(value),
        m_hasReadError(parseError)
    {
    }

    bool JsonReaderObject::HasReadError() const
    {
        return m_hasReadError;
    }

    JsonReaderObject JsonReaderObject::ReadObject(const char* name) const
    {
        JsonValue& value = m_value[name];
        m_hasReadError |= !value.IsObject();
        return JsonReaderObject(value, m_hasReadError);
    }

    JsonReaderArray JsonReaderObject::ReadArray(const char* name) const
    {
        JsonValue& value = m_value[name];
        m_hasReadError |= !value.IsArray();
        return JsonReaderArray(value, m_hasReadError);
    }

    bool JsonReaderObject::ReadBool(const char* name)
    {
        const auto& val = m_value[name];
        if (!val.IsBool())
        {
            m_hasReadError = true;
            return false;
        }
        return val.GetBool();
    }

    char JsonReaderObject::ReadChar(const char* name)
    {
        const auto& val = m_value[name];
        if (!val.IsString() || val.GetStringLength() != 1)
        {
            m_hasReadError = true;
            return false;
        }
        return *val.GetString();
    }

    std::int32_t JsonReaderObject::ReadInt32(const char* name)
    {
        const auto& val = m_value[name];
        if (!val.IsInt())
        {
            m_hasReadError = true;
            return 0;
        }
        return val.GetInt();
    }

    std::int64_t JsonReaderObject::ReadInt64(const char* name)
    {
        const auto& val = m_value[name];
        if (!val.IsInt64())
        {
            m_hasReadError = true;
            return 0;
        }
        return val.GetInt64();
    }

    std::uint32_t JsonReaderObject::ReadUInt32(const char* name)
    {
        const auto& val = m_value[name];
        if (!val.IsUint())
        {
            m_hasReadError = true;
            return 0;
        }
        return val.GetUint();
    }

    std::uint64_t JsonReaderObject::ReadUint64(const char* name)
    {
        const auto& val = m_value[name];
        if (!val.IsUint64())
        {
            m_hasReadError = true;
            return 0;
        }
        return val.GetUint64();
    }

    std::uint32_t JsonReaderObject::ReadString(const char* name, char* dest, std::uint32_t maxLength)
    {
        const auto& val = m_value[name];
        if (!val.IsString())
        {
            m_hasReadError = true;
            return 0;
        }
        strncpy(dest, val.GetString(), maxLength - 1);
        dest[maxLength - 1] = '\0';
        return std::min(maxLength - 1, val.GetStringLength());
    }

    float JsonReaderObject::ReadFloat(const char* name)
    {
        const auto& val = m_value[name];
        if (!val.IsDouble())
        {
            m_hasReadError = true;
            return 0.0f;
        }
        return val.GetDouble();
    }

    std::size_t JsonReaderObject::ReadBits(const char* name, unsigned char* dest, std::size_t numBits)
    {
        const auto& val = m_value[name];
        if (!val.IsString())
        {
            m_hasReadError = true;
            return 0;
        }

        const char* const encodedBits = val.GetString();
        const std::size_t numEncodedBytes = val.GetStringLength();
        const std::size_t numDecodedBytes = cbase64_calc_decoded_length(encodedBits, numEncodedBytes);
        if (!dest || (numDecodedBytes == 0))
        {
            return numDecodedBytes * 8;
        }

        const std::size_t numBytes = ((numBits + 7) >> 3);
        if ((numDecodedBytes == 0) || (numBytes < numDecodedBytes))
        {
            return 0;
        }

        unsigned char* lastByte = &dest[numDecodedBytes - 1];
        unsigned char restoreBits = *lastByte;

        cbase64_decodestate state;
        cbase64_init_decodestate(&state);
        const size_t numWritten = cbase64_decode_block(encodedBits, numEncodedBytes, dest, &state);
        assert(numWritten == numDecodedBytes);

        const std::size_t numTrailingBits = (numBits & 7);
        if (numTrailingBits > 0)
        {
            // clean up end of decoded, clean up start of restoreBits
            // then combine
#ifdef LEFT_TO_RIGHT_BITS
            *lastByte = (*lastByte & ~(0xFF >> numTrailingBits)) |
                        (restoreBits & (0xFF >> numTrailingBits));
#else // RIGHT_TO_LEFT_BITS
            *lastByte = (*lastByte & ~(0xFF << numTrailingBits)) |
                        (restoreBits & (0xFF << numTrailingBits));
#endif
        }
        return numBits;
    }

    std::size_t JsonReaderObject::ReadBytes(const char* name, unsigned char* dest, std::size_t numBytes)
    {
        const std::size_t numBitsRead = ReadBits(name, dest, numBytes * 8);
        assert((numBitsRead & 7) == 0);
        return (numBitsRead / 8);
    }

    JsonReaderFile::JsonReaderFile(FILE* fp, char* buffer, std::size_t length):
        m_fileStream(fp, buffer, length),
        m_document()
    {
    }

    JsonReaderObject JsonReaderFile::ParseObject()
    {
        const int flags = rapidjson::kParseValidateEncodingFlag |
                          rapidjson::kParseStopWhenDoneFlag;

        auto& document = m_document;
        document.ParseStream<flags>(m_fileStream);
        m_hasParseError = document.HasParseError();
        m_hasReadError = false;
        return JsonReaderObject(document, m_hasReadError);
    }

    bool JsonReaderFile::HasParseError() const
    {
        return m_hasParseError;
    }

    bool JsonReaderFile::HasReadError() const
    {
        return m_hasReadError;
    }
}
