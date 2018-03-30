
local base_dir = (solution().basedir .. "/external/celt-e18de77/")

group "external"
    project "celt"
        kind "StaticLib"
        language "C"
        location (_ACTION .. "/" .. project().name)

        -- disable sse and sse2 for valve binary compat.
        -- the osx build of vaudio_celt uses sse2
        configuration "windows"
            buildoptions "/arch:IA32"
        configuration "linux"
            buildoptions "-mfpmath=387"
        configuration "macosx"
            buildoptions { "-mfpmath=sse", "-msse2" }
        configuration {}

        defines
        {
            "CUSTOM_MODES",
            "CUSTOM_MODES_ONLY",
            "DISABLE_FLOAT_API",
            "HAVE_CONFIG_H"
        }
        includedirs
        {
            base_dir .. "include/celt/"
        }
        files
        {
            base_dir .. "**.h",
            base_dir .. "**.c"
        }
        excludes
        {
            base_dir .. "static_modes_fixed.c",
            base_dir .. "static_modes_float.c"
        }
    project "*"
group ""
