#pragma once
#include "gameNode.h"
#include "ship.h"

class battle;

class starcraftScene : public gameNode
{
private:
	ship * _battle;
public:
	starcraftScene(){}
	~starcraftScene(){}

	HRESULT init();
	void release();
	void update();
	void render();
};

