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

	bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event) override;
	void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event) override;
	void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event) override;
	void onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *unused_event) override;

	void playGame();
	void touchEvent(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type);

	Turret *selectedTurret;
	int selectedTurretIndex;
	cocos2d::Vec2 prevPos;
public:
    static cocos2d::Scene* createScene();
	~TowerDefence();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(TowerDefence);

	void update(float) override;
	bool checkCollision(CGCircle *rangeIndicator, cocos2d::Rect rect);
	LevelManager *getLevelManager();

	
};

#endif // __HELLOWORLD_SCENE_H__
