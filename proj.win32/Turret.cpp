#include "Turret.h"
#include "TowerDefenceScene.h"

Turret::Turret(const std::string & turret) :
	BaseObject(turret),
	turretRange(nullptr)
{
	
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
	if(turretRange)
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
	_layer->addChild(turretRange, 0);
}

void Turret::hideTurretRange()
{
	if(turretRange)
		turretRange->removeFromParentAndCleanup(true);
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

Turret::TurretRangeIndicator::TurretRangeIndicator(Turret *turret)
{
	turretRange = cocos2d::DrawNode::create();
	turretRange->drawDot(cocos2d::Vec2(
		turret->getObject()->getPosition().x + turret->getObject()->getContentSize().width / 2,
		turret->getObject()->getPosition().y + turret->getObject()->getContentSize().height / 2),
		55,
		cocos2d::Color4F(1, 1, 1, 0.5f)
	);
	turretRange->setTag(TOWER_RANGE_INDICATOR);
	turret->getObject()->addChild(turretRange, -1);
}

