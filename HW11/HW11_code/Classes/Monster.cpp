#include"Monster.h"
USING_NS_CC;

Factory* Factory::factory = NULL;
Factory::Factory() {
	initSpriteFrame();
}
Factory* Factory::getInstance() {
	if (factory == NULL) {
		factory = new Factory();
	}
	return factory;
}
void Factory::initSpriteFrame(){
	auto texture = Director::getInstance()->getTextureCache()->addImage("Monster.png");
	monsterDead.reserve(4);
	for (int i = 0; i < 4; i++) {
		auto frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(258-48*i,0,42,42)));
		monsterDead.pushBack(frame);
	}
}

Sprite* Factory::createMonster() {
	Sprite* mons = Sprite::create("Monster.png", CC_RECT_PIXELS_TO_POINTS(Rect(364,0,40,40)));
	monster.pushBack(mons);
	return mons;
}

void Factory::removeMonster(Sprite* sp) {
	Animation* animation = Animation::create();
	for (int i = 0; i < monsterDead.size(); i++)
		animation->addSpriteFrame(monsterDead.at(i));
	animation->setDelayPerUnit(0.5f / 4.0f);
	auto action = Animate::create(animation);
	auto sequence = Sequence::create(action, 
		CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, sp)), NULL);
	sp->runAction(sequence);
	monster.eraseObject(sp);
}
void Factory::moveMonster(Vec2 playerPos,float time){
	Vector<Sprite*>::iterator it = monster.begin();
	for (; it != monster.end(); it++) {
		Vec2 mosterPos = (*it)->getPosition();
		Vec2 direction = playerPos - mosterPos;
		direction.normalize();
		(*it)->runAction(MoveBy::create(time, direction * 30));
	}
}

Sprite* Factory::collider(Rect rect) {
	Vector<Sprite*>::iterator it = monster.begin();
	for (; it != monster.end(); it++) {
		if ((*it)->getBoundingBox().intersectsRect(rect)) {
			break;
		}
	}
	if (it == monster.end())
		return nullptr;
	return *it;
}