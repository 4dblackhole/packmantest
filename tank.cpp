#include "stdafx.h"
#include "tank.h"

tank::tank(){}

tank::~tank(){}

HRESULT tank::init()
{
	_direction = TANKDIRECTION_RIGHT;

	_image = IMAGEMANAGER->addFrameImage("tank", "tank.bmp", 
		0, 0, 256, 128, 8, 4, true, RGB(255, 0, 255));

	_speed = 100.0f;

	return S_OK;
}

void tank::release()
{
}

void tank::update()
{
	if (KEYMANAGER->isStayKeyDown(VK_LEFT) && _rc.left > 0)
	{
		_direction = TANKDIRECTION_LEFT;
		tankMove();
	}
	if (KEYMANAGER->isStayKeyDown(VK_RIGHT) && _rc.right < WINSIZEX)
	{
		_direction = TANKDIRECTION_RIGHT;
		tankMove();
	}
	if (KEYMANAGER->isStayKeyDown(VK_UP) && _rc.top > 0)
	{
		_direction = TANKDIRECTION_UP;
		tankMove();
	}
	if (KEYMANAGER->isStayKeyDown(VK_DOWN) && _rc.bottom < WINSIZEY)
	{
		_direction = TANKDIRECTION_DOWN;
		tankMove();
	}
}

void tank::render()
{
	_image->frameRender(getMemDC(), _rc.left, _rc.top);
}

void tank::tankMove()
{
	//가상의 충돌판정 렉트를 만들어둔다.
	RECT rcCollision;

	//타일 검출에 사용할 타일 인덱스
	int tileIndex[2];
	//플레이어 진행방향에 검출될 타일 인덱스 계산할 변수
	int tileX, tileY;

	//가상의 렉트에 탱크 렉트를 대입해준다
	rcCollision = _rc;

	float elapsedTime = TIMEMANAGER->getElapsedTime();
	float moveSpeed = elapsedTime * _speed;

	//해당 방향에 따른 탱크 이미지 프레임 전환 및 움직임에 관한 스위치
	switch (_direction)
	{
		case TANKDIRECTION_LEFT:
			_image->setFrameX(0);
			_image->setFrameY(3);

			_x -= moveSpeed;

			rcCollision = RectMakeCenter(_x, _y, _image->getFrameWidth(), _image->getFrameHeight());
		break;
		case TANKDIRECTION_UP:
			_image->setFrameX(0);
			_image->setFrameY(0);

			_y -= moveSpeed;

			rcCollision = RectMakeCenter(_x, _y, _image->getFrameWidth(), _image->getFrameHeight());
			break;
		case TANKDIRECTION_RIGHT:
			_image->setFrameX(0);
			_image->setFrameY(2);

			_x += moveSpeed;

			rcCollision = RectMakeCenter(_x, _y, _image->getFrameWidth(), _image->getFrameHeight());
			break;
		case TANKDIRECTION_DOWN:
			_image->setFrameX(0);
			_image->setFrameY(1);

			_y += moveSpeed;

			rcCollision = RectMakeCenter(_x, _y, _image->getFrameWidth(), _image->getFrameHeight());
			break;
	}
	
	//0.1의 오차라도 있으면 타일 사이를 탱크가
	//빠져나오지를 못하기 때문에
	//충돌 박스를 줄여준다.
	rcCollision.left += 2;
	rcCollision.top += 2;
	rcCollision.right -= 2;
	rcCollision.bottom -= 2;

	//타일의 좌표를 (1.1)형식으로 바꿔준다.
	tileX = rcCollision.left / TILESIZE;
	tileY = rcCollision.top / TILESIZE;

	//타일을 검출해보자
	switch (_direction)
	{
	case TANKDIRECTION_LEFT:
		tileIndex[0] = tileX + (tileY * TILEX);
		tileIndex[1] = tileX + (tileY + 1) * TILEX;
		break;
	case TANKDIRECTION_UP:
		tileIndex[0] = tileX + tileY * TILEY;
		tileIndex[1] = (tileX + 1) + tileY * TILEX;
		break;
	case TANKDIRECTION_RIGHT:
		tileIndex[0] = (tileX + tileY * TILEX) + 1;
		tileIndex[1] = (tileX + (tileY + 1)  * TILEX) + 1;
		break;
	case TANKDIRECTION_DOWN:
		tileIndex[0] = (tileX + tileY * TILEX) + TILEX;
		tileIndex[1] = (tileX + 1 + tileY * TILEX) + TILEX;
		break;
	}

	//타일 충돌 시켜주자
	for (int i = 0; i < 2; i++)
	{
		RECT rc;
		//타일의 상태가 못움직이는 타일 상태이고 그리고 충돌햇을 떄(intersectrect)
		if (((_tankMap->getTileAttribute()[tileIndex[i]] & ATTR_UNMOVE) == ATTR_UNMOVE) 
			&& IntersectRect(&rc, &_tankMap->getTile()[tileIndex[i]].rc, &rcCollision))
		{
			switch (_direction)
			{
			case TANKDIRECTION_LEFT:
				_rc.left = _tankMap->getTile()[tileIndex[i]].rc.right;
				_rc.right = _rc.left + 30;

				_x = _rc.left + (_rc.right - _rc.left) / 2;
				break;
			case TANKDIRECTION_UP:
				_rc.top = _tankMap->getTile()[tileIndex[i]].rc.bottom;
				_rc.bottom = _rc.top + 30;

				_y = _rc.top + (_rc.bottom - _rc.top) / 2;
				break;
			case TANKDIRECTION_RIGHT:
				_rc.right = _tankMap->getTile()[tileIndex[i]].rc.left;
				_rc.left = _rc.right - 30;

				_x = _rc.left + (_rc.right - _rc.left) / 2;
				break;
			case TANKDIRECTION_DOWN:
				_rc.bottom = _tankMap->getTile()[tileIndex[i]].rc.top;
				_rc.top = _rc.bottom - 30;
				_y = _rc.top + (_rc.bottom - _rc.top) / 2;
				break;
			}
			return;
		}
	}
	_rc = rcCollision;
	_rc = RectMakeCenter(_x, _y, _image->getFrameWidth(), _image->getFrameHeight());
}

void tank::setTankPosition()
{
	_rc = _tankMap->getTile()[_tankMap->getPosFirst()].rc;
	
	_x = _rc.left + (_rc.right - _rc.left) / 2;
	_y = _rc.top + (_rc.bottom - _rc.top) / 2;
}
