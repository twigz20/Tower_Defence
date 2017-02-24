#include "Turret.h"
#include "TowerDefenceScene.h"

Turret::Turret(const std::string & turret) :
	BaseObject(turret),
	turretRange(nullptr)
{
	rangeIndicator = new TurretRangeIndicator(this);
	hideTurretRange();
}

Turret::Turret(const Turret & other) :
	BaseObject(other.getObject()),
	turretRange(nullptr)
{
	stats = other.stats;
	info = other.info;
	getObject()->setPosition(
		other.getObject()->getPositionX(),
		other.getObject()->getPositionY()
	);

	getObject()->addChild(other.getObject()->getChildByTag(TOWER_RANGE_INDICATOR), 0);
}

Turret& Turret::operator=(Turret&& other) {
	//*this = new Turret(other);
	if (this != &other) {
		stats = other.stats;
		info = other.info;
		_layer = other._layer;
		levelManager = other.levelManager;
		rangeIndicator = other.rangeIndicator;
		turretRange = other.turretRange;
		loadSprite(info.image);
		getObject()->addChild(other.getObject()->getChildByTag(TOWER_RANGE_INDICATOR), -1);
	}
	// this may be called once or twice
	// if called twice, 'other' is the just-moved-from V subobject
	return *this;
}

Turret::~Turret()
{
}

Turret_Stats & Turret::getTurretStats()
{
	return stats;
}

Turret_Info & Turret::getTurretInfo()
{
	return info;
}

void Turret::update(float deltaTime)
{
}

void Turret::rotateToTarget(cocos2d::Point target)
{
}

void Turret::showTurretRange()
{
	rangeIndicator->show();
	/*if(turretRange)
		turretRange->removeFromParentAndCleanup(true);

	turretRange = cocos2d::DrawNode::create();
	turretRange->drawDot(
		cocos2d::Vec2(
			getObject()->getPosition().x,
			getObject()->getPosition().y
		),
		stats.range,
		cocos2d::Color4F(0, 1, 0, 0.5f)
	);

	turretRange->setTag(TOWER_RANGE_INDICATOR);
	_layer->addChild(turretRange, 0);*/
}

void Turret::hideTurretRange()
{
	rangeIndicator->hide();
	/*if (rangeIndicator)
		getObject()->removeChildByTag(TOWER_RANGE_INDICATOR);
	if(turretRange)
		turretRange->removeFromParentAndCleanup(true);*/
}

void Turret::setScene(TowerDefence * scene)
{
	_layer = scene;
}

void Turret::setPosition(float x, float y)
{
	BaseObject::setPosition(x, y);

	if (turretRange) {
		turretRange->setPosition(-x,-y);
	}
}

void Turret::resetTurret()
{
	rangeIndicator->resetTurret(this);
}

Turret::TurretRangeIndicator::TurretRangeIndicator(Turret *turret2) :
	turret(turret2)
{
	turretRange = cocos2d::DrawNode::create();
	turretRange->drawDot(cocos2d::Vec2(
		turret->getObject()->getPosition().x + turret->getObject()->getContentSize().width / 2,
		turret->getObject()->getPosition().y + turret->getObject()->getContentSize().height / 2),
		55,
		cocos2d::Color4F(1, 1, 1, 0.5f)
	);
	turretRange->setTag(TOWER_RANGE_INDICATOR);
}

void Turret::TurretRangeIndicator::resetTurret(Turret * t)
{
	this->turret = t;

	turretRange = cocos2d::DrawNode::create();
	turretRange->drawDot(cocos2d::Vec2(
		turret->getObject()->getPosition().x + turret->getObject()->getContentSize().width / 2,
		turret->getObject()->getPosition().y + turret->getObject()->getContentSize().height / 2),
		55,
		cocos2d::Color4F(1, 1, 1, 0.5f)
	);
	turretRange->setTag(TOWER_RANGE_INDICATOR);
	//t->getObject()->addChild(t->getObject()->getChildByTag(TOWER_RANGE_INDICATOR), 0);
}

void Turret::TurretRangeIndicator::show()
{
	turret->getObject()->addChild(turretRange, -1);
}

void Turret::TurretRangeIndicator::hide()
{
	turret->getObject()->removeChildByTag(TOWER_RANGE_INDICATOR);
}

