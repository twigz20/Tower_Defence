#include "TurretStatsDisplay.h"
#include "TurretInfo.h"
#include "TowerDefenceScene.h"
#include "Utils.h"
#define DISPLAY_TAG 150

using namespace cocos2d;

void TurretStatsDisplay::setupDisplay(cocos2d::Vec2 position)
{
	if (getChildrenCount() > 0)
		removeAllChildrenWithCleanup(true);

	sprite = Sprite::create(TURRET_STATS_BACKGROUND);
	sprite->setAnchorPoint(cocos2d::Vec2(0, 0));
	sprite->setScaleX(2.0);
	sprite->setScaleY(2.0);
	sprite->setTag(DISPLAY_TAG);
	addChild(sprite);
	setPosition(position);

	auto nameLabel = Label::createWithSystemFont(info.name, "Arial", 16);
	nameLabel->setAnchorPoint(cocos2d::Vec2(0, 0));
	nameLabel->setPosition(
		sprite->getPosition().x + ((sprite->getContentSize().width * 2.0) * 0.07),
		sprite->getPosition().y + ((sprite->getContentSize().height * 2.0) * 0.84)
	);
	nameLabel->setColor(cocos2d::Color3B::GREEN);
	nameLabel->enableBold();
	addChild(nameLabel);

	std::stringstream ss;
	ss << "Lvl " << info.level;
	auto levelLabel = Label::createWithSystemFont(ss.str().c_str(), "Arial", 16);
	levelLabel->setAnchorPoint(cocos2d::Vec2(0, 0));
	levelLabel->setPosition(
		sprite->getPosition().x + ((sprite->getContentSize().width * 2.0) * 0.65),
		sprite->getPosition().y + ((sprite->getContentSize().height * 2.0) * 0.84)
	);
	levelLabel->setColor(cocos2d::Color3B::GRAY);
	levelLabel->enableBold();
	addChild(levelLabel);
	ss.str(std::string());

	if (info.bulletInfo.hasStun) {
		Sprite *stunSprite = Sprite::create("Graphics/stun.png");
		stunSprite->setAnchorPoint(cocos2d::Vec2(0, 0));
		stunSprite->setPosition(
			sprite->getPosition().x + ((sprite->getContentSize().width * 2.0) * 0.07),
			sprite->getPosition().y + ((sprite->getContentSize().height * 2.0) * 0.76)
		);
		addChild(stunSprite);

		char * buffer = new char[100];
		sprintf(buffer, "STUN:   %.1fs   %.f%%", info.bulletInfo.stunDuration, info.bulletInfo.stunChance * 100);
		auto stunLabel = Label::createWithSystemFont(buffer, "Arial", 12);
		stunLabel->setAnchorPoint(cocos2d::Vec2(0, 0));
		stunLabel->setPosition(
			sprite->getPosition().x + ((sprite->getContentSize().width * 2.0) * 0.15),
			sprite->getPosition().y + ((sprite->getContentSize().height * 2.0) * 0.74)
		);
		stunLabel->setColor(cocos2d::Color3B::YELLOW);
		addChild(stunLabel);
	}

	if (info.bulletInfo.hasSlow) {
		Sprite *slowSprite = Sprite::create("Graphics/slow.png");
		slowSprite->setAnchorPoint(cocos2d::Vec2(0, 0));
		slowSprite->setPosition(
			sprite->getPosition().x + ((sprite->getContentSize().width * 2.0) * 0.07),
			sprite->getPosition().y + ((sprite->getContentSize().height * 2.0) * 0.67)
		);
		addChild(slowSprite);

		char * buffer = new char[100];
		sprintf(buffer, "SLOW:   %.1fs   %.f%%", info.bulletInfo.slowDuration, info.bulletInfo.slowPercentage * 100);
		auto slowLabel = Label::createWithSystemFont(buffer, "Arial", 12);
		slowLabel->setAnchorPoint(cocos2d::Vec2(0, 0));
		slowLabel->setPosition(
			sprite->getPosition().x + ((sprite->getContentSize().width * 2.0) * 0.15),
			sprite->getPosition().y + ((sprite->getContentSize().height * 2.0) * 0.65)
		);
		slowLabel->setColor(cocos2d::Color3B(30,144,255));
		addChild(slowLabel);
	}

	if (info.bulletInfo.hasBleed) {
		Sprite *bleedSprite = Sprite::create("Graphics/bleed.png");
		bleedSprite->setAnchorPoint(cocos2d::Vec2(0, 0));
		bleedSprite->setPosition(
			sprite->getPosition().x + ((sprite->getContentSize().width * 2.0) * 0.07),
			sprite->getPosition().y + ((sprite->getContentSize().height * 2.0) * 0.58)
		);
		addChild(bleedSprite);

		char * buffer = new char[100];
		sprintf(buffer, "BLEED:  %.1fs   %.f DPS", info.bulletInfo.bleedDuration, info.bulletInfo.bleedDps);
		auto bleedLabel = Label::createWithSystemFont(buffer, "Arial", 12);
		bleedLabel->setAnchorPoint(cocos2d::Vec2(0, 0));
		bleedLabel->setPosition(
			sprite->getPosition().x + ((sprite->getContentSize().width * 2.0) * 0.15),
			sprite->getPosition().y + ((sprite->getContentSize().height * 2.0) * 0.56)
		);
		bleedLabel->setColor(cocos2d::Color3B::RED);
		addChild(bleedLabel);
	}

	if (info.bulletInfo.hasSplashDamage) {
		Sprite *splashSprite = Sprite::create("Graphics/area.png");
		splashSprite->setAnchorPoint(cocos2d::Vec2(0, 0));
		splashSprite->setPosition(
			sprite->getPosition().x + ((sprite->getContentSize().width * 2.0) * 0.07),
			sprite->getPosition().y + ((sprite->getContentSize().height * 2.0) * 0.76)
		);
		addChild(splashSprite);

		char * buffer = new char[100];
		sprintf(buffer, "SPLASH:  %.f  %.f DMG", info.bulletInfo.splashRange, info.bulletInfo.damageFrom);
		auto splashLabel = Label::createWithSystemFont(buffer, "Arial", 12);
		splashLabel->setAnchorPoint(cocos2d::Vec2(0, 0));
		splashLabel->setPosition(
			sprite->getPosition().x + ((sprite->getContentSize().width * 2.0) * 0.15),
			sprite->getPosition().y + ((sprite->getContentSize().height * 2.0) * 0.74)
		);
		splashLabel->setColor(cocos2d::Color3B::ORANGE);
		addChild(splashLabel);
	}

	setupDamageDisplay();
	setupFireRateDisplay();
	setupRangeDisplay();

	std::stringstream cost;
	cost << "Cost: " << info.cost;
	costLabel = Label::createWithSystemFont(cost.str(), "Arial", 16);
	costLabel->setAnchorPoint(cocos2d::Vec2(0, 0));
	costLabel->setPosition(
		sprite->getPosition().x + ((sprite->getContentSize().width * 2.0) * 0.07),
		sprite->getPosition().y - ((sprite->getContentSize().width * 2.0) * 0.20)
	);
	costLabel->setColor(cocos2d::Color3B::GRAY);
	addChild(costLabel);
}

void TurretStatsDisplay::clearStats()
{
	sprite->removeAllChildrenWithCleanup(true);
}

void TurretStatsDisplay::setupDamageDisplay()
{
	std::string damageTxt = "DAM";
	auto damageLabel = Label::createWithSystemFont(damageTxt.c_str(), "Arial", 16);
	damageLabel->setAnchorPoint(cocos2d::Vec2(0, 0));
	damageLabel->setPosition(
		sprite->getPosition().x + ((sprite->getContentSize().width * 2.0) * 0.07),
		sprite->getPosition().y + ((sprite->getContentSize().height * 2.0) * 0.35)
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
			sprite->getPosition().x + ((sprite->getContentSize().width * 2.0) * percentagePosition),
			damageLabel->getPositionY()
		);
		addChild(current);
		percentagePosition += 0.09;
	}
}

void TurretStatsDisplay::setupFireRateDisplay()
{
	std::string fireRateTxt = "RTE";
	auto fireRateLabel = Label::createWithSystemFont(fireRateTxt.c_str(), "Arial", 16);
	fireRateLabel->setAnchorPoint(cocos2d::Vec2(0, 0));
	fireRateLabel->setPosition(
		sprite->getPosition().x + ((sprite->getContentSize().width * 2.0) * 0.07),
		sprite->getPosition().y + ((sprite->getContentSize().height * 2.0) * 0.20)
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
			sprite->getPosition().x + ((sprite->getContentSize().width * 2.0) * percentagePosition),
			fireRateLabel->getPositionY()
		);
		addChild(current);
		percentagePosition += 0.09;
	}
}

void TurretStatsDisplay::setupRangeDisplay()
{
	std::string rangeTxt = "RNG";
	auto rangeLabel = Label::createWithSystemFont(rangeTxt.c_str(), "Arial", 16);
	rangeLabel->setAnchorPoint(cocos2d::Vec2(0, 0));
	rangeLabel->setPosition(
		sprite->getPosition().x + ((sprite->getContentSize().width * 2.0) * 0.07),
		sprite->getPosition().y + ((sprite->getContentSize().height * 2.0) * 0.05)
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
			sprite->getPosition().x + ((sprite->getContentSize().width * 2.0) * percentagePosition),
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
		if (info.bulletInfo.damageFrom >= 35)
			bars = 5;
		else if (info.bulletInfo.damageFrom >= 25)
			bars = 4;
		else if (info.bulletInfo.damageFrom >= 20)
			bars = 3;
		else if (info.bulletInfo.damageFrom >= 15)
			bars = 2;
		else
			bars = 1;
	}
	else if(type == "RTE") {
		if (info.cooldown >= 3.0)
			bars = 1;
		else if (info.cooldown >= 1.75)
			bars = 2;
		else if (info.cooldown >= 1.39)
			bars = 3;
		else if (info.cooldown >= 1.35)
			bars = 4;
		else
			bars = 5;
	}
	else {
		if (info.range >= 90)
			bars = 5;
		else if (info.range >= 76)
			bars = 4;
		else if (info.range >= 67)
			bars = 3;
		else if (info.range >= 60)
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

TurretStatsDisplay::TurretStatsDisplay(TowerDefence* game_, TurretInfo &turretInfo, cocos2d::Vec2 position) :
	info(turretInfo),
	game(nullptr),
	sprite(nullptr)
{
	if (init()) {
		game = game_;
		info = turretInfo;
		setupDisplay(position);
	}
}


TurretStatsDisplay::~TurretStatsDisplay()
{
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

void TurretStatsDisplay::changeTurret(TurretInfo &turretInfo)
{
	info = turretInfo;
	setupDisplay(getPosition());
}

void TurretStatsDisplay::changePosition(cocos2d::Vec2 position)
{
	sprite->setPosition(position);
}

void TurretStatsDisplay::show()
{
	sprite->setVisible(true);
}

void TurretStatsDisplay::hide()
{
	sprite->setVisible(false);
}
