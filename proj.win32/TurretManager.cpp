#include "TurretManager.h"
#include "TowerDefenceScene.h"
#include "LevelManager.h"
#include "TurretInfo.h"

#include <sstream>

using namespace rapidjson;

TurretManager::TurretManager(TowerDefence * game_) :
	game(game_),
	currentSelectedTurret(-1),
	turretFactory(game)
{
	loadStarterTurrets();
}

TurretManager::~TurretManager()
{
}

void TurretManager::addTurret(Turret *turret_)
{
	turrets.push_back(new Turret(std::move(*turret_)));
	turrets.back()->activateTurret();
	game->addChild(turrets.back(), 0);
}

TurretFactory::TurretFactory(TowerDefence *game_) :
	game(game_)
{
	turretInfoDoc.Parse(getFileContent(TURRET_FILE).c_str());
}

TurretFactory::~TurretFactory()
{
}

bool TurretFactory::turretExist(std::string & turretName)
{
	return turretInfoDoc.HasMember(turretName.c_str());
}

Turret * TurretFactory::getTurret(std::string & turretName, bool isStarterTurret)
{
	TurretInfo *info = new TurretInfo(turretName);

	Turret *turret = new Turret(isStarterTurret);
	turret->nodeWithTheGame(game, info);

	return std::move(turret);
}

void TurretManager::loadStarterTurrets()
{
	bool moreTurrets = true;
	int turretNumber = 1;
	std::stringstream turretName;
	turretName << "turret-" << turretNumber;
	while (moreTurrets) {
		if (turretFactory.turretExist(turretName.str())) {
			starterTurrets.push_back(turretFactory.getTurret(turretName.str(), true));
			turretName.str(std::string());
			turretName << "turret-" << ++turretNumber;
		}
		else {
			moreTurrets = false;
		}
	}
}

std::vector<Turret*> TurretManager::getStarterTurrets()
{
	return starterTurrets;
}

std::vector<Turret*> TurretManager::getPlacedTurrets()
{
	return turrets;
}

bool TurretManager::hasTurretAtCoord(cocos2d::Vec2 coord)
{
	bool hasTurret = false;
	for (int i = 0; i < turrets.size(); i++) {
		if (turrets[i]->getPosition() == coord)
			hasTurret = true;
	}
	return hasTurret;
}

void TurretManager::showSelectedTurretRange()
{
	turrets[currentSelectedTurret]->showRange();
}

void TurretManager::hideSelectedTurretRange()
{
	turrets[currentSelectedTurret]->hideRange();
}

bool TurretManager::isEmpty()
{
	return turrets.empty();
}

bool TurretManager::isATurretSelected()
{
	return currentSelectedTurret == -1;
}

void TurretManager::selectTurret(int index)
{
	currentSelectedTurret = index;
}

Turret * TurretManager::getSelectedTurret()
{
	return turrets[currentSelectedTurret];
}

void TurretManager::unselectTurret()
{
	currentSelectedTurret = -1;
}
