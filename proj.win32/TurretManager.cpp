#include "TurretManager.h"
#include "TowerDefenceScene.h"
#include "LevelManager.h"
#include "TurretInfo.h"

#include <sstream>
#include <algorithm>

using namespace rapidjson;

TurretManager::TurretManager(TowerDefence * game_) :
	game(game_),
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
	std::for_each(turrets.begin(), turrets.end(), [](std::shared_ptr<Turret> turret) {
		turret->setActive(false);
		turret->lostSightOfEnemy();
		turret->stopAllActions();
		turret->unscheduleAllSelectors();
		turret->unscheduleUpdate();
		turret->unscheduleAllCallbacks();
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
		/*if ((*turret)->isDead() || (*turret)->isMissionCompleted())
		{
			(*turret)->removeAllChildrenWithCleanup(true);
			(*turret)->removeFromParentAndCleanup(true);
			game->getLevelManager()->decreaseCreepAmount();
			turret = turrets.erase(creep);

			if (turret == turrets.end())
			{
				break;
			}
		}*/
	}
}

void TurretManager::addTurret(Turret *turret_)
{
	turrets.push_back(std::make_shared<Turret>(std::move(*turret_)));
	turrets.back()->setTag(turretTag++);
	turrets.back()->setActive(true);
	game->addChild(turrets.back().get(), 0);
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

std::shared_ptr<Turret> TurretFactory::getTurret(std::string & turretName, bool isStarterTurret)
{
	return std::make_shared<Turret>(Turret(game, TurretInfo(turretName), isStarterTurret));
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

void TurretManager::showStarterTurrets()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();

	int xOffset = game->getLevelManager()->getMap()->getTileSize().width / 2;
	int yOffset = game->getLevelManager()->getMap()->getTileSize().height / 2;
	int originalTurretX = visibleSize.width * 0.77;
	int originalTurretY = visibleSize.height * 0.83;
	int turretX = originalTurretX;
	int turretY = originalTurretY;

	int counter = 1;
	std::for_each(starterTurrets.begin(), starterTurrets.end(), 
		[&](std::shared_ptr<Turret> turret) {
			turret->setPosition(cocos2d::Vec2(turretX + xOffset, turretY + yOffset));
			game->addChild(turret.get(), 0);

			if (counter == 1) {
				turretX += visibleSize.width * 0.10;
				counter++;
			}
			else {
				turretX = originalTurretX;
				turretY -= visibleSize.height * 0.115;
				counter = 1;
			}
		}
	);
}

std::vector<std::shared_ptr<Turret>> TurretManager::getStarterTurrets()
{
	return starterTurrets;
}

std::vector<std::shared_ptr<Turret>>TurretManager::getPlacedTurrets()
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
	std::for_each(turrets.begin(), turrets.end(), [](std::shared_ptr<Turret>  t) { t->hideRange(); });
}

void TurretManager::hideAllTurretStats()
{
	std::for_each(turrets.begin(), turrets.end(), [](std::shared_ptr<Turret> t) { t->hideTurretStats(); });
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

std::shared_ptr<Turret> TurretManager::getSelectedTurret()
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
	game->removeChildByTag(turrets[currentSelectedTurret]->getTag());
	turrets.erase(turrets.begin() + currentSelectedTurret);
}
