#include "stdafx.h"
#include "action.h"

action::action() {}

action::~action() {}

HRESULT action::init()
{
	_worldTimeCount = 0;
	_isMoving = false;

	return S_OK;
}

void action::release()
{
}

void action::update()
{
	moving();
}

void action::render()
{
}

void action::moveTo(image * image, float endX, float endY, float time)
{
	if (!_isMoving)
	{
		_image = image;

		_endX = endX;
		_endY = endY;

		_startX = image->getX();
		_startY = image->getY();

		//움직여야할 거리를 구한다.
		_travelRange = getDistance(_startX, _startY, _endX, _endY);

		//각도
		_angle = getAngle(_startX, _startY, _endX, _endY);

		//월드타임은 타임매니저의 것을 끌어와서 넣어줌
		_worldTimeCount = TIMEMANAGER->getWorldTime();

		_time = time;

		_isMoving = true;
	}
}

void action::moving(void)
{
	if (!_isMoving) return;

	//경과시간
	float elapsedTime = TIMEMANAGER->getElapsedTime();

	//속도구하는 것
	float moveSpeed = (elapsedTime / _time) * _travelRange;
	//내속도 = (틱카운터(0.001) / 소요시간) * 거리

	_image->setX(_image->getX() + cosf(_angle) * moveSpeed);
	_image->setY(_image->getY() + -sinf(_angle) * moveSpeed);

	if (_time + _worldTimeCount <= TIMEMANAGER->getWorldTime())
	{
		_worldTimeCount = TIMEMANAGER->getWorldTime();

		_image->setX(_endX);
		_image->setY(_endY);

		_isMoving = false;
	}
}
