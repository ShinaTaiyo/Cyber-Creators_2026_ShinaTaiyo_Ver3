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
#define NUM_PAUSETEX (3)//ポーズで使うコマンドの種類数

//列挙型定義
typedef enum
{
	PAUSE_MENU_CONTINUE = 0,//コンティニューボタン用テクスチャ
	PAUSE_MENU_RETRY,//リトライボタン用テクスチャ
	PAUSE_MENU_QUIT,//クイットボタン用テクスチャ
	PAUSE_MENU_MAX
}PAUSE_MENU;

//構造体定義
typedef struct
{
	int nPauseMenu;//呼び出すコマンドの番号を決める変数
	D3DXVECTOR3 pos;//中心座標を表す。
	bool bChooseMenu;//選んだコマンドが明るくなるようにするフラグ
}Pause;


//プロトタイプ宣言
void InitPause(void);
void UninitPause(void);
void UpdatePause(void);
void DrawPause(void);

void SetEnablePause(int nType);
int GetPauseMenu(int nChooseMenu);
#endif
