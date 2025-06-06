//================================================================================================================================================================================================================================
//
// ビルボードの処理
// Author : Atsumu Kuboichi
//
//================================================================================================================================================================================================================================
#ifndef _BILLBOARD_H_
#define _BILLBOARD_H_

#include "main.h"

//================================================================================================================================================================================================================================
// マクロ定義
//================================================================================================================================================================================================================================
#define MAX_BILLBOARD		(100)									//最大数
#define TUTORIAL_BILLBOARD	(1)										//チュートリアル用種類
#define GAME_BILLBOARD		(1)										//ゲーム用種類
#define NUM_BILLBOARD		(TUTORIAL_BILLBOARD + GAME_BILLBOARD)	//合計種類
#define DIVISION_SCALE		(10.0f)									//拡縮の分割数

//================================================================================================================================================================================================================================
// マクロ定義
//================================================================================================================================================================================================================================
typedef enum
{
	USE_GAME,		//ゲーム
	USE_TUTORIAL,	//チュートリアル
	USE_MAX
}USEMODE;	//使用場面

//================================================================================================================================================================================================================================
// 構造体
//================================================================================================================================================================================================================================
typedef struct
{
	LPD3DXMESH pMesh;		//メッシュの(頂点情報)へのポインタ
	LPD3DXBUFFER pBuffMat;	//マテリアルのポインタ
	DWORD dwNumMat;			//マテリアルの数
	D3DXMATRIX mtxWorld;	//ワールドマトリックス
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 rot;		//向き
	int nUse;				//使用場面
	int nType;				//種類
	float fWidth;			//横幅
	float fHeight;			//高さ
	float fWidthScale;		//横幅拡縮率
	float fHeightScale;		//高さ拡縮率
	float fLength;			//プレイヤーとの距離
	bool bUse;				//使用状態
}Billboard;

//================================================================================================================================================================================================================================
// ゲーム用テクスチャファイル
//================================================================================================================================================================================================================================
static const char* GAME_BILLBOARDFILE[GAME_BILLBOARD] =
{
	"data\\TEXTURE\\heal000.png",	//「Aで回復」
};

//================================================================================================================================================================================================================================
// チュートリアル用テクスチャファイル
//================================================================================================================================================================================================================================
static const char* TUTORIAL_BILLBOARDFILE[TUTORIAL_BILLBOARD] =
{
	"data\\TEXTURE\\RuleBillboard000.png",	//「Aでルール確認」
};

//================================================================================================================================================================================================================================
// プロトタイプ宣言
//================================================================================================================================================================================================================================
void InitBillboard(void);																//初期化処理
void UninitBillboard(void);																//終了処理
void UpdateBillboard(void);																//更新処理
void DrawBillboard(void);																//描画処理
void SetBillboard(D3DXVECTOR3 pos, int nUse, int nType, float fWidth, float fHeight);	//設定処理

#endif
