#pragma once
#include "image.h"

static image* _backBuffer = IMAGEMANAGER->addImage("backBuffer", WINSIZEX, WINSIZEY);//추가

//버튼 메시제(자식윈도우 메세지 번후 구분을 위한 이넘문)
enum CTRL
{
	CTRL_SAVE,        //세이브   
	CTRL_LOAD,		  //로드
	CTRL_TERRAINDRAW, //지형
	CTRL_OBJDRAW,	  //오브젝트
	CTRL_ERASER,	  //지우개
	CTRL_END
};
class gameNode
{
private:
	HDC _hdc;
	bool _managerInit;
public:
	int _ctrSelect; //컨트롤 버튼관련 변수

	virtual HRESULT init(void);
	virtual HRESULT init(bool managerInit);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	virtual void setMap(void);
	virtual void save(void);
	virtual void load(void);

	void setCtrlSelect(int num) { _ctrSelect = num; }

	image* getBackBuffer(void) { return _backBuffer; }
	HDC getMemDC() { return _backBuffer->getMemDC(); }
	HDC getHDC() { return _hdc; }

	LRESULT MainProc(HWND, UINT, WPARAM, LPARAM);

	gameNode();
	virtual ~gameNode();
};

