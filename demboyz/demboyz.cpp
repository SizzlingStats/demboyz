
#include "idemowriter.h"
#include "demoreader.h"
#include <cstdio>
#include <string>

std::string GetExtension(const std::string& filename)
{
    size_t index = filename.find_last_of(".");
    if (index != std::string::npos)
    {
        return filename.substr(index + 1);
    }
    return std::string();
}

enum class FileType
{
    None,
    Dem,
    Json
};

FileType GetFileType(const std::string& filename)
{
    std::string ext = GetExtension(filename);
    if (ext == "dem")
    {
        return FileType::Dem;
    }
    if (ext == "json")
    {
        return FileType::Json;
    }
    return FileType::None;
}

int main(const int argc, const char* argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <in>.dem/json <out>.dem/json\n", argv[0]);
        return -1;
    }

    std::string inputFile(argv[1]);
    std::string outputFile(argv[2]);

    FileType inputType = GetFileType(inputFile);
    FileType outputType = GetFileType(outputFile);
    if (inputType == FileType::None)
    {
        fprintf(stderr, "Error: Bad type for input file\n");
        return -1;
    }
    if (outputType == FileType::None)
    {
        fprintf(stderr, "Error: Bad type for output file\n");
        return -1;
    }

    FILE* inputFp = fopen(inputFile.c_str(), "rb");
    if (!inputFp)
    {
        fprintf(stderr, "Error: Could not open input file\n");
        return -1;
    }

    FILE* outputFp = fopen(outputFile.c_str(), "wb");
    if (!outputFp)
    {
        fprintf(stderr, "Error: Could not open input file\n");
        fclose(inputFp);
        return -1;
    }

    IDemoWriter* writer;
    if (outputType == FileType::Dem)
    {
        writer = IDemoWriter::CreateDemoWriter(outputFp);
    }
    else
    {
        writer = IDemoWriter::CreateJsonWriter(outputFp);
    }

    if (inputType == FileType::Dem)
    {
        DemoReader::ProcessDem(inputFp, writer);
    }
    else
    {
        DemoReader::ProcessJson(inputFp, writer);
    }
    fclose(inputFp);
    fclose(outputFp);

    IDemoWriter::FreeDemoWriter(writer);
    return 0;
}
