//========================================================
//
//１１月７日：Xファイルを読み込む[EDITENEMY.h]
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _EDITENEMY_H_
#define _EDITENEMY_H_

#include "main.h"

#define NUM_EDITENEMY (100)//モデルの種類
#define MAX_EDITENEMY (100)//読み込むモデルの最大数

typedef struct
{
	D3DXVECTOR3 pos;//位置
	D3DXVECTOR3 rot;//向き
	int nType;//種類
	int nIdxShadow;//影のインデックス
	bool bUse;//使用しているかどうか
}EditEnemy;

typedef enum
{
	EDITENEMYTYPE_BILL = 0,//ビル
	EDITENEMYTYPE_BRIDDGE,//橋
	EDITENEMYTYPE_MAX,//最大数
}EDITENEMYTYPE;

//プロトタイプ宣言
void InitEditEnemy(void);
void UninitEditEnemy(void);
void UpdateEditEnemy(void);
void DrawEditEnemy(void);
void SetEditEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType);

#endif


#pragma once
