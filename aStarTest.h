#pragma once
#include "gameNode.h"
#include "tile.h"
#include <vector>
#include <string>

#define TILEWIDTH 32  //Ÿ���� ���α���
#define TILEHEIGHT 32 //Ÿ���� ���α���

#define TILENUMX (WINSIZEX / TILEWIDTH) //Ÿ�� ����
#define TILENUMY (WINSIZEY / TILEHEIGHT) //Ÿ�� ����
class aStarTest : public gameNode
{
private:
	//��ü Ÿ�� ���� ����
	vector<tile*>           _vTotalList;
	vector<tile*>::iterator _viTotalList;

	//�� �� �ִ� Ÿ�� ���� ����
	vector<tile*>           _vOpenList;
	vector<tile*>::iterator _viOpenList;

	//���� ����� ��θ� ���� ����
	vector<tile*>           _vCloseList;
	vector<tile*>::iterator _viCloseList;

	tile* _startTile;
	tile* _endTile;
	tile* _currentTile;

	int _count;
	bool _start;

public:
	aStarTest() {}
	~aStarTest() {}

	HRESULT init();
	
	//Ÿ�� ���� �Լ�
	void setTiles();

	//�� �� �ִ� ���� ã�� ����� �Լ�
	vector<tile*> addOpenList(tile* currentTile);

	//�� ã�� �Լ�
	//����  = ����Լ� �˾ƿ���
	//���ÿ����÷ο�
	void pathFinder(tile* currentTile);

	void release();
	void update();
	void render();
};

