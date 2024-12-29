project "Logger"
    kind "StaticLib"

    warnings "Everything"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    defines {
        "SPDLOG_COMPILED_LIB"
    }

    files {
        "src/**.hpp",
        "src/**.cpp",
    }

    includedirs {
        "src",
    }

    externalwarnings "Off"

    externalincludedirs {
        "vendor/spdlog/include"
    }

    links {
        "spdlog"
    }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        runtime "Release"
        optimize "On"
        symbols "On"

    filter "configurations:Dist"
        runtime "Release"
        optimize "On"
        symbols "Off"

    filter "system:windows"
        defines { "SW_PLATFORM_WINDOWS" }

    filter "system:linux"
        defines { "SW_PLATFORM_LINUX" }

    include "vendor/spdlog"
