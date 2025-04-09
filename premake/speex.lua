
local base_dir = (solution().basedir .. "/external/speex-Speex-1.2.1/")

group "external"
    project "speex"
        kind "StaticLib"
        language "C"
        location (_ACTION .. "/" .. project().name)

        defines
        {
            "HAVE_CONFIG_H"
        }
        includedirs
        {
            base_dir .. "include/"
        }
        files
        {
            base_dir .. "include/**.h",
            base_dir .. "libspeex/**.h",
            base_dir .. "libspeex/**.c",

            -- BUILD_VORBIS_PSY
            --"vorbis_psy.c",
            
            -- BUILD_SMALLFT
            --"smallft.c",
            
            -- BUILD_KISS_FFT
            --"kiss_fft.c", "_kiss_fft_guts.h", "kiss_fft.h", "kiss_fftr.c", "kiss_fftr.h",
        }
        excludes
        {
            base_dir .. "libspeex/testenc.c",
            base_dir .. "libspeex/testenc_uwb.c",
            base_dir .. "libspeex/testenc_wb.c"
        }
    project "*"
group ""
