#pragma once
#include "BaseObject.h"
#include "CGCircle.h"
#include "BulletInfo.h"
#include "Bullet.h"

#define TURRET_RANGE_INDICATOR 1000

class LevelManager;
class TowerDefence;
class Creep;
class Turret;

struct Turret_Info {
	std::string name;
	std::string bullet;
	std::string image;
};

struct Turret_Stats {
	int damage;
	float cooldown;
	int range;
	int cost;
};

class Turret : public BaseObject
{
	Turret_Stats stats;
	Turret_Info info;

	class TurretRangeIndicator {
		cocos2d::DrawNode *turretRange;
		Turret* turret;
	public:
		TurretRangeIndicator(Turret* turret_);
		void show();
		void hide();
		cocos2d::DrawNode *getTurretRange();
	};

	TurretRangeIndicator *rangeIndicator;
	cocos2d::DrawNode *turretRange;

	TowerDefence *_layer;
	LevelManager *levelManager;
	CGCircle *circle;
	Creep* target;
	BulletInfo bulletInfo;
	std::list<Bullet*> m_Bullets;
public:
	Turret(const std::string& turret_);
	Turret(const Turret& other);
	Turret(Turret&& other);
	Turret& operator=(Turret&& other);
	~Turret();

	Turret_Stats& getTurretStats();
	Turret_Info& getTurretInfo();
	void update(float deltaTime);
	void rotateToTarget(Creep* target);
	void shootTarget(Creep* target);

	void showTurretRange();
	void hideTurretRange();
	void setScene(TowerDefence* scene);

	void addRangeIndicator();
	cocos2d::DrawNode *getTurretRange();
	void addCircle();
	bool checkCollision(cocos2d::Rect rect);

	void setTarget(Creep *creep);
	bool hasTarget();
	void removeTarget();
	BulletInfo& getBulletInfo();
};

