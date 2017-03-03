#pragma once
#include <string>
#include "rapidjson/document.h"
#include "Utils.h"

enum TurretType
{
	T1 = 1,
	T2 = 2,
	T3 = 3,
	T4 = 4,
	T5 = 5,
	T6 = 6,
	T7 = 7
};

enum TurretLevel
{
	L1 = 1,
	L2 = 2,
	L3 = 3
};

struct BulletInfo
{
	BulletInfo() {}

	TurretType type;
	std::string image;
	float damageFrom, damageTo, speed;

	bool hasSlow;
	float slowDuration, slowPercentage;

	bool hasSplashDamage;
	float splashRange;

	bool hasBleed;
	float bleedDuration, bleedDps;

	bool hasStun;
	float stunDuration, stunChance;
};

struct TurretInfo
{
	TurretInfo(std::string & turretName);
	void levelUp();

	TurretType type;
	TurretLevel level_;
	BulletInfo bulletInfo;

	std::string name;
	std::string image;
	int level;

	int damage;
	double cooldown;
	int range;
	int cost;
};

