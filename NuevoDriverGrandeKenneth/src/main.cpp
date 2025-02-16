#include "vex.h"

// Inicialización de objetos del dispositivo VEX
using namespace vex;

// Configuración del cerebro, controlador y motores
brain Brain;
controller Controller1;

//  ____________________
// |  [LA1]    [RA1]    |
// |[LB2]         [RB2] |
// |                    |
// |  [LA3]    [RA3]    |
// |[L4B]         [RB4] |
//  --------------------

// Motores del lado izquierdo (puertos 1-4)
motor MotorL1(PORT14, true); 
motor MotorL2(PORT18, false);
motor MotorL3(PORT19, false);
motor MotorL4(PORT20, true);
motor_group Left(MotorL1, MotorL2, MotorL3, MotorL4);

// Motores del lado derecho (puertos 7-10)
motor MotorR1(PORT7, false);
motor MotorR2(PORT8, true);
motor MotorR3(PORT9, true);
motor MotorR4(PORT10, false);
motor_group Right(MotorR1, MotorR2, MotorR3, MotorR4);

// Motores para pruebas (puertos 1-4)
motor MotorPrueba1(PORT1, true);
motor MotorPrueba2(PORT2, false);
motor MotorPrueba3(PORT3, true);
motor MotorPrueba4(PORT4, false);

// Motor para el sistema de recolección
motor Recolector(PORT15, true);
motor Rampa(PORT12, false);

vex::pneumatics Pinza(Brain.ThreeWirePort.A);
vex::pneumatics RecolectorNeumatica(Brain.ThreeWirePort.B);

triport expansor(PORT16);
vex::pneumatics brazo(expansor.C);

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

// Función para controlar los motores de prueba con los botones X y Y
void controlMotoresPrueba() {
    if (Controller1.ButtonX.pressing()) {
        MotorPrueba1.spin(forward, 100, percent);
        MotorPrueba2.spin(forward, 100, percent);
        MotorPrueba3.spin(forward, 100, percent);
        MotorPrueba4.spin(forward, 100, percent);
    } else if (Controller1.ButtonY.pressing()) {
        MotorPrueba1.spin(reverse, 100, percent);
        MotorPrueba2.spin(reverse, 100, percent);
        MotorPrueba3.spin(reverse, 100, percent);
        MotorPrueba4.spin(reverse, 100, percent);
    } else {
        MotorPrueba1.stop();
        MotorPrueba2.stop();
        MotorPrueba3.stop();
        MotorPrueba4.stop();
    }
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
        } else if (controlMode == 3) {
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

        if (Controller1.ButtonR2.pressing()) {
            // Esperamos a que el botón sea liberado para evitar múltiples activaciones en una sola pulsación
            while (Controller1.ButtonR2.pressing()) {
                // Espera a que el botón se suelte
            }

            // Cambiamos el estado del pistón
            pistonAbierto = !pistonAbierto;

            // Ejecutamos la acción correspondiente
            if (pistonAbierto) {
                Pinza.open();
            } else {
                Pinza.close();
            }
        }

        if (Controller1.ButtonB.pressing()) {
            // Esperamos a que el botón sea liberado para evitar múltiples activaciones en una sola pulsación
            while (Controller1.ButtonB.pressing()) {
                // Espera a que el botón se suelte
            }

            // Cambiamos el estado del pistón
            pistonAbierto = !pistonAbierto;

            // Ejecutamos la acción correspondiente
            if (pistonAbierto) {
                brazo.open();
            } else {
                brazo.close();
            }
        }

        if (Controller1.ButtonR1.pressing()) {
            // Esperamos a que el botón sea liberado para evitar múltiples activaciones en una sola pulsación
            while (Controller1.ButtonR1.pressing()) {
                // Espera a que el botón se suelte
            }

            // Cambiamos el estado del pistón
            piston2Abierto = !piston2Abierto;

            // Ejecutamos la acción correspondiente
            if (piston2Abierto) {
                RecolectorNeumatica.open();
            } else {
                RecolectorNeumatica.close();
            }
        }

        // Control de los motores de prueba con los botones X y Y
        controlMotoresPrueba();

        // Espera para evitar saturar el CPU
        task::sleep(20);
    }
}
