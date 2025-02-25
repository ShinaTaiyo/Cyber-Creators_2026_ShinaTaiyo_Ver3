//========================================================
//
//８月３０日：エフェクトもう一回見直す[effect.cpp]
//Author:ShinaTaiyo
//
//=========================================================

#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "main.h"

typedef enum
{
	EFFECTSCALE_NORMAL=0,
	EFFECTSCALE_FAST,
	EFFECTSCALE_MAX
}EFFECTSCALE;

//プロトタイプ宣言
void InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);
void SetEffect(D3DXVECTOR3 pos,D3DXCOLOR col,float fRadius, int nLife,int nType);//posは「発射位置」、moveは「移動量」,damageは「弾の攻撃力」

#endif

