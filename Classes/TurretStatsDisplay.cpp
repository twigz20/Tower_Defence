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

	float spriteScale = 0.25f;
	sprite = Sprite::create("Graphics/UI/Windows 2/Window2_04.png");
	sprite->setAnchorPoint(cocos2d::Vec2(0, 0.5f));
	sprite->setScale(spriteScale);
	sprite->setTag(DISPLAY_TAG);
	addChild(sprite);
	setPosition(position);

	auto nameLabel = Label::createWithTTF(info.name, "fonts/carbon phyber.ttf", 60);
	nameLabel->setAnchorPoint(cocos2d::Vec2(0.5f, 0));
	nameLabel->setPosition(
		sprite->getContentSize().width / 2,
		sprite->getContentSize().height * 0.885
	);
	nameLabel->setColor(cocos2d::Color3B::GREEN);
	nameLabel->enableBold();
	sprite->addChild(nameLabel);

	std::stringstream ss;
	ss << "Level " << info.level_; 
	auto levelLabel = Label::createWithTTF(ss.str().c_str(), "fonts/carbon phyber.ttf", 60);
	levelLabel->setAnchorPoint(cocos2d::Vec2(0.5f, 0));
	levelLabel->setPosition(
		sprite->getContentSize().width / 2,
		sprite->getContentSize().height * 0.745
	);
	levelLabel->setColor(cocos2d::Color3B::GRAY);
	levelLabel->enableBold();
	sprite->addChild(levelLabel);
	ss.str(std::string());

	if (info.bulletInfo.hasStun) {
		Sprite *stunSprite = Sprite::create("Graphics/stun.png");
		stunSprite->setAnchorPoint(cocos2d::Vec2(0, 0));
		stunSprite->setPosition(
			sprite->getContentSize().width * 0.22,
			sprite->getContentSize().height * 0.65
		);
		stunSprite->setScale(4.f);
		sprite->addChild(stunSprite);

		char * buffer = new char[100];
		sprintf(buffer, "STUN:   %.1fs   %.f%%", info.bulletInfo.stunDuration, info.bulletInfo.stunChance * 100);
		auto stunLabel = Label::createWithTTF(buffer, "fonts/carbon bl.ttf", 55);
		stunLabel->setAnchorPoint(cocos2d::Vec2(0, 0));
		stunLabel->setPosition(
			sprite->getContentSize().width * 0.28,
			sprite->getContentSize().height * 0.645
		);
		stunLabel->setColor(cocos2d::Color3B::YELLOW);
		sprite->addChild(stunLabel);
		delete[] buffer;
	}

	if (info.bulletInfo.hasSlow) {
		Sprite *slowSprite = Sprite::create("Graphics/slow.png");
		slowSprite->setAnchorPoint(cocos2d::Vec2(0, 0));
		slowSprite->setPosition(
			sprite->getContentSize().width * 0.22,
			sprite->getContentSize().height * 0.55
		);
		slowSprite->setScale(4.f);
		sprite->addChild(slowSprite);

		char * buffer = new char[100];
		sprintf(buffer, "SLOW:   %.1fs   %.f%%", info.bulletInfo.slowDuration, info.bulletInfo.slowPercentage * 100);
		auto slowLabel = Label::createWithTTF(buffer, "fonts/carbon bl.ttf", 55);
		slowLabel->setAnchorPoint(cocos2d::Vec2(0, 0));
		slowLabel->setPosition(
			sprite->getContentSize().width * 0.28,
			sprite->getContentSize().height * 0.545
		);
		slowLabel->setColor(cocos2d::Color3B(30,144,255));
		sprite->addChild(slowLabel);
		delete[] buffer;
	}

	if (info.bulletInfo.hasBleed) {
		Sprite *bleedSprite = Sprite::create("Graphics/bleed.png");
		bleedSprite->setAnchorPoint(cocos2d::Vec2(0, 0));
		bleedSprite->setPosition(
			sprite->getContentSize().width * 0.22,
			sprite->getContentSize().height * 0.45
		);
		bleedSprite->setScale(4.f);
		sprite->addChild(bleedSprite);

		char * buffer = new char[100];
		sprintf(buffer, "BLEED:  %.1fs   %.f DPS", info.bulletInfo.bleedDuration, info.bulletInfo.bleedDps);
		auto bleedLabel = Label::createWithTTF(buffer, "fonts/carbon bl.ttf", 55);
		bleedLabel->setAnchorPoint(cocos2d::Vec2(0, 0));
		bleedLabel->setPosition(
			sprite->getContentSize().width * 0.28,
			sprite->getContentSize().height * 0.445
		);
		bleedLabel->setColor(cocos2d::Color3B::RED);
		sprite->addChild(bleedLabel);
		delete[] buffer;
	}

	if (info.bulletInfo.hasSplashDamage) {
		Sprite *splashSprite = Sprite::create("Graphics/area.png");
		splashSprite->setAnchorPoint(cocos2d::Vec2(0, 0));
		splashSprite->setPosition(
			sprite->getContentSize().width * 0.22,
			sprite->getContentSize().height * 0.65
		);
		splashSprite->setScale(4.f);
		sprite->addChild(splashSprite);

		char * buffer = new char[100];
		sprintf(buffer, "SPLASH:  %.f  %.f DMG", info.bulletInfo.splashRange, info.bulletInfo.damageFrom);
		auto splashLabel = Label::createWithTTF(buffer, "fonts/carbon bl.ttf", 52);
		splashLabel->setAnchorPoint(cocos2d::Vec2(0, 0));
		splashLabel->setPosition(
			sprite->getContentSize().width * 0.28,
			sprite->getContentSize().height * 0.645
		);
		splashLabel->setColor(cocos2d::Color3B::ORANGE);
		sprite->addChild(splashLabel);
		delete[] buffer;
	}

	setupDamageDisplay();
	setupFireRateDisplay();
	setupRangeDisplay();

	auto costContainer = Sprite::create("Graphics/UI/Window/Window_03.png");
	costContainer->setAnchorPoint(Vec2(0.5, 0.5));
	costContainer->setScale(1.5f);
	costContainer->setPosition(
		sprite->getContentSize().width / 2,
		0
	);
	sprite->addChild(costContainer);

	std::stringstream cost;
	cost << "Cost: " << info.cost;
	costLabel = Label::createWithTTF(cost.str(), "fonts/carbon phyber.ttf", 40);
	costLabel->setAnchorPoint(cocos2d::Vec2(0.5, 0.5));
	costLabel->setPosition(
		costContainer->getContentSize().width / 2,
		costContainer->getContentSize().height * 0.55
	);
	costLabel->setColor(cocos2d::Color3B::GREEN);
	costContainer->addChild(costLabel);
	cost.str(std::string());
}

void TurretStatsDisplay::clearStats()
{
	sprite->removeAllChildrenWithCleanup(true);
}

void TurretStatsDisplay::setupDamageDisplay()
{
	std::string damageTxt = "DAM";
	auto damageLabel = Label::createWithTTF(damageTxt.c_str(), "fonts/carbon bl.ttf", 60);
	damageLabel->setAnchorPoint(cocos2d::Vec2(0, 0));
	damageLabel->setPosition(
		sprite->getContentSize().width * 0.225,
		sprite->getContentSize().height * 0.35
	);
	damageLabel->setColor(cocos2d::Color3B::GRAY);
	sprite->addChild(damageLabel);

	int bars = calculateDisplayBarAmount(damageTxt);
	float percentagePosition = 0.45;

	for (int i = 0; i < bars; i++) {
		Sprite *current = Sprite::create(TURRET_STAT_CURRENT);
		current->setScaleX(8);
		current->setScaleY(8);
		current->setAnchorPoint(cocos2d::Vec2(0, 0));
		current->setPosition(
			sprite->getContentSize().width * percentagePosition,
			damageLabel->getPositionY()
		);
		sprite->addChild(current);
		percentagePosition += 0.065;
	}
}

void TurretStatsDisplay::setupFireRateDisplay()
{
	std::string fireRateTxt = "RTE";
	auto fireRateLabel = Label::createWithTTF(fireRateTxt.c_str(), "fonts/carbon bl.ttf", 60);
	fireRateLabel->setAnchorPoint(cocos2d::Vec2(0, 0));
	fireRateLabel->setPosition(
		sprite->getContentSize().width * 0.225,
		sprite->getContentSize().height * 0.25
	);
	fireRateLabel->setColor(cocos2d::Color3B::GRAY);
	sprite->addChild(fireRateLabel);

	int bars = calculateDisplayBarAmount(fireRateTxt);
	float percentagePosition = 0.45;

	for (int i = 0; i < bars; i++) {
		Sprite *current = Sprite::create(TURRET_STAT_CURRENT);
		current->setScaleX(8);
		current->setScaleY(8);
		current->setAnchorPoint(cocos2d::Vec2(0, 0));
		current->setPosition(
			sprite->getContentSize().width * percentagePosition,
			fireRateLabel->getPositionY()
		);
		sprite->addChild(current);
		percentagePosition += 0.065;
	}
}

void TurretStatsDisplay::setupRangeDisplay()
{
	std::string rangeTxt = "RNG";
	auto rangeLabel = Label::createWithTTF(rangeTxt.c_str(), "fonts/carbon bl.ttf", 60);
	rangeLabel->setAnchorPoint(cocos2d::Vec2(0, 0));
	rangeLabel->setPosition(
		sprite->getContentSize().width * 0.225,
		sprite->getContentSize().height * 0.15
	);
	rangeLabel->setColor(cocos2d::Color3B::GRAY);
	sprite->addChild(rangeLabel);

	int bars = calculateDisplayBarAmount(rangeTxt);
	float percentagePosition = 0.45;

	for (int i = 0; i < bars; i++) {
		Sprite *current = Sprite::create(TURRET_STAT_CURRENT);
		current->setScaleX(8);
		current->setScaleY(8);
		current->setAnchorPoint(cocos2d::Vec2(0, 0));
		current->setPosition(
			sprite->getContentSize().width * percentagePosition,
			rangeLabel->getPositionY()
		);
		sprite->addChild(current);
		percentagePosition += 0.065;
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
		else if (info.bulletInfo.damageFrom >= 11)
			bars = 2;
		else
			bars = 1;
	}
	else if(type == "RTE") {
		if (info.cooldown >= 2.0)
			bars = 1;
		else if (info.cooldown >= 1.75)
			bars = 2;
		else if (info.cooldown >= 1.30)
			bars = 3;
		else if (info.cooldown >= 1.00)
			bars = 4;
		else
			bars = 5;
	}
	else {
		if (info.range >= 110)
			bars = 5;
		else if (info.range >= 90)
			bars = 4;
		else if (info.range >= 75)
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
	game(game_),
	sprite(nullptr)
{
	if (init()) {
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
