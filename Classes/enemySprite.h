#ifndef __ENEMYSPRITE_H__
#define __ENEMYSPRITE_H__

#include "cocos2d.h"
USING_NS_CC;

class EnemySprite : public Sprite
{
public:
	
	EnemySprite(const std::string& name);
	static EnemySprite* MyCreateInit(const std::string& ActorName);
	void stop_Action();
	void startAction();
	void Enemystop_Action();
	void EnemyStartAction();

	void stop_ActionT();
	void startActionT();
	void Enemystop_ActionT();
	void EnemyStartActionT();
	void updateBlood();

	bool onTouchBegan(Touch* touch, Event* event);
	void onTouchEnded(Touch* touch, Event* event);
	void showTime();
	void startMission();
	void Nothing();




	const std::string ActorName;
	bool isMoved;
	int atk;
	int life;
	int Tag;
	int range;
	int atkrange;
};

#endif
