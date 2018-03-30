
#pragma once

#include <stdio.h>
#include <cassert>
#include <cstdint>

class WaveFileWriter
{
public:
    WaveFileWriter():
        m_file(nullptr),
        m_DataBytes(0)
    {
    }

    ~WaveFileWriter()
    {
        assert(!m_file);
    }

    void Init(const char* file, uint32_t sampleRate)
    {
        FILE* fp = fopen(file, "wb");
        assert(fp);

        m_file = fp;
        m_DataBytes = 0;

        const uint32_t chunkSize = 0;

        const uint32_t fmtChunkSize = 16;
        const uint16_t audioFormat = 1;   // pcm
        const uint16_t numChannels = 1;
        const uint32_t bytesPerSample = 2;
        const uint32_t byteRate = sampleRate * numChannels * bytesPerSample;
        const uint16_t blockAlign = numChannels * bytesPerSample;
        const uint16_t bitsPerSample = 16;

        fputs("RIFF", fp);
        fwrite(&chunkSize, sizeof(chunkSize), 1, fp);
        fputs("WAVE", fp);

        fputs("fmt ", fp);
        fwrite(&fmtChunkSize, sizeof(fmtChunkSize), 1, fp);
        fwrite(&audioFormat, sizeof(audioFormat), 1, fp);
        fwrite(&numChannels, sizeof(numChannels), 1, fp);
        fwrite(&sampleRate, sizeof(sampleRate), 1, fp);
        fwrite(&byteRate, sizeof(byteRate), 1, fp);
        fwrite(&blockAlign, sizeof(blockAlign), 1, fp);
        fwrite(&bitsPerSample, sizeof(bitsPerSample), 1, fp);

        fputs("data", fp);
        fwrite(&chunkSize, sizeof(chunkSize), 1, fp);
    }

    void Close()
    {
        FILE* fp = m_file;
        if(!fp)
        {
            return;
        }

        const uint32_t dataSize = m_DataBytes;
        const uint32_t chunkSize = dataSize + 36;

        fseek(fp, 4, SEEK_SET);
        fwrite(&chunkSize, sizeof(chunkSize), 1, fp);

        fseek(fp, 40, SEEK_SET);
        fwrite(&dataSize, sizeof(dataSize), 1, fp);

        fclose(fp);

        m_file = nullptr;
    }

    void WriteSamples(const int16_t* samples, uint32_t numSamples)
    {
        const uint32_t bytesPerSample = 2;
        const size_t elemsWritten = fwrite(samples, bytesPerSample, numSamples, m_file);
        assert(elemsWritten == numSamples);
        m_DataBytes += (elemsWritten * bytesPerSample);
    }

private:
    FILE* m_file;
    uint32_t m_DataBytes;
};
