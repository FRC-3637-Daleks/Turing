// Just a test comment

#ifndef LOGGER_H_
#define LOGGER_H_

#include <stdlib.h>
#include "LogServices.h"
#include <cctype>
#include <time.h>
#include <algorithm>
#include <cstdlib>
#include <memory>

using std::shared_ptr;

/*
class SERVICE_t {
public:
	enum {GENERAL=0, POWER, SENSORS, MOTORS, PNEUMATICS};
	static const char* text[];
};
*/

class LEVEL_t {
public:
	enum {EMER=0, ALERT, CRIT, ERR, WARN, NOTICE, INFO, DEBUG};
	static const char* text[];
};

class SUBSYSTEM {
public:
	enum {TEST = 0};
	static const char* text[];
};

class Logger {
private:
	static shared_ptr<LogService> service;
	static std::function<LogService * ()> factory;
	static string path;
	static unsigned int frame;

public:
//	static const int LogFrame();

public:
	static const string GetLogPath();
	static const string GetTimeDirectory();
	static const string GetMakeDirCommand();
	static const string GetFullPath();
	static const string MakeLogFileName(const string SUBS, const string COMP, const string TYPE);

private:
	static LogService &GetInstance();

public:
	static const int MakeLogValue(const char * const SUBS, const char * const COMP, std::function<double(void)> fn) {
		GetInstance().addNumericLog(fn, MakeLogFileName(string(SUBS), string(COMP), string("double")));
		return 0;
	}

	template<class SUB_TYPE>
	static const int MakeLogValue(const char * const SUBS, const char * const COMP, SUB_TYPE *obj, double (SUB_TYPE::*fn)()) {
		GetInstance().addNumericLog(obj, fn, MakeLogFileName(string(SUBS), string(COMP), string("double")));
		return 0;
	}

	static const int MakeLogBoolean(const char * const SUBS, const char * const COMP, std::function<bool(void)> fn) {
		GetInstance().addBooleanLog(fn, MakeLogFileName(string(SUBS), string(COMP), string("bool")));
		return 0;
	}

	template<class SUB_TYPE>
	static const int MakeLogBoolean(const char * const SUBS, const char * const COMP, SUB_TYPE *obj, double (SUB_TYPE::*fn)()) {
		GetInstance().addBooleanLog(obj, fn, MakeLogFileName(string(SUBS), string(COMP), string("bool")));
		return 0;
	}

public:
	/// This should be called before any other function of Logger is called.
	static void SetFactoryFunction(std::function<LogService * ()> fact) {factory = fact;};

public:
	static const int LogState(const char * const SERV, const int LEV, const char * const text);

	static const int MakeLogValue(const int SUBS, const char * const COMP, std::function<double(void)> fn)
	{
		return MakeLogValue(SUBSYSTEM::text[SUBS], COMP, fn);
	}

	template<class SUB_TYPE>
	static const int MakeLogValue(const int SUBS, const char * const COMP, SUB_TYPE *obj, double (SUB_TYPE::* fn)())
	{
		return MakeLogValue<SUB_TYPE>(SUBSYSTEM::text[SUBS], COMP, obj, fn);
	}

	template<class SUB_TYPE>
	static const int MakeLogValue(const char * const COMP, SUB_TYPE *obj, double (SUB_TYPE::* fn)())
	{
		string str(typeid(SUB_TYPE).name());
		for(auto i = str.begin(); i != str.end(); i++) *i = toupper(*i);   // Makes name upper case
		return MakeLogValue(str.c_str(), COMP, obj, fn);
	}

	/// The subsystem used must have a static function GetComponenentName which returns a c-string based on a id
	template<class SUB_TYPE>
	static const int MakeLogValue(const int COMP, SUB_TYPE *obj, double (SUB_TYPE::* fn)())
	{
		return MakeLogValue(SUB_TYPE::GetComponentName(COMP), obj, fn);
	}

	static const int MakeLogBoolean(const int SUBS, const char * const COMP, std::function<double(void)> fn)
	{
		return MakeLogBoolean(SUBSYSTEM::text[SUBS], COMP, fn);
	}

	template<class SUB_TYPE>
	static const int MakeLogBoolean(const int SUBS, const char * const COMP, SUB_TYPE *obj, double (SUB_TYPE::*fn)())
	{
		return MakeLogBoolean<SUB_TYPE>(SUBSYSTEM::text[SUBS], COMP, obj, fn);
	}

	template<class SUB_TYPE>
	static const int MakeLogBoolean(const char * const COMP, SUB_TYPE *obj, double (SUB_TYPE::* fn)())
	{
		string str(typeid(SUB_TYPE).name());
		for(auto i = str.begin(); i != str.end(); i++) *i = toupper(*i);   // Makes name upper case
		return MakeLogBoolean(str, COMP, obj, fn);
	}

	template<class SUB_TYPE>
	static const int MakeLogBoolean(const int COMP, SUB_TYPE *obj, double (SUB_TYPE::*fn)())
	{
		return MakeLogBoolean(SUB_TYPE::GetComponenentName(COMP), obj, fn);
	}

};

#endif /* LOGGER_H_ */
