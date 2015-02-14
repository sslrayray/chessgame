#ifndef __GameOver_SCENE_H__
#define __GameOver_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;

class GameOver : public cocos2d::Layer
{
public:
	
	static cocos2d::Scene* createScene();
	//初始化GameLayerLayer
	bool init();
	//返回上一个界面
	

	CREATE_FUNC(GameOver);
};

#endif