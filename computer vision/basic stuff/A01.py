# Python program to illustrate
# arithmetic operation of
# bitwise AND of two images
	
# organizing imports
import cv2
import numpy as np
import matplotlib.pyplot as plt
	
# path to input images are specified and
# images are loaded with imread command
img = cv2.imread('mountain.png')
imga = cv2.imread('beach.png')
img1 = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
img2 = cv2.cvtColor(imga, cv2.COLOR_BGR2RGB)

# cv2.bitwise_and is applied over the
# image inputs with applied parameters
#dest_and = cv2.bitwise_and(img2, img1, mask = None)
img2[50:100,50:500] =img1[50:100,50:500]
img2[50:300,210:300] =img1[50:300,210:300]
# the window showing output image
# with the Bitwise AND operation
# on the input images
plt.imshow(img2)
plt.show()


