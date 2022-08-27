#pragma once
#include "gameNode.h"

class InGameScene : public gameNode
{
public:
	InGameScene();
	~InGameScene();

	HRESULT init();
	void release();
	void update();
	void render();
};

