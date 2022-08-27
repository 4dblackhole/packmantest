#pragma once
#include "tileNode.h"
#include "gameNode.h"

class tankMap : public gameNode
{
private:
	tagTile _tiles[TILEX * TILEY];  //Ÿ��
	DWORD _attribute[TILEX * TILEY];	//Ÿ�� �Ӽ�

	int _pos[2];
public:
	tankMap();
	~tankMap();

	HRESULT init();
	void release();
	void update();
	void render();

	void load();

	//Ÿ�� �Ӽ��� �ܺο��� ���� �� �� �ִ� �����ڸ� ������
	DWORD* getTileAttribute() { return _attribute; }

	tagTile* getTile() { return _tiles; }

	int getPosFirst() { return _pos[0]; }
	int getPosSecond() { return _pos[1]; }
};

