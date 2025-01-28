/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       Melissa Ruiz                                              */
/*    Created:      11/19/2024, 12:31:02 PM                                   */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"

using namespace vex;

// A global instance of competition
competition Competition;

brain Brain;
motor MotorL1(PORT14, true); 
motor MotorL2(PORT18, false);
motor MotorL3(PORT19, false);
motor MotorL4(PORT20, true);
motor_group Izq(MotorL1, MotorL2, MotorL3, MotorL4); 

motor MotorR1(PORT7, false);
motor MotorR2(PORT8, true);
motor MotorR3(PORT9, true);
motor MotorR4(PORT10, false);
motor_group Der(MotorR1, MotorR2, MotorR3, MotorR4); 

motor Recolector(PORT1 , true);
motor Rampa(PORT12, true);
motor Garra(PORT15, true);

vex::pneumatics Pinza(Brain.ThreeWirePort.A);
vex::pneumatics RecolectorNeumatica(Brain.ThreeWirePort.B);

// define your global instances of motors and other devices here

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton(void) {

  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
}

double kP=0.5; 
double kI=0.001; 
double kD=0.05; 

double turnkP=0.5; 
double turnkI=0.001; 
double turnkD=0.05; 

//Configuración autónomo 
int desiredValue = 200; 
int desiredTurnValue=0; 

int error; //Sensor value - desire value : positional value -> 
int prevError = 0;  // Position 20 miliseconf ago
int derivative; //error - prevError : speed 
int total_error; //totalError = totalError + error; 

int turnError; //Sensor value - desire value : positional value -> 
int turnPrevError = 0;  // Position 20 miliseconf ago
int turnDerivative; //error - prevError : speed 
int turnTotal_error; //totalError = totalError + error; 

bool resetDriveSensor = false; 

bool enableDrivePID = true; 
int drivePID() {
    while (enableDrivePID) {
        int leftPosition = Izq.position(degrees);
        int rightPosition = Der.position(degrees);

        int averagePosition = (leftPosition + rightPosition) / 2;
        int error = averagePosition - desiredValue;

        int derivative = error - prevError;
        total_error += error;
        if (total_error > 1000) { total_error = 1000; }
        if (total_error < -1000) { total_error = -1000; }

        double lateralMotorPower = kP * error + kD * derivative + kI * total_error;

        int turnDifference = (leftPosition - rightPosition) / 2;
        int turnError = turnDifference - desiredTurnValue;
        int turnDerivative = turnError - turnPrevError;

        double turnMotorPower = turnkP * turnError + turnkD * turnDerivative;

        Izq.spin(forward, lateralMotorPower + turnMotorPower, percentUnits::pct);
        Der.spin(forward, lateralMotorPower - turnMotorPower, percentUnits::pct);

        prevError = error;
        turnPrevError = turnError;

        if (fabs(error) < 5 && fabs(turnError) < 5) {
            break;
        }

        wait(20, msec);
    }
    return 1;
}
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void autonomous(void) {
  vex::task PIDtask(drivePID); 
    desiredValue = 300;
    desiredTurnValue = 600;
    wait(3000, msec);
  // ..........................................................................
  // Insert autonomous user code here.
  // ..........................................................................
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void usercontrol(void) {
  enableDrivePID = false;
  // User control code here, inside the loop
  double turnImportance = 0.5; 
  while (1) {
    // This is the main execution loop for the user control program.
    // Each time through the loop your program should update motor + servo
    // values based on feedback from the joysticks.
    // ........................................................................
    // Insert user code here. This is where you use the joystick values to
    // update your motors, etc.
    // ........................................................................

    wait(20, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
  }
}

//
// Main will set up the competition functions and callbacks.
//
int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}
