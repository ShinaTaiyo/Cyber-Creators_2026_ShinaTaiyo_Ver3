
//========================================================
//
//１０月１４日：マップ読み込み[warp.h]
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _WARP_H_
#define _WARP_H

#include "main.h"

#define MAX_WARP (5000)
#define NORMAL_WARPSIZE (20.0f)//普通のブロックのサイズ



//bool CollisionWarp(void);

//構造体定義
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 PlayerPos;//プレイヤーをワープさせる位置
	bool bUse;//演出を使っているかどうか
	int nRoadMapType;//呼び出すマップの種類
	int nLife;//演出が消えるまでの時間
	float fXsize;//X方向の大きさ
	float fYsize;//Y方向の大きさ
	float fRadius;//半径で大きさを決めている場合の大きさ
	bool bDraw;//描画するかどうかをbUse以外でさらに判定するフラグ
	bool bMapChange;//マップをチェンジするときに呼び出すフラグ
}Warp;




//プロトタイプ宣言
void InitWarp(void);
void UninitWarp(void);
void UpdateWarp(void);
void DrawWarp(void);
void SetWarp(int nRoadMapType, D3DXVECTOR3 pos,D3DXVECTOR3 PlayerPos);
void ResetWarp(void);//ワープを一旦全部消す
Warp* GetWarp(void);
//Warp* GetWarp(void);
#endif


