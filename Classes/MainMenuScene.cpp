#include "MainMenuScene.h"
#include "TowerDefenceScene.h"
#include "Utils.h"

USING_NS_CC;

using namespace cocos2d;
using namespace ui;

Scene* MainMenuScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = MainMenuScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool MainMenuScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	
	auto bgS = cocos2d::Sprite::create("Graphics/UI/Elements 1/BG.png");
	cocos2d::Rect s_visibleRect = Director::getInstance()->getOpenGLView()->getVisibleRect();
	float scaleY = s_visibleRect.size.height / bgS->getContentSize().height;
	float scaleX = s_visibleRect.size.width / bgS->getContentSize().width;
	if (scaleX>scaleY) {
		bgS->setScale(scaleX);
	}
	else {
		bgS->setScale(scaleY);
	}
	bgS->setPosition(Vec2(s_visibleRect.origin.x + s_visibleRect.size.width / 2, s_visibleRect.origin.y + s_visibleRect.size.height / 2));
	addChild(bgS, -2);

	auto logoTopBackRestMiddle = cocos2d::Sprite::create("Graphics/UI/Elements 1/Elements_65.png");
	logoTopBackRestMiddle->setScaleX(0.50);
	logoTopBackRestMiddle->setScaleY(0.30);
	logoTopBackRestMiddle->setPositionX(origin.x + visibleSize.width / 2);
	logoTopBackRestMiddle->setPositionY(origin.y + visibleSize.height * 0.825);
	addChild(logoTopBackRestMiddle, 0);

	auto logoTopBackRestLeft = cocos2d::Sprite::create("Graphics/UI/Elements 1/Elements_65.png");
	logoTopBackRestLeft->setScaleX(0.50);
	logoTopBackRestLeft->setScaleY(0.40);
	logoTopBackRestLeft->setPositionX(visibleSize.width * 0.50 * .725);
	logoTopBackRestLeft->setPositionY(origin.y + visibleSize.height * 0.7775);
	addChild(logoTopBackRestLeft, -1);

	auto logoTopBackRestRight = cocos2d::Sprite::create("Graphics/UI/Elements 1/Elements_65.png");
	logoTopBackRestRight->setScaleX(0.50);
	logoTopBackRestRight->setScaleY(0.40);
	logoTopBackRestRight->setPositionX(visibleSize.width * 0.50 * 1.275);
	logoTopBackRestRight->setPositionY(origin.y + visibleSize.height * 0.7775);
	addChild(logoTopBackRestRight, -1);

	auto logoBotBackRestMiddle = cocos2d::Sprite::create("Graphics/UI/Elements 1/Elements_65.png");
	logoBotBackRestMiddle->setScale(0.40);
	logoBotBackRestMiddle->setScaleY(0.70);
	logoBotBackRestMiddle->setPositionX(origin.x + visibleSize.width * 0.5085);
	logoBotBackRestMiddle->setPositionY(origin.y + visibleSize.height * 0.65);
	addChild(logoBotBackRestMiddle, 0);

	auto logoBotBackRestLeft = cocos2d::Sprite::create("Graphics/UI/Window/Window_10.png");
	logoBotBackRestLeft->setScale(0.50);
	logoBotBackRestLeft->setPositionX(visibleSize.width * 0.325);
	logoBotBackRestLeft->setPositionY(origin.y + visibleSize.height * 0.6285);
	addChild(logoBotBackRestLeft, 0);

	auto logoBotBackRestRight = cocos2d::Sprite::create("Graphics/UI/Window/Window_12.png");
	logoBotBackRestRight->setScale(0.50);
	logoBotBackRestRight->setPositionX(visibleSize.width * 0.685);
	logoBotBackRestRight->setPositionY(origin.y + visibleSize.height * 0.6285);
	addChild(logoBotBackRestRight, 0);

	auto logoTopText = cocos2d::Sprite::create("Graphics/Logo-top.png");
	logoTopText->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height * 0.75));
	addChild(logoTopText, 0);

	auto logoBotText = cocos2d::Sprite::create("Graphics/Logo-bot.png");
	logoBotText->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height * 0.62));
	addChild(logoBotText, 0);

	auto menuItemsContainer = cocos2d::Sprite::create("Graphics/UI/Elements 1/Elements_65.png");
	menuItemsContainer->setScaleX(0.75);
	menuItemsContainer->setPositionX(origin.x + visibleSize.width / 2);
	menuItemsContainer->setPositionY(75);
	addChild(menuItemsContainer, 0);

	auto menuItemsPlay = Button::create("Graphics/UI/Buttons/Button_14.png", "Graphics/UI/Buttons/Button_16.png", "Graphics/UI/Buttons/Button_17.png");
	menuItemsPlay->setScale(1.35);
	menuItemsPlay->setPositionX(menuItemsContainer->getContentSize().width * .1);
	menuItemsPlay->setPositionY(menuItemsContainer->getContentSize().height * .80);
	menuItemsPlay->addTouchEventListener(CC_CALLBACK_2(MainMenuScene::menuPlayCallback, this));
	menuItemsContainer->addChild(menuItemsPlay, 1);

	auto menuItemsPlay2 = Button::create("Graphics/UI/Buttons/Button_54.png", "Graphics/UI/Buttons/Button_56.png", "Graphics/UI/Buttons/Button_57.png");
	menuItemsPlay2->setScale(1.35);
	menuItemsPlay2->setPositionX(menuItemsContainer->getContentSize().width * .30);
	menuItemsPlay2->setPositionY(menuItemsContainer->getContentSize().height * .80);
	menuItemsPlay2->setEnabled(false);
	//menuItemsPlay->addTouchEventListener(CC_CALLBACK_2(Turret::upgradeCallback, this));
	menuItemsContainer->addChild(menuItemsPlay2, 1);

	auto menuItemsPlay3 = Button::create("Graphics/UI/Buttons/Button_46.png", "Graphics/UI/Buttons/Button_48.png", "Graphics/UI/Buttons/Button_49.png");
	menuItemsPlay3->setScale(1.35);
	menuItemsPlay3->setPositionX(menuItemsContainer->getContentSize().width * .5);
	menuItemsPlay3->setPositionY(menuItemsContainer->getContentSize().height * .80);
	menuItemsPlay3->setEnabled(false);
	//menuItemsPlay->addTouchEventListener(CC_CALLBACK_2(Turret::upgradeCallback, this));
	menuItemsContainer->addChild(menuItemsPlay3, 1);

	auto menuItemsSettings = Button::create("Graphics/UI/Buttons/Button_98.png", "Graphics/UI/Buttons/Button_100.png", "Graphics/UI/Buttons/Button_101.png");
	menuItemsSettings->setScale(1.35);
	menuItemsSettings->setPositionX(menuItemsContainer->getContentSize().width * .7);
	menuItemsSettings->setPositionY(menuItemsContainer->getContentSize().height * .80);
	menuItemsSettings->setEnabled(false);
	//menuItemsPlay->addTouchEventListener(CC_CALLBACK_2(Turret::upgradeCallback, this));
	menuItemsContainer->addChild(menuItemsSettings, 1);

	auto menuItemsPlay5 = Button::create("Graphics/UI/Buttons/Button_42.png", "Graphics/UI/Buttons/Button_44.png", "Graphics/UI/Buttons/Button_45.png");
	menuItemsPlay5->setScale(1.35);
	menuItemsPlay5->setPositionX(menuItemsContainer->getContentSize().width * .9);
	menuItemsPlay5->setPositionY(menuItemsContainer->getContentSize().height * .80);
	menuItemsPlay5->setEnabled(false);
	//menuItemsPlay->addTouchEventListener(CC_CALLBACK_2(Turret::upgradeCallback, this));
	menuItemsContainer->addChild(menuItemsPlay5, 1);

	return true;
}

void MainMenuScene::menuPlayCallback(cocos2d::Ref * pSender, cocos2d::ui::Widget::TouchEventType type)
{
	auto scene = TowerDefence::createScene();

	Director::getInstance()->replaceScene(cocos2d::TransitionFade::create(TRANSITION_TIME, scene));
}
