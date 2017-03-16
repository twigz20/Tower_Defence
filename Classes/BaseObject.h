#pragma once
#include "cocos2d.h"

class BaseObject : public cocos2d::Node
{
protected:
	cocos2d::Sprite *sprite;

public:
	BaseObject();
	~BaseObject();

	void setPosition(const cocos2d::Vec2 &position);
	const cocos2d::Vec2& getPosition() const;
	cocos2d::Rect getBoundingBox() const;
	const cocos2d::Size & getContentSize() const;
	cocos2d::Action *runAction(cocos2d::Action* action);
};

