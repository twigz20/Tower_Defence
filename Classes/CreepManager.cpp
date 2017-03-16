#include "CreepManager.h"
#include "LevelManager.h"
#include "CreepInfo.h"
#include <algorithm>

using namespace rapidjson;


CreepManager::CreepManager(TowerDefence* game_) :
	game(game_),
	creepAmountForWave(0),
	creepFactory(game_),
	creepsStarted(0)
{
	if (init())
	{

	}
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
		creeps.back()->getCreepStatus().startDelay = waveProperties.delay;
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
		creep->stopAllActions();
		creep->unscheduleAllSelectors();
		creep->unscheduleUpdate();
		creep->unscheduleAllCallbacks();
		creep->removeAllChildrenWithCleanup(true);
		creep->removeFromParentAndCleanup(true);
		creep = nullptr;
	});
	std::for_each(creepsInPlay.begin(), creepsInPlay.end(), [](std::shared_ptr<Creep> creep) {
		creep->stopAllActions();
		creep->unscheduleAllSelectors();
		creep->unscheduleUpdate();
		creep->unscheduleAllCallbacks();
		creep->removeAllChildrenWithCleanup(true);
		creep->removeFromParentAndCleanup(true);
		creep = nullptr;
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
			game->getLevelManager()->decreaseCreepAmount();
			(*creep) = nullptr;
			creep = creepsInPlay.erase(creep);

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
	/*if (hasNextCreep())
	{
		double seconds_elapsed = static_cast<double>(timer.GetTicks()) / CLOCKS_PER_SEC;
		if (seconds_elapsed >= getNextCreep()->getCreepStatus().startDelay) {
			getNextCreep()->moveToward(end);
			popCreep();
			timer.Reset();
		}
	}*/

	/*for (int i = 0; i < creeps.size(); i++) {
		if (!creeps[i]->isCreepMoving()) {
			//this->scheduleOnce(schedule_selector(CreepManager::MoveToEnd), creeps[i]->getCreepStatus().startDelay);
			//Create delay time action

			CallFunc *runCallback = CallFunc::create(CC_CALLBACK_0(creeps[i]->moveToward, this, end));
			creeps[i]->runAction(Sequence::create(DelayTime::create(creeps[i]->getCreepStatus().startDelay), runCallback, nullptr));
			creeps[i]->setCreepMoving();
		}
	}*/

	cleanUpDeadCreeps();
}

void CreepManager::MoveToEnd(std::shared_ptr<Creep>& creep)
{
	creep->moveToward(end);
	popCreep();
}

void CreepManager::popFromCreeps(int creepIndex)
{
	creepsInPlay.push_back(creeps[creepIndex]);
	creeps.erase(creeps.begin() + creepIndex);
}

void CreepManager::startCreepTimer()
{
	timer.Start();
}

void CreepManager::startWave()
{
	float accumulatedDelay = 0.f;
	for (int i = 0; i < creeps.size(); i++) {
		accumulatedDelay += creeps[i]->getCreepStatus().startDelay;
		auto delay = DelayTime::create(accumulatedDelay);
		CallFunc *runCallback = CallFunc::create(CC_CALLBACK_0(CreepManager::MoveToEnd, this, creeps[i]));
		Sequence *actions = Sequence::create(delay, runCallback, nullptr);
		creeps[i]->runAction(actions);
	}
}
