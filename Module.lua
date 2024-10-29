return function(basePath)
    return {
        Name = "SW Logger Module",
        LibsToLink = { "Logger" },
        IncludeDirs = {
            basePath .. "/src",
            basePath .. "/vendor/spdlog/include"
        },
    }
end