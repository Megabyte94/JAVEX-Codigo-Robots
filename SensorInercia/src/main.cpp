/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       Melissa Ruiz                                              */
/*    Created:      9/20/2024, 11:47:40 AM                                    */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"
using namespace vex;

const double WHEEL_DIAMETER = 4.0;
const double WHEEL_CIRCUMFERENCE = WHEEL_DIAMETER * M_PI;
const double TRACK_WIDTH = 15.5;
const double RELATIVE_DISTANCE_ERROR = 0.4445;

brain Brain;
controller Controller1;
inertial InertiaSensor(PORT11); // Sensor de inercia

// Motores del lado izquierdo (puertos 1-4)
motor MotorL1(PORT14, true); 
motor MotorL2(PORT18, false);
motor MotorL3(PORT19, false);
motor MotorL4(PORT20, true);

// Motores del lado derecho (puertos 7-10)
motor MotorR1(PORT7, false);
motor MotorR2(PORT8, true);
motor MotorR3(PORT9, true);
motor MotorR4(PORT10, false);

// Motor para el sistema de recolección
motor Recolector(PORT1, true);
motor Rampa(PORT12, true);
motor Garra(PORT15, true);

vex::pneumatics Pinza(Brain.ThreeWirePort.A);
vex::pneumatics RecolectorNeumatica(Brain.ThreeWirePort.B);

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
}

// Movimientos basados en el sensor de inercia
void rotateUsingInertia(double targetAngle, double speed) {
    InertiaSensor.setHeading(0, degrees); // Reiniciar el ángulo de referencia
    if (targetAngle > 0) {
        setLeftMotors(speed);
        setRightMotors(-speed);
    } else {
        setLeftMotors(-speed);
        setRightMotors(speed);
    }
    
    while (fabs(InertiaSensor.heading() - targetAngle) > 1.0) {
        task::sleep(10);
    }
    stopAllMotors();
}

void moveStraightWithInertia(double distanceInInches, double speed) {
    resetEncoders();
    InertiaSensor.setHeading(0, degrees); // Reiniciar el ángulo

    distanceInInches = (1.0 - RELATIVE_DISTANCE_ERROR) * distanceInInches;
    double targetRotations = (distanceInInches / WHEEL_CIRCUMFERENCE) * 360;

    setLeftMotors(speed);
    setRightMotors(speed);

    while (fabs(MotorL1.position(rotationUnits::deg)) < targetRotations &&
           fabs(MotorR1.position(rotationUnits::deg)) < targetRotations) {
        // Ajuste en tiempo real basado en el ángulo de desviación
        double error = InertiaSensor.heading();
        setLeftMotors(speed - error * 0.5);
        setRightMotors(speed + error * 0.5);
        task::sleep(10);
    }
    stopAllMotors();
}

void initializeInertiaSensor() {
    if (!InertiaSensor.isCalibrating()) {
        InertiaSensor.calibrate();
    }
    while (InertiaSensor.isCalibrating()) {
        task::sleep(100);
    }
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
    initializeInertiaSensor();

    RecolectorNeumatica.open();
    moveStraightWithInertia(12, 100);
    moveStraightWithInertia(35, 60);
    rotateUsingInertia(20, 50);
    moveStraightWithInertia(18, 100);
    rotateUsingInertia(9, 50);
    Pinza.open();
    moveStraightWithInertia(60, -60);
    Pinza.close();
    recoleccion(100, 2);
    rotateUsingInertia(5, 50);
    moveStraightWithInertia(25, 50);
    recoleccion(100, 1);
    rotateUsingInertia(1.2, 50);
    moveStraightWithInertia(6.75, -50);
    garrita(-20, 0.5);
    garrita(-100, 0.5);
    rotateUsingInertia(10, 50);
    moveStraightWithInertia(45, 100);
    rotateUsingInertia(4, 50);
    moveStraightWithInertia(20, 100);
    rotateUsingInertia(48, 50);
    moveStraightWithInertia(40, 100);

    return 0;
}

