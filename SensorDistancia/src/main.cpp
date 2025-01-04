/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       Melissa Ruiz                                              */
/*    Created:      1/4/2025, 12:33:42 PM                                     */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "vex.h"
using namespace vex;

// Constantes
const double DETECTION_DISTANCE = 800.0; // Distancia en milímetros para detenerse

// Declaración de dispositivos
brain Brain;
controller Controller1;
vex::distance DistSensor(PORT16); // Sensor de distancia conectado al puerto 2

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

// Función para probar el sensor de distancia
void testDistanceSensor() {
    Brain.Screen.clearScreen();
    Brain.Screen.print("Testing Distance Sensor...");
    wait(1, seconds);

    while (true) {
        double distance = DistSensor.objectDistance(mm); // Medir la distancia en milímetros

        // Mostrar la distancia en la pantalla del cerebro
        Brain.Screen.clearLine();
        Brain.Screen.print("Distance: %.2f mm", distance);

        // Si la distancia detectada es menor que el límite, detenerse
        if (distance < DETECTION_DISTANCE && distance > 0) {
            stopAllMotors();
            Brain.Screen.newLine();
            Brain.Screen.print("Obstacle detected!");
            wait(2, seconds); // Pausa para indicar la detección
            break; // Salir del bucle para continuar
        } else {
            // Continuar moviéndose si no hay obstáculos cercanos
            setLeftMotors(50); // Velocidad de prueba
            setRightMotors(50);
        }

        wait(100, msec); // Esperar para evitar demasiadas lecturas consecutivas
    }

    stopAllMotors();
    Brain.Screen.newLine();
    Brain.Screen.print("Test complete.");
}

// Programa principal
int main() {
    // Inicializar sensores
    Brain.Screen.clearScreen();
    Brain.Screen.print("Initializing...");
    wait(2, seconds);

    // Probar el sensor de distancia
    testDistanceSensor();

    return 0;
}
