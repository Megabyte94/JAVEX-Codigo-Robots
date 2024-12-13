import cv2
import numpy as np
import imutils
import os

Datos = 'p'

if not os.path.exists(Datos):
    print('Carpeta creada: ', Datos)
    os.makedirs(Datos)

# Cambié a índice 0 y quité CAP_DSHOW
cap = cv2.VideoCapture(0)

x1, y1 = 190, 80
x2, y2 = 450, 398

count = 0

while True:
    ret, frame = cap.read()
    if not ret: 
        break

    imAux = frame.copy()
    cv2.rectangle(frame, (x1, y1), (x2, y2), (255, 0, 0), 2)

    objeto = imAux[y1:y2, x1:x2]
    objeto = imutils.resize(objeto, width=38)

    # Mostrar las ventanas después del procesamiento
    cv2.imshow('frame', frame)
    cv2.imshow('objeto', objeto)

    k = cv2.waitKey(1)
    if k == 27:  # Tecla ESC para salir
        break

    if k == ord('s'):
        cv2.imwrite(os.path.join(Datos, f'objeto_{count}.jpg'), objeto)
        print('Imagen guardada:', f'objeto_{count}.jpg')
        count += 1

cap.release()
cv2.destroyAllWindows()
