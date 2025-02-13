/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       Melissa Ruiz                                            */
/*    Created:      14/3/2024, 11:48:05                                       */
/*    Description:  V5 project                                                */
/*                                                                            */

#include "vex.h"

// Inicialización de objetos del dispositivo VEX
using namespace vex;

// Configuración del cerebro, controlador y motores
brain Brain;
controller Controller1;

// Motores del lado izquierdo (puertos 1-4)
motor LeftMotor1(PORT14, true); 
motor LeftMotor2(PORT18, false);
motor LeftMotor3(PORT19, false);
motor LeftMotor4(PORT20, true);
motor_group Left(LeftMotor1, LeftMotor2, LeftMotor3, LeftMotor4);

// Motores del lado derecho (puertos 7-10)
motor RightMotor1(PORT7, false);
motor RightMotor2(PORT8, true);
motor RightMotor3(PORT9, true);
motor RightMotor4(PORT10, false);
motor_group Right(RightMotor1, RightMotor2, RightMotor3, RightMotor4);

// Motor para el sistema de recolección
motor Recolector(PORT1, true);
motor Rampa(PORT12, true);

// Modo de control
int controlMode = 0;

// Función para cambiar el modo de control
void switchControlMode() {
    controlMode = (controlMode + 1) % 4;
    Brain.Screen.clearScreen();
    Brain.Screen.print("Modo de control: %d", controlMode);
}

// Función para controlar con dos joysticks
void twoJoysticksControl() {
    int leftSpeed = Controller1.Axis3.position()+Controller1.Axis1.position();
    int rightSpeed = Controller1.Axis3.position()- Controller1.Axis1.position();
    Left.spin(forward, leftSpeed, percent);
    Right.spin(forward, rightSpeed, percent);
}

// Función para controlar con un joystick
void singleJoystickControl() {
    int forwardSpeed = Controller1.Axis3.position();
    int turnSpeed = Controller1.Axis4.position();
    Left.spin(forward, forwardSpeed + turnSpeed, percent);
    Right.spin(forward, forwardSpeed - turnSpeed, percent);
}

// Función para controlar con las flechas
void arrowControl() {
    if (Controller1.ButtonUp.pressing()) {
        Left.spin(forward, 100, percent);
        Right.spin(forward, 100, percent);
    } else if (Controller1.ButtonDown.pressing()) {
        Left.spin(reverse, 100, percent);
        Right.spin(reverse, 100, percent);
    } else if (Controller1.ButtonLeft.pressing()) {
        Left.spin(reverse, 100, percent);
        Right.spin(forward, 100, percent);
    } else if (Controller1.ButtonRight.pressing()) {
        Left.spin(forward, 100, percent);
        Right.spin(reverse, 100, percent);
    } else {
        Left.stop();
        Right.stop();
    }
}

void joystickNewControl(){
  int leftSpeed=Controller1.Axis3.position(); 
  int rightSpeed = Controller1.Axis2.position(); 
  Left.spin(forward, leftSpeed, percent);
  Right.spin(forward, rightSpeed, percent); 
}

// Función principal
int main() {
    while (true) {
        // Cambiar el modo de control con el botón A
        if (Controller1.ButtonA.pressing()) {
            switchControlMode();
            while (Controller1.ButtonA.pressing()) {
                // Esperar a que se suelte el botón A
                task::sleep(10);
            }
        }

        // Control del robot basado en el modo seleccionado
        if (controlMode == 0) {
            twoJoysticksControl();
        } else if (controlMode == 1) {
            singleJoystickControl();
        } else if (controlMode == 2) {
            arrowControl();
        }else if(controlMode == 3){
          joystickNewControl(); 
        }

        // Control del motor recolector y rampa usando L1 y L2
        if (Controller1.ButtonL1.pressing()) {
            Recolector.spin(directionType::fwd, 100, velocityUnits::pct);
            Rampa.spin(directionType::fwd, 65, velocityUnits::pct);
        } else if (Controller1.ButtonL2.pressing()) {
            Recolector.spin(directionType::rev, 100, velocityUnits::pct);
            Rampa.spin(directionType::rev, 65, velocityUnits::pct);
        } else {
            Recolector.stop(brakeType::hold);
            Rampa.stop(brakeType::hold);
        }

        // Espera para evitar saturar el CPU
        task::sleep(20);
    }
}