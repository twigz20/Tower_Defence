#include "CreepManager.h"
#include "LevelManager.h"

using namespace rapidjson;

CreepManager::CreepManager(TowerDefence* _scene, LevelManager *levelManager_) :
	scene(_scene),
	levelManager(levelManager_),
	creepAmountForWave(0),
	creepTag(0)
{
}


CreepManager::~CreepManager()
{
	std::queue<Creep*> empty;
	std::swap(creeps, empty);
}

void CreepManager::addCreep(WaveProperties waveProperties)
{
	for (int i = 0; i < waveProperties.creepQuantity; i++) {
		creeps.push(std::move(creepFactory.getCreep(waveProperties.creepName, waveProperties.delay, levelManager, start)));
		creeps.back()->getObject()->setTag(creepTag++);
		creepAmountForWave++;
	}
}

void CreepManager::popCreep()
{

	creepsInPlay.push_back(creeps.front());
	creeps.pop();
}

Creep* CreepManager::getNextCreep()
{
	return creeps.front();
}

bool CreepManager::hasNextCreep()
{
	return !creeps.empty();
}

CreepManager::CreepFactory::CreepFactory()
{
	creepInfoDoc.Parse(getFileContent(CREEP_FILE).c_str());
}

CreepManager::CreepFactory::~CreepFactory()
{
}

Creep *CreepManager::CreepFactory::getCreep(std::string & creepName, float startDelay, LevelManager *levelManager_, cocos2d::Vec2 start)
{
	const rapidjson::Value& creepInfo = creepInfoDoc[creepName.c_str()];

	Creep *creep = new Creep(creepInfo["source"].GetString());
	creep->setHealth(creepInfo["health"].GetInt());
	creep->setSpeed(creepInfo["speed"].GetInt());
	creep->setGold(creepInfo["gold"].GetInt());
	creep->setStartDelay(startDelay);
	creep->setPosition(levelManager_->getStartPoint().x, levelManager_->getStartPoint().y);
	creep->setLevelManager(levelManager_);

	levelManager_->getMap()->addChild(creep->getObject());

	return creep;
}

void CreepManager::setWayPoints(cocos2d::Vec2 start, cocos2d::Vec2 end)
{
	this->start = start;
	this->end = end;
}

int CreepManager::getCreepAmountForWave()
{
	return creepAmountForWave;
}

void CreepManager::clearManager()
{
	std::queue<Creep*> empty;
	std::swap(creeps, empty);
	creepAmountForWave = 0;
}

std::vector<Creep*> CreepManager::getCreepsInPlay()
{
	return creepsInPlay;
}

void CreepManager::cleanUpCreeps()
{
	for (int i = 0; i < creepsInPlay.size(); i++) {
		delete creepsInPlay[i];
	}

	creepsInPlay.clear();
}
