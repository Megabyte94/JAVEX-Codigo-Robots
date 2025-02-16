#include "vex.h"
#include <vector>
#include <functional>

#pragma once // Para evitar errores de definición múltiple

#include "configuration.h" // Incluimos la configuración de variables

using namespace vex;
using Callbacks = std::vector<std::function<void()>>;


// Funcion para resetear los encoders de los motores
void resetDriveEncoders()
{
    MotorL1.resetPosition();
    MotorL2.resetPosition();
    MotorL3.resetPosition();
    MotorL4.resetPosition();

    MotorR1.resetPosition();
    MotorR2.resetPosition();
    MotorR3.resetPosition();
    MotorR4.resetPosition();

    // -> Solo se resetean los encoders de los motores de la base en este código de prueba
}

// Funcion para mover el robot hacia adelante
bool driveForDistance(double distance, double speed, double timeout, Callbacks callbacks, double callbackBreakpoints[])
{
    bool success = false;
    // Reseteamos los encoders al inicio
    resetDriveEncoders();
    // Crear el temporizador para saber cuanto tiempo ha pasado
    timer t;
    t.clear();
    // Calculamos la distancia en grados
    const double setPoint = distance * DEGREES_PER_INCH;
    // Damos una referencia al motor, para que se mueva a una posicion a una velocidad determinada
    RightMotors.spinToPosition(setPoint, degrees, speed, rpm, false);
    LeftMotors.spinToPosition(setPoint, degrees, speed, rpm, false);

    int currentBreakpoint = 0;

    // CREAMOS UN BUCLE PARA ESPERAR HASTA QUE EL ROBOT LLEGUE A LA DISTANCIA DESEADA O HASTA QUE PASE EL TIMEOUT
    while (LeftMotors.isSpinning() || RightMotors.isSpinning()){   
        // PROTEGEMOS POR TIEMPO
        if(t.time(msec) > timeout) {
            return success;
        }
        // LLAMAMOS A LOS CALLBACKS
        // Si hay callbacks
        if(callbacks.size() > 0 && currentBreakpoint < callbacks.size()) {
            // Vamos a preguntar si ya es momento del primer callback
            if(LeftMotors.position(degrees) >= callbackBreakpoints[currentBreakpoint]) {
                // Llamamos al callback
                callbacks[currentBreakpoint]();
                // Incrementamos el breakpoint
                currentBreakpoint++;
            }
        }
        wait(20, msec);
    }
    success = true;
    return success;
}

// Funcion para girar el robot
bool turnForDegrees(double grados, double speed, double timeout, Callbacks callbacks, double callbackBreakpoints[])
{
    bool success = false;
    // Reseteamos los encoders al inicio
    resetDriveEncoders();
    // Crear el temporizador para saber cuanto tiempo ha pasado
    timer t;
    t.clear();
    
    // Damos una referencia al motor, para que se mueva a una posicion a una velocidad determinada
    LeftMotors.spinToPosition(grados, degrees, speed, rpm);
    RightMotors.spinToPosition(-grados, degrees, speed, rpm);

    int currentBreakpoint = 0;

    // CREAMOS UN BUCLE PARA ESPERAR HASTA QUE EL ROBOT LLEGUE A LA DISTANCIA DESEADA O HASTA QUE PASE EL TIMEOUT
    while (LeftMotors.isSpinning() || RightMotors.isSpinning()){   
        // PROTEGEMOS POR TIEMPO
        if(t.time(msec) > timeout) {
            return success;
        }
        // LLAMAMOS A LOS CALLBACKS
        // Si hay callbacks
        if(callbacks.size() > 0 && currentBreakpoint < callbacks.size()) {
            // Vamos a preguntar si ya es momento del primer callback
            if(LeftMotors.position(degrees) >= callbackBreakpoints[currentBreakpoint]) {
                // Llamamos al callback
                callbacks[currentBreakpoint]();
                // Incrementamos el breakpoint
                currentBreakpoint++;
            }
        }
        wait(20, msec);
    }
    success = true;
    return success;
}

// Funcion para levantar el brazo

/*
// Funcion para la garra
std::function<void()>garra(int speed,double duration) {
    return [speed, duration](){
        Garra.spin(reverse, speed, percent);
    }; 
}
*/

// Funcion para encender la Recoleccion
std::function<void()>recoleccion(int speed,double duration) {
    return [speed, duration](){
        Recolector.spin(reverse, speed, percent);
        Rampa.spin(reverse, speed, percent);
    }; 
}