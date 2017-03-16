#include "SplashScene.h"
#include "MainMenuScene.h"
#include "Utils.h"

USING_NS_CC;

Scene* SplashScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = SplashScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool SplashScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	this->scheduleOnce(schedule_selector(SplashScene::goToMainMenuScene), DISPLAY_TIME_SPLASH_SCENE);

	auto bgS = cocos2d::Sprite::create("Graphics/Splash.jpg");
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
	return true;
}

void SplashScene::goToMainMenuScene(float dt)
{
	auto scene = MainMenuScene::createScene();

	Director::getInstance()->replaceScene(cocos2d::TransitionFade::create(TRANSITION_TIME, scene));
}
