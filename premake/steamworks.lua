
local output_dir = "../../bin"
local steamworks_lib_dir = "../external/steamworks_sdk_155/sdk/redistributable_bin/"
local lib_dir = ""
local lib_name = ""
local path_sep = ""
local copy_cmd = ""
if (os.get() == "windows") then
    lib_dir = ""
    lib_name = "steam_api.dll"
    path_sep = "\\"
    copy_cmd = "copy "
elseif (os.get() == "linux") then
    lib_dir = "linux32/"
    lib_name = "libsteam_api.so"
    path_sep = "/"
    copy_cmd = "cp -f "
elseif (os.get() == "macosx") then
    lib_dir = "osx/"
    lib_name = "libsteam_api.dylib"
    path_sep = "/"
    copy_cmd = "cp -f "
end

includedirs "../external/steamworks_sdk_155/sdk/public"
links "steam_api"
libdirs (steamworks_lib_dir .. lib_dir)

local fullLibPath = ("../" .. steamworks_lib_dir .. lib_dir .. lib_name)
postbuildcommands { path.translate(copy_cmd .. fullLibPath .. " " .. output_dir, path_sep) }
