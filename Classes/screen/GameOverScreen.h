//
// Created by gosia on 19.01.17.
//
#pragma once

#include <cocos2d.h>

class GameOver : public cocos2d::Scene
{
	using inherited = cocos2d::Scene;

public:
	CREATE_FUNC(GameOver);

	bool init() override;
};


