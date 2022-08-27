#include "stdafx.h"
#include "LoadingScene.h"
#include "progressBar.h"

LoadingScene::LoadingScene()
	:_background(nullptr),
	_loadingBar(nullptr),
	currentCount(0)
{
}


LoadingScene::~LoadingScene()
{
}

HRESULT LoadingScene::init(void)
{
	_background = IMAGEMANAGER->addImage("롤", "롤로딩.bmp",
		WINSIZEX, WINSIZEY, true, RGB(255, 0, 255));

	_loadingBar = new progressBar;
	_loadingBar->init(0, WINSIZEY - 20, WINSIZEX, 20);

	_loadingBar->setGauge(0, 0);

	//실제 파일들을 로드할 스레드
	CreateThread(
		NULL,             // 스레드의 보안속성(핸들의 자식인가? 부모인가?)
		0,				  // 스레드의 스택크기(0으로 하면 디폴트값 잡힘)
		ThreadFunction,	  // 스레드의 시작함수
		this,			  // 스레드의 매개변수(본인, 자기자신 돌릴거다)
		NULL,			  // 스레드의 특성(NULL이면 만들자마자 바로 시작)
		0				  // 스레드를 만든 후 스레드의 ID를 넘겨준다.
						  // 요거는 필요없음. 어차피 로딩 다 하면 스레드 알아서 소멸
	);
	return S_OK;
}

void LoadingScene::release(void)
{
	SAFE_DELETE(_loadingBar);
}

void LoadingScene::update(void)
{
	_loadingBar->update();
	_loadingBar->setGauge(currentCount, 100);

	//로딩 다 되면 씬 체인지
	if (currentCount == 100)
	{
		SCENEMANAGER->changeScene("인게");
	}
}

void LoadingScene::render(void)
{
	_background->render(getMemDC(), 0, 0);
	_loadingBar->render();
}

//로딩은 이 스레드가 전담.
//로드가 끝나면 스레드도 알아서 삭제된다.
DWORD CALLBACK ThreadFunction(LPVOID lpParamenter)
{
	//자기 자신 클래스를 명시적 형변환으로 받아온다.
	LoadingScene* LoadHelper = (LoadingScene*)lpParamenter;

	//로딩 카운트가 100이 될 떄 까지 돌려준다.
	while (LoadHelper->currentCount != 100)
	{
		//로딩 예시로 포문 천 번을 돌려주는 거임(의미없다 진짜!!!!!!!!!)
		for (int i = 0; i < 1000; ++i)
		{
			IMAGEMANAGER->addImage("협곡", "롤협곡.bmp", 800, 600, false, NULL);
			
			IMAGEMANAGER->addImage("협곡1", "롤협곡.bmp", 800, 600, false, NULL); 
			IMAGEMANAGER->addImage("협곡2", "롤협곡.bmp", 800, 600, false, NULL); 
			IMAGEMANAGER->addImage("협곡3", "롤협곡.bmp", 800, 600, false, NULL); 
			IMAGEMANAGER->addImage("협곡4", "롤협곡.bmp", 800, 600, false, NULL); 
			IMAGEMANAGER->addImage("협곡5", "롤협곡.bmp", 800, 600, false, NULL); 
			IMAGEMANAGER->addImage("협곡6", "롤협곡.bmp", 800, 600, false, NULL); 
			IMAGEMANAGER->addImage("협곡7", "롤협곡.bmp", 800, 600, false, NULL); 
			printf_s("로딩중\n");
		}
		LoadHelper->currentCount++;
	}
	//로딩 다 되면 스레드 알아서 종료
	return 0;
}
