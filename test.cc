 #include <iostream>
#include <cmath>
#include <cstdlib>
using namespace std;

int main(int argc, char **argv)
{
	float delta_n = 1.8-1.3;
	float n_ice = 1.8;
	float z_0 = 35.0;
	float C_1 = 0.0;
	float b = 2.0 * n_ice;
	for(float C_0=-10;C_0<10;C_0+=0.01)
	{
		float c = n_ice*n_ice - 1.0/C_0/C_0;
		float y_0 = 1.0/C_0 * 1.0/sqrt(c) * z_0;
		if(c>0)
		{
			float z_i = -20.0;
			// for(float z_i=-75.0;z_i<-2;z_i+=1)
			// {
				float n = n_ice - delta_n*exp(z_i/z_0);
				float z_f = z_i+2.0;
				float c = n_ice*n_ice - 1/C_0/C_0;
				float gamma_i = delta_n * exp(z_i/z_0);
				float k_i = log( (gamma_i) / (2.0*sqrt(c)*sqrt(gamma_i*gamma_i-b*gamma_i+c) - b*gamma_i + 2.0*c) );
				float branch_1_i = y_0*k_i-z_0*C_1;
				float gamma_f = delta_n * exp(z_f/z_0);
				float k_f = log( (gamma_f) / (2.0*sqrt(c)*sqrt(gamma_f*gamma_f-b*gamma_f+c) - b*gamma_f + 2.0*c) );
				float branch_1_f = y_0*k_f-z_0*C_1;
				float delta_y = branch_1_f-branch_1_i;
				float delta_z = z_f - z_i;
				float angle = atan(delta_z/delta_y);
				if(angle>0) cout<<angle<<","<<C_0-1.0/n_ice<<endl;
				else cout<<angle<<","<<C_0+1.0/n_ice<<endl;
			// }
		}
	}
}