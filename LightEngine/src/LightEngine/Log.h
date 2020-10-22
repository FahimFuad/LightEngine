#pragma once
#include <memory>
#include "Core.h"
#include "spdlog/spdlog.h"

namespace LightEngine
{
	class LIGHTENGINE_API Log
	{
	public:
		static void Init();
		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static std::shared_ptr < spdlog::logger> s_CoreLogger;
		static std::shared_ptr < spdlog::logger> s_ClientLogger;
	};
}

//Core Log Macros
#define LE_CORE_TRACE(...)    ::LightEngine::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define LE_CORE_DEBUG(...)    ::LightEngine::Log::GetCoreLogger()->debug(__VA_ARGS__)
#define LE_CORE_INFO(...)     ::LightEngine::Log::GetCoreLogger()->info(__VA_ARGS__)
#define LE_CORE_WARN(...)     ::LightEngine::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define LE_CORE_ERROR(...)    ::LightEngine::Log::GetCoreLogger()->error(__VA_ARGS__)
#define LE_CORE_CRITICAL(...) ::LightEngine::Log::GetCoreLogger()->critical(__VA_ARGS__)

//Client Log Macros
#define LE_TRACE(...)         ::LightEngine::Log::GetClientLogger()->trace(__VA_ARGS__)
#define LE_DEBUG(...)         ::LightEngine::Log::GetClientLogger()->debug(__VA_ARGS__)
#define LE_INFO(...)          ::LightEngine::Log::GetClientLogger()->info(__VA_ARGS__)
#define LE_WARN(...)          ::LightEngine::Log::GetClientLogger()->warn(__VA_ARGS__)
#define LE_ERROR(...)         ::LightEngine::Log::GetClientLogger()->error(__VA_ARGS__)
#define LE_CRITICAL(...)      ::LightEngine::Log::GetClientLogger()->critical(__VA_ARGS__)