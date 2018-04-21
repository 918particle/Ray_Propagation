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
	float C_1 = atof(argv[5]);
	float b = 2.0 * n_ice;
	float c = n_ice*n_ice - 1/C_0/C_0;
	for(float z=-500.0;z<=500.0;z+=1.0)
	{
		float gamma = delta_n * exp(z/z_0);
		float y = z_0/sqrt(c)/C_0*log(gamma/(2*sqrt(c)*sqrt(gamma*gamma-b*gamma+2*c)-b*gamma+2*c))-C_1;
		cout<<y<<" "<<-y<<" "<<z<<" "<<log((C_0 * n_ice + 1)/(C_0 * delta_n))<<" "<<log((C_0 * n_ice - 1)/(C_0 * delta_n))<<endl;
	}
}