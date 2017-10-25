#include "Gameover.h"
#include "BreakoutScene.h"

USING_NS_CC;
using namespace CocosDenshion;

cocos2d::Scene * Gameover::createScene()
{
	auto scene = Scene::create();
	auto layer = Gameover::create();
	scene->addChild(layer);
	return scene;
}

bool Gameover::init()
{
	if (!Layer::init())
		return false;
	Size visibleSize = Director::getInstance()->getVisibleSize();
	auto bg = Sprite::create("gameover.jpg");
	bg->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	addChild(bg, 0);
	auto startItem = MenuItemImage::create("start-0.png",
		"start-1.png",
		[](Ref* sender) {
		auto sence = Breakout::createScene();
		Director::getInstance()->replaceScene(TransitionFade::create(1.0f, sence));
	});
	startItem->setPosition(Vec2(visibleSize.width  - 180,  220));
	auto menu = Menu::create(startItem, NULL);
	menu->setPosition(Point(0, 0));
	this->addChild(menu, 1);

	return true;
}
