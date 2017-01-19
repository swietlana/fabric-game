//
// Created by gosia on 17.01.17.
//

#pragma once

#include <cocos2d.h>

class GameScreen : public cocos2d::Scene
{
	using inherited = cocos2d::Scene;
public:
	CREATE_FUNC(GameScreen);

	bool init() override;

	void updateTape(float dt);

	enum Defect
	{
		bigDefect, midDefect, longDefect, littleDefect, smallDefect, noDefect, lastDefect
	};

	enum JarType
	{
		trapezeJar, bigWhiteJar, bigBlackJar, tallJar, noCoverJar, lastJar
	};

	void onEnter() override;

	void onExit() override;

private:
	cocos2d::Node* _tape = nullptr;
	cocos2d::Node* _tapeWithJars = nullptr;
	cocos2d::EventListenerTouchOneByOne* _touchListener = nullptr;
	float _speed = 100;

	cocos2d::Node* createJar(JarType jarType, Defect defect);
	std::vector<cocos2d::Node*> getActiveJars();

};
