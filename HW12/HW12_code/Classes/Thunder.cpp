#include "Thunder.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include <algorithm>

#include "Gameover.h"


USING_NS_CC;

using namespace CocosDenshion;

Scene* Thunder::createScene() {
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = Thunder::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

bool Thunder::init() {
    if ( !Layer::init() ) {
        return false;
    }

    visibleSize = Director::getInstance()->getVisibleSize();

    auto bgsprite = Sprite::create("background.jpg");
    bgsprite->setPosition(visibleSize / 2);
    // bgsprite->setScale(visibleSize.width / bgsprite->getContentSize().width, \
    //     visibleSize.height / bgsprite->getContentSize().height);
    this->addChild(bgsprite, 0);

    player = Sprite::create("player.png");
    player->setPosition(visibleSize.width / 2, player->getContentSize().height + 5);
    player->setName("player");
    this->addChild(player, 1);

	bullets.reserve(10);

    addEnemy(5);

    preloadMusic();
    playBgm();

    addTouchListener();
    addKeyboardListener();
    addCustomListener();

    // TODO
    // add schedule
	schedule(schedule_selector(Thunder::update), 0.01f, kRepeatForever, 0);
    
    return true;
}

void Thunder::preloadMusic() {
    // TODO
	auto audio = SimpleAudioEngine::getInstance();
	audio->preloadBackgroundMusic("music/bgm.mp3");
	audio->preloadBackgroundMusic("music/explore.wav");
	audio->preloadBackgroundMusic("music/fire.wav");

	
}

void Thunder::playBgm() {
    // TODO 
	auto audio = SimpleAudioEngine::getInstance();
	audio->playBackgroundMusic("music/bgm.mp3", true);
}

void Thunder::addEnemy(int n) {
    enemys.resize(n * 3);
    for(unsigned i = 0; i < 3; ++i) {
        char enemyPath[20];
        sprintf(enemyPath, "stone%d.png", 3 - i);
        double width  = (visibleSize.width - 20) / (n + 1.0),
               height = visibleSize.height - (50 * (i + 1));
        for(int j = 0; j < n; ++j) {
            auto enemy = Sprite::create(enemyPath);
            enemy->setAnchorPoint(Vec2(0.5, 0.5));
            enemy->setScale(0.5, 0.5);
            enemy->setPosition(width * (j + 1), height);
            enemys[i * n + j] = enemy;
			addChild(enemy);
        }
    }
}

void Thunder::addTouchListener(){
    // 
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(Thunder::onTouchBegan, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(Thunder::onTouchMoved, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(Thunder::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, player);
}

void Thunder::addKeyboardListener() {
    // TODO
    // 
	auto keyListener = EventListenerKeyboard::create();
	keyListener->onKeyPressed = CC_CALLBACK_2(Thunder::onKeyPressed, this);
	keyListener->onKeyReleased = CC_CALLBACK_2(Thunder::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyListener, player);


}

void Thunder::update(float f) {
    // TODO
	if (player->getPositionX() + move - player->getContentSize().width / 2> 0 && 
		player->getPositionX() + move + player->getContentSize().width / 2< visibleSize.width)
		player->setPosition(player->getPosition() + Vec2(move, 0));
	
	static double count = 0;
	static int dir = 1;
	count += f;
	if (count > 1) { count = 0.0; dir = (dir + 1) % 3; }

	
	for (int j = 0; j < bullets.size(); j++) {
		if (bullets[j] != NULL) {
			bullets[j]->setPosition(bullets[j]->getPositionX(), bullets[j]->getPositionY() + 5);
			if (bullets[j]->getPositionY() > visibleSize.height - 10) {
 				bullets[j]->removeFromParentAndCleanup(true);
				bullets.erase(bullets.begin() + j);
				j--;
			}
		}
	}
	
	for (unsigned i = 0; i < enemys.size(); i++) {
		if (enemys[i] != NULL) {
			if (dir == 0)
				enemys[i]->setPosition(enemys[i]->getPosition() + Vec2(1, 0));
			else if (dir == 1)
				enemys[i]->setPosition(enemys[i]->getPosition() + Vec2(-1, 0));
			else
				enemys[i]->setPosition(enemys[i]->getPosition() + Vec2(0, -1));
			for (int j = 0; j < bullets.size(); j++) {
				if (bullets[j] != NULL && enemys[i] != NULL &&
					bullets[j]->getPosition().getDistance(enemys[i]->getPosition()) < 30) {
					bullets[j]->removeFromParentAndCleanup(true);
					bullets.erase(bullets.begin() + j);
					j--;
					EventCustom e("meet");
					e.setUserData(&i);
					_eventDispatcher->dispatchEvent(&e);
				}
			}
			if (enemys[i] != NULL && player->getPosition().getDistance(enemys[i]->getPosition()) < 30) {
				EventCustom f("meet");
				f.setUserData(&i);
				_eventDispatcher->dispatchEvent(&f);
				_eventDispatcher->removeAllEventListeners();
				auto sence = Gameover::createScene();
				Director::getInstance()->replaceScene(sence);
			}
		}
	}
}

void Thunder::fire() {
   
    auto bullet = Sprite::create("bullet.png");
    bullet->setPosition(player->getPosition());
	addChild(bullet);
	bullets.insert(bullets.begin(), bullet);

	auto audio = SimpleAudioEngine::getInstance();
	audio->playEffect("music/fire.wav");
}

void Thunder::addCustomListener() {
    // TODO
	auto meetListener = EventListenerCustom::create("meet", CC_CALLBACK_1(Thunder::meet, this));
	_eventDispatcher->addEventListenerWithFixedPriority(meetListener, 1);
}

bool Thunder::onTouchBegan(Touch *touch, Event *unused_event) {
    //
	auto touchPos = touch->getLocation();
	auto playerPos = player->getPosition();
	auto playerSize = player->getContentSize();
	auto rect = Rect(playerPos.x - playerSize.width / 2, playerPos.y - playerSize.height / 2,
		playerSize.width, playerSize.height);
	if (rect.containsPoint(touchPos))
		return true;
	return false;
}

void Thunder::onTouchMoved(Touch *touch, Event *unused_event) {
    //
	auto touchPos = touch->getLocation();
	player->setPosition(touchPos);
}

void Thunder::onTouchEnded(Touch *touch, Event *unused_event) {
    //
	fire();
}

void Thunder::onKeyPressed(EventKeyboard::KeyCode code, Event* event) {
    switch (code) {
        case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
        case cocos2d::EventKeyboard::KeyCode::KEY_A:
			if (player->getPositionX() - 1 - player->getContentSize().width / 2> 0 && 
				player->getPositionX() -1 + player->getContentSize().width / 2< visibleSize.width)
				player->setPosition(player->getPositionX() - 1, player->getPositionY());
			move -= 5;
            // TODO
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
        case cocos2d::EventKeyboard::KeyCode::KEY_D:
			if (player->getPositionX() + 1 - player->getContentSize().width / 2> 0 && 
				player->getPositionX() + 1 + player->getContentSize().width / 2< visibleSize.width)
				player->setPosition(player->getPositionX() + 1, player->getPositionY());
			move += 5;
            // TODO
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_SPACE:
            fire();
            break;
        default:
            break;
    }
}

void Thunder::onKeyReleased(EventKeyboard::KeyCode code, Event* event) {
	// TODO
	move = 0;
}

void Thunder::meet(EventCustom* event) {
	
	int index = *(int*)event->getUserData();
	auto audio = SimpleAudioEngine::getInstance();
	audio->playEffect("music/explore.wav");
	enemys[index]->removeFromParentAndCleanup(true);
	enemys[index] = NULL;
}