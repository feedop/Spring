export module simulation:engine;

import std;
import :config;
import :data;

export class SimulationEngine
{
public:
	SimulationEngine(const SimulationConfig& config, const SimulationData& data) :config(config), data(data)
	{}

	void Start()
	{
		running = true;
		simulationThread = std::thread([this]() { threadFunction(); });
	}

	void Stop()
	{
		running = false;
		simulationThread.join();
	}

	void setConfig(const SimulationConfig& config)
	{
		const std::lock_guard<std::mutex> lock(configMutex);
		this->config = config;
	}

	void setData(const SimulationData& data)
	{
		Stop();
		this->data = data;
		Start();
	}

	SimulationData getData()
	{
		const std::lock_guard<std::mutex> lock(dataMutex);
		return data;
	}

private:
	friend class GuiController;

	SimulationConfig config;
	SimulationData data;

	std::mutex configMutex, dataMutex;
	std::atomic_bool running = false;
	std::thread simulationThread;

	void threadFunction()
	{
		SimulationData localData = data;
		SimulationConfig localConfig;
		while (running)
		{
			auto clockBegin = std::chrono::steady_clock::now();

			// Copy config to local
			{
				const std::lock_guard<std::mutex> lock(configMutex);
				localConfig = config;
			}

			// Validate data
			/*if (std::isnan(localData.position))
				std::cerr << "Position NaN\n";
			if (std::isnan(localData.velocity))
				std::cerr << "Velocity NaN\n";*/

			// Calculate forces
			localData.w = localConfig.w(localData.t);
			localData.f = localConfig.c * (localData.w - localData.position);
			localData.g = -localConfig.k * localData.velocity;
			localData.h = localConfig.h(localData.t);

			// First equation: dx/dt = v
			double dx_dt = localData.velocity;
			// Second equation: dv/dt = (c * (w - x) - k * v + h(t)) / m
			double dv_dt = (localData.f + localData.g + localData.h) / localConfig.m;

			// Update x and v using the Euler method
			localData.position += dx_dt * localConfig.dt;  // x(t + dt) = x(t) + dx/dt * dt
			localData.velocity += dv_dt * localConfig.dt;  // v(t + dt) = v(t) + dv/dt * dt
			localData.acceleration = dv_dt;

			localData.t += localConfig.dt;

			// Copy data and sleep
			{
				const std::lock_guard<std::mutex> lock(dataMutex);
				data = localData;
			}
			
			int ns = localConfig.dt * 1e9;
			auto clockEnd = std::chrono::steady_clock::now();
			int elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(clockEnd - clockBegin).count();
			if (elapsed < ns)
				std::this_thread::sleep_for(std::chrono::nanoseconds(ns - elapsed));
			else
				std::cerr << "Can't sleep - the simulation step took too long\n";
		}
	}
};