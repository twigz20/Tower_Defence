#pragma once

#include <vector>
#include "BaseObject.h"
#include "Events.h"

using namespace std;

class Observee;
//-----------------------------------------------------
// Observer Class			
//-----------------------------------------------------
class Observer
{
public:
	virtual ~Observer() {};		// Destructor
	virtual void Notify(Observee* observee) {};
	virtual void onNotify(BaseObject* entity, Twigz::Event event) = 0;
protected:
	//constructor is protected because this class is abstract, it’s only meant to be inherited!
	Observer() {};
private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	Observer(const Observer& yRef) = delete;
	Observer& operator=(const Observer& yRef) = delete;
};