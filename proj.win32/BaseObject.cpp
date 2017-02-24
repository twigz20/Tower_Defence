#include "BaseObject.h"

BaseObject::BaseObject(const std::string & filename)
{
	object = cocos2d::Sprite::create(filename);
}

BaseObject::BaseObject(cocos2d::Sprite * object_)
{
	object = object_;
}

BaseObject::~BaseObject()
{
}

void BaseObject::loadSprite(const std::string & filename)
{
	object = cocos2d::Sprite::create(filename);
}

void BaseObject::setPosition(cocos2d::Vec2 position)
{
	(object)->setPosition(position);
}

void BaseObject::setPosition(float x, float y)
{
	(object)->setPosition(x,y);
}

void BaseObject::setPositionX(float x)
{
	(object)->setPositionX(x);
}

void BaseObject::setPositionY(float y)
{
	(object)->setPositionY(y);
}

float BaseObject::getPositionX()
{
	return (object)->getPositionY();
}

float BaseObject::getPositionY()
{
	return (object)->getPositionY();
}

float BaseObject::getWidth()
{
	return (object)->getBoundingBox().size.width;
}

float BaseObject::getHeight()
{
	return (object)->getBoundingBox().size.height;
}

cocos2d::Vec2 BaseObject::getPosition() const
{
	return object->getPosition();
}

cocos2d::Sprite * BaseObject::getObject() const
{
	return (object);
}
