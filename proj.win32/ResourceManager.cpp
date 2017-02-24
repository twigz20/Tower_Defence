#include "ResourceManager.h"
#include "cocos2d.h"


ResourceManager::ResourceManager()
{
}


ResourceManager::~ResourceManager()
{
	sprites.clear();
}

cocos2d::Sprite* ResourceManager::getTexture(const std::string & filename)
{
	auto i = sprites.find(filename);
	if (i == sprites.end())
	{
		auto sprite = cocos2d::Sprite::create(filename);
		sprites[filename] = std::move(sprite);
		//auto x = std::pair<std::string, std::unique_ptr<cocos2d::Sprite>>(filename, sprite);
		//sprites.insert(std::pair<std::string, std::unique_ptr<cocos2d::Sprite>>(filename, sprite));
		return sprite;
	}
	return i->second;
}
