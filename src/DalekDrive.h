/*
 * DalekDrive.h
 *
 *  Created on: Feb 3, 2015
 *      Author: Edward
 */

#ifndef SRC_DALEKDRIVE_H_
#define SRC_DALEKDRIVE_H_

#include "WPILib.h"


// functions to be stored in array for quick reference
float leftFrontAlg(float x, float y, float theta);
float rightFrontAlg(float x, float y, float theta);
float leftRearAlg(float x, float y, float theta);
float rightRearAlg(float x, float y, float theta);

// class to replace RobotDrive
class DalekDrive
{
public:  // static declares/defines
	enum {LEFT_FRONT=0, RIGHT_FRONT, LEFT_REAR, RIGHT_REAR, N_MOTORS}; // Wheel location defines
	typedef enum {MECANUM_WHEELS, OMNI_WHEELS, TRACTION_WHEELS} Wheel_t;
	typedef enum {TANK_DRIVE, ARCADE_DRIVE} Drive_t;

	// Generic motor class which facilitates giving all wheels the same orders in different configurations
	class Motor
	{
	public:
		typedef float (*MECANUM_FUNCTION)(float, float, float); // function pointer type for converting x y and theta to an individual wheel speed
		static MECANUM_FUNCTION mecFuncs[N_MOTORS];
	private:
		CANTalon *motor;
		int location;
		bool flip;  // keeps track whether this motor is flipped
		int *refCount; // used
		int syncGroup;

	public:
		Motor(CANTalon &m, const int loc, const int sg=0, const bool f=false): motor(&m), location(loc), flip(f), refCount(NULL), syncGroup(sg) {};
		Motor(CANTalon * const m, const int loc, const int sg=0, const bool f=false): motor(m), location(loc), flip(f), refCount(NULL), syncGroup(sg) {};
		Motor(const short id, const int loc, const int sg=0, const bool f=false): motor(new CANTalon(id)), location(loc), flip(f), refCount(new int(1)), syncGroup(sg) {};
		Motor(const Motor& m): motor(m.motor), location(m.location), flip(m.flip), refCount(m.refCount), syncGroup(m.syncGroup) {refCount? ++*refCount:NULL;};
		Motor(): motor(NULL), location(N_MOTORS), flip(false), refCount(NULL), syncGroup(0) {}; // default constructor which sets motor to invalid state
		Motor& operator= (const Motor m);  // uses copy and swap method

	public:
		void SetFlip(const bool b) {flip = b;};
		const bool GetFlip() const {return flip;};

		const int GetLocation() const {return location;};
		CANTalon *GetCan() const {return motor;};

	public:
		CANTalon *operator-> () const {return motor;};
		void SetSpeed(const float speed) {if(motor) motor->Set(speed*(flip? -1:1));};
		void SetMecSpeed(const float x, const float y, const float theta)
		{
			//printf("DalekDrive::Motor::SetMecSpeed(%f, %f, %f)\nlocation=%d\nmecFuncs[location]=%p\n", x, y, theta, location, mecFuncs[location]);
			SetSpeed((mecFuncs[location])(-x, y, -theta));

		};

	public:
		~Motor();
	};

private:  // data members
	Motor motors[N_MOTORS];  //motor array
	Wheel_t wheels; // Defines whether it every supplies the Motors and X val
	int syncGroup;

private:
	void init(const Wheel_t wheelConfig, const Motor m[N_MOTORS], const int syncGroup);
	void UpdateSyncGroup();

public:
	DalekDrive(const Wheel_t wheelConfig, Motor const &leftFront, Motor const &rightFront, Motor const &leftRear, Motor const &rightRear, int syncGroup=1);
	DalekDrive(const Wheel_t wheelConfig, const short leftFront, const short rightFront, const short leftRear, const short rightRear, int syncGroup=1);
	DalekDrive(const Wheel_t wheelConfig, const Motor m[N_MOTORS], int syncGroup=1);

	const Wheel_t GetWheels() const {return wheels;};

	Motor& operator[] (const short index) {return motors[index];};
	const Motor& operator[] (const short index) const {return motors[index];};

public:
	void Drive(const float x, const float y, const float theta);
	void Drive(const float left, const float right);

public:
	void printMotors();
};



#endif /* SRC_DALEKDRIVE_H_ */
