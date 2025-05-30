//========================================================
//
//８月３０日：エフェクトもう一回見直す[health.cpp]
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _HEALTH_H_
#define _HEALTH_H

//プロトタイプ宣言
void InitHealth(void);
void UninitHealth(void);
void UpdateHealth(float fHealthDamage);
void DrawHealth(void);

typedef struct
{
	bool bResultFlag2;
}Result_2;

typedef struct
{
	float fHealth;
	D3DXVECTOR3 pos;//位置
}Health;

Result_2* GetResult_2(void);
Health* GetHealth(void);
#endif

