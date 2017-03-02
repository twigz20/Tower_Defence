#pragma once
#include "rapidjson/document.h"
#include "Utils.h"

#include <algorithm>
#include <iostream>
#include <vector>

#include "cocos2d.h"
#include "Turret.h"

class TowerDefece;
class LevelManager;

class TurretFactory {
	rapidjson::Document turretInfoDoc;
	TowerDefence *game;
public:
	TurretFactory(TowerDefence *game_);
	~TurretFactory();

	bool turretExist(std::string& turretName);
	Turret *getTurret(std::string& turretName, bool isStarterTurret = false);
};

class TurretManager
{
	std::vector<Turret*> starterTurrets;
	std::vector<Turret*> turrets;

	TowerDefence *game;

	TurretFactory turretFactory;
	int currentSelectedTurret;

	void loadStarterTurrets();
	void showStarterTurrets();

	int turretTag;
public:
	TurretManager(TowerDefence* game_);
	~TurretManager();

	void addTurret(Turret *turret);
	std::vector<Turret*> getStarterTurrets();
	std::vector<Turret*> getPlacedTurrets();

	bool hasTurretAtCoord(cocos2d::Vec2 coord);
	void showSelectedTurretRange();
	void hideSelectedTurretRange();
	void hideAllTurretRanges();
	void hideAllTurretStats();
	bool isEmpty();
	bool isATurretSelected();
	void selectTurret(int index);
	Turret* getSelectedTurret();
	void unselectTurret();
};


