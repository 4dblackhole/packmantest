#include "stdafx.h"
#include "sceneManager.h"
#include "gameNode.h"

//����ƽ���� ���� �Ǿ������Ƿ� �ʱ�ȭ�� ���⼭
gameNode* sceneManager::_currentScene = NULL;
gameNode* sceneManager::_loadingScene = NULL;
gameNode* sceneManager::_readyScene = NULL;

HRESULT sceneManager::init(void)
{
	_currentScene = NULL;
	_loadingScene = NULL;
	_readyScene = NULL;
	
	return S_OK;
}

void sceneManager::release(void)
{
	mapSceneIter miSceneList = _mSceneList.begin();

	for (; miSceneList != _mSceneList.end();)
	{
		if (miSceneList->second != NULL)
		{
			if (miSceneList->second == _currentScene) miSceneList->second->release();
			SAFE_DELETE(miSceneList->second);
			miSceneList = _mSceneList.erase(miSceneList);
		}
		else ++miSceneList;
	}

	_mSceneList.clear();
}

void sceneManager::update(void)
{
	if (_currentScene) _currentScene->update();
}

void sceneManager::render(void)
{
	if (_currentScene) _currentScene->render();
}

//�� �߰� �Լ�
gameNode * sceneManager::addScene(string sceneName, gameNode * scene)
{
	//���� ���ٸ� �ΰ� ��ȯ
	if (!scene) return NULL;

	//���� Ű ���� �ο��ؼ� map�����̳ʿ� ����ش�
	_mSceneList.insert(make_pair(sceneName, scene));

	return scene;
}

//�� ����
HRESULT sceneManager::changeScene(string sceneName)
{
	mapSceneIter find = _mSceneList.find(sceneName);

	//���ͷ����Ͱ� ���� ������ ���� == ã�����ϴ°� ���� ���� ��ȯ
	if (find == _mSceneList.end()) return E_FAIL;

	//���� �ٲٷ��� ���� ���� ���̸� �׳� �д�
	if (find->second == _currentScene) return S_OK;

	if (SUCCEEDED(find->second->init()))
	{
		if (_currentScene) _currentScene->release();
		_currentScene = find->second;
		return S_OK;
	}
	return E_FAIL;
}
