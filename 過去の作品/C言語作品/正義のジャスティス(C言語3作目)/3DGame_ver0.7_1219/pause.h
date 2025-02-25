//========================================================
//
//８月３０日：エフェクトもう一回見直す[pause.cpp]
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _PAUSE_H_
#define _PAUSE_H_

#include "main.h"

//マクロ定義
#define MAX_PAUSE_TEX (4)

//列挙型定義
typedef enum
{
	PAUSE_MENU_INDICATE=0,
	PAUSE_MENU_CONTINUE,
	PAUSE_MENU_RETRY,
	PAUSE_MENU_QUIT,
	PAUSE_MENU_MAX
}PAUSE_MENU;

//構造体定義
typedef struct
{
	int nPauseMenu;//呼び出すコマンドの種類を決める変数
	D3DXVECTOR3 pos;//中心座標を表す。
	bool bChooseMenu;//選んだコマンドが明るくなるようにするフラグ
}Pause;


//プロトタイプ宣言
void InitPause(void);
void UninitPause(void);
void UpdatePause(void);
void DrawPause(void);

void SetEnablePause(int nType);
#endif
