#pragma once
#include "BaseObject.h"
#define TOWER_RANGE_INDICATOR 1000

class LevelManager;
class TowerDefence;

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
		TurretRangeIndicator(Turret* turret);
		void resetTurret(Turret* t);
		void show();
		void hide();
	};

	TurretRangeIndicator *rangeIndicator;
	cocos2d::DrawNode *turretRange;

	TowerDefence *_layer;
	LevelManager *levelManager;
public:
	Turret(const std::string& turret);
	Turret(const Turret& other);
	Turret& operator=(Turret&& other);
	~Turret();

	Turret_Stats& getTurretStats();
	Turret_Info& getTurretInfo();
	void update(float deltaTime);
	void rotateToTarget(cocos2d::Point target);

	void showTurretRange();
	void hideTurretRange();
	void setScene(TowerDefence* scene);

	void setPosition(float x, float y);
	void resetTurret();
};

