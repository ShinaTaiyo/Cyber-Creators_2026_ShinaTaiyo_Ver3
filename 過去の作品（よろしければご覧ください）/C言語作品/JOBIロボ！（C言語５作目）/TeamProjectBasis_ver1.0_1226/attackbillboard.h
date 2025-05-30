//========================================================
//
//１１月１３日：３Ｄ空間で攻撃ビルボードを発射する[Attackbillboard.h]
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _Attackbillboard_H_
#define _Attackbillboard_H_

#include "main.h"

#define MAX_ATTACKBILLBOARD (5000)//攻撃ビルボードの最大数
#define NUM_ATTACKBILLBOARD (3)//攻撃ビルボードの種類数

typedef struct
{
	D3DXVECTOR3 pos;//位置
	D3DXVECTOR3 move;//移動量
	D3DXVECTOR3 rot;//向き
	D3DXVECTOR3 Size;//大きさ
	int nType;//種類
	int nLife;//体力
	int nIdxShadow;//影のインデックス
	int nNumEnemy;//何番の敵が撃ってきているか
	float fRWidth;//幅（半径）
	float fRHeight;//高さ（半径)
	float fLength;//距離
	float fRotY;//Y回転
	bool bUse;//使用しているかどうか
	bool bOneFlag;//一回だけ発動させる処理ようフラグ
	bool bChangeFlag;//値を上下させる時に使うフラグ
	int nMaxLife;//最大体力

	float fVXaim;//X方向ベクトル
	float fVYaim;//Y方向ベクトル
	float fVZaim;//Z方向ベクトル
	float fVaim;//総合したベクトル

	int nAnimationCnt;//アニメーションカウント
	int nAnimationPattern;//アニメーションパターン
	int nAnimationPatternH;//縦のアニメーションパターン
}Attackbillboard;

typedef enum
{
	ATTACKBILLBOARD00_STUB = 0,//普通のバレット
	ATTACKBILLBOARD01_THUNDER, //雷
	ATTACKBILLBORAD02_FIRE,    //ファイア
	ATTACKBILLBOARDTYPE_MAX
}AttackbillboardTYPE;

//プロトタイプ宣言
void InitAttackbillboard(void);
void UninitAttackbillboard(void);
void UpdateAttackbillboard(void);
void DrawAttackbillboard(void);
void SetAttackbillboard(int nType, int nLife, float fRWidth, float fRHeight, D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot, int nNumEnemy, float fLength);

Attackbillboard* GetAttackbillboard(void);//攻撃ビルボード構造体の情報をゲットする

#endif
