#include "vex.h"

using namespace vex;

// Creación de los motores en los puertos 15 y 17
motor motor1 = motor(PORT15);
motor motor2 = motor(PORT17);

int main() {
  // Inicialización del controlador
  controller Controller1 = controller(primary);

  while (true) {
    // Si se presiona el botón L1, mueve el motor en el puerto 15
    if (Controller1.ButtonL1.pressing()) {
      motor1.spin(directionType::fwd, 100, velocityUnits::pct);
      motor2.spin(directionType::fwd, -100, velocityUnits::pct);
    } else {
      motor1.stop();
      motor2.stop();
    }

    // Si se presiona el botón L2, mueve el motor en el puerto 17 en dirección contraria
    if (Controller1.ButtonL2.pressing()) {
        motor1.spin(directionType::fwd, -100, velocityUnits::pct);
        motor2.spin(directionType::fwd, 100, velocityUnits::pct);
    } else {
      motor1.stop();
      motor2.stop();
    }

    // Espera un ciclo de control para evitar sobrecargar el procesador
    task::sleep(20);
  }
}
