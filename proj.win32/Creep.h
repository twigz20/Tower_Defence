#ifndef _MYSPRITE_HPP_
#define _MYSPRITE_HPP_

#include "cocos2d.h"
#include "Observee.h"
#include "BaseObject.h"
#include "CreepInfo.h"
#include "TurretInfo.h"
#include "timer.h"

struct Status {
	Status() :
		isSlowed(false), isStunned(false), isBleeding(false),
		stunDuration(0), slowDuration(0), bleedDuration(0),
		bleedDamage(0), slowPercentage(0), speedIncreasePercentage(0),
		inSplashRange(false), inSpeedAuraRange(false), inHealAuraRange(false),
		startDelay(0.f), currentHP(0), dead(false), missionComplete(false)
	{}

	bool isSlowed;
	bool isStunned;
	bool isBleeding;
	bool inSplashRange;
	bool inSpeedAuraRange;
	bool inHealAuraRange;

	float slowDuration;
	float stunDuration;
	float bleedDuration;

	float bleedDamage;

	float slowPercentage;
	float speedIncreasePercentage;

	SimpleTimer slowTimer;
	SimpleTimer stunTimer;
	SimpleTimer bleedTimer;
	SimpleTimer bleedDpsTimer;
	SimpleTimer selfHealAuraTimer;
	SimpleTimer healAuraTimer;

	float startDelay;
	int currentHP;

	bool dead;
	bool missionComplete;
};

class TowerDefence;
class TowerDefence;
class Turret;
class BulletInfo;
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

	cocos2d::CustomCommand _customCommand;

public:
	Creep(TowerDefence* game_, CreepInfo creepInfo);
	Creep(const Creep& other);
	~Creep();

	void initCreep();
	void update(float deltaTime);
	virtual void draw(cocos2d::Renderer* renderer, const cocos2d::Mat4& transform, uint32_t flags);
	virtual void onDraw(const cocos2d::kmMat4 &transform, uint32_t flags);

	void moveToward(cocos2d::Point target);
	bool isMissionCompleted();
	bool isDead();

	void getAttacked(Turret* attacker);
	void gotLostSight(Turret* attacker);
	void getRemoved();
	void getDamaged(BulletInfo bulletInfo);
	void setInSplashRange();
	void setInSpeedAuraRange();
	void setInHealAuraRange();
	void getHealed(int hps);
	void increaseSpeed(float spd);

	Status& getCreepStatus() { return creepStatus; }
	void setCreepStatus(Status val) { creepStatus = val; }
	CreepInfo& getCreepInfo() { return info; }
	void setInfo(CreepInfo val) { info = val; }
private:
	CreepInfo info;
	TowerDefence* game;
	Status creepStatus;
	std::map<int, Turret*> attackedBy;
};

#endif // _MYSPRITE_HPP_