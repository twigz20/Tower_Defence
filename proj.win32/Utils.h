#pragma once
#define WINDOW_HEIGHT 10 * 48
#define WINDOW_WIDTH 15 * 48
#define BACKGROUND_FILE "Graphics/tower-defense/tower-defense-background-stars.png"
#define INITIAL_MAP_FILE "Files/Tile-Maps/Level-1/Map.tmx"
#define WAVES_FILE "Files/waves.json"
#define CREEP_FILE "Files/creep.json"
#define TURRET_FILE "Files/turrets.json"
#define CONFIG_FILE "Files/config.json"
#define TILE_TAKEN 200
#define TURRET_STATS_TAG 201
#define TURRET_STATS_BACKGROUND "Graphics/tower-defense/stats-background.png"
#define TURRET_STAT_CURRENT "Graphics/tower-defense/stat-current.png"
#define TURRET_STAT_UPGRADE "Graphics/tower-defense/stat-upgrade.png"
#define TURRET_SPRITE_TAG 300
#define TURRET_SPRITE_RANGE_TAG 301
#define START_FILE "Graphics/UI/Start.png"
#define SELL_FILE "Graphics/UI/Sell.png"
#define UPGRADE_FILE "Graphics/UI/Upgrade.png"
#define HELP_FILE "Graphics/UI/Help.png"


#include <string>
#include <fstream>
#include <sstream>
#include "cocos2d.h"

static std::string getFileContent(const std::string& path)
{
	std::ifstream file(path);
	std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

	return content;
}

static cocos2d::Node* cloneNode(const cocos2d::Node *source)
{
	cocos2d::Node* clone = cocos2d::Node::create();
	for (cocos2d::Node* srcSubnode : source->getChildren()) {

		cocos2d::Node* subnode;

		if (srcSubnode->getTag() == TURRET_SPRITE_TAG) { 
			cocos2d::Sprite* srcSprite = (cocos2d::Sprite*)srcSubnode;
			subnode = cocos2d::Sprite::createWithTexture(srcSprite->getTexture());
			((cocos2d::Sprite*)subnode)->setSpriteFrame(srcSprite->getSpriteFrame());
		}
		else {
			subnode = cloneNode(srcSubnode);
		}

		subnode->setRotation(srcSubnode->getRotation());
		subnode->setPosition(srcSubnode->getPosition());
		subnode->setAnchorPoint(srcSubnode->getAnchorPoint());
		subnode->setLocalZOrder(srcSubnode->getLocalZOrder());
		clone->addChild(subnode);
	}

	return clone;
}