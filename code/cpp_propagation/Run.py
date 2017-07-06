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

def select_std(): #User inputs desired scatter (Sets scatter_type in cpp files)
	print ''
	print 'Please type in desired Gaussian STD (0 = Specular reflection):'
	scatter = raw_input()
	try:
		float(scatter)
	except:
		print ''
		os.system('echo option invalid, please type in a number')
		scatter = select_std()
	return str(scatter)

def select_reflection_mothed(): #User inputs desired scatter (Sets scatter_type in cpp files)
	print ''
	print 'Please select scattering method, Options:'
	print 's: specific hard coded reflection layers'
	print 'e: every point in propagation can reflect'
	reflection = str(raw_input())
	if (reflection != 's' and reflection != 'e'): #Forces user to pick valid option
		print ''
		os.system('echo option invalid')
		reflection = select_reflection_mothed()
	return reflection

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
	else:
		os.system('rm data/*.dat')
	if os.path.exists("RunPropagator"): #Checks is pre-compiled binary file exists, if so, deletes it <- helps for debugging
		os.system('rm RunPropagator')

### Run executable with specifed scattering
def execute(scatter, reflection):
	time.sleep(.1) #sometimes python runs executive commands to quickly, can cause bugs.
	reflection_str = ''
	print ''
	try:
		int(scatter) or float(scatter)
		print 'Running specular scatters only'
		print 'CPP Program Output:'
	except:
		pass
	else:
		print 'Running diffuse scattering with std: ' + scatter
		print 'CPP Program Output:'				
	if reflection == 's':
		reflection_str = str(1)
	if reflection == 'e':
		reflection_str = str(2)
	executable = './RunPropagator ' + scatter + ' ' + reflection_str
	os.system(executable) #termianl command

### Compiles specific model
def compile(model):
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

### Pre-compile steps
checkFor_executable_directiory()
model = select_model()
scatter = select_std()
reflection = select_reflection_mothed()
compile(model)
execute(scatter, reflection)


