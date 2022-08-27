#pragma once
#include "gameNode.h"
/*
��������
���������� �������� ���� ��
�Ÿ��� �ҿ�ð����� ������ �ӵ���
�˷��� �� ��
(���������� �������� �� �� ������
�̵� �������� ����.)
*/
class action : public gameNode
{
private:
	image * _image;

	float _startX, _startY;//��������
	float _endX, _endY;	   //���� ����

	float _angle;
	float _travelRange;    //����Ÿ�(������ �Ÿ�)
	float _worldTimeCount; //����ǰ� ���� ���� �ð�
	float _time;           //�ð�

	bool _isMoving;        //�������°�
public:
	action();
	~action();

	HRESULT init();
	void release();
	void update();
	void render();

	//                             ������ �Ÿ�x y       ���ʸ��� �����ؾߵǴ°�
	void moveTo(image* image, float endX, float endY, float time);

	void moving(void);

	bool getIsMoving() { return _isMoving; }
};

