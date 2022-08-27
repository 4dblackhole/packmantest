#include "stdafx.h"
#include "mainGame.h"

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
	count = 0;
	title = 0;
	selectkey = 0;
	vmob.clear();
	vOpenList.clear();
	vCloseList.clear();
	vPathList.clear();
	isplayerexist = false;
	isend = false;
	score = 0;
	mazeload();
	scoreload();

	for (int i = 0; i < TILENUMY; i++)
	{
		for (int k = 0; k < TILENUMX; k++)
		{
			maze[i][k].r = RectMake(k*TILESIZE, i*TILESIZE, TILESIZE, TILESIZE);
			
			isopen[i][k] = 0;
			if (maze[i][k].att == 3)
			{
				isplayerexist = true;
				player.exist = true;
				player.x = k * TILESIZE;
				player.y = i * TILESIZE;
				player.ontilex = k;
				player.ontiley = i;
				player.speed = 2;
				player.direction = NOMOVE;
				player.r = RectMake(k*TILESIZE, i*TILESIZE, TILESIZE, TILESIZE);
			}
			if (maze[i][k].att == 4)
			{
				mob.exist = true;
				mob.x = k * TILESIZE;
				mob.y = i * TILESIZE;
				mob.ontilex = k;
				mob.ontiley = i;
				mob.speed = 2;
				mob.direction = NOMOVE;
				mob.r = RectMake(k*TILESIZE, i*TILESIZE, TILESIZE, TILESIZE);
				vmob.push_back(mob);
				maze[i][k].att = 0;
			}
		}
	}

	
	savebutton = RectMake(520, 80, 80, 40);
	loadbutton = RectMake(520, 120, 80, 40);
	wallbutton = RectMake(520, 0, 40, 40);
	nullbutton = RectMake(560, 0, 40, 40);
	mobbutton = RectMake(520, 40, 40, 40);
	pacmanbutton = RectMake(560, 40, 40, 40);
	selecttile = 2;
	mobamount = 0;

	return S_OK;
}

//메모리 해제
void mainGame::release(void)
{
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
				mobamount = 0;
				for (int i = 0; i < TILENUMY; i++)
				{
					for (int k = 0; k < TILENUMX; k++)
					{
						if (maze[i][k].att == 3)isplayerexist = true;
						if (maze[i][k].att == 4)mobamount++;
					}
				}
				if(isplayerexist)title = 1;
			}
			if (selectkey == 1)title = 2;
			if (selectkey == 2)
			{
				selectkey = 0;
				title = 3;
			}
			if (selectkey == 3)PostQuitMessage(0);
		}
	}
	if (title == 1)//게임
	{
		if (KEYMANAGER->isStayKeyDown('S') && KEYMANAGER->isStayKeyDown('D') && KEYMANAGER->isStayKeyDown('F'))init();
		RECT temp;
		for (int i = 0; i < vmob.size(); i++)
		{
			if (IntersectRect(&temp, &player.r, &vmob[i].r))
			{
				if (scoreview < score)scoresave();
				init();
			}
		}
		if ((int)player.x%TILESIZE == 0 && (int)player.y%TILESIZE == 0)
		{
			if (KEYMANAGER->isStayKeyDown(VK_LEFT))
			{
				player.direction = LEFT;
			}
			if (KEYMANAGER->isStayKeyDown(VK_RIGHT))
			{
				player.direction = RIGHT;
			}
			if (KEYMANAGER->isStayKeyDown(VK_UP))
			{
				player.direction = UP;
			}
			if (KEYMANAGER->isStayKeyDown(VK_DOWN))
			{
				player.direction = DOWN;
			}
		}
		movetile(player);
		
		for (int i = 0; i < vmob.size(); i++)
		{
			if (i == 0)mobmove1(vmob[i]);
			else if (i == 1)mobmove2(vmob[i]);
			else if (i == 2)mobmove3(vmob[i]);
			else mobmove2(vmob[i]);
			
			movetile(vmob[i]);
		}
		player.r = RectMake(player.x, player.y, imgpacman->getFrameWidth(), imgpacman->getFrameHeight());
		for (int i = 0; i < vmob.size(); i++)
		{
			vmob[i].r = RectMake(vmob[i].x, vmob[i].y, imgmob->getFrameWidth(), imgmob->getFrameHeight());
		}
		if (maze[player.ontiley][player.ontilex].att == 1)
		{
			maze[player.ontiley][player.ontilex].att = 0;
			score+=10;
		}
	}
	if (title == 2)//맵툴
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
						if (ss == 3 && !isplayerexist)
						{
							isplayerexist = true;
							maze[i][k].att = ss;
						}
						else if (ss == 3 && isplayerexist) {}
						else if (maze[i][k].att == 3)
						{
							isplayerexist = false;
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

			if (RectPointCollision(savebutton, _ptMouse))mazesave();
			if (RectPointCollision(loadbutton, _ptMouse))mazeload();
		}
	}
	if (title == 3)//옵션
	{
		if (KEYMANAGER->isOnceKeyDown('X'))title=0;
		if (KEYMANAGER->isOnceKeyDown(VK_UP)&&player.speed < 5)
		{
			player.speed += 1;
		}
		if (KEYMANAGER->isOnceKeyDown(VK_DOWN)&&player.speed > 1)
		{
			player.speed -= 1;
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
		for (int i = 0; i < vmob.size(); i++)
		{
			imgmob->frameRender(getMemDC(), vmob[i].r.left, vmob[i].r.top, 0, 0);
		}
		imgpacman->frameRender(getMemDC(), player.r.left, player.r.top, 4, 0);
		for (int i = 0; i < 6; i++)
		{
			for (int k = 0; k < 10; k++)
			{
				if ((score / (int)pow(10, i))%10 == k)imgnumber->frameRender(getMemDC(), WINSIZEX / 2 - TILESIZE * (i - 3), 0, k, 0);
			}
		}
		if (KEYMANAGER->isTogglekey('A'))
		{
			for (int i = 0; i < vOpenList.size(); i++)
			{
				Rectangle(getMemDC(), vOpenList[i].idx * 16, vOpenList[i].idy * 16, vOpenList[i].idx * 16 + 16, vOpenList[i].idy * 16 + 16);
				//SetTextColor(getMemDC(), RGB(255, 255, 255));
				/*char ss[128];
				if (i > 0)sprintf_s(ss, "%d %d", vOpenList[1].idx, vOpenList[1].idy);
				TextOut(getMemDC(), 500, 520, ss, strlen(ss));*/
			}
			for (int i = 0; i < vCloseList.size(); i++)
			{
				Ellipse(getMemDC(), vCloseList[i].idx * 16, vCloseList[i].idy * 16, vCloseList[i].idx * 16 + 16, vCloseList[i].idy * 16 + 16);
				if (i > 500)
				{
					Ellipse(getMemDC(), vCloseList[i].idx * 16 - 5, vCloseList[i].idy * 16 - 5, vCloseList[i].idx * 16 + 19, vCloseList[i].idy * 16 + 19);
				}
			}
			for (int i = 0; i < TILENUMY; i++)
			{
				for (int k = 0; k<TILENUMX; k++)
				{
					if (isopen[i][k] == 1) Ellipse(getMemDC(), k * 16, i * 16, k * 16 + 8, i * 16 + 8);
					if (isopen[i][k] == 2) Ellipse(getMemDC(), k * 16, i * 16, k * 16 + 4, i * 16 + 4);
				}
			}
			for (int i = 0; i < vPathList.size(); i++)
			{
				imgnumber->frameRender(getMemDC(), vPathList[i].idx * 16, vPathList[i].idy * 16, 0, 0);
				if (i > 100)
				{
					imgnumber->frameRender(getMemDC(), vPathList[i].idx * 16, vPathList[i].idy * 16, 0, 1);
				}
			}
			Ellipse(getMemDC(), currentTile.idx * 16 + 4, currentTile.idy * 16 + 4, currentTile.idx * 16 + 16, currentTile.idy * 16 + 16);
			Ellipse(getMemDC(), pathTile.idx * 16 + 8, pathTile.idy * 16 + 8, pathTile.idx * 16 + 16, pathTile.idy * 16 + 16);
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
		sprintf_s(ss, "speed : %d", (int)player.speed);
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

void mainGame::movetile(MOB &movemob)
{
	for (int i = 0; i < TILENUMY; i++)
	{
		for (int k = 0; k < TILENUMX; k++)
		{
			if (getDistance(movemob.x, movemob.y, maze[i][k].r.left, maze[i][k].r.top) < TILESIZE/2)//타일 좌표랑 거리차이가 8밑이면 몹의 타일위치를 갱신
			{
				movemob.ontilex = k;
				movemob.ontiley = i;
			}
		}
	}
	if (movemob.direction == LEFT)
	{
		movemob.x -= movemob.speed;
		if (maze[movemob.ontiley][movemob.ontilex - 1].att == 2 && movemob.ontilex*TILESIZE > movemob.x)movemob.x++;
	}
	if (movemob.direction == RIGHT)
	{
		movemob.x += movemob.speed;
		if (maze[movemob.ontiley][movemob.ontilex + 1].att == 2 && movemob.ontilex*TILESIZE < movemob.x)movemob.x--;
	}
	if (movemob.direction == UP)
	{
		movemob.y -= movemob.speed;
		if (maze[movemob.ontiley-1][movemob.ontilex].att == 2 && movemob.ontiley*TILESIZE > movemob.y)movemob.y++;
	}
	if (movemob.direction == DOWN)
	{
		movemob.y += movemob.speed;
		if (maze[movemob.ontiley+1][movemob.ontilex].att == 2 && movemob.ontiley*TILESIZE < movemob.y)movemob.y--;
	}
	//타일 좌표랑 거리차이가 자신의 이동거리 보다 밑일경우 위치를 조정
	if (movemob.direction!=NOMOVE && getDistance(movemob.x, movemob.y, maze[movemob.ontiley][movemob.ontilex].r.left, maze[movemob.ontiley][movemob.ontilex].r.top) < movemob.speed)
	{
		movemob.x = maze[movemob.ontiley][movemob.ontilex].r.left;
		movemob.y = maze[movemob.ontiley][movemob.ontilex].r.top;
		//movemob.direction = NOMOVE;
	}
}

void mainGame::mobmove1(MOB & mob)
{
	vOpenList.clear();
	vCloseList.clear();
	vPathList.clear();

	for (int i = 0; i < TILENUMY; i++)
	{
		for (int k = 0; k < TILENUMX; k++)
		{
			if (maze[i][k].att == 2)astartile[i][k].att = 1;
			else if (maze[i][k].att == 3)astartile[i][k].att = 3;
			else if (maze[i][k].att == 4)astartile[i][k].att = 4;
			else if (maze[i][k].att == 0 || maze[i][k].att == 1)astartile[i][k].att = 0;
			astartile[i][k].idx = k;
			astartile[i][k].idy = i;
			isopen[i][k] = 0;
		}
	}

	ZeroMemory(&endTile, sizeof(ASTARTILE)); //player location
	endTile.att = 3;
	endTile.idx = player.ontilex;
	endTile.idy = player.ontiley;


	//mob1 location
	startTile.att = 4;
	startTile.idx = mob.ontilex;
	startTile.idy = mob.ontiley;
	startTile.hcost = getDistance(startTile.idx, startTile.idy, endTile.idx, endTile.idy)*TILESIZE;
	startTile.gcost = 0;
	startTile.fcost = startTile.gcost + startTile.hcost;
	startTile.parentidx = INFINITE;
	startTile.parentidy = INFINITE;
	isopen[startTile.idy][startTile.idx] = 1;
	isopen[endTile.idy][endTile.idx] = 0;


	vOpenList.push_back(startTile);
	while (1)
	{
		int temptotalcost = 9999999;
		for (viOpenList = vOpenList.begin(); viOpenList != vOpenList.end(); ++viOpenList)
		{
			if (viOpenList->fcost < temptotalcost)
			{
				temptotalcost = viOpenList->fcost;
				currentTile = *viOpenList;
			}
		}

		//for (viOpenList = vOpenList.begin(); viOpenList != vOpenList.end(); ++viOpenList)//주변 4타일을 오픈리스트 추가
		for (int i = 0; i < vOpenList.size(); i++)//주변 4타일을 오픈리스트 추가
		{
			ASTARTILE node;
			for (int a = -1; a < 2; a++)
			{
				for (int b = -1; b < 2; b++)
				{
					if ((a == -1 && b == -1) || (a == -1 && b == 1) || (a == 0 && b == 0) || (a == 1 && b == -1) || (a == 1 && b == 1))continue;
					if (isopen[currentTile.idy + a][currentTile.idx + b] != 0)continue;

					isopen[currentTile.idy + a][currentTile.idx + b] = 1;
					if (astartile[currentTile.idy + a][currentTile.idx + b].att == 0 || astartile[currentTile.idy + a][currentTile.idx + b].att == 3)
					{
						node.att = 0;
						node.idx = currentTile.idx + b;
						node.idy = currentTile.idy + a;
						node.parentidx = currentTile.idx;
						node.parentidy = currentTile.idy;
						node.gcost = currentTile.gcost + 10;
						node.hcost = getDistance(node.idx, node.idy, endTile.idx, endTile.idy)*TILESIZE;
						node.fcost = node.hcost + node.gcost;
						vOpenList.push_back(node);
					}
				}
			}
			for (int j = 0; j < vOpenList.size(); j++)
			{
				if (isopen[vOpenList[j].idy - 1][vOpenList[j].idx] != 0 &&
					isopen[vOpenList[j].idy + 1][vOpenList[j].idx] != 0 &&
					isopen[vOpenList[j].idy][vOpenList[j].idx - 1] != 0 &&
					isopen[vOpenList[j].idy][vOpenList[j].idx + 1] != 0)
				{
					if (isopen[vOpenList[j].idy][vOpenList[j].idx] == 1)
					{
						vCloseList.push_back(vOpenList[j]);
						isopen[vOpenList[j].idy][vOpenList[j].idx] = 2;
						vOpenList.erase(vOpenList.begin() + j);
					}
					else continue;
				}
			}
		}
		isend = false;
		for (int k = 0; k < vOpenList.size(); ++k)
		{
			if (vOpenList[k].idx == player.ontilex && vOpenList[k].idy == player.ontiley)//Openlist에 도착지점이 포함되면
			{
				vCloseList.push_back(vOpenList[k]);
				isend = true;
				pathTile = vOpenList[k];
				vPathList.push_back(pathTile);
				break;
			}
		}
		for (int q = 0; q < vCloseList.size(); q++)
		{
			for (int k = 0; k < vCloseList.size(); k++)
			{
				if (vCloseList[k].idx == pathTile.parentidx && vCloseList[k].idy == pathTile.parentidy)//부모 타일 검색
				{
					vPathList.push_back(pathTile);
					pathTile = vCloseList[k];//부모타일로 변경
					vCloseList.erase(vCloseList.begin() + k);
				}
				if (pathTile.idx == mob.ontilex && pathTile.idy == mob.ontiley)//부모타일이 시작지점일 경우
				{
					break;
				}
			}
		}

		for (int q = 0; q < vPathList.size(); q++)
		{
			if ((int)mob.x%TILESIZE == 0 && (int)mob.y%TILESIZE == 0)
			{
				if (vPathList[q].idx == mob.ontilex - 1 && vPathList[q].idy == mob.ontiley)mob.direction = LEFT;
				if (vPathList[q].idx == mob.ontilex + 1 && vPathList[q].idy == mob.ontiley)mob.direction = RIGHT;
				if (vPathList[q].idx == mob.ontilex&&vPathList[q].idy == mob.ontiley - 1)mob.direction = UP;
				if (vPathList[q].idx == mob.ontilex&&vPathList[q].idy == mob.ontiley + 1)mob.direction = DOWN;
			}
		}
		if (isend)
		{

			break;
		}

		//if (currentTile.idx == player.ontilex&&currentTile.idy == player.ontiley)break;

	}//e.o.w
}

void mainGame::mobmove2(MOB & mob)
{
	if ((int)mob.x%TILESIZE == 0 && (int)mob.y%TILESIZE == 0)
	{
		if (getDistance(player.x, player.y, mob.x, mob.y) < 250)
		{
			if (abs((player.y - mob.y) / (player.x - mob.x)) < 1 && player.x < mob.x)mob.direction = LEFT;
			if (abs((player.y - mob.y) / (player.x - mob.x)) < 1 && player.x > mob.x)mob.direction = RIGHT;
			if (abs((player.y - mob.y) / (player.x - mob.x)) > 1 && player.y < mob.y)mob.direction = UP;
			if (abs((player.y - mob.y) / (player.x - mob.x)) > 1 && player.y > mob.y)mob.direction = DOWN;
		}
		else
		{
			if (mob.direction == LEFT)
			{
				int random = RND->getFromIntTo(0, 9);
				if (random >= 0 && random <= 6)mob.direction = LEFT;
				if (random == 7)mob.direction = UP;
				if (random == 8)mob.direction = RIGHT;
				if (random == 9)mob.direction = DOWN;
			}
			if (mob.direction == UP)
			{
				int random = RND->getFromIntTo(0, 9);
				if (random >= 0 && random <= 6)mob.direction = UP;
				if (random == 7)mob.direction = LEFT;
				if (random == 8)mob.direction = RIGHT;
				if (random == 9)mob.direction = DOWN;
			}
			if (mob.direction == RIGHT)
			{
				int random = RND->getFromIntTo(0, 9);
				if (random >= 0 && random <= 6)mob.direction = RIGHT;
				if (random == 7)mob.direction = UP;
				if (random == 8)mob.direction = LEFT;
				if (random == 9)mob.direction = DOWN;
			}
			if (mob.direction == DOWN)
			{
				int random = RND->getFromIntTo(0, 9);
				if (random >= 0 && random <= 6)mob.direction = DOWN;
				if (random == 7)mob.direction = UP;
				if (random == 8)mob.direction = RIGHT;
				if (random == 9)mob.direction = LEFT;
			}
			if (mob.direction == NOMOVE)
			{
				mob.direction = RND->getFromIntTo(1, 4);
			}
		}
	}
}

void mainGame::mobmove3(MOB & mob)
{
	if (count % 5 == 0)
	{
		mob.direction = RND->getFromIntTo(1, 4);
	}
	if ((int)mob.x%TILESIZE == 0 && (int)mob.y%TILESIZE == 0)
	{
		count++;
	}
}
