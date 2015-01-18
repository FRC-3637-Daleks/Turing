/*
 * LogServices.cpp
 *
 *  Created on: Jan 18, 2015
 *      Author: edward
 */

#include "LogServices.h"


LogService::~LogService()  // Deallocates memory
{
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
