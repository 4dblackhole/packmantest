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

	//������ �� ������ ���Ѵ�.
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
	//ó���� ����Ʈ �ִϸ��̼����� ������ �صд�.
	setDefPlayFrame();

	return S_OK;
}

void animation::release(void){}

void animation::setDefPlayFrame(BOOL reverse, BOOL loop)
{
	//���� ���� ����
	_loop = loop;
	//�÷��� ����Ʈ ���� �ʱ�ȭ
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

//�迭 �ִϸ��̼� ���
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
//���� �ִϸ��̼� ���
void animation::setPlayFrame(int start, int end, BOOL reverse, BOOL loop)
{
	_loop = loop;
	_playList.clear();

	//�ε��� ������ 1���ϋ�
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
//�ִϸ��̼� ������ ����
void animation::setFPS(int framePerSec)
{
	_frameUpdateSec = 1.0f / framePerSec;
}

//������ ���� Ƚ��
void animation::frameUpdate(float elapsedTime)
{
	if (_play)
	{
		_elapsedSec += elapsedTime;

		//������ ���� �ѱ� �ӵ����� �Ѿ��
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
