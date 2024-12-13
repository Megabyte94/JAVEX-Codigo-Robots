import cv2

# Load the trained classifier
classifier = cv2.CascadeClassifier('classifier/cascade.xml')

# Initialize webcam
cap = cv2.VideoCapture(0)

while True:
    ret, frame = cap.read()
    if not ret:
        break

    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

    # Detect objects using the classifier
    objects = classifier.detectMultiScale(gray, scaleFactor=1.3, minNeighbors=10)

    # Draw rectangles around detected objects
    for (x, y, w, h) in objects:
        cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 255, 0), 2)
        cv2.putText(frame, 'Object Detected', (x, y - 10), 2, 0.7, (0, 255, 0), 2, cv2.LINE_AA)

    # Display the frame with detected objects
    cv2.imshow('Detection', frame)

    if cv2.waitKey(1) == 27:  # Press ESC to exit
        break

cap.release()
cv2.destroyAllWindows()
