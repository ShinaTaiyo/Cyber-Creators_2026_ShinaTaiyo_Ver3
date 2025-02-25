
//８月３０日：エフェクトもう一回見直す[pause.cpp]
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _CHARACTER_H_
#define _CHACACTER_H_

#include "main.h"

//マクロ定義
#define MAX_CHARACTER_TEX (2)

//列挙型定義
typedef enum
{
	CHACACTER_TYPE_LIGHT=0,
	CHACACTER_TYPE_HEAVY,
	CHARACTER_TYPE_MAX
}CHARACTER_TYPE;

//構造体定義
typedef struct
{
	int nType;//呼び出すコマンドの種類を決める変数
	D3DXVECTOR3 pos;//中心座標を表す。
	bool bChooseCharacter;
}Character;

typedef struct
{
	int nCharacterNum;
}CharacterNum;

//プロトタイプ宣言
void InitChacacter(void);
void UninitChacacter(void);
void UpdateCharacter(void);
void DrawCharacter(void);
CharacterNum *GetCharacterNum(void);
#endif


#pragma once
