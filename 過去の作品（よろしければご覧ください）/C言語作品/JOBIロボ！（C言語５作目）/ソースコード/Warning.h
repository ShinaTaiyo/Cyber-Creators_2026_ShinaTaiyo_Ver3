//========================================================
//
//３月１４日：ボスの攻撃発動前に警告を出したい。[Warning.h]
//Author:ShinaTaiyo
// 
//
//=========================================================
#ifndef _WARNING_H_
#define _WARNING_H_

#include "main.h"

#define MAX_WARNING (32)//攻撃ビルボードの最大数

typedef struct
{
	D3DXVECTOR3 pos;//位置
	D3DXVECTOR3 Size;//大きさ
	D3DXMATRIX mtxWorld;//マトリックスワールド
	int nLife;//体力
	float fRWidth;//幅（半径）
	float fRHeight;//高さ（半径)
	bool bColChange;//２種類の色を変え続けるフラグ
	bool bUse;//使用しているかどうか
}Warning;

//プロトタイプ宣言
void InitWarning(void);
void UninitWarning(void);
void UpdateWarning(void);
void DrawWarning(void);
void SetWarning(int nLife, float fRWidth, float fRHeight, D3DXVECTOR3 pos);

Warning * GetWarning (void);//攻撃ビルボード構造体の情報をゲットする

#endif
