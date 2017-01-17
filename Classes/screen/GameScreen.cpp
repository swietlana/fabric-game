//
// Created by gosia on 17.01.17.
//

#include "GameScreen.h"
#include <crosslayout/ComposerCocos.h>

bool GameScreen::init() {

	if (cocos2d::Scene::init() == false) {
		return false;
	}
	CrossLayout::ComposerCocos composer;



	return true;
}

void GameScreen::updateTape(float dt) {

}
