#include "vex.h"
using namespace vex;

// Librerías propias
#include "configuration.h"
#include "funciones_posicionales.h"

// Modo de control
int controlMode = 0;
bool pistonAbierto = false;

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