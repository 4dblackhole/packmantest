#include "stdafx.h"
#include "aStarTest.h"

HRESULT aStarTest::init()
{
	setTiles();
	
	_count = _start = 0;

	return S_OK;
}

void aStarTest::setTiles()
{
	//시작지점
	_startTile = new tile;
	_startTile->init(4, 12);
	_startTile->setAttribute("start");

	//끝 지점
	_endTile = new tile;
	_endTile->init(20, 12);
	_endTile->setAttribute("end");

	//현재 타일은 시작타일로
	_currentTile = _startTile;

	for (int i = 0; i < TILENUMY; ++i)
	{
		for (int j = 0; j < TILENUMX; ++j)
		{
			//만약에 i,j가 스타트지점x,y축하고 같을 때
			if (j == _startTile->getIdX() && i == _startTile->getIdY())
			{
				//색깔을 정하자
				_startTile->setColor(RGB(0, 255, 255));
				//벡터에 담는다
				_vTotalList.push_back(_startTile);
				continue;
			}
			if (j == _endTile->getIdX() && i == _endTile->getIdY())
			{
				//색깔을 정한다.
				_endTile->setColor(RGB(10, 120, 55));
				//벡터에 담는다
				_vTotalList.push_back(_endTile);
				continue;
			}
			//시작과 끝이 아닌 것은 타일을 할당 받아서 만든다.
			tile* node = new tile;
			node->init(j, i);
			_vTotalList.push_back(node);
		}
	}
}

vector<tile*> aStarTest::addOpenList(tile * currentTile)
{
	//타일 
	int startX = currentTile->getIdX() - 1;
	int startY = currentTile->getIdY() - 1;

	//내 현재 타일 주변 9개를 검색한다
	//주변 8개만 검색하는 것보다는 내꺼포함 9개 검출하고
	//내 타일만 걸러내는것이 더 이득
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			//토탈 타일 정보를 들고있는 벡터의 정보에서 9개의
			//타일정보를 순차적으로 노드에 담는다.
			tile* node = _vTotalList[(startY * TILENUMX)
				+ startX + j + (i * TILENUMX)];

			//예외처리(노드에 담지 말아야 할것들)(노드가 갈 수없는 상태, 시작위치, 벽)
			if (!node->getIsOpen()) continue;
			if (node->getAttribute() == "start") continue;
			if (node->getAttribute() == "wall") continue;

			//부모 타일 계속 갱신해준다
			node->setParentNode(_currentTile);

			bool addObj = true;

			for (_viOpenList = _vOpenList.begin(); _viOpenList != _vOpenList.end(); ++_viOpenList)
			{
				//이미 있는 애일 경우
				if (*_viOpenList == node)
				{
					addObj = false;
					break;
				}
			}

			//아직 타일 속겅이 끝이 아닐 경우 색을 칠하자
			if (node->getAttribute() != "end") node->setColor(RGB(128, 64, 28));
			
			//이미 체크된 애는 건너뛰자
			if (!addObj) continue;

			//갈 수 있는 타일은 오픈리스트 벡터에 담아준다.
			_vOpenList.push_back(node);
		}
	}
	//현재 타일 주변 9개를 검색해서 갈수 있는 타일은
	//다 체크해서 오픈리스트에 담고 그걸 반환
	return _vOpenList;

	//http://egloos.zum.com/cozycoz/v/9748811
}

void aStarTest::pathFinder(tile * currentTile)
{
	//비교하기 쉽게 임의의 경로비용을 설정해둔다.
	float tempTotalCost = 5000;
	tile* tempTile = NULL;

	//오픈리스트 백터 안에서, 가장 빠른 경로를 뽑아내자
	for (int i = 0; i < addOpenList(currentTile).size(); ++i)
	{
		//H값 연산
		_vOpenList[i]->setCostToGoal((abs(_endTile->getIdX() - _vOpenList[i]->getIdX()) + abs(_endTile->getIdY() - _vOpenList[i]->getIdY())) * 10);
		

		POINT center1 = _vOpenList[i]->getParentNode()->getCenter();
		POINT center2 = _vOpenList[i]->getCenter();

		//G값
		_vOpenList[i]->setCostFromStart((getDistance(center1.x, center1.y, center2.x, center2.y) > TILEWIDTH) ? 14 : 10);
		
		//F값
		_vOpenList[i]->setTotalCost(_vOpenList[i]->getCostToGoal() + _vOpenList[i]->getCostFromStart());

		//이렇게 뽑아낸 총 경로비용들을 임의의 경로비용과 연산해서
		//가장 작은 값을 계속 뽑아낸다.
		if (tempTotalCost > _vOpenList[i]->getTotalCost())
		{
			tempTotalCost = _vOpenList[i]->getTotalCost();
			tempTile = _vOpenList[i];
		}

		bool addobj = true;

		for (_viOpenList = _vOpenList.begin(); _viOpenList != _vOpenList.end(); ++_viOpenList)
		{
			if (*_viOpenList == tempTile)
			{
				addobj = false;
				break;
			}
		}

		_vOpenList[i]->setIsOpen(false);
		
		if (!addobj) continue;

		_vOpenList.push_back(tempTile);
	}
	//만약 가상의 타일이 끝에 도달할 경우
	if (tempTile->getAttribute() == "end")
	{
		//최단 경로는 색칠해줘라
		while (_currentTile->getParentNode() != NULL)
		{
			_currentTile->setColor(RGB(22, 14, 128));
			_currentTile = _currentTile->getParentNode();
		}
		return;
	}

	//최단 경로를 뽑아주자
	_vCloseList.push_back(tempTile);

	for (_viOpenList = _vOpenList.begin();
		_viOpenList != _vOpenList.end(); ++_viOpenList)
	{
		//뽑아낸 최단 경로는 오픈리스트에서 삭제시키자
		if (*_viOpenList == tempTile)
		{
			_viOpenList = _vOpenList.erase(_viOpenList);
			break;
		}
	}
	_currentTile = tempTile;

	//pathFinder(_currentTile);
	/*
	재귀함수 : 함수안에서 똑같은 함수를 호출하는 것
	재귀함수 주의점 : 한 번 호출시 스택메모리에 쌓이게 된다.
	이 정보를 리턴을 시키지 않으면 계속해서 쌓이게 되고
	쌓인 메모리가 1.2메가를 넘어서게 되면 터진다.
	터지는 현상을 스택오버플로우 현상이라고 한다.
	*/
}

void aStarTest::release()
{
}

void aStarTest::update()
{ 
	if (KEYMANAGER->isOnceKeyDown('S'))_start = true;
	//{
		//pathFinder(_currentTile);
	//}

	if (_start)
	{
		_count++;
		if (_count % 5 == 0)
		{
			pathFinder(_currentTile);
			_count = 0;
		}
	}
	if (KEYMANAGER->isStayKeyDown(VK_LBUTTON))
	{
		for (int i = 0; i < _vTotalList.size(); ++i)
		{
			if (PtInRect(&_vTotalList[i]->getRect(), _ptMouse))
			{
				if (_vTotalList[i]->getAttribute() == "start") continue;
				if (_vTotalList[i]->getAttribute() == "end") continue;

				_vTotalList[i]->setIsOpen(false);
				_vTotalList[i]->setAttribute("wall");
				_vTotalList[i]->setColor(RGB(230, 140, 200));
				break;
			}
		}
	}
}

void aStarTest::render()
{
	for (int i = 0; i < _vTotalList.size(); ++i)
	{
		_vTotalList[i]->render();
	}
}
