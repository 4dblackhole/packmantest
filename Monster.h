#pragma once
#include <queue>

class ICharacter
{
public:
	ICharacter(int tx, int ty, float sp);

	void MoveTile(const TILE (*maze)[TILENUMY][TILENUMX]);
	void UpdateRect(int x, int y, int w, int h);

	bool exist = true;//사망여부
	float x, y;//왼쪽위좌표
	int ontilex, ontiley;//타일위치
	float speed;//속도
	int direction = NOMOVE;//방향
	RECT rc;//렉트충돌용 렉트
private:
	//direction 값을 정하는 함수
	virtual void MoveStrategy() = 0;
};

class Player : public ICharacter
{
public:
	Player(int tx, int ty, float sp) :ICharacter(tx, ty, sp) {}

private:
	virtual void MoveStrategy() final;
};

//A* Algorithm
class Monster1 : public ICharacter
{
public:
	Monster1(int tx, int ty, float sp) :ICharacter(tx, ty, sp) {}

	void DebugRender(HDC hdc, image*& imgnumber);

	deque<ASTARTILE*> vOpenList;
	deque<ASTARTILE*> vCloseList;
	vector<ASTARTILE*> vPathList;

	const ICharacter* target = nullptr;
	const TILE(*refMaze)[TILENUMY][TILENUMX] = nullptr;

	ASTARTILE astartile[TILENUMY][TILENUMX];
	int isopen[TILENUMY][TILENUMX];

	enum ASTARTILESTATUS
	{
		NO,
		OPEN,
		CLOSE
	};

private:
	ASTARTILE* openListCurrentTile;

	POINT startTileIdx{};
	POINT endTileIdx{};

	virtual void MoveStrategy() final;

	void AstarTileInit();
	void InputTileToOpenList(int tx, int ty, bool& flag);

};

//Follows Direction
class Monster2 : public ICharacter
{
public:
	Monster2(int tx, int ty, float sp) :ICharacter(tx, ty, sp) {}
	const ICharacter* target = nullptr;
private:
	virtual void MoveStrategy() final;
};

//Random Move
class Monster3 : public ICharacter
{
public:
	Monster3(int tx, int ty, float sp) :ICharacter(tx, ty, sp),count(0) {}
private:
	virtual void MoveStrategy() final;

	int count;
};
