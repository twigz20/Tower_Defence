#include "TowerDefenceScene.h"
#include "SimpleAudioEngine.h"
#include "proj.win32\ResourceManager.h"
#include "proj.win32\LevelManager.h"
#include "proj.win32\TurretManager.h"

#include <iostream>
#include <sstream>

USING_NS_CC;

using namespace cocos2d;
using namespace ui;

void TowerDefence::playGame()
{
	levelManager->startLevel();
}

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
    if ( !Layer::init() )
    {
        return false;
    }

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
	levelManager = new LevelManager(this);
	turretManager = new TurretManager(this, levelManager);
	levelManager->addTurretManager(turretManager);

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

cocos2d::CCPoint TowerDefence::touchToPoint(cocos2d::CCTouch * touch)
{
	// convert the touch object to a position in our cocos2d space
	return CCDirector::sharedDirector()->convertToGL(touch->getLocationInView());
}

bool TowerDefence::isTouchingSprite(cocos2d::CCTouch * touch)
{
	/*if (tag == 1) {
		float distance = this->tower->getPosition().getDistance(
			this->touchToPoint(touch));
		return (distance < 100.0f);
	}*/
	return false;
}

bool TowerDefence::onTouchBegan(cocos2d::Touch * touch, cocos2d::Event * unused_event)
{
	cocos2d::Point touchLoc = touch->getLocation();

	for (int i = 0; i < starterTurrets.size(); i++) {
		if (starterTurrets[i]->getObject()->getBoundingBox().containsPoint(touchLoc))
		{
			prevPos = starterTurrets[i]->getPosition();
			selectedTurret = new Turret(*starterTurrets[i]);
			selectedTurret->showTurretRange();
			selectedTurret->getObject()->setTag(SELECTED_TURRET);
			addChild(selectedTurret->getObject(), 1);
			return true;
		}
	}

	for (int i = 0; i < turretManager->getPlacedTurrets().size(); i++) {
		if (turretManager->getPlacedTurrets().at(i)->getObject()->getBoundingBox().containsPoint(touchLoc))
		{
			prevPos = turretManager->getPlacedTurrets().at(i)->getPosition();
			turretManager->selectTurret(i);
			turretManager->showSelectedTurretRange();
			return true;
		}
	}
	return false;
}

void TowerDefence::onTouchMoved(cocos2d::Touch * touch, cocos2d::Event * unused_event)
{
	if(selectedTurret)
		selectedTurret->setPosition(selectedTurret->getPosition() + touch->getDelta());
}

void TowerDefence::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	if (selectedTurret) {
		selectedTurret->hideTurretRange();
		cocos2d::Vec2 tileCoordForTowerPosition = levelManager->tileCoordForPosition(selectedTurret->getPosition());

		if (!levelManager->isWallAtTileCoord(tileCoordForTowerPosition)) {
			selectedTurret->getObject()->setPosition(prevPos);
		}
		else {
			cocos2d::Vec2 positionOfTileCoord = levelManager->positionForTileCoord(tileCoordForTowerPosition);
			if (!turretManager->hasTurretAtCoord(positionOfTileCoord)) {
				selectedTurret->setPosition(positionOfTileCoord);
				turretManager->addTurret(selectedTurret);
				removeChildByTag(SELECTED_TURRET);
			}
			else {
				selectedTurret->getObject()->setPosition(prevPos);
			}
		}

		if (selectedTurret) {
			delete selectedTurret;
			selectedTurret = nullptr;
		}	
	}

	if(!turretManager->isATurretSelected())
		turretManager->hideSelectedTurretRange();
}

void TowerDefence::onTouchCancelled(cocos2d::Touch * touch, cocos2d::Event * unused_event)
{
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
		addChild(starterTurrets[i]->getObject());

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

void TowerDefence::touchEvent(Ref *sender, Widget::TouchEventType type) 
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		log("Touch Ended");
		playGame();
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}