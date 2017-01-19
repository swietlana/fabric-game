//
// Created by gosia on 17.01.17.
//

#include "GameScreen.h"
#include <iostream>
#include <algorithm>
#include <crosslayout/ComposerCocos.h>

bool GameScreen::init() {

	if (inherited::init() == false) {
		return false;
	}
	CrossLayout::ComposerCocos composer;

//	auto background = cocos2d::Sprite::create("background.png");
//	addChild(background);
//	background->setScale(0.4);
//	composer.center(background).inParent();

	_tape = cocos2d::Node::create();
	addChild(_tape);

	_tapeWithJars = cocos2d::Node::create();
	addChild(_tapeWithJars);

	auto tape1 = cocos2d::Sprite::create("tape.png");
	_tape->addChild(tape1);
	tape1->setAnchorPoint({1, 1});
	composer.leftEdge(tape1).moveTo().parentLeftEdge();
	composer.bottomEdge(tape1).moveTo().parentBottomEdge(200);

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

	schedule([this](float dt) {
		updateTape(dt);
	}, "updateTape");

	schedule([this](float dt) {
		_speed += 20;
	}, 10, "speed");

	schedule([this](float dt) {
		auto jar = createJar(static_cast<JarType>(rand() % JarType::lastJar),
							 static_cast<Defect>(rand() % Defect::lastDefect));
		_tapeWithJars->addChild(jar);
		jar->setAnchorPoint({0, 0});
		const auto spawnPosition = _tape->convertToNodeSpace({getContentSize().width, 0});
		jar->setPosition({spawnPosition.x, 240});
	}, 2, "addJar");


	return true;
}

void GameScreen::updateTape(float dt) {
	_tape->setPositionX(_tape->getPosition().x - _speed * dt);
	_tapeWithJars->setPosition(_tape->getPosition());

	auto children = _tape->getChildren();
	std::sort(children.begin(), children.end(), [](const cocos2d::Node *left, const cocos2d::Node *right) {
		return left->getPosition().x < right->getPosition().x;
	});

	CrossLayout::ComposerCocos composer;
	auto last = children.back();

	const auto outOfSight = _tape->convertToNodeSpace({0, 0});

	for (auto child : children) {
		if (child->getPosition().x < outOfSight.x) {
			composer.move(child).toRightOf(last);
			last = child;
		}
	}


	for (auto child : getActiveJars()) {
		if ((child->getPosition().x + child->getContentSize().width) < outOfSight.x) {
			child->removeFromParent();
		}
	}
}

cocos2d::Node *GameScreen::createJar(JarType jarType, Defect defect) {
	CrossLayout::ComposerCocos composer;
	std::string jarView;

	switch (jarType) {
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
	jarSprite->setScale(0.2);

	std::string defectView;
	switch (defect) {
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

	if (defectView.empty() == false) {
		auto defectSprite = cocos2d::Sprite::create(defectView);
		jarSprite->addChild(defectSprite);
		composer.center(defectSprite).inParent();

	}
	return jarSprite;
}

void GameScreen::onEnter() {
	inherited::onEnter();
	_touchListener = cocos2d::EventListenerTouchOneByOne::create();
	_touchListener->setSwallowTouches(true);
	_touchListener->onTouchBegan = [this](cocos2d::Touch *touch, cocos2d::Event *event) {

		cocos2d::log("Touch %f %f",touch->getLocation().x,touch->getLocation().y);
		cocos2d::Vec2 location = _tapeWithJars->convertToNodeSpace(touch->getLocation());
		for (auto child : getActiveJars()) {
			if (child->getBoundingBox().containsPoint(location)) {
				child->removeFromParent();
				return true;
			}
		}
		return false;
	};
	cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_touchListener,
																								   this);

}

void GameScreen::onExit() {
	inherited::onExit();
	cocos2d::Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(this, false);
}

std::vector<cocos2d::Node *> GameScreen::getActiveJars() {
	std::vector<cocos2d::Node *> jars;
	for (auto child : _tapeWithJars->getChildren()) {
		jars.push_back(child);
	}
	return jars;
}

