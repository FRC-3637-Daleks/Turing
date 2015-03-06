/*
 * RobotConf.cpp
 *
 *  Created on: Mar 5, 2015
 *      Author: edward
 */

#include <fstream>
#include "RobotConf.h"

RobotConf::RobotConf(const string& file): filename(file)
{
	std::ifstream configFile(filename);
	if(configFile.fail())	///< Likely first time created file
		return;

	while(!configFile.eof())
	{
		char buf[255];
		configFile.getline(buf, 255);
		int separator = -1;
		int end;
		for(end = 0; end < 255 && buf[end] != '\0'; end++)
		{
			if(buf[end] == ':')
				separator = end;
		}

		if(separator < 0)
			continue;

		config[string(buf, separator)] = string(buf+separator, end);
	}
}

const bool RobotConf::HasValue(const string& key) const
{
	return config.count(key);
}

const string& RobotConf::GetValue(const string &key, const string& defaultVal) const
{
	if(!HasValue(key))
		config[key] = defaultVal;

	return config.at(key);
}

void RobotConf::SetValue(const string& key, const string& value)
{
	config[key] = value;
}

const int RobotConf::Save(const string &file)
{
	std::ofstream configFile(file);
	if(configFile.fail())
		return -1;

	for(auto i = config.begin(); i != config.end(); i++)
	{
		configFile<<i->first<<':'<<i->second<<'\n';
	}

	configFile.flush();
	return 0;
}





