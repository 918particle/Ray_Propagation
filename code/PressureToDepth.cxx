#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>

std::vector<float> depth_from_pressure(std::vector<float>);
std::vector<float> index_from_density(std::vector<float>);

const float p_0 = 94.306e3; //surface pressure in Pascals
const float p_n = 101.325e3; //one atmosphere in Pascals
const float theta = -10.0; //average temperature of ice shelf/sheet in C
const float rho_0 = 0.34e3; //surface density of firn in kg/m^3
const float rho_i = 0.91670e3; //limiting ice density in kg/m^3
const float g = 9.807; //acceleration due to gravity in m/s/s
const float chi_i = 1.2e-10; //volumetric compressibility of ice in Pascals^{-1}
const float alpha_i = 5.1e-5; //coefficient of linear temperature expansion in C^{-1}
const float beta_a = 3.66e-3; //volumetric expansion of air in C^{-1}

int main(int argc, char** argv)
{
	//1 Read in density vs. pressure numbers
	std::ifstream in(argv[1]);
	float x,y;
	std::vector<float> density;
	std::vector<float> pressure;
	std::vector<float> depth;
	std::vector<float> index;
	while(in.good() && !in.eof())
	{
		in>>x; in>>y;
		density.push_back(y);
		pressure.push_back(x*1.0e5+p_n); //Data was in 10^5 Pa or bars
	}
	in.close();
	//2 Apply functions
	depth = depth_from_pressure(pressure);
	index = index_from_density(density);
	
	//3 Output numbers
	for(int i=0;i<depth.size();++i) std::cout<<-1.0*depth[i]<<" "<<index[i]<<std::endl;
	return 0;
}

std::vector<float> depth_from_pressure(std::vector<float> p)
{
	float first_term = 0.0;
	float second_term = 0.0;
	std::vector<float> result;
	std::vector<float>::iterator i;
	for(i=p.begin();i!=p.end();++i)
	{
		first_term=((*i)-p_0)/(rho_i*g)*(1-chi_i*(((*i)+p_0)/2.0-p_n))*(1+alpha_i*theta);
		second_term=(1.0/(rho_0*g)-(1-chi_i*(p_0-p_n))/(rho_i*g))*p_0*log((*i)/p_0)*(1+beta_a*theta);
		result.push_back(first_term+second_term);
	}
	return result;
}

std::vector<float> index_from_density(std::vector<float> d)
{
	std::vector<float> result;
	std::vector<float>::iterator i;
	for(i=d.begin();i!=d.end();++i)
	{
		result.push_back(1.0+0.86*(*i)/1000.0);
	}
	return result;
}
