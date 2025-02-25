//========================================================
//
//１０月１４日：マップの設定[map.h]
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _MAP_H_
#define _MAP_H

#include "main.h"

#define MAX_MAP (5000)
#define NORMAL_MAPSIZE (20.0f)//普通のブロックのサイズ



//bool CollisionMAP(void);

//構造体定義
//typedef struct
//{
//	D3DXVECTOR3 pos;
//	bool bUse;//演出を使っているかどうか
//	int nType;//演出の種類
//	int nLife;//演出が消えるまでの時間
//	float fXsize;//X方向の大きさ
//	float fYsize;//Y方向の大きさ
//	float fRadius;//半径で大きさを決めている場合の大きさ
//	bool bDraw;//描画するかどうかをbUse以外でさらに判定するフラグ
//}Map;

typedef struct
{
	bool bDefeatBoss1;//ボス１を倒したかどうか判定するフラグ
	bool bDefeatBoss2;//ボス２を倒したかどうか判定するフラグ
	bool bDefeatBoss3;//ボス３を倒したかどうか判定するフラグ
	bool bDefeatBoss4;//ボス４を倒したかどうか判定するフラグ
}Map_h_DefeatBoss;

typedef enum
{
	CHALLENGESTAGE_1 = 0,//ステージ１に挑戦した
	CHALLENGESTAGE_2,//ステージ２に挑戦した
	CHALLENGESTAGE_3,//ステージ３に挑戦した
	CHALLENGESTAGE_4,//ステージ４に挑戦した
	CHALLENGESTAGE_MAX
}CHALLENGESTAGE;

//列挙型定義
typedef enum
{
	MAPTYPE_0 = 0,//マップ番号０
	MAPTYPE_1,//マップ番号１
	MAPTYPE_2,//マップ番号２
	MAPTYPE_3,//マップ番号３
	MAPTYPE_4,//マップ番号４
	MAPTYPE_5,//マップ番号５
	MAPTYPE_6,//マップ番号６
	MAPTYPE_7,//マップ番号７
	MAPTYPE_8,//マップ番号８
	MAPTYPE_9,//マップ番号９
	MAPTYPE_10,//マップ番号１０
	MAPTYPE_11,//マップ番号１１
	MAPTYPE_12,//マップ番号１２
	MAPTYPE_13,//マップ番号１３
	MAPTYPE_14,//マップ番号１４
	MAPTYPE_15,//マップ番号１５
	MAPTYPE_16,//マップ番号１６
	MAPTYPE_17,//マップ番号１７
	MAPTYPE_18,//マップ番号１８
	MAPTYPE_19,//マップ番号１９
	MAPTYPE_20,//マップ番号２０
	MAPTYPE_21,//マップ番号２１
	MAPTYPE_22,//マップ番号２２
	MAPTYPE_23,//マップ番号２３
	MAPTYPE_24,//マップ番号２４
	MAPTYPE_25,//マップ番号２５
	MAPTYPE_26,//マップ番号２６
	MAPTYPE_27,//マップ番号２７
	MAPTYPE_28,//マップ番号２８
	MAPTYPE_29,//マップ番号２９
	MAPTYPE_30,//マップ番号３０
	MAPTYPE_31,//マップ番号３１
	MAPTYPE_32,//マップ番号３２
	MAPTYPE_33,//マップ番号３３
	MAPTYPE_34,//マップ番号３４
	MAPTYPE_35,//マップ番号３５
	MAPTYPE_36,//マップ番号３６
	MAPTYPE_37,//マップ番号３７
	MAPTYPE_38,//マップ番号３８
	MAPTYPE_39,//マップ番号３９
	MAPTYPE_40,//マップ番号４０
	MAPTYPE_41,//マップ番号４１
	MAPTYPE_MAX
}MAPTYPE;

//プロトタイプ宣言
void InitMap(void);
void SetMap(int ntype);
Map_h_DefeatBoss * GetDefeatBoss(void);
int * GetUseMap(void);
int * GetChallengeStage(void);
#endif



