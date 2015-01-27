/*
 * SmartDashService.cpp
 *
 *  Created on: Jan 26, 2015
 *      Author: edward
 */

#include "SmartDashService.h"

SmartDashService * SmartDashService::service = nullptr;

SmartDashService &SmartDashService::GetInstance()
{
	if(service == nullptr)
		service = new SmartDashService;
	return *service;
}
