#include "vex.h"
#include <vector>
#include <functional>

#pragma once // Para evitar errores de definición múltiple

using namespace vex;
using Callbacks = std::vector<std::function<void()>>; // Callbacks


const double WHEEL_DIAMETER = 4.0;
const double WHEEL_CIRCUMFERENCE = WHEEL_DIAMETER * M_PI;
const double TRACK_WIDTH = 15.5;
const double RELATIVE_DISTANCE_ERROR = 0.4445;

//  __
// |  [LA1]    [RA1]    |
// |[LB2]         [RB2] |
// |                    |
// |  [LA3]    [RA3]    |
// |[L4B]         [RB4] |
//  --------------------
brain Brain;
controller Controller1;


// Motores del lado izquierdo (puertos 1-4)
motor MotorL1(PORT14, true); 
motor MotorL2(PORT18, false);
motor MotorL3(PORT19, false);
motor MotorL4(PORT20, true);
//motor_group Left(LeftMotor1, LeftMotor2, LeftMotor3, LeftMotor4);

// Motores del lado derecho (puertos 7-10)
motor MotorR1(PORT7, false);
motor MotorR2(PORT8, true);
motor MotorR3(PORT9, true);
motor MotorR4(PORT10, false);
//motor_group Right(RightMotor1, RightMotor2, RightMotor3, RightMotor4);

// Motor para el sistema de recolección
motor Recolector(PORT15, true);
motor Rampa(PORT12, false);


vex::pneumatics Pinza(Brain.ThreeWirePort.A);
vex::pneumatics RecolectorNeumatica(Brain.ThreeWirePort.B);

triport expansor(PORT16);
vex::pneumatics brazo(expansor.C);