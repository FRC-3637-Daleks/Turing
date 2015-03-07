/*
 * driverSettings.cpp
 *
 *  Created on: Mar 4, 2015
 *      Author: elija_000
 */
// basic file operations
#include <iostream>
#include <fstream>
using namespace std;

int confFile () {
  ofstream myfile;
  myfile.open ("driver.conf"); //open file called driver.conf and set original values
  myfile << "maxFowardVelocity = 1 /n" //sets the max foward speed to 1
		 << "maxStrafeVelocity = 1 /n" //sets max strafe speed to 1
		 << "maxTurnVelocity = 1 /n" //sets max turn speed to 1
		 << "deadzone = 0 /n" //sets area that the stick will not do anything to 0
		 << "leftJoystickSQR = 2 /n" //sets the left precision mode speed squared the regular input
		 << "rightJoystickSQR = 2 /n"//sets the right precision mode to squared the regular input
		  ;
  myfile.close();
  return 0;
}



