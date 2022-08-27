#include "stdafx.h"
#include "enemy.h"


enemy::enemy() { }


enemy::~enemy() {

}

HRESULT enemy::init() {
	return S_OK;
}

HRESULT enemy::init(const char* imageName, POINT position) {
	//초기화

	_currentFrameX = _currentFrameY = 0;
	_count = _fireCount = 0;

	_rndFireCount = RND->getFromIntTo(1, 1000);

	_imageName = IMAGEMANAGER->findImage(imageName);

	_rc = RectMakeCenter(position.x, position.y, _imageName->getFrameWidth(), _imageName->getFrameHeight());


	return S_OK;
}

void enemy::release() {

}

void enemy::update() {
	_count++;

	if (_count % 2 == 0) {
		if (_currentFrameX >= _imageName->getMaxFrameX()) _currentFrameX = 0;

		_imageName->setFrameX(_currentFrameX);
		_currentFrameX++;

		_count = 0;
	}
}

void enemy::render() {
	draw();


}

//움직임 함수
void enemy::move() {

}

//그려주는 함수
void enemy::draw() {
	_imageName->frameRender(getMemDC(), _rc.left, _rc.top, _currentFrameX, _currentFrameY);
}

//총알 발사 카운트 함수
bool enemy::bulletCountFire() {
	_fireCount++;

	if (_fireCount % _rndFireCount == 0) {
		_fireCount = 0;
		_rndFireCount = RND->getFromIntTo(1, 1000);

		return true;
	}

	return false;
}
