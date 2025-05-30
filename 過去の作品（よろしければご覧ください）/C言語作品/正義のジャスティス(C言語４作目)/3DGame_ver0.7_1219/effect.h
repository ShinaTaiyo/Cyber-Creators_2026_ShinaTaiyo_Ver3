//========================================================
//
//１１月１３日：３Ｄ空間でエフェクトを発射する[Effect.h]
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "main.h"

#define MAX_EFFECT (50000)//エフェクトの最大数
#define NUM_EFFECT (1)//エフェクトの種類数

typedef struct
{
	D3DXVECTOR3 pos;//位置
	D3DXVECTOR3 move;//移動量
	D3DXVECTOR3 rot;//向き
	D3DXCOLOR col;//色
	int nType;//種類
	int nLife;//体力
	int nIdxShadow;//影のインデックス
	float fRWidth;//幅（半径）
	float fRHeight;//高さ（半径）
	float fScale;//エフェクトの大きさ
	float fRedution;//エフェクトの大きさを小さくするスピード
	float fRotMove;//動く向きの数値を決める変数
	bool bUse;//使用しているかどうか
}Effect;

typedef enum
{
	EFFECTTYPE_NORMAL = 0,//普通のエフェクト
	EFFECTTYPE_ROLING,//ぐるぐるエフェクト
	EFFECTTYPE_MAX,
}EFFECTTYPE;

//プロトタイプ宣言
void InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);
void SetEffect(int nType, int nLife, float fScale,float fRedution, D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col,float fRotMove);

#endif



