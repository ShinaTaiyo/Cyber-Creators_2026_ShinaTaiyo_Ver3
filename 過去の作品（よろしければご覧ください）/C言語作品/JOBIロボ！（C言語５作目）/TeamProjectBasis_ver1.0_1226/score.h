//========================================================
//
//スコアの処理に関するヘッダーファイル
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _SCORE_H_
#define _SCORE_H_

#include "main.h"

//=========================================================
//マクロ定義
//=========================================================
#define MAX_SCORE	(6)		//最大数
#define	SCORE_POSX	(420.0f)//X座標
#define	SCORE_POSY	(80.0f)	//Y座標

//=========================================================
//構造体の定義
//=========================================================
typedef struct
{
	D3DXVECTOR3 pos;//位置
	bool bUse;//使用しているかどうか
}Score;

//=========================================================
//プロトタイプ宣言
//=========================================================
void InitScore(void);
void UninitScore(void);
void UpdateScore(void);
void DrawScore(void);
void SetScore(int nScore);
void AddScore(int nValue);
int GetScore(void);

#endif
