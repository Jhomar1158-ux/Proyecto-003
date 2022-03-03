import cv2
import numpy as np
import imutils 
import os

# intel_logo_validation
# hp_logo_validation
# logitech_logo_validation

datos='intel_logo_train'

if not os.path.exists(datos):
    print('Carpeta creada:', datos)
    os.makedirs(datos)
cap =cv2.VideoCapture(1,cv2.CAP_DSHOW)

x1,y1=190,80
x2,y2=450,398

count=0

while True:
    ret, frame= cap.read()
    if ret == False: break

    aux= frame.copy()
    cv2.rectangle(frame, (x1,y1),(x2,y2),(255,0,0),2)

    objeto=aux[y1:y2,x1:x2]
    # PROBAR cv2.resize ======
    objeto=imutils.resize(objeto, width=38)

    k=cv2.waitKey(1)
    if k==27:
        break
    if k==ord('s'):
        # intel_logo_validation
        # hp_logo_validation
        # logitech_logo_validation

        cv2.imwrite(datos+"/intel_logo_train_{}.jpg".format(count),objeto)
        print("Imagen almacenada: ", 'intel_logo_train_{}.jpg'.format(count))
        count+=1
    cv2.imshow('frame',frame)
    cv2.imshow('objeto',objeto)

