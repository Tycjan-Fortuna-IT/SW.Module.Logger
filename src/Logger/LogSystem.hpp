/**
 * @file LogSystem.hpp
 * @author TF
 * @copyright Copyright (c) 2025 TF
 */
#pragma once

/**
 * @def SW_LOGGER_DISABLE_ENGINE_LOGS
 * @brief Disables engine logging when defined.
 * @details If this macro is defined before including the logger header, engine logs (e.g., logs tagged with
 * LogType::ENGINE) will be disabled at compile-time.
 * @note Typically set via build system configuration (e.g., -D flag in compiler options).
 * @see LogType::ENGINE, LogSystem
 */

/**
 * @def SW_LOGGER_DISABLE_RUNTIME_LOGS
 * @brief Disables runtime logging when defined.
 * @details If this macro is defined before including the logger header, runtime logs (e.g., logs tagged with
 * LogType::RUNTIME) will be disabled at compile-time.
 * @note Typically set via build system configuration (e.g., -D flag in compiler options).
 * @see LogType::RUNTIME, LogSystem
 */

/**
 * @def SW_LOGGER_DISABLE_ASSERTS
 * @brief Disables assertions when defined.
 * @details If this macro is defined before including the logger header, assertions (e.g., ASSERT macros) will be
 * disabled at compile-time.
 * @note Typically set via build system configuration (e.g., -D flag in compiler options).
 * @see ASSERT
 */

/**
 * @def SW_LOGGER_USE_FMT
 * @brief Enables the use of fmt library for logging when defined.
 * @details If this macro is defined before including the logger header, the fmt library will be used for logging
 * formatting instead of the standard library's format functions.
 * @note Typically set via build system configuration (e.g., -D flag in compiler options).
 */

#include <source_location>

#include <spdlog/spdlog.h>

#if defined(SW_PLATFORM_WINDOWS) && !defined(SW_LOGGER_USE_FMT)
	#define formatns std

	#include <format>
#else
	#define formatns fmt

	#include <spdlog/fmt/fmt.h>
#endif

#include <Common/Defines.hpp>

/**
 * @brief This macro is used to define a new formatter for a custom type.
 *
 * @param T The type for which the formatter is defined.
 *
 * The formatter is defined in the formatns namespace. Either fmt or std. Based on the USE_FMT_FOR_LOGGING macro.
 *
 * Must be used in the global namespace. And closed with the END_CAST_FORMATTER macro.
 *
 * Example usage:
 *
 *     BEGIN_CAST_FORMATTER(std::filesystem::path);
 *     FORMATTER_CAST(std::string, value.string());
 *     END_CAST_FORMATTER;
 *
 *     The above code will define a formatter for std::filesystem::path type.
 *
 *     Available variables:
 *       - value: The value of the type to be formatted.
 *       - ctx: The format context.
 */
#define BEGIN_CAST_FORMATTER(T)                                                                            \
	namespace formatns                                                                                     \
	{                                                                                                      \
		template <>                                                                                        \
		struct formatter<T> : formatter<std::string>                                                       \
		{                                                                                                  \
			formatns::format_context::iterator format(const T& value, formatns::format_context& ctx) const \
			{

#define END_CAST_FORMATTER \
	}                      \
	}                      \
	;                      \
	}

/**
 * @brief This macro is used to format a value using the custom formatter and add it to the output buffer.
 *
 * @param T The type of the value to be formatted.
 * @param value The value to be formatted.
 *
 * Example usage:
 *      FORMATTER_CAST(std::string, value.string());
 */
#define FORMATTER_CAST(T, value) return formatns::formatter<T>().format(value, ctx)

/**
 * @brief This macro is used to define a new formatter for a custom type.
 *
 * @param T The type for which the formatter is defined.
 *
 * The formatter is defined in the formatns namespace. Either fmt or std. Based on the USE_FMT_FOR_LOGGING macro.
 *
 * Must be used in the global namespace. And closed with the END_ADV_FORMATTER macro.
 *
 * Example usage:
 *
 *     struct Vec3
 *     {
 *         float x, y, z;
 *     };
 *
 *     BEGIN_ADV_FORMATTER(Vec3);
 *
 *     *it++ = '[';
 *     it    = FORMATTER_FORMAT(float, value.x);
 *     *it++ = ',';
 *     it    = FORMATTER_FORMAT(float, value.y);
 *     *it++ = ',';
 *     it    = FORMATTER_FORMAT(float, value.z);
 *     *it++ = ']';
 *
 *     END_ADV_FORMATTER;
 *
 *     The above code will define a formatter for std::filesystem::path type.
 *
 *     Available variables:
 *       - value: The value of the type to be formatted.
 *       - it: The iterator to the output buffer.
 *       - ctx: The format context.
 */
#define BEGIN_ADV_FORMATTER(T)                                                                            \
	namespace formatns                                                                                    \
	{                                                                                                     \
		template <>                                                                                       \
		struct formatter<T, char>                                                                         \
		{                                                                                                 \
			constexpr formatns::format_parse_context::iterator parse(formatns::format_parse_context& ctx) \
			{                                                                                             \
				formatns::format_parse_context::iterator it = ctx.begin(), end = ctx.end();               \
				if (it != end && *it != '}')                                                              \
					throw formatns::format_error("invalid format");                                       \
				return it;                                                                                \
			}                                                                                             \
			formatns::format_context::iterator format(T value, formatns::format_context& ctx) const       \
			{                                                                                             \
				formatns::format_context::iterator it = ctx.out();

#define END_ADV_FORMATTER \
	return it;            \
	}                     \
	}                     \
	;                     \
	}

/**
 * @brief This macro is used to format a value using the custom formatter and add it to the output buffer.
 *
 * @param T The type of the value to be formatted.
 * @param value The value to be formatted.
 *
 * Example usage:
 *      it    = FORMATTER_FORMAT(float, value);
 */
#define FORMATTER_FORMAT(T, value) formatns::formatter<T, char>().format(value, ctx)

namespace SW::Logger
{

	/**
	 * @brief Enumeration defining different log types.
	 * This is used to differentiate between engine and runtime logs.
	 * The engine logs are related to the system, while the runtime logs are application-specific messages.
	 */
	enum class LogType
	{
		ENGINE,  ///< system-related messages.
		RUNTIME, ///< application-specific messages.
	};

	/**
	 * @brief Structure defining the log system specification.
	 */
	struct LogSystemSpecification
	{
		const char* LogFilePath              = "logs/SW.log";                     ///< Path to the obtained log file.
		const char* EngineLoggerName         = "ENGINE";                          ///< Name of the engine logger.
		const char* RuntimeLoggerName        = "RUNTIME";                         ///< Name of the runtime logger.
		const char* ConsoleSinkLoggerPattern = "%^[%T] [%n] [%l] [%s:%#]: %v%$";  ///< Pattern for console sink logger.
		const char* FileSinkLoggerPattern    = "[%T] [%l] [%n] [%l] [%s:%#]: %v"; ///< Pattern for file sink logger.
	};

	/**
	 * @brief LogSystem class that handles logging messages.
	 * @warning Initialize() must be called before any logging is done! Otherwise \b UB or crash!
	 * @warning Shutdown() must be called after all logging is done, before application shutdown!
	 */
	class LogSystem
	{
	public:
		/**
		 * @brief Initializes the log system.
		 *
		 * @param spec Specification for the log system.
		 * @warning This method must be called before any logging is done.
		 */
		static void Initialize(const LogSystemSpecification& spec = {});

		/**
		 * @brief Shuts down the log system.
		 *
		 * @warning This method must be called after all logging is done, before application shutdown.
		 */
		static void Shutdown();

		/**
		 * @brief Add an additional sink to the engine logger.
		 * @note This is useful for adding custom sinks to the logger for different output targets.
		 * @param sink The sink to be added.
		 */
		static void AddEngineSink(const spdlog::sink_ptr& sink);

		/**
		 * @brief Add an additional sink to the runtime logger.
		 * @note This is useful for adding custom sinks to the logger for different output targets.
		 * @param sink The sink to be added.
		 */
		static void AddRuntimeSink(const spdlog::sink_ptr& sink);

	public:
		/**
		 * @brief Logger for engine-related messages.
		 * @warning Do not use this logger directly. Use the macros instead.
		 */
		static std::shared_ptr<spdlog::logger> EngineLogger;

		/**
		 * @brief Logger for runtime-related messages.
		 * @warning Do not use this logger directly. Use the macros instead.
		 */
		static std::shared_ptr<spdlog::logger> RuntimeLogger;

	private:
		static LogSystemSpecification s_Spec; ///< Specification for the log system.
	};

} // namespace SW::Logger

// for private use to gather the source location
[[nodiscard]] static constexpr spdlog::source_loc GetCurrentSourceLocation(
    const std::source_location& location = std::source_location::current())
{
	return spdlog::source_loc{location.file_name(), static_cast<int32>(location.line()), location.function_name()};
}

/**
 * @def FORMAT(str, ...)
 * @brief Formats a string using std::vformat with variadic arguments.
 * @details Constructs a formatted string using the provided format string and arguments,
 *          leveraging C++20's std::vformat. The result is returned as a std::string.
 * @param str The format string (std::string_view compatible).
 * @param ... Variable arguments to be formatted into the string.
 * @return std::string containing the formatted result.
 * @note Requires C++20 or later for std::vformat support.
 * @warning The format string must be valid, or a std::format_error exception may be thrown.
 * @see std::vformat, FORMAT_C_STR
 * @example
 * @code
 * std::string result = FORMAT("Value: {}", 42);           // Returns "Value: 42"
 * std::string multi = FORMAT("{} + {} = {}", 2, 3, 5);   // Returns "2 + 3 = 5"
 * @endcode
 */
#define FORMAT(str, ...) std::vformat(str, std::make_format_args(__VA_ARGS__))

/**
 * @def FORMAT_C_STR(str, ...)
 * @brief Formats a string and returns it as a C-style string.
 * @details Similar to FORMAT, but returns the result as a null-terminated const char*
 *          by calling .c_str() on the formatted std::string. Useful for interfacing
 *          with C APIs or legacy code.
 * @param str The format string (std::string_view compatible).
 * @param ... Variable arguments to be formatted into the string.
 * @return const char* pointing to the formatted string's data.
 * @note The returned pointer is only valid as long as the temporary std::string exists.
 * @warning Lifetime of the returned C-string is tied to the temporary std::string;
 *          do not store it beyond the expression's full evaluation.
 * @see FORMAT, std::vformat
 * @example
 * @code
 * const char* result = FORMAT_C_STR("Error: {}", 404);  // Returns "Error: 404"
 * printf("%s\n", FORMAT_C_STR("Name: {}", "John"));     // Prints "Name: John"
 * @endcode
 */
#define FORMAT_C_STR(str, ...) FORMAT(str, __VA_ARGS__).c_str()

/**
 * @def WFORMAT(str, ...)
 * @brief Formats a wide string using std::wvformat with variadic arguments.
 * @details Constructs a formatted wide string using the provided format string and arguments,
 *          leveraging C++20's std::wvformat. The result is returned as a std::wstring.
 * @param str The format string (std::wstring_view compatible).
 * @param ... Variable arguments to be formatted into the wide string.
 * @return std::wstring containing the formatted result.
 * @note Requires C++20 or later for std::wvformat support.
 * @warning The format string must be valid, or a std::format_error exception may be thrown.
 * @see std::wvformat, WFORMAT_C_STR
 * @example
 * @code
 * std::wstring result = WFORMAT(L"Value: {}", 42);           // Returns L"Value: 42"
 * std::wstring multi = WFORMAT(L"{} + {} = {}", 2, 3, 5);   // Returns L"2 + 3 = 5"
 * @endcode
 */
#define WFORMAT(str, ...) std::vformat(str, std::make_wformat_args(__VA_ARGS__))

/**
 * @def WFORMAT_C_STR(str, ...)
 * @brief Formats a wide string and returns it as a C-style wide string.
 * @details Similar to WFORMAT, but returns the result as a null-terminated const wchar_t*
 *          by calling .c_str() on the formatted std::wstring. Useful for interfacing
 *          with wide-character C APIs or legacy code.
 * @param str The format string (std::wstring_view compatible).
 * @param ... Variable arguments to be formatted into the wide string.
 * @return const wchar_t* pointing to the formatted wide string's data.
 * @note The returned pointer is only valid as long as the temporary std::wstring exists.
 * @warning Lifetime of the returned C-string is tied to the temporary std::wstring;
 *          do not store it beyond the expression's full evaluation.
 * @see WFORMAT, std::wvformat
 * @example
 * @code
 * const wchar_t* result = WFORMAT_C_STR(L"Error: {}", 404);  // Returns L"Error: 404"
 * wprintf(L"%s\n", WFORMAT_C_STR(L"Name: {}", L"John"));    // Prints L"Name: John"
 * @endcode
 */
#define WFORMAT_C_STR(str, ...) WFORMAT(str, __VA_ARGS__).c_str()

#ifndef SW_LOGGER_DISABLE_ENGINE_LOGS

    /**
     * @def SW_ENGINE_TRACE(fmt, ...)
     * @brief Logs a trace message for verbose debugging.
     * @details Logs a message at the trace level, including the current source location, using the EngineLogger.
     *          This macro should be used for detailed debugging information that is typically disabled in production.
     * @param fmt The message format string.
     * @param ... Variable arguments to be formatted into the message.
     * @example
     * @code
     * SW_ENGINE_TRACE("Processing item: {}", item_id);  // Logs "Processing item: 42" at trace level
     * @endcode
     */
	#define SW_ENGINE_TRACE(fmt, ...)                                                                \
		::SW::Logger::LogSystem::EngineLogger->log(GetCurrentSourceLocation(), spdlog::level::trace, \
		                                           fmt VA_OPTIONAL_EXPANSION(__VA_ARGS__))

    /**
     * @def SW_ENGINE_INFO(fmt, ...)
     * @brief Logs an informational message for non-error events.
     * @details Logs a message at the info level, including the current source location, using the EngineLogger.
     *          This macro should be used to record normal operational events or status updates.
     * @param fmt The message format string.
     * @param ... Variable arguments to be formatted into the message.
     * @example
     * @code
     * SW_ENGINE_INFO("Application started with version: {}", version);  // Logs "Application started with version: 1.0"
     * @endcode
     */
	#define SW_ENGINE_INFO(fmt, ...)                                                                \
		::SW::Logger::LogSystem::EngineLogger->log(GetCurrentSourceLocation(), spdlog::level::info, \
		                                           fmt VA_OPTIONAL_EXPANSION(__VA_ARGS__))

    /**
     * @def SW_ENGINE_DEBUG(fmt, ...)
     * @brief Logs a debug message for general debugging purposes.
     * @details Logs a message at the debug level, including the current source location, using the EngineLogger.
     *          This macro should be used for debugging information that is more concise than trace-level logs.
     * @param fmt The message format string.
     * @param ... Variable arguments to be formatted into the message.
     * @example
     * @code
     * SW_ENGINE_DEBUG("Current state: {}", state);  // Logs "Current state: idle" at debug level
     * @endcode
     */
	#define SW_ENGINE_DEBUG(fmt, ...)                                                                \
		::SW::Logger::LogSystem::EngineLogger->log(GetCurrentSourceLocation(), spdlog::level::debug, \
		                                           fmt VA_OPTIONAL_EXPANSION(__VA_ARGS__))

    /**
     * @def SW_ENGINE_WARNING(fmt, ...)
     * @brief Logs a warning message for non-critical issues.
     * @details Logs a message at the warning level, including the current source location, using the EngineLogger.
     *          This macro should be used to indicate problems that may cause suboptimal application performance
     *          but do not prevent operation.
     * @param fmt The message format string.
     * @param ... Variable arguments to be formatted into the message.
     * @example
     * @code
     * SW_ENGINE_WARNING("Resource usage high: {}%", usage);  // Logs "Resource usage high: 85%" at warning level
     * @endcode
     */
	#define SW_ENGINE_WARNING(fmt, ...)                                                             \
		::SW::Logger::LogSystem::EngineLogger->log(GetCurrentSourceLocation(), spdlog::level::warn, \
		                                           fmt VA_OPTIONAL_EXPANSION(__VA_ARGS__))

    /**
     * @def SW_ENGINE_ERROR(fmt, ...)
     * @brief Logs an error message for critical runtime issues.
     * @details Logs a message at the error level, including the current source location, using the EngineLogger.
     *          This macro should be used to indicate serious problems that impair application functionality
     *          or prevent normal operation.
     * @param fmt The message format string.
     * @param ... Variable arguments to be formatted into the message.
     * @example
     * @code
     * SW_ENGINE_ERROR("Failed to load resource: {}", resource_name);  // Logs "Failed to load resource: texture.png"
     * @endcode
     */
	#define SW_ENGINE_ERROR(fmt, ...)                                                              \
		::SW::Logger::LogSystem::EngineLogger->log(GetCurrentSourceLocation(), spdlog::level::err, \
		                                           fmt VA_OPTIONAL_EXPANSION(__VA_ARGS__))

    /**
     * @def SW_ENGINE_CRITICAL(fmt, ...)
     * @brief Logs a fatal message and stops the application.
     * @details Logs a message at the critical level, including the current source location, using the EngineLogger.
     *          This macro should be used for unrecoverable errors that require immediate application termination.
     * @param fmt The message format string.
     * @param ... Variable arguments to be formatted into the message.
     * @example
     * @code
     * SW_ENGINE_CRITICAL("Fatal error in module: {}", module_name);  // Logs "Fatal error in module: core" and stops
     * @endcode
     */
	#define SW_ENGINE_CRITICAL(fmt, ...)                                                                \
		::SW::Logger::LogSystem::EngineLogger->log(GetCurrentSourceLocation(), spdlog::level::critical, \
		                                           fmt VA_OPTIONAL_EXPANSION(__VA_ARGS__))

// DEBUG_BREAK();
#else

	#define SW_ENGINE_TRACE(fmt, ...)
	#define SW_ENGINE_INFO(fmt, ...)
	#define SW_ENGINE_DEBUG(fmt, ...)
	#define SW_ENGINE_WARNING(fmt, ...)
	#define SW_ENGINE_ERROR(fmt, ...)
	#define SW_ENGINE_CRITICAL(fmt, ...)

#endif

#ifndef SW_LOGGER_DISABLE_RUNTIME_LOGS

    /**
     * @def SW_RUNTIME_TRACE(fmt, ...)
     * @brief Logs a trace message for verbose runtime debugging.
     * @details Logs a message at the trace level, including the current source location, using the RuntimeLogger.
     *          This macro should be used for detailed debugging information during runtime, typically disabled in
     * production.
     * @param fmt The message format string.
     * @param ... Variable arguments to be formatted into the message.
     * @example
     * @code
     * SW_RUNTIME_TRACE("Processing event: {}", event_id);  // Logs "Processing event: 123" at trace level
     * @endcode
     */
	#define SW_RUNTIME_TRACE(fmt, ...)                                                                \
		::SW::Logger::LogSystem::RuntimeLogger->log(GetCurrentSourceLocation(), spdlog::level::trace, \
		                                            fmt VA_OPTIONAL_EXPANSION(__VA_ARGS__))

    /**
     * @def SW_RUNTIME_INFO(fmt, ...)
     * @brief Logs an informational message for non-error runtime events.
     * @details Logs a message at the info level, including the current source location, using the RuntimeLogger.
     *          This macro should be used to record normal runtime operational events or status updates.
     * @param fmt The message format string.
     * @param ... Variable arguments to be formatted into the message.
     * @example
     * @code
     * SW_RUNTIME_INFO("Runtime initialized with mode: {}", mode);  // Logs "Runtime initialized with mode: normal"
     * @endcode
     */
	#define SW_RUNTIME_INFO(fmt, ...)                                                                \
		::SW::Logger::LogSystem::RuntimeLogger->log(GetCurrentSourceLocation(), spdlog::level::info, \
		                                            fmt VA_OPTIONAL_EXPANSION(__VA_ARGS__))

    /**
     * @def SW_RUNTIME_DEBUG(fmt, ...)
     * @brief Logs a debug message for general runtime debugging.
     * @details Logs a message at the debug level, including the current source location, using the RuntimeLogger.
     *          This macro should be used for debugging information that is more concise than trace-level logs during
     * runtime.
     * @param fmt The message format string.
     * @param ... Variable arguments to be formatted into the message.
     * @example
     * @code
     * SW_RUNTIME_DEBUG("Current runtime state: {}", state);  // Logs "Current runtime state: active" at debug level
     * @endcode
     */
	#define SW_RUNTIME_DEBUG(fmt, ...)                                                                \
		::SW::Logger::LogSystem::RuntimeLogger->log(GetCurrentSourceLocation(), spdlog::level::debug, \
		                                            fmt VA_OPTIONAL_EXPANSION(__VA_ARGS__))

    /**
     * @def SW_RUNTIME_WARNING(fmt, ...)
     * @brief Logs a warning message for non-critical runtime issues.
     * @details Logs a message at the warning level, including the current source location, using the RuntimeLogger.
     *          This macro should be used to indicate problems that may cause suboptimal runtime performance
     *          but do not prevent operation.
     * @param fmt The message format string.
     * @param ... Variable arguments to be formatted into the message.
     * @example
     * @code
     * SW_RUNTIME_WARNING("Runtime memory usage high: {}%", usage);  // Logs "Runtime memory usage high: 90%" at warning
     * level
     * @endcode
     */
	#define SW_RUNTIME_WARNING(fmt, ...)                                                             \
		::SW::Logger::LogSystem::RuntimeLogger->log(GetCurrentSourceLocation(), spdlog::level::warn, \
		                                            fmt VA_OPTIONAL_EXPANSION(__VA_ARGS__))

    /**
     * @def SW_RUNTIME_ERROR(fmt, ...)
     * @brief Logs an error message for critical runtime issues.
     * @details Logs a message at the error level, including the current source location, using the RuntimeLogger.
     *          This macro should be used to indicate serious problems that impair runtime functionality
     *          or prevent normal operation.
     * @param fmt The message format string.
     * @param ... Variable arguments to be formatted into the message.
     * @example
     * @code
     * SW_RUNTIME_ERROR("Failed to process data: {}", data_id);  // Logs "Failed to process data: 456" at error level
     * @endcode
     */
	#define SW_RUNTIME_ERROR(fmt, ...)                                                              \
		::SW::Logger::LogSystem::RuntimeLogger->log(GetCurrentSourceLocation(), spdlog::level::err, \
		                                            fmt VA_OPTIONAL_EXPANSION(__VA_ARGS__))

    /**
     * @def SW_RUNTIME_CRITICAL(fmt, ...)
     * @brief Logs a fatal message and stops the runtime.
     * @details Logs a message at the critical level, including the current source location, using the RuntimeLogger.
     *          This macro should be used for unrecoverable errors that require immediate runtime termination.
     * @param fmt The message format string.
     * @param ... Variable arguments to be formatted into the message.
     * @example
     * @code
     * SW_RUNTIME_CRITICAL("Fatal runtime error in component: {}", component);  // Logs "Fatal runtime error in
     * component: network" and stops
     * @endcode
     */
	#define SW_RUNTIME_CRITICAL(fmt, ...)                                                                \
		::SW::Logger::LogSystem::RuntimeLogger->log(GetCurrentSourceLocation(), spdlog::level::critical, \
		                                            fmt VA_OPTIONAL_EXPANSION(__VA_ARGS__))

#else

	#define SW_RUNTIME_TRACE(fmt, ...)
	#define SW_RUNTIME_INFO(fmt, ...)
	#define SW_RUNTIME_DEBUG(fmt, ...)
	#define SW_RUNTIME_WARNING(fmt, ...)
	#define SW_RUNTIME_ERROR(fmt, ...)
	#define SW_RUNTIME_CRITICAL(fmt, ...)

#endif

#ifndef SW_LOGGER_DISABLE_ASSERTS

    /**
     * @def ASSERT(x, fmt, ...)
     * @brief Asserts that an expression is true, halting the application on failure.
     * @details Evaluates the given expression and logs a critical message with the source location if the expression is
     * false, then triggers a debug break. This macro is only active if SW_LOGGER_DISABLE_ASSERTS is not defined. It
     * should be used for debugging conditions that must hold true during development.
     * @param x The expression to evaluate.
     * @param fmt The message format string (compatible with spdlog formatting) to describe the failure.
     * @param ... Variable arguments to be formatted into the message.
     * @warning The application will halt if the assertion fails via DEBUG_BREAK().
     * @example
     * @code
     * ASSERT(ptr != nullptr, "Pointer is null in function: {}", func_name);  // Halts if ptr is null, logs message
     * @endcode
     */
	#define ASSERT(x, fmt, ...)                                                                                    \
		{                                                                                                          \
			if (!(x))                                                                                              \
			{                                                                                                      \
				::SW::Logger::LogSystem::EngineLogger->log(GetCurrentSourceLocation(), spdlog::level::critical,    \
				                                           "Assertion failed: (" #x                                \
				                                           ")\nMessage: " fmt VA_OPTIONAL_EXPANSION(__VA_ARGS__)); \
				DEBUG_BREAK();                                                                                     \
			}                                                                                                      \
		}
#else

	#define ASSERT(x, fmt, ...)

#endif // SW_LOGGER_DISABLE_ASSERTS

/**
 * @def VERIFY(x, fmt, ...)
 * @brief Verifies that an expression is true, halting the application on failure.
 * @details Evaluates the given expression and logs a critical message with the source location if the expression is
 * false, then triggers a debug break. This macro is always active, regardless of build configuration. It should be used
 * for conditions that must be checked even in release builds.
 * @param x The expression to evaluate.
 * @param fmt The message format string (compatible with spdlog formatting) to describe the failure.
 * @param ... Variable arguments to be formatted into the message.
 * @warning The application will halt if the verification fails via DEBUG_BREAK().
 * @example
 * @code
 * VERIFY(config_loaded, "Failed to load configuration: {}", config_path);  // Halts if config_loaded is false, logs
 * message
 * @endcode
 */
#define VERIFY(x, fmt, ...)                                                                                    \
	{                                                                                                          \
		if (!(x))                                                                                              \
		{                                                                                                      \
			::SW::Logger::LogSystem::EngineLogger->log(GetCurrentSourceLocation(), spdlog::level::critical,    \
			                                           "Assertion failed: (" #x                                \
			                                           ")\nMessage: " fmt VA_OPTIONAL_EXPANSION(__VA_ARGS__)); \
			DEBUG_BREAK();                                                                                     \
		}                                                                                                      \
	}
