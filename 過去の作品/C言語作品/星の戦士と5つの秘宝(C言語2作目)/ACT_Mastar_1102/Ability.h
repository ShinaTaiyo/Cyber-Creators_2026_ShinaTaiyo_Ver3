//========================================================
//
//１０月９日：アクションのUI作成[Ability.h]
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _ABILITY_H_
#define _ABILITY_H_

#include "main.h"

//アビリティ列挙型定義
typedef enum
{
	ABILITYTYPE_NORMAL=0,
	ABILITYTYPE_BULLET,
	ABILITYTYPE_MAX
}ABILITYTYPE;

//アビリティ構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;//位置
	int nAbilityType;//アビリティの種類
	bool bUse;//アビリティ表示をしようしているかどうか
}Ability;

//プロトタイプ宣言
void InitAbility(void);
void UninitAbility(void);
void UpdateAbility(void);
void DrawAbility(void);
void SetAbility(ABILITYTYPE type);//アビリティ設定処理
Ability * GetAbility(void);//アビリティ情報を取得

#endif
