
local base_dir = (solution().basedir .. "/external/json_checker/")

group "external"
    project "json_checker"
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
            base_dir .. "**.cpp"
        }
    project "*"
group ""
