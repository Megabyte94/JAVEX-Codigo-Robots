#include "vex.h"
#include <cmath>

using namespace vex;


//1----------
// Definiciones y constantes

#define M_PI 3.14159265358979323846 // Valor de pi
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
inertial Inercial(PORT11); 

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
motor Recolector(PORT1, true);
motor Rampa(PORT12, true);
motor Garra(PORT15, true);

vex::pneumatics Pinza(Brain.ThreeWirePort.A);
vex::pneumatics RecolectorNeumatica(Brain.ThreeWirePort.B);


//1----------
// Funciones auxiliares

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


//1----------
// Funciones de movimiento

void universalMove(double distanceInInches, double leftSpeed, double rightSpeed, double collectionSpeed) {
    // Resetear los encoders al inicio
    resetEncoders();

    // Ajustar la distancia de acuerdo con el error relativo
    distanceInInches = (1.0 - RELATIVE_DISTANCE_ERROR) * distanceInInches;

    // Calcular las rotaciones necesarias de acuerdo a la distancia
    double targetRotations = (distanceInInches / WHEEL_CIRCUMFERENCE) * 360;

    // Iniciar los motores con la velocidad indicada
    setLeftMotors(leftSpeed);
    setRightMotors(rightSpeed);

    // Si se especifica, iniciar los motores para el sistema de recolección
    Recolector.spin(reverse, collectionSpeed, percent);
    Rampa.spin(reverse, collectionSpeed, percent);

    // Mientras los motores no hayan alcanzado la distancia deseada, continuar moviendo
    while (fabs(MotorL1.position(rotationUnits::deg)) < targetRotations &&
           fabs(MotorR1.position(rotationUnits::deg)) < targetRotations) {
        // Esperar un corto tiempo para permitir que los motores avancen
        task::sleep(10);
    }

    // Detener todos los motores al finalizar el movimiento
    stopAllMotors();
}
void rotateOnAxis(double targetDegrees, double speed) {
    // Resetear los encoders al inicio
    resetEncoders();

    // Calcular las rotaciones necesarias para alcanzar el ángulo objetivo
    double targetRotations = (targetDegrees / 360) * (TRACK_WIDTH * M_PI / WHEEL_DIAMETER) * 360;

    // Iniciar los motores para girar a la velocidad indicada
    setLeftMotors((targetDegrees > 0) ? speed : -speed);
    setRightMotors((targetDegrees > 0) ? -speed : speed);

    // Mientras los motores no hayan alcanzado el número de rotaciones necesario, continuar girando
    while (fabs(MotorL1.position(rotationUnits::deg)) < targetRotations &&
           fabs(MotorR1.position(rotationUnits::deg)) < targetRotations) {
        // Esperar un corto tiempo para permitir que los motores avancen
        task::sleep(10);
    }

    // Detener todos los motores al finalizar la rotación
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


//1----------
// Programa principal

int main() {
    // Calibración del sensor de inercia
    Inercial.calibrate();
    while (Inercial.isCalibrating()) {
        wait(100, msec);
    }

    //1----------
    // Secuencia de movimientos

    RecolectorNeumatica.open();
    universalMove(12, 100, 100, 0);
    universalMove(35, 20, 100, 0);
    universalMove(35, 100, 40, 100);
    universalMove(18, 100, 100, 100);

    rotateOnAxis(20, -100);

    universalMove(33, -45, -70, 0);
    universalMove(35, -90, -50, 0);

    rotateOnAxis(7, 100);

    Pinza.open();

    universalMove(60, -60, -60, 0);

    Pinza.close();

    recoleccion(100,2);

    rotateOnAxis(5, -100);

    universalMove(25, 50, 50, 50);

    recoleccion(100,1);

    rotateOnAxis(1.2, -100);

    universalMove(6.75, -50, -50, 0);

    garrita(-20,0.5);
    garrita(-100,0.5);

    rotateOnAxis(10, 100);

    universalMove(45, 100, 100, 100);

    rotateOnAxis(4, -100);

    universalMove(20, 100, 100, 100);

    rotateOnAxis(48, -100);

    universalMove(40, 100, 100, 100);


    return 0;
}
