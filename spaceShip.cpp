#include "stdafx.h"
#include "spaceShip.h"
#include "enemyManager.h"


spaceShip::spaceShip()
{
}


spaceShip::~spaceShip()
{

}

HRESULT spaceShip::init(void)
{
	_ship = IMAGEMANAGER->addImage("플레이어", "rocket.bmp", 52, 64, true, RGB(255, 0, 255));

	_ship->setX(WINSIZEX / 2);
	_ship->setY(WINSIZEY / 2 + 200);

	_missile = new missile;
	_missile->init(30, 400);

	_heptaho = new heptaho;
	_heptaho->init(3000, 500);

	//추가
	//현재 체력 및 최대체력은 100으로 둔다
	_currentHP = _maxHP = 100;

	_hpBar = new progressBar;
	_hpBar->init(_ship->getX(), _ship->getY() - 20, 52, 10);
	_hpBar->setGauge(_currentHP, _maxHP);

	_alphaValue = 255;

	return S_OK;
}

void spaceShip::release(void)
{

}

void spaceShip::update(void)
{
	if (KEYMANAGER->isStayKeyDown(VK_LEFT) && _ship->getX() > 0)
	{
		_ship->setX(_ship->getX() - 5);
	}

	if (KEYMANAGER->isStayKeyDown(VK_RIGHT) && _ship->getX() + _ship->getWidth() < WINSIZEX)
	{
		_ship->setX(_ship->getX() + 5);
	}

	if (KEYMANAGER->isStayKeyDown(VK_UP) && _ship->getY() > 0)
	{
		_ship->setY(_ship->getY() - 5);
	}

	if (KEYMANAGER->isStayKeyDown(VK_DOWN) && _ship->getY() + _ship->getHeight() < WINSIZEY)
	{
		_ship->setY(_ship->getY() + 5);
	}

	if (KEYMANAGER->isOnceKeyDown(VK_SPACE)) _missile->fire(_ship->getCenterX(), _ship->getCenterY() - 50);

	if (KEYMANAGER->isStayKeyDown('X')) _heptaho->fire(_ship->getCenterX(), _ship->getY() - 15);

	_missile->update();
	_heptaho->update();
	_hpBar->setX(_ship->getX());
	_hpBar->setY(_ship->getY() - 20);
	_hpBar->setGauge(_currentHP, _maxHP);
	//우선순위가 이렇게 중요합니다 여러분!!!!	
	_hpBar->update();

	//저장키
	if (KEYMANAGER->isOnceKeyDown(VK_F1))
	{
		char temp[128];
		vector<string> vStr;

		vStr.push_back(itoa(_currentHP, temp, 10));
		vStr.push_back(itoa(_maxHP, temp, 10));
		vStr.push_back(itoa(_ship->getX(), temp, 10));
		vStr.push_back(itoa(_ship->getY(), temp, 10));
		TXTDATA->txtSave("넵튠.txt", vStr);
	}
	//로드키
	if (KEYMANAGER->isOnceKeyDown(VK_F2))
	{
		vector<string> vStr;

		vStr = TXTDATA->txtLoad("넵튠.txt");

		_currentHP = (atoi(vStr[0].c_str()));
		_maxHP = (atoi(vStr[1].c_str()));
		_ship->setX(atoi(vStr[2].c_str()));
		_ship->setY(atoi(vStr[3].c_str()));
	}

	if (_alphaValue <= 0) _alphaValue = 255;
	_alphaValue--;

	collision();
}

void spaceShip::render(void)
{

	_ship->render(getMemDC(), _ship->getX(), _ship->getY());

	_missile->render();
	_heptaho->render();
	_hpBar->render();
}

void spaceShip::collision(void)
{
	for (int i = 0; i < _heptaho->getVBullet().size(); i++)
	{
		for (int j = 0; j < _em->getVMinion().size(); j++)
		{
			RECT temp;
			if (IntersectRect(&temp, &_heptaho->getVBullet()[i].rc, &_em->getVMinion()[j]->getRect()))
			{
				_heptaho->removeMissile(i);
				_em->removeMinion(j);
				break;
			}
		}
	}
}

void spaceShip::hitDamager(float damage)
{
	_currentHP -= damage;
}

