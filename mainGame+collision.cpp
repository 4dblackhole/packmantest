#include "stdafx.h"
#include "mainGame.h"

//객체지향적인 코딩방식이 생겨나면서 생겨난 방법인데
//한 개의 헤더파일은 여러개의 CPP 파일을 관리할 수 있는데
//게임프로그래밍 특징 상 코드량이 굉장히 많기 때문에
//init 전용 cpp
//release 전용 cpp
//update 전용 cpp
//충돌 전용 cpp 
//등으로 나눠서 코드를 가독성 + 관리를 용이하게 하는 방식이다


void mainGame::collision()
{
	for (int i = 0; i < _ship->getHeptahoMissile()->getVBullet().size(); i++)
	{
		for (int j = 0; j < _em->getVMinion().size(); j++)
		{
			RECT temp;
			if (IntersectRect(&temp,
				&_em->getVMinion()[j]->getRect(),
				&_ship->getHeptahoMissile()->getVBullet()[i].rc))
			{
				_em->removeMinion(j);
				_ship->getHeptahoMissile()->removeMissile(i);
				break;
			}
		}
	}
}