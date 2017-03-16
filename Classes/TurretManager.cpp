#include "TurretManager.h"
#include "TowerDefenceScene.h"
#include "LevelManager.h"
#include "TurretInfo.h"

#include <sstream>
#include <algorithm>

using namespace rapidjson;

TurretManager::TurretManager(TowerDefence* game) :
	game(game),
	currentSelectedTurret(-1),
	currentMouseOverTurret(-1),
	turretFactory(game),
	turretTag(0)
{
	loadStarterTurrets();
}

TurretManager::~TurretManager()
{
	turrets.clear();
	starterTurrets.clear();
}

void TurretManager::reset()
{
	std::for_each(turrets.begin(), turrets.end(), [](Turret* turret) {
		turret->setActive(false);
		turret->stopAllActions();
		turret->unscheduleAllSelectors();
		turret->unscheduleUpdate();
		turret->unscheduleAllCallbacks();
		turret->lostSightOfEnemy();
		turret->removeAllChildrenWithCleanup(true);
		turret->removeFromParentAndCleanup(true);
	});
	turrets.clear();

	loadStarterTurrets();
}

void TurretManager::update(float deltaTime)
{
	for (auto turret = turrets.begin(); turret != turrets.end(); turret++)
	{
		if ((*turret)->getActive())
			(*turret)->update(deltaTime); 

		if ((*turret)->isSold())
		{
			(*turret)->removeAllChildrenWithCleanup(true);
			(*turret)->removeFromParentAndCleanup(true);
			game->getLevelManager()->decreaseCreepAmount();
			turret = turrets.erase(turret);

			if (turret == turrets.end())
			{
				break;
			}
		}
	}
}

void TurretManager::addTurret(Turret *turret_)
{
	turrets.push_back(new Turret(game, TurretInfo(turret_->getTurretInfo().imageHeader), false));
	turrets.back()->setPosition(turret_->getPosition());
	turrets.back()->setTag(turretTag++);
	turrets.back()->setActive(true);
	game->addChild(turrets.back(), 0);
}

TurretFactory::TurretFactory(TowerDefence *game_) :
	game(game_)
{
	auto fileData = cocos2d::FileUtils::getInstance()->getDataFromFile(TURRET_FILE);
	std::string fileStr((const char*)fileData.getBytes(), fileData.getSize());
	turretInfoDoc.Parse(fileStr.c_str());
}

TurretFactory::~TurretFactory()
{
}

bool TurretFactory::turretExist(std::string turretName)
{
	return turretInfoDoc.HasMember(turretName.c_str());
}

Turret TurretFactory::getTurret(std::string turretName, bool isStarterTurret)
{
	return Turret(game, TurretInfo(turretName), isStarterTurret);
}

void TurretManager::loadStarterTurrets()
{
	bool moreTurrets = true;
	int turretNumber = 1;
	std::stringstream turretName;
	turretName << "turret-" << turretNumber;
	while (moreTurrets) {
		if (turretFactory.turretExist(turretName.str())) {
			starterTurrets.push_back(new Turret(game, TurretInfo(turretName.str()), true));
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

std::vector<Turret *>TurretManager::getPlacedTurrets()
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

void TurretManager::hideAllTurretRanges()
{
	std::for_each(turrets.begin(), turrets.end(), [](Turret *  t) { t->hideRange(); });
}

void TurretManager::hideAllTurretStats()
{
	std::for_each(turrets.begin(), turrets.end(), [](Turret * t) { t->hideTurretStats(); });
}

void TurretManager::hideAllTurretHelpers()
{
	std::for_each(turrets.begin(), turrets.end(), [](Turret * t) { t->hideHelperButtons(); });
}

void TurretManager::showSelectedTurretHelpers()
{
	turrets[currentSelectedTurret]->showHelperButtons();
}

void TurretManager::hideSelectedTurretHelpers()
{
	turrets[currentSelectedTurret]->hideHelperButtons();
}

bool TurretManager::isEmpty()
{
	return turrets.empty();
}

bool TurretManager::isATurretSelected()
{
	return currentSelectedTurret != -1;
}

int TurretManager::getMouseOverTurret()
{
	return currentMouseOverTurret;
}

void TurretManager::setMouseOver(int index)
{
	currentMouseOverTurret = index;
}

void TurretManager::unsetMouseOverTurret()
{
	currentMouseOverTurret = -1;
}

bool TurretManager::isMouseOverTurret()
{
	return currentMouseOverTurret == -1;
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

void TurretManager::sellSelectedTurret()
{
	turrets[currentSelectedTurret]->lostSightOfEnemy();
	turrets[currentSelectedTurret]->stopAllActions();
	turrets[currentSelectedTurret]->unscheduleAllSelectors();
	//game->removeChildByTag(turrets[currentSelectedTurret]->getTag());
	turrets.erase(turrets.begin() + currentSelectedTurret);
}
