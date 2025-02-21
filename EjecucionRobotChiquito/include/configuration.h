/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       Kenneth Bustamante                                        */
/*    Created:      14/3/2024, 11:48:05                                       */
/*    Description:  V5 project                                                */
/*                                                                            */

#include "vex.h"
#include <vector>
#include <functional>

#pragma once // Para evitar errores de definición múltiple

using namespace vex;
using Callbacks = std::vector<std::function<void()>>; // Callbacks

/* const double WHEEL_DIAMETER = 4.0;
const double WHEEL_CIRCUMFERENCE = WHEEL_DIAMETER * M_PI;
const double TRACK_WIDTH = 15.5;
const double RELATIVE_DISTANCE_ERROR = 0.4445; */

// VARIABLES DE NAVEGACIÓN DE NÉSTOR
const double WHEEL_DIAMETER = 4.0;
const double WHEEL_BASE = 12.0;
const double WHEEL_CIRCUMFERENCE = 3.14159 * WHEEL_DIAMETER;
const double DEGREES_PER_INCH = 360 / WHEEL_CIRCUMFERENCE;
const double ROBOT_CIRCUMFERENCE_LENGTH = 13*M_PI;
const double RELATIVE_DISTANCE_ERROR = 0.4445; // -> Importado de código viejo

//  ____
// |  [LA1]    [RA1]    |
// |[LB2]         [RB2] |
// |                    |
// |  [LA3]    [RA3]    |
// |[L4B]         [RB4] |
//  --------------------
brain Brain;
controller Controller1;


// Motores del lado izquierdo (puertos 11-14)
motor MotorL1 = motor(PORT11, ratio18_1, false);
motor MotorL2 = motor(PORT12, ratio18_1, false);
motor MotorL3 = motor(PORT13, ratio18_1, false);
motor MotorL4 = motor(PORT14, ratio18_1, false);
motor_group LeftMotors(MotorL1, MotorL2, MotorL3, MotorL4);

// Motores del lado derecho (puertos 3-6)
motor MotorR1 = motor(PORT3, ratio18_1, true);
motor MotorR2 = motor(PORT4, ratio18_1, true); 
motor MotorR3 = motor(PORT5, ratio18_1, true);
motor MotorR4 = motor(PORT6, ratio18_1, true);
motor_group RightMotors(MotorR1, MotorR2, MotorR3, MotorR4);

// Motores del sistema de colgado
motor MotorColgado1(PORT16, true);
motor MotorColgado2(PORT17, false);
motor MotorColgado3(PORT18, true);
motor MotorColgado4(PORT19, false);
motor_group MotoresColgado(MotorColgado1, MotorColgado2, MotorColgado3, MotorColgado4);

// Motor para el sistema de recolección
motor Recolector(PORT1, true);
motor Rampa(PORT2, true);
//motor Garra(PORT1, true);

// Otras conexiones
vex::pneumatics Pinza(Brain.ThreeWirePort.A);
vex::pneumatics NeumaticaRecolector(Brain.ThreeWirePort.B);

triport Expansor(PORT16);
vex::pneumatics Brazo(Expansor.C);







// -> PARTE DEL CÓDIGO DE NÉSTOR

/* brain Brain;

motor leftMotor = motor(PORT1, ratio18_1, false);
motor rightMotor = motor(PORT10, ratio18_1, true);

motor leftLift = motor(PORT2, ratio18_1, false);
motor rightLift = motor(PORT9, ratio18_1, true);

digital_out intake = digital_out(Brain.ThreeWirePort.A);

motor_group lift = motor_group(leftLift, rightLift); */

/* VARIABLES DE NAVEGACION */


/* FUNCIONES DE MOVIMIENTO */

// Funcion para resetear los encoders de los motores
/* void resetDriveEncoders()
{
    leftMotor.resetPosition();
    rightMotor.resetPosition();
}



// Funcion para levantar el brazo
void liftForDegrees(double degrees, double speed)
{
    lift.rotateFor(degrees, degrees, speed, rpm);
}

// Funcion para encender el intake
void intakeOn()
{
    intake.set(true);
}
// Funcion para apagar el intake
void intakeOff()
{
    intake.set(false);
} */