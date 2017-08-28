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
import math as m
import os
from colour import Color
import time

inital_angle = 20.0
final_angle = 49.95
numRays = 3000
numRaysPerAngle = 10
sigmas = [0.0,0.01]
sl = [1000]
executable_name_ext = '' # EX: RunPropagatorGeoffs  -> executable_name_ext = Geoffs


def ploting(sav,NumRays,offset):
	start_time = time.time()
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
		col = int(m.floor(i/10.0))
		for j in range(len(fileI)):
			xpos.append(float(fileI[j].split(" ")[0]))
			zpos.append(float(fileI[j].split(" ")[1]))
		if(NumRays > 150):
			alpha = (1.0/NumRays)*80
			plt.plot(xpos,zpos,color=colors[col],alpha=alpha)
		else:
			alpha = (1.0/NumRays)*20
			plt.plot(xpos,zpos,color=colors[0],alpha=alpha)
	plt.grid()
	#plt.show()
	fig.savefig(saveString)
	plt.close(fig)
	

def select_model(): #User inputs desired model (selects which file to use as main)
	print ''
	print 'Please select model, Options:'
	print 'r: Spice/Rice model'
	print 'm: Moores Bay Model'
	model = str(raw_input())
	if (model != 'r' and model != 'm'): #Forces user to pick valid option
		print ''
		os.system('echo option invalid')
		model = select_model()
	return model  #Python 3 switches raw_input to just input. However, input evaluates expession.

def create_compilation_string(executable_name_ext):
	nonModelFiles = []
	nonModelFiles_index = 0
	if (model != 'r'):
		nonModelFiles.append('RunPropagator.cc')
		nonModelFiles_index += 1
	if (model != 'm'): #Forces user to pick valid option
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
	osString = 'g++ ' + ccString + '-std=c++0x -o RunPropagator' + executable_name_ext
	return osString

def checkFor_executable_directiory():
	if not os.path.isdir("data"): #checks if data directory exists in working directory, if not, creates it.
		os.system('mkdir data')
	else:
		os.system('rm data/*.dat')
	#if os.path.exists("RunPropagator"): #Checks is pre-compiled binary file exists, if so, deletes it <- helps for debugging
	#	os.system('rm RunPropagator')

### Run executable with specifed scattering
def execute(scatter, a1, a2, sl,executable_name_ext,numRaysPerAngle):
	time.sleep(.1) #sometimes python runs executive commands to quickly, can cause bugs.
	executable = './RunPropagator' + executable_name_ext + ' ' + str(scatter) + ' ' + str(2) + ' ' + str(a1) + ' ' + str(a2) + ' ' + str(sl) + ' ' + str(numRaysPerAngle) + ' ' + str(100.0) + ' ' + str(0.0) + ' ' + str(-200.0)
	os.system(executable) #termianl command

### Compiles specific model
def compile(model,executable_name_ext):
	osString = create_compilation_string(executable_name_ext)
	print ''
	if model == 'r':
		print 'Compiling Spice/Rice model'
		print'CPP Compilation Errors:'
		os.system(osString) #termianl command
	if model == 'm':
		print 'Compiling Moores Bay Model'
		print'CPP Compilation Errors:'
		os.system(osString)

def run(inital_angle,final_angle,executable_name_ext,numRays,numRaysPerAngle,sigmas,sl):
	
	rayNumOffset = 0
	for i in range(len(sl)):
		for j in range(len(sigmas)):
			save_str = "RayPaths_angI_" + executable_name_ext + "_" + str(int(inital_angle)) + "_SL_" + str(sl[i]) + "_sigma_" + str(int(sigmas[j])) 	
			execute(sigmas[j],inital_angle,final_angle,sl[i],executable_name_ext,numRaysPerAngle)
			ploting(save_str,numRays,rayNumOffset)
			call1 = "rm data/*.dat"
			os.system(call1)
			os.system('rm -rf ~/.local/share/Trash/*')


model = 'r'
checkFor_executable_directiory()
compile(model,executable_name_ext)

### Run Sim and Plot Paths
run(inital_angle,final_angle,executable_name_ext,numRays,numRaysPerAngle,sigmas,sl)