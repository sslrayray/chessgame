#include "redblock.h"
#include "myActionManager.h"
#include "res.h"

RedBlock* RedBlock::MyCreateInit(){
	CCTexture2D::PVRImagesHavePremultipliedAlpha(true);
	//auto sprite = CCSprite::createWithSpriteFrameName(ActorName+"_1.png");
	const std::string spriteFrameName= "red.png";
	SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(spriteFrameName);
    
#if COCOS2D_DEBUG > 0
    char msg[256] = {0};
    sprintf(msg, "Invalid spriteFrameName: %s", spriteFrameName.c_str());
    CCASSERT(frame != nullptr, msg);
#endif
    auto spriteFrame = frame;
    //return createWithSpriteFrame(frame);
	//newҪ��д
	RedBlock *sprite = new (std::nothrow) RedBlock();
    if (sprite && spriteFrame && sprite->initWithSpriteFrame(spriteFrame))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

bool RedBlock::onTouchBegan(Touch* touch, Event  *event){
	return false;
	if (isAnimate || _currentActor->isMoved) return false;
	
	return TouchInTarget(touch, event);//�жϰ����Ƿ��ڶ���ķ�Χ֮��
}

void RedBlock::onTouchEnded(Touch* touch, Event  *event){
}


void RedBlock::startAction(){
	isAnimate = true;
}

void RedBlock::stopAction(){
}