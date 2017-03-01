#pragma once
#include "WaveManager.h"
#include "CreepManager.h"
#include "TurretManager.h"
#include "proj.win32\timer.h"

class TowerDefence;
class LevelManager : public cocos2d::Object
{
	TowerDefence* game;
	WaveManager *waveManager;
	CreepManager *creepManager;
	TurretManager *turretManager;

	cocos2d::TMXTiledMap *tileMap;
	cocos2d::TMXLayer *_bgLayer, *_objectLayer;
	cocos2d::Vec2 start, end;
	void setBackground();
	void config();
	void loadMap(std::string fileName);
	bool hasProperty(std::string name, cocos2d::Vec2 tileCoord, cocos2d::TMXLayer *layer);
	void loadStartPoint();
	void loadEndPoint();
	bool levelStarted;
	bool levelFinished;
	Wave currentWave;
	int creepAmountForCurrentWave;

	void initWaveManager();
	void initCreepManager();
	void populateCreepManager();

	int gold;
	cocos2d::Label * goldLabel;
	int health;
	cocos2d::Label * healthLabel;
	void setupUi();

public:
	explicit LevelManager(TowerDefence* game_);
	~LevelManager();

	void update(float deltaTime);
	void startLevel();
	void endLevel();
	void decreaseCreepAmount();

	bool isValidTileCoord(cocos2d::Point tileCoord);
	bool isWallAtTileCoord(cocos2d::Point tileCoord);
	bool isExitAtTilecoord(cocos2d::Point tileCoord);
	void removeObjectAtTileCoord(cocos2d::Vec2 tileCoord);
	cocos2d::Vec2 tileCoordForPosition(cocos2d::Vec2 position);
	cocos2d::Vec2 positionForTileCoord(cocos2d::Vec2 tileCoord);
	cocos2d::PointArray *walkableAdjacentTilesCoordForTileCoord(cocos2d::Point &tileCoord);
	void setViewPointCenter(cocos2d::Vec2 position);

	cocos2d::TMXTiledMap *getMap();
	CreepManager *getCreepManager();

	int getGold();
	int getHealth();
	void increaseGold(int gold);
	void decreaseGold(int gold);
	void decreaseHealth();
};

