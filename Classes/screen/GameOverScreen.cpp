//
// Created by gosia on 19.01.17.
//

#include "GameOverScreen.h"
#include "GameScreen.h"
#include <iostream>
#include <algorithm>
#include <crosslayout/ComposerCocos.h>
#include <ui/UIButton.h>

bool GameOver::init()
{
	if (inherited::init() == false)
	{
		return false;
	}
	CrossLayout::ComposerCocos composer;

	auto _gameOverLabel = cocos2d::Label::createWithSystemFont("Game Over", "Arial", 60);
	addChild(_gameOverLabel);
	composer.center(_gameOverLabel).inParent();

	auto playButton = cocos2d::ui::Button::create("menu/play_button.png");
	addChild(playButton);
	composer.topEdge(playButton).moveTo().bottomEdge(_gameOverLabel, 60);
	composer.rightEdge(playButton).moveTo().parentRightEdge(100);

	playButton->addTouchEventListener([](cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
									  {
										  if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
										  {
											  cocos2d::Director::getInstance()->replaceScene(GameScreen::create());
										  }
									  });

	auto exitButton = cocos2d::ui::Button::create("menu/exit_button.png");
	addChild(exitButton);
	composer.topEdge(exitButton).moveTo().bottomEdge(_gameOverLabel, 60);
	composer.leftEdge(exitButton).moveTo().parentLeftEdge(100);

	exitButton->addTouchEventListener([](cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
									  {
										  if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
										  {
											  cocos2d::Director::getInstance()->end();
										  }
									  });
	return true;

}


