#pragma once
#include "gameNode.h"
#include "bullets.h"
#include "progressBar.h"

class enemyManager;

class spaceShip : public gameNode
{
private:
	image* _ship;
	missile* _missile;
	heptaho* _heptaho;

	enemyManager* _em;

	progressBar* _hpBar;

	float _currentHP, _maxHP;

	int _alphaValue;

public:
	virtual HRESULT init(void);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	void collision(void);

	void hitDamager(float damage);

	image* getShipImage() { return _ship; }

	heptaho* getHeptahoMissile() { return _heptaho; }

	void setEmMemoryAddressLink(enemyManager* em) { _em = em; }

	spaceShip();
	~spaceShip();
};

