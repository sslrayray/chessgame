#include "cocos2d.h"
#include "res.h"

USING_NS_CC;

bool TouchInTarget(Touch* touch, Event* event){
	auto target = event->getCurrentTarget();
	Vec2 targetleftbottom = target->getPosition() - Vec2((target->getContentSize())/2);
	Rect rec = Rect(targetleftbottom.x, targetleftbottom.y, 
					target->getContentSize().width, target->getContentSize().height);
	if (rec.containsPoint(coortoMap(touch->getLocation())))
		return true;
	else
		return false;
}

Vec2 maptoCoor(Vec2 mappoint){
	return mappoint + _tileMap->getPosition();
}

Vec2 coortoMap(Vec2 corrpoint){
	return corrpoint - _tileMap->getPosition();
}

Vec2 getXYofNode(Node* p){
	return Vec2(int(p->getPosition().x/_tileMap->getTileSize().width),
				int(p->getPosition().y/_tileMap->getTileSize().height));
}