/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       Melissa Ruiz                                              */
/*    Based on:     David Orozco y Kenneth Bustamente                         */
/*    Created:      11/27/2024, 4:02:42 PM                                    */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "vex.h"

using namespace vex;

// A global instance of vex::brain used for printing to the V5 brain screen
vex::brain       Brain;

// define your global instances of motors and other devices here
const double WHEEL_DIAMETER = 4.0;
const double WHEEL_CIRCUMFERENCE = WHEEL_DIAMETER * M_PI;
const double TRACK_WIDTH = 15.5;
const double RELATIVE_DISTANCE_ERROR = 0.4445;

//  ____
// |  [LA1]    [RA1]    |
// |[LB2]         [RB2] |
// |                    |
// |  [LA3]    [RA3]    |
// |[L4B]         [RB4] |
//  --------------------

controller Controller1;
// Motores del lado izquierdo (puertos 1-4)
motor MotorL1(PORT14, true); 
motor MotorL2(PORT18, false);
motor MotorL3(PORT19, false);
motor MotorL4(PORT20, true);
//motor_group Left(LeftMotor1, LeftMotor2, LeftMotor3, LeftMotor4);

// Motores del lado derecho (puertos 7-10)
motor MotorR1(PORT7, false);
motor MotorR2(PORT8, true);
motor MotorR3(PORT9, true);
motor MotorR4(PORT10, false);
//motor_group Right(RightMotor1, RightMotor2, RightMotor3, RightMotor4);

// Motor para el sistema de recolección
motor Recolector(PORT1 , true);
motor Rampa(PORT12, true);
motor Garra(PORT15, true);

vex::pneumatics Pinza(Brain.ThreeWirePort.A);

//Resetear los encoders de los motores 
void resetEncoders() {
    MotorL1.setPosition(0, rotationUnits::deg);
    MotorL2.setPosition(0, rotationUnits::deg);
    MotorL3.setPosition(0, rotationUnits::deg);
    MotorL4.setPosition(0, rotationUnits::deg);
    MotorR1.setPosition(0, rotationUnits::deg);
    MotorR2.setPosition(0, rotationUnits::deg);
    MotorR3.setPosition(0, rotationUnits::deg);
    MotorR4.setPosition(0, rotationUnits::deg);
}

//Fijar la velocidad de cada uno de los motores del lado izquierdo
void setLeftMotors(double speed) {
    MotorL1.spin(directionType::fwd, speed, velocityUnits::pct);
    MotorL2.spin(directionType::fwd, speed, velocityUnits::pct);
    MotorL3.spin(directionType::fwd, speed, velocityUnits::pct);
    MotorL4.spin(directionType::fwd, speed, velocityUnits::pct);
}

//Fijar la velocidad de cada uno de los motores del lado derecho
void setRightMotors(double speed) {
    MotorR1.spin(directionType::fwd, speed, velocityUnits::pct);
    MotorR2.spin(directionType::fwd, speed, velocityUnits::pct);
    MotorR3.spin(directionType::fwd, speed, velocityUnits::pct);
    MotorR4.spin(directionType::fwd, speed, velocityUnits::pct);
}


//Parar todos los motores 
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
    Garra.stop();
}

//Función para mover una distancia determinada en inches
void moveDistance(double distanceInInches, double speed) {
    //1. reseter encoders 
    resetEncoders();

    //2. calcular la distancia (1 - error)* distancia en inches
    //NOTA: se maneja un rango de error para mayor precisión 
    distanceInInches = (1.0 - RELATIVE_DISTANCE_ERROR) * distanceInInches;

    //3. se tiene que para la rotación del robot para el objetivo se calcula de la siguiente forma
    double targetRotations = (distanceInInches / WHEEL_CIRCUMFERENCE) * 360;

    //4. fijar la velocidad de los motores de acuerdo con los valores por parámetro
    setLeftMotors(speed);
    setRightMotors(speed);

    //5. verificación que los motores alcancen la distancia especificada  
    while (fabs(MotorL1.position(rotationUnits::deg)) < targetRotations &&
           fabs(MotorR1.position(rotationUnits::deg)) < targetRotations) {
        task::sleep(10);
    }
    //6. luego de haber alcanzado la distancia especificada se paran todos los motores
    stopAllMotors();
}

//Función para el movimiento parabólico del robot a cierta distancia
/*NOTA: a diferencia de la función anterior se tiene como parámetro las velocidades de cada uno de los lados 
ya que para hacer el giro parabólico un lado debe inclinarse más que el otro*/
void moveParabolic(double distanceInInches, double speedleft, double speedRight) {
    //1. resetear los encoders de los motores 
    resetEncoders();
    //2. calcular la distancia en inches de acuerdo al rango de error
    distanceInInches = (1.0 - RELATIVE_DISTANCE_ERROR) * distanceInInches;
    //3. calcular la rotación del robot para alcanzar el objetivo con la siguiente fórmula
    double targetRotations = (distanceInInches / WHEEL_CIRCUMFERENCE) * 360;
    //4. fijar la velocidad de los motores por medio de los valores por parámetro
    setLeftMotors(speedleft);
    setRightMotors(speedRight);
    //5. verificación de los motores para alcanzar la distancia del objetivo calculada
    while (fabs(MotorL1.position(rotationUnits::deg)) < targetRotations &&
           fabs(MotorR1.position(rotationUnits::deg)) < targetRotations) {
        task::sleep(10);
    }
    //6. parar todos los motores
    stopAllMotors();
}

//Función para hacer un giro parabólico para la recolección de los aros de acuerdo a donde se encuentran 
void moveParabolicN(double distanceInInches, double speedleft, double speedRight, double speed) {
    //1. resetear los encoders de todos los motores
    resetEncoders();

    //2. accionar los mecanismos de recolección y rampa con la velocidad de los mecanismos 
    Recolector.spin(reverse, speed, percent);
    Rampa.spin(reverse, speed, percent);

    //3. calcular la distancia en inches teniendo en cuenta el rango de error
    distanceInInches = (1.0 - RELATIVE_DISTANCE_ERROR) * distanceInInches;
    //4. calcular la rotación de las ruedas para alcanzar el objetivo con la siguiente fórmula 
    double targetRotations = (distanceInInches / WHEEL_CIRCUMFERENCE) * 360;
    //5. fijar la velocidad de los motores de cada lado de acuerdo a los valores por parámetro
    setLeftMotors(speedleft);
    setRightMotors(speedRight);

    //6. verificación de que los motores alcance la distancia del objetivo
    while (fabs(MotorL1.position(rotationUnits::deg)) < targetRotations &&
           fabs(MotorR1.position(rotationUnits::deg)) < targetRotations) {
        task::sleep(10);
    }
    //7. parar todo los motores
    stopAllMotors();
}

//Función para rotar el robot sobre su propio eje
void rotateOnAxis(double angleInDegrees, double speed) {
    //1. resetear los encoder de los motores de cada uno de los lados 
    resetEncoders();
    //2. calcular la rotación de las ruedas para alcanzar el objetivo por medio de la siguiente fórmula 
    double targetRotations = (angleInDegrees / 360) * (TRACK_WIDTH * M_PI / WHEEL_DIAMETER) * 360;
    //3. fijar velocidad de motores para generar el giro
        //3.1 giro hacia la derecha
        setLeftMotors((angleInDegrees > 0) ? speed : -speed);
        //3.2 giro hacia la izquierda
        setRightMotors((angleInDegrees > 0) ? -speed : speed);
    //4. verificación de que se mantenga el giro hasta alcanzar la distancia calculada 
    while ( (fabs(MotorL1.position(rotationUnits::deg)) < targetRotations) && (fabs(MotorR1.position(rotationUnits::deg)) < targetRotations) ) {
        task::sleep(10);
    }
    //5. para los motores 
    stopAllMotors();
}

//Función para moverse a cierta distancia en inches accionando los mecanismos de recolección 
void moveDistanceN(double distanceInInches, double speed) {
    //1. resetear los encoders de los motores 
    resetEncoders();

    //2. accionar los mecanismos de recolección teniendo en cuenta la velocidad por parámetro 
    Recolector.spin(reverse, speed, percent);
    Rampa.spin(reverse, speed, percent);

    //3. calcular la distancia a recorrer teniendo en cuenta el porcentaje de error 
    distanceInInches = (1.0 - RELATIVE_DISTANCE_ERROR) * distanceInInches;
    
    //4. calcular la rotación de las ruedas para alcanzar el objetivo 
    double targetRotations = (distanceInInches / WHEEL_CIRCUMFERENCE) * 360;

    //5. fijar la velocidad de los motores de acuerdo a los valores por parámetro 
    setLeftMotors(speed);
    setRightMotors(speed);
    //6. verificación de los motores para alcanzar la distancia del objetivo
    while (fabs(MotorL1.position(rotationUnits::deg)) < targetRotations &&
           fabs(MotorR1.position(rotationUnits::deg)) < targetRotations) {
        task::sleep(10);
    }
    //7. para todos los motores
    stopAllMotors();
}

//Función para rotar el robot a cierto ángulo 
void turnAngle(double angleInDegrees, double speed) {
    //1. resetear encoders de todos los motores 
    resetEncoders();

    //2. calcular la rotación de las ruedas para alcanzar el objetivo 
    double targetRotations = (angleInDegrees / 360) * (TRACK_WIDTH * M_PI / WHEEL_DIAMETER) * 360;
    //3. fijar la velocidad de cada uno de los motores para generar el giro 
    //3.1 giro a la derecha
    setLeftMotors((angleInDegrees > 0) ? speed : -speed);
    //3.2 giro a la izquierda
    setRightMotors((angleInDegrees > 0) ? -speed : speed);
    //4. verificación que los motores alcancen la distancia del objetivo
    while ( (fabs(MotorL1.position(rotationUnits::deg)) < targetRotations) && (fabs(MotorR1.position(rotationUnits::deg)) < targetRotations) ) {
        task::sleep(10);
    }
    //5. parar todos los motores
    stopAllMotors();
}

//Función para activar la recolección 
void recoleccion(int speed,double duration) {
  //1. accionar el recolector de acuerdo con el valor por parámetro
  Recolector.spin(reverse, speed, percent);
  //2. accionar rampa de acuerdo con el valor por parámetro
  Rampa.spin(reverse, speed, percent);
  //3. esperar a que termine de acuerdo a la duración 
  wait(duration, seconds);
  //4. parar todos los motores 
  stopAllMotors();
}

//Función para activar la garra del robot 
void garrita(int speed,double duration) {
  //1. accionar la garra del robot de acuerdo a los valores por parámetro
  Garra.spin(reverse, speed, percent);
  //2. esperar a que termine de acuerdo a la duración 
  wait(duration, seconds);
  //3. parar todos los motores
  stopAllMotors();
}

//Recorrido posicional 
/*NOTA: Este recorrido se realizó con el robot grande ya que el robot pequeño se encuentra en construcción*/
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
   int main(){
    //1. Avanzar por la estaca de la derecha de la cancha
    moveDistance(65, 85);
    //2. Rota sobre su eje para el paso 2 asumiendo que es el pequeño
    rotateOnAxis(20, -85); 
    //3. Se dirige al centro de las escalera para tomar la estaca asumiendo que es el pequeño
    moveDistanceN(10, 85); 
    //4. Rota sobre su eje para el paso 3 
    rotateOnAxis(40, 85); 
    //5. Se dirige a las primeras donas para recolectarlas
    moveDistanceN(20, 100);
    moveDistanceN(15, 100);
    moveDistanceN(15, 100); 
    recoleccion(100, 2); 
    //6. Rota un angulo de 20 grados el paso 4
    turnAngle(10, 100); 
    //7. Se dirige a las otros donas para recolectarlas
    moveDistanceN(30, 100); 
    moveDistanceN(10, 100);
    moveDistanceN(10, 100); 
    recoleccion(100, 2); 
    //8. Rota un angulo para el paso 5
    turnAngle(35, -100); 
    //9. Se dirige a las donas de las esquina para recolectarlas
    moveDistanceN(30, 100);
    moveDistanceN(10, 100);
    recoleccion(100, 2);
    //9.1 Se dirige a las donas de la esquina para recolectarlas
    rotateOnAxis(20, 100);
    moveDistanceN(20, 100);
    recoleccion(100, 2);   
    //10. Rota sobre su propio eje para el paso 6
    rotateOnAxis(55, 100); 
    //11. Se dirige a la escalera para escalar el primer nivel
    moveDistance(45,100);
    return 0;
}