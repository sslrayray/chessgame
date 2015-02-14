#include "HelloWorldScene.h"
#include "GameLayer.h"
#include "ActorSprite.h"
#include "SimpleAudioEngine.h"
#include "CocoStudio.h"

using namespace cocostudio;
USING_NS_CC;

enum{
	RedDragonspTag,
	RedDragonFlyTag
};

unsigned int wavmic;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
	
	wavmic =  CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("weage.wav",true); 

	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("images.plist");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("color.plist");

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.


	//背景图
    auto background = Sprite::create("background.png");
	background->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
	this->addChild(background, 0);
	
	//进入菜单
	auto enterItem = MenuItemImage::create("EnterNormal.png",
										   "EnterSelected.png",
										   CC_CALLBACK_1(HelloWorld::enterGameCallback, this));

	enterItem->setPosition(Vec2(origin.x+visibleSize.width/2, 
								origin.y+visibleSize.height/2));
    
    //退出菜单
	auto exitItem = MenuItemImage::create("ExitNormal.png",
										   "ExitSelected.png",
										   CC_CALLBACK_1(HelloWorld::exitGameCallback, this));

	exitItem->setPosition(Vec2(origin.x+visibleSize.width/2, 
								origin.y+visibleSize.height/2-
								exitItem->getContentSize().height*1.5));
    


	
    // create menu, it's an autorelease object
    auto menu = Menu::create(enterItem, NULL);
	menu->addChild(exitItem);

    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

	//设置点击图层的鼠标监听
	auto listener =	EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
	listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
	//加入到事件分发器中
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	CCTexture2D::PVRImagesHavePremultipliedAlpha(true);

	
	auto reddragonsp = CCSprite::createWithSpriteFrameName("reddragon_1.png");
	reddragonsp->setPosition(visibleSize.width - reddragonsp->getContentSize().width, 
		                     visibleSize.height - reddragonsp->getContentSize().height);
	reddragonsp->setFlipX(true);
	this->addChild(reddragonsp, 1, RedDragonspTag);
	
	
	//设置动画
	
	//飞龙动画
	//通过plist加载帧动画;
	
	Vector<cocos2d::SpriteFrame*> animFrames;
	char str[100] = {0};
	for (int i = 1; i < 5; i++){
		sprintf(str, "reddragon_%i.png", i);
		CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(str);
		animFrames.pushBack(frame);
	}
	//设置飞翔动画
	CCAnimation* animation = CCAnimation::createWithSpriteFrames(animFrames, 0.2);
	animation->setLoops(-1);
	auto animationAct = CCAnimate::create(animation);
	animationAct->setTag(RedDragonFlyTag);
	
	reddragonsp->runAction(animationAct);
	//清空缓存

	//cocostudio
	ArmatureDataManager::getInstance()->addArmatureFileInfo("Hero0.png", "Hero0.plist" , "Hero.ExportJson");
	Armature *armature = Armature::create( "Hero");
	armature->setPosition(ccp(visibleSize.width * 0.5, visibleSize.height * 0.5));
	armature->getAnimation()->playWithIndex(1);
	this->addChild(armature);

	CCTexture2D::PVRImagesHavePremultipliedAlpha(true);
	
    return true;
}



bool HelloWorld::onTouchBegan(Touch* touch, Event* eventt){
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("btn.wav"); 
	return true;
}

void HelloWorld::onTouchEnded(Touch * touch, Event* eventt){
	auto reddragonsp = getChildByTag(RedDragonspTag);


	auto location = touch->getLocation();
	auto nowPos = reddragonsp->getPosition();

	if (nowPos.x < location.x)
		(static_cast<Sprite*>(reddragonsp))->setFlippedX(false);
	else 
		(static_cast<Sprite*>(reddragonsp))->setFlippedX(true);
	
	float dis = nowPos.getDistance(location);
	auto stopFlyCallBack = CallFunc::create(CC_CALLBACK_0(HelloWorld::StopRedDragonFly, this));
	auto startFlyCallBack = CallFunc::create(CC_CALLBACK_0(HelloWorld::StartRedDragonFly, this));
	(static_cast<Sprite*>(reddragonsp))->runAction(Sequence::create(//startFlyCallBack,
																	MoveTo::create(1, Vec2(location.x, location.y)),
																	//stopFlyCallBack,
																	nullptr));
}

void HelloWorld::StopRedDragonFly(){
	auto reddragonsp = static_cast<Sprite*>(getChildByTag(RedDragonspTag));
	reddragonsp->stopActionByTag(RedDragonFlyTag);
}

void HelloWorld::StartRedDragonFly(){
	auto reddragonsp = getChildByTag(RedDragonspTag);
	//飞龙动画
	//通过plist加载帧动画
	CCTexture2D::PVRImagesHavePremultipliedAlpha(true);


	Vector<cocos2d::SpriteFrame*> animFrames;
	char str[100] = {0};
	for (int i = 1; i < 5; i++){
		sprintf(str, "reddragon_%i.png", i);
		CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(str);
		animFrames.pushBack(frame);
	}
	//设置飞翔动画
	CCAnimation* animation = CCAnimation::createWithSpriteFrames(animFrames, 0.2);
	animation->setLoops(-1);
	auto animationAct = CCAnimate::create(animation);
	animationAct->setTag(RedDragonFlyTag);
	
	reddragonsp->runAction(animationAct);

	
}

void HelloWorld::enterGameCallback(Ref* pSender){ 
	CocosDenshion::SimpleAudioEngine::sharedEngine()->stopAllEffects();
	auto scene = GameLayer::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(0.5, scene, Color3B(50,50,50)));
}

void HelloWorld::exitGameCallback(Ref* pSender){
	Director::getInstance()->end();
}