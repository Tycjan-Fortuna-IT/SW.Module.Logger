project "SW.Module.Logger"
    kind "StaticLib"

    externalanglebrackets "On"
    externalwarnings "Off"

    targetdir ("%{wks.location}/bin/" .. outputdir)
	objdir ("%{wks.location}/bin-int/" .. outputdir)

    files {
        "src/**.hpp",
        "src/**.cpp",
    }

    includedirs {
        "src",
    }

    externalincludedirs {
        "../SW.Module.Common/src",
        "vendor/spdlog/include"
    }
    