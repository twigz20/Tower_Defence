#include "Turret.h"
#include "TowerDefenceScene.h"
#include "Creep.h"
#include "CreepManager.h"
#include "levelManager.h"
#include "CGCircle.h"
#include "TurretStatsDisplay.h"
#define TURRET_RANGE_INDICATOR 301


using namespace cocos2d;

Turret::Turret(TowerDefence* game_, TurretInfo turretInfo, bool isStarterTurret) :
	game(game_),
	info(turretInfo),
	range(nullptr),
	displayRange(false),
	chosenCreep(nullptr),
	starterTurret(isStarterTurret),
	splashDamageRange(nullptr),
	isShooting(false),
	tsd(nullptr)
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

		if(!isStarterTurret)
			scheduleUpdate();
	}
}

Turret::Turret(const Turret & other) :
	range(nullptr),
	displayRange(false),
	chosenCreep(nullptr),
	splashDamageRange(nullptr),
	rangeIndicator(nullptr),
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
		addChild(sprite, 0);
		tsd = new TurretStatsDisplay(game, info, getPosition());
		tsd->hide();
		game->addChild(tsd, 0);
		hideTurretStats();

		if (!starterTurret)
			scheduleUpdate();
	}
	return *this;
}

Turret::Turret(Turret && other) :
	range(nullptr),
	displayRange(false),
	chosenCreep(nullptr),
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
		addChild(sprite, 0);
		tsd = new TurretStatsDisplay(game,info,getPosition());
		game->addChild(tsd, 0);
		hideTurretStats();

		if (!starterTurret)
			scheduleUpdate();

		other.game = nullptr;
		other.range = nullptr;
		other.tsd = nullptr;
		other.splashDamageRange = nullptr;
	}
	return std::move(*this);
}

Turret::~Turret()
{
	if (rangeIndicator)
		delete rangeIndicator;
	if (splashDamageRange)
		delete splashDamageRange;
}

void Turret::addRangeIndicator()
{
	range = cocos2d::DrawNode::create();
	range->drawCircle(
		cocos2d::Vec2(
			getPosition().x + sprite->getContentSize().width/2,
			getPosition().y + sprite->getContentSize().height/2
		),
		info.range, 
		360, 
		60, 
		false, 
		cocos2d::Color4F::GREEN
	);
	range->setTag(TURRET_RANGE_INDICATOR);
	sprite->addChild(range, 2);
	hideRange();

	rangeIndicator = new CGCircle(
		info.range,
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

TurretInfo& Turret::getTurretInfo()
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

template <typename T> std::string tostr(const T& t) { std::ostringstream os; os << t; return os.str(); }

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
		shootWeapon(0.0);
	else
		this->schedule(schedule_selector(Turret::shootWeapon), info.cooldown, 0, 0);
}

void Turret::chosenEnemyForAttack(std::shared_ptr<Creep> enemy)
{
	chosenCreep = nullptr;
	chosenCreep = enemy;
	attackEnemy();
	chosenCreep->getAttacked(this);
}

void Turret::shootWeapon(float dt)
{
	Sprite * bullet = Sprite::create(info.bulletInfo.image);
	game->addChild(bullet);
	bullet->setPosition(sprite->getPosition());

	MoveTo *actionWithDuration = MoveTo::create(info.bulletInfo.speed, chosenCreep->getPosition());
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
		chosenCreep->getDamaged(info.bulletInfo);

	checkForSplashDamage();
}

void Turret::checkForSplashDamage()
{
	std::vector<std::shared_ptr<Creep>> creepsInPlay = game->getLevelManager()->getCreepManager()->getCreepsInPlay();
	for (int i = 0; i < creepsInPlay.size(); i++)
	{
		if (creepsInPlay[i] && !creepsInPlay[i]->isDead()) {
			if (game->checkCollision(rangeIndicator, creepsInPlay[i]->getBoundingBox()))
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
			if (game->checkCollision(rangeIndicator, creepsInPlay[i]->getBoundingBox()))
			{
				chosenEnemyForAttack(creepsInPlay[i]);
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
	if (chosenCreep) {
		chosenCreep->gotLostSight(this);
		chosenCreep = nullptr;
	}

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
