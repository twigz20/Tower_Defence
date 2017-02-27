#include "Turret.h"
#include "TowerDefenceScene.h"
#include "Creep.h"
#include "levelManager.h"
#include "CGCircle.h"
#define TURRET_RANGE_INDICATOR 1000

using namespace cocos2d;
Turret::Turret(bool isStarterTurret) :
	info(nullptr),
	range(nullptr),
	displayRange(false),
	chosenCreep(nullptr),
	starterTurret(isStarterTurret),
	turretActive(false)
{
}

Turret::Turret(const Turret & other) :
	info(nullptr),
	range(nullptr),
	displayRange(false),
	chosenCreep(nullptr)
{
	info = other.info;
	game = other.game;
	starterTurret = false;
	turretActive = other.turretActive;
	range = other.range;
	displayRange = other.displayRange;
	nodeWithTheGame(game, info);
	setPosition(other.getPosition());
}

Turret::Turret(Turret && other) :
	info(nullptr),
	range(nullptr),
	displayRange(false),
	chosenCreep(nullptr)
{
	*this = std::move(other);
}

Turret& Turret::operator=(Turret&& other) {
	if (this != &other) {
		info = other.info;
		game = other.game;
		range = other.range;
		displayRange = other.displayRange;
		starterTurret = false;
		turretActive = other.turretActive;
		nodeWithTheGame(game, info);
		setPosition(other.getPosition());

		other.info = nullptr;
		other.game = nullptr;
		other.range = nullptr;
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

bool Turret::checkCollision(cocos2d::Rect rect)
{
	return rangeIndicator->isContainRect(rect);
}

Turret* Turret::initWithTheGame(TowerDefence * game_, TurretInfo *turretInfo)
{
	if (init()) {
		game = game_;

		info = turretInfo;
		setName(info->name);
		setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));

		sprite = Sprite::create(info->image);
		addChild(sprite);

		addRangeIndicator();

		scheduleUpdate();
	}

	return this;
}

Turret * Turret::nodeWithTheGame(TowerDefence * game_, TurretInfo *turretInfo)
{
	return initWithTheGame(game_, turretInfo);
}

void Turret::update(float deltaTime)
{
	if (turretActive) {
		if (!starterTurret) {
			if (chosenCreep) {

				if (chosenCreep->isDead()) {
					lostSightOfEnemy();
					return;
				}

				rotateToTarget();
				if (!checkCollision(chosenCreep->getBoundingBox()))
				{
					lostSightOfEnemy();
				}
			}
			else {
				std::vector<Creep*> creepsInPlay = game->getLevelManager()->getCreepManager()->getCreepsInPlay();
				for (Creep * creep : creepsInPlay)
				{
					if (!creep->isDead()) {
						if (checkCollision(creep->getBoundingBox()))
						{
							chosenEnemyForAttack(creep);
							break;
						}
					}
				}
			}
		}
	}
}

void Turret::draw(cocos2d::Renderer * renderer, const cocos2d::Mat4 & transform, uint32_t flags)
{
	_customCommand.init(_globalZOrder + 10);
	_customCommand.func = CC_CALLBACK_0(Turret::onDraw, this, transform, flags);
	renderer->addCommand(&_customCommand);
}

void Turret::onDraw(const cocos2d::kmMat4 & transform, uint32_t flags)
{
	/*if (displayRange) {
		ccDrawColor4B(0, 255, 0, 255);
		ccDrawCircle(sprite->getPosition(), info->range, 360, 60, false);
	}*/
}

void Turret::activateTurret()
{
	turretActive = true;
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

void Turret::attackEnemy()
{
	if (!chosenCreep->isDead())
		this->schedule(schedule_selector(Turret::shootWeapon), info->cooldown);
}

void Turret::chosenEnemyForAttack(Creep *enemy)
{
	chosenCreep = nullptr;
	chosenCreep = enemy;
	attackEnemy();
}

void Turret::shootWeapon(float dt)
{
	Sprite * bullet = Sprite::create(info->bullet);
	game->addChild(bullet);
	bullet->setPosition(sprite->getPosition());

	MoveTo *actionWithDuration = MoveTo::create(0.1, chosenCreep->getPosition());
	CallFunc *actionWithTarget = CallFunc::create(CC_CALLBACK_0(Turret::damageEnemy, this));
	CallFunc *actionWithTarget2 = CallFunc::create(CC_CALLBACK_0(Turret::removeBullet, this, bullet));

	Sequence *actions = Sequence::create(actionWithDuration, actionWithTarget, actionWithTarget2, nullptr);
	bullet->runAction(actions);
}

void Turret::removeBullet(Sprite *bullet)
{
	bullet->removeFromParentAndCleanup(true);
}

void Turret::damageEnemy()
{
	if(chosenCreep && !chosenCreep->isDead())
		chosenCreep->getDamaged(info->damage);
}

void Turret::targetKilled()
{
	if (chosenCreep)
		chosenCreep = nullptr;

	unschedule(schedule_selector(Turret::shootWeapon));
}

void Turret::lostSightOfEnemy()
{
	chosenCreep->gotLostSight(this);
	if (chosenCreep)
		chosenCreep = nullptr;

	unschedule(schedule_selector(Turret::shootWeapon));
}

void Turret::rotateToTarget()
{
	cocos2d::Vec2 normalized(chosenCreep->getPosition().x - getPosition().x,
		chosenCreep->getPosition().y - getPosition().y);
	normalized.normalize();
	sprite->setRotation(CC_RADIANS_TO_DEGREES(atan2(normalized.y, -normalized.x)) + 270);
}
