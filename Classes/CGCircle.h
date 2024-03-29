//
//  CGCircle.h
//  HelloCpp
//
//  Created by Mike on 14-1-16.
//
//

#ifndef __HelloCpp__CGCircle__
#define __HelloCpp__CGCircle__

#include <iostream>
#include "cocos2d.h"
using namespace std;
using namespace cocos2d;

class CGCircle
{
private:
	//float _radius; //Radius
	//Vec2 _position; //Center of a circle

public:
	CC_SYNTHESIZE(float, _radius, Radius);
	CC_SYNTHESIZE(Vec2, _position, Position);

	CGCircle(float radius, Vec2 position) : _radius(radius), _position(position) {};

	bool isContainRect(Rect rect);
};

#endif /* defined(__HelloCpp__CGCircle__) */
