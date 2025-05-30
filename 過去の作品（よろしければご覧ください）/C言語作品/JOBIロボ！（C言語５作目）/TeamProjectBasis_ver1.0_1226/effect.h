//========================================================
//
// エフェクトの処理に関するヘッダーファイル
// Author:ShinaTaiyo
//
//========================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "main.h"

//========================================================
// マクロ定義
//========================================================
#define MAX_EFFECT (50000)//エフェクトの最大数
#define NUM_EFFECT (1)//エフェクトの種類数
#define EFFECT_SCALE (10.0f)//エフェクトのスケール

//========================================================
// 構造体
//========================================================
typedef struct
{
	D3DXVECTOR3 pos;//位置
	D3DXVECTOR3 move;//移動量
	D3DXVECTOR3 rot;//向き
	D3DXCOLOR col;//色
	D3DXMATRIX mtxWorldEffect;//ワールドマトリックス(float4*4)
	int nType;//種類
	int nLife;//体力
	int nMaxLife;//体力最大値
	int nNumAttackModel;//エフェクトを呼んだ攻撃モデルの番号
	int nIdxShadow;//影のインデックス
	float fRWidth;//幅（半径）
	float fRHeight;//高さ（半径）
	float fWidth;//エフェクトの横幅
	float fHeight;//エフェクトの高さ
	float fRedution;//エフェクトの大きさを小さくするスピード
	float fRotMove;//動く向きの数値を決める変数
	bool bUse;//使用しているかどうか
}Effect;

//========================================================
// 状態
//========================================================
typedef enum
{
	EFFECTTYPE_NORMAL = 0,//普通のエフェクト
	EFFECTTYPE_ROLING,//ぐるぐるエフェクト
	EFFECTTYPE_MAX,
}EFFECTTYPE;

//========================================================
// プロトタイプ宣言
//========================================================
void InitEffect(void);//初期化処理
void UninitEffect(void);//終了処理
void UpdateEffect(void);//更新処理
void DrawEffect(void);//描画処理
void SetEffect(int nType, int nLife, float fWidth, float fHeight, float fRedution, D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col,float fRotMove,int nNumModel);//設定処理

#endif
