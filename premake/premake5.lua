
local function add_tag(tag, value, project_name)
    if string.find(_ACTION, "vs") then
        require "vstudio"
        premake.override(premake.vstudio.vc2010.elements, "clCompile",
            function(oldfn, cfg)
                local calls = oldfn(cfg)
                if project_name == nil or cfg.project.name == project_name then
                    table.insert(calls, function(cfg)
                        premake.vstudio.vc2010.element(tag, nil, value)
                    end)
                end
                return calls
            end)
    end
end

solution "demboyz"
    basedir ".."
    location (_ACTION)
    targetdir "../bin"
    startproject "demboyz"
    debugdir "../bin"
    configurations { "Debug", "Release" }
    debugargs
    {
        --"Jan_28_7.47_PM-pl_frontier_final.dem",
        --"out"
    }
    platforms { "x64", "x32" }
    flags { "MultiProcessorCompile", "Symbols" }

    defines { "_CRT_SECURE_NO_WARNINGS", "_SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING" }
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
            buildoptions { "-std=c++17" }
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
        configuration "x64"
            dofile "steamworks.lua"
        configuration "x32"
            dofile "steamworks32.lua"
        configuration {}
        add_tag("SupportJustMyCode", "false", "demboyz")
        add_tag("LanguageStandard", "stdcpp17", "demboyz")
    project "*"

    dofile "json_checker.lua"
    dofile "snappy.lua"
    dofile "sourcesdk.lua"
    dofile "celt.lua"
