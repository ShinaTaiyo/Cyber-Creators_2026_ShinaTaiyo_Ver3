//========================================================
//
// リザルトのスコアの処理に関するヘッダーファイル
// Author : Atsumu Kuboichi
//
//=========================================================
#ifndef _RESULTSCORE_H_
#define _RESULTSCORE_H_

#include "main.h"

//=========================================================
// マクロ定義
//=========================================================
#define MAX_RESULTSCORE	(6)	//桁数
#define NUM_RESULTSCORE	(3)	//表示するスコアの種類
#define MAX_RESULTENEMY	(3)	//敵撃破数
#define MAX_RESULTSCRAP	(4)	//獲得スクラップ数

//=========================================================
// 構造体
//=========================================================
typedef struct
{
	D3DXVECTOR3 pos;//位置
	float fWidth;	//横幅
	float fHeight;	//高さ
	bool bUse;		//使用状況
}ResultScore;

//=========================================================
// プロトタイプ宣言
//=========================================================
void InitResultScore(void);		//初期化処理
void UninitResultScore(void);	//終了処理
void UpdateResultScore(void);	//更新処理
void DrawResultScore(void);		//描画処理

#endif
