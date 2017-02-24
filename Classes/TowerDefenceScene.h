#ifndef __TOWER_DEFENCE_SCENE_H__
#define __TOWER_DEFENCE_SCENE_H__

#include "cocos2d.h"
#include "ui\CocosGUI.h"
#include <memory>
#include <vector>

class Creep;
class LevelManager;
class TurretManager;
class Turret;
class TowerDefence : public cocos2d::Layer
{
	LevelManager *levelManager;
	TurretManager *turretManager;
	int x;

	bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event) override;
	void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event) override;
	void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event) override;
	void onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *unused_event) override;

	std::vector<Turret*> starterTurrets;
	void setStartedTurrets();
	Turret *selectedTurret;
	cocos2d::Vec2 prevPos;

	void playGame();
	void touchEvent(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type);
public:
    static cocos2d::Scene* createScene();

    virtual bool init()  override;

	cocos2d::CCPoint touchToPoint(cocos2d::CCTouch* touch);

	// Returns true if the touch is within the boundary of our sprite
	bool isTouchingSprite(cocos2d::CCTouch* touch);
    
    // implement the "static create()" method manually
    CREATE_FUNC(TowerDefence);

	void update(float) override;
};

#endif // __TOWER_DEFENCE_SCENE_H__
