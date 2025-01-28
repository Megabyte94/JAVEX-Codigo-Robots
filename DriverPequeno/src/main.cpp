#include "vex.h"
using namespace vex;

// Configuración del cerebro y el controlador
brain Brain;
controller Controller1;

vex::pneumatics Pinza(Brain.ThreeWirePort.A); 
// Motores del lado izquierdo
motor MotorL1(PORT14, false); 
motor MotorL2(PORT13, false);
motor MotorL3(PORT12, true);
motor MotorL4(PORT11, true);

// Motores del lado derecho
motor MotorR1(PORT4, true);
motor MotorR2(PORT3, true);
motor MotorR3(PORT2, false);
motor MotorR4(PORT1, false);

// Funciones auxiliares
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
}

// Función para el control manual del robot
void driverControl() {
    while (true) {
        // Leer los valores de los joysticks
        double leftSpeed = Controller1.Axis3.position()+Controller1.Axis1.position();
        double rightSpeed = Controller1.Axis3.position()- Controller1.Axis1.position();
        // Controlar los motores con los joysticks
        setLeftMotors(leftSpeed);
        setRightMotors(rightSpeed);

        // Pequeña pausa para evitar sobrecarga del bucle
        wait(20, msec);
    }
}


// Programa principal
int main() {
    // Iniciar control manual
    driverControl();
    if (Controller1.ButtonR1.pressing()){
        Pinza.open();
    }else (Controller1.ButtonR2.pressing()){
        Pinza.close(); 
    }
    return 0;
}
