#include "Creep.h"
#include "proj.win32\Utils.h"
#include "TowerDefenceScene.h"
#include "LevelManager.h"

using namespace cocos2d;

Creep::Creep(const std::string & filename)
	: BaseObject(filename),
	startDelay(0.f)
{
	
}

Creep::~Creep()
{
}

void Creep::setHealth(const int& health)
{
	stats.health = health;
}

void Creep::setSpeed(const int& speed)
{
	stats.speed = speed;
}

void Creep::setGold(const int& gold)
{
	stats.gold = gold;
}

void Creep::setStartDelay(const float & startDelay)
{
	this->startDelay = startDelay;
}

const int Creep::getHealth() const
{
	return stats.health;
}

const float Creep::getSpeed() const
{
	return stats.speed;
}

const int Creep::getGold() const
{
	return stats.gold;
}

const float Creep::getStartDelay() const
{
	return startDelay;
}

void Creep::update(float deltaTime)
{
}

void Creep::moveToward(cocos2d::Point target)
{
	Point fromTileCoord = levelManager->tileCoordForPosition(this->getPosition());
	Point toTileCoord = levelManager->tileCoordForPosition(target);

	if (fromTileCoord == toTileCoord)
	{
		CCLOG("You're already there! :P");
		return;
	}

	if (!levelManager->isValidTileCoord(toTileCoord) || levelManager->isWallAtTileCoord(toTileCoord))
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
		PointArray *adjSteps = levelManager->walkableAdjacentTilesCoordForTileCoord(currentStep->getPosition());
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

void Creep::setLayer(TowerDefence * layer)
{
	this->_layer = layer;
}

void Creep::setLevelManager(LevelManager * levelManager_)
{
	levelManager = levelManager_;
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
	return this->getPosition() == other->getPosition();
}

std::string Creep::ShortestPathStep::getDescription() const
{
	return cocos2d::StringUtils::format("pos=[%.0f;%.0f] g=%d h=%d f=%d",
		this->getPosition().x, this->getPosition().y,
		this->getGScore(), this->getHScore(), this->getFScore());
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
	Point currentPosition = levelManager->tileCoordForPosition(this->getPosition());

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

	if (levelManager->isExitAtTilecoord(currentPosition))
	{
		object->setVisible(false);
		levelManager->decreaseCreepAmount();
		return;
	}

	// Check whether there are steps on the path to advance
	if (_shortestPath.size() == 0)
	{
		return;
	}

	// The resulting motion next steps
	ShortestPathStep *s = _shortestPath.at(0);

	Point futurePosition = s->getPosition();
	Point diff = futurePosition - currentPosition;
	if (abs(diff.x) > abs(diff.y))
	{
		if (diff.x > 0)
		{
			//this->runAnimation(_facingRightAnimation);
		}
		else
		{
			//this->runAnimation(_facingLeftAnimation);
		}
	}
	else
	{
		if (diff.y > 0)
		{
			//this->runAnimation(_facingForwardAnimation);
		}
		else
		{
			//this->runAnimation(_facingBackAnimation);
		}
	}

	// Setup and callback
	MoveTo *moveAction = MoveTo::create(getSpeed() /100, levelManager->positionForTileCoord(s->getPosition()));
	CallFunc *moveCallback = CallFunc::create(CC_CALLBACK_0(Creep::popStepAndAnimate, this));

	// Removing step
	_shortestPath.erase(0);

	// Running action
	Sequence *moveSequence = Sequence::create(moveAction, moveCallback, nullptr);
	moveSequence->setTag(1);
	(object)->runAction(moveSequence);
}