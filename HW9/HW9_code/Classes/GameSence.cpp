#include "GameSence.h"

USING_NS_CC;

Scene* GameSence::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = GameSence::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool GameSence::init()
{

	if (!Layer::init())
	{
		return false;
	}

	//add touch listener
	EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(GameSence::onTouchBegan, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);


	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto level_bg = Sprite::create("level-background-0.jpg");
	level_bg->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(level_bg, 0);

	//mouseLayer
	mouseLayer = Layer::create();
	mouseLayer->ignoreAnchorPointForPosition(true);
	mouseLayer->setPosition(origin.x, origin.y);

	mouse = Sprite::createWithSpriteFrameName("gem-mouse-0.png");
	Animate* mouseAnimate = Animate::create(AnimationCache::getInstance()->getAnimation("mouseAnimation"));
	mouse->runAction(RepeatForever::create(mouseAnimate));
	mouse->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
	mouseLayer->addChild(mouse, 1);

	this->addChild(mouseLayer, 1);

	//stoneLayer
	stoneLayer = Layer::create();
	stoneLayer->ignoreAnchorPointForPosition(true);
	stoneLayer->setPosition(Point(0, 0));

	stone = Sprite::create("stone.png");
	stone->setPosition(Vec2(origin.x + 560, origin.y + 480));
	stoneLayer->addChild(stone);
	this->addChild(stoneLayer, 3);
	

	

	auto shoot = MenuItemFont::create("Shoot", [&](Ref* sender) {
		auto position = mouse->getPosition();

		auto _stone = Sprite::create("stone.png");
		_stone->setPosition(Vec2(560, 480));
		this->addChild(_stone);
		auto moveTo = MoveTo::create(0.5f, position);
		auto hide = Hide::create();
		_stone->runAction(Sequence::create(moveTo, hide, NULL));

		auto mouse_moveTo = MoveTo::create(1.0f, position - Vec2(random()%100, random()%100));
		mouse->runAction(mouse_moveTo);

		auto diamond = Sprite::create("diamond.png");
		diamond->setPosition(position);
		this->addChild(diamond, 2);
	});
	shoot->setFontName("Marker Felt");
	shoot->setFontSizeObj(60);
	shoot->setPosition(Vec2(visibleSize.width - shoot->getContentSize().width + origin.x - 20, visibleSize.height + origin.y - 180));
	auto menu = Menu::create(shoot, NULL);
	menu->setPosition(Point(0, 0));
	this->addChild(menu, 4);
	
	return true;
}

bool GameSence::onTouchBegan(Touch *touch, Event *unused_event) {

	auto location = touch->getLocation();
	auto cheese = Sprite::create("cheese.png");
	cheese->setPosition(CCNode::convertToNodeSpace(location));
	mouseLayer->addChild(cheese, 0);
	auto moveTo = MoveTo::create(2, CCNode::convertToNodeSpace(location));
	auto rotateTo = RotateTo::create(2.0f, 180.0f);
	auto rotateBy = RotateBy::create(2.0f, 180.0f);	
	mouse->runAction(moveTo);
	mouse->runAction(rotateTo);
	mouse->runAction(rotateBy);

	auto fadeTo = FadeTo::create(5.0f, 0);
	cheese->runAction(fadeTo);
	return true;
}
