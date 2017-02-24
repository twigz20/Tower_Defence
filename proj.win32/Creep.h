#pragma once
#include "BaseObject.h"
//#include "TowerDefenceScene.h"

class LevelManager;
class TowerDefence;
struct Creep_Stats {
	int health;
	int speed;
	int gold;
};

class Creep : public BaseObject
{
	/* Path Finding Algorithm - Start */
	class ShortestPathStep : public cocos2d::Object
	{
	public:
		ShortestPathStep();
		~ShortestPathStep();

		static ShortestPathStep *createWithPosition(const cocos2d::Point &pos);
		bool initWithPosition(const cocos2d::Point &pos);

		int getFScore() const;
		bool isEqual(const ShortestPathStep *other) const;
		std::string getDescription() const;

		CC_SYNTHESIZE(cocos2d::Point, _position, Position);
		CC_SYNTHESIZE(int, _gScore, GScore);
		CC_SYNTHESIZE(int, _hScore, HScore);
		CC_SYNTHESIZE(ShortestPathStep*, _parent, Parent);
	};

	void insertInOpenSteps(Creep::ShortestPathStep *step);
	int computeHScoreFromCoordToCoord(const cocos2d::Point &fromCoord, const cocos2d::Point &toCoord);
	int costToMoveFromStepToAdjacentStep(const ShortestPathStep *fromStep, const ShortestPathStep *toStep);
	ssize_t getStepIndex(const cocos2d::Vector<Creep::ShortestPathStep *> &steps, const Creep::ShortestPathStep *step);
	void constructPathAndStartAnimationFromStep(Creep::ShortestPathStep *step);
	void popStepAndAnimate();

	cocos2d::Vector<ShortestPathStep*> _spOpenSteps;
	cocos2d::Vector<ShortestPathStep*> _spClosedSteps;
	cocos2d::Vector<ShortestPathStep*> _shortestPath;
	/* Path Finding Algorithm - End */

	Creep_Stats stats;
	TowerDefence *_layer;
	LevelManager *levelManager;

	float startDelay;
public:
	Creep(const std::string& creep);
	~Creep();
	void setLayer(TowerDefence* layer);
	void setLevelManager(LevelManager *levelManager_);
	void setHealth(const int& health);
	void setSpeed(const int& speed);
	void setGold(const int& gold);
	void setStartDelay(const float& startDelay);

	const int getHealth() const;
	const float getSpeed() const;
	const int getGold() const;
	const float getStartDelay() const;

	void update(float deltaTime);
	void moveToward(cocos2d::Point target);
};



