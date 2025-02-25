//========================================================
//
//８月３０日：エフェクトもう一回見直す[SkillName.h]
//Author:ShinaTaiyo
//
//=========================================================

#ifndef _SKILLGAGE_H_
#define _SKILLGAGE_H

#include "main.h"



//プロトタイプ宣言
void InitSkillName(void);
void UninitSkillName(void);
void UpdateSkillName(void);
void DrawSkillName(void);

//構造体定義
typedef struct
{
	int nSkillType;//スキルのタイプを決める変数
	float fSkillPattern;//使うスキルを決めさせる変数
	D3DXVECTOR3 pos;//位置
}Skill;



//列挙型定義
typedef enum
{
	SKILLTYPE_FIRETORNADO = 0,//ファイアトルネード
	SKILLTYPE_THUNDERBURST,//サンダーバースト
	SKILLTYPE_METEOBALL,//メテオボール
	SKILLTYPE_KILLNEEDLE,//キルニードル
	SKILLTYPE_CHAINFIRE,//チェインファイア
	SKILLTYPE_ATOMICSLASH,//アトミックスラッシュ
	SKILLNAME_MAX
}SkillType;

Skill* GetSkill(void);
#endif

