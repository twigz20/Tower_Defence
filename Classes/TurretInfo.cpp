#include "TurretInfo.h"



TurretInfo::TurretInfo(std::string turretName)
{
	auto fileData = cocos2d::FileUtils::getInstance()->getDataFromFile(TURRET_FILE);
	std::string fileStr((const char*)fileData.getBytes(), fileData.getSize());

	rapidjson::Document turretInfoDoc;
	turretInfoDoc.Parse(fileStr.c_str());
	const rapidjson::Value& turretInfo = turretInfoDoc[turretName.c_str()];

	level_ = TurretLevel::L1;

	imageHeader = turretName.c_str();
	name = turretInfo["name"].GetString();
	id = turretInfo["id"].GetString();
	image = turretInfo["image"].GetString();
	cooldown = turretInfo["cooldown"].GetFloat();
	range = turretInfo["range"].GetInt();
	cost = turretInfo["cost"].GetInt();

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
	if (level_ < L3)
	{
		int nextLevel = (int)level_ + 1;
		level_ = (TurretLevel)nextLevel;

		bulletInfo.damageFrom *= 1.65;
		bulletInfo.damageTo *= 1.65;

		if (bulletInfo.hasSlow) {
			bulletInfo.slowDuration *= 1.25;
			bulletInfo.slowPercentage *= 1.25;
		}
		if (bulletInfo.hasStun) {
			bulletInfo.stunDuration *= 1.25;
			bulletInfo.stunChance += 0.05;
		}
		if (bulletInfo.hasBleed) {
			bulletInfo.bleedDps += 1.5;
		}
		if (bulletInfo.hasSplashDamage) {
			bulletInfo.hasSplashDamage = bulletInfo.damageFrom / 2;
			bulletInfo.splashRange += 3;
		}

		cooldown = cooldown > 1.0 ? cooldown - 0.15 : cooldown;
		range += 7;

		cost += cost * 0.33;

		std::stringstream ss;
		ss << type << "-" << level_;
		image = "Graphics/tower-defense-turrets/turret-" + ss.str() + ".png";
	}
}

bool TurretInfo::hasLevelUp()
{
	return level_ < L3;
}

