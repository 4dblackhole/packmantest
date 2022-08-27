#pragma once
//한 타일의 규격을 32로한다
#define TILESIZE 32

//타일 갯수 X : 20, Y : 20개로 설정
#define TILEX 20
#define TILEY 20

//타일 총 사이즈
#define TILESIZEX TILESIZE * TILEX
#define TILESIZEY TILESIZE * TILEY

//타일세팅(샘플타일) 갯수
#define SAMPLETILEX 20
#define SAMPLETILEY 9

//타일의 속성을 부여하자
/*
비트필드 : 옛날에는 메모리가 부족하여 효율적으로 사용하기 위해
          메모리를 비트단위로 쪼개서 사용할려고 함.
		  그러나 현제는 메모리가 8,16,32,128이런식으로 높기 때문에
		  요즘은 아예 안쓴다.
*/
#define ATTR_UNMOVE 0x00000001
#define ATTR_SWAMP  0x00000002
#define ATTR_POISION 0x00000004
#define ATTR_FROZEN  0x00000008
/*
enum ATTR
{
	ATTR_UNMOVE,
	ATTR_SWAMP,
	ATTR_POISION,
	ATTR_FROZEN
};
*/

//지형에 대한 정의
enum TERRAIN
{
	TR_CEMENT, TR_DESERT, TR_GRASS, TR_WATER, TR_END
};

//오브젝트에 대한 정의 예시
enum OBJECT
{
	OBJ_BLOCK1, OBJ_BLOCK3, OBJ_BLOCKS,
	OBJ_FLAG1, OBJ_FLAG2,
	OBJ_TANK1, OBJ_TANK2,
	OBJ_NONE
};

//포지션(맵을 로드해 왔을 때 실제로 할당이 될 포시젼 영역
enum POS
{
	POS_FLAG1, POS_FLAG2, POS_TANK1, POS_TANK2
};

struct tagTile
{
	TERRAIN terrain;
	OBJECT obj;
	RECT rc;
	int terrainFrameX;
	int terrainFrameY;
	int objFrameX;
	int objFrameY;
};

struct tagSampleTile
{
	RECT rcTile;
	int terrainFrameX;
	int terrainFrameY;
};

struct tagCurrentTile
{
	int x;
	int y;
};