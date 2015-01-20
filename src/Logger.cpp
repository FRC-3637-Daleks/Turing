#include "Logger.h"
#include "stdio.h"


const char* SERVICE::text[] = {
		"GENERAL", "POWER", "SENSORS", "MOTORS", "PNEUMATICS"
};

const char* LEVEL::text[] = {
		"EMER", "ALERT", "CRIT", "ERR", "WARN", "NOTICE", "INFO", "DEBUG"
};

const char* SUBSYSTEM::text[] = {
		"TEST"
};

const char* COMPONENT::text[] = {
		"TEST"
};

// Takes in Service, level, and state of a part of a robot and stores it in 'data'
void Logger::LogState(int SERVICE, int LEVEL, const char * text) {
	char data[1024];
	int code = (SERVICE << 3) + LEVEL;
	snprintf(data, 1024, "<%d>Service:%s Level:%s %s", code, SERVICE::text[SERVICE], LEVEL::text[LEVEL], text);
}

// Takes in Subsystem, component, and value of a part of a robot and stores it in 'data'
void Logger::LogValue(int SUBSYSTEM, int COMPONENT, double value) {
	char data[1024];
	int code = (SUBSYSTEM << 3) + COMPONENT;
	snprintf(data, 1024, "<%d>Subsystem:%s Component:%s %f", code, SUBSYSTEM::text[SUBSYSTEM], COMPONENT::text[COMPONENT], value);
}
