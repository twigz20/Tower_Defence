#pragma once
#include <memory>
#include <string>

#include "cocos2d.h"

class BaseObject
{
	std::string filename;
protected:
	cocos2d::Sprite* object;
public:
	BaseObject(const std::string& filename);
	BaseObject(cocos2d::Sprite* object_);
	~BaseObject();

	virtual void update(float deltaTime) = 0;

	void setPosition(float x, float y);
	void setPositionX(float x);
	void setPositionY(float y);
	float getPositionX();
	float getPositionY();
	float getWidth();
	float getHeight();

	cocos2d::Vec2 getPosition() const;

	cocos2d::Sprite* getObject() const;
};

