#include "TurretManager.h"
#include "TowerDefenceScene.h"
#include "LevelManager.h"

#include <sstream>

using namespace rapidjson;

TurretManager::TurretManager(TowerDefence * scene_, LevelManager * levelManager_) :
	scene(scene_),
	levelManager(levelManager_)
{
	loadStarterTurrets();
}

TurretManager::~TurretManager()
{
}

void TurretManager::addTurret(Turret *turret2)
{
	Turret *turret = new Turret(turret2->getTurretInfo().image);
	turret->getTurretInfo() = turret2->getTurretInfo();
	turret->getTurretStats() = turret2->getTurretStats();
	turret->getObject()->setPosition(turret2->getObject()->getPosition());
	turrets.push_back(std::move(turret));
	scene->addChild(turrets.back()->getObject(), 0);
}

void TurretManager::update(float deltaTime)
{
}

TurretFactory::TurretFactory()
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

Turret * TurretFactory::getTurret(std::string & turretName)
{
	const Value& turretInfo = turretInfoDoc[turretName.c_str()];

	Turret *turret = new Turret(turretInfo["image"].GetString());
	turret->getTurretInfo().name = turretInfo["name"].GetString();
	turret->getTurretInfo().bullet = turretInfo["bullet_image"].GetString();
	turret->getTurretInfo().image = turretInfo["image"].GetString();
	turret->getTurretStats().damage = turretInfo["damage"].GetInt();
	turret->getTurretStats().cooldown = turretInfo["cooldown"].GetFloat();
	turret->getTurretStats().range = turretInfo["range"].GetInt();
	turret->getTurretStats().cost = turretInfo["cost"].GetInt();
	turret->getObject()->setName(turret->getTurretInfo().name);
	turret->getObject()->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));

	return turret;
}

void TurretManager::loadStarterTurrets()
{
	bool moreTurrets = true;
	int turretNumber = 1;
	std::stringstream turretName;
	turretName << "turret-" << turretNumber;
	while (moreTurrets) {
		if (turretFactory.turretExist(turretName.str())) {
			starterTurrets.push_back(turretFactory.getTurret(turretName.str()));
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

bool TurretManager::hasTurretAtCoord(cocos2d::Vec2 coord)
{
	bool hasTurret = false;
	for (int i = 0; i < turrets.size(); i++) {
		if (turrets[i]->getObject()->getPosition() == coord)
			hasTurret = true;
	}
	return hasTurret;
}
