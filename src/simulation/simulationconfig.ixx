export module simulation:config;

import std;

export struct SimulationConfig
{
	double dt;
	float m;
	float k;
	float c;
	std::function<float(float)> w;
	std::function<float(float)> h;
};