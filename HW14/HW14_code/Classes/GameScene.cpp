#include "GameScene.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include <regex>
using std::regex;
using std::match_results;
using std::regex_match;
using std::cmatch;
using namespace rapidjson;

USING_NS_CC;

cocos2d::Scene* GameScene::createScene() {
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'layer' is an autorelease object
    auto layer = GameScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

bool GameScene::init() {
    if (!Layer::init())
    {
        return false;
    }

    Size size = Director::getInstance()->getVisibleSize();
    visibleHeight = size.height;
    visibleWidth = size.width;

    score_field = TextField::create("Score", "Arial", 30);
    score_field->setPosition(Size(visibleWidth / 4, visibleHeight / 4 * 3));
    this->addChild(score_field, 2);

    submit_button = Button::create();
    submit_button->setTitleText("Submit");
    submit_button->setTitleFontSize(30);
    submit_button->setPosition(Size(visibleWidth / 4, visibleHeight / 4));
    this->addChild(submit_button, 2);

	submit_button->addClickEventListener(CC_CALLBACK_0(GameScene::Submit, this));

    rank_field = TextField::create("", "Arial", 30);
    rank_field->setPosition(Size(visibleWidth / 4 * 3, visibleHeight / 4 * 3));
    this->addChild(rank_field, 2);

    rank_button = Button::create();
    rank_button->setTitleText("Rank");
    rank_button->setTitleFontSize(30);
    rank_button->setPosition(Size(visibleWidth / 4 * 3, visibleHeight / 4));
    this->addChild(rank_button, 2);

	rank_button->addClickEventListener(CC_CALLBACK_0(GameScene::Rank, this));

    return true;
}

void GameScene::Submit() {
	HttpRequest* request = new HttpRequest();
	request->setUrl("http://localhost:8080/submit");
	request->setRequestType(HttpRequest::Type::POST);
	//request->setResponseCallback(CC_CALLBACK_2(GameScene::onHttpRequestCompleted, this));
	string _postData = "score=" + score_field->getString();
	const char* postData = _postData.c_str();
	request->setRequestData(postData, strlen(postData));
	request->setTag("Post Submit");

	//在请求的Header部分加入GAMESESSIONID
	vector<string> headers;
	headers.push_back("Cookie: GAMESESSIONID=" + Global::gameSessionId);
	request->setHeaders(headers);
	cocos2d::network::HttpClient::getInstance()->send(request);
	request->release();
}

void GameScene::onHttpRequestCompleted(HttpClient *sender, HttpResponse *response) {
	if (!response) {
		return;
	}
	if (!response->isSucceed()) {
		log("response failed");
		log("error buffer: %s", response->getErrorBuffer());
		return;
	}
	std::vector<char> *buffer = response->getResponseData();
	rapidjson::Document d;
	d.Parse<0>(Global::toString(buffer).c_str());
	if (d.HasParseError()) //打印解析错误
	{
		log("GetPareseError %s\n", d.GetParseError());
		return;
	}
	if (d.IsObject() && d.HasMember("result")) {
		if (d["result"].GetBool()) {
			string top_rank = d["info"].GetString();
			top_rank.erase(top_rank.begin());
			for (unsigned int i = 0; i < top_rank.length(); i++) {
				if (top_rank[i] == '|')
					top_rank[i] = '\n';
			}
			rank_field->setString(top_rank);  //显示排行榜
		}
	}

	printf("Http Test, dum data: ");
	for (unsigned int i = 0; i < buffer->size(); i++) {
		printf("%c", (*buffer)[i]);
	}
	printf("\n");
}

void GameScene::Rank() {
	HttpRequest* request = new HttpRequest();
	request->setUrl("http://localhost:8080/rank?top=10");
	request->setRequestType(HttpRequest::Type::GET);
	request->setResponseCallback(CC_CALLBACK_2(GameScene::onHttpRequestCompleted, this));
	request->setTag("Get Rank");
	//在请求的Header部分加入GAMESESSIONID
	vector<string> headers;
	headers.push_back("Cookie: GAMESESSIONID=" + Global::gameSessionId);
	request->setHeaders(headers);
	cocos2d::network::HttpClient::getInstance()->send(request);
	request->release();
}


