#include "vex.h"
#include <vector>
#include <functional>
#include <iostream>

#pragma once // Para evitar errores de definición múltiple

#include "configuration.h" // Incluimos la configuración de variables

using namespace vex;
using Callbacks = std::vector<std::function<void()>>;

using std::cout;
using std::endl;

//---------------- FUNCIONES DE MOVIMIENTO ----------------

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

void setMotors(double leftSpeed, double rightSpeed) {
    MotorL1.spin(directionType::fwd, leftSpeed, velocityUnits::pct);
    MotorL2.spin(directionType::fwd, leftSpeed, velocityUnits::pct);
    MotorL3.spin(directionType::fwd, leftSpeed, velocityUnits::pct);
    MotorL4.spin(directionType::fwd, leftSpeed, velocityUnits::pct);

    MotorR1.spin(directionType::fwd, rightSpeed, velocityUnits::pct);
    MotorR2.spin(directionType::fwd, rightSpeed, velocityUnits::pct);
    MotorR3.spin(directionType::fwd, rightSpeed, velocityUnits::pct);
    MotorR4.spin(directionType::fwd, rightSpeed, velocityUnits::pct);
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
    //Garra.stop();
}

void move(double distanceInInches, double speed, double leftSpeedFactor = 1.0, double rightSpeedFactor = 1.0, bool useRecolector = false, bool useRampa = false, bool openRecolectorNeumatica = false, bool closeRecolectorNeumatica = false) {
    resetEncoders();

    if (useRecolector) {
        Recolector.spin(reverse, speed, percent);
    }
    if (useRampa) {
        Rampa.spin(reverse, speed, percent);
    }

    if (openRecolectorNeumatica) {
        RecolectorNeumatica.open();
    }
    if (closeRecolectorNeumatica) {
        RecolectorNeumatica.close();
    }

    distanceInInches = (1.0 - RELATIVE_DISTANCE_ERROR) * distanceInInches;
    double targetRotations = (distanceInInches / WHEEL_CIRCUMFERENCE) * 360;

    setMotors(speed * leftSpeedFactor, speed * rightSpeedFactor);

    while (fabs(MotorL1.position(rotationUnits::deg)) < targetRotations &&
           fabs(MotorR1.position(rotationUnits::deg)) < targetRotations) {
        task::sleep(10);
    }
    stopAllMotors();
}

void rotateOnAxis(double angleInDegrees, double speed, bool openBrazo = false) {
    resetEncoders();
    double targetRotations = (angleInDegrees / 360) * (TRACK_WIDTH * M_PI / WHEEL_DIAMETER) * 360;

    setMotors((angleInDegrees > 0) ? speed : -speed, (angleInDegrees > 0) ? -speed : speed);

    if (openBrazo) {
        brazo.open();
    }

    while (fabs(MotorL1.position(rotationUnits::deg)) < targetRotations &&
           fabs(MotorR1.position(rotationUnits::deg)) < targetRotations) {
        task::sleep(10);
    }
    stopAllMotors();
}

void recoleccion(int speed, double duration) {
    Recolector.spin(reverse, speed, percent);
    Rampa.spin(reverse, speed, percent);
    task::sleep(duration * 1000); // Convertimos segundos a milisegundos
    stopAllMotors();
}

void garrita(int speed, double duration) {
    //Garra.spin(reverse, speed, percent);
    task::sleep(duration * 1000); // Convertimos segundos a milisegundos
    stopAllMotors();
}
