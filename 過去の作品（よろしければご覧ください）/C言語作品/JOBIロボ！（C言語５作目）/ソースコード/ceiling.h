//=======================================================================================================================================================================================================================
//
// 天井の処理に関するヘッダーファイル
// Author : Atsumu Kuboichi
//
//=======================================================================================================================================================================================================================
#ifndef _CEILING_H_
#define _CEILING_H_

#include "main.h"

//=======================================================================================================================================================================================================================
// 構造体
//=======================================================================================================================================================================================================================
typedef struct
{
	PDIRECT3DVERTEXBUFFER9 pVtxBuff;//頂点バッファへのポインタ
	LPDIRECT3DTEXTURE9 pTexture;	//テクスチャへのポインタ
	D3DXMATRIX mtxworld;			//ワールドマトリックス
	D3DXVECTOR3 pos;				//位置
	D3DXVECTOR3 rot;				//向き
	float fWidth;					//横幅
	float fHeight;					//縦幅
}Ceiling;

//=======================================================================================================================================================================================================================
// プロトタイプ宣言
//=======================================================================================================================================================================================================================
void InitCeiling(void);		//初期化処理
void UninitCeiling(void);	//終了処理
void DrawCeiling(void);		//描画処理

#endif
