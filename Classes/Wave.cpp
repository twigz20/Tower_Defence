#include "Wave.h"



Wave::Wave()
{
}

Wave::~Wave()
{
	clearWave();
}

void Wave::setWave(Wave _wave)
{
	while (_wave.hasNextWaveSection()) {
		wave.push(_wave.getNextWaveSection());
		_wave.popWave();
	}
}

void Wave::popWave()
{
	wave.pop();
}

void Wave::clearWave()
{
	std::queue<WaveProperties> empty;
	std::swap(wave, empty);
}

void Wave::addWaveProperties(WaveProperties waveMinionProperties)
{
	wave.push(waveMinionProperties);
}

const WaveProperties Wave::getNextWaveSection()
{
	return wave.front();
}

bool Wave::hasNextWaveSection()
{
	return !wave.empty();
}
