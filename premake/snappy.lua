
local base_dir = (solution().basedir .. "/external/snappy-1.1.3/")

group "external"
    project "snappy"
        kind "StaticLib"
        language "C++"
        location (_ACTION .. "/" .. project().name)

        includedirs
        {
            base_dir .. "include/"
        }
        files
        {
            base_dir .. "**.h",
            base_dir .. "**.cc"
        }
    project "*"
group ""
