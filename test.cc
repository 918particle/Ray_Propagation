#include <iostream>
#include <cmath>
#include <cstdlib>
using namespace std;

bool F(float*,float,float,float);

int main(int argc, char **argv)
{
	//Constants, looking at positive wing of the solution only, because we are interested in slopes.
	float theta_i = 0.0*3.14159/180.0;
	float delta_theta = 0.1*3.14159/180.0;
	float theta_f = 90.0*3.14159/180.0;
	float z_i = -450.0;
	float delta_z = 1.0;
	float z_f = 0.0;
	float C_1 = 0.0;
	//Ice properties
	float n_ice = 1.78;
	float n_snow = 1.29;
	float delta_n = n_ice-n_snow;
	float z_0 = 70.0;
	//Auxiliary variables
	float *out = new float;
	float *out_prior = new float;
	float z = z_i;
	float theta = theta_i;
	float y_prior = 0.0;
	float y = y_prior;
	float delta_y = y-y_prior;
	float theta_m=theta_i;

	while(theta<=theta_f)
	{
		z=z_i;
		while(z<=z_f)
		{
			float gamma = delta_n * exp(z/z_0);
			float gamma_prior = delta_n * exp((z-delta_z)/z_0);
			if(F(out,gamma,n_ice,theta) && F(out_prior,gamma_prior,n_ice,theta))
			{
				y = z_0 / tan(theta) * log(*out)+C_1*z_0;
				y_prior = z_0 / tan(theta) * log(*out_prior)+C_1*z_0;
				delta_y = y-y_prior;
				theta_m = atan(delta_z/delta_y);
				cout<<z<<" "<<theta*180.0/3.14159<<" "<<theta_m*180.0/3.14159<<" "<<(theta_m-theta)*180.0/3.14159<<endl;
			}
			z+=delta_z;
		}
		theta+=delta_theta;
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