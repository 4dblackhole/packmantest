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
	_background = IMAGEMANAGER->addImage("��", "�ѷε�.bmp",
		WINSIZEX, WINSIZEY, true, RGB(255, 0, 255));

	_loadingBar = new progressBar;
	_loadingBar->init(0, WINSIZEY - 20, WINSIZEX, 20);

	_loadingBar->setGauge(0, 0);

	//���� ���ϵ��� �ε��� ������
	CreateThread(
		NULL,             // �������� ���ȼӼ�(�ڵ��� �ڽ��ΰ�? �θ��ΰ�?)
		0,				  // �������� ����ũ��(0���� �ϸ� ����Ʈ�� ����)
		ThreadFunction,	  // �������� �����Լ�
		this,			  // �������� �Ű�����(����, �ڱ��ڽ� �����Ŵ�)
		NULL,			  // �������� Ư��(NULL�̸� �����ڸ��� �ٷ� ����)
		0				  // �����带 ���� �� �������� ID�� �Ѱ��ش�.
						  // ��Ŵ� �ʿ����. ������ �ε� �� �ϸ� ������ �˾Ƽ� �Ҹ�
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

	//�ε� �� �Ǹ� �� ü����
	if (currentCount == 100)
	{
		SCENEMANAGER->changeScene("�ΰ�");
	}
}

void LoadingScene::render(void)
{
	_background->render(getMemDC(), 0, 0);
	_loadingBar->render();
}

//�ε��� �� �����尡 ����.
//�ε尡 ������ �����嵵 �˾Ƽ� �����ȴ�.
DWORD CALLBACK ThreadFunction(LPVOID lpParamenter)
{
	//�ڱ� �ڽ� Ŭ������ ����� ����ȯ���� �޾ƿ´�.
	LoadingScene* LoadHelper = (LoadingScene*)lpParamenter;

	//�ε� ī��Ʈ�� 100�� �� �� ���� �����ش�.
	while (LoadHelper->currentCount != 100)
	{
		//�ε� ���÷� ���� õ ���� �����ִ� ����(�ǹ̾��� ��¥!!!!!!!!!)
		for (int i = 0; i < 1000; ++i)
		{
			IMAGEMANAGER->addImage("����", "������.bmp", 800, 600, false, NULL);
			
			IMAGEMANAGER->addImage("����1", "������.bmp", 800, 600, false, NULL); 
			IMAGEMANAGER->addImage("����2", "������.bmp", 800, 600, false, NULL); 
			IMAGEMANAGER->addImage("����3", "������.bmp", 800, 600, false, NULL); 
			IMAGEMANAGER->addImage("����4", "������.bmp", 800, 600, false, NULL); 
			IMAGEMANAGER->addImage("����5", "������.bmp", 800, 600, false, NULL); 
			IMAGEMANAGER->addImage("����6", "������.bmp", 800, 600, false, NULL); 
			IMAGEMANAGER->addImage("����7", "������.bmp", 800, 600, false, NULL); 
			printf_s("�ε���\n");
		}
		LoadHelper->currentCount++;
	}
	//�ε� �� �Ǹ� ������ �˾Ƽ� ����
	return 0;
}
