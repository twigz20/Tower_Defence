#pragma once
#include <string>
#include "rapidjson/document.h"
#include "Utils.h"

struct TurretInfo
{
	TurretInfo(std::string & turretName);

	std::string name;
	std::string bullet;
	std::string image;

	int damage;
	float cooldown;
	int range;
	int cost;
};

