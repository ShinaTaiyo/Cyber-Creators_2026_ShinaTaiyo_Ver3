//========================================================
//
//１１月７日：Xファイルを読み込む[ExModel.h]
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _EXMODEL_H_
#define _EXMODEL_H_

#include "main.h"

#define NUM_EXMODEL (1)//モデルの種類
#define MAX_EXMODEL (100)//読み込むモデルの最大数

typedef struct
{
	D3DXVECTOR3 pos;//位置
	D3DXVECTOR3 rot;//向き
	int nType;//種類
	float fWidth;//X方向の大きさ
	float fHeight;//Y方向の大きさ
	bool bUse;//使用しているかどうか
}ExModel;


//プロトタイプ宣言
void InitExModel(void);
void UninitExModel(void);
void UpdateExModel(void);
void DrawExModel(void);

#endif
