import cv2
import numpy as np
import serial
import time

# Configurar el puerto serie en la Raspberry Pi
ser = serial.Serial('/dev/ttyS0', 9600, timeout=1)
time.sleep(2)  # Esperar a que la conexión se establezca

# Rango de color azul en el espacio de color HSV
lower_blue = np.array([100, 150, 0])
upper_blue = np.array([140, 255, 255])

# Inicializar la cámara
cap = cv2.VideoCapture(0)  # Usar 0 si es la primera cámara USB conectada

def send_command(command):
    """Envía un comando al cerebro de VEX a través de la conexión serie."""
    ser.write(command.encode())  # Enviar el comando codificado en bytes
    time.sleep(0.5)  # Esperar medio segundo para permitir la ejecución del comando

def move_towards_ring(cx, cy, frame_width):
    """Determina el movimiento del robot basado en la posición del anillo azul."""
    center_x = frame_width // 2
    threshold = 50  # Umbral para considerar si el anillo está centrado

    if cx < center_x - threshold:  # El anillo está a la izquierda
        send_command("MOVELEFT,50,1")  # Moverse a la izquierda
    elif cx > center_x + threshold:  # El anillo está a la derecha
        send_command("MOVERIGHT,50,1")  # Moverse a la derecha
    else:  # El anillo está centrado
        send_command("FORWARD,50,2")  # Moverse hacia adelante

while True:
    ret, frame = cap.read()

    if not ret:
        print("No se pudo capturar la imagen")
        break

    # Convertir la imagen al espacio de color HSV
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

    # Crear una máscara para el color azul
    mask = cv2.inRange(hsv, lower_blue, upper_blue)

    # Encontrar contornos en la máscara
    contours, _ = cv2.findContours(mask, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

    # Dibujar los contornos y obtener la posición del centro de los anillos detectados
    for contour in contours:
        area = cv2.contourArea(contour)
        if area > 500:  # Filtrar pequeños ruidos
            # Obtener el rectángulo delimitador
            x, y, w, h = cv2.boundingRect(contour)
            # Dibujar el rectángulo
            cv2.rectangle(frame, (x, y), (x+w, y+h), (0, 255, 0), 2)
            # Calcular el centro del rectángulo
            cx = x + w // 2
            cy = y + h // 2
            # Dibujar el centro
            cv2.circle(frame, (cx, cy), 5, (255, 0, 0), -1)

            # Mover el robot hacia el anillo detectado
            move_towards_ring(cx, cy, frame.shape[1])

    # Mostrar el resultado
    cv2.imshow('Frame', frame)
    cv2.imshow('Mask', mask)

    # Salir del loop al presionar la tecla 'q'
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# Liberar la cámara y cerrar todas las ventanas
cap.release()
cv2.destroyAllWindows()

# Cerrar la conexión serie
ser.close()
