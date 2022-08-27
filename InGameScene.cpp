#include "stdafx.h"
#include "InGameScene.h"


InGameScene::InGameScene()
{
}


InGameScene::~InGameScene()
{
}

HRESULT InGameScene::init()
{
	return S_OK;
}

void InGameScene::release()
{
}

void InGameScene::update()
{
}

void InGameScene::render()
{
	IMAGEMANAGER->render("Çù°î", getMemDC(), 0, 0);
}
