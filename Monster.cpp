#include "stdafx.h"
#include "Monster.h"

ICharacter::ICharacter(int tx, int ty, float sp)
{
	this->x = tx * TILESIZE;
	this->y = ty * TILESIZE;
	this->ontilex = tx;
	this->ontiley = ty;
	this->speed = sp;
	this->direction = NOMOVE;
	this->rc = RectMake(tx * TILESIZE, ty * TILESIZE, TILESIZE, TILESIZE);
}

void ICharacter::MoveTile(const TILE (*m)[TILENUMY][TILENUMX])
{
	MoveStrategy();
	const TILE (&maze)[TILENUMY][TILENUMX] = *m;
	for (int i = 0; i < TILENUMY; i++)
	{
		for (int k = 0; k < TILENUMX; k++)
		{
			if (getDistance(this->x, this->y, maze[i][k].r.left, maze[i][k].r.top) < TILESIZE / 2)//타일 좌표랑 거리차이가 8밑이면 몹의 타일위치를 갱신
			{
				this->ontilex = k;
				this->ontiley = i;
			}
		}
	}
	if (direction == LEFT)
	{
		this->x -= this->speed;
		if (maze[this->ontiley][this->ontilex - 1].att == 2 && this->ontilex * TILESIZE > this->x)this->x++;
	}
	if (this->direction == RIGHT)
	{
		this->x += this->speed;
		if (maze[this->ontiley][this->ontilex + 1].att == 2 && this->ontilex * TILESIZE < this->x)this->x--;
	}
	if (this->direction == UP)
	{
		this->y -= this->speed;
		if (maze[this->ontiley - 1][this->ontilex].att == 2 && this->ontiley * TILESIZE > this->y)this->y++;
	}
	if (this->direction == DOWN)
	{
		this->y += this->speed;
		if (maze[this->ontiley + 1][this->ontilex].att == 2 && this->ontiley * TILESIZE < this->y)this->y--;
	}
	//타일 좌표랑 거리차이가 자신의 이동거리 보다 밑일경우 위치를 조정
	if (this->direction != NOMOVE && getDistance(this->x, this->y, maze[this->ontiley][this->ontilex].r.left, maze[this->ontiley][this->ontilex].r.top) < this->speed)
	{
		this->x = maze[this->ontiley][this->ontilex].r.left;
		this->y = maze[this->ontiley][this->ontilex].r.top;
		//this->direction = NOMOVE;
	}
}

void ICharacter::UpdateRect(int x, int y, int w, int h)
{
	rc = RectMake(x, y, w, h);
}

void Player::MoveStrategy()
{
	if ((int)x % TILESIZE == 0 && (int)y % TILESIZE == 0)
	{
		if (KEYMANAGER->isStayKeyDown(VK_LEFT))this->direction = LEFT;
		else if (KEYMANAGER->isStayKeyDown(VK_RIGHT)) this->direction = RIGHT;
		else if (KEYMANAGER->isStayKeyDown(VK_UP))this->direction = UP;
		else if (KEYMANAGER->isStayKeyDown(VK_DOWN)) this->direction = DOWN;
	}
}

void Monster1::DebugRender(HDC hdc, image*& imgnumber)
{
	for (int i = 0; i < vOpenList.size(); i++)
	{
		Rectangle(hdc, vOpenList[i]->idx * 16, vOpenList[i]->idy * 16, vOpenList[i]->idx * 16 + 16, vOpenList[i]->idy * 16 + 16);

	}
	int closeListIdx = 0;
	for (auto& it : vCloseList)
	{
		Ellipse(hdc, it->idx * 16, it->idy * 16, it->idx * 16 + 16, it->idy * 16 + 16);
		if (closeListIdx++ > 500)
		{
			Ellipse(hdc, it->idx * 16 - 5, it->idy * 16 - 5, it->idx * 16 + 19, it->idy * 16 + 19);
		}
	}
	for (int i = 0; i < TILENUMY; i++)
	{
		for (int k = 0; k < TILENUMX; k++)
		{
			if (isopen[i][k] == ASTARTILESTATUS::OPEN) Ellipse(hdc, k * 16, i * 16, k * 16 + 8, i * 16 + 8);
			if (isopen[i][k] == ASTARTILESTATUS::CLOSE) Ellipse(hdc, k * 16, i * 16, k * 16 + 4, i * 16 + 4);
		}
	}
	for (int i = 0; i < vPathList.size(); i++)
	{
		imgnumber->frameRender(hdc, vPathList[i]->idx * 16, vPathList[i]->idy * 16, 0, 0);
		if (i > 100)
		{
			imgnumber->frameRender(hdc, vPathList[i]->idx * 16, vPathList[i]->idy * 16, 0, 1);
		}
	}
	Ellipse(hdc, openListCurrentTile->idx* 16 + 4, openListCurrentTile->idy * 16 + 4, openListCurrentTile->idx * 16 + 16, openListCurrentTile->idy * 16 + 16);
	//Ellipse(hdc, pathTile.idx * 16 + 8, pathTile.idy * 16 + 8, pathTile.idx * 16 + 16, pathTile.idy * 16 + 16);
}

void Monster1::AstarTileInit()
{
	if (target == nullptr) return;

	const TILE(&maze)[TILENUMY][TILENUMX] = *refMaze;

	for (int tileY = 0; tileY < TILENUMY; tileY++)
	{
		for (int tileX = 0; tileX < TILENUMX; tileX++)
		{
			if (maze[tileY][tileX].att == TILE::TileAttribute::WALL)
			{
				astartile[tileY][tileX].att = ASTARTILE::AstarAttribute::WALL;
			}
			else
			{
				astartile[tileY][tileX].att = ASTARTILE::AstarAttribute::EMPTY;
			}
			astartile[tileY][tileX].idx = tileX;
			astartile[tileY][tileX].idy = tileY;
			isopen[tileY][tileX] = ASTARTILESTATUS::NO;
		}
	}


}

void Monster1::InputTileToOpenList(int tx, int ty, bool& flag)
{
	ASTARTILE* const& currentAstar = openListCurrentTile;
	int& openStatus = isopen[currentAstar->idy + ty][currentAstar->idx + tx];
	if (openStatus != ASTARTILESTATUS::NO) return;
	openStatus = ASTARTILESTATUS::OPEN;

	ASTARTILE& targetAstarTile = astartile[currentAstar->idy + ty][currentAstar->idx + tx];
	const int& astarTileAtt = targetAstarTile.att;
	if (astarTileAtt == ASTARTILE::AstarAttribute::EMPTY)
	{
		targetAstarTile.parentidx = currentAstar->idx;
		targetAstarTile.parentidy = currentAstar->idy;
		targetAstarTile.gcost = currentAstar->gcost + 10;
		targetAstarTile.hcost = getDistance(targetAstarTile.idx, targetAstarTile.idy, endTileIdx.x, endTileIdx.y) * TILESIZE;
		targetAstarTile.fcost = targetAstarTile.hcost + targetAstarTile.gcost;
		vOpenList.push_back(&targetAstarTile);
	}
	else if (astarTileAtt == ASTARTILE::AstarAttribute::END)
	{
		targetAstarTile.parentidx = currentAstar->idx;
		targetAstarTile.parentidy = currentAstar->idy;
		targetAstarTile.gcost = currentAstar->gcost + 10;
		targetAstarTile.hcost = getDistance(targetAstarTile.idx, targetAstarTile.idy, endTileIdx.x, endTileIdx.y) * TILESIZE;
		targetAstarTile.fcost = targetAstarTile.hcost + targetAstarTile.gcost;
		vCloseList.push_back(&targetAstarTile);
		flag = false;
	}
}

void Monster1::MoveStrategy()
{
	if (target == nullptr) return;
	if ((int)x % TILESIZE != 0 || (int)y % TILESIZE != 0) return;

	vOpenList.clear();
	vCloseList.clear();
	vPathList.clear();

	AstarTileInit();

	astartile[target->ontiley][target->ontilex].att = ASTARTILE::AstarAttribute::END;
	endTileIdx = { target->ontilex, target->ontiley };

	startTileIdx = { ontilex, ontiley };
	ASTARTILE& startAstarTile = astartile[ontiley][ontilex];
	if (startAstarTile.att != ASTARTILE::AstarAttribute::EMPTY) return;

	startAstarTile.att = ASTARTILE::AstarAttribute::START;
	startAstarTile.hcost = getDistance(startAstarTile.idx, startAstarTile.idy, endTileIdx.x, endTileIdx.y) * TILESIZE;
	startAstarTile.gcost = 0;
	startAstarTile.fcost = startAstarTile.gcost + startAstarTile.hcost;
	startAstarTile.parentidx = INFINITE;
	startAstarTile.parentidy = INFINITE;
	isopen[startAstarTile.idy][startAstarTile.idx] = 1;

	vOpenList.push_back(&startAstarTile);

	//find == false
	bool findFlag = true;
	while (!vOpenList.empty())
	{
		//select nearest tile
		int temptotalcost = 9999999;
		for (deque<ASTARTILE*>::const_iterator it = vOpenList.cbegin(); it != vOpenList.cend(); ++it)
		{
			if ((*it)->fcost < temptotalcost)
			{
				temptotalcost = (*it)->fcost;
				openListCurrentTile = *it;
			}
		}

		//주변 4타일을 오픈리스트 추가
		vCloseList.push_back(openListCurrentTile);

		if (findFlag) InputTileToOpenList(-1, 0, findFlag); //left
		if (findFlag) InputTileToOpenList(0, 1, findFlag); //up
		if (findFlag) InputTileToOpenList(1, 0, findFlag); //right
		if (findFlag) InputTileToOpenList(0, -1, findFlag); //down

		isopen[openListCurrentTile->idy][openListCurrentTile->idx] = ASTARTILESTATUS::CLOSE;
		vOpenList.erase(std::find(vOpenList.begin(), vOpenList.end(), openListCurrentTile));

		if (!findFlag) break;
	}

	ASTARTILE* pathTile = vCloseList.back();
	while (pathTile->att != ASTARTILE::AstarAttribute::START)
	{
		vPathList.push_back(pathTile);
		pathTile = &astartile[pathTile->parentidy][pathTile->parentidx];
	}

	if(!vPathList.empty())
	{
		const ASTARTILE& targetTile = *vPathList.back();
		if (targetTile.idx == ontilex - 1 && targetTile.idy == ontiley) direction = LEFT;
		if (targetTile.idx == ontilex + 1 && targetTile.idy == ontiley) direction = RIGHT;
		if (targetTile.idx == ontilex	  && targetTile.idy == ontiley - 1) direction = UP;
		if (targetTile.idx == ontilex	  && targetTile.idy == ontiley + 1) direction = DOWN;
	}

}

void Monster2::MoveStrategy()
{
	if (target == nullptr)
	{
		this->direction = NOMOVE;
		return;
	}

	if ((int)x % TILESIZE != 0 || (int)y % TILESIZE != 0) return;
	const ICharacter& player = *target;
	if (getDistance(player.x, player.y, x, y) < 250) //target is within the distance
	{
		const float targetTangent = abs((player.y - this->y) / (player.x - this->x));
		if (targetTangent < 1 && player.x < this->x) this->direction = LEFT;
		else if (targetTangent < 1 && player.x > this->x) this->direction = RIGHT;
		else if (targetTangent > 1 && player.y < this->y) this->direction = UP;
		else if (targetTangent > 1 && player.y > this->y) this->direction = DOWN;

		return;
	}

	const int random = RND->getFromIntTo(0, 9);
	switch (this->direction)
	{
	case LEFT:
	{
		if (random == 7)this->direction = UP;
		if (random == 8)this->direction = RIGHT;
		if (random == 9)this->direction = DOWN;
	}
	break;
	case UP:
	{
		if (random == 7)this->direction = LEFT;
		if (random == 8)this->direction = RIGHT;
		if (random == 9)this->direction = DOWN;
	}
	break;
	case RIGHT:
	{
		if (random == 7)this->direction = UP;
		if (random == 8)this->direction = LEFT;
		if (random == 9)this->direction = DOWN;
	}
	break;
	case DOWN:
	{
		if (random == 7)this->direction = UP;
		if (random == 8)this->direction = RIGHT;
		if (random == 9)this->direction = LEFT;
	}
	break;

	default:
		this->direction = RND->getFromIntTo(LEFT, DOWN);
		break;
	}

}

void Monster3::MoveStrategy()
{
	if (count % 5 == 0) direction = RND->getFromIntTo(LEFT, DOWN);
	if ((int)x % TILESIZE == 0 && (int)y % TILESIZE == 0) count++;
	
}
