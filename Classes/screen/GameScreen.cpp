//
// Created by gosia on 17.01.17.
//

#include "GameScreen.h"

#include <algorithm>
#include <crosslayout/ComposerCocos.h>

bool GameScreen::init() {

	if (cocos2d::Scene::init() == false) {
		return false;
	}
	CrossLayout::ComposerCocos composer;


	_tape = cocos2d::Node::create();
	addChild(_tape);


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


	return true;
}

void GameScreen::updateTape(float dt) {
	int i = 100;
	_tape->setPositionX(_tape->getPosition().x - i * dt);


	auto children = _tape->getChildren();
	std::sort(children.begin(), children.end(), [](const cocos2d::Node *left, const cocos2d::Node *right) {
		return left->getPosition().x < right->getPosition().x;
	});

	CrossLayout::ComposerCocos composer;
	auto last = children.back();

	for (auto child : children) {
		if (_tape->convertToWorldSpace(child->getPosition()).x < 0) {
			composer.move(child).toRightOf(last);
			last = child;
		}
	}
}

