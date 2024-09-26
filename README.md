# SW Logger Module

The SW Logger module is a simple logging module that can be used to log messages to the console or to a file. The module is designed to be simple to use and easy to integrate into any project.

## Tested Platforms

- [x] Windows (`MSVC`)
- [x] Linux (`clang`)

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

BEGIN_NEW_FORMATTER(std::filesystem::path);

std::string path = value.string();
std::copy(path.begin(), path.end(), it);

END_NEW_FORMATTER;


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

For any type that you want to log, you can define a custom formatter by specializing the `formatter` class. This can be achieved by `BEGIN_NEW_FORMATTER`, `END_FORMATTER` and `FORMATTER_FORMAT` macros.

Example usage:

1. Basic

```cpp
BEGIN_NEW_FORMATTER(std::filesystem::path);

std::string path = value.string();
std::copy(path.begin(), path.end(), it);

END_NEW_FORMATTER;
```

The above code will allow you to log `std::filesystem::path` objects.

2. Advanced

```cpp
struct Vec3
{
	float x, y, z;
};

BEGIN_NEW_FORMATTER(Vec3);

*it++ = '[';
it    = FORMATTER_FORMAT(float, value.x);
*it++ = ',';
it    = FORMATTER_FORMAT(float, value.y);
*it++ = ',';
it    = FORMATTER_FORMAT(float, value.z);
*it++ = ']';

END_NEW_FORMATTER;
```

The above code will allow you to log `Vec3` objects in the following format: `[x, y, z]`.
