#pragma once
#include "tileNode.h"
#include "gameNode.h"

class tankMap : public gameNode
{
private:
	tagTile _tiles[TILEX * TILEY];  //타일
	DWORD _attribute[TILEX * TILEY];	//타일 속성

	int _pos[2];
public:
	tankMap();
	~tankMap();

	HRESULT init();
	void release();
	void update();
	void render();

	void load();

	//타일 속성을 외부에서 접근 할 수 있는 접근자를 만들자
	DWORD* getTileAttribute() { return _attribute; }

	tagTile* getTile() { return _tiles; }

	int getPosFirst() { return _pos[0]; }
	int getPosSecond() { return _pos[1]; }
};

