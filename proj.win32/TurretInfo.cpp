#include "TurretInfo.h"



TurretInfo::TurretInfo(std::string & turretName)
{
	rapidjson::Document turretInfoDoc;
	turretInfoDoc.Parse(getFileContent(TURRET_FILE).c_str());
	const rapidjson::Value& turretInfo = turretInfoDoc[turretName.c_str()];

	name = turretInfo["name"].GetString();
	bullet = turretInfo["bullet_image"].GetString();
	image = turretInfo["image"].GetString();
	damage = turretInfo["damage"].GetInt();
	cooldown = turretInfo["cooldown"].GetFloat();
	range = turretInfo["range"].GetInt();
	cost = turretInfo["cost"].GetInt();
	level = 1;
}

