//========================================================
//
//１１月７日：Xファイルを読み込む[Edit.h]
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _EDIT_H_
#define _EDIT_H_

#include "main.h"

#define NUM_EDIT (100)//モデルの種類
#define MAX_EDIT (100)//読み込むモデルの最大数

typedef struct
{
	D3DXVECTOR3 pos;//位置
	D3DXVECTOR3 rot;//向き
	int nType;//種類
	int nIdxShadow;//影のインデックス
	int nEditMode;//エディットする種類
	bool bUse;//使用しているかどうか
}Edit;

typedef enum
{
	EDITTYPE_MODEL = 0,//モデルの配置を行う
	EDITTYPE_ENEMY,//敵の配置を行う
	EDITTYPE_MAX,//最大数
}EDITTYPE;

//プロトタイプ宣言
void InitEdit(void);
void UninitEdit(void);
void UpdateEdit(void);
void DrawEdit(void);
void SetEdit(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType);
Edit* GetEdit(void);//エディット状態を取得する
#endif


