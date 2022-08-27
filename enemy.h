#pragma once
#include "gameNode.h"

class enemy : public gameNode {
protected:
	image * _imageName;
	RECT _rc;

	//프레임 번호를 따로 두는 이유는
	int _currentFrameX;
	int _currentFrameY;

	int _count;				//카운트
	int _fireCount;			//발사 카운트
	int _rndFireCount;		//랜덤하게 발사하기 위한 장치
public:
	enemy();
	~enemy();

	HRESULT init();
	HRESULT init(const char* imageName, POINT position);
	void release();
	void update();
	void render();

	void move();		//움직임 함수
	void draw();		//그려주는 함수

	bool bulletCountFire();	//총알 발사 카운트 함수

	inline RECT getRect() { return _rc; }


};

