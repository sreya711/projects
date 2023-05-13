import cv2
import os

path = 'data/'
vidname='imagevid.mp4'
pre_imgs = os.listdir(path)
#print(pre_imgs)
img = []
x = 0

for i in pre_imgs:
    i=path+i
    img.append(cv2.imread(i))
height, width, layers = img[0].shape
video = cv2.VideoWriter(vidname, cv2.VideoWriter_fourcc(*'DIVX'), 28,(width,height))

for i in range(len(os.listdir('data'))):
    frame = cv2.imread(os.path.join('data',"%d.jpg"%i))
    video.write(frame)

video.release()


#for i in range(len(img)):
 #   video.write(img[i])
  #  print('frame', i+1, ' of ',len(img))

# cv2.destroyAllWindows()    
video.release()