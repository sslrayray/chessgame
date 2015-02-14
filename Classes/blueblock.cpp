#include "blueblock.h"
#include "myActionManager.h"
#include "res.h"

unsigned int walkmic;

BlueBlock* BlueBlock::MyCreateInit(){
	CCTexture2D::PVRImagesHavePremultipliedAlpha(true);
	//auto sprite = CCSprite::createWithSpriteFrameName(ActorName+"_1.png");
	const std::string spriteFrameName= "blue.png";
	SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(spriteFrameName);
    
#if COCOS2D_DEBUG > 0
    char msg[256] = {0};
    sprintf(msg, "Invalid spriteFrameName: %s", spriteFrameName.c_str());
    CCASSERT(frame != nullptr, msg);
#endif
    auto spriteFrame = frame;
    //return createWithSpriteFrame(frame);
	//new要重写
	BlueBlock *sprite = new (std::nothrow) BlueBlock();
    if (sprite && spriteFrame && sprite->initWithSpriteFrame(spriteFrame))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

bool BlueBlock::onTouchBegan(Touch* touch, Event  *event){
	if (_currentActor!=nullptr && isAnimate || _currentActor->isMoved) return false;
	
	return TouchInTarget(touch, event);//判断按下是否在对象的范围之内
}

void BlueBlock::onTouchEnded(Touch* touch, Event  *event){
	if (!TouchInTarget(touch, event)) 
		return;
	//设置人物走路动作
	auto stopCallBack = CallFunc::create(CC_CALLBACK_0(BlueBlock::stopAction, this));
	auto startCallBack = CallFunc::create(CC_CALLBACK_0(BlueBlock::startAction, this));
	if (_currentActor->getPosition().x < coortoMap(touch->getLocation()).x)
		_currentActor->setFlipX(false);
	else
		_currentActor->setFlipX(true);
	_currentActor->runAction(myActionManager::newRunAction(_currentActor->ActorName));
	_currentActor->runAction(Sequence::create(
				startCallBack,
				myActionManager::MoveTo(_currentActor->getPosition(), coortoMap(touch->getLocation())),	
				stopCallBack,
				nullptr
				));
				
				//_currentActor->setPosition(coortoMap(touch->getLocation()));
}


void BlueBlock::startAction(){
	isAnimate = true;
	walkmic = CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("walk.mp3", true, 1, 0, 0.5);
}

void BlueBlock::stopAction(){
	_currentActor->stopAllActions();
	_currentActor->setSpriteFrame(_currentActor->ActorName+"_5.png");
	_currentActor->isMoved = true;
	isAnimate = false;
	CocosDenshion::SimpleAudioEngine::sharedEngine()->stopEffect(walkmic);
	//_blockroot->removeAllChildren();
	//isGezi = false;
}