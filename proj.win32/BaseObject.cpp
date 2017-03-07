#include "BaseObject.h"



BaseObject::BaseObject()
{
}


BaseObject::~BaseObject()
{
}

void BaseObject::setPosition(const cocos2d::Vec2 & position)
{
	sprite->setPosition(position);
}

const cocos2d::Vec2& BaseObject::getPosition() const
{
	return sprite->getPosition();
}

cocos2d::Rect BaseObject::getBoundingBox() const
{
	return sprite->getBoundingBox();
}

const cocos2d::Size & BaseObject::getContentSize() const
{
	return sprite->getContentSize();
}

cocos2d::Action * BaseObject::runAction(cocos2d::Action * action)
{
	sprite->runAction(action);
	return action;
}
