#include "stdafx.h"
#include "mainGame.h"

//전역변수
HINSTANCE	_hInstance;		//인스턴스 핸들
HWND		_hWnd;			//핸들

LPCSTR _lpszClass = TEXT("Neptune Window API");

POINT _ptMouse;

BOOL _leftButtonDown;

//장자
mainGame _mg;

//함수의 프로토 타입
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

void setWindowSize(int x, int y, int width, int height);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int cmdShow)
{
	MSG			message;		//메시지 구조체
	WNDCLASS	wndClass;		//윈도우 정보 구조체

	_hInstance = hInstance;

	//윈도우 정보를 먼저 셋팅한다
	wndClass.cbClsExtra = 0;										//클래스 여분 메모리
	wndClass.cbWndExtra = 0;										//윈도우 여분 메모리
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	//윈도우 배경 색
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);					//커서 아이콘 모양
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);				//어플 아이콘 모양
	wndClass.hInstance = hInstance;									//윈도우 인스턴스 번호
	wndClass.lpfnWndProc = (WNDPROC)WndProc;						//윈도우 프로시져
	wndClass.lpszClassName = WINNAME;							    //윈도우 클래스 이름
	wndClass.lpszMenuName = NULL;									//윈도우 메뉴 이름
	wndClass.style = CS_HREDRAW | CS_VREDRAW;						//윈도우 창 스타일

																	//윈도우 정보 등록
	RegisterClass(&wndClass);

	_hWnd = CreateWindow(
		WINNAME,
		WINNAME,
		WS_OVERLAPPEDWINDOW,		//윈도우 스타일
		WINSTARTX,					//윈도우 창 시작좌표 X
		WINSTARTY,					//윈도우 창 시작좌표 Y
		WINSIZEX,					//윈도우 가로크기
		WINSIZEY,					//윈도우 세로크기
		NULL,
		(HMENU)NULL,
		hInstance,
		NULL);

	//클라이언트 영역 재조정
	setWindowSize(WINSTARTX, WINSTARTY, WINSIZEX, WINSIZEY);
	//실제로 윈도우 창을 화면에 투영시켜준다(보여준다)
	ShowWindow(_hWnd, cmdShow);

	//GetMessage : 메시지가 들어오면 그때 처리한다
	//PeekMessage : 메시지가 들어오지 않아도 와일문은 돌고있다.

	//메인게임이 실패하면 초기화가 실패하면    -------------------------------------------------추가
	if (FAILED(_mg.init()))
	{
		//종료되거라
		return 0;
	}
	/*
	while (GetMessage(&message, 0, 0, 0))
	{
		TranslateMessage(&message);		//키보드 입력 시 그 메시지 처리 담당
		DispatchMessage(&message);		//실제로 윈도우에 메시지를 전달해주는 역할
	}
	*/
	
	while (true)
	{
		//PM_REMOVE = 메세지 큐에서 메세지 제거
		if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
		{
			//WM_QUIT = 프로그램 종료
			if (message.message == WM_QUIT) break;

			TranslateMessage(&message);		//키보드 입력 시 그 메시지 처리 담당
			DispatchMessage(&message);		//실제로 윈도우에 메시지를 전달해주는 역할
		}
		else
		{
			TIMEMANAGER->update(30.0f);
			_mg.update();
			_mg.render();
		}
	}
	
	_mg.release();

	return message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	return _mg.MainProc(hWnd, iMessage, wParam, lParam);
}

void setWindowSize(int x, int y, int width, int height)
{
	RECT winRect;

	winRect.left = 0;
	winRect.top = 0;
	winRect.right = width;
	winRect.bottom = height;

	//윈도우 사이즈 재조정을 해준다
	AdjustWindowRect(&winRect, WINSTYLE, false);

	//윈도우 위치를 재조정
	SetWindowPos(_hWnd, NULL, x, y, (winRect.right - winRect.left),
		(winRect.bottom - winRect.top), SWP_NOZORDER | SWP_NOMOVE);


}