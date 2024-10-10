#pragma once
#include "gameNode.h"
#include "Monster.h"

class mainGame : public gameNode
{
private:
	int title;//0 ≈∏¿Ã∆≤ 1 ∞‘¿” 2 ∏  ø°µ˜ 3 ø…º«
	int selectkey;

	image *imgmaze;
	image *imgmob;
	image *imgnumber;
	image *imgpacman;
	image *imgmaptool;

	TILE maze[TILENUMY][TILENUMX];

	unique_ptr<Player> player;

	int mobamount;
	vector<ICharacter*> vmob;
	
	RECT savebutton;
	RECT loadbutton;
	RECT wallbutton;
	RECT nullbutton;
	RECT mobbutton;
	RECT pacmanbutton;
	int selecttile;
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

};

