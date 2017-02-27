//
//  CGCircle.cpp
//  HelloCpp
//
//  Created by Mike on 14-1-16.
//
//

#include "CGCircle.h"


bool CGCircle::isContainRect(Rect rect)
{
	bool _isContian = false;
	Point orign = _position;  //The center of a circle, the origin of new coordinate system

							  //The circle tangent square Rect (x, y, width, heigth) x, y is the origin, the bottom left vertex
	Rect square = Rect::Rect(_position.x - _radius, _position.y - _radius,
		2 * _radius, 2 * _radius);

	do {
		Point rectVertex0 = rect.origin; //The lower left
		Point rectVertex1 = rect.origin + Point(rect.size.width, 0); //The lower right
		Point rectVertex2 = rect.origin + Point(rect.size.width, rect.size.height); //The upper right
		Point rectVertex3 = rect.origin + Point(0, rect.size.height); //The upper left
		Point pos[] = { rectVertex0, rectVertex1, rectVertex2, rectVertex3 };

		if ((pos[1].x - orign.x) * (pos[3].x - orign.x) > 0 &&
			(pos[1].y - orign.y) * (pos[3].y - orign.y) > 0)
		{
			//The lower right vertex and left on the vertices in the same quadrant, the rectangular located in one quadrant if each vertex has at least one in the circle is the intersection
			for (int i = 0; i <4; i++) {
				//Rectangular four vertices to judge whether there is at least one located in the circle
				if (((pos[i].x - orign.x) * (pos[i].x - orign.x)
					+ (pos[i].y - orign.y) * (pos[i].y - orign.y))
					<= _radius * _radius)
				{
					goto next;
				}
			}
		}

		if (square.intersectsRect(rect)) {
			goto next;
		}

		for (int i = 0; i <4; i++) {
			if (pos[i] == orign) {
				goto next;
			}
		}

		break;

	next:
		_isContian = true;
	} while (0);

	return _isContian;
}