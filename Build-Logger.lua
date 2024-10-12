project "Logger"
    kind "StaticLib"

    warnings "Everything"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "src/**.hpp",
        "src/**.cpp",
    }

    includedirs {
        "src",
        "vendor/spdlog/include",
    }

    links {
        "spdlog"
    }

    include "vendor/spdlog"
