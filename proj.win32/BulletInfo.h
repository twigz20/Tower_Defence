#pragma once
#include <string>


//enum TurretType;
struct BulletInfo
{
	BulletInfo() {}

	//TurretType type;
	std::string image;
	float damage_from, damage_to, speed;

	bool hasSlow;
	float slowDuration, percentage;

	bool hasSplashDamage;
	float splashRange;

	bool hasBleed;
	float bleedDuration, bleedDps;

	bool hasStun;
	float stunDuration, stunChance;
};
