//========================================================
//
//１１月７日：Xファイルを読み込む[model.h]
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _MODEL_H_
#define _MODEL_H_

#include "main.h"

#define NUM_MODEL (100)//モデルの種類
#define MAX_MODEL (1024)//読み込むモデルの最大数
#define MODELSET "data\\TEXTFILE\\model.txt"

typedef struct
{
	D3DXVECTOR3 pos;//位置
	D3DXVECTOR3 rot;//向き
	D3DXVECTOR3 vtxMinModel;//それぞれの座標のモデルの位置の最小値
	D3DXVECTOR3 vtxMaxModel;//それぞれの座標のモデルの位置の最大値
	int nNumVtx;//頂点数
	DWORD sizeFVF;//頂点フォーマットのサイズ
	BYTE* pVtxBuff;//頂点バッファへのポインタ
	int nType;//種類
	int nIdxShadow;//影のインデックス
	bool bUse;//使用しているかどうか
}Model;

typedef enum
{
	MODELTYPE_BILL = 0,//ビル
	MODELTYPE_BRIDDGE,//橋
	MODELTYPE_MAX,//最大数
}MOELTYPE;

//プロトタイプ宣言
void InitModel(void);
void UninitModel(void);
void UpdateModel(void);
void DrawModel(void);
void SetModel(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType);
int GetNumModel(void);//読み込むモデルの最大数を取得する
bool bCollisionModel(D3DXVECTOR3* pPos,
	D3DXVECTOR3* pPosOld,
	D3DXVECTOR3* pMove,
	D3DXVECTOR3 Size
	);

#endif

