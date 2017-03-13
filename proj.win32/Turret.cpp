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
	active(false)
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
	}
}

Turret::Turret(const Turret & other) :
	range(nullptr),
	displayRange(false),
	target(nullptr),
	splashDamageRange(nullptr),
	isShooting(false),
	tsd(nullptr),
	info(info)
{
	*this = other;
}

Turret& Turret::operator=(const Turret& other) {
	if (this != &other) {
		info = other.info;
		game = other.game;
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
	info(info)
{
	*this = std::move(other);
}

Turret& Turret::operator=(Turret&& other) {
	if (this != &other) {
		info = other.info;
		game = other.game;
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

		other.range = nullptr;
		other.tsd = nullptr;
		other.splashDamageRange = nullptr;
	}
	return std::move(*this);
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
