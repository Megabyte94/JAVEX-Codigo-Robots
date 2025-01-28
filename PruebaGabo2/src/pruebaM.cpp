
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       mtave                                                     */
/*    Created:      10/8/2024, 6:35:38 PM                                     */
/*    Description:  V5 project                                                */
/*                                                                            */

#include "vex.h" 

using namespace vex;

// A global instance of competition
competition Competition;

// define your global instances of motors and other devices here
brain Brain;
controller Controller1;

motor MotorLeft1(PORT1,ratio18_1,true);
motor MotorLeft2(PORT2,ratio18_1,true);
motor MotorLeft3(PORT3,ratio18_1,true);
motor MotorLeft4(PORT4,ratio18_1,true);
motor MotorRight1(PORT5,ratio18_1,false);
motor MotorRight2(PORT6,ratio18_1,false);
motor MotorRight3(PORT7,ratio18_1,false);
motor MotorRight4(PORT8,ratio18_1,false);

motor_group LeftDrive(MotorLeft1, MotorLeft2, MotorLeft3, MotorLeft4);
motor_group RightDrive(MotorRight1, MotorRight2, MotorRight3, MotorRight4);

motor MotorGarra(PORT9,ratio18_1,true);

double speedFactor = 0.5; // Factor de velocidad inicial



/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */


/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */


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

void movimientoGarra(int speed, int time){
  //Establecer velocidad
  MotorGarra.setVelocity(speed,percent);

  //Iniciar movimiento
  MotorGarra.spin(forward);
  wait(time,msec);

  MotorGarra.stop();

  wait(10,msec);
}


void autonomous(void) {

  //Moverse durante 3 segundos
  movimiento(50,50,3000);

  //Mover la garra durante 2 segundos
  movimientoGarra(50,2000);

  //Moverse sobre el eje por 3 segundos hacia la derecha
  turnInPlace(50,3000,false);

  //Moverse durante 3 segundos
  movimiento(50,50,3000);

  //Mover la garra durante 2 segundos
  movimientoGarra(50,2000);
  
  //Moverse 180 grados hacia la izquierda
  turnToDegrees(-180,50);

  //Moverse durante 3 segundos
  movimiento(50,50,3000);


}


/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */


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
}

//
// Main will set up the competition functions and callbacks.
//
int main() {
  // Set up callbacks for autonomous and driver control periods.

 // Competition.drivercontrol(usercontrol);



// Crear hilos para las funciones de ajuste de velocidad y control del robot
  //thread speedThread(adjustSpeedFactor);
 // thread controlThread(controlRobot);

  // Prevent main from exiting with an infinite loop.
  while (true) {
    Competition.autonomous(autonomous); 
    wait(100, msec);
  }
}