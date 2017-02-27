#include "WaveManager.h"
#include "Utils.h"

#define COCOS2D_DEBUG 1

using namespace rapidjson;

WaveManager::WaveManager()
{
	document.Parse(getFileContent(WAVES_FILE).c_str());

	const Value& wavesValue = document["Waves"];
	assert(wavesValue.IsArray());

	for (SizeType i = 0; i < wavesValue.Size(); i++) { // Uses SizeType instead of size_t
		const Value& wavesInfo = wavesValue[i]["Wave"];
		assert(wavesInfo.IsArray());
		Wave w;
		for (SizeType j = 0; j < wavesInfo.Size(); j++) {
			WaveProperties waveMinionProperties;
			waveMinionProperties.creepName = wavesInfo[j]["minion"].GetString();
			waveMinionProperties.creepQuantity = wavesInfo[j]["quantity"].GetInt();
			waveMinionProperties.delay = wavesInfo[j]["delay"].GetFloat();
			w.addWaveProperties(waveMinionProperties);
		}
		waves.push(w);
	}
}


WaveManager::~WaveManager()
{
	std::queue<Wave> empty;
	std::swap(waves, empty);
}

Wave WaveManager::getNextWave()
{
	return waves.front();
}

void WaveManager::popWave()
{
	waves.pop();
}

bool WaveManager::hasNextWave()
{
	return !waves.empty();
}
