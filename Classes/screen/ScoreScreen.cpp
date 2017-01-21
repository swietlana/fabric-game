//
// Created by gosia on 21.01.17.
//
#include "ScoreScreen.h"
#include "StartScreen.h"
#include <iostream>
#include <algorithm>
#include <crosslayout/ComposerCocos.h>
#include <ui/UIButton.h>

struct PlayerScore
{
	std::string name;
	int points;

	PlayerScore(const std::string& name, int points)
			: name(name)
			, points(points)
	{
	}
};

bool ScoreScreen::init()
{
	if (inherited::init() == false)
	{
		return false;
	}
	CrossLayout::ComposerCocos composer;

	auto background = cocos2d::Sprite::create("background.png");
	addChild(background);
	background->setScale(0.5);
	background->setOpacity(100);
	composer.center(background).inParent();

	auto backButton = cocos2d::ui::Button::create("cancel.png");
	backButton->setScale(0.5);
	addChild(backButton);
	composer.leftEdge(backButton).moveTo().parentLeftEdge(20);
	composer.topEdge(backButton).moveTo().parentTopEdge(20);

	backButton->addTouchEventListener([](cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
									  {
										  if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
										  {

											  cocos2d::Director::getInstance()->pushScene(StartScreen::create());
										  }
									  });

	std::vector<std::string> keys = {"User1", "User2", "User3", "User4", "User5", "User6", "User7", "User8"
			, "highScore"};
	std::vector<PlayerScore> players;
	for (auto key : keys)
	{
		auto score = cocos2d::UserDefault::getInstance()->getIntegerForKey(key.c_str(), 0);
		if (key == "highScore")
		{
			key = "You";
		}
		players.push_back(PlayerScore(key, score));
	}

	std::sort(players.begin(), players.end(), [](const PlayerScore& left, const PlayerScore& right)
	{
		return left.points > right.points;
	});

	cocos2d::Label* previous = nullptr;
	for (int i = 0; i < 7 && i < players.size(); ++i)
	{
		auto scoreLabel = cocos2d::Label::createWithTTF("", "fonts/Verdana.ttf", 32);
		addChild(scoreLabel);
		scoreLabel->setTextColor(cocos2d::Color4B(191, 191, 191, 200));
		composer.center(scoreLabel).inParent().horizontally();

		auto player = players.at(i);

		scoreLabel->setString(player.name + " " + std::to_string(player.points));
		if (previous == nullptr)
		{
			composer.topEdge(scoreLabel).moveTo().parentTopEdge(200);
		}
		else
		{
			composer.topEdge(scoreLabel).moveTo().bottomEdge(previous, 20);
		}

		previous = scoreLabel;
	}

	return true;
}



