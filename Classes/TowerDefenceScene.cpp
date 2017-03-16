#include "TowerDefenceScene.h"
#include "SimpleAudioEngine.h"
#include "LevelManager.h"
#include "Turret.h"
#include "TurretManager.h"
#include <sstream>
#include "Utils.h"
#include "GuiElement.h"
//#include <vld.h> 
#define HELP_LABEL 500
#define MOUSE_NOT_OVER 501
#define MOUSE_OVER 502
#define GOLD_LABEL 20
#define HEALTH_LABEL 21
#define WAVE_LABEL 22

USING_NS_CC;

using namespace CocosDenshion;
using namespace cocos2d;
using namespace ui;

void TowerDefence::setUpUi()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	/////////////////////////////
	// 2. add a menu items

	resetItem = MenuItemImage::create(
		"Graphics/UI/Buttons/Button_90.png",
		"Graphics/UI/Buttons/Button_92.png",
		"Graphics/UI/Buttons/Button_93.png",
		CC_CALLBACK_1(TowerDefence::menuResetCallback, this));
	resetItem->setScale(0.40);
	resetItem->setPosition(Vec2(origin.x + visibleSize.width - (resetItem->getContentSize().width * 0.40) / 2,
		origin.y + (resetItem->getContentSize().height * 0.40) / 2));

	playItem = MenuItemImage::create(
		"Graphics/UI/Buttons/Button_14.png",
		"Graphics/UI/Buttons/Button_16.png",
		"Graphics/UI/Buttons/Button_17.png",
		CC_CALLBACK_1(TowerDefence::menuPlayCallback, this));
	playItem->setScale(0.40);
	playItem->setPosition(Vec2(origin.x + visibleSize.width - (playItem->getContentSize().width * 0.40) / 2,
		resetItem->getPosition().y + (playItem->getContentSize().height * 0.40) + 5));

	// create menu, it's an autorelease object
	auto menu = Menu::create(resetItem, playItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	/////////////////////////////
	// 3. add your codes below...

	GuiElement *healthContainer = new GuiElement("Graphics/UI/Elements 1/Elements_58_Right.png", 0.30f);
	healthContainer->setPosition(cocos2d::Vec2(visibleSize.width * 0.99 - (healthContainer->getContentSize().width * 0.30f), visibleSize.height * 0.92));
	addChild(healthContainer);

	auto healthSymbol = Sprite::create("Graphics/UI/Elements 1/Elements_02.png");
	healthSymbol->setAnchorPoint(Vec2(0, 0));
	healthSymbol->setScale(0.6f);
	healthSymbol->setPositionX(healthContainer->getContentSize().width * 0.945 - (healthSymbol->getContentSize().width * 0.60f));
	healthSymbol->setPositionY(healthContainer->getContentSize().height * 0.220);
	healthContainer->attachElement(healthSymbol);

	healthBar = Sprite::create("Graphics/UI/Elements 1/Elements_Life Bar.png");
	healthBar->setScaleX(health);
	healthBar->setAnchorPoint(Vec2(0, 0.5f));
	healthBar->setPositionX(healthContainer->getContentSize().width * 0.085);
	healthBar->setPositionY(healthContainer->getContentSize().height * 0.485);
	healthContainer->attachElement(healthBar);

	auto goldContainer = Sprite::create("Graphics/UI/Elements 1/Elements_Empty_Container_Right.png");
	goldContainer->setAnchorPoint(Vec2(0, 0));
	goldContainer->setScale(0.30f);
	goldContainer->setPosition(visibleSize.width * 0.99 - (goldContainer->getContentSize().width * 0.30f), visibleSize.height * 0.84);
	addChild(goldContainer);

	auto goldSymbol = Sprite::create("Graphics/UI/Elements 1/Elements_09.png");
	goldSymbol->setAnchorPoint(Vec2(0, 0));
	goldSymbol->setScale(0.5f);
	goldSymbol->setPositionX(goldContainer->getContentSize().width * 0.905 - (goldSymbol->getContentSize().width * 0.50f));
	goldSymbol->setPositionY(goldContainer->getContentSize().height * 0.225);
	goldContainer->addChild(goldSymbol);

	std::stringstream g;
	g << gold;
	goldLabel = Label::createWithTTF(g.str(), "fonts/carbon bl.ttf", 50);
	goldLabel->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
	goldLabel->setPositionX(
		goldContainer->getContentSize().width * 0.4
	);
	goldLabel->setPositionY(
		goldContainer->getContentSize().height / 2
	);
	goldLabel->setTag(GOLD_LABEL);
	goldContainer->addChild(goldLabel);

	auto waveContainer = Sprite::create("Graphics/UI/Window/Window_11.png");
	waveContainer->setAnchorPoint(Vec2(0, 0.5f));
	waveContainer->setScale(0.25f);
	waveContainer->setPositionX(visibleSize.width * 0.06);
	waveContainer->setPositionY(visibleSize.height * 0.75);
	addChild(waveContainer);

	std::stringstream wave;
	wave << "Wave: " << levelManager->getWaveNumber() << " / " << levelManager->getMaxWavesForLevel();
	waveLabel = Label::createWithTTF(wave.str(), "fonts/carbon phyber.ttf", 85);
	waveLabel->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
	waveLabel->setPositionX(
		waveContainer->getContentSize().width / 2
	);
	waveLabel->setPositionY(
		waveContainer->getContentSize().height * 0.75
	);
	waveLabel->setTag(WAVE_LABEL);
	waveContainer->addChild(waveLabel);

	auto waveContainer2 = Sprite::create("Graphics/UI/Window/Window_11.png");
	waveContainer2->setAnchorPoint(Vec2(0, 0.5f));
	waveContainer2->setScale(0.25f);
	waveContainer2->setPositionX(visibleSize.width * 0.06);
	waveContainer2->setPositionY(visibleSize.height * 0.45);
	addChild(waveContainer2);
}

void TowerDefence::setStarterTurrets()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	starterTurrets = turretManager->getStarterTurrets();

	float turretStandXInitialPositionPercentage = 0.20;
	float turretStandNextPositionIncrementer = 0.09;
	float turretStandYPosition = 10;

	float turretStandScale = 0.25f;
	cocos2d::Vec2 turretAnchorPoint(0, 0);
	float turretScale = 4.f;
	float turretPositionXPercentage = 0.6;
	float turretPositionYPercentage = 0.8;

	auto leftTurretStandHook = Sprite::create("Graphics/UI/Window/Window_14.png");
	leftTurretStandHook->setAnchorPoint(Vec2(0, 0));
	leftTurretStandHook->setScale(0.25f);
	leftTurretStandHook->setPosition(visibleSize.width * 0.17, -25);
	addChild(leftTurretStandHook, -1);

	std::for_each(starterTurrets.begin(), starterTurrets.end(),
		[&](Turret* turret) {

		auto TurretStand = Sprite::create("Graphics/UI/Elements 1/Elements_99.png");
		TurretStand->setAnchorPoint(turretAnchorPoint);
		TurretStand->setScale(turretStandScale);
		cocos2d::Vec2 turretStandPosition(visibleSize.width * turretStandXInitialPositionPercentage, turretStandYPosition);
		TurretStand->setPosition(turretStandPosition);
		TurretStand->setTag(MOUSE_NOT_OVER);
		addChild(TurretStand);

		turret->setAnchorPoint(cocos2d::Vec2(0.5, 0.5));
		turret->setScale(turretScale);
		turret->setPosition(
			cocos2d::Vec2(
			((TurretStand->getContentSize().width * turretStandScale) * turretPositionXPercentage) - (turret->getContentSize().width) / 6,
				((TurretStand->getContentSize().height * turretStandScale) * turretPositionYPercentage) - (turret->getContentSize().height) / 2
			)
		);

		starterTurretStands.push_back(std::move(TurretStand));
		starterTurretStands.back()->addChild(turret, 1);
		starterTurretStands.back()->setName(turret->getTurretInfo().name);

		turretStandXInitialPositionPercentage += turretStandNextPositionIncrementer;
	}
	);

	auto rightTurretStandHook = Sprite::create("Graphics/UI/Window/Window_15.png");
	rightTurretStandHook->setAnchorPoint(Vec2(0, 0));
	rightTurretStandHook->setScale(0.25f);
	rightTurretStandHook->setPosition(visibleSize.width * 0.755, -25);
	addChild(rightTurretStandHook, -1);
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

	auto eventListener = EventListenerTouchOneByOne::create();
	eventListener->onTouchBegan = CC_CALLBACK_2(TowerDefence::onTouchBegan, this);
	eventListener->onTouchMoved = CC_CALLBACK_2(TowerDefence::onTouchMoved, this);
	eventListener->onTouchEnded = CC_CALLBACK_2(TowerDefence::onTouchEnded, this);
	eventListener->onTouchCancelled = CC_CALLBACK_2(TowerDefence::onTouchCancelled, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventListener, this);

	auto _mouseListener = EventListenerMouse::create();
	_mouseListener->onMouseMove = CC_CALLBACK_1(TowerDefence::onMouseMove, this);
	//this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_mouseListener, this);

	this->scheduleUpdate();

	levelManager = new LevelManager(this);
	turretManager = new TurretManager(this);

	config();
	setUpUi();
	setStarterTurrets();

    return true;
}

void TowerDefence::onMouseMove(cocos2d::Event *event)
{
	EventMouse* e = (EventMouse*)event;
	cocos2d::Vec2 touchLoc(cocos2d::Vec2(e->getCursorX(), e->getCursorY()));

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

	for (int i = 0; i < starterTurretStands.size(); i++) {
		if (starterTurretStands[i]->getBoundingBox().containsPoint(touchLoc)) {
			starterTurretStands[i]->setTexture("Graphics/UI/Elements 1/Elements_100.png");
			starterTurretStands[i]->setTag(MOUSE_OVER);

			auto visibleSize = Director::getInstance()->getVisibleSize();
			cocos2d::Vec2 turretStatsPosition = cocos2d::Vec2(
				visibleSize.width * 0.78,
				visibleSize.height * 0.55);
			starterTurrets[i]->showTurretStats(turretStatsPosition);
		}
		else {
			starterTurretStands[i]->setTag(MOUSE_NOT_OVER);
			starterTurrets[i]->hideTurretStats();
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

void TowerDefence::menuPlayCallback(cocos2d::Ref * pSender)
{
	playGame();
}

void TowerDefence::menuResetCallback(cocos2d::Ref * pSender)
{
	config();
	healthBar->setScaleX(health);
	std::stringstream goldTxt;
	goldTxt << this->gold;
	goldLabel->setString(goldTxt.str());
	levelManager->reset();
	turretManager->reset();
}

bool TowerDefence::onTouchBegan(cocos2d::Touch * touch, cocos2d::Event * unused_event)
{
	turretManager->hideAllTurretHelpers();
	turretManager->hideAllTurretStats();
	turretManager->hideAllTurretRanges();

	cocos2d::Vec2 touchLoc = touch->getLocation();
	for (int i = 0; i < starterTurrets.size(); i++) {
		if (starterTurretStands[i]->getBoundingBox().containsPoint(touchLoc))
		{
			starterTurretStands[i]->setTexture("Graphics/UI/Elements 1/Elements_101.png");
			prevPos = starterTurrets[i]->getPosition();
			selectedTurret = new Turret(this, TurretInfo(starterTurrets[i]->getTurretInfo().imageHeader), false);
			selectedTurret->setPosition(
				cocos2d::Vec2(
					starterTurretStands[i]->getPosition().x + selectedTurret->getPosition().x,
					starterTurretStands[i]->getPosition().y + selectedTurret->getPosition().y
				)
			);
			selectedTurret->setRangeColor(cocos2d::Color4F(1, 0, 0, 0.25));
			selectedTurret->showRange();
			selectedTurret->setTag(SELECTED_TURRET);
			auto visibleSize = Director::getInstance()->getVisibleSize();
			cocos2d::Vec2 turretStatsPosition = cocos2d::Vec2(
				visibleSize.width * 0.78,
				visibleSize.height * 0.55);
			selectedTurret->showTurretStats(turretStatsPosition);
			addChild(selectedTurret, 1);
			return true;
		}
	}

	for (int i = 0; i < turretManager->getPlacedTurrets().size(); i++) {
		if (turretManager->getPlacedTurrets().at(i)->getBoundingBox().containsPoint(touchLoc)) {
			turretManager->getPlacedTurrets().at(i)->showHelperButtons();
			turretManager->getPlacedTurrets().at(i)->showRange();
			return true;
		}
	}

	return false;
}

void TowerDefence::onTouchMoved(cocos2d::Touch * touch, cocos2d::Event * unused_event)
{
	if (selectedTurret) {
		selectedTurret->setPosition(selectedTurret->getPosition() + touch->getDelta());
		cocos2d::Vec2 tileCoordForTowerPosition = levelManager->tileCoordForPosition(selectedTurret->getPosition());
		if (levelManager->isWallAtTileCoord(tileCoordForTowerPosition)) {
			selectedTurret->setRangeColor(cocos2d::Color4F(0, 1, 0, 0.25));
			selectedTurret->showRange();
		}
		else {
			selectedTurret->setRangeColor(cocos2d::Color4F(1, 0, 0, 0.25));
			selectedTurret->showRange();
		}
	}
}

void TowerDefence::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	if (selectedTurret) 
	{
		if (gold >= selectedTurret->getTurretInfo().cost) 
		{
			selectedTurret->hideRange();
			cocos2d::Vec2 tileCoordForTowerPosition = levelManager->tileCoordForPosition(selectedTurret->getPosition());

			if (levelManager->isWallAtTileCoord(tileCoordForTowerPosition))
			{
				cocos2d::Vec2 positionOfTileCoord = levelManager->positionForTileCoord(tileCoordForTowerPosition);
				if (!turretManager->hasTurretAtCoord(positionOfTileCoord))
				{
					decreaseGold(selectedTurret->getTurretInfo().cost);
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

void TowerDefence::config()
{
	auto fileData = cocos2d::FileUtils::getInstance()->getDataFromFile(CONFIG_FILE);
	std::string fileStr((const char*)fileData.getBytes(), fileData.getSize());

	rapidjson::Document configInfoDoc;
	configInfoDoc.Parse(fileStr.c_str());

	if (configInfoDoc.HasMember("config"))
	{
		const rapidjson::Value& configInfo = configInfoDoc["config"];
		healthDecrementer = 1 / configInfo["health"].GetFloat();
		health = 1;
		gold = configInfo["gold"].GetInt();

		if (configInfo.HasMember("Music"))
		{
			if (configInfo["Music"]["play"].GetBool())
				CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic(
					configInfo["Music"]["source"].GetString(),
					true
				);
		}
	}
	else {
		health = 20;
		gold = 75;
	}
}

void TowerDefence::playGame()
{
	levelManager->startLevel();
}

void TowerDefence::update(float delta)
{
	levelManager->update(delta);
	turretManager->update(delta);

	for (int i = 0; i < starterTurretStands.size(); i++) {
		if (starterTurrets[i]->getTurretInfo().cost > gold)
			starterTurretStands[i]->setTexture("Graphics/UI/Elements 1/Elements_90.png");
		else {
			if (starterTurretStands[i]->getTag() != MOUSE_OVER) {
				starterTurretStands[i]->setTexture("Graphics/UI/Elements 1/Elements_99.png");
			}
		}
	}
}

LevelManager * TowerDefence::getLevelManager()
{
	return levelManager;
}

void TowerDefence::enablePlayButton()
{

}

void TowerDefence::decreaseHealth()
{
	health -= healthDecrementer;

	if (health <= 0)
		health = 0;

	healthBar->setScaleX(health);
}

void TowerDefence::increaseGold(int gold)
{
	this->gold += gold;
	std::stringstream goldTxt;
	goldTxt << this->gold;
	goldLabel->setString(goldTxt.str());
}

void TowerDefence::decreaseGold(int gold)
{
	this->gold -= gold;
	std::stringstream goldTxt;
	goldTxt << this->gold;
	goldLabel->setString(goldTxt.str());
}

int TowerDefence::getGold()
{
	return gold;
}

int TowerDefence::getHealth()
{
	return health;
}

void TowerDefence::setWaveNumber(int waveNumber)
{
	std::stringstream waveTxt;
	waveTxt << "Wave: " << waveNumber << " / " << levelManager->getMaxWavesForLevel();
	waveLabel->setString(waveTxt.str());
}

bool TowerDefence::checkCollision(CGCircle *rangeIndicator, cocos2d::Rect rect)
{
	return rangeIndicator->isContainRect(rect);
}
