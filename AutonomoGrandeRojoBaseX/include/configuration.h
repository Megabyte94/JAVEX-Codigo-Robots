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
// |  [FL1]    [FR2]    |
// |[BL2]         [BR1] |
// |                    |
// |  [FL2]    [FR1]    |
// |  ------------------|
brain Brain;
controller Controller1;

// Motores de la diagonal 1 (Frontal Izquierda a Trasera Derecha)
motor MotorFL1(PORT14, true); 
motor MotorBL2(PORT18, false); 
motor_group Diagonal1(MotorFL1, MotorBL2);

// Motores de la diagonal 2 (Frontal Derecha a Trasera Izquierda)
motor MotorFR2(PORT7, false); 
motor MotorBR1(PORT8, true); 
motor_group Diagonal2(MotorFR2, MotorBR1);

// Motor para el sistema de recolección
motor Rec1(PORT6, true);
motor Rec2(PORT5, false);
motor_group Recolector(Rec1, Rec2);
motor Rampa(PORT12, true);
motor Garra(PORT13, true);

vex::pneumatics Pinza(Brain.ThreeWirePort.A);
vex::pneumatics RecolectorNeumatica(Brain.ThreeWirePort.B);

triport expansor(PORT16);
vex::pneumatics brazo(expansor.C);
