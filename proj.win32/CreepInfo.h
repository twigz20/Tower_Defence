#pragma once
#include <string>
#include "rapidjson/document.h"
#include "Utils.h"

struct HealAura
{
	int area_;
	int healPerSecond_;
};

struct SpeedAura
{
	int area_;
	double percentage_;
};

struct CreepInfo
{
	CreepInfo(std::string & creepName);
	bool hasSpeedAura() const;
	bool hasHealAura() const;
	bool hasStunResistance() const;
	bool hasSlowResistance() const;
	bool hasBleedResistance() const;

	std::string name;
	std::string image;

	int health;
	float speed;
	int gold;
	double stunResistance, slowResistance, bleedResistance;
	HealAura healAura;
	SpeedAura speedAura;
};

