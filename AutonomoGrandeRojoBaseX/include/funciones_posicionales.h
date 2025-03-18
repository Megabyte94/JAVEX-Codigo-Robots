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
    MotorFL1.setPosition(0, rotationUnits::deg);
    MotorFR2.setPosition(0, rotationUnits::deg);
    MotorBL2.setPosition(0, rotationUnits::deg);
    MotorBR1.setPosition(0, rotationUnits::deg);
}

void setMotors(double diagonal1Speed, double diagonal2Speed) {
    // Para la diagonal 1 (MotorFL1 y MotorBL2)
    MotorFL1.spin(directionType::fwd, diagonal1Speed, velocityUnits::pct);
    MotorBL2.spin(directionType::fwd, diagonal1Speed, velocityUnits::pct);

    // Para la diagonal 2 (MotorFR2 y MotorBR1)
    MotorFR2.spin(directionType::fwd, diagonal2Speed, velocityUnits::pct);
    MotorBR1.spin(directionType::fwd, diagonal2Speed, velocityUnits::pct);
}

void stopAllMotors() {
    MotorFL1.stop();
    MotorFR2.stop();
    MotorBL2.stop();
    MotorBR1.stop();
    Recolector.stop();
    Rampa.stop();
    Garra.stop();
}

void move(double distanceInInches, double speed, double diagonal1SpeedFactor = 1.0, double diagonal2SpeedFactor = 1.0, bool useRecolector = false, bool useRampa = false, bool openRecolectorNeumatica = false, bool closeRecolectorNeumatica = false) {
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

    // Ajustamos las velocidades de las diagonales
    setMotors(speed * diagonal1SpeedFactor, speed * diagonal2SpeedFactor);

    while (fabs(MotorFL1.position(rotationUnits::deg)) < targetRotations &&
           fabs(MotorFR2.position(rotationUnits::deg)) < targetRotations) {
        task::sleep(10);
    }
    stopAllMotors();
}

void rotateOnAxis(double angleInDegrees, double speed, bool openBrazo = false) {
    resetEncoders();
    double targetRotations = (angleInDegrees / 360) * (TRACK_WIDTH * M_PI / WHEEL_DIAMETER) * 360;

    // Para rotar, se ajusta la velocidad de las diagonales de forma opuesta
    setMotors((angleInDegrees > 0) ? speed : -speed, (angleInDegrees > 0) ? -speed : speed);

    if (openBrazo) {
        brazo.open();
    }

    while (fabs(MotorFL1.position(rotationUnits::deg)) < targetRotations &&
           fabs(MotorFR2.position(rotationUnits::deg)) < targetRotations) {
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
    Garra.spin(reverse, speed, percent);
    task::sleep(duration * 1000); // Convertimos segundos a milisegundos
    stopAllMotors();
}
