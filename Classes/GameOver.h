#ifndef __GameOver_SCENE_H__
#define __GameOver_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;

class GameOver : public cocos2d::Layer
{
public:
	
	static cocos2d::Scene* createScene();
	//��ʼ��GameLayerLayer
	bool init();
	//������һ������
	

	CREATE_FUNC(GameOver);
};

#endif