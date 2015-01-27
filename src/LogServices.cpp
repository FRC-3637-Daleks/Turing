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
		return;
	}
	while(ls->getThreadState() == LogService::THREAD_STATE_INIT);
	int failed = 0;
	clock_t c = 0;
	while(ls->getThreadState() == THREAD_STATE_RUNNING && failed >= 0)
	{
		if(clock() - c > ls->getLogPeriod()*CLOCKS_PER_SEC/1000)
		{
			failed = ls->LogAll();
			c = clock();
		}
	}
    ls->LogAllCurrent();
    if(ls->getThreadState() == THREAD_STATE_RUNNING)
    	; // In which the logger quit due to a callback function returning KILL
}

LogService::~LogService()  // Deallocates memory
{
	joinThread();
	for(auto i = logObjects.begin(); i != logObjects.end(); i++)
    {
		delete *i;
    }
    logObjects.clear();
}

const int LogService::LogAll()
{
	int ret = 0;
    // Iterators become invalidated across threads
	for(unsigned int i = 0; i < logObjects.size() && ret >= 0; i++)
		ret |= logObjects[i]->Log();

	frames++;
	return ret;
}

const int LogService::LogAllCurrent()
{
    int ret = 0;
    for(unsigned int i = 0; i < logObjects.size() && ret >= 0; i++)
        ret |= logObjects[i]->logCurrent();
    
    return ret;
}

FileLogger::FileLogger(const string &file, const string &command): LogService(), writer(0)
{
	createLogDir(command);
	stateOut.open(file, std::ios_base::out);
    runThread();
}

FileLogger::~FileLogger()
{
    logText()<<"[FILELOGGER][INFO] Closing file output streams"<<endl;
    logText()<<"[FILELOGGER][INFO] Deleting LogService"<<endl;
    joinThread();
	for(auto i = outStreams.begin(); i != outStreams.end(); i++)
	{
		delete *i;
	}
	outStreams.clear();
    logCurrent();
}

ofstream& FileLogger::makeLogStream(const string &file)
{
	outStreams.push_back(new ofstream(file, std::ios_base::out));
	return *outStreams.back();
}

void FileLogger::createLogDir(const string &command)
{
	if(system(command.c_str()) >= 0)
		logText()<<"[FILELOGGER][INFO] `"<<command<<"` Directory made successfully"<<endl;
}

void FileLogger::logText(const string &text)
{
	logText()<<text<<endl;
}

ostream& FileLogger::logText()
{
    if(doubleBuffer[writer].eof())
        doubleBuffer[writer].clear();
	doubleBuffer[writer].fill('0');
	doubleBuffer[writer].width(FRAME_TEXT_WIDTH);
	return doubleBuffer[writer]<<currentFrameTime()<<": ";
}

const int FileLogger::logCurrent()
{
    const short prev = writer;
	if(writer == 0) writer = 1;
    else if(writer == 1) writer = 0;
    if(doubleBuffer[prev].rdbuf()->in_avail() > 0)
    {
        doubleBuffer[prev]>>stateOut.rdbuf();
    }
	stateOut.flush();
	return 0;
}

const int FileLogger::LogAll()
{
    if(currentFrame() % getFramesUntilWrite() == 0)
        logCurrent();
	return LogService::LogAll();
}
