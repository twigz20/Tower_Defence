#include "CreepManager.h"
#include "LevelManager.h"
#include "CreepInfo.h"
#include <algorithm>

using namespace rapidjson;

CreepManager::CreepManager(TowerDefence* game_) :
	game(game_),
	creepAmountForWave(0),
	creepFactory(game_)
{
}


CreepManager::~CreepManager()
{
	std::for_each(creeps.begin(), creeps.end(), [](std::shared_ptr<Creep> creep) {
		creep->removeAllChildrenWithCleanup(true);
		creep->removeFromParentAndCleanup(true);
	});
}

void CreepManager::addCreep(WaveProperties waveProperties)
{
	for (int i = 0; i < waveProperties.creepQuantity; i++) {
		creeps.push_back(creepFactory.getCreep(waveProperties.creepName));
		creeps.back()->setPosition(start);
		creeps.back()->setStartDelay(waveProperties.delay);
		game->addChild(creeps.back().get());
		creepAmountForWave++;
	}
}

void CreepManager::popCreep()
{
	creepsInPlay.push_back(creeps.front());
	creeps.erase(creeps.begin());
}

std::shared_ptr<Creep> CreepManager::getNextCreep()
{
	return creeps.front();
}

bool CreepManager::hasNextCreep()
{
	return !creeps.empty();
}

CreepManager::CreepFactory::CreepFactory(TowerDefence* game_) :
	game(game_)
{
	creepInfoDoc.Parse(getFileContent(CREEP_FILE).c_str());
}

CreepManager::CreepFactory::~CreepFactory()
{
}

std::shared_ptr<Creep> CreepManager::CreepFactory::getCreep(std::string & creepName)
{
	return std::make_shared<Creep>(Creep(game, CreepInfo(creepName)));
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
	std::for_each(creeps.begin(), creeps.end(), [](std::shared_ptr<Creep> creep) {
		creep->removeAllChildrenWithCleanup(true);
		creep->removeFromParentAndCleanup(true);
	});
	creepAmountForWave = 0;
}

std::vector<std::shared_ptr<Creep>> CreepManager::getCreepsInPlay()
{
	return creepsInPlay;
}

void CreepManager::cleanUpDeadCreeps()
{
	for (auto creep = creepsInPlay.begin(); creep != creepsInPlay.end(); creep++)
	{
		if ((*creep)->isDead() || (*creep)->isMissionCompleted())
		{
			(*creep)->removeAllChildrenWithCleanup(true);
			(*creep)->removeFromParentAndCleanup(true);
			creep = creepsInPlay.erase(creep);

			game->getLevelManager()->decreaseCreepAmount();

			if (creep == creepsInPlay.end())
			{
				break;
			}
		}
	}
}

void CreepManager::clearCreeps()
{
	creepsInPlay.clear();
}

void CreepManager::update(float deltaTime)
{
	if (hasNextCreep())
	{
		float time = timer.GetTicks();
		float startDelay = getNextCreep()->getStartDelay() * 1000;
		if (time >= startDelay) {
			getNextCreep()->moveToward(end);
			popCreep();
			timer.Reset();
		}
	}

	cleanUpDeadCreeps();
}

void CreepManager::startCreepTimer()
{
	timer.Start();
}
