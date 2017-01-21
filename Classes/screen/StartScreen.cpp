//
// Created by gosia on 16.01.17.
//

#include <iostream>
#include "StartScreen.h"
#include "GameScreen.h"
#include "ScoreScreen.h"
#include <crosslayout/ComposerCocos.h>
#include <ui/UIButton.h>
#include <functional>

using namespace std;

bool StartScreen::init()
{
	if (cocos2d::Scene::init() == false)
	{
		return false;
	}

	CrossLayout::ComposerCocos composer;

	auto background = cocos2d::Sprite::create("background.png");
	addChild(background);
	background->setScale(0.5);
	background->setOpacity(100);
	composer.center(background).inParent();

	auto playButton = cocos2d::ui::Button::create("play_red.png");
	addChild(playButton);
	composer.center(playButton).inParent();

	playButton->addTouchEventListener([](cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
									  {
										  if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
										  {
											  cocos2d::Director::getInstance()->pushScene(GameScreen::create());
										  }
									  });

	auto scaleUpPlay = cocos2d::EaseExponentialInOut::create(cocos2d::ScaleTo::create(1.0f, 1.2f));
	auto scaleDownPlay = cocos2d::EaseExponentialInOut::create(cocos2d::ScaleTo::create(1.0f, 1.0f));
	auto seqScalePlay = cocos2d::Sequence::create({scaleUpPlay, scaleDownPlay});
	playButton->runAction(cocos2d::RepeatForever::create(seqScalePlay));

	auto exitButton = cocos2d::ui::Button::create("power_red.png");
	addChild(exitButton);
	composer.topEdge(exitButton).moveTo().bottomEdge(playButton, 30);
	composer.leftEdge(exitButton).moveTo().parentLeftEdge(100);

	exitButton->addTouchEventListener([](cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
									  {
										  if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
										  {
											  cocos2d::Director::getInstance()->end();
										  }
									  });

	auto scoresButton = cocos2d::ui::Button::create("scores.png");
	addChild(scoresButton);
	composer.topEdge(scoresButton).moveTo().bottomEdge(playButton, 30);
	composer.rightEdge(scoresButton).moveTo().parentRightEdge(100);

	scoresButton->addTouchEventListener([](cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
								  {
									  if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
									  {
										  cocos2d::Director::getInstance()->pushScene(ScoreScreen::create());
									  }
								  });

	return true;
}

