import csv
import cv2
import numpy as np


images = []
msrmts = []

#sample data udacity gave

def addImageInfo(images, msrmts,line, fCount, cameraNum):
	correction = 0.1
	source_path = line[cameraNum]
	filename = source_path.split('/')[-1]
	current_path = 'data'+fCount+'/IMG/'+filename
	image = cv2.imread(current_path)

	#regular image
	msrmt = float(line[3])

	#mirror image
	measurement_flipped = -msrmt

	#adjustment for left camera=1/right camera=2 images
	if cameraNum == 1:
		msrmt += correction
		measurement_flipped -= correction
	elif cameraNum == 2:
		msrmt -= correction
		measurement_flipped += correction
	#add regular	
	images.append(image)
	msrmts.append(msrmt)

	#add flipped
	msrmts.append(measurement_flipped)
	image_flipped = np.fliplr(image)
	images.append(image_flipped)
	
def collectData(images, msrmts,fCount):
	lines = []
	with open('data'+fCount+'/driving_log.csv') as csvfile:
		reader = csv.reader(csvfile)
		for line in reader:
			lines.append(line)
	for line in lines:
		if 'speed' in line:
			continue
		#CENTER CAMERA is 0, Left is 1, Right is 2
		addImageInfo(images, msrmts, line, fCount, 0)
		#addImageInfo(images, msrmts, line, fCount, 1)
		#addImageInfo(images, msrmts, line, fCount, 2)

#collectData(images, msrmts,'5')
collectData(images, msrmts,'1')
collectData(images, msrmts,'2')
#collectData(images, msrmts,'3')
#collectData(images, msrmts,'4')

X_train = np.array(images)
y_train = np.array(msrmts)

from keras.models import Sequential
from keras.layers import Flatten, Dense, Lambda, Dropout
from keras.layers import Convolution2D, MaxPooling2D, Cropping2D

model = Sequential()
model.add(Lambda(lambda x: (x / 255.0) - 0.5, input_shape=(160,320,3)))
model.add(Cropping2D(cropping=((70,25), (0,0))))
model.add(Convolution2D(24,5,5,subsample=(2,2), activation="relu"))
model.add(Convolution2D(36,5,5,subsample=(2,2), activation="relu"))
model.add(Convolution2D(48,5,5,subsample=(2,2), activation="relu"))
model.add(Convolution2D(64,3,3, activation="relu"))
model.add(Convolution2D(64,3,3, activation="relu"))
model.add(Flatten())
model.add(Dense(100))
model.add(Dropout(0.5))
model.add(Dense(50))
#model.add(Dropout(0.25))
model.add(Dense(10))
model.add(Dense(1))

model.compile(loss='mse', optimizer='adam')
model.fit(X_train, y_train, validation_split=0.2, shuffle=True, nb_epoch=3)

model.save('model.h5')
