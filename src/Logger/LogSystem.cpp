#include "LogSystem.hpp"

#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace SW::Logger
{
	LogSystemSpecification LogSystem::s_Spec = {};

	std::shared_ptr<spdlog::logger> LogSystem::EngineLogger;
	std::shared_ptr<spdlog::logger> LogSystem::RuntimeLogger;

	void LogSystem::AddEngineSink(const spdlog::sink_ptr& sink)
	{
		EngineLogger->sinks().push_back(sink);
	}

	void LogSystem::AddRuntimeSink(const spdlog::sink_ptr& sink)
	{
		RuntimeLogger->sinks().push_back(sink);
	}

	void LogSystem::Initialize(const LogSystemSpecification& spec)
	{
		s_Spec = spec;

		std::vector<spdlog::sink_ptr> logSinks;

		logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
		logSinks.emplace_back(std::make_shared<spdlog::sinks::daily_file_sink_mt>(spec.LogFilePath, 0, 0));

		logSinks[0]->set_pattern(spec.ConsoleSinkLoggerPattern);
		logSinks[1]->set_pattern(spec.FileSinkLoggerPattern);

		EngineLogger =
		    std::make_shared<spdlog::logger>(spec.EngineLoggerName, std::begin(logSinks), std::end(logSinks));
		EngineLogger->set_level(spdlog::level::trace);
		EngineLogger->flush_on(spdlog::level::trace);
		spdlog::register_logger(EngineLogger);

		RuntimeLogger =
		    std::make_shared<spdlog::logger>(spec.RuntimeLoggerName, std::begin(logSinks), std::end(logSinks));
		RuntimeLogger->set_level(spdlog::level::trace);
		RuntimeLogger->flush_on(spdlog::level::trace);
		spdlog::register_logger(RuntimeLogger);
	}

	void LogSystem::Shutdown()
	{
		EngineLogger.reset();
		RuntimeLogger.reset();

		spdlog::drop_all();
	}

} // namespace SW::Logger
