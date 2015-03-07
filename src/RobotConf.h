/*
 * RobotConf.h
 *
 *  Created on: Mar 5, 2015
 *      Author: edward
 */

#ifndef SRC_ROBOTCONF_H_
#define SRC_ROBOTCONF_H_

#include <unordered_map>
#include <sstream>

using std::unordered_map;
using std::string;
using std::stringstream;


class RobotConf
{
private:
	unordered_map<string, string> config;
	string filename;

public:
	RobotConf(const string& file=string("robot.conf"));
	RobotConf(const RobotConf& other): config(other.config), filename(other.filename) {};
	virtual ~RobotConf() {Save();};

public:
	const bool HasValue(const string& key) const;

	const string& GetValue(const string &key, const string& defaultVal = string(""));
	template<typename T> const T GetValue(const string &key, const string& defaultVal = string(""))
	{
		stringstream ss(GetValue(key, defaultVal));
		T ret;
		ss>>ret;
		return ret;
	}

	void SetValue(const string& key, const string& value);	///< Sets or Adds the value to the current configuration
	template<typename T> void SetValue(const string& key, const T& value)
	{
		stringstream ss;
		ss<<value;
		SetValue(key, ss.str());
	}

public:
	const int Save(const string& file);
	const int Save() {return Save(filename);};
};



#endif /* SRC_ROBOTCONF_H_ */
