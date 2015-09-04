#include "Logging.h"
#include <ctime>
#include "CMDColor.h"
#include <SDL.h>

#pragma warning( disable : 4996)

Logging::Logging() : m_enableLogging(true)
{
	ColorCMD::ConsoleColorInit();
	SetConsoleTitle("Banana Light Log");

	m_levelInfo[LogLevel::FATAL_ERROR]		= TagLevelInfo("FATAL_ERR  ", true);
	m_levelInfo[LogLevel::NON_FATAL_ERROR]	= TagLevelInfo("NON_FAT_ERR", true);
	m_levelInfo[LogLevel::WARNING]			= TagLevelInfo("WARNING    ", true);
	m_levelInfo[LogLevel::SUCCESS]			= TagLevelInfo("SUCCESS    ", true);
	m_levelInfo[LogLevel::DEBUG_PRINT]		= TagLevelInfo("DEBUG_PRINT", true);
	m_levelInfo[LogLevel::INIT_PRINT]		= TagLevelInfo("INIT_PRINT ", true);
	m_levelInfo[LogLevel::START_PRINT]		= TagLevelInfo("START_PRINT", true);
	m_levelInfo[LogLevel::PINK_PRINT]		= TagLevelInfo("PINK_PRINT ", true);
	m_levelInfo[LogLevel::MASS_DATA_PRINT]	= TagLevelInfo("DATA_PRINT ", false);
	m_levelInfo[LogLevel::NOLEVEL]			= TagLevelInfo("NOLEVEL    ", true);
	m_levelInfo[LogLevel::HELP_PRINT]		= TagLevelInfo("HELP_PRINT ", true);
}

Logging::~Logging()
{
#ifdef _DEBUG
	CloseLogStream();
#endif // _DEBUG
}

bool Logging::OpenLogStream(std::string p_path)
{
#ifdef _DEBUG
	// current date/time based on current system
	time_t currentTime = time(0);

	tm gmtm;
	// convert now to tm struct for UTC
	gmtime_s(&gmtm, &currentTime);
	//Generate file name from date and time
	std::string fileName = std::to_string(gmtm.tm_year+1900) + GetTimeString(gmtm.tm_mon+1) + std::to_string(gmtm.tm_mday) + "_" + GetTimeString(gmtm.tm_hour+1) + "-" + GetTimeString(gmtm.tm_min) + "-" + GetTimeString(gmtm.tm_sec);
	std::string logName = p_path + fileName + ".txt";
	//Open log file stream
	fopen_s(&m_logFile, logName.c_str(), "w");
#endif // _DEBUG
	return true;

}

bool Logging::CloseLogStream()
{
	if(m_logFile)
	{   //Close stream
		fclose(m_logFile);
		return true;
	}
	return false;
}

std::string Logging::GetTimeString( int p_time )
{
	if(p_time < 10)
		return "0" + std::to_string(p_time);
	else
		return std::to_string(p_time);
}

//////////////////////////////////////////////////////////////////////////
//FILE AND CONSOLE LOGGING
//////////////////////////////////////////////////////////////////////////
void Logging::LT( std::string p_func, int p_line, LogLevel::LogLevel p_vLevel, const char* p_format, ... )
{
	va_list args;
	va_start (args, p_format);
	if(CheckLevel(p_vLevel))
	{
		WriteToConsole(p_func, p_line, p_vLevel, p_format, args);
		WriteToFile(p_func, p_line, p_vLevel, p_format, args);
	}
/*
	if(p_vLevel == LogLevel::FATAL_ERROR)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, 
			"FATAL ERROR!",
			StringFormat(p_format, args).c_str(),
			NULL);
	}
	*/
	va_end (args);
}


//////////////////////////////////////////////////////////////////////////
//Final write functions
//////////////////////////////////////////////////////////////////////////
void Logging::WriteToFile(std::string p_func, int p_line, LogLevel::LogLevel p_vLevel,std::string p_format, va_list p_args )
{
#ifdef _DEBUG
	std::string output = GetTimeFormatString() + "    " + m_levelInfo[p_vLevel].Name +  "    " + p_format + "    [" + p_func + ", Line: " + std::to_string(p_line) + "]" + "\n";

	vfprintf (m_logFile, output.c_str(), p_args);
	fflush(m_logFile);
#endif // _DEBUG
}

void Logging::WriteToConsole(std::string p_func, int p_line, LogLevel::LogLevel p_vLevel, std::string p_format, va_list p_args, bool writeFileLine )
{
	if(!m_enableLogging)
		return;

	std::string output;
	if (!writeFileLine)
		output = GetTimeFormatString() + "    "  + m_levelInfo[p_vLevel].Name +  "    " + p_format  +"\n";
	else
		output = GetTimeFormatString() + "    "  + m_levelInfo[p_vLevel].Name +  "    " + p_format + "    [" + p_func + ", Line: " + std::to_string(p_line) + "]" + "\n";

	switch (p_vLevel)
	{
	case LogLevel::FATAL_ERROR:
		{
			ColorCMD::SetColor(ColorCMD::ConsoleColor::RED, ColorCMD::defbackcol);
			std::cout<<"";
			break;
		}
	case LogLevel::NON_FATAL_ERROR:
		{
			ColorCMD::SetColor(ColorCMD::ConsoleColor::RED, ColorCMD::defbackcol);
			std::cout<<"";
			break;
		}
	case LogLevel::WARNING:
		{
			ColorCMD::SetColor(ColorCMD::ConsoleColor::YELLOW, ColorCMD::defbackcol);
			std::cout<<"";
			break;
		}
	case LogLevel::SUCCESS:
		{
			ColorCMD::SetColor(ColorCMD::ConsoleColor::GREEN, ColorCMD::defbackcol);
			std::cout<<"";
			break;
		}
	case LogLevel::DEBUG_PRINT:
		{
			ColorCMD::SetColor(ColorCMD::ConsoleColor::WHITE, ColorCMD::defbackcol);
			std::cout<<"";
			break;
		}
	case LogLevel::INIT_PRINT:
		{
			ColorCMD::SetColor(ColorCMD::ConsoleColor::BLUE, ColorCMD::defbackcol);
			std::cout<<"";
			break;
		}
	case LogLevel::START_PRINT:
		{
			ColorCMD::SetColor(ColorCMD::ConsoleColor::AQUA, ColorCMD::defbackcol);
			std::cout<<"";
			break;
		}
	case LogLevel::PINK_PRINT:
		{
			ColorCMD::SetColor(ColorCMD::ConsoleColor::PINK, ColorCMD::defbackcol);
			std::cout<<"";
			break;
		}
	case LogLevel::MASS_DATA_PRINT:
		{
			ColorCMD::SetColor(ColorCMD::ConsoleColor::WHITE, ColorCMD::defbackcol);
			std::cout<<"";
			break;
		}
	case LogLevel::NOLEVEL:
		{
			ColorCMD::SetColor(ColorCMD::ConsoleColor::GRAY, ColorCMD::defbackcol);
			std::cout<<"";
			break;
		}
	case LogLevel::HELP_PRINT:
		{
			ColorCMD::SetColor(ColorCMD::ConsoleColor::DARK_GREEN, ColorCMD::defbackcol);
			std::cout<<"";
			break;
		}
	default:
		break;
	}
	
	
	vprintf(output.c_str(), p_args);
	//std::cout<<"     ";
	//ColorCMD::SetColor(ColorCMD::ConsoleColor::GRAY, ColorCMD::BLACK);
	//std::cout << "[" + p_func + ", Line: " + std::to_string(p_line) + "]" + "\n";

	ColorCMD::SetColor(ColorCMD::ConsoleColor::WHITE, ColorCMD::defbackcol);
	std::cout<<"";

}

//////////////////////////////////////////////////////////////////////////
//Returns a correctly formatted time string
//////////////////////////////////////////////////////////////////////////
std::string Logging::GetTimeFormatString()
{
	time_t currentTime = time(0);

	tm gmtm;

	gmtime_s(&gmtm, &currentTime);

	return GetTimeString(gmtm.tm_hour+1) + ":" + GetTimeString(gmtm.tm_min) + ":" + GetTimeString(gmtm.tm_sec);
}

//////////////////////////////////////////////////////////////////////////
//Checks for filters
//////////////////////////////////////////////////////////////////////////
bool Logging::CheckLevel( LogLevel::LogLevel p_level )
{
	return m_levelInfo[p_level].Enabled;
}

//////////////////////////////////////////////////////////////////////////
//Help methods
//////////////////////////////////////////////////////////////////////////
std::string Logging::GetNameFromPath( std::string p_path )
{
	std::string cutPath;
	std::string::size_type slashIndex, dotIndex;

	// Extract the file name
	cutPath		= p_path;
	slashIndex	= cutPath.find_last_of("/")+1;
	if(slashIndex == 0)
		slashIndex	= cutPath.find_last_of("\\")+1;
	cutPath		= cutPath.substr(slashIndex, cutPath.size());
	dotIndex	= cutPath.find_last_of(".");
	cutPath		= cutPath.substr(0, dotIndex);
	return cutPath;
}

//////////////////////////////////////////////////////////////////////////
//Parse console commands
//////////////////////////////////////////////////////////////////////////
void Logging::ParseCommand()
{
	char module[32];
	char value[32];

	LogText(LogLevel::HELP_PRINT, "Enter command:");

	std::cin.get(module, 32, ' ');
	std::cin.get(module, 32, ' ');

	if(std::string(module) ==  " help")
	{
		LogText(LogLevel::HELP_PRINT, "[LOGGING COMMANDS]");
		LogText(LogLevel::HELP_PRINT, "-l clear - Clear log");
		LogText(LogLevel::HELP_PRINT, "-l [on/off] - Turn logging on or off");
		LogText(LogLevel::HELP_PRINT, "-l status - Print status of log prints and levels");
		LogText(LogLevel::HELP_PRINT, "-l [LogLevel] [0/1] - Activate or deactivate a specific log level");
	}
	else if(std::string(module) == " clear")
	{
		LogText(LogLevel::NOLEVEL, "Cleared console log!");
		ClearLog();
	}
	else if(std::string(module) == " on")
	{
		m_enableLogging = true;
		LogText(LogLevel::NOLEVEL, "Enabled console logging");
	}
	else if(std::string(module) == " off")
	{
		LogText(LogLevel::NOLEVEL, "Disabled console logging");
		m_enableLogging = false;
	}
	else if(std::string(module) == " status" || std::string(module) == " s")
	{
		PrintStatus();
	}
	else if(std::string(module) == " NOLEVEL" || std::string(module) == " NOTAG" || std::string(module) == " HELP_PRINT")
	{
	}
	else
	{
		for(auto itr = m_levelInfo.begin(); itr != m_levelInfo.end(); itr++)
		{
			std::string checkName = itr->second.Name;
			std::size_t found = checkName.find_last_not_of(" ");

			if (found != std::string::npos)
				checkName.erase(found+1);
			else
				checkName.clear(); // checkName is all whitespace

			if(std::string(module) == checkName)
			{	
				std::cin.get(value, 32, ' ');
				itr->second.Enabled = (atoi(value) == 1);
				LogText(LogLevel::NOLEVEL, "Set %s to %s", checkName, value);
				return;
			}
		}
	}
}

void Logging::PrintStatus()
{
	LogText(LogLevel::NOLEVEL, "LogLevel status");
	for(auto itr = m_levelInfo.begin(); itr != m_levelInfo.end(); itr++)
	{
		LogText(LogLevel::NOLEVEL, "\t\t[%s] : %d", itr->second.Name.c_str(), (int)itr->second.Enabled);
	}
}

void Logging::ClearLog()
{
	system("cls");
}

//StackOverflow magic
std::string Logging::StringFormat(const std::string p_fmt, va_list p_args) 
{
	int final_n, n = ((int)p_fmt.size()) * 2; /* reserve 2 times as much as the length of the fmt_str */
	std::string str;
	std::unique_ptr<char[]> formatted;
	while(1) {
		formatted.reset(new char[n]); /* wrap the plain char array into the unique_ptr */
		strcpy(&formatted[0], p_fmt.c_str());
		final_n = vsnprintf(&formatted[0], n, p_fmt.c_str(), p_args);
		if (final_n < 0 || final_n >= n)
			n += abs(final_n - n + 1);
		else
			break;
	}
	return std::string(formatted.get());
}