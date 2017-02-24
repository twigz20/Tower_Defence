#pragma once
#include "cocos2d.h"

#include <map>
#include <memory>

class ResourceManager
{
	ResourceManager();
	std::map<std::string, cocos2d::Sprite*> sprites;

public:
	virtual ~ResourceManager();
	cocos2d::Sprite* getTexture(const std::string& filename);

	static ResourceManager& getInstance()
	{
		static ResourceManager rm;
		return rm;
	}
};

