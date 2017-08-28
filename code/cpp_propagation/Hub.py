##################################################################################
#########################    Run.py (Bash Script)  ###############################
##################################################################################
##################################################################################
##################################################################################
########                                                               ###########
########    Scrpit for choosing Model, scattering type, checking for   ###########
########    required directories/files, compiling .cc files, creating  ###########
########    RunPropagator executable, and running executable.          ###########  
########                                                               ###########
##################################################################################
##################################################################################
##################################################################################

import matplotlib.pyplot  as plt
import math
import os
import time
import numpy as np
from colour import Color
import copy


sl = [10,1000]
scatter = [0.0,0.001,0.003,0.005,0.007,0.01,0.03,0.05,0.07,0.1]
angles = [180.0]
rang = 0.05
Num_Rays = 999
numRaysPerAngle = 1000
em_x = 1400.0
em_y = 0.0
em_z = -22.0
save_dir = 'ChechRev'


### setting up the cpp sim
def create_compilation_string():
	nonModelFiles = []
	nonModelFiles_index = 0
	nonModelFiles.append('MooresBayModel.cc')
	nonModelFiles_index += 1
	cwd = os.getcwd()
	ccString = ''
	for file in os.listdir(cwd):
		nonModel = False
		if file.endswith(".cc"):
			for i in range(len(nonModelFiles)):
				if file == str(nonModelFiles[i]):
					nonModel = True
			if (not nonModel):
				ccString += str(file) + ' '
	osString = 'g++ ' + ccString + ' -o RunPropagator'
	return osString

def checkFor_executable_directiory():
	if not os.path.isdir("data"): #checks if data directory exists in working directory, if not, creates it.
		os.system('mkdir data')
	else:
		os.system('rm data/*.dat')
	#if os.path.exists("RunPropagator"): #Checks is pre-compiled binary file exists, if so, deletes it <- helps for debugging
	#	os.system('rm RunPropagator')

### Run executable with specifed scattering
def execute(scatter, a1, a2, c, sl,num,numRaysPerAngle, em_x, em_y, em_z):
	time.sleep(.1) #sometimes python runs executive commands to quickly, can cause bugs.
	executable = './RunPropagator' + num + ' ' + str(scatter) + ' ' + str(2) + ' ' + str(a1) + ' ' + str(a2) + ' ' + str(sl) + ' ' + str(numRaysPerAngle) + ' ' + str(em_x) + ' ' + str(em_y) + ' ' + str(em_z)
	os.system(executable) #termianl command

### Compiles specific model
def compile(model):
	osString = create_compilation_string()
	os.system(osString) #termianl command

### Photometer
def binarySearch(alist, item):
	first = 0
	last = len(alist)-1
	while abs(last-first) > 1:
		midpoint = (first + last)//2
		if float(alist[midpoint].split(" ")[0]) < item:
			first = midpoint
		else:
			last = midpoint
	return [first, last]

def Photometer(Num_Files,Area,X,Z,num_phot,angle,Seperation):
	Counts = []
	PhotometerAngle = angle*3.14/180.0
	NumberOfPhotometers = num_phot
	PhotometerArea = Area
	PhotometerX = X
	PhotometerZ = Z
	PhotometerSeperation = Seperation
	NumOfFiles = Num_Files
	xBeforePhotometer = 0
	xAfterPhotometer = 1
	zBeforePhotometer = 0
	zAfterPhotometer = 1
	flux = []
	for i in range(NumberOfPhotometers):
		flux.append(0)
	for i in range(NumOfFiles):
		FileName = "data/propagation_path_" + str(i) + ".dat"
		FileI = open(FileName, "r").read().splitlines()
		for j in range(len(flux)):
			PhotometerXAtJ = PhotometerX + (j*PhotometerSeperation*math.cos(PhotometerAngle))
			PhotometerZAtJ = PhotometerZ + (j*PhotometerSeperation*math.sin(PhotometerAngle))
			k = binarySearch(FileI,PhotometerXAtJ)
			first = FileI[k[0]].split(" ")
			last = FileI[k[1]].split(" ")
			xBeforePhotometer = float(first[0])
			xAfterPhotometer = float(last[0])
			zBeforePhotometer = float(first[1])
			zAfterPhotometer = float(last[1])
			if xAfterPhotometer != xBeforePhotometer:
				slope = (zAfterPhotometer - zBeforePhotometer)/(xAfterPhotometer - xBeforePhotometer)
				zPositionAtPhotometer = (slope * (PhotometerXAtJ - xBeforePhotometer)) + zBeforePhotometer
				if zPositionAtPhotometer < (PhotometerZAtJ + PhotometerArea/2) and zPositionAtPhotometer > (PhotometerZAtJ - PhotometerArea/2):
					flux[j] = flux[j] + 1	
	Counts.append(flux)
	return Counts[0]


### Plotting
def plotRayPaths(sav,NumRays,offset,direc):
	NumFiles = NumRays
	size = [4000, 2000]
	boxSizeX = 10
	boxSizeZ = 5
	xliml = 0
	xlimu = 4000
	zliml = -1000
	zlimu = 1000
	boxNumX = int((xlimu - xliml) / boxSizeX)
	boxNumZ = int((zlimu - zliml) / boxSizeZ)
	boxPointColor = []

	startColor = Color("yellow")
	html = list(startColor.range_to(Color("purple"),NumFiles+1))
	colors = []
	
	for i in range(len(html)):
		colors.append(str(html[i]))
	
	saveString = sav + ".png"# + str(scatter[i]) + ".png"
	
	fig = plt.figure()
	plt.xlim(xliml,xlimu)
	plt.ylim(zliml,zlimu)
	plt.xlabel("Range (m)")
	plt.ylabel("Depth (m)")
	
	for i in range(NumFiles):
		FileName = "data" + direc + "/propagation_path_" + str(i + offset) + ".dat"
		fileI = open(FileName, "r").read().splitlines()	
		xpos = []
		zpos = []	
		for i in range(len(fileI)):
			xpos.append(float(fileI[i].split(" ")[0]))
			zpos.append(float(fileI[i].split(" ")[1]))
		plt.plot(xpos,zpos,color="black",alpha=.01)
	plt.grid()
	#plt.show()
	fig.savefig(saveString)
	plt.close(fig)

def plotFluxVsDistance(num_runs,num_phot,data,Num_Rays,scatter,angles,colors,x,step,save,log,lim,xliml,xlimu,yliml,ylimu):
	html = colors
	phot_data = data
	xData = []
	yData = []
	count = 0
	slopes = []
	sigmas = []
	xdata = []
	ydata1 = []
	ydata2 = []
	ydata3 = []
	ydata4 = []
	z = 0
	### Pre Plot Data
	#for i in range(num_runs):
	#	for k in range(6):
	#		del phot_data[i][0]
	#for i in range(num_runs):
	#	for k in range(18):
	#		del phot_data[i][len(phot_data[i])-(k+1)]

	for i in range(len(phot_data[0])):
		xdata.append(float(x + step*i))

	for i in range(num_runs):
		data_per_run = []
		for j in range(len(phot_data[0])):
			data_per_run.append(float(phot_data[i][j]))
		ydata1.append(data_per_run)

		data_per_run = []
		for j in range(len(phot_data[0])):
			data_per_run.append(float(phot_data[i][j] * xdata[j]))
		ydata2.append(data_per_run)

		data_per_run = []
		for j in range(len(phot_data[0])):
			if phot_data[i][j] > 0.0:
				data_per_run.append(float(math.log(phot_data[i][j])))
			else:
				data_per_run.append(-666)
		ydata3.append(data_per_run)

		data_per_run = []
		for j in range(len(phot_data[0])):
			if phot_data[i][j] > 0.0:
				data_per_run.append(float(math.log(phot_data[i][j]) * xdata[j]))
			else:
				data_per_run.append(-666)
		ydata4.append(data_per_run)

	### Plotting
	fig = plt.figure()
	plt.xlabel('distance (m)')
	plt.ylabel('number of rays')
	plt.gca().set_ylim(bottom=0)
	if lim:
		plt.ylim(yliml,ylimu)
		plt.xlim(xliml,xlimu)
	for i in range(num_runs):
		label = '$\sigma$: ' + (str("{0:.2f}".format(scatter[i]*180.0/3.14)))
		plt.plot(xdata,ydata1[i], 'o',label=label, color=html[i])
	plt.legend(loc='best',prop={'size':8})
	saveString = "FluxX" + save + ".png"
	fig.savefig(saveString)
	plt.close(fig)

	### Plotting with fits
	fig = plt.figure()
	plt.xlabel('distance (m)')
	plt.ylabel('number of rays')
	#plt.gca().set_ylim(bottom=0)
	if lim:
		plt.ylim(yliml,ylimu*2)
		plt.xlim(xliml,xlimu)
	for i in range(num_runs):
		label = '$\sigma$: ' + (str("{0:.2f}".format(scatter[i]*180.0/3.14)))
		x = np.linspace(0,3000,600)
		y = []
		xcut = []
		ycut = []
		for val in range(len(ydata1[i])):
			if ydata1[i][val] > 0.0:
				xcut.append(xdata[val])
				ycut.append(ydata1[i][val])
		if len(ycut) > 0:
			slope, ln_b = np.polyfit(xcut,ycut,1)
			for val in range(len(x)):
				y.append(slope * x[val] + ln_b)				
			label = label + ', m:' + str("{0:.4f}".format(float(slope)))
			xerr = 1/math.sqrt(len(ycut))
			yerr = xerr
			label = label + ', $\epsilon$:' + str("{0:.2f}".format(float(xerr)))
			plt.plot(x,y,'-',label=label,color=html[i])#yerr,xerr,'o',color=html[i])
			sigmas.append(scatter[i]*180.0/3.14)
			slopes.append(slope)
		plt.legend(loc='best',prop={'size':8})
	saveString = "FluxX" + save + "_withFit.png"
	fig.savefig(saveString)
	plt.close(fig)

	### Plotting
	fig = plt.figure()
	plt.xlabel('distance (m)')
	plt.ylabel('number of rays times distance')
	plt.gca().set_ylim(bottom=0)
	if lim:
		plt.ylim(yliml,ylimu)
		plt.xlim(xliml,xlimu)
	for i in range(num_runs):
		label = '$\sigma$: ' + (str("{0:.2f}".format(scatter[i]*180.0/3.14)))
		plt.plot(xdata,ydata2[i], 'o',label=label, color=html[i])
	plt.legend(loc='best',prop={'size':8})
	saveString = "FluxDistanceX" + save + ".png"
	fig.savefig(saveString)
	plt.close(fig)

	### Plotting with fits
	fig = plt.figure()
	plt.xlabel('distance (m)')
	plt.ylabel('number of rays times distance')
	#plt.gca().set_ylim(bottom=0)
	if lim:
		plt.ylim(yliml,ylimu*2)
		plt.xlim(xliml,xlimu)
	for i in range(num_runs):
		label = '$\sigma$: ' + (str("{0:.2f}".format(scatter[i]*180.0/3.14)))
		x = np.linspace(0,3000,600)
		y = []
		xcut = []
		ycut = []
		for val in range(len(ydata2[i])):
			if ydata2[i][val] > 0.0:
				xcut.append(xdata[val])
				ycut.append(ydata2[i][val])
		if len(ycut) > 0:
			slope, ln_b = np.polyfit(xcut,ycut,1)
			for val in range(len(x)):
				y.append(slope * x[val] + ln_b)				
			label = label + ', m:' + str("{0:.4f}".format(float(slope)))
			xerr = 1/math.sqrt(len(ycut))
			yerr = xerr
			label = label + ', $\epsilon$:' + str("{0:.2f}".format(float(xerr)))
			plt.plot(x,y,'-',label=label,color=html[i])#yerr,xerr,'o',color=html[i])
			sigmas.append(scatter[i]*180.0/3.14)
			slopes.append(slope)
		plt.legend(loc='best',prop={'size':8})
	saveString = "FluxDistanceX" + save + "_withFit.png"
	fig.savefig(saveString)
	plt.close(fig)

	### Plotting
	fig = plt.figure()
	plt.xlabel('distance (m)')
	plt.ylabel('number of rays')
	plt.gca().set_ylim(bottom=0)
	if lim:
		plt.ylim(yliml,ylimu)
		plt.xlim(xliml,xlimu)
	for i in range(num_runs):
		label = '$\sigma$: ' + (str("{0:.2f}".format(scatter[i]*180.0/3.14)))
		plt.plot(xdata,ydata3[i], 'o',label=label, color=html[i])
	plt.legend(loc='best',prop={'size':8})
	saveString = "LnFluxX" + save + ".png"
	fig.savefig(saveString)
	plt.close(fig)

	### Plotting with fits
	fig = plt.figure()
	plt.xlabel('distance (m)')
	plt.ylabel('number of rays')
	#plt.gca().set_ylim(bottom=0)
	if lim:
		plt.ylim(yliml,ylimu)
		plt.xlim(xliml,xlimu)
	for i in range(num_runs):
		label = '$\sigma$: ' + (str("{0:.2f}".format(scatter[i]*180.0/3.14)))
		x = np.linspace(0,3000,600)
		y = []
		xcut = []
		ycut = []
		for val in range(len(ydata3[i])):
			if ydata3[i][val] > 0.0:
				xcut.append(xdata[val])
				ycut.append(ydata3[i][val])
		if len(ycut) > 0:
			slope, ln_b = np.polyfit(xcut,ycut,1)
			for val in range(len(x)):
				y.append(slope * x[val] + ln_b)				
			label = label + ', m:' + str("{0:.4f}".format(float(slope)))
			xerr = 1/math.sqrt(len(ycut))
			yerr = xerr
			label = label + ', $\epsilon$:' + str("{0:.2f}".format(float(xerr)))
			plt.plot(x,y,'-',label=label,color=html[i])#yerr,xerr,'o',color=html[i])
			sigmas.append(scatter[i]*180.0/3.14)
			slopes.append(slope)
		plt.legend(loc='best',prop={'size':8})
	saveString = "LnFluxX" + save + "_withFit.png"
	fig.savefig(saveString)
	plt.close(fig)

	### Plotting
	fig = plt.figure()
	plt.xlabel('distance (m)')
	plt.ylabel('number of rays times distance')
	plt.gca().set_ylim(bottom=0)
	if lim:
		plt.ylim(yliml,ylimu*2)
		plt.xlim(xliml,xlimu)
	for i in range(num_runs):
		label = '$\sigma$: ' + (str("{0:.2f}".format(scatter[i]*180.0/3.14)))
		plt.plot(xdata,ydata4[i], 'o',label=label, color=html[i])
	plt.legend(loc='best',prop={'size':8})
	saveString = "LnFluxDistanceX" + save + ".png"
	fig.savefig(saveString)
	plt.close(fig)

	### Plotting with fits
	fig = plt.figure()
	plt.xlabel('distance (m)')
	plt.ylabel('number of rays times distance')
	#plt.gca().set_ylim(bottom=0)
	if lim:
		plt.ylim(yliml,ylimu*2)
		plt.xlim(xliml,xlimu)
	for i in range(num_runs):
		label = '$\sigma$: ' + (str("{0:.2f}".format(scatter[i]*180.0/3.14)))
		x = np.linspace(0,3000,600)
		y = []
		xcut = []
		ycut = []
		for val in range(len(ydata4[i])):
			if ydata4[i][val] > 0.0:
				xcut.append(xdata[val])
				ycut.append(ydata4[i][val])
		if len(ycut) > 0:
			slope, ln_b = np.polyfit(xcut,ycut,1)
			for val in range(len(x)):
				y.append(slope * x[val] + ln_b)				
			label = label + ', m:' + str("{0:.4f}".format(float(slope)))
			xerr = 1/math.sqrt(len(ycut))
			yerr = xerr
			label = label + ', $\epsilon$:' + str("{0:.2f}".format(float(xerr)))
			plt.plot(x,y,'-',label=label,color=html[i])#yerr,xerr,'o',color=html[i])
			sigmas.append(scatter[i]*180.0/3.14)
			slopes.append(slope)
		plt.legend(loc='best',prop={'size':8})
	saveString = "LnFluxDistanceX" + save + "_withFit.png"
	fig.savefig(saveString)
	plt.close(fig)

	return [sigmas,slopes]

def plotFluxVsDepth(num_runs,num_phot,Count,Num_Rays,scatter,angles,colors,x,step,save,log,lim,xliml,xlimu,yliml,ylimu):
	html = colors
	phot_data = Count
	xData = []
	yData = []
	count = 0
	slopes = []
	sigmas = []
	z = 0
	### Pre Plot Data
	for i in range(num_runs):
		for j in range(len(phot_data[0])):
			phot_data[i][j] = float(phot_data[i][j])

	for i in range(len(phot_data[0])):
		xData.append(float(x + step*i))
	### PLOTTING
	fig = plt.figure()
	plt.xlabel('depth (m)')
	plt.ylabel('number of rays')
	plt.gca().set_ylim(bottom=0)
	if lim:
		plt.xlim(xliml,xlimu)
		plt.ylim(yliml,ylimu)
	for i in range(num_runs):
		yData = phot_data[i]
		label = '$\sigma$: ' + (str("{0:.2f}".format(scatter[i]*180.0/3.14)))
		plt.plot(xData,yData, 'o',label=label, color=html[i])
	plt.legend(loc='best',prop={'size':8})
	#plt.show()
	saveString = "FluxZ" + save + ".png"
	fig.savefig(saveString)
	plt.close(fig)
	return 0

### Pre-compile steps
def run(sl,scatter,angles,num,rang,Num_Rays,numRaysPerAngle,em_x,em_y,em_z):
	global XPHOTDATA
	global ZPHOTDATA
	global XPHOTPLOTDATA

	checkFor_executable_directiory()
	model = 'r'
	#angle = [0.0, 1.146, ]
	red = Color("red")
	html = list(red.range_to(Color("purple"),len(scatter) + 5))
	colors = []
	for i in range(len(html)):
		colors.append(str(html[i]))
	index = 0
	for i in range(len(colors)):
		if len(colors[index]) < 6:
			colors.pop(index)
			index-=1
		index+=1

	#colors = ['black','silver','rosybrown','red','darksalmon','sienna','tan','gold','darkkhaki','green','darkturquoise','navy','plum','orange','darkcyan','seagreen','darkgreen','bisque','slateblue','deeppink','wheat','magenta','powerblue','ivory']
	reflection = 'e'
	#compile(model)

	PhotometerDataX = [[],[],[],[],[],[],[],[],[],[]]
	PhotometerDataZ = [[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[]]

	checks1 = []
	checks2 = []

	num_runs = len(scatter) * len(angles)
	num_phot = 40
	dTheta = 0.1
	x = 500.0
	photx_x = 500.0
	photx_step = 50.0
	step = 50.0
	phot_areasX = [40,2,2,2,2,2,2,2,2,2]
	phot_depthsX = [-20.0,-1.0,-3.0,-5.0,-7.0,-9.0,-11.0,-13.0,-15.0,-17.0]
	phot_angleX = 0.0
	phot_seperationX = 50.0
	phot_areaZ = 1.0
	phot_initialDepthZ = -0.5
	phot_angleZ = 90.0
	phot_seperationZ = -1.0

	for i in range(len(scatter)):
		for j in range(len(angles)):
			execute(scatter[i],angles[j],angles[j] + rang, 0, sl,num,numRaysPerAngle,em_x,em_y,em_z)
			time.sleep(.1) #sometimes python runs executive commands to quickly, can cause bugs.
			num_phot = 40
			for i in range(10):
				PhotometerDataX[i].append(Photometer(Num_Rays,phot_areasX[i],photx_x,phot_depthsX[i],num_phot,phot_angleX,phot_seperationX)) #angle = 0.0 : X-Dir, 90 : Y-Dir.
			checks1.append(Photometer(Num_Rays,2.0,1400,-22.0,1,0.0,50.0))		
			checks2.append(Photometer(Num_Rays,2.0,100,-200.0,1,0.0,50.0))		
			num_phot = 100
			for i in range(20):
				PhotometerDataZ[i].append(Photometer(Num_Rays,phot_areaZ,photx_x + i*50.0,phot_initialDepthZ,num_phot,phot_angleZ,phot_seperationZ)) #angle = 0.0 : X-Dir, 90 : Y-Dir.
			call1 = "rm data/*.dat"
			os.system(call1)
			os.system('rm -rf ~/.local/share/Trash/*')


	xliml = 500
	xlimu = 1500
	yliml = 0
	ylimu = 9
	plot_data = []
	num_phot = 40
	for i in range(len(PhotometerDataX)):
		save_str = "count" + str(i) + "_z_" + str(int(phot_depthsX[i])) + "_a_" + str(int(phot_areasX[i]))
		plot_data.append(plotFluxVsDistance(num_runs,num_phot,PhotometerDataX[i],Num_Rays,scatter,angles,colors,photx_x,photx_step,save_str,True,True,xliml,xlimu,yliml,ylimu))

	num_phot = 100
	x = -0.5
	step = -1
	xliml = -100
	xlimu = 0
	yliml = 0
	ylimu = 200
	for i in range(20):
		save_str = "count" + str(i) + "_z_" + str(int(photx_x + 50.0*i))
		plotFluxVsDepth(num_runs,num_phot,PhotometerDataZ[i],Num_Rays,scatter,angles,colors,x,step,save_str,False,True,xliml,xlimu,yliml,ylimu)

	XPHOTDATA.append(PhotometerDataX)
	ZPHOTDATA.append(PhotometerDataZ)
	XPHOTPLOTDATA.append(plot_data)
	print "eiweiss"
	print checks1
	print checks2

	return 0

XPHOTDATA = []
ZPHOTDATA = []
XPHOTPLOTDATA = []

print save_dir

for j in range(len(sl)):
	for i in range(len(scatter)*len(angles)):
		call1 = 'rm data' + str(i) + "/*.dat"
		os.system(call1)
	os.system('rm -rf ~/.local/share/Trash/*')
	run(sl[j],scatter,angles,'',rang,Num_Rays,numRaysPerAngle,em_x,em_y,em_z)
	call2 = 'mkdir ' + str(sl[j])
	os.system(call2)
	call3 = 'mv *.png ' + str(sl[j]) + '/'
	os.system(call3)
	call4 = 'mv *.eps ' + str(sl[j]) + '/'
	os.system(call4)
	call5 = 'mv ' + str(sl[j]) + '/ /media/geoffrey/GeoffsTbite/'
	os.system(call5)

call2 = 'mkdir /media/geoffrey/GeoffsTbite/' + save_dir
os.system(call2)
for i in range(len(sl)):
	call3 = 'mv /media/geoffrey/GeoffsTbite/' + str(sl[i]) + '/ /media/geoffrey/GeoffsTbite/' + save_dir + '/' 
	os.system(call3)


#red = Color("yellow")
#colors = list(red.range_to(Color("purple"),len(sl)+1))
#html = []
#
#for i in range(len(colors)):
#	html.append(str(colors[i]))
#
#
#fig = plt.figure()
#for i in range(len(sl)):
#	label = 'Scattering Length: ' + str(sl[i])
#	plt.plot(fowty[i][0],fowty[i][1],'-',label=label,color=html[i])
#plt.legend(loc='best',prop={'size':8})
#plt.xlabel("STD of diffuse scatter (degrees)")
#plt.ylabel("Slope of semilog fitted attenuation lengths")
#saveString ="/media/geoffrey/GeoffsTbite/" +  save_dir + "/SigmaSTD40m.png"# + str(scatter[i]) + ".png"
##plt.show()
#fig.savefig(saveString)
#plt.close(fig)
#
#fig = plt.figure()
#for i in range(len(sl)):
#	label = 'Scattering Length: ' + str(sl[i])
#	plt.plot(twos[i][0],twos[i][1],'-',label=label,color=html[i])
#plt.legend(loc='best',prop={'size':8})
#plt.xlabel("STD of diffuse scatter (degrees)")
#plt.ylabel("Slope of semilog fitted attenuation lengths")
#saveString ="/media/geoffrey/GeoffsTbite/" +  save_dir + "/SigmaSTD2m.png"# + str(scatter[i]) + ".png"
##plt.show()
#fig.savefig(saveString)
#plt.close(fig)
#
#fig = plt.figure()
#for i in range(len(sl)):
#	label = 'Scattering Length: ' + str(sl[i])
#	plt.plot(fowty2[i][0],fowty2[i][1],'-',label=label,color=html[i])
#plt.legend(loc='best',prop={'size':8})
#plt.xlabel("STD of diffuse scatter (degrees)")
#plt.ylabel("Slope of semilog fitted attenuation lengths")
#saveString ="/media/geoffrey/GeoffsTbite/" +  save_dir + "/SigmaSTD40m2.png"# + str(scatter[i]) + ".png"
##plt.show()
#fig.savefig(saveString)
#plt.close(fig)
#
#fig = plt.figure()
#for i in range(len(sl)):
#	label = 'Scattering Length: ' + str(sl[i])
#	plt.plot(twos2[i][0],twos2[i][1],'-',label=label,color=html[i])
#plt.legend(loc='best',prop={'size':8})
#plt.xlabel("STD of diffuse scatter (degrees)")
#plt.ylabel("Slope of semilog fitted attenuation lengths")
#saveString ="/media/geoffrey/GeoffsTbite/" +  save_dir + "/SigmaSTD2m2.png"# + str(scatter[i]) + ".png"
##plt.show()
#fig.savefig(saveString)
#plt.close(fig)

os.system('mkdir Output')
FileName = 'Output/Rev' + str(sl[0]) + '.py'
call = 'touch ' + FileName
os.system(call)
file_object  = open(FileName, 'w')
file_object.write('Photometer X Data: \n')
file_object.write(str(XPHOTPLOTDATA))
file_object.write('\nPhotometer Z  Data: \n')
file_object.write(str(XPHOTPLOTDATA))
file_object.write('\nPlot Data: \n')
file_object.write(str(XPHOTPLOTDATA))