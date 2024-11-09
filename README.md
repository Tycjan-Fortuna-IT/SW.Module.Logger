# SW Logger Module

The SW Logger module is a lightweight, flexible logging utility designed for seamless integration into any project. It provides easy-to-use features for logging messages, errors, warnings, and other information either to the console or to a file. The SW Logger module is structured for simplicity and high configurability, allowing developers to tailor logging behaviors and formats to their specific needs.

## Tested Platforms
Used C++ dialect version: `20`
- [x] Windows (`MSVC 2022`)
- [x] Linux (`clang 18.x`)

## Dependencies (SW modules)
None

## Installation (Premake5 - FancyBuildSystem)
Add inside your main dependencies.lua following line:
```lua
FBS.ImportModule("Engine/modules/Logger");
```
Replacing with correct path to the module.

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

## Additional Defines

- #### `SW_LOGGER_USE_FMT`

Use the `fmt` library for formatting log messages. If this is not defined, `#include <format>` will be used.

- #### `SW_LOGGER_DISABLE_ASSERTS`

Disable the use of `ASSERT`, removes them at compile time.

- #### `SW_LOGGER_DISABLE_SYSTEM_LOGS`

Disable the use of `SYSTEM_TRACE`, `SYSTEM_INFO`, `SYSTEM_DEBUG`, `SYSTEM_WARN`, `SYSTEM_ERROR`, `SYSTEM_FATAL`. Removes them at compile time.

- #### `SW_LOGGER_DISABLE_APP_LOGS`

Disable the use of `APP_TRACE`, `APP_INFO`, `APP_DEBUG`, `APP_WARN`, `APP_ERROR`, `APP_FATAL`. Removes them at compile time.

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
	    .LogFileName              = "logs/SW.log",
	    .ConsoleSinkLoggerPattern = "%^[%T] [%n] [%l]: %v%$",
	    .FileSinkLoggerPattern    = "[%T] [%l] [%n] [%l]: %v",
	    .SystemLoggerName         = "SYSTEM",
	    .AppLoggerName            = "APP",
	};

        // Initialize the logger. Important! This must be called before any logging is done.
	SW::LogSystem::Initialize(spec);

	std::filesystem::path path = "C:/Users/username/Documents";
	SYSTEM_TRACE("This is a trace message {} / {}", 12, path);

	SYSTEM_TRACE("This is a trace message {} / {}", 12, "some text");
	SYSTEM_INFO("This is an info message {} / {}", 12, "some text");
	SYSTEM_DEBUG("This is a debug message {} / {}", 12, "some text");
	SYSTEM_WARN("This is a warning message {} / {}", 12, "some text");
	SYSTEM_ERROR("This is an error message {} / {}", 12, "some text");
	SYSTEM_FATAL("This is a fatal message {} / {}", 12, "some text");

	APP_TRACE("This is a trace message {} / {}", 12, "some text");
	APP_INFO("This is an info message {} / {}", 12, "some text");
	APP_DEBUG("This is a debug message {} / {}", 12, "some text");
	APP_WARN("This is a warning message {} / {}", 12, "some text");
	APP_ERROR("This is an error message {} / {}", 12, "some text");
	APP_FATAL("This is a fatal message {} / {}", 12, "some text");

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
