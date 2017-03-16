#include "WaveManager.h"
#include "Utils.h"
#include "picojson.h"
#include "cocos2d.h"

#define COCOS2D_DEBUG 1

using namespace rapidjson;

WaveManager::WaveManager()
{
	auto fileData = cocos2d::FileUtils::getInstance()->getDataFromFile(WAVES_FILE);
	std::string fileStr((const char*)fileData.getBytes(), fileData.getSize());
	document.Parse(fileStr.c_str());

	const Value& wavesValue = document["Waves"];
	assert(wavesValue.IsArray());
	maxWavesForLevel = wavesValue.Size();
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

	/*auto fileData = cocos2d::FileUtils::getInstance()->getDataFromFile(WAVES_FILE);
	std::string fileStr((const char*)fileData.getBytes(), fileData.getSize());

	//std::string fileStr = getFileContent(WAVES_FILE).c_str();
	picojson::value v;
	std::string err;
	picojson::parse(v, fileStr.begin(), fileStr.end(), &err);

	picojson::object& obj = v.get<picojson::object>();
	auto wavesArray = obj["Waves"].get<picojson::array>();

	for (picojson::array::const_iterator i = wavesArray.begin(); i != wavesArray.end(); ++i)
	{

		Wave w;
		auto waveObj = (*i).get<picojson::object>();
		auto creepsArray = waveObj["Wave"].get<picojson::array>();

		for (picojson::array::const_iterator creeps = creepsArray.begin(); creeps != creepsArray.end(); ++creeps)
		{
			auto creepObj = (*creeps).get<picojson::object>();
			WaveProperties waveMinionProperties;
			waveMinionProperties.creepName = creepObj["minion"].get<std::string>();
			waveMinionProperties.creepQuantity = creepObj["quantity"].get<double>();
			waveMinionProperties.delay = creepObj["delay"].get<double>();
			w.addWaveProperties(waveMinionProperties);
		}
		waves.push(w);
	}*/
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

void WaveManager::reloadWaves()
{
	std::queue<Wave> empty;
	std::swap(waves, empty);

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

int WaveManager::getMaxWavesForLeveL()
{
	return maxWavesForLevel;
}
