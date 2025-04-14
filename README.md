# SW.Module.Logger

The SW.Module.Logger is a lightweight, flexible logging utility designed for seamless integration into any project. It provides easy-to-use features for logging messages, errors, warnings, and other information either to the console or to a file. The SW.Module.Logger is structured for simplicity and high configurability, allowing developers to tailor logging behaviors and formats to their specific needs.

## Tested Platforms

Used C++ dialect version: `20` (or higher, the std::format library is required unless you use custom formatters from fmt, see `SW_LOGGER_USE_FMT` in the docs).

- [x] Windows (`MSVC 2022`)
- [x] Linux (`clang 18.x`)

## Dependencies (SW modules)

| Module           | Link                                                  | Version |
| ---------------- | ----------------------------------------------------- | ------- |
| SW.Module.Common | https://github.com/Tycjan-Fortuna-IT/SW.Module.Common | master  |

Placed in the same directory as the SW.Module.Logger module.
It is recommended to keep all modules in the same directory e.g. `Engine/modules/`.

## Installation (CMake)

Add the following to your CMakeLists.txt file:

```cmake
add_subdirectory(PATH_TO_THIS_MODULE)
target_link_libraries(${PROJECT_NAME} Logger)
target_include_directories(${PROJECT_NAME} PUBLIC PATH_TO_THIS_MODULE/src)
```

Example:

```cmake
add_subdirectory(Modules/Logger)
target_link_libraries(${PROJECT_NAME} Logger)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/Engine/Modules/Logger/src)
```

## Installation (Premake5 - FancyBuildSystem)

Add inside your main dependencies.lua following line:

```lua
FBS.ImportModule("Engine/modules/Logger");
```

Replacing with correct path to the module.

Otherwise if not using FBS, just follow the standard installation process of libraries in Premake5.

## Example usage

```cpp
#include <Logger/Logger.hpp>

BEGIN_ADV_FORMATTER(std::filesystem::path);

std::string path = value.string();
std::copy(path.begin(), path.end(), it);

END_ADV_FORMATTER;


int main(int argc, char* argv[])
{
	const SW::LogSystemSpecification spec = {
	    .LogFilePath              = "logs/SW.log",
	    .ConsoleSinkLoggerPattern = "%^[%T] [%n] [%l]: %v%$",
	    .FileSinkLoggerPattern    = "[%T] [%l] [%n] [%l]: %v",
	    .EngineLoggerName         = "SYSTEM",
	    .RuntimeLoggerName            = "APP",
	};

        // Initialize the logger. Important! This must be called before any logging is done.
	SW::LogSystem::Initialize(spec);

	std::filesystem::path path = "C:/Users/username/Documents";
	SW_ENGINE_TRACE("This is a trace message {} / {}", 12, path);

	SW_ENGINE_TRACE("This is a trace message {} / {}", 12, "some text");
	SW_ENGINE_INFO("This is an info message {} / {}", 12, "some text");
	SW_ENGINE_DEBUG("This is a debug message {} / {}", 12, "some text");
	SW_ENGINE_WARN("This is a warning message {} / {}", 12, "some text");
	SW_ENGINE_ERROR("This is an error message {} / {}", 12, "some text");
	SW_ENGINE_FATAL("This is a fatal message {} / {}", 12, "some text");

	SW_RUNTIME_TRACE("This is a trace message {} / {}", 12, "some text");
	SW_RUNTIME_INFO("This is an info message {} / {}", 12, "some text");
	SW_RUNTIME_DEBUG("This is a debug message {} / {}", 12, "some text");
	SW_RUNTIME_WARN("This is a warning message {} / {}", 12, "some text");
	SW_RUNTIME_ERROR("This is an error message {} / {}", 12, "some text");
	SW_RUNTIME_FATAL("This is a fatal message {} / {}", 12, "some text");

        ASSERT(false, "This is an assertion failure message {} / {}", 12, "some text");

        VERIFY(false, "This is a verification failure message {} / {}", 12, "some text");

        // Shutdown the logger. Important! This must be called before the application exits.
	SW::LogSystem::Shutdown();

	return 0;
}
```

## Custom Formatters

For any type that you want to log, you can define a custom formatter by specializing the `formatter` class.

Example usage:

1. Cast (for easily convertible types)

```cpp
BEGIN_CAST_FORMATTER(std::filesystem::path);

FORMATTER_CAST(std::string, value.string());

END_CAST_FORMATTER;
```

2. Basic

```cpp
BEGIN_ADV_FORMATTER(std::filesystem::path);

std::string path = value.string();
std::copy(path.begin(), path.end(), it);

END_ADV_FORMATTER;
```

The above code will allow you to log `std::filesystem::path` objects.

3. Advanced

```cpp
struct Vec3
{
	float x, y, z;
};

BEGIN_ADV_FORMATTER(Vec3);

*it++ = '[';
it    = FORMATTER_FORMAT(float, value.x);
*it++ = ',';
it    = FORMATTER_FORMAT(float, value.y);
*it++ = ',';
it    = FORMATTER_FORMAT(float, value.z);
*it++ = ']';

END_ADV_FORMATTER;
```

The above code will allow you to log `Vec3` objects in the following format: `[x, y, z]`.
