/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       Kenneth Bustamante                                        */
/*    Created:      14/3/2024, 11:48:05                                       */
/*    Description:  V5 project                                                */
/*                                                                            */

#include "vex.h"
using namespace vex;

const double WHEEL_DIAMETER = 4.0;
const double WHEEL_CIRCUMFERENCE = WHEEL_DIAMETER * M_PI;
const double TRACK_WIDTH = 15.5;
const double RELATIVE_DISTANCE_ERROR = 0.4445;

//  ____
// |  [LA1]    [RA1]    |
// |[LB2]         [RB2] |
// |                    |
// |  [LA3]    [RA3]    |
// |[L4B]         [RB4] |
//  --------------------
brain Brain;
controller Controller1;


// Motores del lado izquierdo (puertos 1-4)
motor MotorL1(PORT14, true); 
motor MotorL2(PORT18, false);
motor MotorL3(PORT19, false);
motor MotorL4(PORT20, true);
//motor_group Left(LeftMotor1, LeftMotor2, LeftMotor3, LeftMotor4);

// Motores del lado derecho (puertos 7-10)
motor MotorR1(PORT7, false);
motor MotorR2(PORT8, true);
motor MotorR3(PORT9, true);
motor MotorR4(PORT10, false);
//motor_group Right(RightMotor1, RightMotor2, RightMotor3, RightMotor4);

// Motor para el sistema de recolección
motor Recolector(PORT15, true);
motor Rampa(PORT12, true);
motor Garra(PORT1, true);


vex::pneumatics Pinza(Brain.ThreeWirePort.A);
vex::pneumatics RecolectorNeumatica(Brain.ThreeWirePort.B);

triport expansor(PORT16);
vex::pneumatics brazo(expansor.C);

void resetEncoders() {
    MotorL1.setPosition(0, rotationUnits::deg);
    MotorL2.setPosition(0, rotationUnits::deg);
    MotorL3.setPosition(0, rotationUnits::deg);
    MotorL4.setPosition(0, rotationUnits::deg);
    MotorR1.setPosition(0, rotationUnits::deg);
    MotorR2.setPosition(0, rotationUnits::deg);
    MotorR3.setPosition(0, rotationUnits::deg);
    MotorR4.setPosition(0, rotationUnits::deg);
}

void setLeftMotors(double speed) {
    MotorL1.spin(directionType::fwd, speed, velocityUnits::pct);
    MotorL2.spin(directionType::fwd, speed, velocityUnits::pct);
    MotorL3.spin(directionType::fwd, speed, velocityUnits::pct);
    MotorL4.spin(directionType::fwd, speed, velocityUnits::pct);
}

void setRightMotors(double speed) {
    MotorR1.spin(directionType::fwd, speed, velocityUnits::pct);
    MotorR2.spin(directionType::fwd, speed, velocityUnits::pct);
    MotorR3.spin(directionType::fwd, speed, velocityUnits::pct);
    MotorR4.spin(directionType::fwd, speed, velocityUnits::pct);
}

void stopAllMotors() {
    MotorL1.stop();
    MotorL2.stop();
    MotorL3.stop();
    MotorL4.stop();
    MotorR1.stop();
    MotorR2.stop();
    MotorR3.stop();
    MotorR4.stop();
    Recolector.stop();
    Rampa.stop();
    Garra.stop();
    brazo.close();
}

void moveDistance(double distanceInInches, double speed) {
    resetEncoders();

    distanceInInches = (1.0 - RELATIVE_DISTANCE_ERROR) * distanceInInches;

    double targetRotations = (distanceInInches / WHEEL_CIRCUMFERENCE) * 360;

    setLeftMotors(speed);
    setRightMotors(speed);

    while (fabs(MotorL1.position(rotationUnits::deg)) < targetRotations &&
           fabs(MotorR1.position(rotationUnits::deg)) < targetRotations) {
        task::sleep(10);
    }
    stopAllMotors();
}

void moveParabolic(double distanceInInches, double speedleft, double speedRight) {
    resetEncoders();

    distanceInInches = (1.0 - RELATIVE_DISTANCE_ERROR) * distanceInInches;

    double targetRotations = (distanceInInches / WHEEL_CIRCUMFERENCE) * 360;

    
    setLeftMotors(speedleft);
    setRightMotors(speedRight);

    while (fabs(MotorL1.position(rotationUnits::deg)) < targetRotations &&
           fabs(MotorR1.position(rotationUnits::deg)) < targetRotations) {
        task::sleep(10);
    }
    stopAllMotors();
}

void moveParabolicN(double distanceInInches, double speedleft, double speedRight, double speed) {
    resetEncoders();

    Recolector.spin(reverse, speed, percent);
    Rampa.spin(reverse, speed, percent);

    distanceInInches = (1.0 - RELATIVE_DISTANCE_ERROR) * distanceInInches;

    double targetRotations = (distanceInInches / WHEEL_CIRCUMFERENCE) * 360;

    
    setLeftMotors(speedleft);
    setRightMotors(speedRight);

    while (fabs(MotorL1.position(rotationUnits::deg)) < targetRotations &&
           fabs(MotorR1.position(rotationUnits::deg)) < targetRotations) {
        task::sleep(10);
    }
    stopAllMotors();
}

void rotateOnAxis(double angleInDegrees, double speed) {
    resetEncoders();
    double targetRotations = (angleInDegrees / 360) * (TRACK_WIDTH * M_PI / WHEEL_DIAMETER) * 360;

    setLeftMotors((angleInDegrees > 0) ? speed : -speed);
    setRightMotors((angleInDegrees > 0) ? -speed : speed);

    while ( (fabs(MotorL1.position(rotationUnits::deg)) < targetRotations) && (fabs(MotorR1.position(rotationUnits::deg)) < targetRotations) ) {
        task::sleep(10);
    }
    stopAllMotors();
}

void rotateOnAxisN(double angleInDegrees, double speed) {
    resetEncoders();

    double targetRotations = (angleInDegrees / 360) * (TRACK_WIDTH * M_PI / WHEEL_DIAMETER) * 360;

    setLeftMotors((angleInDegrees > 0) ? speed : -speed);
    setRightMotors((angleInDegrees > 0) ? -speed : speed);
    
    brazo.open();

    while ( (fabs(MotorL1.position(rotationUnits::deg)) < targetRotations) && (fabs(MotorR1.position(rotationUnits::deg)) < targetRotations) ) {
        task::sleep(10);
    }
    stopAllMotors();
    
}

void moveDistanceN(double distanceInInches, double speed) {
    resetEncoders();

    Recolector.spin(reverse, speed, percent);
    Rampa.spin(reverse, speed, percent);

    RecolectorNeumatica.close();

    distanceInInches = (1.0 - RELATIVE_DISTANCE_ERROR) * distanceInInches;

    double targetRotations = (distanceInInches / WHEEL_CIRCUMFERENCE) * 360;

    setLeftMotors(speed);
    setRightMotors(speed);

    while (fabs(MotorL1.position(rotationUnits::deg)) < targetRotations &&
           fabs(MotorR1.position(rotationUnits::deg)) < targetRotations) {
        task::sleep(10);
    }
    stopAllMotors();

    
}

void moveDistanceB(double distanceInInches, double speed) {
    resetEncoders();

    brazo.open();
    
    distanceInInches = (1.0 - RELATIVE_DISTANCE_ERROR) * distanceInInches;

    double targetRotations = (distanceInInches / WHEEL_CIRCUMFERENCE) * 360;

    setLeftMotors(speed);
    setRightMotors(speed);

    while (fabs(MotorL1.position(rotationUnits::deg)) < targetRotations &&
           fabs(MotorR1.position(rotationUnits::deg)) < targetRotations) {
        task::sleep(10);
    }
    stopAllMotors();

    
}

void turnAngle(double angleInDegrees, double speed) {
    resetEncoders();
    double targetRotations = (angleInDegrees / 360) * (TRACK_WIDTH * M_PI / WHEEL_DIAMETER) * 360;

    setLeftMotors((angleInDegrees > 0) ? speed : -speed);
    setRightMotors((angleInDegrees > 0) ? -speed : speed);

    while ( (fabs(MotorL1.position(rotationUnits::deg)) < targetRotations) && (fabs(MotorR1.position(rotationUnits::deg)) < targetRotations) ) {
        task::sleep(10);
    }
    stopAllMotors();
}

void recoleccion(int speed,double duration) {
  Recolector.spin(reverse, speed, percent);
  Rampa.spin(reverse, speed, percent);
  wait(duration, seconds);
  stopAllMotors();
}

void garrita(int speed,double duration) {
  Garra.spin(reverse, speed, percent);
  wait(duration, seconds);
  stopAllMotors();
}

int main() {

    /*moveParabolic(42, 100, 57);
    moveDistance(30, 100);
    moveDistanceB(20, -100);*/


    /*moveParabolic(42, 100, 56);
    moveDistance(25, 100);
    rotateOnAxisN(86, 100);
    rotateOnAxis(15, 100);
    moveParabolic(60, -100, -75);
    rotateOnAxis(17, -100);
    moveDistance(60, -100);
    Pinza.open();
    recoleccion(100,2);*/

    
    
    //RecolectorNeumatica.open();
    /*moveParabolic(47, 100, 80);
    rotateOnAxis(20, 100);
    moveDistance(10, 100);
    rotateOnAxisN(33, 100);*/


    moveParabolic(42, 100, 60);
    moveDistance(25, 100);
    rotateOnAxisN(43, 100);
    rotateOnAxis(6, 100);
    moveParabolicN(38.5, 90, 100, 100);
    /*recoleccion(100,2);
    rotateOnAxis(21, 100);
    moveDistance(35, -100);
    Pinza.open();*/




    //moveParabolicN(45, 100, 55, 100);
    //recoleccion(100,1.5);
    //rotateOnAxis(13, 100);
    //moveParabolic(65, -100, -98);
    //moveDistance(65, -100);

    //moveParabolicN(35, 100, 40, 100);
    //moveDistanceN(18, 100);
    //rotateOnAxis(40, 100);
    //moveDistance(60, -100);
    //moveParabolic(33, -45, -70);
    //moveParabolic(35, -90, -50);
    //rotateOnAxis(9, -100);
    //rotateOnAxis(60, 100);
    //Pinza.open();
    //moveDistance(60, -60);
    //Pinza.close();
    //recoleccion(100,2);
    //rotateOnAxis(5, -100);
    //moveDistanceN(25, 50);
    //recoleccion(100,1);
    //rotateOnAxis(1.2, -100);
    //moveDistance(6.75, -50);
    //garrita(-20,0.5);
    //garrita(-100,0.5);
    //rotateOnAxis(10, 100);
    //moveDistanceN(45, 100);
    //rotateOnAxis(4, -100);
    //moveDistanceN(20, 100);
    //rotateOnAxis(48, -100);
    //moveDistanceN(40, 100);








    //RecolectorNeumatica.close();
    //recoleccion(100, 4);
    
    //moveDistance(46, 100);
    //rotateOnAxis(40, 100);
    //moveParabolic(20, 80, 100);
    //moveParabolic(70, 1, 100);
    //RecolectorNeumatica.open();
    //moveDistanceN(30, 100);
    return 0;
}