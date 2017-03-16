#pragma once
#include "cocos2d.h"

class TowerDefence;
struct TurretInfo;
class TurretStatsDisplay : public cocos2d::Node
{
	TurretInfo &info;
	TowerDefence* game;
	cocos2d::Sprite *sprite;
	cocos2d::CustomCommand _customCommand;

	void setupDamageDisplay();
	void setupFireRateDisplay();
	void setupRangeDisplay();
	int calculateDisplayBarAmount(std::string type);

	cocos2d::Label * costLabel;

public:
	TurretStatsDisplay(TowerDefence* game_, TurretInfo &turretInfo, cocos2d::Vec2 position);
	~TurretStatsDisplay();

	virtual void draw(cocos2d::Renderer* renderer, const cocos2d::Mat4& transform, uint32_t flags);
	virtual void onDraw(const cocos2d::kmMat4 &transform, uint32_t flags);
	void setPosition(const cocos2d::Vec2 &position);
	const cocos2d::Vec2& getPosition() const;
	cocos2d::Rect getBoundingBox() const;
	
	void changeTurret(TurretInfo &turretInfo);
	void changePosition(cocos2d::Vec2 position);
	void setupDisplay(cocos2d::Vec2 position);
	void clearStats();
	void show();
	void hide();
	void showCost();
	void hideCost();
};

