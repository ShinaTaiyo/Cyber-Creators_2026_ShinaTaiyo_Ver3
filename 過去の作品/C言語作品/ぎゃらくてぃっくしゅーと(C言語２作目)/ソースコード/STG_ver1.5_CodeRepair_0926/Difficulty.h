//========================================================
//
//９月１７日：難易度選択実装[Difficulty.h]
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _DIFFICULTY_H_
#define _DIFFICULTY_H_

#include "main.h"

//マクロ定義
#define MAX_DIFFICULTY_TEX (5)
#define MAX_WAVELENGTH_TEX (4)

//列挙型定義
typedef enum
{
	DIFFICULTY_BG=0,
	DIFFICULTY_EASY,
	DIFFICULTY_NORMAL,
	DIFFICULTY_HARD,
	DIFFICULTY_VERYHARD,
	WAVELENGTH_SHORT,
	WAVELENGTH_NORMAL,
	WAVELENGTH_LONG,
	WAVELENGTH_VERYLONG,
	DIFFICULTY_MAX
}DIFFICULTY;

//構造体定義
typedef struct
{
	int nNumDifficulty;//選ぶ難易度の種類を決める変数
	D3DXVECTOR3 pos;//中心座標を表す。
	bool bChooseDifficulty;//選んだコマンドが明るくなるようにするフラグ
}Difficulty;

typedef struct
{
	int nWaveLength;//ウェーブの長さを決める変数
	int nDifficulty;//難易度を決める変数
	int nDecideCount;//決定した回数
}GameCustom;

//プロトタイプ宣言
void InitDifficulty(void);
void UninitDifficulty(void);
void UpdateDifficulty(void);
void DrawDifficulty(void);
GameCustom *GetGameCustom(void);

#endif
