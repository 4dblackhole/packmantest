#include "stdafx.h"
#include "starcraftScene.h"
#include "battle.h"

HRESULT starcraftScene::init()
{
	_battle = new battle;
	_battle->init("battle", WINSIZEX / 2, WINSIZEY / 2);
	
	return S_OK;
}

void starcraftScene::release()
{
}

void starcraftScene::update()
{
	_battle->update();
}

void starcraftScene::render()
{
	_battle->render();
}
