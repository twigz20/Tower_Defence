#include "LevelManager.h"
#include "TowerDefenceScene.h"

using namespace cocos2d;

void LevelManager::initCreepManager()
{
	creepManager = new CreepManager(scene, this);
	creepManager->setWayPoints(start, end);
}

void LevelManager::initWaveManager()
{
	waveManager = new WaveManager();
}

void LevelManager::populateCreepManager()
{
	if (waveManager->hasNextWave()) {
		currentWave.setWave(waveManager->getNextWave());

		creepManager->clearManager();
		while (currentWave.hasNextWaveSection()) {
			creepManager->addCreep(currentWave.getNextWaveSection());
			currentWave.popWave();
		}
		creepAmountForCurrentWave = creepManager->getCreepAmountForWave();
		waveManager->popWave();
	}
	levelFinished = false;
}

void LevelManager::setBackground()
{
	cocos2d:Sprite* bgS = cocos2d::Sprite::create(BACKGROUND_FILE);
	cocos2d::Rect s_visibleRect = Director::getInstance()->getOpenGLView()->getVisibleRect();
	float scaleY = s_visibleRect.size.height / bgS->getContentSize().height;
	float scaleX = s_visibleRect.size.width / bgS->getContentSize().width;
	if (scaleX>scaleY) {
		bgS->setScale(scaleX);
	}
	else {
		bgS->setScale(scaleY);
	}
	bgS->setPosition(Point(s_visibleRect.origin.x + s_visibleRect.size.width / 2, s_visibleRect.origin.y + s_visibleRect.size.height / 2));
	scene->addChild(bgS, -2);
}

void LevelManager::loadMap(std::string fileName)
{
	auto str = cocos2d::String::createWithContentsOfFile(cocos2d::FileUtils::getInstance()->fullPathForFilename(fileName.c_str()).c_str());
	tileMap = cocos2d::TMXTiledMap::createWithXML(str->getCString(), "");
	tileMap->retain();
	_bgLayer = tileMap->layerNamed("Background");
	scene->addChild(tileMap, -1);
}

void LevelManager::loadStartPoint()
{
	cocos2d::TMXObjectGroup *wayPoints = tileMap->getObjectGroup("Way Points");
	auto startPoint = wayPoints->getObject("Start Point");
	start.x = startPoint["x"].asInt() + tileMap->getTileSize().width / 2;
	start.y = startPoint["y"].asInt() + tileMap->getTileSize().height / 2;
}

void LevelManager::loadEndPoint()
{
	cocos2d::TMXObjectGroup *wayPoints = tileMap->getObjectGroup("Way Points");
	auto endPoint = wayPoints->getObject("End Point");
	end.x = endPoint["x"].asInt();
	end.y = endPoint["y"].asInt();
}

bool LevelManager::hasProperty(std::string name, cocos2d::Vec2 tileCoord, cocos2d::TMXLayer * layer)
{
	if (!isValidTileCoord(tileCoord))
		return false;

	int gid = layer->getTileGIDAt(tileCoord);
	Value properties = tileMap->getPropertiesForGID(gid);
	if (properties.getType() != Value::Type::NONE) {
		auto valueMap = properties.asValueMap();
		auto v = valueMap.find(name);
		return v != valueMap.end() ? true : false;
	}
	return false;
}

LevelManager::LevelManager(TowerDefence* _scene) :
	scene(_scene),
	levelStarted(false),
	levelFinished(false),
	creepAmountForCurrentWave(0)
{
	setBackground();
	loadMap(INITIAL_MAP_FILE);
	loadStartPoint();
	loadEndPoint();

	initCreepManager();
	initWaveManager();
	populateCreepManager();
}


LevelManager::~LevelManager()
{
	if (waveManager)
		delete waveManager;
	if (creepManager)
		delete creepManager;
	if (tileMap)
		delete tileMap;
	if (_bgLayer)
		delete _bgLayer;
	if (_objectLayer)
		delete _objectLayer;
}

void LevelManager::update(float deltaTime)
{
	if (levelStarted) {
		if(creepManager->hasNextCreep()) 
		{
			float time = t.GetTicks();
			float startDelay = creepManager->getNextCreep()->getStartDelay()*1000;
			if (time >= startDelay) {
				creepManager->getNextCreep()->moveToward(end);
				creepManager->popCreep();
				t.Reset();
			}
		}
		if(creepAmountForCurrentWave == 0) 
		{
			levelStarted = false;
			levelFinished = true;
			populateCreepManager();
		}
	}
}

void LevelManager::startLevel()
{
	if (!levelStarted) {
		levelStarted = true;
		t.Start();
	}
}

void LevelManager::endLevel()
{
	levelFinished = true;
}

void LevelManager::decreaseCreepAmount()
{
	creepAmountForCurrentWave--;
}

void LevelManager::setWayPoints(cocos2d::Vec2 start, cocos2d::Vec2 end)
{
	this->start = start;
	this->end = end;
}

bool LevelManager::isValidTileCoord(cocos2d::Point tileCoord)
{
	if (tileCoord.x < 0 || tileCoord.y < 0 ||
		tileCoord.x >= tileMap->getMapSize().width ||
		tileCoord.y >= tileMap->getMapSize().height) {
		return false;
	}
	else {
		return true;
	}
}

bool LevelManager::isWallAtTileCoord(cocos2d::Point tileCoord)
{
	return hasProperty("Wall", tileCoord, _bgLayer);
}

bool LevelManager::isExitAtTilecoord(cocos2d::Point tileCoord)
{
	return hasProperty("Exit", tileCoord, _bgLayer);
}

cocos2d::Vec2 LevelManager::tileCoordForPosition(cocos2d::Vec2 position)
{
	int x = position.x / tileMap->getTileSize().width;
	int y = ((tileMap->getMapSize().height * tileMap->getTileSize().height) - position.y) / tileMap->getTileSize().height;
	return Vec2(x, y);
}

cocos2d::Vec2 LevelManager::positionForTileCoord(cocos2d::Vec2 tileCoord)
{
	int x = (tileCoord.x * tileMap->getTileSize().width) + tileMap->getTileSize().width / 2;
	int y = (tileMap->getMapSize().height * tileMap->getTileSize().height) - (tileCoord.y * tileMap->getTileSize().height) - tileMap->getTileSize().height / 2;
	return Vec2(x, y);
}

void LevelManager::removeObjectAtTileCoord(cocos2d::Vec2 tileCoord)
{
	_objectLayer->removeTileAt(tileCoord);
}

void LevelManager::setViewPointCenter(cocos2d::Vec2 position)
{
	auto winSize = Director::getInstance()->getWinSize();

	float x = MAX(position.x, winSize.width / 2);
	float y = MAX(position.y, winSize.height / 2);
	x = MIN(x, (tileMap->getMapSize().width * tileMap->getTileSize().width) - winSize.width / 2);
	y = MIN(y, (tileMap->getMapSize().height * tileMap->getTileSize().height) - winSize.height / 2);

	auto actualPosition = cocos2d::Vec2{ x, y };
	auto centerOfView = cocos2d::Vec2(winSize.width / 2, winSize.height / 2);
	cocos2d::Vec2 viewPoint = centerOfView - actualPosition;
	tileMap->setPosition(viewPoint);
}

cocos2d::PointArray * LevelManager::walkableAdjacentTilesCoordForTileCoord(cocos2d::Point & tileCoord)
{
	PointArray *tmp = PointArray::create(4);

	// On the
	Point p(tileCoord.x, tileCoord.y - 1);
	if (isValidTileCoord(p) && !isWallAtTileCoord(p))
	{
		tmp->addControlPoint(p);
	}

	// The left
	p.setPoint(tileCoord.x - 1, tileCoord.y);
	if (this->isValidTileCoord(p) && !this->isWallAtTileCoord(p))
	{
		tmp->addControlPoint(p);
	}

	// Under the
	p.setPoint(tileCoord.x, tileCoord.y + 1);
	if (this->isValidTileCoord(p) && !this->isWallAtTileCoord(p))
	{
		tmp->addControlPoint(p);
	}

	// Right
	p.setPoint(tileCoord.x + 1, tileCoord.y);
	if (this->isValidTileCoord(p) && !this->isWallAtTileCoord(p))
	{
		tmp->addControlPoint(p);
	}

	return tmp;
}

cocos2d::TMXTiledMap *LevelManager::getMap()
{
	return tileMap;
}

cocos2d::Vec2 LevelManager::getStartPoint()
{
	return start;
}

cocos2d::Vec2 LevelManager::getEndPoint()
{
	return end;
}

cocos2d::TMXLayer * LevelManager::getBackgroundLayer()
{
	return _bgLayer;
}
