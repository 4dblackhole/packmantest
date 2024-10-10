#include "stdafx.h"
#include "mainGame.h"

constexpr float defaultSpeed = 2.0f;
constexpr float mob1Speed = 1.5f;

mainGame::mainGame() {}

mainGame::~mainGame() {}

//초기화
HRESULT mainGame::init(void)
{
	//upcasting : 부모클래스를 자식클래스에서 호출한다
	gameNode::init(true);  //게임노드의 init 함수에 타이머 셋팅 및 키매니져 초기화 관련 함수를 실행한다.

	IMAGEMANAGER->addImage("title", "img\\title21039040.bmp", 608, 608, false, NULL);
	IMAGEMANAGER->addImage("cursor", "img\\selectkey.bmp", 20, 20, false, NULL);
	imgmaze = IMAGEMANAGER->addFrameImage("maze", "img\\maze.bmp", 416, 32, 26, 2, false, NULL);
	imgmob = IMAGEMANAGER->addFrameImage("mob", "img\\mob.bmp", 256, 48, 16, 3, false, NULL);
	imgnumber = IMAGEMANAGER->addFrameImage("number", "img\\number.bmp", 416, 32, 26, 2, false, NULL);
	imgpacman = IMAGEMANAGER->addFrameImage("pacman", "img\\pacman.bmp", 256, 48, 16, 3, false, NULL);

	IMAGEMANAGER->addImage("maptoolpanel", "img\\maptool.bmp", 80, 160, false, NULL);
	imgmaptool = IMAGEMANAGER->addFrameImage("maptool", "img\\maptool10x10.bmp", 20, 20, 2, 2, false, NULL);
	title = 0;
	selectkey = 0;
	vmob.clear();
	score = 0;
	mazeload();
	scoreload();

	int mobIdx = 0;
	for (int tileY = 0; tileY < TILENUMY; tileY++)
	{
		for (int tileX = 0; tileX < TILENUMX; tileX++)
		{
			maze[tileY][tileX].r = RectMake(tileX*TILESIZE, tileY*TILESIZE, TILESIZE, TILESIZE);
			
			if (maze[tileY][tileX].att == TILE::TileAttribute::PLAYER)
			{
				player.reset(new Player(tileX, tileY, defaultSpeed));
			}
			if (maze[tileY][tileX].att == TILE::TileAttribute::MOB)
			{
				ICharacter* tempMob;
				if (mobIdx == 0) //mob1
				{
					tempMob = new Monster1(tileX, tileY, mob1Speed);
					Monster1* ptr = dynamic_cast<Monster1*>(tempMob);
					ptr->refMaze = &maze;
				}
				else if (mobIdx == 2) // mob3
				{
					tempMob = new Monster3(tileX, tileY, defaultSpeed);
				}
				else
				{
					tempMob = new Monster2(tileX, tileY, defaultSpeed);
					Monster2* ptr = dynamic_cast<Monster2*>(tempMob);
				}
				vmob.push_back(tempMob);
				++mobIdx;
			}
		}
	}

	for (auto& it : vmob)
	{
		Monster1* m1 = dynamic_cast<Monster1*>(it);
		if (m1 != nullptr)
		{
			m1->target = player.get();
			continue;
		}
		Monster2* m2 = dynamic_cast<Monster2*>(it);
		if (m2 != nullptr)
		{
			m2->target = player.get();
			continue;
		}
	}
	
	savebutton = RectMake(520, 80, 80, 40);
	loadbutton = RectMake(520, 120, 80, 40);
	wallbutton = RectMake(520, 0, 40, 40);
	nullbutton = RectMake(560, 0, 40, 40);
	mobbutton = RectMake(520, 40, 40, 40);
	pacmanbutton = RectMake(560, 40, 40, 40);
	selecttile = 2;

	return S_OK;
}

//메모리 해제
void mainGame::release(void)
{
	for (ICharacter*& it : vmob) delete it;
	gameNode::release(); //손대지 마셈
}

//연산은 여기서 한다
void mainGame::update(void)
{
	gameNode::update();  //손대지 마셈

	if (title == 0)//타이틀
	{
		if (KEYMANAGER->isOnceKeyDown(VK_UP) && selectkey > 0)
		{
			selectkey--;
		}
		if (KEYMANAGER->isOnceKeyDown(VK_DOWN) && selectkey < 3)
		{
			selectkey++;
		}
		if (KEYMANAGER->isOnceKeyDown('Z'))
		{
			if (selectkey == 0)
			{
				for (int i = 0; i < TILENUMY; i++)
				{
					for (int k = 0; k < TILENUMX; k++)
					{
						if (maze[i][k].att == 3)
						{
							player->exist = true;
							title = 1;
							break;
						}
					}
				}
			}
			if (selectkey == 1)
			{
				title = 2;
			}
			if (selectkey == 2)
			{
				selectkey = 0;
				title = 3;
			}
			if (selectkey == 3)
			{
				PostQuitMessage(0);
			}
			return;
		}
	}
	else if (title == 1)//게임
	{
		if (KEYMANAGER->isStayKeyDown('S') && KEYMANAGER->isStayKeyDown('D') && KEYMANAGER->isStayKeyDown('F'))init();
		RECT temp;
		for (auto& mobIter : vmob)
		{
			if (IntersectRect(&temp, &player->rc, &mobIter->rc))
			{
				if (scoreview < score)scoresave();
				init();
				break;
			}
		}
		player->MoveTile(&maze);
		player->UpdateRect(player->x, player->y, imgpacman->getFrameWidth(), imgpacman->getFrameHeight());
		
		for (int i = 0; i < vmob.size(); i++)
		{
			vmob[i]->MoveTile(&maze);
			vmob[i]->UpdateRect(vmob[i]->x, vmob[i]->y, imgmob->getFrameWidth(), imgmob->getFrameHeight());
		}

		if (maze[player->ontiley][player->ontilex].att == TILE::TileAttribute::ITEM)
		{
			maze[player->ontiley][player->ontilex].att = TILE::TileAttribute::EMPTY;
			score+=10;
		}
	}
	else if (title == 2)//맵툴
	{
		if (KEYMANAGER->isOnceKeyDown('X'))init();
		for (int i = 0; i < TILENUMY; i++)
		{
			for (int k = 0; k < TILENUMX; k++)
			{
				for (int ss = 1; ss < 5; ss++)
				{
					if (KEYMANAGER->isStayKeyDown(VK_LBUTTON) && RectPointCollision(RectMake(k * 10, i * 10, 10, 10), _ptMouse) && selecttile == ss)
					{
						if (ss == 3 && !player->exist)
						{
							player->exist = true;
							maze[i][k].att = ss;
						}
						else if (ss == 3 && player->exist) {}
						else if (maze[i][k].att == 3)
						{
							player->exist = false;
							maze[i][k].att = ss;
						}
						else maze[i][k].att = ss;
					}
				}
			}
		}
		if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
		{
			if (RectPointCollision(nullbutton, _ptMouse))selecttile = 1;//아이템
			if (RectPointCollision(wallbutton, _ptMouse))selecttile = 2;//벽
			if (RectPointCollision(pacmanbutton, _ptMouse))selecttile = 3;//플레이어
			if (RectPointCollision(mobbutton, _ptMouse))selecttile = 4;//몹

			if (RectPointCollision(savebutton, _ptMouse))
			{
				mazesave();
				MessageBeep(MB_OK);
			}
			if (RectPointCollision(loadbutton, _ptMouse))
			{
				mazeload();
				MessageBeep(MB_OK);
			}
		}
	}
	else if (title == 3)//옵션
	{
		if (KEYMANAGER->isOnceKeyDown('X'))title=0;
		if (KEYMANAGER->isOnceKeyDown(VK_UP)&&player->speed < 5)
		{
			player->speed += 1;
		}
		if (KEYMANAGER->isOnceKeyDown(VK_DOWN)&&player->speed > 1)
		{
			player->speed -= 1;
		}
	}
}

//그리는 것은 이제 여기에다
void mainGame::render()
{
	PatBlt(getMemDC(), 0, 0, WINSIZEX, WINSIZEY, WHITENESS);
	//================== 이 위는 손대지 마시오 =========================
	if (title == 0)
	{
		IMAGEMANAGER->findImage("title")->render(getMemDC());
		for (int i = 0; i < 4; i++)
		{
			if (selectkey == i)IMAGEMANAGER->findImage("cursor")->render(getMemDC(), 210, 390 + 41 * i);
		}
	}
	if (title == 1)
	{
		for (int i = 0; i < TILENUMY; i++)
		{
			for (int k = 0; k < TILENUMX; k++)
			{
				if (maze[i][k].att == 0)imgmaze->frameRender(getMemDC(), maze[i][k].r.left, maze[i][k].r.top, 19, 1);
				if (maze[i][k].att == 1)imgmaze->frameRender(getMemDC(), maze[i][k].r.left, maze[i][k].r.top, 16, 1);
				if (maze[i][k].att == 2)
				{
					if (i > 0 && i < TILENUMY - 1 && k>0 && k < TILENUMX - 1)
					{
						if (maze[i][k - 1].att != 2 && maze[i][k + 1].att != 2 && maze[i - 1][k].att != 2 && maze[i + 1][k].att != 2)imgmaze->frameRender(getMemDC(), maze[i][k].r.left, maze[i][k].r.top, 0, 0);
						if (maze[i][k - 1].att == 2 && maze[i][k + 1].att != 2 && maze[i - 1][k].att != 2 && maze[i + 1][k].att != 2)imgmaze->frameRender(getMemDC(), maze[i][k].r.left, maze[i][k].r.top, 1, 0);
						if (maze[i][k - 1].att != 2 && maze[i][k + 1].att != 2 && maze[i - 1][k].att == 2 && maze[i + 1][k].att != 2)imgmaze->frameRender(getMemDC(), maze[i][k].r.left, maze[i][k].r.top, 2, 0);
						if (maze[i][k - 1].att != 2 && maze[i][k + 1].att == 2 && maze[i - 1][k].att != 2 && maze[i + 1][k].att != 2)imgmaze->frameRender(getMemDC(), maze[i][k].r.left, maze[i][k].r.top, 3, 0);
						if (maze[i][k - 1].att != 2 && maze[i][k + 1].att != 2 && maze[i - 1][k].att != 2 && maze[i + 1][k].att == 2)imgmaze->frameRender(getMemDC(), maze[i][k].r.left, maze[i][k].r.top, 4, 0);
						if (maze[i][k - 1].att == 2 && maze[i][k + 1].att != 2 && maze[i - 1][k].att == 2 && maze[i + 1][k].att != 2)imgmaze->frameRender(getMemDC(), maze[i][k].r.left, maze[i][k].r.top, 5, 0);
						if (maze[i][k - 1].att == 2 && maze[i][k + 1].att == 2 && maze[i - 1][k].att != 2 && maze[i + 1][k].att != 2)imgmaze->frameRender(getMemDC(), maze[i][k].r.left, maze[i][k].r.top, 6, 0);
						if (maze[i][k - 1].att == 2 && maze[i][k + 1].att != 2 && maze[i - 1][k].att != 2 && maze[i + 1][k].att == 2)imgmaze->frameRender(getMemDC(), maze[i][k].r.left, maze[i][k].r.top, 7, 0);
						if (maze[i][k - 1].att != 2 && maze[i][k + 1].att == 2 && maze[i - 1][k].att == 2 && maze[i + 1][k].att != 2)imgmaze->frameRender(getMemDC(), maze[i][k].r.left, maze[i][k].r.top, 8, 0);
						if (maze[i][k - 1].att != 2 && maze[i][k + 1].att != 2 && maze[i - 1][k].att == 2 && maze[i + 1][k].att == 2)imgmaze->frameRender(getMemDC(), maze[i][k].r.left, maze[i][k].r.top, 9, 0);
						if (maze[i][k - 1].att != 2 && maze[i][k + 1].att == 2 && maze[i - 1][k].att != 2 && maze[i + 1][k].att == 2)imgmaze->frameRender(getMemDC(), maze[i][k].r.left, maze[i][k].r.top, 10, 0);
						if (maze[i][k - 1].att != 2 && maze[i][k + 1].att == 2 && maze[i - 1][k].att == 2 && maze[i + 1][k].att == 2)imgmaze->frameRender(getMemDC(), maze[i][k].r.left, maze[i][k].r.top, 11, 0);
						if (maze[i][k - 1].att == 2 && maze[i][k + 1].att == 2 && maze[i - 1][k].att != 2 && maze[i + 1][k].att == 2)imgmaze->frameRender(getMemDC(), maze[i][k].r.left, maze[i][k].r.top, 12, 0);
						if (maze[i][k - 1].att == 2 && maze[i][k + 1].att != 2 && maze[i - 1][k].att == 2 && maze[i + 1][k].att == 2)imgmaze->frameRender(getMemDC(), maze[i][k].r.left, maze[i][k].r.top, 13, 0);
						if (maze[i][k - 1].att == 2 && maze[i][k + 1].att == 2 && maze[i - 1][k].att == 2 && maze[i + 1][k].att != 2)imgmaze->frameRender(getMemDC(), maze[i][k].r.left, maze[i][k].r.top, 14, 0);
						if (maze[i][k - 1].att == 2 && maze[i][k + 1].att == 2 && maze[i - 1][k].att == 2 && maze[i + 1][k].att == 2)imgmaze->frameRender(getMemDC(), maze[i][k].r.left, maze[i][k].r.top, 15, 0);
					}
					if (i == 0)
					{
						if (k == 0)imgmaze->frameRender(getMemDC(), maze[i][k].r.left, maze[i][k].r.top, 10, 0);
						else if (k == TILENUMX-1)imgmaze->frameRender(getMemDC(), maze[i][k].r.left, maze[i][k].r.top, 7, 0);
						else
						{
							if (maze[i][k - 1].att != 2 && maze[i][k + 1].att != 2 && maze[i - 1][k].att != 2 && maze[i + 1][k].att != 2)imgmaze->frameRender(getMemDC(), maze[i][k].r.left, maze[i][k].r.top, 0, 0);
							if (maze[i][k - 1].att == 2 && maze[i][k + 1].att != 2 && maze[i - 1][k].att != 2 && maze[i + 1][k].att != 2)imgmaze->frameRender(getMemDC(), maze[i][k].r.left, maze[i][k].r.top, 1, 0);
							if (maze[i][k - 1].att != 2 && maze[i][k + 1].att == 2 && maze[i - 1][k].att != 2 && maze[i + 1][k].att != 2)imgmaze->frameRender(getMemDC(), maze[i][k].r.left, maze[i][k].r.top, 3, 0);
							if (maze[i][k - 1].att != 2 && maze[i][k + 1].att != 2 && maze[i - 1][k].att != 2 && maze[i + 1][k].att == 2)imgmaze->frameRender(getMemDC(), maze[i][k].r.left, maze[i][k].r.top, 4, 0);
							if (maze[i][k - 1].att != 2 && maze[i][k + 1].att != 2 && maze[i - 1][k].att != 2 && maze[i + 1][k].att == 2)imgmaze->frameRender(getMemDC(), maze[i][k].r.left, maze[i][k].r.top, 4, 0);
							if (maze[i][k - 1].att == 2 && maze[i][k + 1].att == 2 && maze[i - 1][k].att != 2 && maze[i + 1][k].att != 2)imgmaze->frameRender(getMemDC(), maze[i][k].r.left, maze[i][k].r.top, 6, 0);
							if (maze[i][k - 1].att == 2 && maze[i][k + 1].att == 2 && maze[i - 1][k].att != 2 && maze[i + 1][k].att == 2)imgmaze->frameRender(getMemDC(), maze[i][k].r.left, maze[i][k].r.top, 12, 0);
						}
					}
					else if (i == TILENUMY - 1)
					{
						if (k == 0)imgmaze->frameRender(getMemDC(), maze[i][k].r.left, maze[i][k].r.top, 8, 0);
						else if (k == TILENUMX - 1)imgmaze->frameRender(getMemDC(), maze[i][k].r.left, maze[i][k].r.top, 5, 0);
						else
						{
							if (maze[i][k - 1].att != 2 && maze[i][k + 1].att != 2 && maze[i - 1][k].att != 2 && maze[i + 1][k].att != 2)imgmaze->frameRender(getMemDC(), maze[i][k].r.left, maze[i][k].r.top, 0, 0);
							if (maze[i][k - 1].att == 2 && maze[i][k + 1].att != 2 && maze[i - 1][k].att != 2 && maze[i + 1][k].att != 2)imgmaze->frameRender(getMemDC(), maze[i][k].r.left, maze[i][k].r.top, 1, 0);
							if (maze[i][k - 1].att != 2 && maze[i][k + 1].att != 2 && maze[i - 1][k].att == 2 && maze[i + 1][k].att != 2)imgmaze->frameRender(getMemDC(), maze[i][k].r.left, maze[i][k].r.top, 2, 0);
							if (maze[i][k - 1].att != 2 && maze[i][k + 1].att == 2 && maze[i - 1][k].att != 2 && maze[i + 1][k].att != 2)imgmaze->frameRender(getMemDC(), maze[i][k].r.left, maze[i][k].r.top, 3, 0);
							if (maze[i][k - 1].att == 2 && maze[i][k + 1].att == 2 && maze[i - 1][k].att != 2 && maze[i + 1][k].att != 2)imgmaze->frameRender(getMemDC(), maze[i][k].r.left, maze[i][k].r.top, 6, 0);
							if (maze[i][k - 1].att == 2 && maze[i][k + 1].att == 2 && maze[i - 1][k].att == 2 && maze[i + 1][k].att != 2)imgmaze->frameRender(getMemDC(), maze[i][k].r.left, maze[i][k].r.top, 14, 0);
						}
					}
					if (k == 0 && i != 0 && i != TILENUMY - 1)
					{
						if (maze[i][k - 1].att == 2 && maze[i][k + 1].att != 2 && maze[i - 1][k].att != 2 && maze[i + 1][k].att != 2)imgmaze->frameRender(getMemDC(), maze[i][k].r.left, maze[i][k].r.top, 0, 0);
						if (maze[i][k - 1].att == 2 && maze[i][k + 1].att != 2 && maze[i - 1][k].att == 2 && maze[i + 1][k].att != 2)imgmaze->frameRender(getMemDC(), maze[i][k].r.left, maze[i][k].r.top, 2, 0);
						if (maze[i][k - 1].att == 2 && maze[i][k + 1].att == 2 && maze[i - 1][k].att != 2 && maze[i + 1][k].att != 2)imgmaze->frameRender(getMemDC(), maze[i][k].r.left, maze[i][k].r.top, 3, 0);
						if (maze[i][k - 1].att == 2 && maze[i][k + 1].att != 2 && maze[i - 1][k].att != 2 && maze[i + 1][k].att == 2)imgmaze->frameRender(getMemDC(), maze[i][k].r.left, maze[i][k].r.top, 4, 0);
						if (maze[i][k - 1].att == 2 && maze[i][k + 1].att == 2 && maze[i - 1][k].att == 2 && maze[i + 1][k].att != 2)imgmaze->frameRender(getMemDC(), maze[i][k].r.left, maze[i][k].r.top, 8, 0);
						if (maze[i][k - 1].att == 2 && maze[i][k + 1].att != 2 && maze[i - 1][k].att == 2 && maze[i + 1][k].att == 2)imgmaze->frameRender(getMemDC(), maze[i][k].r.left, maze[i][k].r.top, 9, 0);
						if (maze[i][k - 1].att == 2 && maze[i][k + 1].att == 2 && maze[i - 1][k].att != 2 && maze[i + 1][k].att == 2)imgmaze->frameRender(getMemDC(), maze[i][k].r.left, maze[i][k].r.top, 10, 0);
						if (maze[i][k - 1].att == 2 && maze[i][k + 1].att == 2 && maze[i - 1][k].att == 2 && maze[i + 1][k].att == 2)imgmaze->frameRender(getMemDC(), maze[i][k].r.left, maze[i][k].r.top, 11, 0);
					}
					if (k == TILENUMX - 1 && i != 0 && i != TILENUMY - 1)
					{
						if (maze[i][k - 1].att != 2 && maze[i][k + 1].att == 2 && maze[i - 1][k].att != 2 && maze[i + 1][k].att != 2)imgmaze->frameRender(getMemDC(), maze[i][k].r.left, maze[i][k].r.top, 0, 0);
						if (maze[i][k - 1].att == 2 && maze[i][k + 1].att == 2 && maze[i - 1][k].att != 2 && maze[i + 1][k].att != 2)imgmaze->frameRender(getMemDC(), maze[i][k].r.left, maze[i][k].r.top, 1, 0);
						if (maze[i][k - 1].att != 2 && maze[i][k + 1].att == 2 && maze[i - 1][k].att == 2 && maze[i + 1][k].att != 2)imgmaze->frameRender(getMemDC(), maze[i][k].r.left, maze[i][k].r.top, 2, 0);
						if (maze[i][k - 1].att != 2 && maze[i][k + 1].att == 2 && maze[i - 1][k].att != 2 && maze[i + 1][k].att == 2)imgmaze->frameRender(getMemDC(), maze[i][k].r.left, maze[i][k].r.top, 4, 0);
						if (maze[i][k - 1].att == 2 && maze[i][k + 1].att == 2 && maze[i - 1][k].att == 2 && maze[i + 1][k].att != 2)imgmaze->frameRender(getMemDC(), maze[i][k].r.left, maze[i][k].r.top, 5, 0);
						if (maze[i][k - 1].att == 2 && maze[i][k + 1].att == 2 && maze[i - 1][k].att != 2 && maze[i + 1][k].att == 2)imgmaze->frameRender(getMemDC(), maze[i][k].r.left, maze[i][k].r.top, 7, 0);
						if (maze[i][k - 1].att != 2 && maze[i][k + 1].att == 2 && maze[i - 1][k].att == 2 && maze[i + 1][k].att == 2)imgmaze->frameRender(getMemDC(), maze[i][k].r.left, maze[i][k].r.top, 9, 0);
						if (maze[i][k - 1].att == 2 && maze[i][k + 1].att == 2 && maze[i - 1][k].att == 2 && maze[i + 1][k].att == 2)imgmaze->frameRender(getMemDC(), maze[i][k].r.left, maze[i][k].r.top, 13, 0);
					}
				}
				if (maze[i][k].att == 3)
				{
					imgmaze->frameRender(getMemDC(), maze[i][k].r.left, maze[i][k].r.top, 19, 1);
				}
				if (maze[i][k].att == 4)
				{
					imgmaze->frameRender(getMemDC(), maze[i][k].r.left, maze[i][k].r.top, 19, 1);
				}
			}
		}
		for (auto& mobIter : vmob)
		{
			imgmob->frameRender(getMemDC(), mobIter->rc.left, mobIter->rc.top, 0, 0);
		}
		imgpacman->frameRender(getMemDC(), player->rc.left, player->rc.top, 4, 0);
		for (int i = 0; i < 6; i++)
		{
			for (int k = 0; k < 10; k++)
			{
				if ((score / (int)pow(10, i))%10 == k)imgnumber->frameRender(getMemDC(), WINSIZEX / 2 - TILESIZE * (i - 3), 0, k, 0);
			}
		}

		if (KEYMANAGER->isTogglekey('A'))
		{
			for (auto& mobIter : vmob)
			{
				Monster1* ptr = dynamic_cast<Monster1*>(mobIter);
				if (ptr != nullptr) ptr->DebugRender(getMemDC(), imgnumber);
			}
		}
	}
	if (title == 2)
	{
		for (int i = 0; i < TILENUMY; i++)
		{
			for (int k = 0; k < TILENUMX; k++)
			{
				if (maze[i][k].att == 0 || maze[i][k].att == 1)
				{
					imgmaptool->setFrameX(1);
					imgmaptool->setFrameY(0);
				}
				if (maze[i][k].att == 2)
				{
					imgmaptool->setFrameX(0);
					imgmaptool->setFrameY(0);
				}
				if (maze[i][k].att == 3)
				{
					imgmaptool->setFrameX(1);
					imgmaptool->setFrameY(1);
				}
				if (maze[i][k].att == 4)
				{
					imgmaptool->setFrameX(0);
					imgmaptool->setFrameY(1);
				}
				imgmaptool->frameRender(getMemDC(), imgmaptool->getFrameWidth() * k, imgmaptool->getFrameWidth() * i);

			}
		}
		IMAGEMANAGER->findImage("maptoolpanel")->render(getMemDC(), 520, 0);

		char str[128];
		wsprintf(str, "SCORE : %d", scoreview);
		TextOut(getMemDC(), 20, WINSIZEY - 50 , str, lstrlen(str));
	}
	if (title == 3)
	{
		char ss[128];
		sprintf_s(ss, "speed : %d", (int)player->speed);
		TextOut(getMemDC(), 300, 350, ss, strlen(ss));
	}
	
	/*SetTextColor(getMemDC(), RGB(255, 255, 255));
	char sss[128];
	sprintf_s(sss, "%d", vmob[0].direction);
	TextOut(getMemDC(), 500, 500, sss, strlen(sss));*/
	TIMEMANAGER->render(getMemDC());
	//================== 이 아래는 손대지 마시오 ========================
	this->getBackBuffer()->render(getHDC(), 0, 0);
}

bool mainGame::RectPointCollision(RECT rc, POINT pt)
{
	if (pt.x > rc.left&&pt.x<rc.right&&pt.y>rc.top&& pt.y < rc.bottom)return true;
	else return false;
}

void mainGame::mazesave()
{
	HANDLE file;
	DWORD write;

	file = CreateFile("MAZE.txt", GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	for (int i = 0; i < TILENUMY; i++)
	{
		for (int k = 0; k < TILENUMX; k++)
		{
			WriteFile(file, &maze[i][k].att, sizeof(int), &write, NULL);
			if (maze[i][k].att == 4)
			{
				int  fads = 324;
			}
		}
	}
	CloseHandle(file);
}

void mainGame::mazeload()
{
	HANDLE file;
	DWORD read;

	file = CreateFile("MAZE.txt", GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	for (int i = 0; i < TILENUMY; i++)
	{
		for (int k = 0; k < TILENUMX; k++)
		{
			ReadFile(file, &maze[i][k].att, sizeof(int), &read, NULL);
		}
	}
	CloseHandle(file);
}


void mainGame::scoresave()
{
	HANDLE file;
	DWORD write;

	file = CreateFile("MAZESCORE.txt", GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	
	WriteFile(file, &score, sizeof(int), &write, NULL);
	
	CloseHandle(file);
}

void mainGame::scoreload()
{
	HANDLE file;
	DWORD read;

	file = CreateFile("MAZESCORE.txt", GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	ReadFile(file, &scoreview, sizeof(int), &read, NULL);
	
	CloseHandle(file);
}
