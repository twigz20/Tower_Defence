#pragma once
#include "cocos2d.h"
#include "TurretInfo.h"

class TowerDefence;
class Creep;
class CGCircle;
class Turret : public cocos2d::Node
{
	TurretInfo *info;

	TowerDefence* game;
	cocos2d::Sprite *sprite;

	Turret* initWithTheGame(TowerDefence* game_, TurretInfo *turretInfo);
	cocos2d::CustomCommand _customCommand;
	
	bool displayRange;

	bool attacking;
	Creep *chosenCreep;

	bool starterTurret;
	bool turretActive;

	CGCircle *rangeIndicator;
	CGCircle *splashDamageRange;
	void addRangeIndicator();

	cocos2d::DrawNode *range;
public:
	Turret(bool isStarterTurret = false);
	Turret(const Turret& other);
	Turret(Turret&& other);
	Turret& operator=(Turret&& other);
	~Turret();

	bool checkCollision(CGCircle *rangeIndicator, cocos2d::Rect rect);

	Turret* nodeWithTheGame(TowerDefence* game_, TurretInfo *turretInfo);
	void update(float deltaTime);
	virtual void draw(cocos2d::Renderer* renderer, const cocos2d::Mat4& transform, uint32_t flags);
	virtual void onDraw(const cocos2d::kmMat4 &transform, uint32_t flags);
	void setPosition(const cocos2d::Vec2 &position);
	const cocos2d::Vec2& getPosition() const;
	cocos2d::Rect getBoundingBox() const;

	void activateTurret();
	TurretInfo* getTurretInfo();
	void showRange();
	void hideRange();

	// Add method definition
	void targetKilled();
	void attackEnemy();
	void chosenEnemyForAttack(Creep *enemy);
	void shootWeapon(float dt);
	void removeBullet(cocos2d::Sprite *bullet);
	void damageEnemy();
	void lostSightOfEnemy();
	void rotateToTarget();
};

