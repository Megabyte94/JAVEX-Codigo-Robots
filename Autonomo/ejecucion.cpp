/*
AUTÓNOMO NO. 1: ROBOT VEX HIGH STAKES 2024
POR KENNETH BUSTAMANTE ZULUAGA
PUJ - JAVEX
*/

#include "vex.h"
using namespace vex;

// Funciones
void moveForward(int speed, int duration);
void moveBackward(int speed, int duration);
void turnRight90(int speed);
void turnLeft90(int speed);
void stop();

// Configuración del cerebro y los motores
brain Brain;

// Crear instancias de los motores y asignarlos a los puertos correspondientes del cerebro
motor MotorL1(PORT1, true);  // Motor izquierdo en el puerto 1, con engranaje estándar y sin inversión de dirección
motor MotorL2(PORT2, false);  // Motor izquierdo en el puerto 2, con engranaje estándar y sin inversión de dirección
motor MotorL3(PORT11, false);  // Motor izquierdo en el puerto 3, con engranaje estándar y sin inversión de dirección
motor MotorL4(PORT12, true);  // Motor izquierdo en el puerto 3, con engranaje estándar y sin inversión de dirección
motor MotorR1(PORT9, false);   // Motor derecho en el puerto 4, con engranaje estándar y dirección invertida
motor MotorR2(PORT10, true); // Motor derecho en el puerto 5, con engranaje estándar y dirección invertida
motor MotorR3(PORT19, true); // Motor derecho en el puerto 6, con engranaje estándar y dirección invertida
motor MotorR4(PORT20, false);  // Motor derecho en el puerto 6, con engranaje estándar y dirección invertida

motor Recolector(PORT4, true);
motor Rampa(PORT3, true);


// Función para mover el robot hacia adelante
void stop() {
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
}

void moveForward(int speed, int duration) {
  MotorL1.spin(forward, speed, percent);
  MotorL2.spin(forward, speed, percent);
  MotorL3.spin(forward, speed, percent);
  MotorL4.spin(forward, speed, percent);
  MotorR1.spin(forward, speed, percent);
  MotorR2.spin(forward, speed, percent);
  MotorR3.spin(forward, speed, percent);
  MotorR4.spin(forward, speed, percent);
  wait(duration, seconds);
  stop();
}

void moveForward1(int speed, double duration) {|
  MotorL1.spin(forward, speed, percent);
  MotorL2.spin(forward, speed, percent);
  MotorL3.spin(forward, speed, percent);
  MotorL4.spin(forward, speed, percent);
  MotorR1.spin(forward, speed, percent);
  MotorR2.spin(forward, speed, percent);
  MotorR3.spin(forward, speed, percent);
  MotorR4.spin(forward, speed, percent);
  wait(duration, seconds);
  stop();
}

// Función para mover el robot hacia atrás
void moveBackward(int speed, int duration) {
  MotorL1.spin(reverse, speed, percent);
  MotorL2.spin(reverse, speed, percent);
  MotorL3.spin(reverse, speed, percent);
  MotorL4.spin(reverse, speed, percent);
  MotorR1.spin(reverse, speed, percent);
  MotorR2.spin(reverse, speed, percent);
  MotorR3.spin(reverse, speed, percent);
  MotorR4.spin(reverse, speed, percent);
  wait(duration, seconds);
  stop();
}

void moveBackward1(int speedLeft,int speedRight, int duration) {
  MotorL1.spin(reverse, speedLeft, percent);
  MotorL2.spin(reverse, speedLeft, percent);
  MotorL3.spin(reverse, speedLeft, percent);
  MotorL4.spin(reverse, speedLeft, percent);
  MotorR1.spin(reverse, speedRight, percent);
  MotorR2.spin(reverse, speedRight, percent);
  MotorR3.spin(reverse, speedRight, percent);
  MotorR4.spin(reverse, speedRight, percent);
  wait(duration, seconds);
  stop();
}
// Función para girar el robot 90 grados a la derecha
void turnRight90(int speed) {
  MotorL1.spin(forward, speed, percent);
  MotorL2.spin(forward, speed, percent);
  MotorL3.spin(forward, speed, percent);
  MotorL4.spin(forward, speed, percent);
  MotorR1.spin(reverse, speed, percent);
  MotorR2.spin(reverse, speed, percent);
  MotorR3.spin(reverse, speed, percent);
  MotorR4.spin(reverse, speed, percent);
  wait(1.5, seconds);
  stop();
}
// Función para girar el robot 90 grados a la derecha
void turnLeft90(int speed) {
  MotorL1.spin(reverse, speed, percent);
  MotorL2.spin(reverse, speed, percent);
  MotorL3.spin(reverse, speed, percent);
  MotorL4.spin(reverse, speed, percent);
  MotorR1.spin(forward, speed, percent);
  MotorR2.spin(forward, speed, percent);
  MotorR3.spin(forward, speed, percent);
  MotorR4.spin(forward, speed, percent);
  wait(1.5, seconds);
  stop();
}

void recoleccion(int speed,int duration) {
  Recolector.spin(reverse, speed, percent);
  Rampa.spin(reverse, 65, percent);
  wait(duration, seconds);
  stop();
}



int main() {
  // vexcodeInit();

  // Mover el robot hacia adelante por 10 segundos a una velocidad del 50%
  moveForward(100, 1.9);
  //turnRight90(21.5,1);
  moveBackward1(30,35,3.5);
  moveBackward1(0,25,1);
  moveForward1(80, 1.25);
  recoleccion(100,2);
  turnLeft90(17);
  moveForward1(80, 0.325);
  recoleccion(100,2);
  turnRight90(21.5);
  turnRight90(16);
  moveForward1(80, 0.8);
  recoleccion(100,2);
  turnRight90(21.5);
  turnRight90(19);
  moveForward1(80, 1.2);

  // Girar el robot 90 grados a la derecha a una velocidad del 50%
  /*turnRight90(21.5);
  //
  turnLeft90(21.5);

  // Mover el robot hacia atrás por 10 segundos a una velocidad del 50%
  moveBackward(50, 3);

  wait(1.5, seconds);*/
  stop();

  // Mover el robot hacia la izquierda por 5 segundos a una velocidad del 50%
  // moveLeft(50, 5);

  return 0;
}

// Función para utilizar sensores y mostrar valores
/*void usoConSensores() {
    distance DistanceSensor(PORT1); // Sensor de distancia en el puerto 1
    gyro GyroSensor(Brain.ThreeWirePort.A); // Giroscopio en el puerto A de los
puertos de tres cables del cerebro rotation RotationSensor(PORT2); // Sensor de
rotación en el puerto 2

    // Leer la distancia medida por el sensor de distancia
    double distanceValue = DistanceSensor.objectDistance(mm); // Medida en
milímetros

    // Leer el ángulo del giroscopio
    double angle = GyroSensor.angle(degrees); // Ángulo en grados

    // Leer la rotación medida por el sensor de rotación
    double rotationValue = RotationSensor.rotation(degrees); // Rotación en
grados

    // Imprimir los valores de los sensores en la pantalla del cerebro
    Brain.Screen.print("Distance: %f mm", distanceValue);
    Brain.Screen.print("Angle: %f degrees", angle);
    Brain.Screen.print("Rotation: %f degrees", rotationValue);
}*/