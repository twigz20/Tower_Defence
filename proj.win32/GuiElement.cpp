#include "GuiElement.h"
#include "cocos2d.h"

using namespace cocos2d;

GuiElement::GuiElement(const char * filename, float scale)
{
	sprite = Sprite::create(filename);
	sprite->setScale(scale);
	sprite->setAnchorPoint(Vec2(0, 0));
	addChild(sprite);
}

GuiElement::~GuiElement()
{
}

void GuiElement::attachElement(cocos2d::Node * child)
{
	sprite->addChild(child);
}