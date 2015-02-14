#ifndef __ACTORSPRITE_H__
#define __ACTORSPRITE_H__

#include "cocos2d.h"
USING_NS_CC;

class ActorSprite : public Sprite
{
public:
	
	ActorSprite(const std::string& name);
	static ActorSprite* MyCreateInit(const std::string& ActorName);
	void stopAction();
	void startAction();

	bool onTouchBegan(Touch* touch, Event* event);
	void onTouchEnded(Touch* touch, Event* event);

	const std::string ActorName;
	bool isMoved;
	int atk;
	int life;
	int Tag;
	int range;
	int atkrange;
};

#endif
