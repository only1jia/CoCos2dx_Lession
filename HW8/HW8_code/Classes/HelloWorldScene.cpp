#include "HelloWorldScene.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
	
    auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
	auto studentId = Label::createWithTTF("14331388", "fonts/Marker Felt.ttf", 24);
#pragma execution_character_set("utf-8"); //解决中文不乱码
	auto studentName = Label::createWithSystemFont("郑泽佳", "Microsoft Yahei", 24);

    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

	studentId->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height - studentId->getContentSize().height - label->getContentSize().height));

	studentName->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height - studentId->getContentSize().height - label->getContentSize().height -
		studentName->getContentSize().height));
    // add the label as a child to this layer
    this->addChild(label, 2);
	this->addChild(studentId, 3);
	this->addChild(studentName, 4);

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("background.jpg");
	

    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);
	// add a menu item
	auto item = MenuItemToggle::createWithCallback([&](Ref*) { log("Toggle touched!"); }, 
		MenuItemFont::create("On"), MenuItemFont::create("Off"), NULL);
	// set the font of the item
	MenuItemFont::setFontName("Marker Felt");
	MenuItemFont::setFontSize(40);
	// position the item
	item->setPosition(Point(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 
		- item->getContentSize().height));
	//create menu, it's an autorelease object
	auto menu1 = Menu::create(item, NULL);
	menu1->setPosition(Point(0, 0));
	//add the menu1 as a child to this layer
	addChild(menu1);
    
    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
