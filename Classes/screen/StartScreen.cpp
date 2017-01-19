//
// Created by gosia on 16.01.17.
//

#include <iostream>
#include "StartScreen.h"
#include "GameScreen.h"
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

	auto playButton = cocos2d::ui::Button::create("menu/play_button.png");
	addChild(playButton);
	composer.center(playButton).inParent();

	playButton->addTouchEventListener([](cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
									  {
										  if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
										  {
											  cocos2d::Director::getInstance()->pushScene(GameScreen::create());
										  }
									  });

	auto exitButton = cocos2d::ui::Button::create("menu/exit_button.png");
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

	auto stats = cocos2d::Sprite::create("menu/chart_button.png");
	addChild(stats);
	composer.topEdge(stats).moveTo().bottomEdge(playButton, 30);
	composer.rightEdge(stats).moveTo().parentRightEdge(100);

	return true;
}
