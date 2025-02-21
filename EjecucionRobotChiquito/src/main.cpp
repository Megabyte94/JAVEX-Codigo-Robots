/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       Kenneth Bustamante                                        */
/*    Created:      2/15/2025, 3:38:02 PM                                     */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"
using namespace vex;

// Librerías propias
#include "configuration.h"
#include "funciones_posicionales.h"
#include "driver.h"
#include "autonomousCode.h"

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
  // Ir en movimiento parabólico hacia adelante por la estaca, girando un poco hacia la derecha
  moveParabolicVIEJO(37, 100, 55, false, "noArm");
  //moveParabolicVIEJO(42, 100, 55, false, "noArm");

  // Desplazar una distancia adicional para ir por la estaca, bajando el Brazo
  moveDistanceVIEJO(27, 100, false, "noArm");
  //moveDistanceVIEJO(30, 100, false, "noArm");

  // Activar el Brazo e instantáneamente girar hacia la derecha para tomar la estaca de nuestro lado
  turnForAngle(70, 100, false, "openArm");

  //-----> FIN PARTE "SEGURA" / Intentar escalar

  // Movimiento parabólico en reversa antes de escalar
  moveParabolicVIEJO(60, -100, -55, false, "closeArm");

  // Giro 90° hacia la izquierda para escalar
  turnForAngle(-90, 100, false, "noArm");

  // Mover un poco hacia adelante para escalar
  moveDistanceVIEJO(10, 100, false, "noArm");

  // Esperar 23 segundos para escalar
  wait(22, sec);

  // Escalar
  MotoresColgado.spin(reverse, 100, percent);
  wait(6, sec);
  MotoresColgado.stop();

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
  while (1) {
    // This is the main execution loop for the user control program.
    // Each time through the loop your program should update motor + servo
    // values based on feedback from the joysticks.

    // ........................................................................
    // Insert user code here. This is where you use the joystick values to
    // update your motors, etc.

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
      } else if (controlMode == 1) {
          singleJoystickControl();
      } else if (controlMode == 2) {
          arrowControl();
      }else if(controlMode == 3){
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

      if(Controller1.ButtonR1.pressing())
      {
          // Esperamos a que el botón sea liberado para evitar múltiples activaciones en una sola pulsación
          while(Controller1.ButtonR1.pressing()) {
              // Espera a que el botón se suelte
          }

          // Cambiamos el estado del pistón
          piston2Abierto = !piston2Abierto;
          
          // Ejecutamos la acción correspondiente
          if(piston2Abierto) {
              NeumaticaRecolector.open();
          } else {
              NeumaticaRecolector.close();
          }
      }



      // Espera para evitar saturar el CPU
      task::sleep(20);
  }

    // ........................................................................

    wait(20, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
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

  //autonomous();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}
