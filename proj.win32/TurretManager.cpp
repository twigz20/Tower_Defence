#include "TurretManager.h"
#include "TowerDefenceScene.h"
#include "LevelManager.h"

#include <sstream>

using namespace rapidjson;

TurretManager::TurretManager(TowerDefence * scene_, LevelManager * levelManager_) :
	scene(scene_),
	levelManager(levelManager_),
	currentSelectedTurret(-1)
{
	loadStarterTurrets();
}

TurretManager::~TurretManager()
{
}

void TurretManager::addTurret(Turret *turret_)
{
	turrets.push_back(new Turret(std::move(*turret_)));
	turrets.back()->addCircle();
	turrets.back()->setScene(scene);
	scene->addChild(turrets.back()->getObject(), 0);
}

void TurretManager::update(float deltaTime)
{
	for (int i = 0; i < turrets.size(); i++) {
		turrets[i]->update(deltaTime);
	}
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
	const rapidjson::Value& turretInfo = turretInfoDoc[turretName.c_str()];

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
	turret->getBulletInfo().image = turretInfo["bullet_image"].GetString();
	turret->getBulletInfo().damage_to = turret->getTurretStats().damage;
	turret->getBulletInfo().damage_from = turret->getTurretStats().damage;

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

std::vector<Turret*> TurretManager::getPlacedTurrets()
{
	return turrets;
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

void TurretManager::showSelectedTurretRange()
{
	turrets[currentSelectedTurret]->showTurretRange();
}

void TurretManager::hideSelectedTurretRange()
{
	turrets[currentSelectedTurret]->hideTurretRange();
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

void TurretManager::unselectTurret()
{
	currentSelectedTurret = -1;
}

bool TurretManager::checkCollision(int index, cocos2d::Rect rect)
{
	return turrets[index]->checkCollision(rect);
}

void TurretManager::cleanUpTargets()
{
	for (int i = 0; i < turrets.size(); i++) {
		turrets[i]->removeTarget();
	}
}
