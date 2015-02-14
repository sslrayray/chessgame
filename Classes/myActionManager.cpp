#include "myActionManager.h"
#include "res.h"

const int ActorRun = 1;
const int ActorStop = 5;
const int ActorBack = 9;
const int ActorAttacked = 13;
const int ActorAttack = 17;
const int ActorBackActtack = 21;
const int ActorDie = 25;
const int ActorWin = 29;

Animate* myActionManager::newRunAction(std::string texture){
	Vector<cocos2d::SpriteFrame*> animFrames;
	char str[100] = {0};
	for (int i = 0; i < 4; i++){
		sprintf(str, (texture+"_%i.png").data(), i+ActorRun);
		CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(str);
		animFrames.pushBack(frame);
	}
	//设置飞翔动画
	CCAnimation* animation = CCAnimation::createWithSpriteFrames(animFrames, 0.3);
	animation->setLoops(-1);
	return CCAnimate::create(animation);
}

Animate* myActionManager::newAttackAction(std::string texture){
	Vector<cocos2d::SpriteFrame*> animFrames;
	char str[100] = {0};
	for (int i = 0; i < 4; i++){
		sprintf(str, (texture+"_%i.png").data(), i+ActorAttack);
		CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(str);
		animFrames.pushBack(frame);
	}
	//设置打人
	CCAnimation* animation = CCAnimation::createWithSpriteFrames(animFrames, 0.2);
	animation->setLoops(1);
	return CCAnimate::create(animation);
}

Animate* myActionManager::newAttackedAction(std::string texture){
	Vector<cocos2d::SpriteFrame*> animFrames;
	char str[100] = {0};
	for (int i = 0; i < 4; i++){
		sprintf(str, (texture+"_%i.png").data(), i+ActorAttacked);
		CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(str);
		animFrames.pushBack(frame);
	}
	//设置被打
	CCAnimation* animation = CCAnimation::createWithSpriteFrames(animFrames, 0.2);
	animation->setLoops(1);
	return CCAnimate::create(animation);
}


Action* myActionManager::MoveTo(Vec2 src, Vec2 dst){//已经是地图坐标
	
	int dst_x = (dst.x / _tileMap->getTileSize().width);
	int dst_y = (dst.y/ _tileMap->getTileSize().height);

	int src_x = (src.x / _tileMap->getTileSize().width);
	int src_y = (src.y/ _tileMap->getTileSize().height);

	std::vector<Vec2> tv =  _tileMap->getPath(Vec2(src_x, src_y), Vec2(dst_x, dst_y), 255);
	Vector<FiniteTimeAction*> temp;
	for (auto i: tv){
		temp.pushBack(MoveBy::create(0.5, Vec2(i.x*_tileMap->getTileSize().width, i.y*_tileMap->getTileSize().height)));
	}
	
	//return Sequence::create(MoveBy::create(0.5, Vec2(-100, -100)), MoveTo::create(0.5, dst), nullptr);
	return Sequence::create(temp);
}

Action* myActionManager::AIMoveTo(Vec2 src, int MovRange, int AtkRange){
	
	int src_x = (src.x / _tileMap->getTileSize().width);
	int src_y = (src.y/ _tileMap->getTileSize().height);

	std::vector<Vec2> tv =  _tileMap->actionAI(Vec2(src_x, src_y), MovRange, AtkRange);

	Vector<FiniteTimeAction*> temp;

	for (auto i: tv){
		temp.pushBack(MoveBy::create(0.5, Vec2(i.x*_tileMap->getTileSize().width, i.y*_tileMap->getTileSize().height)));
	}
	if (temp.size() == 0){
		temp.pushBack(MoveBy::create(0.2, Vec2(0, 0)));
	}
	//return Sequence::create(MoveBy::create(0.5, Vec2(-100, -100)), MoveTo::create(0.5, dst), nullptr);
	return Sequence::create(temp);
}
