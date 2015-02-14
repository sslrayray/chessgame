#include "cocos2d.h"
#include "ActorSprite.h"
#include "GameMap.h"

//extern CCTMXTiledMap* _tileMap;
extern GameMap* _tileMap;
extern ActorSprite* _currentActor;
extern bool isGezi;
extern bool isAnimate;
extern Sprite* _blockroot;


extern bool TouchInTarget(Touch* touch, Event* event);
extern Vec2 maptoCoor(Vec2 mappoint);
extern Vec2 coortoMap(Vec2 corrpoint);
extern Vec2 getXYofNode(Node* p);


extern ActorSprite* targetAI;

const Vec2 up = Vec2(0, 1);
const Vec2 down = Vec2(0, -1);
const Vec2 left = Vec2(-1, 0);
const Vec2 right = Vec2(1, 0);

extern bool gameoverFlag;
extern bool gamewinFlag;