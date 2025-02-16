#include "vex.h"
#include <vector>
#include <functional>
#include <iostream>

#pragma once // Para evitar errores de definición múltiple

#include "configuration.h" // Incluimos la configuración de variables

using namespace vex;
using Callbacks = std::vector<std::function<void()>>;

using std::cout;
using std::endl;


// Funcion para resetear los encoders de los motores
void resetDriveEncoders() {
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
bool driveForDistance(double distance, double speed, double timeout, Callbacks callbacks, double callbackBreakpoints[]) {
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
// Función para girar el robot en su propio eje usando la conversión de distancia a grados de rotación
bool turnForDegrees(double angulo, double speed, double timeout, Callbacks callbacks, double callbackBreakpoints[]) {
    bool success = false;

    // Reseteamos los encoders al inicio
    resetDriveEncoders();

    // Crear el temporizador para saber cuánto tiempo ha pasado
    timer t;
    t.clear();

    const double distancia = (ROBOT_CIRCUMFERENCE_LENGTH / 360) * angulo;
    
    // Calcular la distancia en grados de rotación (convertimos la distancia a grados)
    const double grados = distancia * DEGREES_PER_INCH;

    // Damos una referencia al motor, para que se mueva a una posición a una velocidad determinada
    LeftMotors.spinToPosition(grados, degrees, speed, rpm, false);
    RightMotors.spinToPosition(-grados, degrees, speed, rpm);

    int currentBreakpoint = 0;
    
    wait(10, msec);

    Brain.Screen.print("Prueba 1");
    printf("Prueba 1");
    cout << "Prueba 1" << endl;
    wait(10, msec);

    cout << "LeftMotors spinning: " << LeftMotors.isSpinning() << endl;
    cout << "RightMotors isDone: " << RightMotors.isDone() << endl;


    // Creamos un bucle para esperar hasta que el robot llegue a la distancia deseada o hasta que pase el timeout
    while (LeftMotors.isSpinning() || RightMotors.isDone()) {
        Brain.Screen.print("Prueba 2");
        printf("Prueba 2");
        cout << "Prueba 2" << endl;

        // Protegemos por tiempo
        if(t.time(msec) > timeout) {
            return success;
        }

        // Llamamos a los callbacks
        if(callbacks.size() > 0 && currentBreakpoint < callbacks.size()) {
            if(LeftMotors.position(degrees) >= callbackBreakpoints[currentBreakpoint]) {
                callbacks[currentBreakpoint]();
                currentBreakpoint++;
            }
        }

        wait(20, msec);  // Esperar un poco antes de comprobar nuevamente
    }
    wait(10, msec);
    Brain.Screen.print("Callback %d ejecutado", currentBreakpoint);
    printf("Callback %d ejecutado", currentBreakpoint);
    cout << "Callback " << currentBreakpoint << " ejecutado" << endl;
    
    success = true;
    return success;
}


bool parabolicMove(double distance, double speedX, double speedY, double timeout, Callbacks callbacks, double callbackBreakpoints[]) {
    bool success = false;
    resetDriveEncoders();
    timer t;
    t.clear();

    // Calculate setpoint for forward movement
    const double setPoint = distance * DEGREES_PER_INCH;
    const double midPoint = setPoint / 2;

    int currentBreakpoint = 0;

    while (true) {
        if(t.time(msec) > timeout) {
            return success;
        }

        double currentPosition = LeftMotors.position(degrees);
        if(currentPosition >= setPoint) break;

        // Calculate parabolic speed adjustment
        double x = currentPosition - midPoint;
        double speedAdjustment = -(speedY * (x * x) / (midPoint * midPoint)) + speedY;
        double currentSpeed = speedX + speedAdjustment;

        RightMotors.spin(forward, currentSpeed, rpm);
        LeftMotors.spin(forward, currentSpeed, rpm);

        if(callbacks.size() > 0 && currentBreakpoint < callbacks.size()) {
            if(currentPosition >= callbackBreakpoints[currentBreakpoint]) {
                callbacks[currentBreakpoint]();
                currentBreakpoint++;
            }
        }
        wait(20, msec);
    }

    RightMotors.stop();
    LeftMotors.stop();
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
std::function<void()>recoleccion(int speed, double duration) {
    return [speed, duration](){
        Recolector.spin(reverse, speed, percent);
        Rampa.spin(reverse, speed, percent);
        Brain.Screen.print("Recolector encendido");
    }; 
}