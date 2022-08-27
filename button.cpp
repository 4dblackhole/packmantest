#include "stdafx.h"
#include "button.h"

HRESULT button::init(const char* imageName, int x, int y, POINT btnDownFramePoint, POINT btnUpFrmaePoint, CALLBACK_FUNCTION cbFunction)
{
	//콜백함수는 함수포인터로 전역화 시켜놨기 때문에 형변환은 요로케 해라
	_callbackFunction = static_cast<CALLBACK_FUNCTION>(cbFunction);

	_direction = BUTTONDIRECTION_NULL;

	_x = x;
	_y = y;

	_btnUpFramePoint = btnUpFrmaePoint;
	_btnDownFramePoint = btnDownFramePoint;

	_imageName = imageName;

	_image = IMAGEMANAGER->findImage(imageName);

	_rc = RectMakeCenter(x, y, _image->getFrameWidth(),
		_image->getFrameHeight());
	return S_OK;
}

void button::release(void)
{
}

void button::update(void)
{
	if (PtInRect(&_rc, _ptMouse))
	{
		if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
		{
			_direction = BUTTONDIRECTION_DOWN;
		}
		else if (KEYMANAGER->isOnceKeyUp(VK_LBUTTON) 
			&& _direction == BUTTONDIRECTION_DOWN)
		{
			_direction = BUTTONDIRECTION_UP;
			//이때 콜백함수가 실행됨
			_callbackFunction();
		}
	}
	else _direction = BUTTONDIRECTION_NULL;
}

void button::render(void)
{
	switch (_direction)
	{
	case BUTTONDIRECTION_NULL: case BUTTONDIRECTION_UP:
		_image->frameRender(getMemDC(), _rc.left,
			_rc.top, _btnUpFramePoint.x,
			_btnUpFramePoint.y);
		break;
	case BUTTONDIRECTION_DOWN:
		_image->frameRender(getMemDC(), _rc.left,
			_rc.top, _btnDownFramePoint.x,
			_btnDownFramePoint.y);
		break;

	}
}
