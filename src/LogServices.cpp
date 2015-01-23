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
	{
		std::cout<<"Logging thread passed with null LogService"<<std::endl;
		return;
	}
	int failed = 0;
	std::cout<<"Logging thread created successfully, entering infinite loop"<<std::endl;
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

	frames++;
	return ret;
}

FileLogger::FileLogger(const string &file, const string &command): LogService(file.c_str()), writer(true)
{
	createLogDir(command);
	stateOut.open(file, std::ios_base::out);
}

FileLogger::~FileLogger()
{
	logCurrent();
	logText()<<"[INFO] Closing file output streams"<<endl;
	for(auto i = outStreams.begin(); i != outStreams.end(); i++)
	{
		delete *i;
	}
	logText()<<"[INFO] Deleting LogService"<<endl;
}

ofstream& FileLogger::makeLogStream(const string &file)
{
	outStreams.push_back(new ofstream(file, std::ios_base::binary));
	return *outStreams.back();
}

void FileLogger::createLogDir(const string &command)
{
	if(system(command.c_str()) >= 0)
		logText()<<"[INFO] `"<<command<<"` Directory made successfully"<<endl;
}

void FileLogger::logText(const string &text)
{
	doubleBuffer[writer].fill('0');
	doubleBuffer[writer].width(FRAME_TEXT_WIDTH);
	doubleBuffer[writer]<<currentFrame()<<": "<<text<<endl;
}

ostream& FileLogger::logText()
{
	doubleBuffer[writer].fill('0');
	doubleBuffer[writer].width(FRAME_TEXT_WIDTH);
	return doubleBuffer[writer]<<currentFrame()<<": ";
}

const int FileLogger::logCurrent()
{
	writer = !writer;
	doubleBuffer[!writer].get(*stateOut.rdbuf());
	stateOut.flush();
	return 0;
}

const int FileLogger::LogAll()
{
	logCurrent();
	return LogService::LogAll();
}
