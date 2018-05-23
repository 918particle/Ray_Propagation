#include <iostream>
#include <cmath>
#include <cstdlib>
using namespace std;

bool F(float*,float,float,float);

int main(int argc, char **argv)
{
	//Inputs
	float theta_i = atof(argv[1])*3.14159/180.0;
	float delta_theta = atof(argv[2])*3.14159/180.0;
	float theta_f = atof(argv[3])*3.14159/180.0;
	float z_i = atof(argv[4]);
	float delta_z = atof(argv[5]);
	float z_f = atof(argv[6]);
	float C_1 = atof(argv[7]);
	//Constants
	float n_ice = 1.78;
	float n_snow = 1.29;
	float delta_n = n_ice-n_snow;
	float z_0 = 70.0;
	float *out = new float;
	for(float theta=theta_i;theta<=theta_f;theta+=delta_theta)
	{
		for(float z=z_i;z<=z_f;z+=delta_z)
		{
			float gamma = delta_n * exp(z/z_0);
			if(F(out,gamma,n_ice,theta))
			{
				float y_plus = (z_0 / tan(theta) * log(*out)+C_1*z_0)*0.6;
				float y_minus = -y_plus;
				cout<<" "<<y_plus<<" "<<y_minus<<" "<<z<<endl;
			}
		}
	}
}

bool F(float *out,float gamma,float n_ice, float theta_i)
{
	float c = n_ice*n_ice*sin(theta_i)*sin(theta_i);
	float b = 2.0*n_ice;
	if((gamma*gamma-b*gamma+c)>0)
	{
		*out = gamma/(2*sqrt(c)*sqrt(gamma*gamma-b*gamma+c)-b*gamma+2.0*c);
		return true;
	}
	else return false;
}