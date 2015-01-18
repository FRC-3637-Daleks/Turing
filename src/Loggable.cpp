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
	if(buf.size() == flushValue)
		return logCurrent();
	return 0;
}

const int NumericLog::logCurrent()
{
	out.write((char *)buf.data(), sizeof(double)*buf.size());
	buf.clear();
	return 0;
}

const int BooleanLog::Log()
{
	if(curPos == 0 || curPos >= 8)
	{
		buf.push_back(0);
		curPos = 0;
	}

	buf.back() |= fn()<<(curPos++);

	if(buf.size() == flushBytes)
		return logCurrent();
	return 0;
}

const int BooleanLog::logCurrent()
{
	out.write(buf.data(), buf.size());
	buf.clear();
	return 0;
}
