/*
 * LogServices.cpp
 *
 *  Created on: Jan 18, 2015
 *      Author: edward
 */

#include "LogServices.h"

void LogService::LoggingThread(LogService * const ls)
{
	if(ls == nullptr)
		return;
	int failed = 0;
	while(ls->isRunning() && failed >= 0) failed = ls->LogAll();
}

LogService::~LogService()  // Deallocates memory
{
	running = false;
	if(logThread.joinable()) logThread.join();
	for(auto i = logObjects.begin(); i != logObjects.end(); i++)
		delete *i;
}

const int LogService::LogAll()
{
	int ret = 0;
	for(auto i = logObjects.begin(); i != logObjects.end(); i++)
		ret |= (*i)->Log();

	return ret;
}

FileLogger::FileLogger(const string &file, const string &command): LogService(file.c_str()), writer(true)
{
	createLogDir(command);
	stateOut.open(file, std::ios_base::out);
}

FileLogger::~FileLogger()
{
	for(auto i = outStreams.begin(); i != outStreams.end(); i++)
		delete *i;
}

ofstream& FileLogger::makeLogStream(const string &file)
{
	outStreams.push_back(new ofstream(file, std::ios_base::binary));
	return *outStreams.back();
}

void FileLogger::createLogDir(const string &command)
{
	system(command.c_str());
}

void FileLogger::logText(const string &text)
{
	doubleBuffer[writer]<<text<<endl;
}

ostream& FileLogger::logText()
{
	return doubleBuffer[writer];
}

const int FileLogger::logCurrent()
{
	writer = !writer;
	doubleBuffer[!writer].get(*stateOut.rdbuf());
	stateOut.flush();
	return 0;
}



