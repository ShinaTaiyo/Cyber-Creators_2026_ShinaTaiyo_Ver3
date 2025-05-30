//========================================================
//
//８月３０日：エフェクトもう一回見直す[score.h]
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _SCORE_H_
#define _SCORE_H_

#include "main.h"

//プロトタイプ宣言
void InitScore(void);
void UninitScore(void);
void UpdateScore(void);
void DrawScore(void);
void SetScore(int nScore);//posは「発射位置」、moveは「移動量」
void AddScore(int nValue);
int GetScore(void);

#endif
