//========================================================
//
//１０月９日：アクションのUI作成[Combo.h]
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _COMBO_H_
#define _COMBO_H_

#include "main.h"

//プロトタイプ宣言
void InitCombo(void);
void UninitCombo(void);
void UpdateCombo(void);
void DrawCombo(void);
void AddCombo(int nCombo);
void ResetCombo(void);//コンボ数をリセットする処理
int GetCombo(void);


#endif

