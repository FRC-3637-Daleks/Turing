/*
 * SmartDashService.h
 *
 *  Created on: Jan 26, 2015
 *      Author: edward
 */

#ifndef SRC_SMARTDASHSERVICE_H_
#define SRC_SMARTDASHSERVICE_H_

#include "WPILib.h"

#include "LogServices.h"

#include "Watch_Logs.h"

class SmartDashService: public DataService
{
private:
	static SmartDashService *service;

public:
	static SmartDashService& GetInstance();

public:
	SmartDashService(): DataService(true, SMARTDASH_PUSHING_PERIOD)
	{
		SmartDashboard::init();
	};

public:
	template<typename DATA_TYPE, class SUBSYSTEM_CLASS>
	void addLog(SUBSYSTEM_CLASS * const obj, DATA_TYPE (SUBSYSTEM_CLASS::*func)(), const string &name)
	{
		addLog(std::bind(func, obj), name);
	}

	template<typename DATA_TYPE>
	void addLog(std::function<DATA_TYPE(void)> func, const string &key)
	{
		appendLog(new ValueLog<DATA_TYPE>(func, AddSmartDashExtension<DATA_TYPE>(key)));
	}
};

#endif /* SRC_SMARTDASHSERVICE_H_ */
