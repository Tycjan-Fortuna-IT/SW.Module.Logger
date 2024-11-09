return function(basePath)
    return {
        Name = "SW Logger Module",
        Defines = {
            "SPDLOG_COMPILED_LIB"
        },
        LibsToLink = { "Logger", "spdlog" },
        IncludeDirs = {
            basePath .. "/src",
            basePath .. "/vendor/spdlog/include"
        },
    }
end