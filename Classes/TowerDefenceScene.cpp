#include "TowerDefenceScene.h"
#include "SimpleAudioEngine.h"
#include "proj.win32\LevelManager.h"
#include "proj.win32\TurretManager.h"
#include "proj.win32\TurretStatsDisplay.h"
#include <sstream>

USING_NS_CC;

using namespace cocos2d;
using namespace ui;

Scene* TowerDefence::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = TowerDefence::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool TowerDefence::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(TowerDefence::menuCloseCallback, this));
    
    closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

	levelManager = new LevelManager(this);
	turretManager = new TurretManager(this);

	setStartedTurrets();
	
	auto eventListener = EventListenerTouchOneByOne::create();
	eventListener->onTouchBegan = CC_CALLBACK_2(TowerDefence::onTouchBegan, this);
	eventListener->onTouchMoved = CC_CALLBACK_2(TowerDefence::onTouchMoved, this);
	eventListener->onTouchEnded = CC_CALLBACK_2(TowerDefence::onTouchEnded, this);
	eventListener->onTouchCancelled = CC_CALLBACK_2(TowerDefence::onTouchCancelled, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventListener, this);

	auto button = Button::create("Graphics/Button.png");
	button->setTitleText("Start Wave");
	button->setTitleFontName("fonts/Marker Felt.ttf");
	button->setTitleFontSize(12.0f);
	button->setPosition(cocos2d::Vec2(400 + levelManager->getMap()->getTileSize().width / 2, 400 + levelManager->getMap()->getTileSize().width / 2));
	button->addTouchEventListener(CC_CALLBACK_2(TowerDefence::touchEvent, this));

	addChild(button);

	this->scheduleUpdate();

    return true;
}


void TowerDefence::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
    
    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/
    
    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);
       
}

bool TowerDefence::onTouchBegan(cocos2d::Touch * touch, cocos2d::Event * unused_event)
{
	cocos2d::Point touchLoc = touch->getLocation();

	for (int i = 0; i < starterTurrets.size(); i++) {
		if (starterTurrets[i]->getBoundingBox().containsPoint(touchLoc))
		{
			prevPos = starterTurrets[i]->getPosition();
			selectedTurret = new Turret(*starterTurrets[i]);
			selectedTurret->showRange();
			selectedTurret->setTag(SELECTED_TURRET);
			addChild(selectedTurret, 1);

			TurretStatsDisplay *tsd = new TurretStatsDisplay();
			tsd->nodeWithTheGame(
				this,
				starterTurrets[i]->getTurretInfo(),
				cocos2d::Vec2(
					Director::getInstance()->getVisibleSize().width * 0.77,
					Director::getInstance()->getVisibleSize().height * 0.10)
			);
			tsd->setTag(TURRET_STATS_TAG);
			addChild(tsd);

			return true;
		}
	}

	for (int i = 0; i < turretManager->getPlacedTurrets().size(); i++) {
		if (turretManager->getPlacedTurrets().at(i)->getBoundingBox().containsPoint(touchLoc))
		{
			turretManager->selectTurret(i);
			turretManager->showSelectedTurretRange();
			prevPos = turretManager->getSelectedTurret()->getPosition();

			TurretStatsDisplay *tsd = new TurretStatsDisplay();
			tsd->nodeWithTheGame(
				this,
				turretManager->getSelectedTurret()->getTurretInfo(),
				cocos2d::Vec2(
					turretManager->getSelectedTurret()->getPosition().x + levelManager->getMap()->getTileSize().width / 2,
					turretManager->getSelectedTurret()->getPosition().y - levelManager->getMap()->getTileSize().height / 2
				)
			);
			tsd->setTag(TURRET_STATS_TAG);
			turretManager->getSelectedTurret()->addChild(tsd,1);

			return true;
		}
	}

	return false;
}

void TowerDefence::onTouchMoved(cocos2d::Touch * touch, cocos2d::Event * unused_event)
{
	if (selectedTurret)
		selectedTurret->setPosition(selectedTurret->getPosition() + touch->getDelta());
}

void TowerDefence::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	if (selectedTurret) 
	{
		selectedTurret->hideRange();
		cocos2d::Vec2 tileCoordForTowerPosition = levelManager->tileCoordForPosition(selectedTurret->getPosition());

		if (levelManager->isWallAtTileCoord(tileCoordForTowerPosition)) 
		{
			cocos2d::Vec2 positionOfTileCoord = levelManager->positionForTileCoord(tileCoordForTowerPosition);
			if (!turretManager->hasTurretAtCoord(positionOfTileCoord)) 
			{
				selectedTurret->setPosition(positionOfTileCoord);
				turretManager->addTurret(selectedTurret);
				removeChildByTag(SELECTED_TURRET);
			}
		}

		selectedTurret->removeFromParentAndCleanup(true);
	}

	if (!turretManager->isATurretSelected()) {
		turretManager->hideSelectedTurretRange();
		turretManager->getSelectedTurret()->removeChildByTag(TURRET_STATS_TAG);
	}

	removeChildByTag(TURRET_STATS_TAG);
}

void TowerDefence::onTouchCancelled(cocos2d::Touch * touch, cocos2d::Event * unused_event)
{
}

void TowerDefence::playGame()
{
	levelManager->startLevel();
}

void TowerDefence::touchEvent(cocos2d::Ref * sender, cocos2d::ui::Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		playGame();
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

void TowerDefence::setStartedTurrets()
{
	starterTurrets = turretManager->getStarterTurrets();

	int xOffset = levelManager->getMap()->getTileSize().width / 2;
	int yOffset = levelManager->getMap()->getTileSize().height / 2;
	int originalTurretX = 550;
	int originalTurretY = 400;
	int turretX = originalTurretX;
	int turretY = originalTurretY;

	int counter = 1;
	for (int i = 0; i < starterTurrets.size(); i++) {
		starterTurrets[i]->setPosition(cocos2d::Vec2(turretX + xOffset, turretY + yOffset));
		addChild(starterTurrets[i],0);

		if (counter == 1) {
			turretX += 75;
			counter++;
		}
		else {
			turretX = originalTurretX;
			turretY -= 75;
			counter = 1;
		}
	}
}

void TowerDefence::update(float delta)
{
	levelManager->update(delta);
}

bool TowerDefence::circleCollision(cocos2d::Vec2 circlePoint, float radius, cocos2d::Vec2 circlePointTwo, float radiusTwo)
{
	float xdif = circlePoint.x - circlePointTwo.x;
	float ydif = circlePoint.y - circlePointTwo.y;

	float distance = sqrt(xdif*xdif + ydif*ydif);

	if (distance <= radius + radiusTwo)
		return true;

	return false;
}

bool TowerDefence::intersects(cocos2d::Vec2 c, float radius, cocos2d::Rect r)
{
	/*cocos2d::Vec2 circleDistance;
	circleDistance.x = abs(circle.x - rect.getMinX());
	circleDistance.y = abs(circle.y - rect.getMinY());

	if (circleDistance.x > (rect.getMaxX() / 2 + radius)) { return false; }
	if (circleDistance.y > (rect.getMaxY() / 2 + radius)) { return false; }

	if (circleDistance.x <= (rect.getMaxX() / 2)) { return true; }
	if (circleDistance.y <= (rect.getMaxY() / 2)) { return true; }

	float cornerDistance_sq = ((circleDistance.x - rect.getMaxX() / 2) * (circleDistance.x - rect.getMaxX() / 2)) +
		((circleDistance.y - rect.getMaxY() / 2) *  (circleDistance.y - rect.getMaxY() / 2));

	return (cornerDistance_sq <= (radius * radius));*/

	float cx = abs(c.x - r.getMinX() - r.getMidX());
	float xDist = r.getMidX() + radius;
	if (cx > xDist)
		return false;
	float cy = abs(c.y - r.getMinY() - r.getMidY());
	float yDist = r.getMidY() + radius;
	if (cy > yDist)
		return false;
	if (cx <= r.getMidX() || cy <= r.getMidY())
		return true;
	float xCornerDist = cx - r.getMidX();
	float yCornerDist = cy - r.getMidY();
	float xCornerDistSq = xCornerDist * xCornerDist;
	float yCornerDistSq = yCornerDist * yCornerDist;
	float maxCornerDistSq = radius * radius;
	return xCornerDistSq + yCornerDistSq <= maxCornerDistSq;
}

void TowerDefence::ccFillPoly(cocos2d::Vec2 * poli, int points, bool closePolygon)
{
}

void TowerDefence::enemyGotKilled()
{
}

void TowerDefence::getHpDamage()
{
}

LevelManager * TowerDefence::getLevelManager()
{
	return levelManager;
}
