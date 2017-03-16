#pragma once
#include "BaseObject.h"

class TowerDefence;
class GuiElement : public BaseObject
{
	float scale;

public:
	GuiElement(const char* filename, float scale = 1.f);
	~GuiElement();

	void attachElement(cocos2d::Node * child);
};

