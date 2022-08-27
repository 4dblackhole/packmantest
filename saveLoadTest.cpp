#include "stdafx.h"
#include "saveLoadTest.h"


saveLoadTest::saveLoadTest()
{
}


saveLoadTest::~saveLoadTest()
{

}

void saveLoadTest::update()
{
	if (KEYMANAGER->isOnceKeyDown(VK_F1)) save();
	if (KEYMANAGER->isOnceKeyDown(VK_F2)) load();
}

void saveLoadTest::save()
{
	//파일 입출력
	// C - FILE*
	// C++ - ifstream, ofstream
	// WIN32 - CreateFile

	HANDLE file;
	DWORD write;

	char str[128] = "아이돌마스터 신데렐라 걸즈";
	//                 파일이름           쓰기 권환  공유모드 상속(자식)에게 값을 넘길것인가, 항상 새로운 파일을 만든다, 모든 속성(읽기 전용, 숨김, 운영체제전용)을 지정하지 않는다, 생성된 템플릿 파일의 유효한 핸들에 속성을 넣는 BOOL값
	file = CreateFile("칸자키란코.txt", GENERIC_WRITE, FALSE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	//	     대상파일의 핸들, 데이터가 들어있는 버퍼쓰고자 하는 바이트수, 쓰고자 하는 바이트 수를 리턴하기위한DWORD변수의 포인터, 비동기입출력을 할때 사용 BOOL값
	//DWORD = 32비트 64비트 관계없이 컴파일하여 1개를 나타내는 것
	WriteFile(file, str, strlen(str), &write, NULL);

	CloseHandle(file);

}

void saveLoadTest::load()
{
	HANDLE file;
	DWORD read;

	char str[128];
	//                                   읽기 권환                 파일이 존재하면 연다, 모든 속성을 지정하지 않는다.
	file = CreateFile("칸자키란코.txt", GENERIC_READ, FALSE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	ReadFile(file, str, strlen(str), &read, NULL);

	CloseHandle(file);
	// MB_OK -> 화면에 확인버튼만 출력되게 한다.(버튼이 1개일 경우 자동 반환이 되어 따로 반환시킬 필요가 없다.)
	MessageBox(_hWnd, str, "뭐 적어야되지;", MB_OK);

}
