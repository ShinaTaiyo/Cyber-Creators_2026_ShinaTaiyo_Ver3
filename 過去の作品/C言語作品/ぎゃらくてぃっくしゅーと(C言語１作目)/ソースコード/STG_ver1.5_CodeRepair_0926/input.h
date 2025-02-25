//========================================================
//
//８月３０日：エフェクトもう一回見直す[input.cpp]
//Author:ShinaTaiyo
//
//=========================================================

#ifndef _INPUT_H_
#define _INPUT_H_

#include "main.h"

typedef enum
{
	JOYKEY_UP = 0,//十字キー（上）
	JOYKEY_DOWN,//十字キー（下）
	JOYKEY_LEFT,//十字キー（左）
	JOYKEY_RIGHT,//十字キー（右）
	JOYKEY_START,
	JOYKEY_BACK,
	JOYKEY_LPUSH,
	JOYKEY_RPUSH,
	JOYKEY_L1,//LTボタン（予定）
	JOYKEY_R1,//RTボタン（予定）
	JOYKEY_L2,
	JOYKEY_R2,
	JOYKEY_A,//Aボタン
	JOYKEY_B,//Bボタン
	JOYKEY_X,//Xボタン
	JOYKEY_Y,//Yボタン
	JOYKEY_MAX
}JOYKEY;
//プロトタイプ宣言
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd);


void UninitKeyboard(void);
void UpdateKeyboard(void);
bool GetKeyboardPress(int nKey);
bool GetKeyboardTrigger(int nKey);//トリガー押し

HRESULT InitJoypad(void);
void UninitJoypad(void);
void UpdateJoypad(void);
bool GetJoypadPress(JOYKEY key);
bool GetJoyPadTrigger(JOYKEY key);
#endif
