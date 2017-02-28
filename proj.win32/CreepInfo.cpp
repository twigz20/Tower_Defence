#include "CreepInfo.h"

CreepInfo::CreepInfo(std::string & creepName)
{
	rapidjson::Document creepInfoDoc;
	creepInfoDoc.Parse(getFileContent(CREEP_FILE).c_str());
	const rapidjson::Value& creepInfo = creepInfoDoc[creepName.c_str()];

	image = creepInfo["source"].GetString();
	health = creepInfo["health"].GetInt();
	speed = creepInfo["speed"].GetFloat();
	gold = creepInfo["gold"].GetInt();
	name = creepInfo["name"].GetString();

	if (creepInfo.HasMember("heal_aura"))
	{
		healAura = HealAura{ creepInfo["heal_aura"]["area"].GetInt(), creepInfo["heal_aura"]["hps"].GetInt() };
	}
	else
		healAura = HealAura{ 0, 0 };

	if (creepInfo.HasMember("speed_aura"))
	{
		speedAura = SpeedAura{ creepInfo["speed_aura"]["area"].GetInt(), creepInfo["speed_aura"]["percentage"].GetDouble() };
	}
	else
		speedAura = SpeedAura{ 0, 0 };

	if (creepInfo.HasMember("stunResistance"))
		stunResistance = creepInfo["stunResistance"].GetDouble();
	else
		stunResistance = 0;

	if (creepInfo.HasMember("bleedResistance"))
		bleedResistance = creepInfo["bleedResistance"].GetDouble();
	else
		bleedResistance = 0;

	if (creepInfo.HasMember("slowResistance"))
		slowResistance = creepInfo["slowResistance"].GetDouble();
	else
		slowResistance = 0;
}

bool CreepInfo::hasSpeedAura() const
{
	return speedAura.area_ > 0;
}

bool CreepInfo::hasHealAura() const
{
	return healAura.area_ > 0;
}

bool CreepInfo::hasStunResistance() const
{
	return stunResistance != 0;
}

bool CreepInfo::hasSlowResistance() const
{
	return slowResistance != 0;
}

bool CreepInfo::hasBleedResistance() const
{
	return bleedResistance != 0;
}
