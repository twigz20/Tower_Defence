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

	bool levelStarted;
	bool levelFinished;
	Wave currentWave;
	int creepAmountForCurrentWave;

	cocos2d::Vec2 start, end;
	clock_t t2;

	cocos2d::TMXTiledMap *tileMap;
	cocos2d::TMXLayer *_bgLayer, *_objectLayer;

	void initCreepManager();
	void initWaveManager();
	void populateCreepManager();

	void setBackground();
	void loadMap(std::string fileName);
	void loadStartPoint();
	void loadEndPoint();

	bool hasProperty(std::string name, cocos2d::Vec2 tileCoord, cocos2d::TMXLayer *layer);

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

	cocos2d::Vec2 tileCoordForPosition(cocos2d::Vec2 position);
	cocos2d::Vec2 positionForTileCoord(cocos2d::Vec2 tileCoord);

	void removeObjectAtTileCoord(cocos2d::Vec2 tileCoord);

	void setViewPointCenter(cocos2d::Vec2 position);
	cocos2d::PointArray *walkableAdjacentTilesCoordForTileCoord(cocos2d::Point &tileCoord);

	cocos2d::TMXTiledMap *getMap();
	cocos2d::Vec2 getStartPoint();
	cocos2d::Vec2 getEndPoint();
	cocos2d::TMXLayer *getBackgroundLayer();

	CreepManager *getCreepManager();
};

