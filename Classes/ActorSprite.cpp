#include "ActorSprite.h"
#include "res.h"
#include "blueblock.h"
#include "redblock.h"
#include "myActionManager.h"

USING_NS_CC;



ActorSprite::ActorSprite(const std::string& name):ActorName(name),isMoved(false){}

ActorSprite* ActorSprite::MyCreateInit(const std::string& ActorName){
	CCTexture2D::PVRImagesHavePremultipliedAlpha(true);
	//auto sprite = CCSprite::createWithSpriteFrameName(ActorName+"_1.png");
	const std::string spriteFrameName= ActorName+"_5.png";
	SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(spriteFrameName);
    
#if COCOS2D_DEBUG > 0
    char msg[256] = {0}; 
    sprintf(msg, "Invalid spriteFrameName: %s", spriteFrameName.c_str());
    CCASSERT(frame != nullptr, msg);
#endif
    auto spriteFrame = frame;
    //return createWithSpriteFrame(frame);
	//new要重写
	ActorSprite *sprite = new (std::nothrow) ActorSprite(ActorName);
    if (sprite && spriteFrame && sprite->initWithSpriteFrame(spriteFrame))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

bool ActorSprite::onTouchBegan(Touch* touch, Event* event){
	auto target = static_cast<ActorSprite*>(event->getCurrentTarget());
	if (isAnimate||target->isMoved) return false; 
	return TouchInTarget(touch, event);//判断按下是否在对象的范围之内
}

void ActorSprite::onTouchEnded(Touch* touch, Event* event){
	
	if (!TouchInTarget(touch, event)) //如果没有点到一个人不会生成格子
		return;
	if (isGezi){
		isGezi = false;
		_blockroot->removeAllChildren();
		return;
	}
		
	isGezi = true;
	auto target = static_cast<ActorSprite*>(event->getCurrentTarget());
	if (TouchInTarget(touch, event)){
		if (_currentActor!=nullptr){
			_currentActor->stopAllActions();
			_currentActor->setSpriteFrame(_currentActor->ActorName+"_5.png");
		}
		_currentActor = static_cast<ActorSprite*>(target);
		_currentActor->runAction(myActionManager::newRunAction(_currentActor->ActorName));
		_blockroot->removeAllChildren();
		Vec2 corp = touch->getLocation();
		Vec2 mapp = coortoMap(corp);
		
		_tileMap->getMoveRange(Vec2(int(mapp.x)/int(_tileMap->getTileSize().width), 
							int(mapp.y)/int(_tileMap->getTileSize().height)), target->range);
		_tileMap->getAttackRange(Vec2(int(mapp.x)/int(_tileMap->getTileSize().width), 
							int(mapp.y)/int(_tileMap->getTileSize().height)), target->range+target->atkrange);
	}

}

void ActorSprite::stopAction(){
	if(_currentActor==nullptr) return;
	_currentActor->stopAllActions();
	isAnimate = false;
}

void ActorSprite::startAction(){
	isAnimate = true;
}