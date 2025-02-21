/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       kenneth_busta                                             */
/*    Created:      2/19/2025, 7:49:14 AM                                     */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"
using namespace vex;

// Librerías propias
#include "configuration.h"
#include "funciones_posicionales.h"
#include "driver.h"

// A global instance of competition
competition Competition;

// Librerías propias
// define your global instances of motors and other devices here

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton(void) {

  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void autonomous(void) {
    // Autonomo ROBOT GRANDE (ALIANZA AZUL) nacional
    
    moveParabolic(42, 100, 55); 
    //2. Se mueve una distancia adicional para ir por la estaca
    moveDistance(25, 100);
    //3. Gira el robot accionando la estaca para poder quitarla de la posición inicial
    rotateOnAxisN(54, 100);
    //4. Gira una distancia adicional para posicionar la estaca 
    rotateOnAxis(6, 100);
    //PASO 2-3
    RecolectorNeumatica.open();
    //5. Realiza un movimeinto parabolico para poder ir por la estaca de nuestra cancha
    moveParabolicN(39, 100, 90, 100);
    //6. Se realiza un movimiento parabolico adicional dado cado la estaca se enganche al robot
    moveParabolic(35, 100, 90);
    //7. Se realiza un giro sobre su propio eje para poder coger la estaca de nuestra cancha
    rotateOnAxis(40, -100);
    //8. Se recorre una distancia adicional para poder recoger la estaca de manera parabolica 
    moveParabolic(15, 100, 98);
    //9. Se hace un recorrido parabolico para enderezar el robot y coger la estaca 
    moveParabolic(28, -76, -50);
    rotateOnAxis(25, 100);
    //10. Se acciona la pinza para coger la estaca 
    Pinza.open(); 
    moveDistance(30, -100);
    //11. Mete la dona de precarga en la estaca 
    recoleccion(100, 2);
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void usercontrol(void) {
  // User control code here, inside the loop
  while (true) {
    // Cambiar el modo de control con el botón A
    if (Controller1.ButtonY.pressing()) {
        switchControlMode();
        while (Controller1.ButtonA.pressing()) {
            // Esperar a que se suelte el botón A
            task::sleep(10);
        }
    }

    // Control del robot basado en el modo seleccionado
    if (controlMode == 0) {
        twoJoysticksControl();
    }else if(controlMode == 1){
        joystickNewControl(); 
    }

    // Control del motor recolector y rampa usando L1 y L2
    if (Controller1.ButtonL1.pressing()) {
        Recolector.spin(directionType::fwd, 100, velocityUnits::pct);
        Rampa.spin(directionType::fwd, 100, velocityUnits::pct);
    } else if (Controller1.ButtonL2.pressing()) {
        Recolector.spin(directionType::rev, 100, velocityUnits::pct);
        Rampa.spin(directionType::rev, 100, velocityUnits::pct);
    } else {
        Recolector.stop(brakeType::hold);
        Rampa.stop(brakeType::hold);
    }

    // Control del motor recolector y rampa usando L1 y L2
    if (Controller1.ButtonX.pressing()) {
        Garra.spin(directionType::fwd, 100, velocityUnits::pct);
    } else if (Controller1.ButtonA.pressing()) {
        Garra.spin(directionType::rev, 100, velocityUnits::pct);
    } else {
        Garra.stop(brakeType::hold);
    }

    if(Controller1.ButtonR2.pressing())
    {
        // Esperamos a que el botón sea liberado para evitar múltiples activaciones en una sola pulsación
        while(Controller1.ButtonR2.pressing()) {
            // Espera a que el botón se suelte
        }

        // Cambiamos el estado del pistón
        pistonAbierto = !pistonAbierto;
        
        // Ejecutamos la acción correspondiente
        if(pistonAbierto) {
            Pinza.open();
        } else {
            Pinza.close();
        }
    }

    if(Controller1.ButtonR1.pressing())
    {
        RecolectorNeumatica.open();
    }else{RecolectorNeumatica.close();}

    if(Controller1.ButtonB.pressing())
    {
        // Esperamos a que el botón sea liberado para evitar múltiples activaciones en una sola pulsación
        while(Controller1.ButtonB.pressing()) {
            // Espera a que el botón se suelte
        }

        // Cambiamos el estado del pistón
        pistonAbierto = !pistonAbierto;
        
        // Ejecutamos la acción correspondiente
        if(pistonAbierto) {
            brazo.open();
        } else {
            brazo.close();
        }
    }
    }   


    // Espera para evitar saturar el CPU
    wait(20, msec);
}

//
// Main will set up the competition functions and callbacks.
//
int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}
