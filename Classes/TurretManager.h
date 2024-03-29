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

	bool turretExist(std::string turretName);
	Turret getTurret(std::string turretName, bool isStarterTurret = false);
};

class TurretManager
{
	TowerDefence *game;
	TurretFactory turretFactory;
	std::vector<Turret*> starterTurrets;
	std::vector<Turret*> turrets;

	int currentSelectedTurret;
	int currentMouseOverTurret;

	void loadStarterTurrets();

	int turretTag;
public:
	TurretManager(TowerDefence* game);
	~TurretManager();

	void reset();
	void update(float deltaTime);
	void addTurret(Turret *turret);
	std::vector<Turret*> getStarterTurrets();
	std::vector<Turret *> getPlacedTurrets();

	bool hasTurretAtCoord(cocos2d::Vec2 coord);
	void showSelectedTurretRange();
	void hideSelectedTurretRange();
	void hideAllTurretRanges();
	void hideAllTurretStats();
	void hideAllTurretHelpers();
	void showSelectedTurretHelpers();
	void hideSelectedTurretHelpers();
	bool isEmpty();
	bool isATurretSelected();
	int getMouseOverTurret();
	void setMouseOver(int index);
	void unsetMouseOverTurret();
	bool isMouseOverTurret();
	void selectTurret(int index);
	Turret * getSelectedTurret();
	void unselectTurret();
	void sellSelectedTurret();
};


