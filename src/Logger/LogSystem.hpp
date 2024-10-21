/**
 * @file LogSystem.hpp
 * @author SW
 * @version 0.2.1
 * @date 2024-21-10
 *
 * @copyright Copyright (c) 2024 SW
 */
#pragma once

#include <spdlog/spdlog.h>

#include <memory>
#include <string>

#ifdef SW_LOGGER_USE_FMT
	#define formatns fmt

	#include <spdlog/fmt/fmt.h>
#else
	#define formatns std

	#include <format>
#endif

#ifdef __GNUC__
	#define VA_OPTIONAL_EXPANSION(...) __VA_OPT__(, )##__VA_ARGS__
#else
	#define VA_OPTIONAL_EXPANSION(...) , ##__VA_ARGS__
#endif

#ifndef DEBUG_BREAK
	#ifdef SW_PLATFORM_WINDOWS
		#define DEBUG_BREAK() __debugbreak()
	#else
		#include <signal.h>
		#define DEBUG_BREAK() raise(SIGTRAP)
	#endif
#endif

namespace SW
{
	using u8  = unsigned char;
	using i16 = signed short;

	// Enumeration defining different log types.
	enum LogType : u8
	{
		SYSTEM, // system-related messages.
		APP,    // application-specific messages.
	};

	// Enumeration defining different log levels.
	enum LogLevel : u8
	{
		LOG_LEVEL_TRACE, // trace messages
		LOG_LEVEL_INFO,  // informational messages
		LOG_LEVEL_WARN,  // warning messages
		LOG_LEVEL_DEBUG, // debug messages
		LOG_LEVEL_ERROR, // error messages
		LOG_LEVEL_FATAL, // fatal messages
	};

	// Log system specification struct.
	// For more information about custom formatting, see spdlog documentation.
	// (https://github.com/gabime/spdlog/wiki/3.-Custom-formatting)
	struct LogSystemSpecification
	{
		std::string LogFileName              = "logs/SW.log";
		std::string ConsoleSinkLoggerPattern = "%^[%T] [%n] [%l]: %v%$";
		std::string FileSinkLoggerPattern    = "[%T] [%l] [%n] [%l]: %v";
		std::string SystemLoggerName         = "SYSTEM";
		std::string AppLoggerName            = "APP";
	};

	// Log system class that handles logging messages.
	// Initialize() must be called before any logging is done!
	// Shutdown() must be called after all logging is done, before application shutdown!
	class LogSystem
	{
	public:
		static void Initialize(const LogSystemSpecification& spec = {});
		static void Shutdown();

		// Adds a sink to the system logger.
		static void AddSystemSink(const spdlog::sink_ptr& sink);

		// Adds a sink to the app logger.
		static void AddAppSink(const spdlog::sink_ptr& sink);

		// Prints using spdlog logger logs to specified sinks.
		// This method should not be used directly, but rather through the FATAL, ERROR, WARN, INFO, DEBUG, and TRACE
		// macros. example usage: PrintMessage(LogType::ENGINE, LogLevel::LOG_LEVEL_FATAL, "Assertion failed: ", "info
		// --> {}", "some info {}", 5);
		template <typename... Args>
		static void PrintMessage(LogType type, LogLevel level, std::string_view tag, std::string_view format = "",
		                         Args&&... args)
		{
			std::string message = formatns::vformat(format, formatns::make_format_args(args...));

			PrepareAndPrint(type, level, tag, message);
		}

		// Reports an assertion failure.
		// This method should not be used directly, but rather through the ASSERT macro.
		static void ReportAssertionFailure(const char* expression, const char* message, const char* file, i16 line);

	private:
		// Prepares and sends the message to the logger.
		static void PrepareAndPrint(LogType type, LogLevel level, std::string_view tag, const std::string& message);
	};

#ifndef SW_LOGGER_DISABLE_ASSERTS
	// Asserts that an expression is true. If the expression is false, the application will be halted.
	// Does the check only if SW_LOGGER_ENABLE_ASSERTS is defined.
	#define ASSERT(x, ...)                                                                            \
		{                                                                                             \
			if (!(x))                                                                                 \
			{                                                                                         \
				::SW::LogSystem::PrintMessage(::SW::LogType::SYSTEM, ::SW::LogLevel::LOG_LEVEL_FATAL, \
				                              "Assertion failed: " #x                                 \
				                              " info --> " VA_OPTIONAL_EXPANSION(__VA_ARGS__));       \
				DEBUG_BREAK();                                                                        \
			}                                                                                         \
		}
#else
	#define ASSERT(x, ...)
#endif // SW_LOGGER_ENABLE_ASSERTS

// Verifies that an expression is true. If the expression is false, the application will be halted.
// Always enabled.
#define VERIFY(x, ...)                                                                                              \
	{                                                                                                               \
		if (!(x))                                                                                                   \
		{                                                                                                           \
			::SW::LogSystem::PrintMessage(::SW::LogType::SYSTEM, ::SW::LogLevel::LOG_LEVEL_FATAL,                   \
			                              "Assertion failed: " #x " info --> " VA_OPTIONAL_EXPANSION(__VA_ARGS__)); \
			DEBUG_BREAK();                                                                                          \
		}                                                                                                           \
	}

// Logs a message.
// This macro should not be used directly, but rather through the FATAL, ERROR, WARN, INFO, DEBUG, and TRACE macros.
#define SW_LOG(type, level, msg, ...)                                             \
	::SW::LogSystem::PrintMessage(::SW::LogType::type, ::SW::LogLevel::level, "", \
	                              msg VA_OPTIONAL_EXPANSION(__VA_ARGS__))

#ifndef SW_LOGGER_DISABLE_SYSTEM_LOGS
	// Logs a trace message. System use only.
	// This macro should be used for verbose debugging purposes.
	#define SYSTEM_TRACE(msg, ...) SW_LOG(SYSTEM, LOG_LEVEL_TRACE, msg, ##__VA_ARGS__)

	// Logs an info message. System use only.
	// This macro should be used for non-erronuous informational purposes.
	#define SYSTEM_INFO(msg, ...) SW_LOG(SYSTEM, LOG_LEVEL_INFO, msg, ##__VA_ARGS__)

	// Logs a debug message. System use only.
	// This macro should be used for debugging purposes.
	#define SYSTEM_DEBUG(msg, ...) SW_LOG(SYSTEM, LOG_LEVEL_DEBUG, msg, ##__VA_ARGS__)

	// Logs a warning message. System use only.
	// This macro should be used to indicate non-critial problems with the application that cause it to run
	// suboptimally.
	#define SYSTEM_WARN(msg, ...) SW_LOG(SYSTEM, LOG_LEVEL_WARN, msg, ##__VA_ARGS__)

	// Logs an error message. System use only.
	// This macro should be used to indicate critical runtime problems that cause the application to run improperly or
	// not at all.
	#define SYSTEM_ERROR(msg, ...) SW_LOG(SYSTEM, LOG_LEVEL_ERROR, msg, ##__VA_ARGS__)

	// Logs a fatal message. System use only.
	// This macro should be used to stop the application when hit.
	#define SYSTEM_FATAL(msg, ...)                               \
		{                                                        \
			SW_LOG(SYSTEM, LOG_LEVEL_FATAL, msg, ##__VA_ARGS__); \
			DEBUG_BREAK();                                       \
		}
#else
	#define SYSTEM_LOG(type, level, msg, ...)
	#define SYSTEM_TRACE(msg, ...)
	#define SYSTEM_INFO(msg, ...)
	#define SYSTEM_DEBUG(msg, ...)
	#define SYSTEM_WARN(msg, ...)
	#define SYSTEM_ERROR(msg, ...)
	#define SYSTEM_FATAL(msg, ...)
#endif

#ifndef SW_LOGGER_DISABLE_APP_LOGS
	// Logs a message. Application use only.
	// This macro should be used for verbose debugging purposes.
	#define APP_TRACE(msg, ...) SW_LOG(APP, LOG_LEVEL_TRACE, msg, ##__VA_ARGS__)

	// Logs a message. Application use only.
	// This macro should be used for non-erronuous informational purposes.
	#define APP_INFO(msg, ...) SW_LOG(APP, LOG_LEVEL_INFO, msg, ##__VA_ARGS__)

	// Logs a message. Application use only.
	// This macro should be used for debugging purposes.
	#define APP_DEBUG(msg, ...) SW_LOG(APP, LOG_LEVEL_DEBUG, msg, ##__VA_ARGS__)

	// Logs a warning message. Application use only.
	// This macro should be used to indicate non-critial problems with the application that cause it to run
	// suboptimally.
	#define APP_WARN(msg, ...) SW_LOG(APP, LOG_LEVEL_WARN, msg, ##__VA_ARGS__)

	// Logs an error message. Application use only.
	// This macro should be used to indicate critical runtime problems that cause the application to run improperly or
	// not at all.
	#define APP_ERROR(msg, ...) SW_LOG(APP, LOG_LEVEL_ERROR, msg, ##__VA_ARGS__)

	// Logs a fatal message. Application use only.
	// This macro should be used to stop the application when hit.
	#define APP_FATAL(msg, ...)                               \
		{                                                     \
			SW_LOG(APP, LOG_LEVEL_FATAL, msg, ##__VA_ARGS__); \
			DEBUG_BREAK();                                    \
		}
#else
	#define APP_LOG(type, level, msg, ...)
	#define APP_TRACE(msg, ...)
	#define APP_INFO(msg, ...)
	#define APP_DEBUG(msg, ...)
	#define APP_WARN(msg, ...)
	#define APP_ERROR(msg, ...)
	#define APP_FATAL(msg, ...)
#endif
} // namespace SW
