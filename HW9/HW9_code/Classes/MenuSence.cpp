	#include "MenuSence.h"
#include "GameSence.h"
USING_NS_CC;

Scene* MenuSence::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = MenuSence::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool MenuSence::init()
{

    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto bg_sky = Sprite::create("menu-background-sky.jpg");
	bg_sky->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y + 150));
	this->addChild(bg_sky, 0);

	auto bg = Sprite::create("menu-background.png");
	bg->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y - 60));
	this->addChild(bg, 0);

	auto miner = Sprite::create("menu-miner.png");
	miner->setPosition(Vec2(150 + origin.x, visibleSize.height / 2 + origin.y - 60));
	this->addChild(miner, 1);

	auto leg = Sprite::createWithSpriteFrameName("miner-leg-0.png");
	Animate* legAnimate = Animate::create(AnimationCache::getInstance()->getAnimation("legAnimation"));
	leg->runAction(RepeatForever::create(legAnimate));
	leg->setPosition(110 + origin.x, origin.y + 102);
	this->addChild(leg, 1);

	auto face = Sprite::createWithSpriteFrameName("miner-face-normal.png");
	Animate* faceAnimate = Animate::create(AnimationCache::getInstance()->getAnimation("faceAnimation"));
	face->runAction(RepeatForever::create(faceAnimate));
	face->setPosition(162 + origin.x, 365 + origin.y);
	this->addChild(face, 2);

	auto gm_text = Sprite::create("gold-miner-text.png");
	gm_text->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y + 180));
	this->addChild(gm_text, 1);

	auto ms_gold = Sprite::create("menu-start-gold.png");
	ms_gold->setPosition(Vec2(visibleSize.width + origin.x - 180, origin.y + 180));
	this->addChild(ms_gold, 1);

	auto startItem = MenuItemImage::create("start-0.png",
										   "start-1.png",
		                                    [](Ref* sender) {
		auto sence = GameSence::createScene();
		Director::getInstance()->replaceScene(sence);
						});
	startItem->setPosition(Vec2(visibleSize.width + origin.x - 180, origin.y + 220));
	auto menu = Menu::create(startItem, NULL);
	menu->setPosition(Point(0, 0));
	this->addChild(menu, 3);
    return true;
}

