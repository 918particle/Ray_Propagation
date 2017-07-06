#########################################################################
#########################################################################
##############                                          #################
##############            Plotting Flux Data            #################
##############                                          #################
#########################################################################
#########################################################################
#########################################################################

import matplotlib.pyplot  as plt
import math as m
import os

# Presets, can use these to bypass python inputs, just make sure get_all_inputs is commented out
PhotometerAngle = 0
NumberOfPhotometers = 10
PhotometerArea = 10
PhotometerX = 386.5
PhotometerZ = -13
PhotometerSeperation = 20

def get_input(): #User inputs desired scatter (Sets scatter_type in cpp files)
	inpt = raw_input()
	try:
		float(inpt)
	except:
		print ''
		os.system('echo option invalid, please type in a number')
		inpt = get_input()
	return float(inpt)

def get_all_inputs():
	global PhotometerX
	global PhotometerZ
	global PhotometerArea
	global NumberOfPhotometers
	global PhotometerSeperation
	global PhotometerAngle
	print ''
	print 'Please type number of binary files to input:'
	NumOfFiles = int(get_input())
	print 'Please type photometer x position:'
	PhotometerX = get_input()
	print 'Please type photometer z position:'
	PhotometerZ = get_input()
	print 'Please type photometer area:'
	PhotometerArea = get_input()
	print 'Please type number photometers:'
	NumberOfPhotometers = int(get_input())
	print 'Please type photometers seperation distance:'
	PhotometerSeperation = get_input()
	print 'please type angle at which to seperate photometers (with respect to ice surface)'
	PhotometerAngle = get_input()

#get_all_inputs()
NumOfFiles = 10
xBeforePhotometer = 0
xAfterPhotometer = 0
zBeforePhotometer = 0
zAfterPhotometer = 0
flux = []
for i in range(NumberOfPhotometers):
	flux.append(0)
for i in range(NumOfFiles):
	FileName = "data/propagation_path_" + str(i) + ".dat"
	FileI = open(FileName, "r").read().splitlines()
	xPos = []
	zPos = []
	for j in range(len(FileI)):
		FileLineI = FileI[j].split(" ")
		xPos.append(float(FileLineI[0]))
		zPos.append(float(FileLineI[1]))
	for j in range(len(flux)):
		PhotometerXAtJ = PhotometerX + (j*PhotometerSeperation*m.cos(PhotometerAngle))
		PhotometerZAtJ = PhotometerZ + (j*PhotometerSeperation*m.sin(PhotometerAngle))
		for k in range(len(xPos)):
			xValue = xPos[k]
			if xValue < (PhotometerXAtJ):
				xBeforePhotometer = xValue
				zBeforePhotometer = zPos[k]
			if xValue > (PhotometerXAtJ):
				xAfterPhotometer = xValue
				zAfterPhotometer = zPos[k]
				break
		slope = (zAfterPhotometer - zBeforePhotometer)/(xAfterPhotometer - xBeforePhotometer)
		zPositionAtPhotometer = (slope * (PhotometerXAtJ - xBeforePhotometer)) + zBeforePhotometer
		if zPositionAtPhotometer < (PhotometerZAtJ + PhotometerArea/2) and zPositionAtPhotometer > (PhotometerZAtJ - PhotometerArea/2):
			flux[j] = flux[j] + 1

fig = plt.figure()
titleString = "flux dependence on distance"
plt.title(titleString)
xLabelString = "Distance (steps of " + str(PhotometerSeperation) +"m)"
plt.xlabel(xLabelString)
plt.ylabel("Flux")
xData = []
fluxTheory = []
for i in range(NumberOfPhotometers):
	xData.append(i*PhotometerSeperation)
fluxTheory.append(flux[0])
for i in range(NumberOfPhotometers - 1):  # 1/r theory
	fluxTheory.append(fluxTheory[i]*(PhotometerSeperation * (i + 1))/(PhotometerSeperation * (i + 2)))
plt.plot(xData,flux, 'o', label='Sim Data')
plt.plot(xData,fluxTheory, 'o',label='Theory')
plt.legend()
#plt.show()
saveString = "Flux_Plot.png"
fig.savefig(saveString)
plt.close(fig)

