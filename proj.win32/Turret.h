#pragma once
#include "cocos2d.h"
#include "TurretInfo.h"

class TowerDefence;
class Creep;
class CGCircle;
class TurretStatsDisplay;
class Turret : public cocos2d::Node
{
	TowerDefence* game;
	TurretInfo info;
	cocos2d::Sprite *sprite;
	bool starterTurret;
	
	bool isShooting;
	std::shared_ptr<Creep> chosenCreep;

	TurretStatsDisplay *tsd;
	bool displayRange;
	cocos2d::DrawNode *range;
	CGCircle *rangeIndicator;
	void addRangeIndicator();
	CGCircle *splashDamageRange;
	void checkForSplashDamage();

public:
	Turret(TowerDefence* game_, TurretInfo turretInfo, bool isStarterTurret = false);
	Turret(const Turret& other);
	Turret(Turret&& other);
	Turret& operator=(const Turret& other);
	Turret& operator=(Turret&& other);
	~Turret();

	void update(float deltaTime);
	void setPosition(const cocos2d::Vec2 &position);
	const cocos2d::Vec2& getPosition() const;
	cocos2d::Rect getBoundingBox() const;
	const cocos2d::Size & getContentSize() const;

	void showTurretStats(cocos2d::Vec2 position);
	void hideTurretStats();
	void setAsNormalTurret();
	TurretInfo& getTurretInfo();
	void showRange();
	void hideRange();

	void getNextTarget();
	void targetKilled();
	void attackEnemy();
	void chosenEnemyForAttack(std::shared_ptr<Creep> enemy);
	void shootWeapon(float dt);
	void removeBullet(cocos2d::Sprite *bullet);
	void damageEnemy();
	void lostSightOfEnemy();
	void rotateToTarget();
};

