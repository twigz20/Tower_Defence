#pragma once
#include "cocos2d.h"
#include "Observer.h"
#include "TurretInfo.h"

class TowerDefence;
class Creep;
class TurretStatsDisplay;
class CGCircle;
class Turret : public BaseObject
{
	cocos2d::CustomCommand _customCommand;
public:
	Turret(TowerDefence* game_, TurretInfo turretInfo, bool isStarterTurret = false);
	Turret(const Turret& other);
	Turret(Turret&& other);
	Turret& operator=(const Turret& other);
	Turret& operator=(Turret&& other);
	~Turret();

	void setPosition(const cocos2d::Vec2 &position);

	void update(float deltaTime);
	void upgrade();
	void showTurretStats(cocos2d::Vec2 position);
	void hideTurretStats();
	void setAsNormalTurret();
	void setRangeColor(cocos2d::Color4F color); 
	void showRange();
	void hideRange();
	TurretInfo& getTurretInfo();

	void getNextTarget();
	void targetKilled();
	void attackEnemy();
	void chosenEnemyForAttack(std::shared_ptr<Creep> enemy);
	void shootWeapon(float dt);
	void removeBullet(cocos2d::Sprite *bullet);
	void damageEnemy();
	void lostSightOfEnemy();
	void rotateToTarget();

	bool collidesWithCreep(cocos2d::Rect creepRect);

	bool getStarterTurret() const { return starterTurret; }
	void setStarterTurret(bool val) { starterTurret = val; }
	bool getIsShooting() const { return isShooting; }
	void setIsShooting(bool val) { isShooting = val; }
	bool getActive() const { return active; }
	void setActive(bool val) { active = val; }
private:
	TowerDefence* game;
	TurretInfo info;

	TurretStatsDisplay *tsd;
	bool displayRange;
	cocos2d::DrawNode *range;
	void addRangeIndicator(cocos2d::Color4F color = cocos2d::Color4F(0, 1, 0, 0.25));
	CGCircle *splashDamageRange;
	void checkForSplashDamage();

	std::shared_ptr<Creep> target;

	bool starterTurret;
	bool isShooting;
	bool active;
};

