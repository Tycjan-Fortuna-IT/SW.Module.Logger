return function(importPath)
    return {
        Name = "SW.Module.Logger",
        LibsToLink = { "SW.Module.Logger" },
        IncludeDirs = {
            importPath .. "/src", importPath .. "/vendor/spdlog/include"
        },
    }
end