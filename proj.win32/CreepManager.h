#pragma once
#include "Creep.h"
#include <queue>
#include "rapidjson/document.h"
#include "Utils.h"
#include "Wave.h"
#include "TowerDefenceScene.h"
#include "timer.h"

class CreepManager
{
	class CreepFactory {
		rapidjson::Document creepInfoDoc;
		TowerDefence* game;
	public:
		CreepFactory(TowerDefence* game_);
		~CreepFactory();

		Creep *getCreep(std::string& creepName);
	};

	CreepFactory *creepFactory;
	std::queue<Creep*> creeps;
	std::vector<Creep*> creepsInPlay;

	TowerDefence* game;

	cocos2d::Vec2 start, end;
	int creepAmountForWave;
	SimpleTimer timer;
public:
	CreepManager(TowerDefence* game);
	~CreepManager();

	void update(float deltaTime);
	void clearManager();
	void cleanUpDeadCreeps();
	void clearCreeps();
	void setWayPoints(cocos2d::Vec2 start, cocos2d::Vec2 end);

	std::vector<Creep*> getCreepsInPlay();
	void addCreep(WaveProperties waveProperties);
	void popCreep();
	bool hasNextCreep();
	Creep* getNextCreep();
	int getCreepAmountForWave();
	void startCreepTimer();
};