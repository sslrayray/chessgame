#include "GameLayer.h"
#include "blueblock.h"
#include "ActorSprite.h"
#include "myActionManager.h"
#include "res.h"
#include "GameMap.h"
#include "GameOver.h"
#include "GameWin.h"

USING_NS_CC;


enum{
	RedDragonspTag,
	RedDragonActTag,
	LeaderspTag,
	LeaderActTag
};

GameMap* _tileMap = nullptr;
//CCTMXTiledMap* _tileMap = nullptr;
ActorSprite* _currentActor = nullptr;
Sprite* _blockroot = nullptr;

bool isGezi = false;
bool isAnimate = false;
bool gameoverFlag = false;
bool gamewinFlag = false;

//增加
Vec2 preTouch;//用来判断是单击而不是滑动，判断抬起的时候和触摸的时候的距离
//增加


Scene* GameLayer::createScene(){
	auto scene = Scene::create();
	auto layer = GameLayer::create();//调用init初始化
	scene->addChild(layer);
	return scene;
}

//初始化GameLayerLayer
bool GameLayer::init(){
	if (!Layer::init()){
		return false;
	}

	_tileMap = nullptr;
	//CCTMXTiledMap* _tileMap = nullptr;
	_currentActor = nullptr;
	_blockroot = nullptr;

	isGezi = false;
	isAnimate = false;
	gameoverFlag = false;
	gamewinFlag = false;


	this->schedule(schedule_selector(GameLayer::testUpdate));  
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("game_bgm.mp3"); 
	

	Size VisibleSize = Director::getInstance()->getVisibleSize();


	_tileMap = GameMap::gameMapWithTMXFile("desert.tmx");
	addChild(_tileMap, 0);
	//设置移动格子的根节点
	_blockroot = Sprite::createWithSpriteFrameName("transparent.png");
	_blockroot->setPosition(_blockroot->getContentSize()/2);
	_tileMap->addChild(_blockroot, 3);
	//设置当前人物
	//新建一个人物类,人物是leader类型（蓝毛）
	//auto leader = ActorSprite::MyCreateInit("leader");
	//_currentActor = leader;


	//设置地图的鼠标监听
	auto maplistener = EventListenerTouchOneByOne::create();
	
	maplistener->onTouchBegan = [](Touch* touch, Event* event){
		preTouch = touch->getLocation();
		return true;
	};

	maplistener->onTouchMoved = [](Touch* touch, Event* event){
		auto target = event->getCurrentTarget();
		target->setPosition(target->getPosition()+Vec2(0, touch->getDelta().y));
		Point mapsize =target->getContentSize();
		Point visiblesize = Director::getInstance()->getVisibleSize();
		if (target->getPosition().y > 0 || target->getPosition().y < (visiblesize.y - mapsize.y))
			target->setPosition(target->getPosition() - Vec2(0, touch->getDelta().y));
	};

	maplistener->onTouchEnded = [](Touch* touch, Event* event){
		float a = touch->getLocation().distance(preTouch);
		if (touch->getLocation().distance(preTouch)< 20){
			if (isGezi){
			isGezi = false;
			_blockroot->removeAllChildren();
			}
			CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("btn.wav"); 
		}
	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(maplistener, _tileMap);

	//设置结束
	std::vector<Vec2> walkable;
	walkable.push_back(Vec2(7, 1));
	walkable.push_back(Vec2(6, 1));

	std::vector<Vec2> attackable;
	attackable.push_back(Vec2(7, 0));
	attackable.push_back(Vec2(6, 0));

	
	//设置回合结束的按钮
	auto endMissionItem = MenuItemImage::create("endmission.png",
                                           "endmission.png",
										   CC_CALLBACK_1(GameLayer::MissionEnd, this));
	endMissionItem->setPosition(VisibleSize.width - endMissionItem->getContentSize().width, 
								VisibleSize.height - endMissionItem->getContentSize().height);
	endMissionItem->setScale(2);

	


	//退出的按钮
	
	auto closeItem = MenuItemImage::create("CloseNormal.png",
                                           "CloseSelected.png",
										   CC_CALLBACK_1(GameLayer::memuTurnBack, this));
	closeItem->setPosition(VisibleSize.width-2*closeItem->getContentSize().width, 2*closeItem->getContentSize().height);
	closeItem->setScale(2);
	auto menu = Menu::create(closeItem, endMissionItem, nullptr);
	
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);
	

	return true;
}


//退出游戏
void GameLayer::memuTurnBack(cocos2d::Ref* pSender){
	Director::getInstance()->end();
}

void GameLayer::MissionEnd(Ref* pSender){
	if (isAnimate)
		return;
	if (_tileMap->enemyArray.size() > 0){
		_tileMap->enemyArray.at(0)->showTime();
	}
	//Director::getInstance()->end();
	update(1);
}

void GameLayer::testUpdate(float dt){
	if (gameoverFlag){
		CocosDenshion::SimpleAudioEngine::sharedEngine()->stopAllEffects();
		auto scene = GameOver::createScene();
		Director::getInstance()->replaceScene(TransitionFade::create(0.5, scene, Color3B(50,50,50)));
		gameoverFlag = false;
	}

	if (gamewinFlag){
		CocosDenshion::SimpleAudioEngine::sharedEngine()->stopAllEffects();
		auto scene = GameWin::createScene();
		Director::getInstance()->replaceScene(TransitionFade::create(0.5, scene, Color3B(50,50,50)));
		gamewinFlag = false;
	}
}