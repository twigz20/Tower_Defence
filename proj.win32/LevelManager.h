#pragma once
#include "Utils.h"
#include "proj.win32\timer.h"
#include "Wave.h"

class TowerDefence;
class WaveManager;
class CreepManager;
class LevelManager : public cocos2d::Object
{
	TowerDefence* game;
	std::unique_ptr<WaveManager> waveManager;
	std::shared_ptr<CreepManager> creepManager;

	cocos2d::TMXTiledMap *tileMap;
	cocos2d::TMXLayer *bgLayer, *objectLayer;
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

	void setupUi();

public:
	explicit LevelManager(TowerDefence* game_);
	~LevelManager();

	void update(float deltaTime);
	void startLevel();
	void endLevel();
	void reset();
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
	std::shared_ptr<CreepManager> getCreepManager();
};

