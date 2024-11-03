
#include "io/idemowriter.h"
#include "io/demoreader.h"
#include "json_checker/JSON_checker.h"
#include <cstdio>
#include <string>
#include <cassert>
#include <filesystem>

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
    Json,
    ConLog,
    Voice
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
    if (ext == "con")
    {
        return FileType::ConLog;
    }
    return FileType::None;
}

int RocketModeFixMain(const int argc, const char* argv[])
{
    assert(argc == 4);
    assert(!strcmp(argv[1], "-rocketmodefix"));

    const char* inputDemo = argv[2];
    const char* outputDemo = argv[3];
    FILE* inputFp = fopen(inputDemo, "rb");
    if (!inputFp)
    {
        fprintf(stderr, "Error: Could not open input file\n");
        return -1;
    }

    FILE* outputFp = fopen(outputDemo, "wb");
    if (!outputFp)
    {
        fprintf(stderr, "Error: Could not open input file\n");
        fclose(inputFp);
        return -1;
    }

    fprintf(stdout, "Fixing up rocketmode demo %s to %s\n", inputDemo, outputDemo);

    IDemoWriter* writer = IDemoWriter::CreateRocketModeFixerDemoWriter(outputFp);
    DemoReader::ProcessDem(inputFp, writer);
    IDemoWriter::FreeDemoWriter(writer);

    fclose(outputFp);
    fclose(inputFp);
    return 0;
}

int VoiceMain(const int argc, const char* argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <Input>.dem <OutputDir>\n", argv[0]);
        return -1;
    }

    const char* outputDir = argv[2];

    std::filesystem::path outputDirPath(outputDir);
    std::filesystem::create_directories(outputDirPath);
    if (!std::filesystem::is_directory(outputDirPath))
    {
        fprintf(stderr, "Error: Invalid output directory\n");
        return -1;
    }

    const char* inputFile = argv[1];
    FILE* inputFp = fopen(inputFile, "rb");
    if (!inputFp)
    {
        fprintf(stderr, "Error: Could not open input file\n");
        return -1;
    }

    fprintf(stdout, "Extracting voice data from %s to %s\n", inputFile, outputDir);

    IDemoWriter* writer = IDemoWriter::CreateVoiceDataWriter(outputDir);
    DemoReader::ProcessDem(inputFp, writer);
    IDemoWriter::FreeDemoWriter(writer);

    fclose(inputFp);
    return 0;
}

int DemboyzMain(const int argc, const char* argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <in>.dem/json <out>.dem/json/con\n", argv[0]);
        return -1;
    }

    std::string inputFile(argv[1]);
    std::string outputFile(argv[2]);
    if (inputFile == outputFile)
    {
        fprintf(stderr, "Error: Input and output file cannot be the same!\n");
        return -1;
    }

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

    IDemoWriter* writer = nullptr;
    if (outputType == FileType::Dem)
    {
        writer = IDemoWriter::CreateDemoWriter(outputFp);
    }
    else if (outputType == FileType::Json)
    {
        writer = IDemoWriter::CreateJsonWriter(outputFp);
    }
    else if (outputType == FileType::ConLog)
    {
        writer = IDemoWriter::CreateConLogWriter(outputFp);
    }
    else
    {
        assert(false);
    }

    if (inputType == FileType::Dem)
    {
        DemoReader::ProcessDem(inputFp, writer);
    }
    else if (inputType == FileType::Json)
    {
        DemoReader::ProcessJson(inputFp, writer);
    }
    else
    {
        assert(false);
    }
    IDemoWriter::FreeDemoWriter(writer);

    fclose(inputFp);
    fclose(outputFp);

    /*if (outputType == FileType::Json)
    {
        FILE* outputFp = fopen(outputFile.c_str(), "rb");
        JSON_checker jc = new_JSON_checker(20);
        int next_char = 0;
        while ((next_char = fgetc(outputFp)) > 0)
        {
            if (!JSON_checker_char(jc, next_char))
            {
                fprintf(stderr, "JSON_checker_char: syntax error\n");
            }
        }
        if (!JSON_checker_done(jc))
        {
            fprintf(stderr, "JSON_checker_end: syntax error\n");
        }
        fclose(outputFp);
    }*/
    return 0;
}

int main(const int argc, const char* argv[])
{
    if (argc == 4 && !strcmp(argv[1], "-rocketmodefix"))
    {
        return RocketModeFixMain(argc, argv);
    }
    else if (argc > 2 && GetExtension(argv[2]).empty())
    {
        return VoiceMain(argc, argv);
    }
    return DemboyzMain(argc, argv);
}
