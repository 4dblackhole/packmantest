#include "stdafx.h"
#include "sceneManager.h"
#include "gameNode.h"

//스태틱으로 선언 되어있으므로 초기화는 여기서
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

//씬 추가 함수
gameNode * sceneManager::addScene(string sceneName, gameNode * scene)
{
	//씬이 없다면 널값 반환
	if (!scene) return NULL;

	//씬에 키 값을 부여해서 map컨테이너에 담아준다
	_mSceneList.insert(make_pair(sceneName, scene));

	return scene;
}

//씬 변경
HRESULT sceneManager::changeScene(string sceneName)
{
	mapSceneIter find = _mSceneList.find(sceneName);

	//이터레이터가 맵의 끝까지 갔다 == 찾고자하는게 없다 실패 반환
	if (find == _mSceneList.end()) return E_FAIL;

	//만약 바꾸려는 씬이 현재 씬이면 그냥 둔다
	if (find->second == _currentScene) return S_OK;

	if (SUCCEEDED(find->second->init()))
	{
		if (_currentScene) _currentScene->release();
		_currentScene = find->second;
		return S_OK;
	}
	return E_FAIL;
}
