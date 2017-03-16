#pragma once
#include <queue>
#include <string>
#include <utility>

struct WaveProperties {
	std::string creepName;
	int creepQuantity;
	float delay;
};

class Wave
{
	std::queue<WaveProperties> wave;
public:
	Wave();
	~Wave();

	void setWave(Wave wave);
	void popWave();
	void clearWave();
	void addWaveProperties(WaveProperties waveMinionProperties);
	const WaveProperties getNextWaveSection();
	bool hasNextWaveSection();
};

