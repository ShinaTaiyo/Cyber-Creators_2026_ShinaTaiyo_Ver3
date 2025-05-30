//========================================================
//
//１０月３１９日：宝獲得処理[Treasure.h]
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _TREASURE_H_
#define _TREASURE_H

#include "main.h"
//プロトタイプ宣言
void InitTreasure(void);
void UninitTreasure(void);
void UpdateTreasure(void);
void DrawTreasure(void);

//構造体定義
typedef struct
{
	int nTreasureType;//スキルのタイプを決める変数
	D3DXVECTOR3 pos;
	bool bUse;//宝を使っているかどうか
	int type;//宝の種類
	int nLife;//宝が消えるまでの時間
	int nTreasureCount;//宝が始まってからのカウントを開始する。
	float fTreasurePattern;//アニメーションの種類
	float fTreasurePatternCountY;//アニメーションのY座標をずらすカウント
	bool bCountStartFlag;//宝が始まってからのカウントを開始するフラグを宣言する。
	int nAnimationRepeat;//アニメーションをリピートする変数
	bool bDraw;//描画するかどうかをbUse以外でさらに判定するフラグ
	int nCntEnemy;//n番目の敵のいる位置を判定する
	float fColor_a;//色のアルファ値を操作する変数
	bool bColorChange_a;//色のアルファ値をインクリメントするか、デクリメントするかを決めるフラグ
}Treasure;



//列挙型定義
typedef enum
{
	TREASURETYPE_BOX = 0,//宝獲得表示用
	TREASURETYPE_REAF,//草の宝
	TREASURETYPE_ICE,//氷の宝
	TREASURETYPE_FIRE,//炎の宝
	TREASURETYPE_THUNDER,//雷の宝
	TREASURETYPE_LAST,//サイゴノタカラ
	TreasureTYPE_MAX
}TREASURETYPE;

void SetTreasure(TREASURETYPE type, int nCntEnemy, D3DXVECTOR3 pos);

//Treasure* GetTreasure(void);
#endif


