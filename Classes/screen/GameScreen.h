//
// Created by gosia on 17.01.17.
//

#pragma once

#include <cocos2d.h>

class GameScreen : public cocos2d::Scene {

public:
	CREATE_FUNC(GameScreen);

	bool init() override;

	void updateTape(float dt);

private:
	cocos2d::Node* _tape = nullptr;
};