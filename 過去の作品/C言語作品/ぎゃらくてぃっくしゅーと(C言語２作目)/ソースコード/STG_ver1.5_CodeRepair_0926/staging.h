//========================================================
//
//８月３０日：エフェクトもう一回見直す[staging.h]
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _STAGING_H_
#define _STAGING_H

#include "main.h"
//プロトタイプ宣言
void InitStaging(void);
void UninitStaging(void);
void UpdateStaging(void);
void DrawStaging(void);

//構造体定義
typedef struct
{
	int nStagingType;//スキルのタイプを決める変数
	D3DXVECTOR3 pos;
	bool bUse;//演出を使っているかどうか
	int type;//演出の種類
	int nLife;//演出が消えるまでの時間
	int nStagingCount;//演出が始まってからのカウントを開始する。
	float fStagingPattern;//アニメーションの種類
	float fStagingPatternCountY;//アニメーションのY座標をずらすカウント
	bool bCountStartFlag;//演出が始まってからのカウントを開始するフラグを宣言する。
	int nAnimationRepeat;//アニメーションをリピートする変数
	bool bDraw;//描画するかどうかをbUse以外でさらに判定するフラグ
	int nCntEnemy;//n番目の敵のいる位置を判定する
	float fColor_a;//色のアルファ値を操作する変数
	bool bColorChange_a;//色のアルファ値をインクリメントするか、デクリメントするかを決めるフラグ
}Staging;



//列挙型定義
typedef enum
{
	STAGINGTYPE_WARNING = 0,//ルールが書かれた画面を呼び出す。
	STAGINGTYPE_APPER,//敵出現位置を予告する演出
    STAGINGTYPE_BULLETWARNING,//特定の弾を出現を予告する
	STAGINGTYPE_PRESSSTART,//「PressStart」の文字を表示する。
	STAGINGTYPE_PRESSENTER,//「PressEnter」の文字を表示する。
	STAGINGTYPE_SHIELD,//「スキル「イージスシールド」の代わりに使う列挙型定義
	STAGINGTYPE_NORMAL_CLEARBONUS,//ノーマルモードのクリアボーナス表記
	STAGINGTYPE_HARD_CLEARBONUS,//ハードモードのクリアボーナス表記
	STAGINGTYPE_VERYHARD_CLEARBONUS,//ベリーハードのクリアボーナス表記
	STAGINGTYPE_RANKING_BG,//ランキングの背景をここで一旦表示させる。
	STAGINGTYPE_MAX
}STAGINGTYPE;

void SetStaging(STAGINGTYPE type,int nCntEnemy,D3DXVECTOR3 pos);

//Staging* GetStaging(void);
#endif

