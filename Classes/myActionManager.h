#ifndef __MYACTIONMANAGER__
#define __MYACTIONMANAGER__

#include "cocos2d.h"
USING_NS_CC;

class myActionManager {
public:
	static Animate* newRunAction(std::string texture);
	static Animate* newAttackAction(std::string texture);
	static Animate* newAttackedAction(std::string texture);
	static Action* MoveTo(Vec2 src, Vec2 dst);
	static Action* AIMoveTo(Vec2 src, int MovRange, int AtkRange);
};

#endif
