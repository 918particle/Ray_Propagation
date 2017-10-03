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

global globalTime 
globalTime = time.time();

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
	osString = 'g++ ' + ccString + '-std=c++11 -o RunPropagator'
	return osString

def index(change,value,change2,value2):
	FileName = "NearFit.dat"
	fileItms = open(FileName, "r").read().splitlines()	
	FileName = "NearFitMod.dat"
	file = open(FileName, "w")
	depth = []
	index = []	
	for j in range(len(fileItms)):
		depth.append(float(fileItms[j].split(" ")[0]))
		index.append(float(fileItms[j].split(" ")[1]))
		if j == change:
			index[j] = index[j] + value
		if j == change2:
			index[j] = index[j] + value2
		file.write(str(depth[j]) + ' ')
		file.write(str(index[j]) + '\n')
	file.close()

	
def plotIndex(change,change2):	
	FileName = "NearFitMod.dat"#"Maker2/SPICE_data.dat"
	fileI = open(FileName, "r").read().splitlines()
	data = []
	count = 0
	for i in range(len(fileI)):
		characters = ''
		for j in range(len(fileI[i])):
			density = False
			if fileI[i][j] == ' ':
				data.append(float(characters))
				characters = ''
			else:
				characters += fileI[i][j]
		data.append(float(characters))
	depth = []
	density = []
	for i in range(len(data)):
		if i%2 == 0:
			depth.append(-data[i])
		else:
			density.append(data[i])#1.0+0.86*data[i]/1000.0)
	
	fig = plt.figure()	
	plt.xlabel("Depth (m)")
	plt.ylabel("Index")
	plt.plot(depth,density,'-',label="Spice data",color='blue')
	
	A = 1.78;
	B = 0.427;
	C = 0.014;
	z = np.arange(0,120,1)
	n = []
	for i in range(len(z)):
		z[i] = -z[i]
		n.append(A-B*math.exp(C*z[i]))
		
	plt.plot(z,n,'-',label="Fit",color='green')
	plt.legend()
	saveString = "XNvsZN" + str(change) + "_" + str(change2) + ".png"# + str(scatter[i]) + ".png"
	fig.savefig(saveString)
	plt.close(fig)

def checkForDirectiory(directory):
	if not os.path.isdir(directory): #checks if data directory exists in working directory, if not, creates it.
		call = 'mkdir ' + directory
		os.system(call)
	else:
		if directory == "data":
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
def binarySearch(alist, item, reverse):
	if not reverse:
		first = 0
		last = len(alist)-1
		while abs(last-first) > 1:
			midpoint = (first + last)//2
			if float(alist[midpoint].split(" ")[0]) < item:
				first = midpoint
			else:
				last = midpoint
		return [first, last]
	else:
		first = 0
		last = len(alist)-1
		while abs(last-first) > 1:
			midpoint = (first + last)//2
			if float(alist[midpoint].split(" ")[0]) < item:
				last = midpoint
			else:
				first = midpoint
		return [last, first]

def Photometer(Num_Files,Area,X,Z,num_phot,angle,Seperation,reverse_propagatian):
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
			k = binarySearch(FileI,PhotometerXAtJ,reverse_propagatian)
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
def plotRayPaths(sav,NumRays,offset):
	NumFiles = NumRays
	xliml = 0
	xlimu = 1500
	zliml = -300
	zlimu = 300
		
	saveString = sav + ".png"# + str(scatter[i]) + ".png"

	startColor = Color("red")
	html = list(startColor.range_to(Color("purple"),int(NumRays/10.0)+7))
	colors = []
	for i in range(len(html)):
		colors.append(str(html[i]))
	index = 0
	for i in range(len(colors)):
		if len(colors[index]) < 6:
			colors.pop(index)
			index-=1
		index+=1
	
	fig = plt.figure()
	plt.xlim(xliml,xlimu)
	plt.ylim(zliml,zlimu)
	plt.xlabel("Range (m)")
	plt.ylabel("Depth (m)")
	for i in range(NumFiles):
		FileName = "data/propagation_path_" + str(i + offset) + ".dat"
		fileI = open(FileName, "r").read().splitlines()	
		xpos = []
		zpos = []	
		col = int(math.floor(i/10.0))
		for j in range(len(fileI)):
			xpos.append(float(fileI[j].split(" ")[0]))
			zpos.append(float(fileI[j].split(" ")[1]))
		if(NumRays > 150):
			alpha = (1.0/NumRays)*80
			plt.plot(xpos,zpos,color=colors[col],alpha=alpha)
		else:
			alpha = (1.0/NumRays)*20
			plt.plot(xpos,zpos,color=colors[col],alpha=alpha)
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
def run(sl,scatter,angles,num,rang,Num_Rays,numRaysPerAngle,em_x,em_y,em_z,reverse_propagatian,ider,changes):
	checkForDirectiory("data")
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
	compile(model)

	PhotometerDataX = [[]]#[],[],[],[],[],[],[],[],[],[]]
	PhotometerDataZ = [[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[]]

	checks1 = []
	checks2 = []

	num_runs = len(scatter) * len(angles)
	num_phot = 40
	dTheta = 0.1
	x = 550.0
	photx_x = 500.0
	photx_step = 50.0
	step = 50.0
	phot_areasX = [2]
	phot_depthsX = [-changes[ider] - 1]
	phot_angleX = 0.0
	phot_seperationX = 50.0
	phot_areaZ = 1.0
	phot_initialDepthZ = -0.5
	phot_angleZ = 90.0
	phot_seperationZ = -1.0
	currentTime = time.time()
	for i in range(len(scatter)):
		scatter_str = ['00','0001','001','01']
		for j in range(len(angles)):
			currentTime = time.time()
			print "Time: starting cpp simulation: " + str(currentTime - globalTime) + " seconds"
			execute(scatter[i],angles[j],angles[j] + rang, 0, sl,num,numRaysPerAngle,em_x,em_y,em_z)
			currentTime = time.time()
			print "Time: After cpp Sim, befor ray plots: " + str(currentTime - globalTime) + " seconds"
			time.sleep(.1) #sometimes python runs executive commands to quickly, can cause bugs.
			#if scatter[i] == 0.0 or scatter[i] == 0.01 or scatter[i] == 0.1:
			rayNumOffset = 0
			save_str = "RayPaths_" + str(reverse_propagatian) + "_angI_" + str(int(angles[j])) + "_SL_" + str(sl) + "_sigma_" + scatter_str[i]	
			plotRayPaths(save_str,Num_Rays,rayNumOffset)
			currentTime = time.time()
			print "Time: After ray plots, befor python photometer code: " + str(currentTime - globalTime) + " seconds"
			num_phot = 40
			for i in range(len(phot_depthsX)):
				PhotometerDataX[i].append(Photometer(Num_Rays,phot_areasX[i],x,phot_depthsX[i],num_phot,phot_angleX,phot_seperationX,reverse_propagatian)) #angle = 0.0 : X-Dir, 90 : Y-Dir.
			checks1.append(Photometer(Num_Rays,2.0,1400,-22.0,1,0.0,50.0,reverse_propagatian)[0])		
			print checks1
			checks2.append(Photometer(Num_Rays,2.0,100,-200.0,1,0.0,50.0,reverse_propagatian)[0])		
			currentTime = time.time()
			print "Time: After photometer code: " + str(currentTime - globalTime) + " seconds"
			num_phot = 100
			for i in range(18):
				PhotometerDataZ[i].append(Photometer(Num_Rays,phot_areaZ,photx_x + i*50.0,phot_initialDepthZ,num_phot,phot_angleZ,phot_seperationZ,reverse_propagatian)) #angle = 0.0 : X-Dir, 90 : Y-Dir.
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
	for i in range(len(PhotometerDataZ)):
		save_str = "count" + str(i) + "_z_" + str(int(photx_x + 50.0*i))
		plotFluxVsDepth(num_runs,num_phot,PhotometerDataZ[i],Num_Rays,scatter,angles,colors,x,step,save_str,False,True,xliml,xlimu,yliml,ylimu)

	global XPHOTDATA
	global ZPHOTDATA
	global XPHOTPLOTDATA
	global TRX
	global RCV

	XPHOTDATA.append(PhotometerDataX)
	ZPHOTDATA.append(PhotometerDataZ)
	XPHOTPLOTDATA.append(plot_data)
	print "eiweiss"
	print checks1
	print checks2
	TRX.append(checks1)
	RCV.append(checks2)

	return 0

currentTime = time.time()
print "Start time: " + str(currentTime - globalTime) + " seconds"
checkForDirectiory("Output")

TRX = []
RCV = []
XPHOTDATA = []
ZPHOTDATA = []
XPHOTPLOTDATA = []

outputDir = '$RAYPROP/'
names = 'Sim_Results'  # Name of sav directory
changes = [21,19,17,15,13,10,7,5,3,2,1] # Depths to loop through for changing the index data file NearFit.dat note 20 is 20 meters below surface
for k in range(1):
	for ider in range(1):	
		change = changes[ider]  # Locaation in depth (ex: 21 meters below the surface) For artifically changing the index data file
		value = -0.01 # Amount to change index value by
		change2 = changes[ider] - 4*(k+1)
		value2 = -0.01
		index(change,value,change2,value2) # Produces the new index data file
		plotIndex(change,change2) # Makes a plot of new index data file
		sl = [1000] # Scattering length 1000 ~= 115 meters
		scatter = [0.001] # standard deviation for diffuse scatters (in radians)
		angles = [20.0] # Starting Launch angle (degrees)
		rang = 29.95  # Range of angles to lanuch at starting at "angles" form above. Goes in 0.1 degree steps. For range of 30 degrees, write 29.95. .9 because of counting from 0, and .95 for rounding error.
		Num_Rays = 299 # Do math. what are numRaysPerAngle and rang. Note for 300 rays, write 299 (count from 0)
		numRaysPerAngle = 1 # as name suggests
		em_x = 100.0 # Emitter location in meters
		em_y = 0.0
		em_z = -200.0
		reverse_propagatian = False # This is for the python photometers "Collectors". if the rays mainly travel in -x direction then set to True
		save_dir = names
		
		print "Save Directory Name: " + save_dir
		
		for j in range(len(sl)):
			os.system('rm -rf ~/.local/share/Trash/*')
			currentTime = time.time()
			run(sl[j],scatter,angles,'',rang,Num_Rays,numRaysPerAngle,em_x,em_y,em_z,reverse_propagatian,ider,changes)
			call2 = 'mkdir ' + str(sl[j])
			os.system(call2)
			call3 = 'mv *.png ' + str(sl[j]) + '/'
			os.system(call3)
			call5 = 'mv ' + str(sl[j]) + '/ ' + outputDir
			os.system(call5)
		
		call2 = 'mkdir ' + outputDir + save_dir
		os.system(call2)
		for i in range(len(sl)):
			call3 = 'mv ' + outputDir + str(sl[i]) + '/ ' + outputDir + save_dir + '/' 
			os.system(call3)
	
		
	FileName = 'Output/' + names + '.py'
	call = 'touch ' + FileName
	os.system(call)
	file_object  = open(FileName, 'w')
	file_object.write('Photometer X Data: \n')
	file_object.write(str(XPHOTDATA))
	file_object.write('\nPhotometer Z  Data: \n')
	file_object.write(str(ZPHOTDATA))
	file_object.write('\nPlot Data: \n')
	file_object.write(str(XPHOTPLOTDATA))
	file_object.write('\nTRX: \n')
	file_object.write(str(TRX))
	file_object.write('\nRCV: \n')
	file_object.write(str(RCV))
	file_object.close()
	call4 = 'mv Output ' + outputDir + save_dir 
	os.system(call4)
	
currentTime = time.time()
print "Final time: " + str(currentTime - globalTime) + " seconds"