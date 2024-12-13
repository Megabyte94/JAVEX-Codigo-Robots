#include "vex.h"

using namespace vex;

// Configuración del cerebro y los motores
brain Brain;

// Crear instancias de los motores y asignarlos a los puertos correspondientes del cerebro
motor MotorL1(PORT1, false);
motor MotorL2(PORT2, false);
motor MotorL3(PORT3, false);
motor MotorR1(PORT4, true);
motor MotorR2(PORT5, true);
motor MotorR3(PORT6, true);

// Inicializar el giroscopio en el puerto A
gyro GyroSensor(Brain.ThreeWirePort.A);

// Función para mover el robot hacia adelante
void moveForward(int speed, int duration) {
  MotorL1.spin(forward, speed, percent);
  MotorL2.spin(forward, speed, percent);
  MotorL3.spin(forward, speed, percent);
  MotorR1.spin(forward, speed, percent);
  MotorR2.spin(forward, speed, percent);
  MotorR3.spin(forward, speed, percent);
  wait(duration, seconds);
  MotorL1.stop();
  MotorL2.stop();
  MotorL3.stop();
  MotorR1.stop();
  MotorR2.stop();
  MotorR3.stop();
}

// Función para girar el robot 90 grados a la derecha validado con el giroscopio
void turnRight90(int speed) {
  GyroSensor.startCalibration(); // Calibrar giroscopio
  wait(2, seconds); // Esperar a que termine la calibración

  GyroSensor.setHeading(0, degrees); // Restablecer el ángulo del giroscopio a 0

  while(GyroSensor.heading(degrees) < 90) {
    MotorL1.spin(forward, speed, percent);
    MotorL2.spin(forward, speed, percent);
    MotorL3.spin(forward, speed, percent);
    MotorR1.spin(reverse, speed, percent);
    MotorR2.spin(reverse, speed, percent);
    MotorR3.spin(reverse, speed, percent);
  }

  // Detener los motores una vez alcanzados los 90 grados
  MotorL1.stop();
  MotorL2.stop();
  MotorL3.stop();
  MotorR1.stop();
  MotorR2.stop();
  MotorR3.stop();
  
  Brain.Screen.print("Giro completado. Ángulo: %f grados", GyroSensor.heading(degrees));
}

int main() {
  vexcodeInit();
  
  // Mover el robot hacia adelante por 10 segundos a una velocidad del 50%
  moveForward(50, 10);

  // Girar el robot 90 grados a la derecha a una velocidad del 50% validando con el giroscopio
  turnRight90(50);

  return 0;
}