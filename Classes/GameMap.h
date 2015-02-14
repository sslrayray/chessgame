#ifndef __GAME_MAP_H__
#define __GAME_MAP_H__

#include "cocos2d.h"
#include "ActorSprite.h"
#include "enemySprite.h"
#include "blueblock.h"
#include "redblock.h"

USING_NS_CC;

class GameMap : public TMXTiledMap
{
	CC_SYNTHESIZE(TMXLayer*, floorLayer, FloorLayer);
	CC_SYNTHESIZE(TMXLayer*, wallLayer, WallLayer);

public:
	GameMap(void);
	~GameMap(void);

	static GameMap* gameMapWithTMXFile(const char *tmxFile);

	int mapHeight;
	int mapWidth;

	std::set<Vec2> wallArray;
	std::set<Vec2> actorArray;
	Vector<EnemySprite*> enemyArray;
	Vector<ActorSprite*> friendArray;

	std::vector<Vec2> getPath(Vec2 src, Vec2 dst, int maxStepCount);
	void getMoveRange(Vec2 pos, int maxMoveRange);
	void getAttackRange(Vec2 pos, int maxAttackRange);
	void setRange(Vec2 pos, std::string colorName);
	std::vector<Vec2> actionAI(Vec2 pos, int maxMoveRange, int maxAttackRange);
	
protected:
	void extraInit();
	void initWall();
	void initActor();
	ActorSprite* getFriendByPos(Vec2 pos);
	EnemySprite* getEnemyByPos(Vec2 pos);
	
};

#endif