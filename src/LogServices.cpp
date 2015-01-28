/*
 * LogServices.cpp
 *
 *  Created on: Jan 18, 2015
 *      Author: edward
 */

#include "LogServices.h"

void DataService::LoggingThread(DataService * const ls)
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
		if(clock() - c >= (int)ls->getLogPeriod()*CLOCKS_PER_SEC/1000)
		{
			failed = ls->LogAll();
			c = clock();
		}
	}
    ls->LogAllCurrent();
    if(ls->getThreadState() == THREAD_STATE_RUNNING)
    	; // In which the logger quit due to a callback function returning KILL
}

DataService::~DataService()
{
	joinThread();
	for(auto i = logObjects.begin(); i != logObjects.end(); i++)
    {
		delete *i;
    }
    logObjects.clear();
}

const int DataService::LogAll()
{
	int ret = 0;
	for(unsigned int i = 0; i < logObjects.size() && ret >=0; i++)
		ret |= logObjects[i]->Log();

	return ret;
}

const int DataService::LogAllCurrent()
{
	int ret = 0;
	for(unsigned int i = 0; i < logObjects.size() && ret >= 0; i++)
		ret |= logObjects[i]->logCurrent();

	return ret;
}

LogService::LogService(const bool start, const unsigned int period, const bool hijackSTD, const unsigned int f):
				DataService(start, period), oldCout(std::cout.rdbuf()), oldCerr(std::cerr.rdbuf()), framesUntilWrite(f), frames(0)
{
	if(hijackSTD)
	{
		std::ios_base::sync_with_stdio(true);
		std::cout.rdbuf(coutRedirect.rdbuf());
		std::cerr.rdbuf(cerrRedirect.rdbuf());
	}
};

LogService::~LogService()  // Deallocates memory
{
}

const int LogService::LogAll()
{
	frames++;

	return DataService::LogAll();
}

const int LogService::logCurrent()
{
	if(coutRedirect.rdbuf()->in_avail() > 0)
	{
		oldCout<<coutRedirect.str();
		/*char buf[255];
		while(!coutRedirect.eof())
		{
			coutRedirect.getline(buf, 255);
			logText()<<"[STDOUT][INFO] "<<buf<<std::endl;
		}*/
		logText()<<"[STDOUT][INFO]"<<coutRedirect.rdbuf()<<std::endl;
		coutRedirect.clear();
	}

	if(coutRedirect.rdbuf()->in_avail() > 0)
	{
		oldCerr<<cerrRedirect.str();
		char buf[255];
		while(!cerrRedirect.eof())
		{
			coutRedirect.getline(buf, 255);
			logText()<<"[STDERR][ERR] "<<buf<<std::endl;
		}
		coutRedirect.clear();
	}

	return 0;
}

void LogService::logStdout(const string &text)
{
	logStdout()<<text<<endl;
}

ostream& LogService::logStdout()
{
	oldCout.width(FRAME_TEXT_WIDTH);
	oldCout.fill('0');
	return oldCout<<currentFrameTime()<<": ";
}

void LogService::logStderr(const string &text)
{
	logStderr()<<text<<endl;
}

ostream& LogService::logStderr()
{
	oldCerr.width(FRAME_TEXT_WIDTH);
	oldCerr.fill('0');
	return oldCerr<<currentFrameTime()<<": ";
}

FileLogger::FileLogger(const string &file, const string &command): LogService(false, FILE_LOGGING_PERIOD), writer(0)
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
	{
		logText()<<"[FILELOGGER][INFO] `"<<command<<"` Directory made successfully"<<endl;
	}
}

const unsigned int FileLogger::makeInfo(const string &file)
{
	ofstream info(file, std::ios_base::out);
	info<<"BUILT:"<<__DATE__<<' '<<__TIME__<<endl;
	return 0;
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
	LogService::logCurrent();
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
