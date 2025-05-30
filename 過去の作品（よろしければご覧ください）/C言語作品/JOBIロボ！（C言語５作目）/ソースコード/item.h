//========================================================
//
//１１月７日：Xファイルを読み込む[Item.h]
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _Item_H_
#define _Item_H_

#include "main.h"

#define NUM_ITEM (1)//アイテムの種類
#define MAX_ITEM (1024)//読み込む敵の最大数
#define ITEMSET "data\\TEXTFILE\\Item.txt"

#define CANDYDISP_WIDTH (85.0f)//キャンディ表示の横幅
#define CANDYDISP_HEIGHT (35.0f)//キャンディ表示の高さ

#define CANDY_DIGIT (1) //表示する桁数
#define CANDYNUM_HEIGHT (35.0f) //キャンディ数値の高さ
#define CANDYNUM_WIDTH (CANDYNUM_HEIGHT * 0.6f)  //キャンディ数値の横幅

#define Item_SCALE (10.0f)
#define MAX_ITEMTEXTURE (32)

typedef struct
{
	D3DXVECTOR3 pos;//位置
	D3DXVECTOR3 posOld;//1F前の位置
	D3DXVECTOR3 move;//移動量
	D3DXVECTOR3 rot;//向き
	D3DXVECTOR3 vtxMinItem;//それぞれの座標の敵の位置の最小値
	D3DXVECTOR3 vtxMaxItem;//それぞれの座標の敵の位置の最大値

	int nNumVtx;//頂点数
	DWORD sizeFVF;//頂点フォーマットのサイズ
	BYTE* pVtxBuff;//頂点バッファへのポインタ
	int nType;//種類
	int nIdxShadow;//影のインデックス
	bool bUse;//使用しているかどうか
	bool bIsLanding;//地面にいるかどうか
	int nNumLandingModel;//どのモデルの上に乗っているか
	
	D3DXVECTOR3 Size;//敵の大きさ
}Item;

//=============================
//アイテム
//=============================
typedef struct
{
	LPD3DXMESH pMeshItem;//メッシュ（頂点情報）へのポインタ（モデルの種類数分）
	LPD3DXBUFFER pBuffMatItem;//マテリアルへのポインタ
	DWORD dwNumMatItem;//マテリアルの数
	LPDIRECT3DTEXTURE9 apTextureItem[MAX_ITEMTEXTURE];//テクスチャへのポインタ
}ItemMat;

typedef struct
{
	bool bUse;      //使用状態
	D3DXVECTOR3 pos;//位置
	int nGetCandy;  //キャンディ獲得数
}CandyDisp;

typedef struct
{
	bool bUse;      //使用状態
	float fPosTexU; //テクスチャ座標
	D3DXVECTOR3 pos;//位置
}CandyNum;

typedef enum
{
	ITEM00_CANDY = 0,//アイテム０、キャンディ
	ITEM_MAX
}ITEMTYPE;

//プロトタイプ宣言

//アイテム
void InitItem(void);
void UninitItem(void);
void UpdateItem(void);
void DrawItem(void);

//キャンディ表示
void InitCandyDisp(void);
void UninitCandyDisp(void);
void UpdateCandyDisp(void);
void DrawCandyDisp(void);

//キャンディ獲得数表示
void InitCandyNum(void);
void UninitCandyNum(void);
void UpdateCandyNum(void);
void DrawCandyNum(void);
void AddCandyNum(int nCandy);

void SetItem(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType);
int GetNumItem(void);//読み込む敵の最大数を取得する
Item* GetItem(void);//敵構造体の情報を取得する

#endif
