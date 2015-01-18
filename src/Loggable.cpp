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
	out.flush();  // Ensures output stream is writing to file
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

	if(buf.size() == flushBytes && curPos == 7)
		return logCurrent();
	return 0;
}

const int BooleanLog::logCurrent()
{
	char tmp;
	if(curPos != 7)
	{
		tmp = buf.back();  // Ensures 0s are not written to frames which did not occur
		buf.pop_back();
	}
	out.write(buf.data(), buf.size());
	curPos = 0;
	buf.clear();
	out.flush();
	if(curPos != 7)
		buf.push_back(tmp);  // Pushes back known frames
	return 0;
}
