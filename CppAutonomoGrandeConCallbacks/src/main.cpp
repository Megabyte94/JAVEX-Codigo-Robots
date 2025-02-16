/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       kenneth_busta                                             */
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
  // ..........................................................................
  // Insert autonomous user code here.
  // ..........................................................................

  //driveForDistance(24, 50, 5000, Callbacks(), {});

  parabolicMove(24, 100, 50, 5000, Callbacks(), {});

  /* /*PASO 1* 
  //1. Va hacia adelante por la estaca
  moveParabolic(42, 100, 60); 
  //2. Se mueve una distancia adicional para ir por la estaca
  moveDistance(25, 100);
  //3. Gira el robot accionando la estaca para poder quitarla de la posición inicial
  rotateOnAxisN(43, 100);
  //4. Gira una distancia adicional para posicionar la estaca 
  rotateOnAxis(6, 100);
  //PASO 2-3
  RecolectorNeumatica.open();
  //5. Realiza un movimeinto parabolico para poder ir por la estaca de nuestra cancha
  moveParabolicN(39, 90, 100, 100);
  //6. Se realiza un movimiento parabolico adicional dado cado la estaca se enganche al robot
  moveParabolic(35, 90, 100);
  //7. Se realiza un giro sobre su propio eje para poder coger la estaca de nuestra cancha
  rotateOnAxis(40, 100);
  //8. Se recorre una distancia adicional para poder recoger la estaca de manera parabolica 
  moveParabolic(15,-98, -100);
  //9. Se hace un recorrido parabolico para enderezar el robot y coger la estaca 
  moveParabolic(28, -76, -50);
  rotateOnAxis(9.5, -100);
  //10. Se acciona la pinza para coger la estaca 
  Pinza.open(); 
  moveDistance(20, -100);
  //11. Mete la dona de precarga en la estaca 
  recoleccion(100, 1);
  //PASO 4
  //12. Recolecta las donas de al lado de la estaca de la alianza
  RecolectorNeumatica.close();
  moveParabolic(10, 38, 76);
  moveDistanceN(18, 100);
  recoleccion(100, 2); 

    

  //13. Se dirige hacia la esquina 
  rotateOnAxis(20, 100); 
  moveDistanceN(20, 100); 
  //14. Se dirige a recolectar las donas de al frente 
  moveParabolicN(20, 100, 95, 100);
  recoleccion(100, 2);
  //15. Se dirige a tirar las donas de la esquina 
  brazo.close(); 
  moveParabolic(15, 85, 15);
  rotateOnAxis(18, 100); 
  //16. Gira para poder ir a la escalera
  rotateOnAxis(20, 100);  
  moveDistance(25, 100); */
  

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
  //Competition.autonomous(autonomous);
  //Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  //pre_auton();

  autonomous();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}
