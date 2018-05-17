 #include <iostream>
#include <cmath>
#include <cstdlib>
using namespace std;

int main(int argc, char **argv)
{
	// float delta_n = atof(argv[1])-atof(argv[2]);
	float delta_n = 1.8-1.3;
	// float n_ice = atof(argv[1]);
	float n_ice = 1.8;
	// float z_0 = atof(argv[3]);
	float z_0 = 35.0;
	// float C_1 = atof(argv[5]);
	float z_i = -200;
	float z_f = -100;
	float C_1 = 0.0;
	float b = 2.0 * n_ice;
	for(float C_0=1/n_ice+0.01;C_0<=2.0;C_0+=0.01)
	{
		float c = n_ice*n_ice - 1/C_0/C_0;
		float y_0 = 1.0/C_0 * 1.0/sqrt(c) * z_0;
		if(c>0)
		{

			float gamma_i = delta_n * exp(z_i/z_0);
			float k_i = log( (gamma_i) / (2.0*sqrt(c)*sqrt(gamma_i*gamma_i-b*gamma_i+c) - b*gamma_i + 2.0*c) );
			float branch_1_i = y_0*k_i-z_0*C_1;

			float gamma_f = delta_n * exp(z_f/z_0);
			float k_f = log( (gamma_f) / (2.0*sqrt(c)*sqrt(gamma_f*gamma_f-b*gamma_f+c) - b*gamma_f + 2.0*c) );
			float branch_1_f = y_0*k_f-z_0*C_1;

			float delta_y = branch_1_f-branch_1_i;
			float delta_z = z_f - z_i;
			float angle = atan(delta_z/delta_y);

			cout<<angle<<" "<<n_ice-delta_n*exp(z_i/z_0)<<" "<<n_ice-delta_n*exp(z_f/z_0)<<" "<<C_0<<endl;
		}
		else cout<<"out of range."<<endl;
	}
}

// for(float z=initialDepth;z<=finalDepth;z+=0.5)
// {
// 	float gamma = delta_n * exp(z/z_0);
// 	float y_0 = 1.0/C_0 * 1.0/sqrt(c) * z_0;
// 	float k = log( (gamma) / (2.0*sqrt(c)*sqrt(gamma*gamma-b*gamma+c) - b*gamma + 2.0*c) );
// 	float branch_1 = y_0*k-z_0*C_1;
// 	float branch_2 = -y_0*k+z_0*C_1;
// 	cout<<branch_1<<" "<<branch_2<<" "<<z<<" "<<n_ice-delta_n*exp(z/z_0)<<endl;
// }