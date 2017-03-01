#include "Turret.h"
#include "TowerDefenceScene.h"
#include "Creep.h"
#include "levelManager.h"
#include "CGCircle.h"
#define TURRET_RANGE_INDICATOR 301


using namespace cocos2d;

Turret::Turret(TowerDefence* game_, TurretInfo *turretInfo, bool isStarterTurret) :
	game(game_),
	info(turretInfo),
	range(nullptr),
	displayRange(false),
	chosenCreep(nullptr),
	starterTurret(isStarterTurret),
	splashDamageRange(nullptr),
	isShooting(false)
{
	if (init()) {
		setName(info->name);
		setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));

		sprite = Sprite::create(info->image);
		addChild(sprite);

		addRangeIndicator();

		if(!isStarterTurret)
			scheduleUpdate();
	}
}

Turret::Turret(const Turret & other) :
	info(nullptr),
	range(nullptr),
	displayRange(false),
	chosenCreep(nullptr),
	splashDamageRange(nullptr),
	rangeIndicator(nullptr),
	isShooting(false)
{
	*this = other;
}

Turret& Turret::operator=(const Turret& other) {
	if (this != &other) {
		info = other.info;
		game = other.game;
		range = other.range;
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
		addChild(sprite);

		if (!starterTurret)
			scheduleUpdate();
	}
	return *this;
}

Turret::Turret(Turret && other) :
	info(nullptr),
	range(nullptr),
	displayRange(false),
	chosenCreep(nullptr),
	splashDamageRange(nullptr),
	isShooting(false)
{
	*this = std::move(other);
}

Turret& Turret::operator=(Turret&& other) {
	if (this != &other) {
		info = other.info;
		game = other.game;
		range = other.range;
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
		addChild(sprite);

		if (!starterTurret)
			scheduleUpdate();

		other.info = nullptr;
		other.game = nullptr;
		other.range = nullptr;
		other.splashDamageRange = nullptr;
	}
	return std::move(*this);
}

Turret::~Turret()
{
}

void Turret::addRangeIndicator()
{
	range = cocos2d::DrawNode::create();
	range->drawCircle(
		cocos2d::Vec2(
			getPosition().x + sprite->getContentSize().width/2,
			getPosition().y + sprite->getContentSize().height/2
		),
		info->range, 
		360, 
		60, 
		false, 
		cocos2d::Color4F::GREEN
	);
	range->setTag(TURRET_RANGE_INDICATOR);
	sprite->addChild(range, -1);
	hideRange();

	rangeIndicator = new CGCircle(
		info->range,
		getPosition()
	);
}

void Turret::update(float deltaTime)
{
	if (chosenCreep && !chosenCreep->isDead()) {
		rotateToTarget();
		if (!game->checkCollision(rangeIndicator, chosenCreep->getBoundingBox()))
			lostSightOfEnemy();
	}
	else {
		getNextTarget();
	}
}

void Turret::setAsNormalTurret()
{
	starterTurret = false;
}

TurretInfo* Turret::getTurretInfo()
{
	return info;
}

void Turret::showRange()
{
	sprite->getChildByTag(TURRET_RANGE_INDICATOR)->setVisible(true);
}

void Turret::hideRange()
{
	sprite->getChildByTag(TURRET_RANGE_INDICATOR)->setVisible(false);
}

void Turret::setPosition(const cocos2d::Vec2 & position)
{
	sprite->setPosition(position);
	if(rangeIndicator)
		rangeIndicator->setPosition(getPosition());
}

const cocos2d::Vec2& Turret::getPosition() const
{
	return sprite->getPosition();
}

cocos2d::Rect Turret::getBoundingBox() const
{
	return sprite->getBoundingBox();
}

const cocos2d::Size & Turret::getContentSize() const
{
	return sprite->getContentSize();
}

void Turret::attackEnemy()
{
	if (!isShooting)
		shootWeapon(0.0);

	this->schedule(schedule_selector(Turret::shootWeapon), info->cooldown, 0, 0);
}

void Turret::chosenEnemyForAttack(Creep *enemy)
{
	chosenCreep = nullptr;
	chosenCreep = enemy;
	attackEnemy();
	chosenCreep->getAttacked(this);
}

void Turret::shootWeapon(float dt)
{
	Sprite * bullet = Sprite::create(info->bulletInfo.image);
	game->addChild(bullet);
	bullet->setPosition(sprite->getPosition());

	MoveTo *actionWithDuration = MoveTo::create(0.1, chosenCreep->getPosition());
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
	if(chosenCreep)
		chosenCreep->getDamaged(info->bulletInfo);

	checkForSplashDamage();
}

void Turret::checkForSplashDamage()
{
	std::vector<Creep*> creepsInPlay = game->getLevelManager()->getCreepManager()->getCreepsInPlay();
	for (Creep * creep : creepsInPlay)
	{
		if (creep && !creep->isDead()) {
			if (game->checkCollision(rangeIndicator, creep->getBoundingBox()))
			{
				chosenEnemyForAttack(creep);
				return;
			}
		}
	}
}

void Turret::getNextTarget()
{
	std::vector<Creep*> creepsInPlay = game->getLevelManager()->getCreepManager()->getCreepsInPlay();
	for (Creep * creep : creepsInPlay)
	{
		if (!creep->isDead()) {
			if (game->checkCollision(rangeIndicator, creep->getBoundingBox()))
			{
				chosenEnemyForAttack(creep);
				break;
			}
		}
	}
}

void Turret::targetKilled()
{
	if (chosenCreep)
		chosenCreep = nullptr;

	unschedule(schedule_selector(Turret::shootWeapon));
	isShooting = false;
}

void Turret::lostSightOfEnemy()
{
	chosenCreep->gotLostSight(this);
	if (chosenCreep)
		chosenCreep = nullptr;

	unschedule(schedule_selector(Turret::shootWeapon));
	isShooting = false;
}

void Turret::rotateToTarget()
{
	cocos2d::Vec2 normalized(chosenCreep->getPosition().x - getPosition().x,
		chosenCreep->getPosition().y - getPosition().y);
	normalized.normalize();
	sprite->setRotation(CC_RADIANS_TO_DEGREES(atan2(normalized.y, -normalized.x)) + 270);
}
