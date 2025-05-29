//========================================================
//
//１１月７日：Xファイルを読み込む[AttackModel.h]
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _ATTACKMODEL_H_
#define _ATTACKMODEL_H_

#include "main.h"

#define NUM_ATTACKMODEL (3)//モデルの種類
#define MAX_ATTACKMODEL (1024)//読み込むモデルの最大数
#define ATTACKMODELSET "data\\TEXTFILE\\AttackModel.txt"

typedef struct
{
	D3DXVECTOR3 pos;//位置
	D3DXVECTOR3 move;//移動量
	D3DXVECTOR3 rot;//向き
	D3DXVECTOR3 vtxMinAttackModel;//それぞれの座標のモデルの位置の最小値
	D3DXVECTOR3 vtxMaxAttackModel;//それぞれの座標のモデルの位置の最大値
	int nNumVtx;//頂点数
	DWORD sizeFVF;//頂点フォーマットのサイズ
	BYTE* pVtxBuff;//頂点バッファへのポインタ
	int nType;//種類
	int nIdxShadow;//影のインデックス
	int nLife;//体力
	bool bUse;//使用しているかどうか
	bool bOneFlag;//一回だけ処理を発動させるフラグ
}AttackModel;

typedef enum
{
	ATTACKMODELTYPE_FIRETORNADO = 0,//主人公の技：ファイアトルネード
	ATTACKMODELTYPE_METEOBALL,//主人公の技：メテオボール
	ATTACKMODELTYPE_KILLNEEDLE,//主人公の技：キルニードル
	ATTACKMODELTYPE_MAX,//最大数
}ATTACKMODELTYPE;

//プロトタイプ宣言
void InitAttackModel(void);
void UninitAttackModel(void);
void UpdateAttackModel(void);
void DrawAttackModel(void);
void SetAttackModel(D3DXVECTOR3 pos, D3DXVECTOR3 rot,D3DXVECTOR3 move, int nType,int nLife);
int GetNumAttackModel(void);//読み込むモデルの最大数を取得する
AttackModel* GetAttackModel(void);//構造体の情報を取得する
#endif


