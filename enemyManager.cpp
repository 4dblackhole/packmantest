#include "stdafx.h"
#include "enemyManager.h"
#include "spaceShip.h"


enemyManager::enemyManager() { }


enemyManager::~enemyManager() {

}

HRESULT enemyManager::init() {
	_bullet = new bullet;
	_bullet->init("bullet", 30, 700);

	return S_OK;
}

void enemyManager::release() {

}

void enemyManager::update() {
	for (_viMinion = _vMinion.begin(); _viMinion != _vMinion.end(); ++_viMinion) {
		(*_viMinion)->update();
	}

	minionBulletFire();
	_bullet->update();
	//***************************************추가
	collision();
}

void enemyManager::render() {
	for (_viMinion = _vMinion.begin(); _viMinion != _vMinion.end(); ++_viMinion) {
		(*_viMinion)->render();
	}

	_bullet->render();
}


//적 생성 함수
void enemyManager::setMinion(void) {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 6; j++) {
			enemy* ufo;
			ufo = new minion;
			ufo->init("enemy", PointMake(80 + j * 80, 80 + i * 100));

			_vMinion.push_back(ufo);
		}
	}
}

//적 총알 발사함수
void enemyManager::minionBulletFire(void) 
{
	for (_viMinion = _vMinion.begin(); _viMinion != _vMinion.end(); ++_viMinion)
	{
		if ((*_viMinion)->bulletCountFire()) 
		{
			RECT rc = (*_viMinion)->getRect();
			/*
			_bullet->fire(rc.left + (rc.right - rc.left) / 2,
				rc.bottom + 5, -(PI / 2), 7.0f);
				*/
			//이걸로 교체
			_bullet->fire(rc.left + (rc.right - rc.left) / 2,
				rc.bottom + 5,
				getAngle(rc.left + (rc.right - rc.left) / 2,
					rc.bottom + 5,
					_ship->getShipImage()->getCenterX(),
					_ship->getShipImage()->getY()
				), 7.0f);
		}
	}
}

void enemyManager::removeMinion(int arrNum)
{
	SAFE_DELETE(_vMinion[arrNum]);
	_vMinion.erase(_vMinion.begin() + arrNum);
}

void enemyManager::collision(void)
{
	for (int i = 0; i < _bullet->getVBullet().size(); i++)
	{
		RECT temp;
		if (IntersectRect(&temp, &_bullet->getVBullet()[i].rc,
			&RectMake(_ship->getShipImage()->getX(),
				_ship->getShipImage()->getY(),
				_ship->getShipImage()->getWidth(),
				_ship->getShipImage()->getHeight())))
		{
			_ship->hitDamager(10.0f);
			_bullet->removeBullet(i);
			break;
		}
	}
}
