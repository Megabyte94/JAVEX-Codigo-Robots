#include "vex.h"

using namespace vex;

// Configuración del cerebro y los motores
brain Brain;
motor MotorL1(PORT1, false);
motor MotorL2(PORT2, false);
motor MotorL3(PORT3, false);
motor MotorR1(PORT4, true);
motor MotorR2(PORT5, true);
motor MotorR3(PORT6, true);

// Configuración del puerto serie para la comunicación
serial PortSerial = serial(Brain.ThreeWirePort.A); // Usando el puerto A para la comunicación en serie

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

// Función para girar el robot 90 grados a la derecha
void turnRight90(int speed) {
  // Código de giro usando el giroscopio...
  // (Mismo código que en tu ejemplo anterior)
}

// Función para mover el robot hacia la izquierda
void moveLeft(int speed, int duration) {
  MotorL1.spin(reverse, speed, percent);
  MotorL2.spin(reverse, speed, percent);
  MotorL3.spin(reverse, speed, percent);
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

int main() {
  vexcodeInit();
  
  char buffer[32];
  
  while (true) {
    if (PortSerial.read(buffer, sizeof(buffer))) { // Leer desde el puerto serie
      // Interpretar el comando recibido
      if (strncmp(buffer, "FORWARD", 7) == 0) {
        int speed, duration;
        sscanf(buffer, "FORWARD,%d,%d", &speed, &duration);
        moveForward(speed, duration);
      } else if (strncmp(buffer, "TURNRIGHT", 9) == 0) {
        int speed;
        sscanf(buffer, "TURNRIGHT,%d", &speed);
        turnRight90(speed);
      } else if (strncmp(buffer, "MOVELEFT", 8) == 0) {
        int speed, duration;
        sscanf(buffer, "MOVELEFT,%d,%d", &speed, &duration);
        moveLeft(speed, duration);
      }
    }
  }
  
  return 0;
}
