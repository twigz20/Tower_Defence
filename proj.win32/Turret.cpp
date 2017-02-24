#include "Turret.h"
#include "TowerDefenceScene.h"

Turret::Turret(const std::string & turret_) :
	BaseObject(turret_),
	rangeIndicator(nullptr)
{
	addRangeIndicator();
}

Turret::Turret(const Turret & other) :
	BaseObject(other.info.image),
	rangeIndicator(nullptr)
{
	stats = other.stats;
	info = other.info;
	_layer = other._layer;
	levelManager = other.levelManager;
	addRangeIndicator();
	setPosition(other.getPosition());
}

Turret::Turret(Turret && other) :
	BaseObject(other.info.image),
	rangeIndicator(nullptr)
{
	*this = std::move(other);
}

Turret& Turret::operator=(Turret&& other) {
	if (this != &other) {
		stats = other.stats;
		info = other.info;
		_layer = other._layer;
		levelManager = other.levelManager;
		loadSprite(info.image);
		addRangeIndicator();
		setPosition(other.getPosition());

		other.stats.cooldown = 0;
		other.stats.cost = 0;
		other.stats.damage = 0;
		other.stats.range = 0;
		other.info.bullet = std::string();
		other.info.image = std::string();
		other.info.name = std::string();
		other.turretRange = nullptr;
		other.object = nullptr;
		other.rangeIndicator = nullptr;
		other._layer = nullptr;
		other.levelManager = nullptr;
	}
	return std::move(*this);
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
}

void Turret::hideTurretRange()
{
	rangeIndicator->hide();
}

void Turret::setScene(TowerDefence * scene)
{
	_layer = scene;
}

void Turret::addRangeIndicator()
{
	rangeIndicator = new TurretRangeIndicator(this);
}

Turret::TurretRangeIndicator::TurretRangeIndicator(Turret *turret_) :
	turret(turret_)
{
	turretRange = cocos2d::DrawNode::create();
	turretRange->drawDot(cocos2d::Vec2(
		turret->getPosition().x + turret->getObject()->getContentSize().width / 2,
		turret->getPosition().y + turret->getObject()->getContentSize().height / 2),
		55,
		cocos2d::Color4F(0, 1, 0, 0.5f)
	);
	turretRange->setTag(TURRET_RANGE_INDICATOR);
	turret->getObject()->addChild(turretRange, -1);

	hide();
}

void Turret::TurretRangeIndicator::show()
{
	turret->getObject()->getChildByTag(TURRET_RANGE_INDICATOR)->setVisible(true);
}

void Turret::TurretRangeIndicator::hide()
{
	turret->getObject()->getChildByTag(TURRET_RANGE_INDICATOR)->setVisible(false);
}

