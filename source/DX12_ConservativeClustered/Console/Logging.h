#pragma once

#include <string>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdarg.h>
#include <map>
#include <memory>

namespace LogLevel
{
	enum LogLevel
	{
		FATAL_ERROR,
		NON_FATAL_ERROR,
		WARNING,
		SUCCESS,
		DEBUG_PRINT,
		INIT_PRINT,
		START_PRINT,
		PINK_PRINT,
		MASS_DATA_PRINT,
		NOLEVEL,
		HELP_PRINT,
	};
}


class Logging
{
	public:
		Logging();
		~Logging();

		void LT(std::string p_func, int p_line, LogLevel::LogLevel p_vLevel, const char* p_format, ...);
		
		void LogText(LogLevel::LogLevel p_vLevel, const char* p_format, ...){};

		void ParseCommand();

		bool OpenLogStream(std::string p_path);
	private:

		struct TagLevelInfo
		{
			TagLevelInfo(std::string p_name, bool p_enabled) : Name(p_name), Enabled(p_enabled){}
			TagLevelInfo(){}
			std::string Name;
			bool		Enabled;
		};

		bool m_enableLogging;

		bool CloseLogStream();

		FILE* m_logFile;

		std::string GetTimeString(int p_time);
		std::string GetTimeFormatString();

		void WriteToFile(std::string p_func, int p_line, LogLevel::LogLevel p_vLevel, std::string p_format, va_list p_args);
		void WriteToConsole(std::string p_func, int p_line, LogLevel::LogLevel p_vLevel, std::string p_format, va_list p_args, bool writeFileLine = false);
		
		bool CheckLevel(LogLevel::LogLevel p_level);

		std::string GetNameFromPath( std::string p_path );

		std::map<LogLevel::LogLevel, TagLevelInfo> m_levelInfo;

		void PrintStatus();

		void ClearLog();

		std::string StringFormat(const std::string p_fmt, va_list p_args) ;

};
//(LogLevel::LogLevel p_vLevel, const char* p_format, ...)
#define LogText(...) LT(__FUNCTION__, __LINE__, ##__VA_ARGS__)

