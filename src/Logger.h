// Just a test comment

#ifndef LOGGER_H_
#define LOGGER_H_

class SERVICE {
public:
	enum {GENERAL=0, POWER, SENSORS, MOTORS, PNEUMATICS};
	static const char* text[];
};

class LEVEL {
public:
	enum {EMER=0, ALERT, CRIT, ERR, WARN, NOTICE, INFO, DEBUG};
	static const char* text[];
};

class SUBSYSTEM {
public:
	enum {TEST = 0};
	static const char* text[];
};

class COMPONENT {
public:
	enum {TEST = 0};
	static const char* text[];
};

class Logger {
public:
	static void LogState(int SERVICE, int LEVEL, const char * text);
	static void LogValue(int SUBSYSTEM, int COMPONENT, double value);
};

#endif /* LOGGER_H_ */
