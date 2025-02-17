#include "vex.h"
#include <vector>
#include <functional>

#pragma once // Para evitar errores de definición múltiple

using namespace vex;

// Librerías propias
#include "configuration.h"
#include "funciones_posicionales.h"

// Modo de control
int controlMode = 0;
bool pistonAbierto = false;
bool piston2Abierto = false; 

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
    LeftMotors.spin(forward, leftSpeed, percent);
    RightMotors.spin(forward, rightSpeed, percent);
}

// Función para controlar con un joystick
void singleJoystickControl() {
    int forwardSpeed = Controller1.Axis3.position();
    int turnSpeed = Controller1.Axis4.position();
    LeftMotors.spin(forward, forwardSpeed + turnSpeed, percent);
    RightMotors.spin(forward, forwardSpeed - turnSpeed, percent);
}

// Función para controlar con las flechas
void arrowControl() {
    if (Controller1.ButtonUp.pressing()) {
        LeftMotors.spin(forward, 100, percent);
        RightMotors.spin(forward, 100, percent);
    } else if (Controller1.ButtonDown.pressing()) {
        LeftMotors.spin(reverse, 100, percent);
        RightMotors.spin(reverse, 100, percent);
    } else if (Controller1.ButtonLeft.pressing()) {
        LeftMotors.spin(reverse, 100, percent);
        RightMotors.spin(forward, 100, percent);
    } else if (Controller1.ButtonRight.pressing()) {
        LeftMotors.spin(forward, 100, percent);
        RightMotors.spin(reverse, 100, percent);
    } else {
        LeftMotors.stop();
        RightMotors.stop();
    }
}
void joystickNewControl(){
  int leftSpeed=Controller1.Axis3.position(); 
  int rightSpeed = Controller1.Axis2.position(); 
  LeftMotors.spin(forward, leftSpeed, percent);
  RightMotors.spin(forward, rightSpeed, percent); 
}

// Función principal
/* int main() {
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
            Rampa.spin(directionType::fwd, 100, velocityUnits::pct);
        } else if (Controller1.ButtonL2.pressing()) {
            Recolector.spin(directionType::rev, 100, velocityUnits::pct);
            Rampa.spin(directionType::rev, 100, velocityUnits::pct);
        } else {
            Recolector.stop(brakeType::hold);
            Rampa.stop(brakeType::hold);
        }

        if(Controller1.ButtonR2.pressing())
        {
            // Esperamos a que el botón sea liberado para evitar múltiples activaciones en una sola pulsación
            while(Controller1.ButtonR2.pressing()) {
                // Espera a que el botón se suelte
            }

            // Cambiamos el estado del pistón
            pistonAbierto = !pistonAbierto;
            
            // Ejecutamos la acción correspondiente
            if(pistonAbierto) {
                Pinza.open();
            } else {
                Pinza.close();
            }
        }
        
        if(Controller1.ButtonB.pressing())
        {
            // Esperamos a que el botón sea liberado para evitar múltiples activaciones en una sola pulsación
            while(Controller1.ButtonB.pressing()) {
                // Espera a que el botón se suelte
            }

            // Cambiamos el estado del pistón
            pistonAbierto = !pistonAbierto;
            
            // Ejecutamos la acción correspondiente
            if(pistonAbierto) {
                brazo.open();
            } else {
                brazo.close();
            }
        }

        if(Controller1.ButtonR1.pressing())
        {
            // Esperamos a que el botón sea liberado para evitar múltiples activaciones en una sola pulsación
            while(Controller1.ButtonR1.pressing()) {
                // Espera a que el botón se suelte
            }

            // Cambiamos el estado del pistón
            piston2Abierto = !piston2Abierto;
            
            // Ejecutamos la acción correspondiente
            if(piston2Abierto) {
                RecolectorNeumatica.open();
            } else {
                RecolectorNeumatica.close();
            }
        }



        // Espera para evitar saturar el CPU
        task::sleep(20);
    }
} */