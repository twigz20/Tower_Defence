#pragma once
#define WINDOW_HEIGHT 10 * 48
#define WINDOW_WIDTH 15 * 48
#define BACKGROUND_FILE "Graphics/tower-defense/tower-defense-background-stars.png"
#define INITIAL_MAP_FILE "Map1.tmx"
#define WAVES_FILE "Files/waves.json"
#define CREEP_FILE "Files/creep.json"
#define TURRET_FILE "Files/turrets.json"
#define TILE_TAKEN 200
#define TURRET_STATS_TAG 201
#define TURRET_STATS_BACKGROUND "Graphics/tower-defense/stats-background.png"
#define TURRET_STAT_CURRENT "Graphics/tower-defense/stat-current.png"
#define TURRET_STAT_UPGRADE "Graphics/tower-defense/stat-upgrade.png"


#include <string>
#include <fstream>
#include <sstream>

enum States { NONE, CHOOSECREEP, BUYSTATE, TURRETCLICK, TURRETTRACKSTATE, SPLASHSTATE, MAINMENU, PLAY, MULTIPLAYER, SETTINGS, HIGHSCORELIST };

static std::string getFileContent(const std::string& path)
{
	std::ifstream file(path);
	std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

	return content;
}