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

class BoolInput
{
private:
	bool state;

public:
	BoolInput(): state(false) {};
	virtual ~BoolInput() {};

public:
	const bool GetState() const {return state;};
	void SetState(const bool s) {state = s;};
	virtual void Update()=0;
};

class FlickInput: public BoolInput
{
private:
	bool previous;
	std::function<bool()> callback;

public:
	explicit FlickInput(const std::function<bool()> &fn): previous(false), callback(fn) {};
	FlickInput(FlickInput&& other): previous(other.previous), callback(other.callback) {other.callback = nullptr;};
	FlickInput(const FlickInput& other)=delete;
	virtual ~FlickInput() {};

public:
	void Update() override;
	const bool Pressed() {return callback();};
	const bool GetUpdate() {Update(); return GetState();};
};

class ToggleInput: public BoolInput
{
private:
	bool previous;
	std::function<bool()> callback;

public:
	explicit ToggleInput(const std::function<bool()> &fn): previous(false), callback(fn) {};
	ToggleInput(ToggleInput&& other): previous(other.previous), callback(other.callback) {other.callback = nullptr;};
	ToggleInput(const ToggleInput&)=delete;
	virtual ~ToggleInput() {};

public:
	void Update() override;
	const bool Pressed() {return callback();};
	const bool GetUpdate() {Update(); return GetState();};
};


#endif /* SRC_TOGGLEINPUT_H_ */
