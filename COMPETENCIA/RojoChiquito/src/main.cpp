/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       kenneth_busta                                             */
/*    Created:      2/19/2025, 9:07:08 AM                                     */
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
  //Recorrido posicional 
  //NOTA: Este recorrido se realizó con el robot grande ya que el robot pequeño se encuentra en construcción/
  //FASES SEGÚN ESTRATEGIA 1 
  /*Parámetros de cada una de las funciones
  moveDistance(distanciaInches, velocidad);
  moveParabolic(distanciaInches, velocidadIzq, velocidadDer);
  moveParabolicN(distanciaInches, veocidadIzq, velocidadDer, velocidadMecanismos);
  rotateOnAxis(angulo, velocidad);
  moveDistanceN(distanciaInches, velocidadMecanismos);
  turnAngle(angulo, velocidad);
  recoleccion(velocidad, duracion);
  garrita(velocidad, duracion);  
  */
  //1. Avanzar por la estaca de la derecha de la cancha

  moveParabolic(45, 100, 75);
  //rotateOnAxis(4, 100);
  Brazo.open();
  rotateOnAxis(40, 100);

  //-------> ANTERIOR
  /*moveParabolic(51, 90, 75);
  rotateOnAxis(4, 100);
  RecolectorNeumatica.open();
  rotateOnAxis(27, 100);*/

  //moveParabolic(60, -43, -65);
  //RecolectorNeumatica.close();
  /*
  moveParabolic(37, 35, 65);
  moveParabolic(25, 65, 25);
  moveParabolic(20, 25, 80);
  moveParabolic(20, -25, -80);*/
  //moveParabolicN(40, 65, 42,100);
  //recoleccion(100, 2);
  //moveDistance(20, -70);
  //
  //moveDistanceN(20, 100);
  //
  //2. Rota sobre su eje para el paso 2 asumiendo que es el pequeño
  //rotateOnAxis(30,-100);
  /*
  rotateOnAxis(20, -50); 
  //3. Se dirige al centro de las escalera para tomar la estaca asumiendo que Des el pequeño
  moveDistanceN(10, 50); 
  //4. Rota sobre su eje para el paso 3 
  rotateOnAxis(40, 50); 
  //5. Se dirige a las primeras donas para recolectarlas
  moveDistanceN(20, 50);
  moveDistanceN(15, 50);
  moveDistanceN(15, 50); 
  recoleccion(100, 2); 
  //6. Rota un angulo de 20 grados el paso 4
  turnAngle(10, 100); 
  //7. Se dirige a las otros donas para recolectarlas
  moveDistanceN(30, 50); 
  moveDistanceN(10, 50);
  moveDistanceN(10, 50); 
  recoleccion(100, 2); 
  //8. Rota un angulo para el paso 5
  turnAngle(35, -50); 
  //9. Se dirige a las donas de las esquina para recolectarlas
  moveDistanceN(30, 50);
  moveDistanceN(10, 50);
  recoleccion(100, 2);
  //9.1 Se dirige a las donas de la esquina para recolectarlas
  rotateOnAxis(20, 50);
  moveDistanceN(20, 50);
  recoleccion(100, 2);   
  //10. Rota sobre su propio eje para el paso 6
  rotateOnAxis(55, 50); 
  //11. Se dirige a la escalera para escalar el primer nivel
  moveDistance(45,50);
  */

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
        Recolector2.spin(directionType::rev, 100, velocityUnits::pct);
    } else if (Controller1.ButtonL2.pressing()) {
        Recolector.spin(directionType::fwd, 100, velocityUnits::pct);
        Rampa.spin(directionType::fwd, 100, velocityUnits::pct);
        Recolector2.spin(directionType::fwd, 100, velocityUnits::pct);
    } else {
        Recolector.stop(brakeType::hold);
        Rampa.stop(brakeType::hold);
        Recolector2.stop(brakeType::hold);
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
