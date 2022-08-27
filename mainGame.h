#pragma once
#include "gameNode.h"
#define TILESIZE 16
#define TILENUMX WINSIZEX/TILESIZE
#define TILENUMY WINSIZEY/TILESIZE
struct TILE
{
	int att;//속성 0빈공간 1아이템 2벽 3플레이어 4몹
	RECT r;
};
struct ASTARTILE
{
	int att;//0빈공간 1벽 4시작 3끝
	int idx, idy;//위치
	int parentidx, parentidy;//부모위치

	float fcost, gcost, hcost;//각 비용
};
struct MOB
{
	bool exist;//사망여부
	float x, y;//왼쪽위좌표
	int ontilex, ontiley;//타일위치
	float speed;//속도
	float direction;//방향
	RECT r;//렉트충돌용 렉트
};
enum DIRECTION
{
	NOMOVE,LEFT,UP,RIGHT,DOWN
};
class mainGame : public gameNode
{
private:
	int title;//0 타이틀 1 게임 2 맵 에딧 3 옵션
	int selectkey;

	image *imgmaze;
	image *imgmob;
	image *imgnumber;
	image *imgpacman;
	image *imgmaptool;

	TILE maze[TILENUMY][TILENUMX];
	ASTARTILE astartile[TILENUMY][TILENUMX];
	int isopen[TILENUMY][TILENUMX];
	MOB player;
	MOB mob;
	int mobamount;
	vector<MOB> vmob;
	vector<ASTARTILE> vOpenList;
	vector<ASTARTILE>::iterator viOpenList;
	vector<ASTARTILE> vCloseList;
	vector<ASTARTILE>::iterator viCloseList;
	vector<ASTARTILE> vPathList;
	vector<ASTARTILE>::iterator viPathList;
	
	ASTARTILE currentTile;
	ASTARTILE pathTile;

	ASTARTILE startTile;
	ASTARTILE endTile;

	RECT savebutton;
	RECT loadbutton;
	RECT wallbutton;
	RECT nullbutton;
	RECT mobbutton;
	RECT pacmanbutton;
	int selecttile;
	bool isplayerexist;
	bool isend;
	int count=0;
	int score = 0;
	int scoreview = 0;
public:
	mainGame();
	~mainGame();

	virtual HRESULT init(void);
	virtual void release(void);
	virtual void update(void);
	virtual void render();
	
	bool RectPointCollision(RECT rc,POINT pt);

	void mazesave();
	void mazeload();

	void scoresave();
	void scoreload();

	void movetile(MOB &movemob);
	void mobmove1(MOB &mob);//aStar
	void mobmove2(MOB &mob);//방향만 따라감
	void mobmove3(MOB &mob);//랜덤이동

	void pathfinder();
};

