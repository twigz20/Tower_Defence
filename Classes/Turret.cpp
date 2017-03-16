#include "Turret.h"
#include "TowerDefenceScene.h"
#include "Creep.h"
#include "CreepManager.h"
#include "levelManager.h"
#include "CGCircle.h"
#include "TurretStatsDisplay.h"
#define TURRET_RANGE_INDICATOR 301
#define BULLET_TAG 303

using namespace cocos2d;
using namespace ui;

Turret::Turret(TowerDefence* game_, TurretInfo turretInfo, bool isStarterTurret) :
	game(game_),
	info(turretInfo),
	range(nullptr),
	displayRange(false),
	target(nullptr),
	starterTurret(isStarterTurret),
	splashDamageRange(nullptr),
	isShooting(false),
	tsd(nullptr),
	active(false),
	sold(false)
{
	if (init()) {
		setName(info.name);
		setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));

		sprite = Sprite::create(info.image);
		addChild(sprite);

		tsd = new TurretStatsDisplay(game,
			info,
			cocos2d::Vec2(
				Director::getInstance()->getVisibleSize().width * 0.77,
				Director::getInstance()->getVisibleSize().height * 0.10)
		);
		tsd->setTag(TURRET_STATS_TAG);
		game->addChild(tsd, 0);
		hideTurretStats();

		addRangeIndicator();
		createHelperButtons();
	}
}

Turret::Turret(const Turret & other) :
	range(nullptr),
	displayRange(false),
	target(nullptr),
	splashDamageRange(nullptr),
	isShooting(false),
	tsd(nullptr),
	info(info),
	sold(false)
{
	*this = other;
}

Turret& Turret::operator=(const Turret& other) {
	if (this != &other) {
		info = other.info;
		game = other.game;
		sold = other.sold;
		splashDamageRange = other.splashDamageRange;
		displayRange = other.displayRange;
		starterTurret = other.starterTurret;
		isShooting = other.isShooting;
		sprite = Sprite::createWithTexture(other.sprite->getTexture());
		sprite->setScale(other.sprite->getScale());
		sprite->setRotation(other.sprite->getRotation());
		sprite->setContentSize(other.sprite->getContentSize());
		addRangeIndicator();
		setPosition(other.sprite->getPosition());
		addChild(sprite, 0);
		tsd = new TurretStatsDisplay(game, info, getPosition());
		tsd->hide();
		game->addChild(tsd, 0);
		hideTurretStats();
		createHelperButtons();
	}
	return *this;
}

Turret::Turret(Turret && other) :
	range(nullptr),
	displayRange(false),
	target(nullptr),
	splashDamageRange(nullptr),
	isShooting(false),
	tsd(nullptr),
	info(info),
	sold(false)
{
	*this = other;
}

Turret& Turret::operator=(Turret&& other) {
	if (this != &other) {
		info = other.info;
		game = other.game;
		sold = other.sold;
		splashDamageRange = other.splashDamageRange;
		displayRange = other.displayRange;
		starterTurret = other.starterTurret;
		isShooting = other.isShooting;
		sprite = Sprite::createWithTexture(other.sprite->getTexture());
		sprite->setScale(other.sprite->getScaleX(), other.sprite->getScaleY());
		sprite->setRotation(other.sprite->getRotation());
		sprite->setContentSize(other.sprite->getContentSize());
		addRangeIndicator();
		setPosition(other.sprite->getPosition());
		addChild(sprite, 0);
		tsd = new TurretStatsDisplay(game,info,getPosition());
		game->addChild(tsd, 0);
		hideTurretStats();
		createHelperButtons();

		other.range = nullptr;
		other.tsd = nullptr;
		other.splashDamageRange = nullptr;
	}
	return *this;
}

Turret::~Turret()
{
	if (splashDamageRange)
		delete splashDamageRange;
	if(target)
		target = nullptr;
}

void Turret::addRangeIndicator(cocos2d::Color4F color)
{
	range = cocos2d::DrawNode::create();
	range->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
	range->drawDot(getPosition(), info.range, cocos2d::Color4F(color));
	range->setTag(TURRET_RANGE_INDICATOR);
	addChild(range, -1);
	hideRange();
}

void Turret::update(float deltaTime)
{
	if (!starterTurret && getActive()) {
		if (target && !target->isDead()) {
			rotateToTarget();
			if (!collidesWithCreep(target->getBoundingBox()))
				lostSightOfEnemy();
		}
		else {
			getNextTarget();
		}
	}
}

void Turret::setAsNormalTurret()
{
	starterTurret = false;
}

void Turret::setRangeColor(cocos2d::Color4F color)
{
	removeChildByTag(TURRET_RANGE_INDICATOR);
	addRangeIndicator(color);
}

TurretInfo& Turret::getTurretInfo()
{
	return info;
}

void Turret::showRange()
{
	getChildByTag(TURRET_RANGE_INDICATOR)->setVisible(true);
}

void Turret::hideRange()
{
	getChildByTag(TURRET_RANGE_INDICATOR)->setVisible(false);
}

void Turret::setPosition(const cocos2d::Vec2 & position)
{
	sprite->setPosition(position);
	if(range)
		range->setPosition(getPosition());

	sellButton->setPosition(cocos2d::Vec2(
		getPosition().x - 40,
		getPosition().y - 40)
	);

	upgradeButton->setPosition(cocos2d::Vec2(
		getPosition().x + 40,
		getPosition().y - 40)
	);

	helpButton->setPosition(cocos2d::Vec2(
		getPosition().x,
		getPosition().y + 40)
	);
}

void Turret::upgrade()
{
	info.levelUp();
	tsd->setupDisplay(getPosition());

	std::string upgradeImage = info.id;
	upgradeImage.append("-");
	upgradeImage.append(tostr((int)info.level_).c_str());
	upgradeImage.append(".png");
	sprite->setTexture("Graphics/tower-defense-turrets/" + upgradeImage);
}

void Turret::showTurretStats(cocos2d::Vec2 position)
{
	if (tsd) {
		tsd->setupDisplay(position);
		tsd->setVisible(true);
	}
}

void Turret::hideTurretStats()
{
	if(tsd)
		tsd->setVisible(false);
}

void Turret::attackEnemy()
{
	if (!isShooting)
		shootWeapon(0.0f);

	this->schedule(schedule_selector(Turret::shootWeapon), info.cooldown);
}

void Turret::chosenEnemyForAttack(std::shared_ptr<Creep> enemy)
{
	target = nullptr;
	target = enemy;
	attackEnemy();
	target->getAttacked(this);
}

void Turret::shootWeapon(float dt)
{
	Sprite * bullet = Sprite::create(info.bulletInfo.image);
	bullet->setTag(BULLET_TAG);
	game->addChild(bullet);
	bullet->setPosition(sprite->getPosition());
	bullet->setRotation(sprite->getRotation());

	MoveTo *actionWithDuration = MoveTo::create(info.bulletInfo.speed, target->getPosition());
	CallFunc *actionWithTarget = CallFunc::create(CC_CALLBACK_0(Turret::damageEnemy, this));
	CallFunc *actionWithTarget2 = CallFunc::create(CC_CALLBACK_0(Turret::removeBullet, this, bullet));

	Sequence *actions = Sequence::create(actionWithDuration, actionWithTarget, actionWithTarget2, nullptr);
	bullet->runAction(actions);
	isShooting = true;
}

void Turret::removeBullet(Sprite *bullet)
{
	bullet->removeFromParentAndCleanup(true);
}

void Turret::damageEnemy()
{
	if(target && !target->isDead())
		target->getDamaged(info.bulletInfo);

	if(info.bulletInfo.hasSplashDamage)
		checkForSplashDamage();
}

void Turret::checkForSplashDamage()
{
	std::vector<std::shared_ptr<Creep>> creepsInPlay = game->getLevelManager()->getCreepManager()->getCreepsInPlay();
	for (int i = 0; i < creepsInPlay.size(); i++)
	{
		if (creepsInPlay[i] && !creepsInPlay[i]->isDead()) {
			if (collidesWithCreep(creepsInPlay[i]->getBoundingBox()))
			{
				chosenEnemyForAttack(creepsInPlay[i]);
				return;
			}
		}
	}
}

void Turret::getNextTarget()
{
	std::vector<std::shared_ptr<Creep>> creepsInPlay = game->getLevelManager()->getCreepManager()->getCreepsInPlay();
	for (int i = 0; i < creepsInPlay.size(); i++)
	{
		if (!creepsInPlay[i]->isDead()) {
			if (collidesWithCreep(creepsInPlay[i]->getBoundingBox()))
			{
				chosenEnemyForAttack(creepsInPlay[i]);
				break;
			}
		}
	}
}

void Turret::targetKilled()
{
	if (target)
		target = nullptr;

	unschedule(schedule_selector(Turret::shootWeapon));
	isShooting = false;
}

void Turret::lostSightOfEnemy()
{
	if (target) {
		target->gotLostSight(this);
		target = nullptr;
	}

	unschedule(schedule_selector(Turret::shootWeapon));
	isShooting = false;
}

void Turret::rotateToTarget()
{
	cocos2d::Vec2 normalized(target->getPosition().x - getPosition().x,
		target->getPosition().y - getPosition().y);
	normalized.normalize();
	sprite->setRotation(CC_RADIANS_TO_DEGREES(atan2(normalized.y, -normalized.x)) + 270);
}

bool Turret::collidesWithCreep(cocos2d::Rect creepRect)
{
	return creepRect.intersectsCircle(cocos2d::Vec2(range->getBoundingBox().getMidX(), range->getBoundingBox().getMidY()), info.range);
}

void Turret::createHelperButtons()
{
	sellButton = Button::create(SELL_FILE, "Graphics/UI/Buttons/Button_60.png", "Graphics/UI/Buttons/Button_61.png");
	sellButton->setVisible(false);
	sellButton->setPosition(Vec2::ZERO);
	sellButton->addTouchEventListener(CC_CALLBACK_2(Turret::sellCallback, this));
	sellButton->setScale(0.325);
	addChild(sellButton);

	upgradeButton = Button::create(UPGRADE_FILE, "Graphics/UI/Buttons/Button_32.png", "Graphics/UI/Buttons/Button_33.png");
	upgradeButton->setVisible(false);
	upgradeButton->setPosition(Vec2::ZERO);
	upgradeButton->addTouchEventListener(CC_CALLBACK_2(Turret::upgradeCallback, this));
	upgradeButton->setScale(0.325);
	addChild(upgradeButton);

	helpButton = Button::create(HELP_FILE, "Graphics/UI/Buttons/Button_28.png", "Graphics/UI/Buttons/Button_29.png");
	helpButton->setVisible(false);
	helpButton->setPosition(Vec2::ZERO);
	helpButton->addTouchEventListener(CC_CALLBACK_2(Turret::helpCallback, this));
	helpButton->setScale(0.325);
	addChild(helpButton);

	sellButton->setPosition(cocos2d::Vec2(
		getPosition().x - 40,
		getPosition().y - 40)
	);

	upgradeButton->setPosition(cocos2d::Vec2(
		getPosition().x + 40,
		getPosition().y - 40)
	);

	helpButton->setPosition(cocos2d::Vec2(
		getPosition().x,
		getPosition().y + 40)
	);

	hideHelperButtons();
}

void Turret::sellCallback(cocos2d::Ref * pSender, cocos2d::ui::Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
	{
		game->increaseGold(
			info.cost
			- (info.cost * 0.33)
		);

		sold = true;

		lostSightOfEnemy();
		stopAllActions();
		unscheduleAllSelectors();

		hideHelperButtons();
	}
	break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

void Turret::upgradeCallback(cocos2d::Ref * pSender, cocos2d::ui::Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
	{
		if (game->getGold() >=
			(info.cost - (info.cost * 0.33))
			&& info.hasLevelUp())
		{
			game->decreaseGold(
				info.cost
				- (info.cost * 0.33)
			);

			upgrade();
			if (!info.hasLevelUp()) {
				upgradeButton->setEnabled(false);
			}
		}

		hideHelperButtons();
	}
	break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

void Turret::helpCallback(cocos2d::Ref * pSender, cocos2d::ui::Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
	{
		auto visibleSize = Director::getInstance()->getVisibleSize();
		cocos2d::Vec2 turretStatsPosition = cocos2d::Vec2(
			visibleSize.width * 0.78,
			visibleSize.height * 0.55);
		showTurretStats(turretStatsPosition);
		hideHelperButtons();
		hideRange();
	}
	break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

void Turret::showHelperButtons()
{
	sellButton->setVisible(true);
	upgradeButton->setVisible(true);
	helpButton->setVisible(true);
}

void Turret::hideHelperButtons()
{
	sellButton->setVisible(false);
	upgradeButton->setVisible(false);
	helpButton->setVisible(false);
}

bool Turret::isSold()
{
	return sold;
}
