#include "stdafx.h"
#include "animation.h"

animation::animation()
	:_frameNum(0),
	_frameWidth(0),
	_frameHeight(0),
	_loop(FALSE),
	_frameUpdateSec(0),
	_elapsedSec(0),
	_nowPlayIndex(0),
	_play(FALSE)
{
}

HRESULT animation::init(int totalW, int totalH, int frameW, int frameH)
{
	_frameWidth = frameW;
	int _frameNumWidth = totalW / _frameWidth;

	_frameHeight = frameH;
	int _frameNumHeight = totalH / _frameHeight;

	//프레임 총 갯수를 구한다.
	_frameNum = _frameNumWidth * _frameNumHeight;

	for (int i = 0; i < _frameNumHeight; ++i)
	{
		for (int j = 0; j < _frameNumWidth; ++j)
		{
			POINT framePos;
			framePos.x = j * _frameWidth;
			framePos.y = i * _frameHeight;

			_frameList.push_back(framePos);
		}
	}
	//처음엔 디폴트 애니메이션으로 셋팅을 해둔다.
	setDefPlayFrame();

	return S_OK;
}

void animation::release(void){}

void animation::setDefPlayFrame(BOOL reverse, BOOL loop)
{
	//루프 여부 대입
	_loop = loop;
	//플레이 리스트 벡터 초기화
	_playList.clear();

	if (reverse)
	{
		if (_loop)
		{
			for (int i = 0; i < _frameNum; ++i)
			{
				_playList.push_back(i);
			}
			for (int i = _frameNum - 1; i >= 0; --i)
			{
				_playList.push_back(i);
			}
		}
		else
		{
			for (int i = 0; i < _frameNum; ++i)
			{
				_playList.push_back(i);
			}
			for (int i = _frameNum - 1; i >= 0; --i)
			{
				_playList.push_back(i);
			}
		}
	}
	else
	{
		if (_loop)
		{
			for (int i = 0; i < _frameNum; ++i)
			{
				_playList.push_back(i);
			}
		}
		else
		{
			for (int i = 0; i < _frameNum; ++i)
			{
				_playList.push_back(i);
			}
		}
	}
}

//배열 애니메이션 재생
void animation::setPlayFrame(int * playArr, int arrLen, BOOL loop)
{
	_loop = loop;
	_playList.clear();
	
	//_frameNum[5] = { 7, 1, 3, 5, 0 }
	for (int i = 0; i < arrLen; ++i)
	{
		_playList.push_back(playArr[i]);
	}
}
//구간 애니메이션 재생
void animation::setPlayFrame(int start, int end, BOOL reverse, BOOL loop)
{
	_loop = loop;
	_playList.clear();

	//인덱스 갯수가 1장일떄
	if (start == end)
	{
		_playList.clear();
		stop();
		return;
	}

	if (start > end)
	{
		if (reverse)
		{
			if (_loop)
			{
				for (int i = start; i >= end; --i)
				{
					_playList.push_back(i);
				}
				for (int i = end + 1; i <= start; ++i)
				{
					_playList.push_back(i);
				}
			}
			else
			{
				for (int i = start; i >= end; --i)
				{
					_playList.push_back(i);
				}
				for (int i = end + 1; i <= start; ++i)
				{
					_playList.push_back(i);
				}
			}
		}
		else
		{
			if (_loop)
			{
				for (int i = start; i >= end; --i)
				{
					_playList.push_back(i);
				}
			}
			else
			{
				for (int i = start; i >= end; --i)
				{
					_playList.push_back(i);
				}
			}
		}
	}
	else//(start < end)
	{
		if (reverse)
		{
			if (_loop)
			{
				for (int i = start; i <= end; ++i)
				{
					_playList.push_back(i);
				}
				for (int i = end - 1; i >= start; --i)
				{
					_playList.push_back(i);
				}
			}
			else
			{
				for (int i = start; i <= end; ++i)
				{
					_playList.push_back(i);
				}
				for (int i = end - 1; i >= start; --i)
				{
					_playList.push_back(i);
				}
			}
		}
		else
		{
			if (_loop)
			{
				for (int i = start; i <= end; ++i)
				{
					_playList.push_back(i);
				}
			}
			else
			{
				for (int i = start; i <= end; ++i)
				{
					_playList.push_back(i);
				}
			}
		}
	}
}
//애니메이션 프레임 셋팅
void animation::setFPS(int framePerSec)
{
	_frameUpdateSec = 1.0f / framePerSec;
}

//프레임 감을 횟수
void animation::frameUpdate(float elapsedTime)
{
	if (_play)
	{
		_elapsedSec += elapsedTime;

		//프레임 한장 넘길 속도보다 넘어가면
		//if(count % 10 == 0)
		if (_elapsedSec >= _frameUpdateSec)
		{
			_elapsedSec -= _frameUpdateSec;
			_nowPlayIndex++;

			if (_nowPlayIndex == _playList.size())
			{
				if (_loop) _nowPlayIndex = 0;
				else
				{
					_nowPlayIndex--;
					_play = FALSE;
				}
			}
		}
	}
}

void animation::start(void)
{
	_play = true;
	_nowPlayIndex = 0;
}

void animation::stop(void)
{
	_play = FALSE;
	_nowPlayIndex = 0;
}

void animation::pause(void)
{
	_play = FALSE;
}

void animation::resume(void)
{
	_play = TRUE;
}

