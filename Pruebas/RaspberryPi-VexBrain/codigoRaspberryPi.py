import serial
import time

# Configurar el puerto serie en la Raspberry Pi
ser = serial.Serial('/dev/ttyS0', 9600, timeout=1)
time.sleep(2)  # Esperar a que la conexión se establezca

# Función para enviar comandos al cerebro de VEX
def send_command(command):
    ser.write(command.encode())  # Enviar el comando codificado en bytes
    time.sleep(0.5)  # Esperar medio segundo para permitir la ejecución del comando

# Ejemplo de uso: enviar comandos al cerebro de VEX
send_command("FORWARD,50,10")  # Mover hacia adelante a 50% de velocidad por 10 segundos
send_command("TURNRIGHT,50")   # Girar 90 grados a la derecha a 50% de velocidad
send_command("MOVELEFT,50,5")  # Mover hacia la izquierda por 5 segundos a 50% de velocidad

# Cerrar la conexión serie
ser.close()
