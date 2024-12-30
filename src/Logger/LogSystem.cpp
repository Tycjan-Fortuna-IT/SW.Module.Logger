#include "LogSystem.hpp"

#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace SW::Logger
{
#define ANSI_COLOR_BLACK      "\x1b[30m"
#define ANSI_COLOR_RED        "\x1b[31m"
#define ANSI_COLOR_GREEN      "\x1b[32m"
#define ANSI_COLOR_YELLOW     "\x1b[33m"
#define ANSI_COLOR_BLUE       "\x1b[34m"
#define ANSI_COLOR_MAGENTA    "\x1b[35m"
#define ANSI_COLOR_CYAN       "\x1b[36m"
#define ANSI_COLOR_WHITE      "\x1b[37m"
#define ANSI_BG_COLOR_BLACK   "\x1b[40m"
#define ANSI_BG_COLOR_RED     "\x1b[41m"
#define ANSI_BG_COLOR_GREEN   "\x1b[42m"
#define ANSI_BG_COLOR_YELLOW  "\x1b[43m"
#define ANSI_BG_COLOR_BLUE    "\x1b[44m"
#define ANSI_BG_COLOR_MAGENTA "\x1b[45m"
#define ANSI_BG_COLOR_CYAN    "\x1b[46m"
#define ANSI_BG_COLOR_WHITE   "\x1b[47m"
#define ANSI_COLOR_RESET      "\x1b[0m"

	std::shared_ptr<spdlog::logger> LogSystem::s_SystemLogger;
	std::shared_ptr<spdlog::logger> LogSystem::s_AppLogger;

	void LogSystem::Initialize(const LogSystemSpecification& spec /*= {}*/)
	{
		std::vector<spdlog::sink_ptr> logSinks;

		logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
		logSinks.emplace_back(std::make_shared<spdlog::sinks::daily_file_sink_mt>(spec.LogFileName, 0, 0));

		logSinks[0]->set_pattern(spec.ConsoleSinkLoggerPattern);
		logSinks[1]->set_pattern(spec.FileSinkLoggerPattern);

		s_SystemLogger = std::make_shared<spdlog::logger>(spec.SystemLoggerName, begin(logSinks), end(logSinks));
		s_SystemLogger->set_level(spdlog::level::trace);
		s_SystemLogger->flush_on(spdlog::level::trace);
		spdlog::register_logger(s_SystemLogger);

		s_AppLogger = std::make_shared<spdlog::logger>(spec.AppLoggerName, begin(logSinks), end(logSinks));
		s_AppLogger->set_level(spdlog::level::trace);
		s_AppLogger->flush_on(spdlog::level::trace);
		spdlog::register_logger(s_AppLogger);
	}

	void LogSystem::Shutdown()
	{
		s_SystemLogger.reset();
		s_AppLogger.reset();
		spdlog::drop_all();
	}

	void LogSystem::AddSystemSink(const spdlog::sink_ptr& sink)
	{
		s_SystemLogger->sinks().push_back(sink);
	}

	void LogSystem::AddAppSink(const spdlog::sink_ptr& sink)
	{
		s_AppLogger->sinks().push_back(sink);
	}

	void LogSystem::ReportAssertionFailure(const char* expression, const char* message, const char* file, i16 line)
	{
		LogSystem::PrintMessage(LogType::SYSTEM, LogLevel::LOG_LEVEL_FATAL,
		                        "Assertion Failure: {}, message: '{}', in file: {}, line: {}\n", expression, message,
		                        file, line);
	}

	void LogSystem::PrepareAndPrint(LogType type, LogLevel level, std::string_view tag, const std::string& message)
	{
		const std::shared_ptr<spdlog::logger>& logger = type == LogType::SYSTEM ? s_SystemLogger : s_AppLogger;

		PrepareAndPrint(logger, type, level, tag, message);
	}

	void LogSystem::PrepareAndPrint(const std::shared_ptr<spdlog::logger>& logger, LogType type, LogLevel level,
	                                std::string_view tag, const std::string& message)
	{
		switch (level)
		{
		case LOG_LEVEL_TRACE:
			logger->trace("{0}{1}", tag, message);
			break;
		case LOG_LEVEL_INFO:
			logger->info("{0}{1}", tag, message);
			break;
		case LOG_LEVEL_WARN:
			logger->warn("{0}{1}", tag, message);
			break;
		case LOG_LEVEL_DEBUG:
			logger->debug("{0}{1}", tag, message);
			break;
		case LOG_LEVEL_ERROR:
			logger->error("{0}{1}", tag, message);
			break;
		case LOG_LEVEL_FATAL:
			logger->critical("{0}{1}", tag, message);
			break;
		}
	}

} // namespace SW::Logger
