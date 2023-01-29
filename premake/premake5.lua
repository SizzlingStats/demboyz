
require "vstudio"
local function add_tag(tag, value, project_name)
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

solution "demboyz"
    basedir ".."
    location (_ACTION)
    targetdir "../bin"
    startproject "demboyz"
    configurations { "Debug", "Release" }
    debugargs
    {
        --"D:/projects/demboyz_data/steam_voice_test/Jan_28_7.47_PM-pl_frontier_final.dem",
        --"D:/projects/demboyz_data/steam_voice_test/out"
    }
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
        dofile "steamworks.lua"
        add_tag("SupportJustMyCode", "false", "demboyz")
        add_tag("LanguageStandard", "stdcpp17", "demboyz")
    project "*"

    dofile "json_checker.lua"
    dofile "snappy.lua"
    dofile "sourcesdk.lua"
    dofile "celt.lua"
