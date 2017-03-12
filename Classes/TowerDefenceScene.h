#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "ui\CocosGUI.h"
#include <memory>
#include <vector>
#include "proj.win32\CGCircle.h"
#define SELECTED_TURRET 10001

class Creep;
class LevelManager;
class TurretManager;
class Turret;
class TowerDefence : public cocos2d::Layer
{
	LevelManager *levelManager;
	TurretManager *turretManager;

	cocos2d::MenuItemImage *playItem;
	cocos2d::MenuItemImage *resetItem;

	bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event) override;
	void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event) override;
	void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event) override;
	void onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *unused_event) override;

	void onMouseMove(cocos2d::Event * event);

	void playGame();
	void touchEvent(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type);

	Turret *selectedTurret;
	int selectedTurretIndex;
	cocos2d::Vec2 prevPos;

	cocos2d::ui::Button *sell;
	cocos2d::ui::Button *upgrade;
	cocos2d::ui::Button *help;

	void setUpUi();

	std::vector<cocos2d::Sprite*> starterTurretStands;
	std::vector<std::shared_ptr<Turret>> starterTurrets;
	void setStarterTurrets();
public:
    static cocos2d::Scene* createScene();
	~TowerDefence();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
	void menuPlayCallback(cocos2d::Ref* pSender);
	void menuResetCallback(cocos2d::Ref* pSender);

	void sellCallback(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
	void upgradeCallback(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
	void helpCallback(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
	void repeatCallback(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
    
    // implement the "static create()" method manually
    CREATE_FUNC(TowerDefence);

	void update(float) override;
	bool checkCollision(CGCircle *rangeIndicator, cocos2d::Rect rect);
	LevelManager *getLevelManager();

	void enablePlayButton();
};

#endif // __HELLOWORLD_SCENE_H__
