#include "TowerDefenceScene.h"
#include "SimpleAudioEngine.h"
#include "proj.win32\LevelManager.h"
#include "proj.win32\TurretManager.h"
#include <sstream>
#include "proj.win32\Utils.h"
#include <vld.h> 
#define HELP_LABEL 500

USING_NS_CC;

using namespace CocosDenshion;
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

TowerDefence::~TowerDefence()
{
	if (levelManager)
		delete levelManager;
	if (turretManager)
		delete turretManager;
	if(selectedTurret)
		delete selectedTurret;
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
                                           "Graphics/UI/CloseNormal.png",
                                           "Graphics/UI/CloseSelected.png",
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
	
	auto eventListener = EventListenerTouchOneByOne::create();
	eventListener->onTouchBegan = CC_CALLBACK_2(TowerDefence::onTouchBegan, this);
	eventListener->onTouchMoved = CC_CALLBACK_2(TowerDefence::onTouchMoved, this);
	eventListener->onTouchEnded = CC_CALLBACK_2(TowerDefence::onTouchEnded, this);
	eventListener->onTouchCancelled = CC_CALLBACK_2(TowerDefence::onTouchCancelled, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventListener, this);


	auto _mouseListener = EventListenerMouse::create();
	_mouseListener->onMouseMove = CC_CALLBACK_1(TowerDefence::onMouseMove, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_mouseListener, this);

	auto button = Button::create(START_FILE);
	button->setTitleText("Start Wave");
	button->setTitleFontName("fonts/Marker Felt.ttf");
	button->setTitleFontSize(12.0f);
	button->setPosition(cocos2d::Vec2(400 + levelManager->getMap()->getTileSize().width / 2, 400 + levelManager->getMap()->getTileSize().width / 2));
	button->addTouchEventListener(CC_CALLBACK_2(TowerDefence::touchEvent, this));
	addChild(button);

	sell = Button::create(SELL_FILE);
	sell->setVisible(false);
	sell->setPosition(Vec2::ZERO);
	sell->addTouchEventListener(CC_CALLBACK_2(TowerDefence::sellCallback, this));
	sell->setScale(0.325);
	addChild(sell);

	upgrade = Button::create(UPGRADE_FILE);
	upgrade->setVisible(false);
	upgrade->setPosition(Vec2::ZERO);
	upgrade->addTouchEventListener(CC_CALLBACK_2(TowerDefence::upgradeCallback, this));
	upgrade->setScale(0.325);
	addChild(upgrade);

	help = Button::create(HELP_FILE);
	help->setVisible(false);
	help->setPosition(Vec2::ZERO);
	help->addTouchEventListener(CC_CALLBACK_2(TowerDefence::helpCallback, this));
	help->setScale(0.325);
	addChild(help);

	// add a "close" icon to exit the progress. it's an autorelease object
	auto repeatItem = Button::create("Graphics/UI/Repeat.png");
	repeatItem->setPosition(Vec2(origin.x + visibleSize.width - (repeatItem->getContentSize().width*0.325) / 2,
		origin.y + (repeatItem->getContentSize().height*0.325) / 2 + closeItem->getContentSize().height));
	repeatItem->addTouchEventListener(CC_CALLBACK_2(TowerDefence::repeatCallback, this));
	repeatItem->setScale(0.325);
	addChild(repeatItem);

	this->scheduleUpdate();

    return true;
}

void TowerDefence::onMouseMove(cocos2d::Event *event)
{
	EventMouse* e = (EventMouse*)event;
	cocos2d::Point touchLoc(cocos2d::Vec2(e->getCursorX(), e->getCursorY()));

	for (int i = 0; i < turretManager->getPlacedTurrets().size(); i++) {
		if (turretManager->getPlacedTurrets().at(i)->getBoundingBox().containsPoint(touchLoc))
		{
			turretManager->getPlacedTurrets().at(i)->showRange();
			cocos2d::Vec2 turretStatsPosition = cocos2d::Vec2(
				turretManager->getPlacedTurrets().at(i)->getPosition().x + levelManager->getMap()->getTileSize().width / 2,
				turretManager->getPlacedTurrets().at(i)->getPosition().y - levelManager->getMap()->getTileSize().height / 2
			);
			turretManager->getPlacedTurrets().at(i)->showTurretStats(turretStatsPosition);
		}
		else {
			turretManager->getPlacedTurrets().at(i)->hideRange();
			turretManager->getPlacedTurrets().at(i)->hideTurretStats();
		}
	}
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

void TowerDefence::sellCallback(cocos2d::Ref * pSender, cocos2d::ui::Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
	{
		levelManager->increaseGold(
			turretManager->getSelectedTurret()->getTurretInfo().cost
			- (turretManager->getSelectedTurret()->getTurretInfo().cost * 0.33)
		);
		turretManager->sellSelectedTurret();
		turretManager->unselectTurret();
	}
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}

	sell->setVisible(false);
	upgrade->setVisible(false);
	help->setVisible(false);
}

void TowerDefence::upgradeCallback(cocos2d::Ref * pSender, cocos2d::ui::Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
	{
		if (levelManager->getGold() >=
			(turretManager->getSelectedTurret()->getTurretInfo().cost - (turretManager->getSelectedTurret()->getTurretInfo().cost * 0.33)) 
			&& turretManager->getSelectedTurret()->getTurretInfo().hasLevelUp())
		{
			levelManager->decreaseGold(
				turretManager->getSelectedTurret()->getTurretInfo().cost
				- (turretManager->getSelectedTurret()->getTurretInfo().cost * 0.33)
			);

			turretManager->getSelectedTurret()->upgrade();
			turretManager->unselectTurret();
		}
	}
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}

	sell->setVisible(false);
	upgrade->setVisible(false);
	help->setVisible(false);
}

void TowerDefence::helpCallback(cocos2d::Ref * pSender, cocos2d::ui::Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
	{
		auto helpLabel = Label::createWithSystemFont("No Information at this Moment.", "Arial", 30);
		helpLabel->setAnchorPoint(cocos2d::Vec2(0, 0));
		helpLabel->setPosition(
			Director::getInstance()->getVisibleSize().width * 0.15,
			Director::getInstance()->getVisibleSize().height * 0.55
		);
		helpLabel->setColor(cocos2d::Color3B::GREEN);
		helpLabel->enableBold();
		helpLabel->setTag(HELP_LABEL);
		addChild(helpLabel);
	}
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}

	sell->setVisible(false);
	upgrade->setVisible(false);
	help->setVisible(false);
}

void TowerDefence::repeatCallback(cocos2d::Ref * pSender, cocos2d::ui::Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
	{
		turretManager->reset();
		levelManager->reset();
	}
	break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

bool TowerDefence::onTouchBegan(cocos2d::Touch * touch, cocos2d::Event * unused_event)
{
	removeChildByTag(HELP_LABEL);
	sell->setVisible(false);
	upgrade->setVisible(false);
	help->setVisible(false);

	cocos2d::Point touchLoc = touch->getLocation();
	std::vector<std::shared_ptr<Turret>> starterTurrets = turretManager->getStarterTurrets();
	for (int i = 0; i < starterTurrets.size(); i++) {
		if (starterTurrets[i]->getBoundingBox().containsPoint(touchLoc))
		{
			prevPos = starterTurrets[i]->getPosition();
			selectedTurret = new Turret(*starterTurrets[i]);
			selectedTurret->setAsNormalTurret();
			selectedTurret->showRange();

			cocos2d::Vec2 turretStatsPosition = cocos2d::Vec2(
				Director::getInstance()->getVisibleSize().width * 0.77,
				Director::getInstance()->getVisibleSize().height * 0.10);
			selectedTurret->showTurretStats(turretStatsPosition);
			selectedTurret->setTag(SELECTED_TURRET);
			addChild(selectedTurret, 1);
			return true;
		}
	}

	for (int i = 0; i < turretManager->getPlacedTurrets().size(); i++) {
		if (turretManager->getPlacedTurrets().at(i)->getBoundingBox().containsPoint(touchLoc))
		{
			turretManager->selectTurret(i);
			sell->setPosition(cocos2d::Vec2(
				turretManager->getSelectedTurret()->getPosition().x - 40,
				turretManager->getSelectedTurret()->getPosition().y - 40)
			);
			sell->setVisible(true);
			upgrade->setPosition(cocos2d::Vec2(
				turretManager->getSelectedTurret()->getPosition().x + 40,
				turretManager->getSelectedTurret()->getPosition().y - 40)
			);
			upgrade->setVisible(true);
			help->setPosition(cocos2d::Vec2(
				turretManager->getSelectedTurret()->getPosition().x,
				turretManager->getSelectedTurret()->getPosition().y + 40)
			);
			help->setVisible(true);
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
		if (levelManager->getGold() >= selectedTurret->getTurretInfo().cost) 
		{
			selectedTurret->hideRange();
			cocos2d::Vec2 tileCoordForTowerPosition = levelManager->tileCoordForPosition(selectedTurret->getPosition());

			if (levelManager->isWallAtTileCoord(tileCoordForTowerPosition))
			{
				cocos2d::Vec2 positionOfTileCoord = levelManager->positionForTileCoord(tileCoordForTowerPosition);
				if (!turretManager->hasTurretAtCoord(positionOfTileCoord))
				{
					levelManager->decreaseGold(selectedTurret->getTurretInfo().cost);
					selectedTurret->setPosition(positionOfTileCoord);
					selectedTurret->hideTurretStats();
					turretManager->addTurret(selectedTurret);
					removeChildByTag(SELECTED_TURRET);
				}
			}
		}

		selectedTurret->hideTurretStats();
		selectedTurret->removeFromParentAndCleanup(true);
		delete selectedTurret;
		selectedTurret = nullptr;
	}
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

void TowerDefence::update(float delta)
{
	levelManager->update(delta);
	turretManager->update(delta);
}

LevelManager * TowerDefence::getLevelManager()
{
	return levelManager;
}

bool TowerDefence::checkCollision(CGCircle *rangeIndicator, cocos2d::Rect rect)
{
	return rangeIndicator->isContainRect(rect);
}
