
local output_dir = "../../bin"
local steamworks_lib_dir = "../external/steamworks_sdk_161/sdk/redistributable_bin/"
local lib_dir = ""
local lib_name = ""
local links_name = ""
local path_sep = ""
local copy_cmd = ""
if (os.get() == "windows") then
    lib_dir = "win64\\"
    lib_name = "steam_api64.dll"
    links_name = "steam_api64"
    path_sep = "\\"
    copy_cmd = "copy "
elseif (os.get() == "linux") then
    lib_dir = "linux64/"
    lib_name = "libsteam_api.so"
    links_name = "steam_api"
    path_sep = "/"
    copy_cmd = "cp -f "
elseif (os.get() == "macosx") then
    lib_dir = "osx/"
    lib_name = "libsteam_api.dylib"
    links_name = "steam_api"
    path_sep = "/"
    copy_cmd = "cp -f "
end

includedirs "../external/steamworks_sdk_161/sdk/public"
links (links_name)
libdirs (steamworks_lib_dir .. lib_dir)

local fullLibPath = ("../" .. steamworks_lib_dir .. lib_dir .. lib_name)
postbuildcommands { path.translate(copy_cmd .. fullLibPath .. " " .. output_dir, path_sep) }
