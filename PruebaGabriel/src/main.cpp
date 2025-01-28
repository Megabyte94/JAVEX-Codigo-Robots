/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       HP-Laptop                                                 */
/*    Created:      10/8/2024, 7:12:51 PM                                     */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "vex.h"

using namespace vex;

// A global instance of vex::brain used for printing to the V5 brain screen
vex::brain       Brain;

// define your global instances of motors and other devices here
motor MotorI1 = motor(PORT1, ratio18_1, false);
motor MotorI2 = motor(PORT2, ratio18_1, false);
motor MotorI3 = motor(PORT3, ratio18_1, false);
motor MotorI4 = motor(PORT4, ratio18_1, false);
motor MotorD1 = motor(PORT5, ratio18_1, true);
motor MotorD2 = motor(PORT6, ratio18_1, true);
motor MotorD3 = motor(PORT7, ratio18_1, true);
motor MotorD4 = motor(PORT8, ratio18_1, true);

void stop() {
  MotorI1.stop();
  MotorI2.stop();
  MotorI3.stop();
  MotorI4.stop();
  MotorD1.stop();
  MotorD2.stop();
  MotorD3.stop();
  MotorD4.stop();
}

void moveForward(int speed, int duration) {
  MotorI1.spin(forward, speed, percent);
  MotorI2.spin(forward, speed, percent);
  MotorI3.spin(forward, speed, percent);
  MotorI4.spin(forward, speed, percent);
  MotorD1.spin(forward, speed, percent);
  MotorD2.spin(forward, speed, percent);
  MotorD3.spin(forward, speed, percent);
  MotorD4.spin(forward, speed, percent);
  wait(duration, seconds);
  stop();
}

// Función para mover el robot hacia atrás
void moveBackward(int speed, int duration) {
  MotorI1.spin(reverse, speed, percent);
  MotorI2.spin(reverse, speed, percent);
  MotorI3.spin(reverse, speed, percent);
  MotorI4.spin(reverse, speed, percent);
  MotorD1.spin(reverse, speed, percent);
  MotorD2.spin(reverse, speed, percent);
  MotorD3.spin(reverse, speed, percent);
  MotorD4.spin(reverse, speed, percent);
  wait(duration, seconds);
  stop();
}

// Función para girar el robot 90 grados a la derecha
void turnRight90(int speed) {
  MotorI1.spin(forward, speed, percent);
  MotorI2.spin(forward, speed, percent);
  MotorI3.spin(forward, speed, percent);
  MotorI4.spin(forward, speed, percent);
  MotorD1.spin(reverse, speed, percent);
  MotorD2.spin(reverse, speed, percent);
  MotorD3.spin(reverse, speed, percent);
  MotorD4.spin(reverse, speed, percent);
  wait(1.5, seconds);
  stop();
}

// Función para girar el robot 90 grados a la izquierda
void turnLeft90(int speed) {
  MotorI1.spin(reverse, speed, percent);
  MotorI2.spin(reverse, speed, percent);
  MotorI3.spin(reverse, speed, percent);
  MotorI4.spin(reverse, speed, percent);
  MotorD1.spin(forward, speed, percent);
  MotorD2.spin(forward, speed, percent);
  MotorD3.spin(forward, speed, percent);
  MotorD4.spin(forward, speed, percent);
  wait(1.5, seconds);
  stop();
}

// Función para girar el robot 45 grados a la derecha
void turnRight45(int speed) {
  MotorI1.spin(forward, speed, percent);
  MotorI2.spin(forward, speed, percent);
  MotorI3.spin(forward, speed, percent);
  MotorI4.spin(forward, speed, percent);
  MotorD1.spin(reverse, speed, percent);
  MotorD2.spin(reverse, speed, percent);
  MotorD3.spin(reverse, speed, percent);
  MotorD4.spin(reverse, speed, percent);
  wait(0.6, seconds);
  stop();
}

// Función para girar el robot 45 grados a la izquierda
void turnLeft90(int speed) {
  MotorI1.spin(reverse, speed, percent);
  MotorI2.spin(reverse, speed, percent);
  MotorI3.spin(reverse, speed, percent);
  MotorI4.spin(reverse, speed, percent);
  MotorD1.spin(forward, speed, percent);
  MotorD2.spin(forward, speed, percent);
  MotorD3.spin(forward, speed, percent);
  MotorD4.spin(forward, speed, percent);
  wait(0.6, seconds);
  stop();
}

int main() {
   
    while(1) {
        
        // Allow other tasks to run
        moveForward(50, 4);
        turnLeft90(25);
        turnLeft90(25);
        moveForward(50, 2);
        turnLeft90(25);
        moveForward(50, 1);
        turnRight90(25);
        moveForward(50, 1);
        turnRight90(25);
        moveForward(50, 2);
        turnRight45(25);
        moveForward(50, 1);
        turnRight90(25);
        turnRight90(25);
        moveForward(50, 3); 
        stop();
        this_thread::sleep_for(10);
    }
}
