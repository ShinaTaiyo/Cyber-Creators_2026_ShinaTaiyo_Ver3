//=======================================================================================================================================================================================================================
//
// エディタの処理に関するヘッダーファイル
// Author : Atsumu Kuboichi
//
//=======================================================================================================================================================================================================================
#ifndef _EDIT_H_ //このマクロ定義がされなかったら
#define _EDIT_H_ //2重インクルード防止のマクロ定義

#include "main.h"
#include "model.h"
#include "enemy.h"

//=======================================================================================================================================================================================================================
// マクロ定義
//=======================================================================================================================================================================================================================
#define EDIT_SPEED		(5.0f)								//キー入力の移動量
#define EDIT_DIRECTION	(8)									//向きの分割数
#define EDIT_TURN		(D3DX_PI * (1.0f / EDIT_DIRECTION))	//回転量
#define NUM_CLASS		(1)									//配置物の種類
#define NUM_EDITTYPE	(NUM_MODEL)							//配置物の合計種類
#define MAX_EDITMAT		(100)								//各モデルのマテリアル数の最大数

//=======================================================================================================================================================================================================================
// 構造体
//=======================================================================================================================================================================================================================
typedef struct
{
	D3DXMATRIX mtxWorld;				//ワールドマトリックス
	D3DCOLORVALUE Diffuse[MAX_EDITMAT];	//色合い
	D3DXVECTOR3 pos;					//位置
	D3DXVECTOR3 rot;					//向き
	int nType;							//種類
	int nClass;							//配置物の種類(0:オブジェクト)
	int nUse;							//ゲームフィールドかちびロボハウスかの判定
	bool bUse;							//使用しているかどうか
	bool bBuild;						//配置済みかどうかの処理
}Edit;

//================================================================================================================================================================================================================================
// 各種類のXファイル
//================================================================================================================================================================================================================================
static int NUM_EDIT[NUM_CLASS] =
{
	NUM_MODEL,
};

//=======================================================================================================================================================================================================================
// プロトタイプ宣言
//=======================================================================================================================================================================================================================
void InitEdit(void);												//初期化処理
void UninitEdit(void);												//終了処理
void UpdateEdit(void);												//更新処理
void DrawEdit(void);												//描画処理
void WriteModel(void);												//モデル情報の書き出し処理
void SetEdit(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nUse);//設定処理
Edit* GetEdit(void);												//構造体の情報の取得

#endif
