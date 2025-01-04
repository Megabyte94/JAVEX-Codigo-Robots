/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       Melissa Ruiz                                              */
/*    Created:      1/4/2025, 12:40:54 PM                                     */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "vex.h"
using namespace vex;

// Configuración de componentes
brain Brain;
controller Controller1;

// Motores del lado izquierdo
motor MotorL1(PORT14, true); 
motor MotorL2(PORT18, false);
motor MotorL3(PORT19, false);
motor MotorL4(PORT20, true);

// Motores del lado derecho
motor MotorR1(PORT7, false);
motor MotorR2(PORT8, true);
motor MotorR3(PORT9, true);
motor MotorR4(PORT10, false);

// Posición del sensor en el robot respecto al centro de rotación: X = 0 mm, Y = 0 mm, Z = 100 mm
// Construct a GPS Sensor "GPS1" with the gps class, with
// the origin at (10, 10) and using a 180 degree offset.
gps GPS1 = gps(PORT13, 180);

// Funciones auxiliares
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
}


void printGPSPosition() {
    // Obtener posición actual
    double posX = GPS1.xPosition(mm);
    double posY = GPS1.yPosition(mm);
    double heading = GPS1.heading(); // En grados

    // Imprimir en la pantalla del cerebro
    Brain.Screen.clearScreen();
    Brain.Screen.setCursor(1, 1);
    Brain.Screen.print("X: %.2f mm", posX);
    Brain.Screen.newLine();
    Brain.Screen.print("Y: %.2f mm", posY);
    Brain.Screen.newLine();
    Brain.Screen.print("Heading: %.2f deg", heading);

    wait(500, msec); // Pequeño retraso para actualizar
}

int main() {
    // Calibrar el GPS
    Brain.Screen.print("Calibrating GPS...");
    GPS1.calibrate();
    while (GPS1.isCalibrating()) {
        wait(100, msec);
    }
    Brain.Screen.clearScreen();
    Brain.Screen.print("GPS Ready!");

    // Prueba de movimiento con lectura de GPS
    while (true) {
        // Prueba: Avanzar y mostrar la posición
        setLeftMotors(30);
        setRightMotors(30);
        printGPSPosition();

        // Detener después de 5 segundos
        wait(3, seconds);
        stopAllMotors();

        // Mostrar posición final
        printGPSPosition();
        wait(5, seconds);
        break; // Salir del bucle
    }

    return 0;
}