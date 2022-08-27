#include "stdafx.h"
#include "pixelCollision.h"


pixelCollision::pixelCollision(){ }


pixelCollision::~pixelCollision(){ }

HRESULT pixelCollision::init()
{
	//언던 마운틴 생성
	IMAGEMANAGER->addImage("언덕", "mountain.bmp", WINSIZEX, WINSIZEY, true, RGB(255, 0, 255));
	//볼 이미지 생성
	_ball = IMAGEMANAGER->addImage("ball", "ball.bmp", 60, 60, true, RGB(255, 0, 255));

	//중점 좌표(픽셀 충돌로 인해 자동으로 색이 없는 지점으로 나감)
	_x = WINSIZEX / 2 - 150;
	_y = WINSIZEY / 2 + 100;

	//공 이미지 정보로 렉트를 생성
	//>getWidth() = 이미지의 크기로 생성
	_rc = RectMakeCenter(_x, _y, _ball->getWidth(), _ball->getHeight());

	_probeY = _y + _ball->getHeight() / 2;


	return S_OK;
}

void pixelCollision::release()
{

}

void pixelCollision::update()
{
	if (KEYMANAGER->isStayKeyDown(VK_LEFT)) _x -= 3;
	if (KEYMANAGER->isStayKeyDown(VK_RIGHT)) _x += 3;

	//상자는 움직이니 업데이트에서 좌표를 계속해서 받아준다.
	_rc = RectMakeCenter(_x, _y, _ball->getWidth(), _ball->getHeight());

	_probeY = _y + _ball->getHeight() / 2;

	for (int i = _probeY - 50; i < _probeY + 50; ++i)
	{
		//GetPixel = 특정 위치의(x,y)에 있는 픽셀의 rgb값을 돌려줍니다.
		//이 작업을 하기 위해서는 Select된 비트맵의 정보구조를 파악하고 비트맵의 이미지 버퍼를 얻어와
		//해당 위치에 존재하는 BYTE값을 조사하여 RGB값을 뻄
		//COLORREF = RGB색상을 표현
		//RGB색상으로 표현 = "언덕"이라는 이미지의 RGB값을 조사
		COLORREF color = GetPixel(IMAGEMANAGER->findImage("언덕")->getMemDC(), _x, i);

		//GetRValue = 색상값 출력 코드
		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);
		//만약 RGB색상이 마젠타 색상이 아닐경우
		if (!(r == 255 && g == 0 && b == 255))
		{
			_y = i - _ball->getHeight() / 2;
			break;
		}

	}
}

void pixelCollision::render()
{
	IMAGEMANAGER->findImage("언덕")->render(getMemDC());

	_ball->render(getMemDC(), _rc.left, _rc.top);

}


