//========================================================
//
//１１月７日：Xファイルを読み込む[Summon.h]
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _SUMMON_H_
#define _SUMMON_H_

#include "main.h"

#define NUM_SUMMON (100)//敵の種類
#define MAX_SUMMON (1024)//読み込む敵の最大数
#define SUMMONSET "data\\TEXTFILE\\Summon.txt"


//プロトタイプ宣言
void InitSummon(void);
void UpdateSummon(void);
void SetSummon(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType);

#endif
