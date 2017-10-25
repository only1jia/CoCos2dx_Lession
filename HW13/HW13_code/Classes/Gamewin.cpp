#include "Gamewin.h"
#include "BreakoutScene.h"

USING_NS_CC;
using namespace CocosDenshion;

cocos2d::Scene * Gamewin::createScene()
{
	auto scene = Scene::create();

	auto layer = Gamewin::create();

	scene->addChild(layer);

	return scene;
}

bool Gamewin::init()
{
	if (!Layer::init()) {
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();

	auto background = Sprite::create("success.jpg");
	background->setPosition(visibleSize / 2);
	background->setScale(visibleSize.width / background->getContentSize().width, visibleSize.height / background->getContentSize().height);
	this->addChild(background, 0);

	auto startItem = MenuItemImage::create("start-0.png",
		"start-1.png",
		[](Ref* sender) {
		auto sence = Breakout::createScene();
		Director::getInstance()->replaceScene(TransitionFade::create(1.0f, sence));
	});
	startItem->setPosition(Vec2(visibleSize.width - 180, 220));
	auto menu = Menu::create(startItem, NULL);
	menu->setPosition(Point(0, 0));
	this->addChild(menu, 1);

	return true;
}
