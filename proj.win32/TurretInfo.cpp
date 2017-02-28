#include "TurretInfo.h"



TurretInfo::TurretInfo(std::string & turretName)
{
	rapidjson::Document turretInfoDoc;
	turretInfoDoc.Parse(getFileContent(TURRET_FILE).c_str());
	const rapidjson::Value& turretInfo = turretInfoDoc[turretName.c_str()];

	level_ = TurretLevel::L1;

	name = turretInfo["name"].GetString();
	image = turretInfo["image"].GetString();
	cooldown = turretInfo["cooldown"].GetFloat();
	range = turretInfo["range"].GetInt();
	cost = turretInfo["cost"].GetInt();
	level = TurretLevel::L1;

	if (turretInfo.HasMember("bullet_properties"))
	{
		bulletInfo.image = turretInfo["bullet_properties"]["bullet_image"].GetString();
		bulletInfo.damageFrom = turretInfo["bullet_properties"]["damage_from"].GetDouble();
		bulletInfo.damageTo = turretInfo["bullet_properties"]["damage_to"].GetDouble();
		bulletInfo.speed = turretInfo["bullet_properties"]["speed"].GetDouble();
	}
	else
	{
		bulletInfo.damageFrom = 0;
		bulletInfo.damageTo = 0;
		bulletInfo.speed = 0;
	}

	// Splash
	if (turretInfo.HasMember("splash_range")) {
		bulletInfo.splashRange = turretInfo["splash_range"].GetDouble();
		bulletInfo.hasSplashDamage = true;
	}
	else {
		bulletInfo.splashRange = 0;
		bulletInfo.hasSplashDamage = false;
	}

	// Stun
	if (turretInfo.HasMember("stun")) {
		bulletInfo.stunDuration = turretInfo["stun"]["duration"].GetDouble();
		bulletInfo.stunChance = turretInfo["stun"]["chance"].GetDouble();
		bulletInfo.hasStun = true;
	}
	else {
		bulletInfo.stunDuration = 0;
		bulletInfo.stunChance = 0;
		bulletInfo.hasStun = false;
	}

	// Bleed
	if (turretInfo.HasMember("bleed")) {
		bulletInfo.bleedDuration = turretInfo["bleed"]["duration"].GetDouble();
		bulletInfo.bleedDps = turretInfo["bleed"]["dps"].GetDouble();
		bulletInfo.hasBleed = true;
	}
	else {
		bulletInfo.bleedDuration = 0;
		bulletInfo.bleedDps = 0;
		bulletInfo.hasBleed = false;
	}

	// Slow
	if (turretInfo.HasMember("slow")) {
		bulletInfo.slowDuration = turretInfo["slow"]["duration"].GetDouble();
		bulletInfo.slowPercentage = turretInfo["slow"]["percentage"].GetDouble();
		bulletInfo.hasSlow = true;
	}
	else {
		bulletInfo.slowDuration = 0;
		bulletInfo.slowPercentage = 0;
		bulletInfo.hasSlow = false;
	}
}

void TurretInfo::levelUp()
{
	if (level < L3)
	{
		int nextLevel = (int)level + 1;
		level_ = (TurretLevel)nextLevel;

		bulletInfo.damageFrom *= 1.2;
		bulletInfo.damageTo *= 1.2;

		cooldown = cooldown > 1.0 ? cooldown - 0.5 : cooldown;
		range += 5;

		std::stringstream ss;
		ss << type << "-" << level;
		image = "resource/tower-defense-turrets/turret-" + ss.str() + ".png";
	}
}

