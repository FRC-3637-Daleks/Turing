#ifndef _CONFIG_H_
#define _CONFIG_H_

#define LOG_STATE_OUT_FILE_NAME "states.txt"
#define LOG_HOME "/home/lvuser/log/"

#define DEFAULT_BUFFER_FRAMES 20

#define FRAME_TEXT_WIDTH 7
#define FLOATING_POINT_SIG_FIGS 6

#define DEBUG_MODE


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


#endif /*_CONFIG_H_*/
