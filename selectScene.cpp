#include "stdafx.h"
#include "selectScene.h"


HRESULT selectScene::init()
{
	_btnSceneChange = new button;
	//버튼 초기화(버튼이미지 키값, X, Y좌표, 프레임X, Y 좌표, 콜백적용될 함수이름)
	_btnSceneChange->init("버튼", WINSIZEX / 2, WINSIZEY - 100,
		PointMake(0, 1), PointMake(0, 0), cbSceneChange);

	_btnAngleDown = new button;
	_btnAngleDown->init("버튼", WINSIZEX / 2 - 200, WINSIZEY / 2,
		PointMake(0, 1), PointMake(0, 0), cbAngleDown);

	_btnAngleUp = new button;
	_btnAngleUp->init("버튼", WINSIZEX / 2 + 200, WINSIZEY / 2,
		PointMake(0, 1), PointMake(0, 0), cbAngleUp);

	_action = new action;
	_action->init();

	_actionImage = IMAGEMANAGER->findImage("bullet");
	_actionImage->setX(WINSIZEX);
	_actionImage->setY(WINSIZEY);

	_action->moveTo(_actionImage, 50, 50, 15.0f);
	return S_OK;
}

void selectScene::release()
{
}

void selectScene::update()
{
	_btnSceneChange->update();
	_btnAngleDown->update();
	_btnAngleUp->update();
	_action->update();
}

void selectScene::render()
{
	TextOut(getMemDC(), WINSIZEX / 2, 100, "셀렉트씬", strlen("셀렉트씬"));
	_btnSceneChange->render();
	_btnAngleDown->render();
	_btnAngleUp->render();
	_actionImage->render(getMemDC(), _actionImage->getX(), _actionImage->getY());
}

void selectScene::cbSceneChange()
{
	SCENEMANAGER->changeScene("스타씬");
}

void selectScene::cbAngleUp()
{
	DATABASE->setElementDataAngle( "battle", DATABASE->getElementData("battle")->angle += 0.2f);
}

void selectScene::cbAngleDown()
{
	DATABASE->setElementDataAngle("battle", DATABASE->getElementData("battle")->angle -= 0.2f);
}

