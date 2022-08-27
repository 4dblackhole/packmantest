#pragma once
#include "singletonBase.h"
#include <map>

#include "inc/fmod.hpp"	//fmod.hpp파일 인클루드
#pragma comment(lib, "lib/fmodex64_vc.lib")	//라이브러리 파일 등록

using namespace FMOD;

//사운드 수
#define SOUNDBUFFER 10
//여분의 채널(복수의 노래를 틀기위해서) 혹시몰라서
#define EXTRACHANNELBUFFER 5
//위에 꺼 합친거 15개
#define TOTALSOUNDBUFFER SOUNDBUFFER + EXTRACHANNELBUFFER	

class soundManager : public singletonBase<soundManager>
{
private:
	typedef map<string, Sound**>				arrSounds;
	typedef map<string, Sound**>::iterator		arrSoundsIter;
	typedef map<string, Channel**>				arrChannels;
	typedef map<string, Channel**>::iterator	arrChannelsIter;

private:
	//fmod 사운드 클래스(밑에 전부)
	System * _system;
	//소리 재생 관련 클래스
	Sound** _sound;
	//소리 재생 채널 관련 클래스
	Channel** _channel;

	arrSounds _mTotalSounds;

public:
	soundManager();
	~soundManager();

	HRESULT init();
	void release();
	void update();

	//사운드 추가 함수
	void addSound(string keyName, string soundName, bool bgm, bool loop);

	// 1.0 maximum 0.0 ~ 1.0 -> 0 ~ 255
	void play(string keyName, float volume = 1.0f);
	void stop(string keyName);
	void pause(string keyName);
	void resume(string keyName);

	bool isPlaySound(string keyName);
	bool isPauseSound(string keyName);

};

