#include "vex.h"

using namespace vex;

brain Brain;
serial SerialPort(Brain.ThreeWirePort.A);

int main() {
    char buffer[100];
    int index = 0;

    Brain.Screen.clearScreen();
    Brain.Screen.setCursor(1, 1);
    Brain.Screen.print("Esperando comandos...");

    while (true) {
        if (SerialPort.available() > 0) {
            char receivedChar = SerialPort.read();  // Lectura byte desde puerto serial

            // Si no es un salto de línea, concatenar
            if (receivedChar != '\n') {
                buffer[index++] = receivedChar;
            } else {
                // Terminar comando recibido con caracter nulo
                buffer[index] = '\0';
                index = 0;

                Brain.Screen.clearScreen();
                Brain.Screen.setCursor(1, 1);
                Brain.Screen.print("Recibido: %s", buffer);

                if (strncmp(buffer, "SHOW:", 5) == 0) {
                    Brain.Screen.setCursor(2, 1);
                    Brain.Screen.print("Mensaje: %s", &buffer[5]);
                }
            }
        }

        vex::this_thread::sleep_for(100);
    }
}
