//========================================================
//
//８月３０日：エフェクトもう一回見直す[score.h]
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _SCOREBONUS_H_
#define _SCOREBONUS_H_

#include "main.h"

//プロトタイプ宣言
void InitScoreBonus(void);
void UninitScoreBonus(void);
void UpdateScoreBonus(void);
void DrawScoreBonus(void);
void SetScoreBonus(int nScore);//ウェーブクリア時にスコアボーナスをセットする

#endif

