#pragma once
#include "Creep.h"
#include <queue>
#include "rapidjson/document.h"
#include "Utils.h"
#include "Wave.h"
#include "TowerDefenceScene.h"
#include "timer.h"

class CreepManager : public cocos2d::Node
{
	class CreepFactory {
		rapidjson::Document creepInfoDoc;
		TowerDefence* game;
	public:
		CreepFactory(TowerDefence* game_);
		~CreepFactory();

		std::shared_ptr<Creep> getCreep(std::string& creepName);
	};

	CreepFactory creepFactory;
	std::vector<std::shared_ptr<Creep>> creeps;
	std::vector<std::shared_ptr<Creep>> creepsInPlay;

	TowerDefence* game;

	cocos2d::Vec2 start, end;
	int creepAmountForWave;
	SimpleTimer timer;

	void MoveToEnd(std::shared_ptr<Creep>& creep);
	void popFromCreeps(int creepIndex);

	int creepsStarted;
public:
	CreepManager(TowerDefence* game);
	~CreepManager();

	void update(float deltaTime);
	void clearManager();
	void cleanUpDeadCreeps();
	void clearCreeps();
	void setWayPoints(cocos2d::Vec2 start, cocos2d::Vec2 end);

	std::vector<std::shared_ptr<Creep>> getCreepsInPlay();
	void addCreep(WaveProperties waveProperties);
	void popCreep();
	bool hasNextCreep();
	std::shared_ptr<Creep> getNextCreep();
	int getCreepAmountForWave();
	void startCreepTimer();

	void startWave();
};