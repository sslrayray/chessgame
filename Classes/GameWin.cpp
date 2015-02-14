#include "GameWin.h"
#include "GameLayer.h"

USING_NS_CC;


Scene* GameWin::createScene()
{
    // 'scene' is an autorelease object

	auto scene = Scene::create();
    
    // 'layer' is an autorelease object
	auto layer = GameWin::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

bool GameWin::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
	
	
	Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
	//背景图
    auto background = Sprite::create("gamewin.png");
	background->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
	this->addChild(background, 0);
	
	//进入菜单
	

	//设置点击图层的鼠标监听
	auto listener =	EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [](Touch* touch, Event* event){
		return true;
	};
	listener->onTouchEnded = [](Touch* touch, Event* event){
		CocosDenshion::SimpleAudioEngine::sharedEngine()->stopAllEffects();
		auto scene = GameLayer::createScene();
		Director::getInstance()->replaceScene(TransitionFade::create(0.5, scene, Color3B(50,50,50)));
	};
	//加入到事件分发器中
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}