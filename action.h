#pragma once
#include "gameNode.h"
/*
선형보간
시작점에서 끝점으로 보낼 때
거리와 소요시간으로 보내는 속도를
알려고 할 때
(시작점에서 끝점으로 갈 때 최적의
이동 방향으로 간다.)
*/
class action : public gameNode
{
private:
	image * _image;

	float _startX, _startY;//시작지점
	float _endX, _endY;	   //도착 지점

	float _angle;
	float _travelRange;    //여행거리(도달할 거리)
	float _worldTimeCount; //실행되고 나서 월드 시간
	float _time;           //시간

	bool _isMoving;        //움지였는가
public:
	action();
	~action();

	HRESULT init();
	void release();
	void update();
	void render();

	//                             도달할 거리x y       몇초만에 도달해야되는가
	void moveTo(image* image, float endX, float endY, float time);

	void moving(void);

	bool getIsMoving() { return _isMoving; }
};

