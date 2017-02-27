#pragma once
#include <string>
#include "rapidjson/document.h"
#include "Utils.h"

struct CreepInfo
{
	CreepInfo(std::string & creepName);

	std::string name;
	std::string image;

	int health;
	float speed;
	int gold;
};

