//========================================================
//
//１１月７日：Xファイルを読み込む[Block.h]
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _BLOCK_H_
#define _BLOCK_H_

#include "main.h"

#define NUM_BLOCK (1)//モデルの種類
#define MAX_BLOCK (100)//読み込むモデルの最大数

typedef struct
{
	D3DXVECTOR3 pos;//位置
	D3DXVECTOR3 rot;//向き
	int nType;//種類
	int nIdxShadow;//影のインデックス
	float fWidth;//X方向の大きさ
	float fHeight;//Y方向の大きさ
	bool bUse;//使用しているかどうか
}Block;

typedef enum
{
	BLOCKTYPE_RED = 0,//赤ブロック
	BLOCKTYPE_BRUE,//青ブロック
	BLOCKTYPE_MAX
}BLOCKTYPE;

//プロトタイプ宣言
void InitBlock(void);
void UninitBlock(void);
void UpdateBlock(void);
void DrawBlock(void);
void SetBlock(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType);
Block* GetBlock(void);

#endif


