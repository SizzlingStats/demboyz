
solution "demboyz"
    basedir ".."
    location (_ACTION)
    startproject "demboyz"
    configurations { "Debug", "Release" }
    platforms "x32"
    flags { "MultiProcessorCompile", "Symbols" }

    defines "_CRT_SECURE_NO_WARNINGS"
    
    -- GCC specific build options.
    configuration "gmake"
        -- Enables C++11 support.
        buildoptions { "-std=c++0x" }

    project "demboyz"
        kind "ConsoleApp"
        language "C++"
        files
        {
            "../demboyz/**.h",
            "../demboyz/**.cpp",
            "../external/sourcesdk/*.h",
            "../external/sourcesdk/*.cpp"
        }
        includedirs
        {
            "../external",
            "../demboyz"
        }

        configuration "Debug"
            targetdir (_ACTION .. "/build/Debug")
            defines { "DEBUG" }
            flags { "Symbols" }

        configuration "Release"
            targetdir (_ACTION .. "/build/Release")
            defines { "NDEBUG" }
            optimize "Full"
