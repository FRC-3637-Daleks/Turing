/*
 * Loggable.cpp
 *
 *  Created on: Jan 17, 2015
 *      Author: edward
 */

#include "Loggable.h"

const int NumericLog::Log()
{
	buf.push_back(fn());
	if(buf.size() == flushFrames)
		return logCurrent();
	return 0;
}

const int NumericLog::logCurrent()
{
    if(buf.size() == 0)
        return 0;
	for(auto i = buf.begin(); i != buf.end(); i++) out<<*i<<std::endl;
	//out.write((char *)buf.data(), sizeof(double)*buf.size());
	buf.clear();
	out.flush();  // Ensures output stream is writing to file
	return 0;
}

const int BooleanLog::Log()
{
	buf.push_back(fn());
	if(buf.size() == flushFrames)
		return logCurrent();
	return 0;
}

const int BooleanLog::logCurrent()
{
    if(buf.size() == 0)
        return 0;
	for(auto i = buf.begin(); i != buf.end(); i++) out<<int(*i)<<std::endl;
//	out.write(buf.data(), buf.size());
	buf.clear();
	out.flush();
	return 0;
}
