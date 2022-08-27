#include "stdafx.h"
#include "gameNode.h"

gameNode::gameNode()
{
}

gameNode::~gameNode()
{

}

//전체 바꾸기
HRESULT gameNode::init()	  
{
	_hdc = GetDC(_hWnd);
	_managerInit = false;

	return S_OK;
}
//함수 추가
HRESULT gameNode::init(bool managerInit)
{
	_hdc = GetDC(_hWnd);
	_managerInit = managerInit;

	if (_managerInit)
	{
		SetTimer(_hWnd, 1, 10, NULL);
		KEYMANAGER->init();
		IMAGEMANAGER->init();
		SOUNDMANAGER->init();
		TIMEMANAGER->init();
		EFFECTMANAGER->init();
		SCENEMANAGER->init();
		DATABASE->init();
	}

	return S_OK;
}

//메모리 해제(내용 번부 변경)
void gameNode::release()	  
{
	if (_managerInit)
	{
		KillTimer(_hWnd, 1);
		KEYMANAGER->release();
		KEYMANAGER->releaseSingleton();
		IMAGEMANAGER->release();
		IMAGEMANAGER->releaseSingleton();
		SOUNDMANAGER->release();
		SOUNDMANAGER->releaseSingleton();
		TIMEMANAGER->release();
		TIMEMANAGER->releaseSingleton();
		EFFECTMANAGER->release();
		EFFECTMANAGER->releaseSingleton();
		SCENEMANAGER->release();
		SCENEMANAGER->releaseSingleton();
		DATABASE->release();
		DATABASE->releaseSingleton();
	}

	ReleaseDC(_hWnd, _hdc);
}

//업데이트(갱신)
void gameNode::update()		  
{
	SOUNDMANAGER->update();
}

void gameNode::render(void)
{
}

void gameNode::setMap(void)
{
}

void gameNode::save(void)
{
}

void gameNode::load(void)
{
}

//main에서 복사해서 가져오기
LRESULT gameNode::MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;				//Handel Device Context
	PAINTSTRUCT ps;

	static RECT _rc;

	switch (iMessage)
	{
	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);
		this->render();
		EndPaint(hWnd, &ps);
	}
	break;

	case WM_TIMER:
		this->update();
		break;
		
	case WM_LBUTTONDOWN:
		_leftButtonDown = true;
		this->setMap();
		break;

	case WM_LBUTTONUP:
		_leftButtonDown = false;
		break;

	case WM_MOUSEMOVE:
	{
		_ptMouse.x = static_cast<float>LOWORD(lParam);
		_ptMouse.y = static_cast<float>HIWORD(lParam);
		
		if (_leftButtonDown) this->setMap();
	}
	break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case CTRL_SAVE:
			this->save();
			break;
		case CTRL_LOAD:
			this->load();
			InvalidateRect(hWnd, NULL, false);
			break;
		default:
			this->setCtrlSelect(LOWORD(wParam));
			break;
		}
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			PostMessage(hWnd, WM_DESTROY, 0, 0);
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}
