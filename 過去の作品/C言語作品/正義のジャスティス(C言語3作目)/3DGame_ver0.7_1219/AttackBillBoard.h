//========================================================
//
//１１月１３日：３Ｄ空間で弾を発射する[AttackBillBoard.h]
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _ATTACKBILLBOARD_H_
#define _ATTACKBILLBOARD_H_

#include "main.h"

#define MAX_ATTACKBILLBOARD (5000)//攻撃ビルボードの最大数
#define NUM_ATTACKBILLBOARD (3)//攻撃ビルボードの種類数

typedef struct
{
	D3DXVECTOR3 pos;//位置
	D3DXVECTOR3 move;//移動量
	D3DXVECTOR3 rot;//向き
	D3DXVECTOR3 SavePos;//位置を保存する
	int nType;//種類
	int nLife;//体力
	int nIdxShadow;//影のインデックス
	float fRWidth;//幅（半径）
	float fRHeight;//高さ（半径）
	float fLength;//距離
	bool bUse;//使用しているかどうか
	bool bOneFlag;//一度だけ発動させる処理用
	int nAnimationPattern;
	int nAnimationCnt;
}AttackBillBoard;

typedef enum
{
	A_BILLBOARDTYPE_THUNDER = 0,//サンダー
	A_BILLBOARDTYPE_SLASH,//通常攻撃1「スラッシュ」
	A_BILLBOARDTYPE_ATOMICSLASH,//主人公の必殺技「アトミックスラッシュ」
	A_BILLBOARDTYPE_MAX
}A_BILLBOARDTYPE;

//プロトタイプ宣言
void InitAttackBillBoard(void);
void UninitAttackBillBoard(void);
void UpdateAttackBillBoard(void);
void DrawAttackBillBoard(void);
void SetAttackBillBoard(int nType, int nLife, float fRWidth, float fRHeight, D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot);

AttackBillBoard* GetAttackBillBoard(void);//弾構造体の情報をゲットする

#endif



