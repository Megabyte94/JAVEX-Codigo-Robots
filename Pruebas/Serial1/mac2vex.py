import serial
import time

# Select the correct serial port for your VEX Brain
port = '/dev/tty.usbmodem1101'  # Try /dev/tty.usbmodem1103 if this doesn't work
baudrate = 115200

# Open the serial connection
ser = serial.Serial(port, baudrate, timeout=1)

def send_command(command):
    ser.write(command.encode())  # Send the command as bytes
    print(f"Sent: {command}")
    time.sleep(0.5)  # Allow some time for the VEX brain to process the command

try:
    # Send a command to display a message on the VEX brain's screen
    send_command('SHOW:Hello VEX!')

except Exception as e:
    print(f"Error: {e}")

finally:
    ser.close()  # Close the serial connection
