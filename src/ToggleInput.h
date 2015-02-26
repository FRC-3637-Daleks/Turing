/*
 * ToggleInput.h
 *
 *  Created on: Feb 26, 2015
 *      Author: Edward
 */

#ifndef SRC_TOGGLEINPUT_H_
#define SRC_TOGGLEINPUT_H_

#include <functional>
#include "WPILib.h"

class ToggleInput
{
private:
	bool state;
	bool previous;
	std::function<bool()> callback;

public:
	explicit ToggleInput(const std::function<bool()> &fn): state(false), previous(false), callback(fn) {};
	ToggleInput(ToggleInput&& other): state(other.state), previous(other.previous), callback(other.callback) {other.callback = nullptr;};
	ToggleInput(const ToggleInput&)=delete;
	virtual ~ToggleInput() {};

public:
	void Update();
	const bool Pressed() {return callback();};
	void SetState(const bool s) {state = s;};
	const bool GetState() const {return state;};
	const bool GetUpdate() {Update(); return GetState();};
};


#endif /* SRC_TOGGLEINPUT_H_ */
