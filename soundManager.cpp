#include "stdafx.h"
#include "soundManager.h"

soundManager::soundManager()
	:_system(NULL),
	_channel(NULL),
	_sound(NULL)
{
}


soundManager::~soundManager()
{

}

HRESULT soundManager::init()
{
	//FMOD ���忣�� ����Ϸ��� �ʱ�ȭ
	System_Create(&_system);
	//FMOD_INIT_NORMAL = fmod �ʱ�ȭ
	//null -  �������̺� ���ڳ�?
	_system->init(TOTALSOUNDBUFFER, FMOD_INIT_NORMAL, NULL);

	_sound = new Sound*[TOTALSOUNDBUFFER];
	_channel = new Channel*[TOTALSOUNDBUFFER];

	ZeroMemory(_sound, sizeof(_sound));
	ZeroMemory(_channel, sizeof(_channel));


	return S_OK;
}

void soundManager::release()
{
	if (_channel != NULL || _sound != NULL)
	{
		for (int i = 0; i < TOTALSOUNDBUFFER; ++i)
		{
			if (_channel != NULL)
			{
				if (_channel[i]) _channel[i]->stop();
				continue;
			}
			if (_sound != NULL)
			{
				if (_sound[i] != NULL) _sound[i]->release();
			}
		}
	}

	SAFE_DELETE_ARRAY(_channel);
	SAFE_DELETE_ARRAY(_sound);

	if (_system != NULL)
	{
		_system->release();
		_system->close();
	}
}

void soundManager::update()
{
	_system->update();
}


//���� �߰� �Լ�
void soundManager::addSound(string keyName, string soundName, bool bgm, bool loop)
{
	if (loop)
	{
		if (bgm)
		{
			//createStream = 1���̻� ����Ǵ� ������ �Ҷ� ���� ������ ȿ�����̶�� �ϴµ� ������ ��
			//FMOD_LOOP_NORMAL = �ݺ������ �ϰڴ�.
			//null = ���� ���Ͽ� �߰� ������ �ְڳ�(�ƿ����� �˾Ƽ� �̹��� �������� ���⿡���� �����ش�) ���� ���� �̹������
			//_mTotalSounds = �� �����̳� ������
			_system->createStream(soundName.c_str(), FMOD_LOOP_NORMAL, NULL, &_sound[_mTotalSounds.size()]);
		}
		else
		{
			_system->createSound(soundName.c_str(), FMOD_LOOP_NORMAL, NULL, &_sound[_mTotalSounds.size()]);
		}
	}
	else
	{
		if (bgm)
		{
			//FMOD_DEFAULT = �Ϲ� ���
			_system->createStream(soundName.c_str(), FMOD_DEFAULT, NULL, &_sound[_mTotalSounds.size()]);
		}
		else
		{
			_system->createSound(soundName.c_str(), FMOD_DEFAULT, NULL, &_sound[_mTotalSounds.size()]);
		}
	}

	_mTotalSounds.insert(make_pair(keyName, &_sound[_mTotalSounds.size()]));
}


// 1.0 maximum 0.0 ~ 1.0 -> 0 ~ 255
void soundManager::play(string keyName, float volume)
{
	arrSoundsIter iter = _mTotalSounds.begin();
	//�迭 �� ���°���� ���˸������� ī��Ʈ ����
	int count = 0;

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_system->playSound(FMOD_CHANNEL_FREE, *iter->second, false, &_channel[count]);

			_channel[count]->setVolume(volume);
			break;
		}
	}
}

void soundManager::stop(string keyName)
{
	arrSoundsIter iter = _mTotalSounds.begin();
	//�迭 �� ���°���� ���˸������� ī��Ʈ ����
	int count = 0;

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_channel[count]->stop();
			break;
		}
	}
}

void soundManager::pause(string keyName)
{
	arrSoundsIter iter = _mTotalSounds.begin();
	//�迭 �� ���°���� ���˸������� ī��Ʈ ����
	int count = 0;

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_channel[count]->setPaused(true);
			break;
		}
	}
}

void soundManager::resume(string keyName)
{
	arrSoundsIter iter = _mTotalSounds.begin();
	//�迭 �� ���°���� ���˸������� ī��Ʈ ����
	int count = 0;

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_channel[count]->setPaused(false);
			break;
		}
	}
}


bool soundManager::isPlaySound(string keyName)
{
	bool isPlay;

	arrSoundsIter iter = _mTotalSounds.begin();

	int count = 0;

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_channel[count]->isPlaying(&isPlay);
			break;
		}
	}

	return isPlay;
}

bool soundManager::isPauseSound(string keyName)
{
	bool isPause;

	arrSoundsIter iter = _mTotalSounds.begin();

	int count = 0;

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_channel[count]->getPaused(&isPause);
			break;
		}
	}

	return isPause;
}