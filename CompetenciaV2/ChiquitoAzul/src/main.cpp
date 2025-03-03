/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       kenneth_busta                                             */
/*    Created:      2/20/2025, 7:53:55 AM                                     */
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
  /*moveParabolic(42, 80, 50);
  rotateOnAxisN(55, 100);*/

  RecolectorNeumatica.open();
  //wait(15000,msec);
  activarMotorFantasma(100, 10);
  //moveDistance(27.5, -100);
  moveParabolic(39, -100, -85);
  rotateOnAxis(22, -100);
  //moveDistance(15,-100);
  moveParabolic(30, -65, -100);

  recoleccion(100, 2);
  recoleccion(-100, 1);
  moveParabolic(45, 80, 100);

    /* RecolectorNeumatica.open();
    //wait(10000,msec);
    garrita(100, 10);
    //moveDistance(27.5, -100);
    moveParabolic(42.5, -100, -100);
    rotateOnAxis(19, 100);
    //moveDistance(15,-100);
    moveParabolic(30, -100, -80);
    recoleccion(-100, 2);
    recoleccion(100, 2);
    moveParabolic(25, 100, 80);
    moveParabolic(25, 100, 80); */
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
    if (Controller1.ButtonA.pressing()) {
        switchControlMode();
        while (Controller1.ButtonA.pressing()) {
            // Esperar a que se suelte el botón A
            task::sleep(10);
        }
    }
    
    // Control del robot basado en el modo seleccionado
    if (controlMode == 0) {
        twoJoysticksControl(); 
    }
  
    // Control del motor recolector y rampa usando L1 y L2
    if (Controller1.ButtonL1.pressing()) {
        Recolector.spin(directionType::rev, 100, velocityUnits::pct);
        Rampa.spin(directionType::rev, 100, velocityUnits::pct);
        //Recolector2.spin(directionType::rev, 100, velocityUnits::pct);
    } else if (Controller1.ButtonL2.pressing()) {
        Recolector.spin(directionType::fwd, 100, velocityUnits::pct);
        Rampa.spin(directionType::fwd, 100, velocityUnits::pct);
        //Recolector2.spin(directionType::fwd, 100, velocityUnits::pct);
    } else {
        Recolector.stop(brakeType::hold);
        Rampa.stop(brakeType::hold);
        //Recolector2.stop(brakeType::hold);
    }
  
    // Control del motor recolector y rampa usando L1 y L2
    if (Controller1.ButtonDown.pressing()) {
        Escalada.spin(directionType::fwd, 100, velocityUnits::pct);
    } else if (Controller1.ButtonY.pressing()) {
        Escalada.spin(directionType::rev, 100, velocityUnits::pct);
    } else {
        Escalada.stop(brakeType::hold);
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
        // Esperamos a que el botón sea liberado para evitar múltiples activaciones en una sola pulsación
        while(Controller1.ButtonR1.pressing()) {
            // Espera a que el botón se suelte
        }
  
        // Cambiamos el estado del pistón
        pistonAbierto = !pistonAbierto;
              
        // Ejecutamos la acción correspondiente
        if(pistonAbierto) {
            RecolectorNeumatica.open();
        } else {
            RecolectorNeumatica.close();
        }
    }
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
            Brazo.open();
        } else {
            Brazo.close();
        }
    }
  
  
  
    // Espera para evitar saturar el CPU
    wait(20, msec);
  }
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
