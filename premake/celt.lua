
local base_dir = (solution().basedir .. "/external/celt-0.11.1/")

group "external"
    project "celt"
        kind "StaticLib"
        language "C"
        location (_ACTION .. "/" .. project().name)

        defines
        {
            "restrict="
        }
        includedirs
        {
            base_dir .. "include/"
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
