#ifndef __GameLayer_SCENE_H__
#define __GameLayer_SCENE_H__

#include "cocos2d.h"
#include "ActorSprite.h"

USING_NS_CC;

class GameLayer : public cocos2d::Layer
{
public:
	
	static cocos2d::Scene* createScene();
	//��ʼ��GameLayerLayer
	bool init();
	//������һ������
	
	void memuTurnBack(Ref* pSender);
	void MissionEnd(Ref* pSender);
	void testUpdate(float dt);

	CREATE_FUNC(GameLayer);
};

#endif
