#include "CreepManager.h"
#include "LevelManager.h"
#include "CreepInfo.h"

using namespace rapidjson;

CreepManager::CreepManager(TowerDefence* game_) :
	game(game_),
	creepAmountForWave(0)
{
	creepFactory = new CreepFactory(game);
}


CreepManager::~CreepManager()
{
	std::queue<Creep*> empty;
	std::swap(creeps, empty);

	if(creepFactory)
		delete creepFactory;
}

void CreepManager::addCreep(WaveProperties waveProperties)
{
	for (int i = 0; i < waveProperties.creepQuantity; i++) {
		creeps.push(std::move(creepFactory->getCreep(waveProperties.creepName)));
		creeps.back()->setPosition(start);
		creeps.back()->setStartDelay(waveProperties.delay);
		game->addChild(creeps.back());
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

CreepManager::CreepFactory::CreepFactory(TowerDefence* game_) :
	game(game_)
{
	creepInfoDoc.Parse(getFileContent(CREEP_FILE).c_str());
}

CreepManager::CreepFactory::~CreepFactory()
{
	if (game)
		delete game;
}

Creep *CreepManager::CreepFactory::getCreep(std::string & creepName)
{
	return new Creep(game, new CreepInfo(creepName));
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

void CreepManager::cleanUpDeadCreeps()
{
	for (auto creep = creepsInPlay.begin(); creep != creepsInPlay.end(); creep++)
	{
		if ((*creep)->isDead() || (*creep)->isMissionCompleted())
		{
			(*creep)->removeAllChildrenWithCleanup(true);
			(*creep)->removeFromParentAndCleanup(true);
			delete (*creep);
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
