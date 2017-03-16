#pragma once
#include "rapidjson/document.h"
#include "Utils.h"

#include <iostream>
#include <vector>

#include "cocos2d.h"
#include "Wave.h"

class WaveManager
{
	rapidjson::Document document;
	std::queue<Wave> waves;
	int maxWavesForLevel;
public:
	WaveManager();
	~WaveManager();

	Wave getNextWave();
	void popWave();
	bool hasNextWave();

	void reloadWaves();
	int getMaxWavesForLeveL();
};

