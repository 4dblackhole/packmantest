#pragma once
#include "gameNode.h"
#include "minion.h"
#include "bullets.h"
#include <vector>

//전방선언의 이유
//

class spaceShip;

class enemyManager : public gameNode {
private:
	typedef vector<enemy*> vEnemy;
	typedef vector<enemy*>::iterator viEnemy;

private:
	vEnemy _vMinion;
	viEnemy _viMinion;

	bullet* _bullet;

	spaceShip* _ship;

public:
	enemyManager();
	~enemyManager();

	HRESULT init();
	void release();
	void update();
	void render();

	//적 생성 함수
	void setMinion(void);
	//적 총알 발사함수
	void minionBulletFire(void);
	//*************************************************************************
	void removeMinion(int arrNum);

	void collision(void);

	void setSpaceShipMemoryAddressLink(spaceShip* ship) { _ship = ship; }
	//*************************************************************************
	//미니언 벡터 접근자
	vector<enemy*> getVMinion() { return _vMinion; }
	vector<enemy*>::iterator getVIMinion() { return _viMinion; }

};

