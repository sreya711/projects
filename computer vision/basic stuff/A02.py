import cv2
import os

cam = cv2.VideoCapture("vid.mov")
currentframe = 0

while(True):
	ret,frame = cam.read()

	if ret:
		# if video is still left continue creating images
		name = './data/' + str(currentframe) + '.jpg'
		print ('Creating...' + name)
		cv2.imwrite(name, frame)
		currentframe += 1
	else:
		break

cam.release()
cv2.destroyAllWindows()