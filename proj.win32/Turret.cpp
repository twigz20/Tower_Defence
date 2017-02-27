#include "Turret.h"
#include "TowerDefenceScene.h"
#include "Creep.h"
#include <math.h>

using namespace cocos2d;

Turret::Turret(const std::string & turret_) :
	BaseObject(turret_),
	rangeIndicator(nullptr),
	target(nullptr)
{
	addRangeIndicator();
}

Turret::Turret(const Turret & other) :
	BaseObject(other.info.image),
	rangeIndicator(nullptr),
	target(nullptr)
{
	stats = other.stats;
	info = other.info;
	_layer = other._layer;
	bulletInfo = other.bulletInfo;
	levelManager = other.levelManager;
	addRangeIndicator();
	setPosition(other.getPosition());
}

Turret::Turret(Turret && other) :
	BaseObject(other.info.image),
	rangeIndicator(nullptr),
	target(nullptr)
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
		bulletInfo = other.bulletInfo;

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
	for (auto It = m_Bullets.begin(); It != m_Bullets.end(); ++It)
	{
		(*It)->update(deltaTime);
		if ((*It)->isDead())
		{
			delete *It;
			It = m_Bullets.erase(It);
			if (It == m_Bullets.end())
			{
				break;
			}
		}
	}
}

void Turret::rotateToTarget(Creep* target_)
{
	if(!hasTarget())
		setTarget(target_);

	if (!checkCollision(target->getObject()->getBoundingBox())) {
		delete target;
		target = nullptr;
		return;
	}

	cocos2d::Point target(target->getPosition());
	
	// Calculation
	Vec2 difference = getPosition()- target;
	float rotationRadians = ccpToAngle(difference);
	float rotationDegrees = -CC_RADIANS_TO_DEGREES(rotationRadians);
	rotationDegrees += 270.0f; // Because Image is Facing Up

	object->setRotation(rotationDegrees);

	
	//m_Bullets.pop_back();
	/*Vec2 normalized = ccpNormalize(
		ccp(
			target.x - getPositionX(),
			target.y - getPositionY()
		));

	/*float angle = atan2(getPositionY() - target.y, getPositionX() - target.x);
	angle = angle * (180 / M_PI);

	// The following if statement is optional and converts our angle from being
	// -180 to +180 degrees to 0-360 degrees.  It is completely optional
	if (angle < 0)
	{
		angle = 360 - (-angle);
	}

	//textOut.text = "X:" + stage.mouseX + " Y:" + stage.mouseY + " angle:" + Math.round(angle);

	// Atan2 results have 0 degrees point down the positive X axis, while our image is pointed up.
	// Therefore we simply add 90 degrees to the rotation to orient our image
	// If 0 degrees is to the right on your image, you do not need to add 90*/
	//object->setRotation(CC_RADIANS_TO_DEGREES(atan2(normalized.y,-normalized.x))+90);

	/*Vec2 EnemyPosition(target);
	Vec2 direction(target - getPosition());
	ccpNormalize(direction);

	int adjacent = getPosition().x - EnemyPosition.x;
	int opposite = EnemyPosition.y - getPosition().y;

	if (opposite == 0) {
		opposite++;
	}
	if (adjacent == 0) {
		adjacent++;
	}

	float angle = atan(adjacent / opposite) * 180 / M_PI;
	if (opposite > 0)
		angle += 180;

	object->setRotation(angle);*/

	// animation
	//CCRotateBy * turnBy = [CCRotateBy actionWithDuration : 0.5f angle : rotateByDegrees];
	//CCEaseIn * ease = [CCEaseIn actionWithAction : turnBy rate : 4];
	//[sourceSprite runAction : ease];
}

void Turret::shootTarget(Creep * target)
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

cocos2d::DrawNode * Turret::getTurretRange()
{
	return rangeIndicator->getTurretRange();
}

void Turret::addCircle()
{
	circle = new CGCircle(
		stats.range,
		getPosition()
	);
}

bool Turret::checkCollision(cocos2d::Rect rect)
{
	return circle->isContainRect(rect);
}

void Turret::setTarget(Creep * creep)
{
	target = new Creep(*creep);
}

bool Turret::hasTarget()
{
	return target != nullptr;
}

void Turret::removeTarget()
{
	//target->getObject()->removeFromParent();
	if (target) {
		delete target;
		target = nullptr;
	}
}

BulletInfo & Turret::getBulletInfo()
{
	return bulletInfo;
}

Turret::TurretRangeIndicator::TurretRangeIndicator(Turret *turret_) :
	turret(turret_)
{
	turretRange = cocos2d::DrawNode::create();
	turretRange->drawDot(cocos2d::Vec2(
		turret->getPosition().x + turret->getObject()->getContentSize().width / 2,
		turret->getPosition().y + turret->getObject()->getContentSize().height / 2),
		turret->getTurretStats().range,
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

cocos2d::DrawNode * Turret::TurretRangeIndicator::getTurretRange()
{
	return turretRange;
}

