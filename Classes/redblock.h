#ifndef __REDBLOCK_H__
#define __REDBLOCK_H__

#include "cocos2d.h"
USING_NS_CC;

class RedBlock : public Sprite
{
public:
	static RedBlock* MyCreateInit();
	
	bool onTouchBegan(Touch* touch, Event  *event);
	void onTouchEnded(Touch* touch, Event  *event);
	void startAction();
	void stopAction();
};

#endif
