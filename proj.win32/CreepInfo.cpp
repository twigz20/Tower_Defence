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
}
