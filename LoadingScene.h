#pragma once
#include "gameNode.h"

class progressBar;
class LoadingScene : public gameNode
{
public:
	progressBar * _loadingBar;
	image* _background;

	int currentCount;
public:
	LoadingScene();
	~LoadingScene();

	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);
};

static DWORD CALLBACK ThreadFunction(LPVOID lpParamenter);

