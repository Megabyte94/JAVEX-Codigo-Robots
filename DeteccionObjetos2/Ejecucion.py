import cv2
cap = cv2.VideoCapture(1,cv2.CAP_DSHOW)

objetoClassif = cv2.CascadeClassifier('cascade.xml')

while True:
    ret, frame = cap.read()
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    
    object = objetoClassif.detectMultiScale(gray,
    scaleFactor = 1.3,
    minNeighbors = 10)

    for (x,y,w,h) in object:
        cv2.rectangle(frame, (x,y),(x+w,y+h),(0,255,0),2)
        cv2.putText(frame, 'USB' , (x,y-10),2,0.7,(0,255,0),2,cv2.LINE_AA)
    
    cv2.imshow('frame', frame)

    if cv2.waitKey(1) == 27:
        break

cap.release()
cv2.destroyAllWindows()