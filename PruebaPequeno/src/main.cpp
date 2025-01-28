/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       HP-Laptop                                                 */
/*    Created:      12/2/2024, 3:36:52 PM                                     */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "vex.h"

using namespace vex;

// A global instance of vex::brain used for printing to the V5 brain screen
brain Brain;
controller Controller1;

// define your global instances of motors and other devices here


motor MotorL1(PORT20, true);
motor MotorL2(PORT19, true);
motor MotorR1(PORT1, false);
motor MotorR2(PORT2, false);    

int main() {
   
    while(true) {
        int speedL= Controller1.Axis3.position(percent); 
        int speedR = Controller1.Axis2. position(percent);  
        MotorL1.spin(directionType::fwd, speedL, percent);
        MotorL2.spin(directionType::fwd, speedL, percent);  
        MotorR1.spin(directionType::fwd, speedR, percent);
        MotorR2.spin(directionType::fwd, speedR, percent); 
    }
}
