#pragma once
#include "Creep.h"
#include <queue>
#include "rapidjson/document.h"
#include "Utils.h"
#include "Wave.h"
#include "TowerDefenceScene.h"

class CreepManager
{
	class CreepFactory {
		rapidjson::Document creepInfoDoc;
	public:
		CreepFactory();
		~CreepFactory();

		Creep *getCreep(std::string& creepName, float startDelay, LevelManager *levelManager_, cocos2d::Vec2 end);
	};

	CreepFactory creepFactory;
	std::queue<Creep*> creeps;

	TowerDefence* scene;
	LevelManager *levelManager;

	cocos2d::Vec2 start, end;
	int creepAmountForWave;
public:
	CreepManager(TowerDefence* scene, LevelManager *levelManager);
	~CreepManager();

	void addCreep(WaveProperties waveProperties);
	void popCreep();
	Creep* getNextCreep();
	bool hasNextCreep();

	void setWayPoints(cocos2d::Vec2 start, cocos2d::Vec2 end);
	int getCreepAmountForWave();
	void clearManager();
};

