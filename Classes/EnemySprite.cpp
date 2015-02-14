#include "EnemySprite.h"
#include "res.h"
#include "blueblock.h"
#include "myActionManager.h"
#include "GameOver.h"

USING_NS_CC;

Action* NowRunAction = nullptr;

EnemySprite::EnemySprite(const std::string& name):ActorName(name),isMoved(false){}

EnemySprite* EnemySprite::MyCreateInit(const std::string& ActorName){
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
	EnemySprite *sprite = new (std::nothrow) EnemySprite(ActorName);
    if (sprite && spriteFrame && sprite->initWithSpriteFrame(spriteFrame))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

bool EnemySprite::onTouchBegan(Touch* touch, Event* event){
	auto target = static_cast<EnemySprite*>(event->getCurrentTarget());
	if (isAnimate||!isGezi) return false; //场面上没有格子的时候不能被攻击
	return TouchInTarget(touch, event);//判断按下是否在对象的范围之内
}

void EnemySprite::onTouchEnded(Touch* touch, Event* event){
	if (!TouchInTarget(touch, event)||_currentActor == nullptr) //如果没有点到一个人不会生成格子
		return;
	
	auto target = static_cast<EnemySprite*> (event->getCurrentTarget());
	if (_currentActor->getPosition().distance(target->getPosition()) > (_tileMap->getTileSize().width)*(_currentActor->atkrange))
		return;//如果当前活动人物不在他旁边的话不动
	
	isGezi = false;
	_blockroot->removeAllChildren();
	
	auto stopCallBack = CallFunc::create(CC_CALLBACK_0(EnemySprite::stop_Action, this));
	auto startCallBack = CallFunc::create(CC_CALLBACK_0(EnemySprite::startAction, this));
	
	if (_currentActor->getPositionX() > target->getPositionX()){
		_currentActor->setFlippedX(true); 
		target->setFlippedX(false); 
	}else{
		_currentActor->setFlippedX(false);
		target->setFlippedX(true); 
	}
	_currentActor->stopAllActions();
	_currentActor->isMoved = true;
 	_currentActor->runAction(Sequence::create(
								startCallBack,
								myActionManager::newAttackAction(_currentActor->ActorName),
								stopCallBack,
								nullptr
								));
	auto EnStopCallBack = CallFunc::create(CC_CALLBACK_0(EnemySprite::Enemystop_Action, this));
	auto EnStartCallBack = CallFunc::create(CC_CALLBACK_0(EnemySprite::EnemyStartAction, this));
	
	target->life -= _currentActor->atk;
	target->runAction(Sequence::create(
						EnStartCallBack,
						myActionManager::newAttackedAction(target->ActorName),
						EnStopCallBack,
						nullptr)
					);
	
}

void EnemySprite::stop_Action(){
	if (_currentActor == nullptr) return;
	_currentActor->stopAllActions();
	_currentActor->setSpriteFrame(_currentActor->ActorName+"_5.png");
	isAnimate = false;
}

void EnemySprite::startAction(){
	isAnimate = true;
	std::string bgm = "";
	if (_currentActor->ActorName == "leader") bgm = "knife.wav"; else
	if (_currentActor->ActorName == "boss") bgm = "cannon.wav"; else
	if (_currentActor->ActorName == "devil") bgm = "magic.wav"; else
		bgm = "shoot.wav";
	
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(bgm.data());
}

void EnemySprite::Enemystop_Action(){
	this->stopAllActions();
	this->setSpriteFrame(this->ActorName+"_5.png");
	isAnimate = false;
	if (this->life <= 0){
		auto p = this->getParent();
		_tileMap->enemyArray.eraseObject(this);
		_tileMap->actorArray.erase(getXYofNode(this));
		p->removeChild(this, true);
		if (_tileMap->enemyArray.size() == 0){
			gamewinFlag = true;
		}
		//注意，map的vector数组也要删掉！
	}else{
		auto p = this->getChildByTag(this->Tag);
		p->runAction(ScaleTo::create(0.5, float(this->life), 1.0));
	}
	
}

void EnemySprite::EnemyStartAction(){
	isAnimate = true;
}

int myindex = -1;

void EnemySprite::showTime(){
	if (isAnimate && myindex == -1) return;
	isAnimate = true;
	myindex ++;
	//打架
	auto action = myActionManager::AIMoveTo(this->getPosition(),this->range, this->atkrange );///生成动作

	if (targetAI!=nullptr){
		if (targetAI->getPositionX() < this->getPositionX())
			this->setFlippedX(true);
		else
			this->setFlippedX(false);
		NowRunAction = this->runAction(myActionManager::newRunAction(this->ActorName));
	}
	if (myindex < _tileMap->enemyArray.size() - 1){
		auto NextCallBack = CallFunc::create(CC_CALLBACK_0(EnemySprite::showTime, _tileMap->enemyArray.at(myindex+1)));//可以写成this
		auto BeginCallBack = CallFunc::create(CC_CALLBACK_0(EnemySprite::Nothing, this));
		auto startAtkCallBack = CallFunc::create(CC_CALLBACK_0(EnemySprite::startActionT, this)); //开始攻击
		auto stopAtkCallBack = CallFunc::create(CC_CALLBACK_0(EnemySprite::stop_ActionT, this));  //停止攻击
		auto startAtkedCallBack = CallFunc::create(CC_CALLBACK_0(EnemySprite::EnemyStartActionT, this)); //开始被攻击
		auto stopAtkedCallBack = CallFunc::create(CC_CALLBACK_0(EnemySprite::Enemystop_ActionT, this)); //停止被攻击
		auto updateBldCallBack = CallFunc::create(CC_CALLBACK_0(EnemySprite::updateBlood, this)); //

		
		int delayTime = 0;
		if (targetAI != nullptr)
			delayTime = 2;
		this->runAction(
			Sequence::create(
			BeginCallBack,
			action,
			startAtkCallBack, //敌人攻击
			startAtkedCallBack, //被攻击
			updateBldCallBack,
			DelayTime::create(delayTime),
			stopAtkCallBack,
			stopAtkedCallBack,
			NextCallBack,
			nullptr	)
			);

	}else{
		auto EndCallBack = CallFunc::create(CC_CALLBACK_0(EnemySprite::startMission, this));
		auto BeginCallBack = CallFunc::create(CC_CALLBACK_0(EnemySprite::Nothing, this));
		_tileMap->enemyArray.at(myindex)->runAction(
			Sequence::create(
				BeginCallBack,
				action,
				EndCallBack,
				nullptr)
			);
	}

}

void EnemySprite::startMission(){
	myindex = -1;
	isAnimate = false;
	for (auto i: _tileMap->friendArray){
		i->isMoved = false;
	}
	_currentActor = nullptr;
	if (targetAI == nullptr) {
		return;
	}
	this->stopAction(NowRunAction);
	auto BeginCallBack = CallFunc::create(CC_CALLBACK_0(EnemySprite::Nothing, this));
	auto stopAtkedCallBack = CallFunc::create(CC_CALLBACK_0(EnemySprite::Enemystop_ActionT, this)); //停止被攻击
	auto stopAtkCallBack = CallFunc::create(CC_CALLBACK_0(EnemySprite::stop_ActionT, this));  //停止攻击
	targetAI->runAction(Sequence::create(
		BeginCallBack,
		myActionManager::newAttackedAction(targetAI->ActorName),
		stopAtkedCallBack,
		nullptr )
		);
	this->runAction(Sequence::create(
		BeginCallBack,
		myActionManager::newAttackAction(this->ActorName),
		stopAtkCallBack,
		nullptr )
		);
	std::string bgm = "";
	if (this->ActorName == "leader") bgm = "knife.wav"; else
	if (this->ActorName == "boss") bgm = "cannon.wav"; else
	if (this->ActorName == "devil") bgm = "magic.wav"; else
		bgm = "shoot.wav";
	
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(bgm.data());
	
}

void EnemySprite::Nothing(){
}



void EnemySprite::stop_ActionT(){
	this->setSpriteFrame(this->ActorName+"_5.png");
}

void EnemySprite::startActionT(){
	this->stopAction(NowRunAction);
	if (targetAI == nullptr) return;
	std::string bgm = "";
	if (this->ActorName == "leader") bgm = "knife.wav"; else
	if (this->ActorName == "boss") bgm = "cannon.wav"; else
	if (this->ActorName == "devil") bgm = "magic.wav"; else
		bgm = "shoot.wav";
	
	this->runAction(myActionManager::newAttackAction(this->ActorName));
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(bgm.data());
}


void EnemySprite::Enemystop_ActionT(){
	if (targetAI == nullptr) return;
	targetAI->stopAllActions();
	targetAI->setSpriteFrame(targetAI->ActorName+"_5.png");

	if(targetAI!= nullptr){
		targetAI->life -= this->atk;
		if (targetAI->life <= 0){
			auto p = this->getParent();
			_tileMap->friendArray.eraseObject(targetAI, true);
			_tileMap->actorArray.erase(getXYofNode(targetAI));
			p->removeChild(targetAI, true);
			targetAI = nullptr;
			if (_tileMap->friendArray.size() == 0){
				gameoverFlag = true;
			}
		//注意，map的vector数组也要删掉！
		}
	}
}

void EnemySprite::EnemyStartActionT(){
	isAnimate = true;
	if (targetAI == nullptr) return;
	targetAI->runAction(myActionManager::newAttackedAction(targetAI->ActorName));
}

void EnemySprite::updateBlood(){
	if (targetAI == nullptr) return;
	auto p = targetAI->getChildByTag(targetAI->Tag);
	p->runAction(ScaleTo::create(0.5, float(targetAI->life), 1.0));
}