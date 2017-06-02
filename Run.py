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


import os
import time

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

def select_scatter(): #User inputs desired scatter (Sets scatter_type in cpp files)
	print ''
	print 'Please select scattering type, Options:'
	print 's: specular reflections'
	print 'l: lambertian diffuse reflections (gaussian dist. about specular angle)'
	scatter = str(raw_input())
	if (scatter != 's' and scatter != 'l'): #Forces user to pick valid option
		print ''
		os.system('echo option invalid')
		scatter = select_scatter()
	return scatter

def create_compilation_string():
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
	osString = 'g++ ' + ccString + ' -o RunPropagator'
	return osString

def checkFor_executable_directiory():
	if not os.path.isdir("data"): #checks if data directory exists in working directory, if not, creates it.
		os.system('mkdir data')
	if os.path.exists("RunPropagator"): #Checks is pre-compiled binary file exists, if so, deletes it <- helps for debugging
		os.system('rm RunPropagator')

### Pre-compile steps
checkFor_executable_directiory()
model = select_model()
scatter = select_scatter()

### Compiles specific model
osString = create_compilation_string()
print ''
if model == 'r':
	print 'Compiling Spice/Rice model'
	print'CPP Compilation Errors:'
	os.system(osString) #termianl command
if model == 'm':
	print 'Compiling Moores Bay Model'
	print'CPP Compilation Errors:'
	os.system(osString)

time.sleep(.1) #sometimes python runs executive commands to quickly, can cause bugs.

### Run executable with specifed scattering
print ''
if scatter == 's':
	print 'Running specual scatters only'
	print 'CPP Program Output:'
	os.system('./RunPropagator 1') #termianl command
if scatter == 'l':
	print 'Running lamberitan diffuse scattering'
	print 'CPP Program Output:'
	os.system('./RunPropagator 2') #termianl command