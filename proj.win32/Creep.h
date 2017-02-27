#pragma once
#include "cocos2d.h"

class CreepInfo;
class TowerDefence;
class Turret;
class Creep : public cocos2d::Node
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

	CreepInfo *info;
	TowerDefence* game;
	cocos2d::Sprite *sprite;

	float startDelay;

	Creep* initWithTheGame(TowerDefence* game_, CreepInfo *creepInfo);
	cocos2d::CustomCommand _customCommand;

	int currentHP;
	bool dead;
	bool missionComplete;
	std::vector<Turret*> attackedBy;

public:
	Creep();
	~Creep();

	Creep* nodeWithTheGame(TowerDefence* game_, CreepInfo *creepInfo);
	
	void setScene(TowerDefence *scene);
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

	virtual void draw(cocos2d::Renderer* renderer, const cocos2d::Mat4& transform, uint32_t flags);
	virtual void onDraw(const cocos2d::kmMat4 &transform, uint32_t flags);
	void setPosition(const cocos2d::Vec2 &position);
	const cocos2d::Vec2& getPosition() const;
	cocos2d::Rect getBoundingBox() const;

	CreepInfo& getCreepInfo();

	// Add method definitions
	void getRemoved();
	void getAttacked(Turret *attacker);
	void gotLostSight(Turret *attacker);
	void getDamaged(int damage);
	bool isDead();
	bool isMissionCompleted();
};



