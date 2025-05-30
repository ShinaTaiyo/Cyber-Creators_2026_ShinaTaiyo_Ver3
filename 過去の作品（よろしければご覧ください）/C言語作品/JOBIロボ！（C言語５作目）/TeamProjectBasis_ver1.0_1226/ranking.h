//=========================================================
//
// ランキング画面の処理に関するヘッダーファイル
// Author : Atsumu Kuboichi
//
//=========================================================
#ifndef _RANKING_H_
#define _RANKING_H

#include "main.h"

//=========================================================
// プロトタイプ宣言
//=========================================================
void InitRanking(void);		                       //初期化処理
void UninitRanking(void);	                       //終了処理
void UpdateRanking(void);	                       //更新処理
void DrawRanking(void);		                       //描画処理
void SetRanking(int nScore);                       //ランキング設定処理
void RankingPos(D3DXVECTOR3 pos,D3DXVECTOR3 Scale);//ランキングのスコア表示の位置を決める
#endif
