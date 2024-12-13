import serial
import time

port = '/dev/ttyUSB0'
#baudrate = 115200
baudrate = 118200

ser = serial.Serial(port, baudrate, timeout=1)

def send_command(command):
    ser.write(command.encode()) #Enviar el comando como bytes
    print(f"Comando enviado: {command}")
    time.sleep(0.5)

try:
    send_command('SHOW:Hola desde la Raspberry Pi!')

except Exception as e:
    print(f"Error: {e}")

finally:
    ser.close()
