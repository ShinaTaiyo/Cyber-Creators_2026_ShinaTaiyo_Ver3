//========================================================
//
//１２月２６日：チーム制作基盤作り[title.h]
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _SHADOW_H
#define _SHADOW_H_

#include "main.h"


#define MAX_SHADOW (10000)

typedef struct
{
	D3DXVECTOR3 pos;//位置
	D3DXVECTOR3 rot;//向き
	D3DXMATRIX mtxWorld;//マトリックスワールド
	int nNumBullet;//読み込んだ弾の番号
	bool bUse;//使用しているかどうか
}Shadow;

//プロトタイプ宣言
void InitShadow(void);
void UninitShadow(void);
void UpdateShadow(void);
void DrawShadow(void);

int SetShadow(void);
void SetPositionShadow(int nldxShadow, D3DXVECTOR3 pos);
Shadow* GetShadow(void);
void KillShadow(int nIdxShadow);

#endif

