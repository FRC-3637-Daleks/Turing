#include "Logger.h"
#include "stdio.h"

/********* STATIC LOGGER DATA MEMBERS *************/

LogService *Logger::service = nullptr;
string Logger::path;
unsigned int Logger::frame = 0;

std::function<LogService *()> Logger::factory = []()
{
	return new FileLogger(Logger::GetFullPath()+"state.txt", Logger::GetMakeDirCommand());
};

/********** STATIC LOGGER CONSTANTS *****************/
/*
const char* SERVICE::text[] = {
		"GENERAL", "POWER", "SENSORS", "MOTORS", "PNEUMATICS"
};
*/

const char* LEVEL_t::text[] = {
		"EMER", "ALERT", "CRIT", "ERR", "WARN", "NOTICE", "INFO", "DEBUG"
};

const char* SUBSYSTEM::text[] = {
		"TEST"
};

const char* COMPONENT::text[] = {
		"TEST"
};

/*
const int Logger::LogFrame()
{
	return GetInstance().LogAll();
}
*/


/******** STATIC LOGGER GET FUNCTIONS **********/

const string Logger::GetLogPath()
{
	return string("/home/lvuser/log");
}

const string Logger::GetTimeDirectory()
{
	string date(ctime(NULL));
	std::for_each(date.begin(), date.end(), [](char &c) {if(c == ' ') c = '-';});
	return date.substr(4);
}

const string Logger::GetFullPath()
{
	if(path.empty())
		path = GetLogPath()+GetTimeDirectory()+"/";
	return path;
}

const string Logger::GetMakeDirCommand()
{
	return string("mkdir ")+(GetFullPath());
}

const string Logger::MakeLogFileName(const string SUBS, const string COMP, const string TYPE)
{
	return GetFullPath()+SUBS+'.'+COMP+'.'+TYPE;
}

LogService &Logger::GetInstance()
{
	if(service == nullptr)
		service = factory();
	return *service;
}

// Takes in Service, level, and state of a part of a robot and stores it in 'data'
const int Logger::LogState(const char * const SERV, const int LEV, const char * text) {
	GetInstance().logText()<<frame<<": ["<<SERV<<"]["<<LEVEL_t::text[LEV]<<"] "<<text<<std::endl;
#ifdef DEBUG
	if(LEV == LEVEL_t::DEBUG) std::cout<<"["<<SERV<<"] "<<text<<std::endl;
#endif
	return 0;
}
