#pragma once
#include "cocos2d.h"
#include "CreepManager.h"

struct BulletInfo;
class Bullet : public BaseObject
{
	float AngleBetweenR(cocos2d::Vec2 &v1, cocos2d::Vec2 &v2);

	float AngleBetweenD(cocos2d::Vec2 &v1, cocos2d::Vec2 &v2);

	float Dot(cocos2d::Vec2 &v1, cocos2d::Vec2 &v2);

	cocos2d::Vec2 direction;
	Creep *target;
public:
	Bullet(BulletInfo& info, Creep* Target, cocos2d::Vec2 Position, cocos2d::Vec2 Direction);

	~Bullet();

	void update(float deltaTime);


	bool isDead();

	void setDead(bool dead);

	void shoot();


protected:
	void onUpdate(const float deltaTime);
	void onHit(Creep* TheEnemy);

	BulletInfo& info;

	bool isDead_;
};
