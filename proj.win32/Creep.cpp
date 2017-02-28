#include "Creep.h"
#include "proj.win32\Utils.h"
#include "TowerDefenceScene.h"
#include "LevelManager.h"
#include "CreepInfo.h"
#include "CGCircle.h"

#define HEALTH_BAR_WIDTH 30
#define HEALTH_BAR_ORIGIN -10

using namespace cocos2d;

Creep::Creep() :
	startDelay(0.f),
	currentHP(0),
	dead(false),
	missionComplete(false),
	isSlowed(false),
	isStunned(false),
	isBleeding(false),
	stunDuration(0),
	slowDuration(0),
	bleedDuration(0),
	bleedDamage(0),
	slowPercentage(0),
	inSplashRange(false),
	inSpeedAuraRange(false),
	inHealAuraRange(false),
	speedIncreasePercentage(0)
{
	
}

Creep * Creep::initWithTheGame(TowerDefence * game_, CreepInfo * creepInfo)
{
	if (init()) {
		game = game_;

		info = creepInfo;
		currentHP = info->health;

		sprite = Sprite::create(info->image);
		addChild(sprite);

		if (info->hasSpeedAura()) {
			cocos2d::DrawNode *speedAuraRange = DrawNode::create();
			speedAuraRange->drawDot(
				cocos2d::Vec2(
					sprite->getPosition().x + sprite->getContentSize().width / 2,
					sprite->getPosition().y + sprite->getContentSize().height / 2
				),
				info->speedAura.area_, 
				cocos2d::Color4F(0, 1, 0, 0.5f));
			sprite->addChild(speedAuraRange, -1);
		}

		if (info->hasHealAura()) {
			cocos2d::DrawNode *healAuraRange = DrawNode::create();
			healAuraRange->drawDot(
				cocos2d::Vec2(
					sprite->getPosition().x + sprite->getContentSize().width / 2,
					sprite->getPosition().y + sprite->getContentSize().height / 2
				),
				info->healAura.area_,
				cocos2d::Color4F(0, 0, 1, 0.5f));
			sprite->addChild(healAuraRange, -1);
			healAuraTimer.Start();
			selfHealAuraTimer.Start();
		}

		Vector<SpriteFrame*> animFrames(6);
		char str[100] = { 0 };
		for (int i = 1; i <= 6; i++)
		{
			std::stringstream ss;
			ss << "Graphics/creep/" << info->name << "/" << i << ".png";
			auto frame = SpriteFrame::create(ss.str().c_str(), Rect(0, 00, 46, 46));
			animFrames.pushBack(frame);
			ss.clear();
		}
		Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.18f);
		RepeatForever *animateMovement = RepeatForever::create(Animate::create(animation));
		sprite->runAction(animateMovement);

		scheduleUpdate();
	}

	return this;
}

Creep::~Creep()
{
}

Creep * Creep::nodeWithTheGame(TowerDefence * game_, CreepInfo * creepInfo)
{
	return initWithTheGame(game_, creepInfo);
}

void Creep::setHealth(const int& health)
{
	info->health = health;
}

void Creep::setSpeed(const int& speed)
{
	info->speed = speed;
}

void Creep::setGold(const int& gold)
{
	info->gold = gold;
}

void Creep::setStartDelay(const float & startDelay)
{
	this->startDelay = startDelay;
}

const int Creep::getHealth() const
{
	return info->health;
}

const float Creep::getSpeed() const
{
	return info->speed;
}

const int Creep::getGold() const
{
	return info->gold;
}

const float Creep::getStartDelay() const
{
	return startDelay;
}

void Creep::moveToward(cocos2d::Point target)
{
	Point fromTileCoord = game->getLevelManager()->tileCoordForPosition(sprite->getPosition());
	Point toTileCoord = game->getLevelManager()->tileCoordForPosition(target);

	if (fromTileCoord == toTileCoord)
	{
		CCLOG("You're already there! :P");
		return;
	}

	if (!game->getLevelManager()->isValidTileCoord(toTileCoord) || game->getLevelManager()->isWallAtTileCoord(toTileCoord))
	{
		//SimpleAudioEngine::getInstance()->playEffect("hitWall.wav");
		return;
	}

	CCLOG("From: %f, %f", fromTileCoord.x, fromTileCoord.y);
	CCLOG("To: %f, %f", toTileCoord.x, toTileCoord.y);


	//bool pathFound = false;
	_spOpenSteps.clear();
	_spClosedSteps.clear();

	// First, add the cat box coordinates to the open list
	this->insertInOpenSteps(ShortestPathStep::createWithPosition(fromTileCoord));

	do
	{
		// Get the minimum value of the F steps
		// Because the sequence table, the first step is always the minimum F value
		ShortestPathStep *currentStep = _spOpenSteps.at(0);

		// Add the current step to the closed list
		_spClosedSteps.pushBack(currentStep);

		// It will be removed from open list
		// Note that, if you want to remove open from the list, care should be taken to the object's memory
		_spOpenSteps.erase(0);

		// If the current step is the target square coordinate, then completed
		if (currentStep->getPosition() == toTileCoord)
		{
			//pathFound = true;
			this->constructPathAndStartAnimationFromStep(currentStep);
			ShortestPathStep *tmpStep = currentStep;
			/*CCLOG("PATH FOUND :");
			do
			{
			CCLOG("%s", tmpStep->getDescription().c_str());
			tmpStep = tmpStep->getParent(); // Retrogression
			} while (tmpStep); // Until no one step*/

			_spOpenSteps.clear();
			_spClosedSteps.clear();
			break;
		}

		// Get the coordinates of the current step of the adjacent squares
		PointArray *adjSteps = game->getLevelManager()->walkableAdjacentTilesCoordForTileCoord(currentStep->getPosition());
		for (ssize_t i = 0; i <adjSteps->count(); ++i)
		{
			ShortestPathStep *step = ShortestPathStep::createWithPosition(adjSteps->getControlPointAtIndex(i));

			// Inspection procedures Is it right? Already in the closed list
			if (this->getStepIndex(_spClosedSteps, step) != -1)
			{
				continue;
			}

			// Calculated from the current step this step cost
			int moveCost = this->costToMoveFromStepToAdjacentStep(currentStep, step);

			// Check this step is already in the list of open
			ssize_t index = this->getStepIndex(_spOpenSteps, step);

			// Is not on the open list, add it
			if (index == -1)
			{
				// Set the current step as a step
				step->setParent(currentStep);

				// The G value is equivalent to the step value of G + from one step to the cost
				step->setGScore(currentStep->getGScore() + moveCost);

				// The value of H is then step into the mobile target square coordinate estimates
				step->setHScore(this->computeHScoreFromCoordToCoord(step->getPosition(), toTileCoord));

				// In order to add to the open list
				this->insertInOpenSteps(step);
			}
			else
			{
				// Get the old steps, its value has been calculated
				step = _spOpenSteps.at(index);

				// Check whether the G value is lower than the current step by step value
				if ((currentStep->getGScore() + moveCost) <step->getGScore())
				{
					// The G value is equivalent to the step value of G + from one step to the cost
					step->setGScore(currentStep->getGScore() + moveCost);

					// Because G value changes, the F value will change
					// So in order to keep the open list ordered, will need to take this step removed, re ordered into

					// Before the removal, need to maintain a reference
					step->retain();

					// It can safely remove, need not worry to be released
					_spOpenSteps.erase(index);

					// In order to insert again
					this->insertInOpenSteps(step);

					// Now you can release it, because the open list should hold it
					step->release();
				}
			}
		}
	} while (_spOpenSteps.size() > 0);

	//if (!pathFound)
	if (_shortestPath.empty())
	{
		//	SimpleAudioEngine::getInstance()->playEffect("hitWall.wav");
	}
}

void Creep::setPosition(const cocos2d::Vec2 & position)
{
	sprite->setPosition(position);
}

const cocos2d::Vec2 & Creep::getPosition() const
{
	return sprite->getPosition();
}

cocos2d::Rect Creep::getBoundingBox() const
{
	return sprite->getBoundingBox();
}

CreepInfo & Creep::getCreepInfo()
{
	return *info;
}

void Creep::update(float deltaTime)
{
	if (isSlowed) {
		if (slowTimer.GetTicks() >= slowDuration * 1000) {
			isSlowed = false;
			slowTimer.Stop();
		}
	}
	if (isStunned) {
		if (stunTimer.GetTicks() >= stunDuration * 1000) {
			isStunned = false;
			sprite->resume();
			stunTimer.Stop();
		}
	}
	if (isBleeding) {
		if (bleedDpsTimer.GetTicks() >= 1000) {
			currentHP -= bleedDamage;
			bleedDpsTimer.Reset();
		}
		if (bleedTimer.GetTicks() >= bleedDuration * 1000) {
			isBleeding = false;
			bleedTimer.Stop();
			bleedDpsTimer.Stop();
		}
	}

	if (info->hasSpeedAura()) {
		CGCircle *speedRadius = new CGCircle(
			info->speedAura.area_,
			getPosition()
		);
		std::vector<Creep*> creepsInPlay = game->getLevelManager()->getCreepManager()->getCreepsInPlay();
		for (Creep * creep : creepsInPlay)
		{
			if (creep != this && !creep->isDead()) {
				if (game->checkCollision(speedRadius, creep->getBoundingBox()))
				{
					creep->increaseSpeed(info->speedAura.percentage_);
				}
				else {
					inSpeedAuraRange = false;
				}
			}
		}
		delete speedRadius;
	}

	if (info->hasHealAura()) {
		if (selfHealAuraTimer.GetTicks() >= 1000) {
			getHealed(info->healAura.healPerSecond_);
			selfHealAuraTimer.Reset();
		}
		CGCircle *healRadius = new CGCircle(
			info->healAura.area_,
			getPosition()
		);
		std::vector<Creep*> creepsInPlay = game->getLevelManager()->getCreepManager()->getCreepsInPlay();
		for (Creep * creep : creepsInPlay)
		{
			if (creep != this && !creep->isDead()) {
				if (game->checkCollision(healRadius, creep->getBoundingBox()))
				{
					creep->setInHealAuraRange();
					if (healAuraTimer.GetTicks() >= 1000) {
						creep->getHealed(info->healAura.healPerSecond_);
						healAuraTimer.Reset();
					}
				}
			}
		}
		delete healRadius;
	}


	if (isBleeding) {
		if(currentHP <= 0)
			getRemoved();
	}
}

void Creep::getRemoved()
{
	currentHP = 0;
	dead = true;
	log("Gold Gained: %d", info->gold);
	game->getLevelManager()->increaseGold(info->gold);
}

void Creep::getAttacked(Turret * attacker)
{
	//attackedBy.push_back(attacker);
}

void Creep::gotLostSight(Turret * attacker)
{
	//attackedBy.push_back(attacker);
}

void Creep::getDamaged(BulletInfo bulletInfo)
{
	if (!isDead()) {
		currentHP -= bulletInfo.damageFrom;

		if (bulletInfo.hasSlow) {
			slowDuration = bulletInfo.slowDuration;
			slowPercentage = bulletInfo.slowPercentage;
			isSlowed = true;
			slowTimer.Start();
		}
		if (bulletInfo.hasStun) {
			int chance = cocos2d::RandomHelper::random_int(1, 100);
			if (chance <= (bulletInfo.stunChance * 100)) {
				sprite->pause();
				isStunned = true;
				stunDuration = bulletInfo.stunDuration;
				stunTimer.Start();
			}
		}
		if (bulletInfo.hasBleed) {
			bleedDamage = bulletInfo.bleedDps;
			bleedDuration = bulletInfo.bleedDuration;
			isBleeding = true;
			bleedTimer.Start();
			bleedDpsTimer.Start();
		}
		if (bulletInfo.hasSplashDamage && !inSplashRange) {
			CGCircle *damageRadius = new CGCircle(
				bulletInfo.splashRange,
				getPosition()
			);
			std::vector<Creep*> creepsInPlay = game->getLevelManager()->getCreepManager()->getCreepsInPlay();
			for (Creep * creep : creepsInPlay)
			{
				if (creep != this && !creep->isDead()) {
					if (game->checkCollision(damageRadius, creep->getBoundingBox()))
					{
						creep->setInSplashRange();
						creep->getDamaged(bulletInfo);
					}
				}
			}
			delete damageRadius;
			inSplashRange = false;
		}

		if (currentHP <= 0)
		{
			getRemoved();
		}
	}
}

bool Creep::isDead()
{
	return dead;
}

bool Creep::isMissionCompleted()
{
	return missionComplete;
}

void Creep::setInSplashRange()
{
	inSplashRange = true;
}

void Creep::setInSpeedAuraRange()
{
	inSpeedAuraRange = true;
}

void Creep::setInHealAuraRange()
{
	inHealAuraRange = true;
}

void Creep::getHealed(int hps)
{
	currentHP += hps;
	if (currentHP > info->health)
		currentHP = info->health;
	//inHealAuraRange = false;
}

void Creep::increaseSpeed(float spd)
{
	speedIncreasePercentage = spd;
	inSpeedAuraRange = true;
}

void Creep::constructPathAndStartAnimationFromStep(Creep::ShortestPathStep *step)
{
	_shortestPath.clear();

	do
	{
		// Don't add the starting position
		if (step->getParent())
		{
			// Always insert to the index in 0 position, in order to reverse path
			_shortestPath.insert(0, step);
		}
		step = step->getParent(); // Retrogression
	} while (step); // Until no one step

	for (const ShortestPathStep *s : _shortestPath)
	{
		CCLOG("%s", s->getDescription().c_str());
	}

	this->popStepAndAnimate();
}

void Creep::setScene(TowerDefence * scene)
{
	game = scene;
}

Creep::ShortestPathStep::ShortestPathStep() :
	_position(cocos2d::Point::ZERO),
	_gScore(0),
	_hScore(0),
	_parent(nullptr)
{
}

Creep::ShortestPathStep::~ShortestPathStep()
{
}

Creep::ShortestPathStep *Creep::ShortestPathStep::createWithPosition(const cocos2d::Point &pos)
{
	ShortestPathStep *pRet = new ShortestPathStep();
	if (pRet && pRet->initWithPosition(pos))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		CC_SAFE_DELETE(pRet);
		return nullptr;
	}
}

bool Creep::ShortestPathStep::initWithPosition(const cocos2d::Point &pos)
{
	bool bRet = false;
	do
	{
		this->setPosition(pos);

		bRet = true;
	} while (0);

	return bRet;
}

int Creep::ShortestPathStep::getFScore() const
{
	return this->getGScore() + this->getHScore();
}

bool Creep::ShortestPathStep::isEqual(const Creep::ShortestPathStep *other) const
{
	return getPosition() == other->getPosition();
}

std::string Creep::ShortestPathStep::getDescription() const
{
	return cocos2d::StringUtils::format("pos=[%.0f;%.0f] g=%d h=%d f=%d",
		getPosition().x, getPosition().y,
		getGScore(), this->getHScore(), this->getFScore());
}

void Creep::insertInOpenSteps(Creep::ShortestPathStep *step)
{
	int stepFScore = step->getFScore();
	ssize_t count = _spOpenSteps.size();
	ssize_t i = 0;
	for (; i <count; ++i)
	{
		if (stepFScore <= _spOpenSteps.at(i)->getFScore())
		{
			break;
		}
	}
	_spOpenSteps.insert(i, step);
}

int Creep::computeHScoreFromCoordToCoord(const Point &fromCoord, const Point &toCoord)
{
	// Here using the Manhattan method, calculated from the current steps to reach the goal of step in horizontal and vertical direction, the total number of steps
	// Ignore the obstacles in the way of possible
	return abs(toCoord.x - fromCoord.x) + abs(toCoord.y - fromCoord.y);
}

int Creep::costToMoveFromStepToAdjacentStep(const ShortestPathStep *fromStep, const ShortestPathStep *toStep)
{
	// Because you can't walk sideways, but because of the terrain is not capable of walking and walking cost are the same
	// If you can move diagonally, or swamps, hills and so on, then it must be different
	return 1;
}

ssize_t Creep::getStepIndex(const cocos2d::Vector<Creep::ShortestPathStep *> &steps, const Creep::ShortestPathStep *step)
{
	for (ssize_t i = 0; i <steps.size(); ++i)
	{
		if (steps.at(i)->isEqual(step))
		{
			return i;
		}
	}
	return -1;
}

void Creep::popStepAndAnimate()
{
	Point currentPosition = game->getLevelManager()->tileCoordForPosition(sprite->getPosition());

	/*if (levelManager->isBoneAtTilecoord(currentPosition))
	{
		//SimpleAudioEngine::getInstance()->playEffect("pickup.wav");
		//_numBones++;
		//levelManager->showNumBones(_numBones);
		levelManager->removeObjectAtTileCoord(currentPosition);
	}
	/*else if (levelManager->isDogAtTilecoord(currentPosition))
	{
		if (//_numBones == 0)
		{
			//levelManager->loseGame();
			return;
		}
		else
		{
			_numBones--;
			//levelManager->showNumBones(_numBones);
			levelManager->removeObjectAtTileCoord(currentPosition);
			//SimpleAudioEngine::getInstance()->playEffect("catAttack.wav");
		}
	}
	else if (levelManager->isExitAtTilecoord(currentPosition))
	{
		//levelManager->winGame();
		return;
	}
	else
	{
		//SimpleAudioEngine::getInstance()->playEffect("step.wav");
	}*/

	if (game->getLevelManager()->isExitAtTilecoord(currentPosition))
	{
		missionComplete = true;
		game->getLevelManager()->decreaseHealth();
		return;
	}

	if (isStunned) {
		return;
	}

	// Check whether there are steps on the path to advance
	if (_shortestPath.size() == 0)
	{
		return;
	}

	// The resulting motion next steps
	ShortestPathStep *s = _shortestPath.at(0);

	RotateTo *rotation;
	Point futurePosition = s->getPosition();
	Point diff = futurePosition - currentPosition;
	if (abs(diff.x) > abs(diff.y))
	{
		if (diff.x > 0)
		{
			rotation = RotateTo::create(0.5, 0);
		}
		else
		{
			rotation = RotateTo::create(0.5, 180);
		}
	}
	else
	{
		if (diff.y > 0)
		{
			rotation = RotateTo::create(0.5, 90);
		}
		else
		{
			rotation = RotateTo::create(0.5, 270);
		}
	}
	sprite->runAction(rotation);

	float movementSpeed = info->hasSpeedAura() ? info->speed - (info->speed * info->speedAura.percentage_) : info->speed;
	movementSpeed = isSlowed ? movementSpeed + (movementSpeed * slowPercentage) : movementSpeed;
	
	//if(inSpeedAuraRange)
		//movementSpeed = movementSpeed - .5;//movementSpeed = movementSpeed - (movementSpeed * speedIncreasePercentage);

	// Setup and callback
	MoveTo *moveAction = MoveTo::create(movementSpeed, game->getLevelManager()->positionForTileCoord(s->getPosition()));
	CallFunc *moveCallback = CallFunc::create(CC_CALLBACK_0(Creep::popStepAndAnimate, this));

	// Removing step
	_shortestPath.erase(0);

	// Running action
	Sequence *moveSequence = Sequence::create(moveAction, moveCallback, nullptr);
	moveSequence->setTag(1);
	sprite->runAction(moveSequence);
}

void Creep::draw(cocos2d::Renderer * renderer, const cocos2d::Mat4 & transform, uint32_t flags)
{
	_customCommand.init(_globalZOrder + 20);
	_customCommand.func = CC_CALLBACK_0(Creep::onDraw, this, transform, flags);
	renderer->addCommand(&_customCommand);
}

void Creep::onDraw(const cocos2d::kmMat4 & transform, uint32_t flags)
{
	ccDrawSolidRect(ccp(sprite->getPosition().x + HEALTH_BAR_ORIGIN,
	sprite->getPosition().y + 18),
	ccp(sprite->getPosition().x + HEALTH_BAR_ORIGIN + HEALTH_BAR_WIDTH,
	sprite->getPosition().y + 16),
	ccc4f(1.0, 0, 0, 1.0));

	ccDrawSolidRect(ccp(sprite->getPosition().x + HEALTH_BAR_ORIGIN,
	sprite->getPosition().y + 18),
	ccp(sprite->getPosition().x + HEALTH_BAR_ORIGIN + (float)(currentHP * HEALTH_BAR_WIDTH) / info->health,
	sprite->getPosition().y + 16),
	ccc4f(0, 1.0, 0, 1.0));
}