
solution "demboyz"
    basedir ".."
    location (_ACTION)
    targetdir "../bin"
    startproject "demboyz"
    configurations { "Debug", "Release" }
    platforms "x32"
    flags { "MultiProcessorCompile", "Symbols" }

    defines "_CRT_SECURE_NO_WARNINGS"
    configuration "Debug"
        defines { "DEBUG" }
    configuration "Release"
        defines { "NDEBUG" }
        optimize "Full"
    configuration {}

    project "demboyz"
        kind "ConsoleApp"
        language "C++"
        configuration "gmake"
            buildoptions { "-std=c++11" }
        configuration {}
        files
        {
            "../demboyz/**.h",
            "../demboyz/**.cpp"
        }
        includedirs
        {
            "../external/json_checker/include",
            "../external/cbase64-1.1/include",
            "../external/sourcesdk/include",
            "../external/rapidjson-1.0.2/include",
            "../external/snappy-1.1.3/include",
            "../external/celt-e18de77/include",
            "../demboyz"
        }
        links
        {
            "json_checker",
            "sourcesdk",
            "celt"
        }
    project "*"

    dofile "json_checker.lua"
    dofile "snappy.lua"
    dofile "sourcesdk.lua"
    dofile "celt.lua"
