#include "Bullet.h"
#include "BulletInfo.h"

using namespace cocos2d;

Bullet::Bullet(BulletInfo & info_, Creep * Target, cocos2d::Vec2 Position, cocos2d::Vec2 Direction) :
	BaseObject(info_.image),
	info(info_),
	isDead_(false),
	direction(Direction),
	target(Target)
{
	setPosition(Position);
	float Angle = AngleBetweenD(cocos2d::Vec2(0, -1), direction);
	if (direction.x < 0)
	{
		object->setRotation(Angle);
	}
	else
	{
		object->setRotation(360 - Angle);
	}
}

Bullet::~Bullet()
{
}

void Bullet::update(const float deltaTime)
{
	//cocos2d::MoveBy(direction * info.speed * deltaTime);
	//MoveTo *moveAction = MoveTo::create(100 / 100, direction * 100 * deltaTime);
	//CallFunc *moveCallback = CallFunc::create(CC_CALLBACK_0(Bullet::update, this));

	// Running action
	//Sequence *moveSequence = Sequence::create(moveAction, nullptr, nullptr);
	//moveSequence->setTag(1);
	//(object)->runAction(moveSequence);
}

float Bullet::AngleBetweenR(cocos2d::Vec2 &v1, cocos2d::Vec2 &v2)
{
	return acos(Dot(v1, v2));
}

float Bullet::AngleBetweenD(cocos2d::Vec2 &v1, cocos2d::Vec2 &v2)
{
	return CC_RADIANS_TO_DEGREES(AngleBetweenR(v1, v2));
}

float Bullet::Dot(cocos2d::Vec2 &v1, cocos2d::Vec2 &v2)
{
	return v1.x * v2.x + v1.y * v2.y;
}

bool Bullet::isDead()
{
	return false;
}

void Bullet::setDead(bool dead)
{
}

void Bullet::shoot()
{
	//cocos2d::MoveBy(direction * info.speed * deltaTime);
	//MoveTo *moveAction = MoveTo::create(100 / 100, direction * 100 * deltaTime);
	//CallFunc *moveCallback = CallFunc::create(CC_CALLBACK_0(Bullet::update, this));

	// Running action
	//Sequence *moveSequence = Sequence::create(moveAction, nullptr, nullptr);
	//moveSequence->setTag(1);
	//(object)->runAction(moveSequence);

	//MoveTo *moveAction = MoveTo::create(0.5, target->getPosition());
	MoveTo *move = MoveTo::create(0.1f,target->getPosition());
	//move->MoveTo::initWithDuration(0.1, target->getPosition());
	Sequence *moveSequence = Sequence::create(move);
	object->runAction(move);
	/*[bullet runAction : [CCSequence actions :
	[CCMoveTo actionWithDuration : 0.1 position : chosenEnemy.mySprite.position],
		[CCCallFunc actionWithTarget : self selector : @selector(damageEnemy)],
		[CCCallFuncN actionWithTarget : self selector : @selector(removeBullet : )], nil]];*/
}

void Bullet::onUpdate(const float deltaTime)
{
}

void Bullet::onHit(Creep * TheEnemy)
{
}
