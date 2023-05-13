import cv2
vid = cv2.VideoCapture(0)
img_dir = "Images/"
cv2.namedWindow("video")
img_name = "Img"
i=0
while True:
    check,frame = vid.read()
    if not check:
        break

    cv2.imshow("video",frame)
    k = cv2.waitKey(1)
    if k%256 == 27:
    # ESC pressed
        print("Escape hit, closing...")
        break
    elif k%256 == 32:
        # SPACE pressed
        img = img_dir+img_name+str(i)+".png"
        cv2.imwrite(img,frame)
        print(img)
        i += 1
vid.release()
cv2.destroyAllWindows()