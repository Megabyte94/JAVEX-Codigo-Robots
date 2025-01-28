//----------------------------------------------------------------------------/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       Melissa Ruiz                                              */
/*    Created:      14/3/2024, 11:48:05                                       */
/*    Description:  V5 project                                                */
/*                                                                            */
//----------------------------------------------------------------------------/

#include "vex.h"

// Inicialización de objetos del dispositivo VEX
using namespace vex;

// Configuración del cerebro, controlador y motores
brain Brain;
controller Controller1;

// Motores del lado izquierdo (puertos 1-4)
motor LeftMotor1(PORT14, true); 
motor LeftMotor2(PORT18, false);
motor LeftMotor3(PORT19, false);
motor LeftMotor4(PORT20, true);
motor_group LeftDrive(LeftMotor1, LeftMotor2, LeftMotor3, LeftMotor4);

// Motores del lado derecho (puertos 7-10)
motor RightMotor1(PORT7, false);
motor RightMotor2(PORT8, true);
motor RightMotor3(PORT9, true);
motor RightMotor4(PORT10, false);
motor_group RightDrive(RightMotor1, RightMotor2, RightMotor3, RightMotor4);

// Motor para el sistema de recolección
motor MotorRecoleccion(PORT1, false);
motor Rampa(PORT12, false); 


double speedFactor = 0.5; // Factor de velocidad inicial

// Funcion para el movimiento autonomo.
void movimiento(int leftSpeed, int rightSpeed, int timeMs) {
    // Establece la velocidad de los grupos de motores
    LeftDrive.setVelocity(leftSpeed, percent);
    RightDrive.setVelocity(rightSpeed, percent);

    // Inicia el movimiento
    LeftDrive.spin(forward);
    RightDrive.spin(forward);

    // Espera durante el tiempo especificado
    wait(timeMs, msec);

    // Detiene los motores
    LeftDrive.stop();
    RightDrive.stop();

    wait(10,msec);
}

void movimientoRampa(int speed){
    Rampa.setVelocity(speed,percent);
    Rampa.spin(forward);
}

void turnInPlace(int speed, int durationMs, bool turnLeft) {
    if (turnLeft) {
        // Girar a la izquierda: el grupo de motores izquierdo va hacia atrás y el derecho hacia adelante
        LeftDrive.setVelocity(-speed, percent);
        RightDrive.setVelocity(speed, percent);
    } else {
        // Girar a la derecha: el grupo de motores izquierdo va hacia adelante y el derecho hacia atrás
        LeftDrive.setVelocity(speed, percent);
        RightDrive.setVelocity(-speed, percent);
    }

    // Inicia el movimiento
    LeftDrive.spin(forward);
    RightDrive.spin(forward);

    // Espera durante el tiempo especificado en milisegundos
    wait(durationMs, msec);

    // Detiene los motores
    LeftDrive.stop();
    RightDrive.stop();

    wait(10,msec);
}

void turnToDegrees(int degrees, int speed) {
    // Restablecer la posición del motor
    LeftDrive.setPosition(0, vex::rotationUnits::deg);
    RightDrive.setPosition(0, vex::rotationUnits::deg);

    LeftDrive.setVelocity(speed, percent);
    RightDrive.setVelocity(speed, percent);

    if (degrees > 0) {
        // Girar a la derecha
        LeftDrive.spin(forward);
        RightDrive.spin(reverse);
    } else {
        // Girar a la izquierda
        LeftDrive.spin(reverse);
        RightDrive.spin(forward);
    }

    while (abs(LeftDrive.position(vex::rotationUnits::deg)) < abs(degrees) && abs(RightDrive.position(vex::rotationUnits::deg)) < abs(degrees)) {
        wait(10, msec); // Espera un pequeño tiempo para evitar el uso excesivo del CPU
    }

    LeftDrive.stop();
    RightDrive.stop();

    wait(10,msec);
}

void movimientoRecoleccion(int speed){
  //Establecer velocidad
  MotorRecoleccion.setVelocity(speed,percent);

  //Iniciar movimiento
  MotorRecoleccion.spin(forward);
}

void motoresAgarre(int speed, int time){
    movimientoRampa(50);
    movimientoRecoleccion(50);
    wait(time,msec);
    Rampa.stop();
    MotorRecoleccion.stop();


}

void autonomous(void) {


  movimiento(50,50,1000);

  //Mover la garra durante 2 segundos
  //movimientoGarra(50,2000);

  
  turnInPlace(50,1000,false);

  
  movimiento(50,50,1000);

  
  //Moverse 180 grados hacia la izquierda
  //turnToDegrees(-180,50);

  


}


// Función para ajustar el factor de velocidad
void adjustSpeedFactor() {
    while (true) {
        // Aumentar la velocidad cuando se presiona el botón Up
        if (Controller1.ButtonUp.pressing()) {
            speedFactor += 0.1;
            if (speedFactor > 1.0) speedFactor = 1.0; // Limitar al 100%
            wait(300, msec); // Debounce para evitar múltiples incrementos rápidos
        }

        // Reducir la velocidad cuando se presiona el botón Down
        if (Controller1.ButtonDown.pressing()) {
            speedFactor -= 0.1;
            if (speedFactor < 0.1) speedFactor = 0.1; // Limitar al 10%
            wait(300, msec); // Debounce para evitar múltiples decrementos rápidos
        }

        wait(20, msec); // Esperar un breve periodo antes de volver a comprobar
    }
}
/*
void controlRobot() {
    while (true) {
        // Obtener el valor del joystick del controlador y ajustar con el factor de velocidad
        int leftSpeed = Controller1.Axis3.position() * speedFactor;
        int rightSpeed = Controller1.Axis2.position() * speedFactor;

        // Configurar la velocidad de los grupos de motores en función del joystick
        LeftDrive.setVelocity(leftSpeed, percent);
        RightDrive.setVelocity(rightSpeed, percent);

        // Mover los grupos de motores
        LeftDrive.spin(forward);
        RightDrive.spin(forward);

        wait(20, msec); // Esperar un breve periodo para prevenir sobrecargar el procesador
    }
}*/
int main() {
    movimiento(60,60,1000);
    motoresAgarre(50,3000);
   // turnInPlace(90,500,true); //Giro 180

    while (true) {
         
    }
    wait(20,msec);
}


