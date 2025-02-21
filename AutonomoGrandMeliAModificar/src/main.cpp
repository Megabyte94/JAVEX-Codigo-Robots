

#include "vex.h"

using namespace vex;

const double WHEEL_DIAMETER = 4.0;
const double WHEEL_CIRCUMFERENCE = WHEEL_DIAMETER * M_PI;
const double TRACK_WIDTH = 15.5;
const double RELATIVE_DISTANCE_ERROR = 0.4445;

//  __
// |  [LA1]    [RA1]    |
// |[LB2]         [RB2] |
// |                    |
// |  [LA3]    [RA3]    |
// |[L4B]         [RB4] |
//  --------------------
brain Brain;
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
motor Recolector(PORT15, true);
motor Rampa(PORT12, false);


vex::pneumatics Pinza(Brain.ThreeWirePort.A);
vex::pneumatics RecolectorNeumatica(Brain.ThreeWirePort.B);

triport expansor(PORT16);
vex::pneumatics brazo(expansor.C);

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

void setLeftMotors(double speed) {
    MotorL1.spin(directionType::fwd, speed, velocityUnits::pct);
    MotorL2.spin(directionType::fwd, speed, velocityUnits::pct);
    MotorL3.spin(directionType::fwd, speed, velocityUnits::pct);
    MotorL4.spin(directionType::fwd, speed, velocityUnits::pct);
}

void setRightMotors(double speed) {
    MotorR1.spin(directionType::fwd, speed, velocityUnits::pct);
    MotorR2.spin(directionType::fwd, speed, velocityUnits::pct);
    MotorR3.spin(directionType::fwd, speed, velocityUnits::pct);
    MotorR4.spin(directionType::fwd, speed, velocityUnits::pct);
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
    brazo.close();
}

void moveDistance(double distanceInInches, double speed) {
    resetEncoders();

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
    resetEncoders();

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
    resetEncoders();

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
    resetEncoders();
    double targetRotations = (angleInDegrees / 360) * (TRACK_WIDTH * M_PI / WHEEL_DIAMETER) * 360;

    setLeftMotors((angleInDegrees > 0) ? speed : -speed);
    setRightMotors((angleInDegrees > 0) ? -speed : speed);

    while ( (fabs(MotorL1.position(rotationUnits::deg)) < targetRotations) && (fabs(MotorR1.position(rotationUnits::deg)) < targetRotations) ) {
        task::sleep(10);
    }
    stopAllMotors();
}

void rotateOnAxisN(double angleInDegrees, double speed) {
    resetEncoders();

    double targetRotations = (angleInDegrees / 360) * (TRACK_WIDTH * M_PI / WHEEL_DIAMETER) * 360;

    setLeftMotors((angleInDegrees > 0) ? speed : -speed);
    setRightMotors((angleInDegrees > 0) ? -speed : speed);
    
    brazo.open();

    while ( (fabs(MotorL1.position(rotationUnits::deg)) < targetRotations) && (fabs(MotorR1.position(rotationUnits::deg)) < targetRotations) ) {
        task::sleep(10);
    }
    stopAllMotors();
    
}

void moveDistanceN(double distanceInInches, double speed) {
    resetEncoders();

    Recolector.spin(reverse, speed, percent);
    Rampa.spin(reverse, speed, percent);

    RecolectorNeumatica.close();

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
    resetEncoders();

    brazo.open();
    
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
    resetEncoders();
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



int main() {
    /*PASO 1*/ 
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

    /*

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
    moveDistance(25, 100); 
    1*/
    
    //Termina el recorrido autónomo del robot grande 
    return 0;
}