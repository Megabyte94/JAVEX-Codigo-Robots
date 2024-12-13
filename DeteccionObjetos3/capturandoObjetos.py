import cv2
import os
import imutils

# Folder for storing captured images
Datos = 'n'

# Ensure the folder exists or create it
if not os.path.exists(Datos):
    print('Creating folder: ', Datos)
    os.makedirs(Datos)

# Initialize the webcam (use 0 for the default webcam)
cap = cv2.VideoCapture(0)

# Coordinates for the capture rectangle
x1, y1 = 190, 80
x2, y2 = 450, 398

count = 0

while True:
    ret, frame = cap.read()
    
    # Check if the frame was captured successfully
    if not ret:
        print("Error: Failed to capture frame from camera.")
        break

    imAux = frame.copy()
    # Draw a rectangle where the object will be captured
    cv2.rectangle(frame, (x1, y1), (x2, y2), (255, 0, 0), 2)

    # Crop the region of interest (ROI)
    objeto = imAux[y1:y2, x1:x2]
    objeto = imutils.resize(objeto, width=38)

    # Show the current frame and the object to be captured
    cv2.imshow('Frame', frame)
    cv2.imshow('Object', objeto)

    k = cv2.waitKey(1)
    if k == 27:  # Press ESC to exit
        break

    if k == ord('s'):  # Press 's' to save the image
        # Construct the image path
        image_path = os.path.join(Datos, f'objeto_{count}.jpg')
        
        # Save the image and confirm it was saved
        cv2.imwrite(image_path, objeto)
        if os.path.exists(image_path):
            print(f'Image saved successfully: {image_path}')
            count += 1
        else:
            print(f'Error: Image not saved: {image_path}')

cap.release()
cv2.destroyAllWindows()
