/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       HP-Laptop                                                 */
/*    Created:      12/16/2024, 6:42:29 PM                                    */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "vex.h"

using namespace vex;

// A global instance of vex::brain used for printing to the V5 brain screen
vex::brain       Brain;
vex::controller Controller1;

vex::pneumatics Pinza(Brain.ThreeWirePort.A); 
// Motores del lado izquierdo
motor MotorL1(PORT14, false); 
motor MotorL2(PORT13, false);
motor MotorL3(PORT12, true);
motor MotorL4(PORT11, true);

// Motores del lado derecho
motor MotorR1(PORT4, true);
motor MotorR2(PORT3, true);
motor MotorR3(PORT2, false);
motor MotorR4(PORT1, false);

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

// Función para el control manual del robot
void driverControl() {
    while (true) {
        // Leer los valores de los joysticks
        double leftSpeed = Controller1.Axis3.position()-Controller1.Axis1.position();
        double rightSpeed = Controller1.Axis3.position()+ Controller1.Axis1.position();
        // Controlar los motores con los joysticks
        setLeftMotors(leftSpeed);
        setRightMotors(rightSpeed);

        // Pequeña pausa para evitar sobrecarga del bucle
        wait(20, msec);
        if (Controller1.ButtonL1.pressing()){
            Pinza.open();
        }else if (Controller1.ButtonL2.pressing()){
            Pinza.close(); 
        }
    }
}

// Programa principal
int main() {
    // Iniciar control manual
    driverControl(); 
    return 0; 
}