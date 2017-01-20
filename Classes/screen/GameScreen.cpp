//
// Created by gosia on 17.01.17.
//

#include "GameScreen.h"
#include "StartScreen.h"
#include "GameOverScreen.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <crosslayout/ComposerCocos.h>
#include <ui/UIButton.h>

bool GameScreen::init()
{

	if (inherited::init() == false)
	{
		return false;
	}
	CrossLayout::ComposerCocos composer;

	auto background = cocos2d::Sprite::create("background.png");
	addChild(background);
	background->setScale(0.5);
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

	_tape = cocos2d::Node::create();
	addChild(_tape);

	_tapeWithJars = cocos2d::Node::create();
	addChild(_tapeWithJars);

	auto tape1 = cocos2d::Sprite::create("tape.png");
	_tape->addChild(tape1);
	tape1->setAnchorPoint({1, 1});
	composer.leftEdge(tape1).moveTo().parentLeftEdge();
	composer.bottomEdge(tape1).moveTo().parentBottomEdge(150);

	auto tape2 = cocos2d::Sprite::create("tape.png");
	_tape->addChild(tape2);
	tape2->setAnchorPoint({1, 1});
	composer.center(tape2).in(tape1).vertically();
	composer.leftEdge(tape2).moveTo().rightEdge(tape1);

	auto tape3 = cocos2d::Sprite::create("tape.png");
	_tape->addChild(tape3);
	tape3->setAnchorPoint({1, 1});
	composer.center(tape3).in(tape1).vertically();
	composer.leftEdge(tape3).moveTo().rightEdge(tape2);

	_moneySign = cocos2d::Sprite::create("money.png");
	addChild(_moneySign);
	_moneySign->setScale(0.5);
	composer.topEdge(_moneySign).moveTo().parentTopEdge(20);
	composer.rightEdge(_moneySign).moveTo().parentRightEdge(10);
	composer.center(_moneySign).in(backButton).vertically();

	_showPoints = cocos2d::Label::createWithTTF("0", "fonts/Verdana.ttf", 32);
	addChild(_showPoints);
	_showPoints->setTextColor(cocos2d::Color4B(51, 51, 51, 200));
	composer.center(_showPoints).in(_moneySign).vertically();
	composer.rightEdge(_showPoints).moveTo().leftEdge(_moneySign, 20);

	_helloTip = cocos2d::Label::createWithSystemFont("Crush broken jars", "Verdana", 40);
	addChild(_helloTip);
	_helloTip->setTextColor(cocos2d::Color4B::WHITE);
	composer.center(_helloTip).inParent().horizontally();
	composer.topEdge(_helloTip).moveTo().bottomEdge(backButton, 260);
	auto helloScaleUp = cocos2d::ScaleTo::create(0.5f, 1.5f);
	auto helloScaleDown = cocos2d::ScaleTo::create(0.5f, 1.0f);
	_helloTip->runAction(cocos2d::Sequence::createWithTwoActions(helloScaleUp, helloScaleDown));
	auto helloTint = cocos2d::TintTo::create(1.0f, 51, 51, 51);
	_helloTip->runAction(cocos2d::Sequence::createWithTwoActions(
			cocos2d::DelayTime::create(8)
			, cocos2d::Spawn::create(helloTint
									 , cocos2d::FadeTo::create(1, 128)
									 , nullptr
			)));

	schedule([this](float dt)
			 {
				 updateTape(dt);
			 }, "updateTape");

	schedule([this](float dt)
			 {
				 _speed += 30;
			 }, 15, "speed");

	schedule([this](float dt)
			 {
				 cocos2d::log("Speed %f", _speed);
				 createRandomJar();
			 }, 2, "addJar");

	return true;
}

void GameScreen::updateTape(float dt)
{
	_tape->setPositionX(_tape->getPosition().x - _speed * dt);
	_tapeWithJars->setPosition(_tape->getPosition());

	auto children = _tape->getChildren();
	std::sort(children.begin(), children.end(), [](const cocos2d::Node* left, const cocos2d::Node* right)
	{
		return left->getPosition().x < right->getPosition().x;
	});

	CrossLayout::ComposerCocos composer;
	auto last = children.back();

	const auto outOfSight = _tape->convertToNodeSpace({0, 0});

	for (auto child : children)
	{
		if (child->getPosition().x < outOfSight.x)
		{
			composer.move(child).toRightOf(last);
			last = child;
		}
	}

	for (auto child : getActiveJars())
	{
		if ((child->getPosition().x + child->getContentSize().width * child->getScaleX()) < outOfSight.x)
		{
			deletedByTape(child);
			child->removeFromParent();
		}
	}
}

cocos2d::Node* GameScreen::createJar(JarType jarType, Defect defect)
{
	CrossLayout::ComposerCocos composer;
	std::string jarView;

	switch (jarType)
	{
		case JarType::bigBlackJar:
			jarView = "jars/bigBlackJar.png";
			break;
		case JarType::bigWhiteJar:
			jarView = "jars/bigWhiteJar.png";
			break;
		case JarType::trapezeJar:
			jarView = "jars/trapezeJar.png";
			break;
		case JarType::tallJar:
			jarView = "jars/tallJar.png";
			break;
		case JarType::noCoverJar:
			jarView = "jars/noCoverJar.png";
			break;
		default:

			break;
	}
	auto jarSprite = cocos2d::Sprite::create(jarView);
	jarSprite->setScale(0.3);

	std::string defectView;
	switch (defect)
	{
		case Defect::bigDefect:
			defectView = "jars/bigDefect.png";
			break;
		case Defect::midDefect:
			defectView = "jars/midDefect.png";
			break;
		case Defect::longDefect:
			defectView = "jars/longDefect.png";
			break;
		case Defect::smallDefect:
			defectView = "jars/smallDefect.png";
			break;
		case Defect::littleDefect:
			defectView = "jars/littleDefect.png";
			break;
		default:
			break;
	}

	if (defectView.empty() == false)
	{
		auto defectSprite = cocos2d::Sprite::create(defectView);
		defectSprite->setName("defect");
		jarSprite->addChild(defectSprite);
		composer.center(defectSprite).inParent();

	}
	return jarSprite;
}

void GameScreen::onEnter()
{
	inherited::onEnter();
	_touchListener = cocos2d::EventListenerTouchOneByOne::create();
	_touchListener->setSwallowTouches(true);
	_touchListener->onTouchBegan = [this](cocos2d::Touch* touch, cocos2d::Event* event)
	{
		cocos2d::Vec2 location = _tapeWithJars->convertToNodeSpace(touch->getLocation());
		for (auto child : getActiveJars())
		{
			if (child->getBoundingBox().containsPoint(location))
			{
				deletedByUser(child);
				child->removeFromParent();
				return true;
			}
		}

		return false;

	};

	cocos2d::Director::getInstance()->getEventDispatcher()
			->addEventListenerWithSceneGraphPriority(_touchListener, this);

}

void GameScreen::onExit()
{
	inherited::onExit();
	cocos2d::Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(this, false);
}

std::vector<cocos2d::Node*> GameScreen::getActiveJars()
{
	std::vector<cocos2d::Node*> jars;
	for (auto child : _tapeWithJars->getChildren())
	{
		jars.push_back(child);
	}
	return jars;
}

bool GameScreen::hasDefect(cocos2d::Node* jar)
{
	return jar->getChildByName("defect") != nullptr;
}

void GameScreen::deletedByUser(cocos2d::Node* jar)
{
	if (hasDefect(jar))
	{
		_points += 10;
	}
	else
	{
		_points -= 5;
	}
	cocos2d::log("Points %d", _points);
	updatePointsLabel();
	checkForGameOver();
}

void GameScreen::deletedByTape(cocos2d::Node* jar)
{
	if (hasDefect(jar))
	{
		_points -= 5;
	}
	else
	{
		_points += 10;
	}
	cocos2d::log("Points %d", _points);
	updatePointsLabel();
	checkForGameOver();
}

void GameScreen::updatePointsLabel()
{
	CrossLayout::ComposerCocos composer;
	_showPoints->setString(std::to_string(_points));
	composer.center(_showPoints).in(_moneySign).vertically();
	composer.rightEdge(_showPoints).moveTo().leftEdge(_moneySign, 10);
	auto scaleUpPoints = cocos2d::EaseExponentialInOut::create(cocos2d::ScaleTo::create(0.2f, 1.5f));
	auto scaleDownPoints = cocos2d::EaseExponentialInOut::create(cocos2d::ScaleTo::create(0.3f, 1.0f));
	auto seqScalePoints = cocos2d::Sequence::create({scaleUpPoints, scaleDownPoints});
	_showPoints->runAction(seqScalePoints);

}

void GameScreen::checkForGameOver()
{
	if (_points <= -30)
	{
		cocos2d::Director::getInstance()->replaceScene(GameOver::create());
	}
}

void GameScreen::createRandomJar()
{
	auto jar = createJar(static_cast<JarType>(rand() % JarType::lastJar), static_cast<Defect>(
			rand() %
			Defect::lastDefect));
	_tapeWithJars->addChild(jar);
	jar->setAnchorPoint({0, 0});
	const auto spawnPosition = _tape->convertToNodeSpace({getContentSize().width, 0});
	jar->setPosition({spawnPosition.x, 190});
}
