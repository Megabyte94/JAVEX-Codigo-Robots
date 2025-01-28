/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       aulasingenieria                                           */
/*    Created:      10/17/2024, 4:29:46 PM                                    */
/*    Description:  V5 project                                                */
/*                                                                            */

#include "vex.h"  

using namespace vex;

// A global instance of vex::brain used for printing to the V5 brain screen
vex::brain       Brain;

// define your global instances of motors and other devices here
motor MotorI1 = motor(PORT14, ratio18_1, true);
motor MotorI2 = motor(PORT18, ratio18_1, false);
motor MotorI3 = motor(PORT19, ratio18_1, false);
motor MotorI4 = motor(PORT20, ratio18_1, true);
motor MotorD1 = motor(PORT7, ratio18_1, false);
motor MotorD2 = motor(PORT8, ratio18_1, true);
motor MotorD3 = motor(PORT9, ratio18_1, true);
motor MotorD4 = motor(PORT10, ratio18_1, false);
motor_group LeftDrive = motor_group(MotorI1, MotorI2, MotorI3, MotorI4); 
motor_group RightDrive = motor_group(MotorD1, MotorD2, MotorD3, MotorD4); 

motor reco1 = motor(PORT1, ratio6_1, true);
motor reco2 = motor(PORT12, ratio6_1, true);


double speedFactor = 0.5; // Factor de velocidad inicial

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




int main() {

while(true){
 //Moverse durante 3 segundos
  movimiento(50,50,3000);
  //Moverse sobre el eje por 3 segundos hacia la derecha
  turnInPlace(50,3000,false);

  //Moverse durante 3 segundos
  movimiento(50,50,3000);
  
  //Moverse 180 grados hacia la izquierda
  turnToDegrees(-180,50);

  //Moverse durante 3 segundos
  movimiento(50,50,3000);

}

}

/*
void stop() {
  Izq.stop(); 
  Der.stop(); 
}

//Recolectar dona
void recolectarDona(int speed, int duration ) {
  reco1.spin(reverse, speed, percent); 
  reco2.spin(reverse, speed, percent); 
  wait(0.3, seconds);
  stop();
  wait(duration, seconds);
  reco1.stop();
  reco2.stop();
}

void moveForward(int speed, int duration) {
  Izq.spin(forward, speed, percent); 
  Der.spin(forward, speed, percent); 
  wait(duration, seconds);
  stop();
}

// Función para mover el robot hacia atrás
void moveBackward(int speed, int duration) {
  Izq.spin(reverse, speed, percent); 
  Der.spin(reverse, speed, percent); 
  wait(duration, seconds);
  stop();
}

// Función para girar el robot 90 grados a la derecha
void turnRight90(int speed) {
  Izq.spin(forward, speed, percent); 
  Der.spin(reverse, speed, percent); 
  wait(1, seconds);
  //stop();
}

// Función para girar el robot 90 grados a la izquierda
void turnLeft90(int speed) {
  Izq.spin(reverse, speed, percent); 
  Der.spin(forward, speed, percent); 
  wait(1, seconds);
  //stop();
}

// Función para girar el robot 45 grados a la derecha
void turnRight45(int speed) {
  Izq.spin(forward, speed, percent); 
  Der.spin(reverse, speed, percent); 
  wait(0.3, seconds);
  //stop();
}

// Función para girar el robot 45 grados a la izquierda
void turnLeft45(int speed) {
  Izq.spin(reverse, speed, percent); 
  Der.spin(forward, speed, percent); 
  wait(0.3, seconds);
  //stop();
}

// Función para girar el robot 60 grados a la derecha
void turnRight60(int speed) {
  Izq.spin(forward, speed, percent); 
  Der.spin(reverse, speed, percent); 
  wait(0.5, seconds);
  //stop();
}

// Función para girar el robot 60 grados a la derecha
void turnRight70(int speed) {
  Izq.spin(forward, speed, percent); 
  Der.spin(reverse, speed, percent); 
  wait(0.8, seconds);
  //stop();
}

// Función para girar el robot 60 grados a la derecha
void turnLeft70(int speed) {
  Izq.spin(reverse, speed, percent); 
  Der.spin(forward, speed, percent); 
  wait(0.8, seconds);
  //stop();
}

// Función para girar el robot 60 grados a la izquierda
void turnLeft60(int speed) {
  Izq.spin(reverse, speed, percent); 
  Der.spin(forward, speed, percent); 
  wait(0.6, seconds);
  //stop();
}

int main() {
   
    while(true) {
        moveForward(70, 2.2);
        turnRight60(25);
        moveForward(60, 2);
        turnRight90(25);
        turnRight90(25);
        moveForward(70, 2);
        stop();
        break;
        this_thread::sleep_for(10);
    }
}*/