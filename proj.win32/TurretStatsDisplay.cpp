#include "TurretStatsDisplay.h"
#include "TurretInfo.h"
#include "TowerDefenceScene.h"
#include "Utils.h"

using namespace cocos2d;

TurretStatsDisplay * TurretStatsDisplay::initWithTheGame(TowerDefence * game_, TurretInfo * turretInfo, cocos2d::Vec2 position)
{
	if (init()) {
		game = game_;
		info = turretInfo;
		sprite = Sprite::create(TURRET_STATS_BACKGROUND);
		sprite->setAnchorPoint(cocos2d::Vec2(0, 0));
		sprite->setScale(1.5);
		addChild(sprite);
		setPosition(position);
		setupDisplay();
	}

	return this;
}

void TurretStatsDisplay::setupDisplay()
{
	auto nameLabel = Label::createWithSystemFont(info->name, "Arial", 18);
	nameLabel->setAnchorPoint(cocos2d::Vec2(0, 0));
	nameLabel->setPosition(
		sprite->getPosition().x + ((sprite->getContentSize().width * 1.5) * 0.07),
		sprite->getPosition().y + ((sprite->getContentSize().height * 1.5) * 0.77)
	);
	nameLabel->setColor(cocos2d::Color3B::GREEN);
	addChild(nameLabel);

	std::stringstream ss;
	ss << "Level " << info->level;
	auto levelLabel = Label::createWithSystemFont(ss.str().c_str(), "Arial", 18);
	levelLabel->setAnchorPoint(cocos2d::Vec2(0, 0));
	levelLabel->setPosition(
		sprite->getPosition().x + ((sprite->getContentSize().width * 1.5) * 0.07),
		sprite->getPosition().y + ((sprite->getContentSize().height * 1.5) * 0.58)
	);
	levelLabel->setColor(cocos2d::Color3B::GRAY);
	addChild(levelLabel);

	setupDamageDisplay();
	setupFireRateDisplay();
	setupRangeDisplay();

	std::stringstream cost;
	cost << "Cost: " << info->cost;
	costLabel = Label::createWithSystemFont(cost.str(), "Arial", 18);
	costLabel->setAnchorPoint(cocos2d::Vec2(0, 0));
	costLabel->setPosition(
		sprite->getPosition().x + ((sprite->getContentSize().width * 1.5) * 0.07),
		sprite->getPosition().y - ((sprite->getContentSize().width * 1.5) * 0.20)
	);
	costLabel->setColor(cocos2d::Color3B::GRAY);
	addChild(costLabel);
}

void TurretStatsDisplay::setupDamageDisplay()
{
	std::string damageTxt = "DAM";
	auto damageLabel = Label::createWithSystemFont(damageTxt.c_str(), "Arial", 18);
	damageLabel->setAnchorPoint(cocos2d::Vec2(0, 0));
	damageLabel->setPosition(
		sprite->getPosition().x + ((sprite->getContentSize().width * 1.5) * 0.07),
		sprite->getPosition().y + ((sprite->getContentSize().height * 1.5) * 0.35)
	);
	damageLabel->setColor(cocos2d::Color3B::GRAY);
	addChild(damageLabel);

	int bars = calculateDisplayBarAmount(damageTxt);
	float percentagePosition = 0.48;

	for (int i = 0; i < bars; i++) {
		Sprite *current = Sprite::create(TURRET_STAT_CURRENT);
		current->setScaleX(1.75);
		current->setScaleY(2.5);
		current->setAnchorPoint(cocos2d::Vec2(0, 0));
		current->setPosition(
			sprite->getPosition().x + ((sprite->getContentSize().width * 1.5) * percentagePosition),
			damageLabel->getPositionY()
		);
		addChild(current);
		percentagePosition += 0.09;
	}
}

void TurretStatsDisplay::setupFireRateDisplay()
{
	std::string fireRateTxt = "RTE";
	auto fireRateLabel = Label::createWithSystemFont(fireRateTxt.c_str(), "Arial", 18);
	fireRateLabel->setAnchorPoint(cocos2d::Vec2(0, 0));
	fireRateLabel->setPosition(
		sprite->getPosition().x + ((sprite->getContentSize().width * 1.5) * 0.07),
		sprite->getPosition().y + ((sprite->getContentSize().height * 1.5) * 0.20)
	);
	fireRateLabel->setColor(cocos2d::Color3B::GRAY);
	addChild(fireRateLabel);

	int bars = calculateDisplayBarAmount(fireRateTxt);
	float percentagePosition = 0.48;

	for (int i = 0; i < bars; i++) {
		Sprite *current = Sprite::create(TURRET_STAT_CURRENT);
		current->setScaleX(1.75);
		current->setScaleY(2.5);
		current->setAnchorPoint(cocos2d::Vec2(0, 0));
		current->setPosition(
			sprite->getPosition().x + ((sprite->getContentSize().width * 1.5) * percentagePosition),
			fireRateLabel->getPositionY()
		);
		addChild(current);
		percentagePosition += 0.09;
	}
}

void TurretStatsDisplay::setupRangeDisplay()
{
	std::string rangeTxt = "RNG";
	auto rangeLabel = Label::createWithSystemFont(rangeTxt.c_str(), "Arial", 18);
	rangeLabel->setAnchorPoint(cocos2d::Vec2(0, 0));
	rangeLabel->setPosition(
		sprite->getPosition().x + ((sprite->getContentSize().width * 1.5) * 0.07),
		sprite->getPosition().y + ((sprite->getContentSize().height * 1.5) * 0.05)
	);
	rangeLabel->setColor(cocos2d::Color3B::GRAY);
	addChild(rangeLabel);

	int bars = calculateDisplayBarAmount(rangeTxt);
	float percentagePosition = 0.48;

	for (int i = 0; i < bars; i++) {
		Sprite *current = Sprite::create(TURRET_STAT_CURRENT);
		current->setScaleX(1.75);
		current->setScaleY(2.5);
		current->setAnchorPoint(cocos2d::Vec2(0, 0));
		current->setPosition(
			sprite->getPosition().x + ((sprite->getContentSize().width * 1.5) * percentagePosition),
			rangeLabel->getPositionY()
		);
		addChild(current);
		percentagePosition += 0.09;
	}
}

int TurretStatsDisplay::calculateDisplayBarAmount(std::string type)
{
	int bars = 0;
	if (type == "DAM") {
		if (info->damage >= 35)
			bars = 5;
		else if (info->damage >= 25)
			bars = 4;
		else if (info->damage >= 20)
			bars = 3;
		else if (info->damage >= 15)
			bars = 2;
		else
			bars = 1;
	}
	else if(type == "RTE") {
		if (info->cooldown >= 3.0)
			bars = 1;
		else if (info->cooldown >= 2.0)
			bars = 2;
		else if (info->cooldown >= 1.75)
			bars = 3;
		else if (info->cooldown >= 1.5)
			bars = 4;
		else
			bars = 5;
	}
	else {
		if (info->range >= 90)
			bars = 5;
		else if (info->range >= 76)
			bars = 4;
		else if (info->range >= 67)
			bars = 3;
		else if (info->range >= 60)
			bars = 2;
		else
			bars = 1;
	}

	return bars;
}

void TurretStatsDisplay::showCost()
{
	costLabel->setVisible(true);
}

void TurretStatsDisplay::hideCost()
{
	costLabel->setVisible(false);
}

TurretStatsDisplay::TurretStatsDisplay() :
	info(nullptr),
	game(nullptr),
	sprite(nullptr)
{
}


TurretStatsDisplay::~TurretStatsDisplay()
{
}

TurretStatsDisplay * TurretStatsDisplay::nodeWithTheGame(TowerDefence * game_, TurretInfo * turretInfo, cocos2d::Vec2 position)
{
	return initWithTheGame(game_, turretInfo, position);
}

void TurretStatsDisplay::draw(cocos2d::Renderer * renderer, const cocos2d::Mat4 & transform, uint32_t flags)
{
	_customCommand.init(_globalZOrder + 10);
	_customCommand.func = CC_CALLBACK_0(TurretStatsDisplay::onDraw, this, transform, flags);
	renderer->addCommand(&_customCommand);
}

void TurretStatsDisplay::onDraw(const cocos2d::kmMat4 & transform, uint32_t flags)
{

}

void TurretStatsDisplay::setPosition(const cocos2d::Vec2 & position)
{
	sprite->setPosition(position);
}

const cocos2d::Vec2 & TurretStatsDisplay::getPosition() const
{
	return sprite->getPosition();
}

cocos2d::Rect TurretStatsDisplay::getBoundingBox() const
{
	return sprite->getBoundingBox();
}

void TurretStatsDisplay::changeTurret(TurretInfo * turretInfo)
{
	if (info)
		delete info;

	info = turretInfo;
	setupDisplay();
}

void TurretStatsDisplay::show()
{
	sprite->setVisible(true);
}

void TurretStatsDisplay::hide()
{
	sprite->setVisible(false);
}
