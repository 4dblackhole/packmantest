#pragma once
#include "gameNode.h"
#define TILESIZE 16
#define TILENUMX WINSIZEX/TILESIZE
#define TILENUMY WINSIZEY/TILESIZE
struct TILE
{
	int att;//�Ӽ� 0����� 1������ 2�� 3�÷��̾� 4��
	RECT r;
};
struct ASTARTILE
{
	int att;//0����� 1�� 4���� 3��
	int idx, idy;//��ġ
	int parentidx, parentidy;//�θ���ġ

	float fcost, gcost, hcost;//�� ���
};
struct MOB
{
	bool exist;//�������
	float x, y;//��������ǥ
	int ontilex, ontiley;//Ÿ����ġ
	float speed;//�ӵ�
	float direction;//����
	RECT r;//��Ʈ�浹�� ��Ʈ
};
enum DIRECTION
{
	NOMOVE,LEFT,UP,RIGHT,DOWN
};
class mainGame : public gameNode
{
private:
	int title;//0 Ÿ��Ʋ 1 ���� 2 �� ���� 3 �ɼ�
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
	void mobmove2(MOB &mob);//���⸸ ����
	void mobmove3(MOB &mob);//�����̵�

	void pathfinder();
};

