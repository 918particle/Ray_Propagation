#include <iostream>
#include <cmath>
#include <cstdlib>
using namespace std;

int main(int argc, char **argv)
{
	float delta_n = atof(argv[1])-atof(argv[2]);
	float n_ice = atof(argv[1]);
	float z_0 = atof(argv[3]);
	float C_0 = atof(argv[4]);
	// float C_1 = atof(argv[5]);
	float C_1 = sqrt((n_ice/C_0)*(n_ice/C_0)-1.0);
	float b = 2.0 * n_ice;
	float c = n_ice*n_ice - 1/C_0/C_0;
	if(c>0)
	{
		for(float z=-500.0;z<=-100.0;z+=0.5)
		{
			float gamma = delta_n * exp(z/z_0);
			float y_0 = 1.0/C_0 * 1.0/sqrt(c) * z_0;
			float k = log( (gamma) / (2.0*sqrt(c)*sqrt(gamma*gamma-b*gamma+c) - b*gamma + 2.0*c) );
			float branch_1 = y_0*k-z_0*C_1;
			float branch_2 = -y_0*k+z_0*C_1;
			cout<<branch_1<<" "<<branch_2<<" "<<z<<endl;
		}
	}
	else cout<<"c < 0"<<endl;
}