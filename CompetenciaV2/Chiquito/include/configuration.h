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

//  ____
// |  [LA1]    [RA1]    |
// |[LB2]         [RB2] |
// |                    |
// |  [LA3]    [RA3]    |
// |[L4B]         [RB4] |
//  --------------------
brain Brain;
controller Controller1;

// Motores del lado izquierdo (puertos 1-4)
motor MotorL1(PORT13, false); 
motor MotorL2(PORT3, true);
motor MotorL3(PORT11, false);
motor MotorL4(PORT12, true);
motor_group Left(MotorL1, MotorL2, MotorL3, MotorL4);

// Motores del lado derecho (puertos 7-10)
motor MotorR1(PORT4, true);
motor MotorR2(PORT5, false);
motor MotorR3(PORT2, true);
motor MotorR4(PORT6, false);
motor_group Right(MotorR1, MotorR2, MotorR3, MotorR4);

// Motor para el sistema de recolección
motor Recolector(PORT1, true);
motor Rampa(PORT2, false);
motor Recolector2(PORT14, false);
motor Escalada1(PORT17, true);
motor Escalada2(PORT19, false);
motor Escalada3(PORT20, true);
motor Escalada4(PORT16, false);
motor_group Escalada(Escalada1, Escalada2, Escalada3, Escalada4);


vex::pneumatics Pinza(Brain.ThreeWirePort.A);
vex::pneumatics RecolectorNeumatica(Brain.ThreeWirePort.B);
vex::pneumatics Brazo(Brain.ThreeWirePort.C);
