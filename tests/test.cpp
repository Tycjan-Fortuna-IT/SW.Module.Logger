#include <Logger/Logger.hpp>

#include <filesystem>

BEGIN_CAST_FORMATTER(std::filesystem::path);
FORMATTER_CAST(std::string, value.string());
END_CAST_FORMATTER;

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

int main(int argc, char* argv[])
{
	const SW::Logger::LogSystemSpecification spec = {
	    .LogFileName              = "logs/SW.log",
	    .ConsoleSinkLoggerPattern = "%^[%T] [%n] [%l]: %v%$",
	    .FileSinkLoggerPattern    = "[%T] [%l] [%n] [%l]: %v",
	    .SystemLoggerName         = "SYSTEM",
	    .AppLoggerName            = "APP",
	};

	SW::Logger::LogSystem::Initialize(spec);

	std::filesystem::path path = "C:/Users/username/Documents";
	SYSTEM_TRACE("This is a trace message {} / {}", 12, path);

	Vec3 vector = {1.0f, 2.0f, 3.0f};
	SYSTEM_TRACE("This is a trace message {} / {}", 12, vector);

	ASSERT(false, "This is an assertion failure message {} / {}", 12, "some text");

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

	SW::Logger::LogSystem::Shutdown();

	return 0;
}
