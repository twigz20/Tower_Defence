#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "ui\CocosGUI.h"
#include <memory>
#include <vector>
#include "CGCircle.h"
#define SELECTED_TURRET 10001

class Creep;
class LevelManager;
class TurretManager;
class Turret;
class TowerDefence : public cocos2d::Layer
{
	LevelManager *levelManager;
	TurretManager *turretManager;
	cocos2d::TMXTiledMap *tileMap;
	cocos2d::TMXLayer *bgLayer, *objectLayer;
	cocos2d::MenuItemImage *playItem;
	cocos2d::MenuItemImage *resetItem;

	bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	void onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *unused_event);

	void onMouseMove(cocos2d::Event * event);

	int gold;
	cocos2d::Label * goldLabel;
	float healthDecrementer;
	float health;
	cocos2d::Label * healthLabel;
	cocos2d::Sprite *healthBar;

	cocos2d::Sprite *statsContainer;
	cocos2d::Label * waveLabel;

	void config();
	void playGame();

	Turret *selectedTurret;
	int selectedTurretIndex;
	cocos2d::Vec2 prevPos;

	void setUpUi();

	std::vector<cocos2d::Sprite*> starterTurretStands;
	std::vector<Turret*> starterTurrets;
	void setStarterTurrets();
public:
    static cocos2d::Scene* createScene();
	~TowerDefence();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
	void menuPlayCallback(cocos2d::Ref* pSender);
	void menuResetCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(TowerDefence);

	void update(float) override;
	bool checkCollision(CGCircle *rangeIndicator, cocos2d::Rect rect);
	LevelManager *getLevelManager();

	void enablePlayButton();
	void decreaseHealth();
	void increaseGold(int gold);
	void decreaseGold(int gold);

	int getGold();
	int getHealth();

	void setWaveNumber(int waveNumber);
};

#endif // __HELLOWORLD_SCENE_H__
