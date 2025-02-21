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

//---------------- FUNCIONES DE MOVIMIENTO ----------------

// Funcion para encender la Recoleccion (sin tiempo)
std::function<void()>recoleccion(int speed/*, double duration*/) {
    return [speed](){
        Recolector.spin(reverse, speed, percent);
        Rampa.spin(reverse, speed, percent);
        Brain.Screen.print("Recolector encendido");
    }; 
}
// Funcion para encender la Recoleccion (con tiempo)
void recoleccionTemporizada(int speed, double duration) {
    Recolector.spin(reverse, speed, percent);
    Rampa.spin(reverse, speed, percent);
    wait(duration, seconds);
    Recolector.stop();
    Rampa.stop();
    Brain.Screen.print("Recolector apagado");
}

/*
// Funcion para la garra
std::function<void()>garra(int speed,double duration) {
    return [speed, duration](){
        Garra.spin(reverse, speed, percent);
    }; 
}
*/

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
void stopAllMotors() {
    MotorL1.stop();
    MotorL2.stop();
    MotorL3.stop();
    MotorL4.stop();
    MotorR1.stop();
    MotorR2.stop();
    MotorR3.stop();
    MotorR4.stop();
    Recolector.stop();
    Rampa.stop();
    //Brazo.close();
}

// Funcion para mover el robot una distancia
void moveDistanceVIEJO(double distanceInInches, double speed, bool activarRecoleccion, std::string movimientoBrazo) {
    resetDriveEncoders();

    if (activarRecoleccion) {
        Recolector.spin(reverse, 100, percent);
        Rampa.spin(reverse, 100, percent);
    }
    if (movimientoBrazo == "openArm") {
        Brazo.open();
    }
    else if (movimientoBrazo == "closeArm") {
        Brazo.close();
    }

    NeumaticaRecolector.close();

    distanceInInches = (1.0 - RELATIVE_DISTANCE_ERROR) * distanceInInches;

    double targetRotations = (distanceInInches / WHEEL_CIRCUMFERENCE) * 360;

    LeftMotors.spin(forward, speed, percent);
    RightMotors.spin(forward, speed, percent);

    while (fabs(MotorL1.position(rotationUnits::deg)) < targetRotations &&
           fabs(MotorR1.position(rotationUnits::deg)) < targetRotations) {
        task::sleep(10);
    }
    stopAllMotors();
}

// Funcion para mover el robot de forma parabólica
void moveParabolicVIEJO(double distanceInInches, double speedleft, double speedRight, bool activarRecoleccion, std::string movimientoBrazo) {
    resetDriveEncoders();

    distanceInInches = (1.0 - RELATIVE_DISTANCE_ERROR) * distanceInInches;

    double targetRotations = (distanceInInches / WHEEL_CIRCUMFERENCE) * 360;

    LeftMotors.spin(forward, speedleft, percent);
    RightMotors.spin(forward, speedRight, percent);

    // Activar recolección si es necesario
    if (activarRecoleccion) {
        Recolector.spin(reverse, 100, percent);
        Rampa.spin(reverse, 100, percent);
    }

    if (movimientoBrazo == "openArm") {
        Brazo.open();
    }
    else if (movimientoBrazo == "closeArm") {
        Brazo.close();
    }

    while (fabs(MotorL1.position(rotationUnits::deg)) < targetRotations &&
           fabs(MotorR1.position(rotationUnits::deg)) < targetRotations) {
        task::sleep(10);
    }
    stopAllMotors();
}

// Haz una función cómo la de turnForDegrees pero que se llame turnForAngle y que no tenga timeout ni callbacks
// EN RPM
void turnForAngle(double angulo, double speed, bool activarRecolección, std::string movimientoBrazo) {
    // Reseteamos los encoders al inicio
    resetDriveEncoders();

    // Activar recolección si es necesario
    if (activarRecolección) {
        Recolector.spin(reverse, 100, percent);
        Rampa.spin(reverse, 100, percent);
    }

    if (movimientoBrazo == "openArm") {
        Brazo.open();
    }
    else if (movimientoBrazo == "closeArm") {
        Brazo.close();
    }

    const double distancia = (ROBOT_CIRCUMFERENCE_LENGTH / 360) * angulo;
    
    // Calcular la distancia en grados de rotación (convertimos la distancia a grados)
    const double grados = distancia * DEGREES_PER_INCH;

    // Damos una referencia al motor, para que se mueva a una posición a una velocidad determinada
    LeftMotors.spinToPosition(grados, degrees, speed, rpm, false);
    RightMotors.spinToPosition(-grados, degrees, speed, rpm);

    waitUntil(!LeftMotors.isSpinning() && !RightMotors.isSpinning());
}

// FUNCIONES NÉSTOR (NO USADAS):

// Funcion para mover el robot hacia adelante
bool driveForDistance(double distance, double speed, double timeout, bool activarRecoleccion, Callbacks callbacks, double callbackBreakpoints[]) {
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

    // Activar recolección si es necesario
    if (activarRecoleccion) {
        recoleccion(100);
    }

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

// Función para girar el robot en su propio eje usando la conversión de distancia a grados de rotación
bool turnForDegrees(double angulo, double speed, double timeout, bool activarRecolección, std::string movimientoBrazo, Callbacks callbacks, double callbackBreakpoints[]) {
    bool success = false;

    // Reseteamos los encoders al inicio
    resetDriveEncoders();

    // Crear el temporizador para saber cuánto tiempo ha pasado
    timer t;
    t.clear();

    // Activar recolección si es necesario
    if (activarRecolección) {
        Recolector.spin(reverse, 100, percent);
        Rampa.spin(reverse, 100, percent);
    }

    if (movimientoBrazo == "openArm") {
        Brazo.open();
    }
    else if (movimientoBrazo == "closeArm") {
        Brazo.close();
    }

    const double distancia = (ROBOT_CIRCUMFERENCE_LENGTH / 360) * angulo;
    
    // Calcular la distancia en grados de rotación (convertimos la distancia a grados)
    const double grados = distancia * DEGREES_PER_INCH;

    // Damos una referencia al motor, para que se mueva a una posición a una velocidad determinada
    LeftMotors.spinToPosition(grados, degrees, speed, rpm, false);
    RightMotors.spinToPosition(-grados, degrees, speed, rpm);

    int currentBreakpoint = 0;
    
    wait(10, msec);

    /* Brain.Screen.print("Prueba 1");
    printf("Prueba 1");
    cout << "Prueba 1" << endl;
    wait(10, msec); */

    cout << "LeftMotors spinning: " << LeftMotors.isSpinning() << endl;
    cout << "RightMotors isDone: " << RightMotors.isDone() << endl;


    // Creamos un bucle para esperar hasta que el robot llegue a la distancia deseada o hasta que pase el timeout
    while (LeftMotors.isSpinning() || RightMotors.isDone()) {
        /* Brain.Screen.print("Prueba 2");
        printf("Prueba 2");
        cout << "Prueba 2" << endl; */

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
    /* Brain.Screen.print("Callback %d ejecutado", currentBreakpoint);
    printf("Callback %d ejecutado", currentBreakpoint);
    cout << "Callback " << currentBreakpoint << " ejecutado" << endl; */
    
    success = true;
    return success;
}

// NO SIRVE
/* bool parabolicMove(double distance, double speedX, double speedY, double timeout, Callbacks callbacks, double callbackBreakpoints[]) {
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
} */

// -> CÓDIGO ANTERIOR

/* void stopAllMotors() {
    MotorL1.stop();
    MotorL2.stop();
    MotorL3.stop();
    MotorL4.stop();
    MotorR1.stop();
    MotorR2.stop();
    MotorR3.stop();
    MotorR4.stop();
    Recolector.stop();
    Rampa.stop();
    Brazo.close();
}

void moveDistance(double distanceInInches, double speed) {
    resetDriveEncoders();

    distanceInInches = (1.0 - RELATIVE_DISTANCE_ERROR) * distanceInInches;

    double targetRotations = (distanceInInches / WHEEL_CIRCUMFERENCE) * 360;

    setLeftMotors(speed);
    setRightMotors(speed);

    while (fabs(MotorL1.position(rotationUnits::deg)) < targetRotations &&
           fabs(MotorR1.position(rotationUnits::deg)) < targetRotations) {
        task::sleep(10);
    }
    stopAllMotors();
}

void moveParabolic(double distanceInInches, double speedleft, double speedRight) {
    resetDriveEncoders();

    distanceInInches = (1.0 - RELATIVE_DISTANCE_ERROR) * distanceInInches;

    double targetRotations = (distanceInInches / WHEEL_CIRCUMFERENCE) * 360;

    
    setLeftMotors(speedleft);
    setRightMotors(speedRight);

    while (fabs(MotorL1.position(rotationUnits::deg)) < targetRotations &&
           fabs(MotorR1.position(rotationUnits::deg)) < targetRotations) {
        task::sleep(10);
    }
    stopAllMotors();
}

void moveParabolicN(double distanceInInches, double speedleft, double speedRight, double speed) {
    resetDriveEncoders();

    Recolector.spin(reverse, speed, percent);
    Rampa.spin(reverse, speed, percent);

    distanceInInches = (1.0 - RELATIVE_DISTANCE_ERROR) * distanceInInches;

    double targetRotations = (distanceInInches / WHEEL_CIRCUMFERENCE) * 360;

    
    setLeftMotors(speedleft);
    setRightMotors(speedRight);

    while (fabs(MotorL1.position(rotationUnits::deg)) < targetRotations &&
           fabs(MotorR1.position(rotationUnits::deg)) < targetRotations) {
        task::sleep(10);
    }
    stopAllMotors();
}

void rotateOnAxis(double angleInDegrees, double speed) {
    resetDriveEncoders();
    double targetRotations = (angleInDegrees / 360) * (TRACK_WIDTH * M_PI / WHEEL_DIAMETER) * 360;

    setLeftMotors((angleInDegrees > 0) ? speed : -speed);
    setRightMotors((angleInDegrees > 0) ? -speed : speed);

    while ( (fabs(MotorL1.position(rotationUnits::deg)) < targetRotations) && (fabs(MotorR1.position(rotationUnits::deg)) < targetRotations) ) {
        task::sleep(10);
    }
    stopAllMotors();
}

void rotateOnAxisN(double angleInDegrees, double speed) {
    resetDriveEncoders();

    double targetRotations = (angleInDegrees / 360) * (TRACK_WIDTH * M_PI / WHEEL_DIAMETER) * 360;

    setLeftMotors((angleInDegrees > 0) ? speed : -speed);
    setRightMotors((angleInDegrees > 0) ? -speed : speed);
    
    Brazo.open();

    while ( (fabs(MotorL1.position(rotationUnits::deg)) < targetRotations) && (fabs(MotorR1.position(rotationUnits::deg)) < targetRotations) ) {
        task::sleep(10);
    }
    stopAllMotors();
    
}

void moveDistanceN(double distanceInInches, double speed) {
    resetDriveEncoders();

    Recolector.spin(reverse, speed, percent);
    Rampa.spin(reverse, speed, percent);

    NeumaticaRecolector.close();

    distanceInInches = (1.0 - RELATIVE_DISTANCE_ERROR) * distanceInInches;

    double targetRotations = (distanceInInches / WHEEL_CIRCUMFERENCE) * 360;

    setLeftMotors(speed);
    setRightMotors(speed);

    while (fabs(MotorL1.position(rotationUnits::deg)) < targetRotations &&
           fabs(MotorR1.position(rotationUnits::deg)) < targetRotations) {
        task::sleep(10);
    }
    stopAllMotors();

    
}

void moveDistanceB(double distanceInInches, double speed) {
    resetDriveEncoders();

    Brazo.open();
    
    distanceInInches = (1.0 - RELATIVE_DISTANCE_ERROR) * distanceInInches;

    double targetRotations = (distanceInInches / WHEEL_CIRCUMFERENCE) * 360;

    setLeftMotors(speed);
    setRightMotors(speed);

    while (fabs(MotorL1.position(rotationUnits::deg)) < targetRotations &&
           fabs(MotorR1.position(rotationUnits::deg)) < targetRotations) {
        task::sleep(10);
    }
    stopAllMotors();

    
}

void turnAngle(double angleInDegrees, double speed) {
    resetDriveEncoders();
    double targetRotations = (angleInDegrees / 360) * (TRACK_WIDTH * M_PI / WHEEL_DIAMETER) * 360;

    setLeftMotors((angleInDegrees > 0) ? speed : -speed);
    setRightMotors((angleInDegrees > 0) ? -speed : speed);

    while ( (fabs(MotorL1.position(rotationUnits::deg)) < targetRotations) && (fabs(MotorR1.position(rotationUnits::deg)) < targetRotations) ) {
        task::sleep(10);
    }
    stopAllMotors();
}

void recoleccion(int speed,double duration) {
  Recolector.spin(reverse, speed, percent);
  Rampa.spin(reverse, speed, percent);
  wait(duration, seconds);
  stopAllMotors();
}

void garrita(int speed,double duration) {
  Garra.spin(reverse, speed, percent);
  wait(duration, seconds);
  stopAllMotors();
}
*/