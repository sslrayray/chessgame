#include "GameMap.h"
#include "res.h"
#include "enemySprite.h"

ActorSprite* targetAI = nullptr;

GameMap::GameMap(void)
{
	//sGlobal->gameMap = this;
}

GameMap::~GameMap(void)
{
	this->unscheduleAllSelectors();
}

GameMap* GameMap::gameMapWithTMXFile(const char *tmxFile)
{
	GameMap *pRet = new GameMap();

	if (pRet->initWithTMXFile(tmxFile))
	{
		pRet->extraInit();
		pRet->autorelease();
		return pRet;
	}

	CC_SAFE_DELETE(pRet);
	return NULL;
}

void GameMap::extraInit()
{
	floorLayer = this->getLayer("floor");
	initWall();
	initActor();
}

void GameMap::initWall()
{
	wallLayer = this->getLayer("wall");

	Size s = wallLayer->getLayerSize();
	mapHeight = s.height;
	mapWidth = s.width; 

	for (int x=0; x<s.width; x++)
	{
		for (int y=0; y<s.height; y++)
		{
			int gid = wallLayer->getTileGIDAt(Point(x, y));
			if (gid)
			{
				Vec2 pos = Vec2(x, s.height-1-y);
				wallArray.insert(pos);
			}
		}
	}
}

void GameMap::initActor()
{
	TMXObjectGroup* group = this->objectGroupNamed("actor");

	const ValueVector &objects = group->getObjects();

	for (ValueVector::const_iterator it=objects.begin(); it!=objects.end(); it++)
	{
		const ValueMap &dict = (*it).asValueMap();
		
		int x = dict.at("x").asInt();
		int y = dict.at("y").asInt();
		Vec2 pos = Vec2(x, y);
		std::string texture = dict.at("texture").asString();
		
		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(texture+".plist");
		
		if (dict.at("enemy").asInt() == 0){
			auto actor = ActorSprite::MyCreateInit(texture);
			//初始化属性值
			actor->atk = dict.at("atk").asInt();
			actor->Tag = dict.at("tag").asInt();
			actor->life = dict.at("life").asInt();
			actor->range = dict.at("range").asInt();
			//增加了属性
			actor->atkrange = dict.at("atkrange").asInt();
			//


			
			auto listener = EventListenerTouchOneByOne::create();
			listener->onTouchBegan = CC_CALLBACK_2(ActorSprite::onTouchBegan, actor);
			listener->onTouchEnded = CC_CALLBACK_2(ActorSprite::onTouchEnded, actor);
			listener->setSwallowTouches(true);
			_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, actor);
			//-----------------------改了，添加监听
			actor->setPosition(pos+_tileSize/2);
			addChild(actor, 4);
			//增加血条
			Sprite* bloodsp = Sprite::create("blood.png");
			
			bloodsp->setScaleX(actor->life);
			bloodsp->setPosition(Vec2(actor->getContentSize().width/2, actor->getContentSize().height));
			actor->addChild(bloodsp);
			bloodsp->setTag(actor->Tag);
			//增加血条

			//把人物存放到容器中
			friendArray.pushBack(actor);
			actor->setName(actor->ActorName);
		}else{
			auto actor = EnemySprite::MyCreateInit(texture);
			actor->setOpacity(100);
			//初始化属性值
			actor->atk = dict.at("atk").asInt();
			actor->Tag = dict.at("tag").asInt();
			actor->life = dict.at("life").asInt();
			actor->range = dict.at("range").asInt();
			actor->atkrange = dict.at("atkrange").asInt();
			//
			
			auto listener = EventListenerTouchOneByOne::create();
			listener->setSwallowTouches(true);
			listener->onTouchBegan = CC_CALLBACK_2(EnemySprite::onTouchBegan, actor);
			listener->onTouchEnded = CC_CALLBACK_2(EnemySprite::onTouchEnded, actor);
			listener->setSwallowTouches(true);
			_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, actor);
			//--------------------改了，添加监听
			actor->setPosition(pos+_tileSize/2);
			addChild(actor, 4);

			Sprite* bloodsp = Sprite::create("blood.png");
			
			bloodsp->setScaleX(actor->life);
			bloodsp->setPosition(Vec2(actor->getContentSize().width/2, actor->getContentSize().height));
			actor->addChild(bloodsp);
			bloodsp->setTag(actor->Tag);

			//增加坏人存入vector中
			enemyArray.pushBack(actor);
			actor->setName(actor->ActorName);
		}
		
		
		

		int width = dict.at("width").asInt();
		int height = dict.at("height").asInt();
		x /= width;
		y /= height;
		pos = Vec2(x, y);
		actorArray.insert(pos);
	}
}

//new

ActorSprite* GameMap::getFriendByPos(Vec2 pos)
{
	for (auto i : friendArray)
	{
		if (getXYofNode(i) == pos)
		{
			return i;
		}
	}
	return NULL;
}

EnemySprite* GameMap::getEnemyByPos(Vec2 pos)
{
	for (auto i : enemyArray)
	{
		if (getXYofNode(i) == pos)
		{
			return i;
		}
	}
	return NULL;
}
//----



std::vector<Vec2> GameMap::getPath(Vec2 src, Vec2 dst, int maxStepCount)
{
	std::vector<Vec2> bfs;
	std::vector<int> stepCount;
	std::set<Vec2> flag;
	std::vector<Vec2> path;

	bfs.push_back(src);
	stepCount.push_back(0);
	flag.insert(src);
	path.push_back(Vec2(0, 0));

	Vec2 tmp;
	int index = 0;
	while (index < bfs.size())
	{
		if (stepCount.at(index) == maxStepCount)
			break;
		tmp = bfs.at(index)+up;
		if ((tmp.y<mapHeight) && (flag.count(tmp)==0) && (wallArray.count(tmp)==0) && (actorArray.count(tmp)==0))
		{
			bfs.push_back(tmp);
			stepCount.push_back(stepCount.at(index)+1);
			flag.insert(tmp);
			path.push_back(up);
			if (tmp == dst)
				break;
		}
		tmp = bfs.at(index)+down;
		if ((tmp.y>=0) && (flag.count(tmp)==0) && (wallArray.count(tmp)==0) && (actorArray.count(tmp)==0))
		{
			bfs.push_back(tmp);
			stepCount.push_back(stepCount.at(index)+1);
			flag.insert(tmp);
			path.push_back(down);
			if (tmp == dst)
				break;
		}
		tmp = bfs.at(index)+left;
		if ((tmp.x>=0) && (flag.count(tmp)==0) && (wallArray.count(tmp)==0) && (actorArray.count(tmp)==0))
		{
			bfs.push_back(tmp);
			stepCount.push_back(stepCount.at(index)+1);
			flag.insert(tmp);
			path.push_back(left);
			if (tmp == dst)
				break;
		}
		tmp = bfs.at(index)+right;
		if ((tmp.x<mapWidth) && (flag.count(tmp)==0) && (wallArray.count(tmp)==0) && (actorArray.count(tmp)==0))
		{
			bfs.push_back(tmp);
			stepCount.push_back(stepCount.at(index)+1);
			flag.insert(tmp);
			path.push_back(right);
			if (tmp == dst)
				break;
		}
		index++;
	}

	std::vector<Vec2> seq;
	if (flag.count(dst))
	{
		index=bfs.size()-1;
		tmp = dst;
		while (tmp != src)
		{
			while (bfs.at(index) != tmp)
			{
				index--;
			}
			seq.insert(seq.begin(), path.at(index));
			tmp = tmp - path.at(index);
		}
		actorArray.erase(src);
		actorArray.insert(dst);

	}
	return seq;
}

void GameMap::getMoveRange(Vec2 pos, int maxMoveRange)
{
	std::vector<Vec2> bfs;
	std::vector<int> stepCount;
	std::set<Vec2> flag;

	bfs.push_back(pos);
	stepCount.push_back(0);
	flag.insert(pos);

	Vec2 tmp;
	int index = 0;
	while (index < bfs.size())
	{
		if (stepCount.at(index) == maxMoveRange)
			break;
		tmp = bfs.at(index)+up;
		if ((tmp.y<mapHeight) && (flag.count(tmp)==0) && (wallArray.count(tmp)==0) && (actorArray.count(tmp)==0))
		{
			bfs.push_back(tmp);
			stepCount.push_back(stepCount.at(index)+1);
			flag.insert(tmp);
			setRange(tmp, "blue");
		}
		tmp = bfs.at(index)+down;
		if ((tmp.y>=0) && (flag.count(tmp)==0) && (wallArray.count(tmp)==0) && (actorArray.count(tmp)==0))
		{
			bfs.push_back(tmp);
			stepCount.push_back(stepCount.at(index)+1);
			flag.insert(tmp);
			setRange(tmp, "blue");
		}
		tmp = bfs.at(index)+left;
		if ((tmp.x>=0) && (flag.count(tmp)==0) && (wallArray.count(tmp)==0) && (actorArray.count(tmp)==0))
		{
			bfs.push_back(tmp);
			stepCount.push_back(stepCount.at(index)+1);
			flag.insert(tmp);
			setRange(tmp, "blue");
		}
		tmp = bfs.at(index)+right;
		if ((tmp.x<mapWidth) && (flag.count(tmp)==0) && (wallArray.count(tmp)==0) && (actorArray.count(tmp)==0))
		{
			bfs.push_back(tmp);
			stepCount.push_back(stepCount.at(index)+1);
			flag.insert(tmp);
			setRange(tmp, "blue");
		}
		index++;
	}
}

void GameMap::getAttackRange(Vec2 pos, int maxAttackRange)
{
	std::vector<Vec2> bfs;
	std::vector<int> stepCount;
	std::set<Vec2> flag;

	bfs.push_back(pos);
	stepCount.push_back(0);
	flag.insert(pos);

	Vec2 tmp;
	int index = 0;
	while (index < bfs.size())
	{
		if (stepCount.at(index) == maxAttackRange)
			break;
		tmp = bfs.at(index)+up;
		if ((tmp.y<mapHeight) && (flag.count(tmp)==0) && (wallArray.count(tmp)==0))
		{
			bfs.push_back(tmp);
			stepCount.push_back(stepCount.at(index)+1);
			flag.insert(tmp);
			if (getEnemyByPos(tmp) != NULL)
				setRange(tmp, "red");
		}
		tmp = bfs.at(index)+down;
		if ((tmp.y>=0) && (flag.count(tmp)==0) && (wallArray.count(tmp)==0))
		{
			bfs.push_back(tmp);
			stepCount.push_back(stepCount.at(index)+1);
			flag.insert(tmp);
			if (getEnemyByPos(tmp) != NULL)
				setRange(tmp, "red");
		}
		tmp = bfs.at(index)+left;
		if ((tmp.x>=0) && (flag.count(tmp)==0) && (wallArray.count(tmp)==0))
		{
			bfs.push_back(tmp);
			stepCount.push_back(stepCount.at(index)+1);
			flag.insert(tmp);
			if (getEnemyByPos(tmp) != NULL)
				setRange(tmp, "red");
		}
		tmp = bfs.at(index)+right;
		if ((tmp.x<mapWidth) && (flag.count(tmp)==0) && (wallArray.count(tmp)==0))
		{
			bfs.push_back(tmp);
			stepCount.push_back(stepCount.at(index)+1);
			flag.insert(tmp);
			if (getEnemyByPos(tmp) != NULL)
				setRange(tmp, "red");
		}
		index++;
	}
}

void GameMap::setRange(Vec2 pos, std::string colorName)
{
	if (colorName.compare("blue") == 0)
	{
		auto colornode = BlueBlock::MyCreateInit();
		colornode->setPosition((pos.x+0.5)*_tileMap->getTileSize().width, 
			(pos.y+0.5)*_tileMap->getTileSize().height);
		colornode->setOpacity(100);
		_blockroot->addChild(colornode, 0);
				
		//设置移动格子的回调函数

		auto gezilistener = EventListenerTouchOneByOne::create();

		gezilistener->setSwallowTouches(true);//触摸不会向下传递
		gezilistener->onTouchBegan = CC_CALLBACK_2(BlueBlock::onTouchBegan, colornode);
		gezilistener->onTouchEnded = CC_CALLBACK_2(BlueBlock::onTouchEnded, colornode);
	
		//auto p = colornode->getEventDispatcher();
		_eventDispatcher->addEventListenerWithSceneGraphPriority(gezilistener, colornode);
	}
	else
	{
		auto colornode = RedBlock::MyCreateInit();
		colornode->setPosition((pos.x+0.5)*_tileMap->getTileSize().width, 
			(pos.y+0.5)*_tileMap->getTileSize().height);
		colornode->setOpacity(100);
		_blockroot->addChild(colornode, 0);
	}
}

std::vector<Vec2> GameMap::actionAI(Vec2 pos, int maxMoveRange, int maxAttackRange)
{
	targetAI = NULL;
	std::vector<Vec2> bfs;
	std::vector<int> stepCount;
	std::set<Vec2> flag;
	std::vector<Vec2> path;

	bfs.push_back(pos);
	stepCount.push_back(0);
	flag.insert(pos);
	path.push_back(Vec2(0, 0));

	Vec2 tmp;
	int index = 0;
	while (index < bfs.size())
	{
		if ((stepCount.at(index) < maxMoveRange+maxAttackRange))
		{
			tmp = bfs.at(index)+up;
			if ((tmp.y<mapHeight) && (wallArray.count(tmp)==0))
			{
				if (flag.count(tmp)==0)
				{
					bfs.push_back(tmp);
					int step = stepCount.at(index);
					if (actorArray.count(tmp))
					{
						if (step < maxMoveRange)
						{
							step = maxMoveRange;
						}
					}
					stepCount.push_back(step+1);
					flag.insert(tmp);
					path.push_back(up);
				}
				else
				{
					if (actorArray.count(tmp) == 0)
					{
						int i = bfs.size()-1;
						while (bfs.at(i) != tmp)
						{
							i--;
						}
						if (stepCount.at(i) > stepCount.at(index)+1)
						{
							stepCount[i] = stepCount.at(index)+1;
							path[i] = up;
						}
					}
				}
			}
			tmp = bfs.at(index)+down;
			if ((tmp.y>=0) && (wallArray.count(tmp)==0))
			{
				if (flag.count(tmp)==0)
				{
					bfs.push_back(tmp);
					int step = stepCount.at(index);
					if (actorArray.count(tmp))
					{
						if (step < maxMoveRange)
						{
							step = maxMoveRange;
						}
					}
					stepCount.push_back(step+1);
					flag.insert(tmp);
					path.push_back(down);
				}
				else
				{
					if (actorArray.count(tmp) == 0)
					{
						int i = bfs.size()-1;
						while (bfs.at(i) != tmp)
						{
							i--;
						}
						if (stepCount.at(i) > stepCount.at(index)+1)
						{
							stepCount[i] = stepCount.at(index)+1;
							path[i] = down;
						}
					}
				}
			}
			tmp = bfs.at(index)+left;
			if ((tmp.x>=0) && (wallArray.count(tmp)==0))
			{
				if (flag.count(tmp)==0)
				{
					bfs.push_back(tmp);
					int step = stepCount.at(index);
					if (actorArray.count(tmp))
					{
						if (step < maxMoveRange)
						{
							step = maxMoveRange;
						}
					}
					stepCount.push_back(step+1);
					flag.insert(tmp);
					path.push_back(left);
				}
				else
				{
					if (actorArray.count(tmp) == 0)
					{
						int i = bfs.size()-1;
						while (bfs.at(i) != tmp)
						{
							i--;
						}
						if (stepCount.at(i) > stepCount.at(index)+1)
						{
							stepCount[i] = stepCount.at(index)+1;
							path[i] = left;
						}
					}
				}
			}
			tmp = bfs.at(index)+right;
			if ((tmp.x<mapWidth) && (wallArray.count(tmp)==0))
			{
				if (flag.count(tmp)==0)
				{
					bfs.push_back(tmp);
					int step = stepCount.at(index);
					if (actorArray.count(tmp))
					{
						if (step < maxMoveRange)
						{
							step = maxMoveRange;
						}
					}
					stepCount.push_back(step+1);
					flag.insert(tmp);
					path.push_back(right);
				}
				else
				{
					if (actorArray.count(tmp) == 0)
					{
						int i = bfs.size()-1;
						while (bfs.at(i) != tmp)
						{
							i--;
						}
						if (stepCount.at(i) > stepCount.at(index)+1)
						{
							stepCount[i] = stepCount.at(index)+1;
							path[i] = right;
						}
					}
				}
			}
		}

		tmp = bfs.at(index);
		if (actorArray.count(tmp))
		{
			ActorSprite *actor = getFriendByPos(tmp);
			if (actor != NULL)
			{
				targetAI = actor;
				break;
			}
		}
		index++;
	}

	int cnt = 0;
	std::vector<Vec2> seq;
	if (targetAI != NULL)
	{
		index=bfs.size()-1;
		tmp = getXYofNode(targetAI);
		while (tmp != pos)
		{
			while (bfs.at(index) != tmp)
			{
				index--;
			}
			if (cnt == maxAttackRange)
			{
				actorArray.erase(pos);
				actorArray.insert(tmp);
			}
			if (cnt >= maxAttackRange)
			{
				seq.insert(seq.begin(), path.at(index));
			}
			tmp = tmp - path.at(index);
			cnt++;
		}
	}
	return seq;
}