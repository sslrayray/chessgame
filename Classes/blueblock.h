#ifndef __BLUEBLOCK_H__
#define __BLUEBLOCK_H__

#include "cocos2d.h"
USING_NS_CC;

class BlueBlock : public Sprite
{
public:
	static BlueBlock* MyCreateInit();
	
	bool onTouchBegan(Touch* touch, Event  *event);
	void onTouchEnded(Touch* touch, Event  *event);
	void startAction();
	void stopAction();
};

#endif
