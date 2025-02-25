//========================================================
//
//１１月７日：Xファイルを読み込む[Item.h]
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _Item_H_
#define _Item_H_

#include "main.h"

#define NUM_ITEM (4)//敵の種類
#define MAX_ITEM (1024)//読み込む敵の最大数
#define ITEMSET "data\\TEXTFILE\\Item.txt"

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

	
	D3DXVECTOR3 Size;//敵の大きさ
}Item;

typedef enum
{
	ITEM00_HEALHART = 0,//体力回復アイテム
	ITEM01_ENERGYBOOST,//MP回復アイテム
	ITEM02_POWFULSWORD,//攻撃力上昇アイテム
	ITEM03_CAPSULEBARRIER,//攻撃無効アイテム
	ITEM_MAX
}ITEMTYPE;

//プロトタイプ宣言
void InitItem(void);
void UninitItem(void);
void UpdateItem(void);
void DrawItem(void);
void SetItem(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType);
int GetNumItem(void);//読み込む敵の最大数を取得する
Item* GetItem(void);//敵構造体の情報を取得する


#endif

