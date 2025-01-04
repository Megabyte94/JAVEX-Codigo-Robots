/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       Melissa Ruiz                                              */
/*    Created:      1/4/2025, 12:46:39 PM                                     */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/


#include "vex.h"
using namespace vex;

const double WHEEL_DIAMETER = 4.0;
const double WHEEL_CIRCUMFERENCE = WHEEL_DIAMETER * M_PI;
const double TRACK_WIDTH = 15.5;
const double RELATIVE_DISTANCE_ERROR = 0.4445;

// Firmas de las funciones
//  ____
// |  [LA1]    [RA1]    |
// |[LB2]         [RB2] |
// |                    |
// |  [LA3]    [RA3]    |
// |[L4B]         [RB4] |
//  --------------------
brain Brain;
controller Controller1;
inertial Inercial(PORT11); 

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

void moveDistance(double distanceInInches, double speed) {
    resetEncoders();
    distanceInInches = (1.0 - RELATIVE_DISTANCE_ERROR) * distanceInInches;
    double targetRotations = (distanceInInches / WHEEL_CIRCUMFERENCE) * 360;

    double initialAngle = Inercial.rotation(degrees);

    setLeftMotors(speed);
    setRightMotors(speed);

    while (fabs(MotorL1.position(rotationUnits::deg)) < targetRotations &&
           fabs(MotorR1.position(rotationUnits::deg)) < targetRotations) {
        double currentAngle = Inercial.rotation(degrees);
        double correction = (currentAngle - initialAngle) * 0.5; // Factor de corrección
        setLeftMotors(speed + correction);
        setRightMotors(speed + correction);

        task::sleep(10);
    }
    stopAllMotors();
}


void moveParabolic(double distanceInInches, double speedLeft, double speedRight) {
    resetEncoders();
    Inercial.setHeading(0, degrees); 
    
    distanceInInches = (1.0 - RELATIVE_DISTANCE_ERROR) * distanceInInches;
    double targetRotations = (distanceInInches / WHEEL_CIRCUMFERENCE) * 360;

    setLeftMotors(speedLeft);
    setRightMotors(speedRight);

    while (fabs(MotorL1.position(rotationUnits::deg)) < targetRotations &&
           fabs(MotorR1.position(rotationUnits::deg)) < targetRotations) {
        double error = Inercial.heading(); 
        setLeftMotors(speedLeft + error * 0.5);
        setRightMotors(speedRight + error * 0.5);
        task::sleep(10);
    }
    stopAllMotors();
}

void moveParabolicN(double distanceInInches, double speedLeft, double speedRight, double speed) {
    resetEncoders();
    Inercial.setHeading(0, degrees);

    Recolector.spin(reverse, speed, percent);
    Rampa.spin(reverse, speed, percent);

    distanceInInches = (1.0 - RELATIVE_DISTANCE_ERROR) * distanceInInches;
    double targetRotations = (distanceInInches / WHEEL_CIRCUMFERENCE) * 360;

    setLeftMotors(speedLeft);
    setRightMotors(speedRight);

    while (fabs(MotorL1.position(rotationUnits::deg)) < targetRotations &&
           fabs(MotorR1.position(rotationUnits::deg)) < targetRotations) {
        double error = Inercial.heading(); 
        setLeftMotors(speedLeft + error * 0.5);
        setRightMotors(speedRight + error * 0.5);
        task::sleep(10);
    }
    stopAllMotors();
}

void rotateOnAxis(double targetDegrees, double speed) {
    // Reiniciar el sensor de inercia
    resetEncoders();
    Inercial.resetRotation();
    // Determinar el sentido de giro (positivo para derecha, negativo para izquierda)
    double direction = targetDegrees > 0 ? 1.0 : -1.0;

    speed = speed*direction; 

    // Configurar los motores para girar
    setLeftMotors(speed); 
    setRightMotors(-speed); 

    // Bucle para ajustar el giro
    while (fabs(Inercial.rotation(degrees)) < fabs(targetDegrees)) {
        wait(10, msec);
    }

    // Detener los motores al alcanzar el ángulo deseado
    stopAllMotors(); 
}


void moveDistanceN(double distanceInInches, double speed) {
    resetEncoders();

    Recolector.spin(reverse, speed, percent);
    Rampa.spin(reverse, speed, percent);
    RecolectorNeumatica.close();

    distanceInInches = (1.0 - RELATIVE_DISTANCE_ERROR) * distanceInInches;
    double targetRotations = (distanceInInches / WHEEL_CIRCUMFERENCE) * 360;

    double initialAngle = Inercial.rotation(degrees);

    setLeftMotors(speed);
    setRightMotors(speed);

    while (fabs(MotorL1.position(rotationUnits::deg)) < targetRotations &&
           fabs(MotorR1.position(rotationUnits::deg)) < targetRotations) {
        double currentAngle = Inercial.rotation(degrees);
        double correction = (currentAngle - initialAngle) * 0.5; // Factor de corrección
        setLeftMotors(speed + correction);
        setRightMotors(speed + correction);

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
    RecolectorNeumatica.open();
    moveDistance(12, 100);
    moveParabolic(35, 20, 100);
    moveParabolicN(35, 100, 40, 100);
    moveDistanceN(18, 100);
    rotateOnAxis(20, -100);
    //moveDistance(60, -100);
    moveParabolic(33, -45, -70);
    moveParabolic(35, -90, -50);
    rotateOnAxis(7, 100);
    //rotateOnAxis(60, 100);
    Pinza.open();
    moveDistance(60, -60);
    Pinza.close();
    recoleccion(100,2);
    rotateOnAxis(5, -100);
    moveDistanceN(25, 50);
    recoleccion(100,1);
    rotateOnAxis(1.2, -100);
    moveDistance(6.75, -50);
    garrita(-20,0.5);
    garrita(-100,0.5);
    rotateOnAxis(10, 100);
    moveDistanceN(45, 100);
    rotateOnAxis(4, -100);
    moveDistanceN(20, 100);
    rotateOnAxis(48, -100);
    moveDistanceN(40, 100);
    return 0;
}

