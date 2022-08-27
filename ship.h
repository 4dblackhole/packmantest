#pragma once
#include "gameNode.h"
#include "bullets.h"

class ship : public gameNode
{
protected:
	image * _image;
	RECT _rc;
	float _x, _y;
	float _angle;
	float _speed;
	float _radius;

	bullet* _bullet;
public:
	ship();
	~ship();

	virtual HRESULT init(const char* imageName, int x, int y);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	virtual void draw(void);
	virtual void keyControl(void);
	virtual void move(void);

	virtual void bulletFire(void);
	virtual void bulletMove(void);
	virtual void bulletDraw(void);

	void setAngle(float angle) { _angle = angle; }
	float getAngle() { return _angle; }
};

