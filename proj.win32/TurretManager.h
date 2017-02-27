#pragma once
#include "rapidjson/document.h"
#include "Utils.h"

#include <iostream>
#include <vector>

#include "cocos2d.h"
#include "Turret.h"

class TowerDefece;
class LevelManager;

class TurretFactory {
	rapidjson::Document turretInfoDoc;
public:
	TurretFactory();
	~TurretFactory();

	bool turretExist(std::string& turretName);
	Turret *getTurret(std::string& turretName);
};

class TurretManager
{
	std::vector<Turret*> starterTurrets;
	std::vector<Turret*> turrets;

	TowerDefence *scene;
	LevelManager *levelManager;

	TurretFactory turretFactory;
	int currentSelectedTurret;
public:
	TurretManager(TowerDefence* scene_, LevelManager *levelManager_);
	~TurretManager();

	void addTurret(Turret *turret);
	void update(float deltaTime);

	void loadStarterTurrets();
	std::vector<Turret*> getStarterTurrets();
	std::vector<Turret*> getPlacedTurrets();

	bool hasTurretAtCoord(cocos2d::Vec2 coord);
	void showSelectedTurretRange();
	void hideSelectedTurretRange();
	bool isEmpty();
	bool isATurretSelected();
	void selectTurret(int index);
	void unselectTurret();

	bool checkCollision(int index, cocos2d::Rect rect);
	void cleanUpTargets();
};


