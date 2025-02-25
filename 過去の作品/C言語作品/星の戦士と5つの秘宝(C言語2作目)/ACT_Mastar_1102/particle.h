//========================================================
//
//８月３０日：パーティクルやる[particle.h]
//Author:ShinaTaiyo
//
//=========================================================

#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "main.h"


#define MAX_PARTICLE (4096)
//typedef enum
//{
//	EFFECTSCALE_NORMAL = 0,
//	EFFECTSCALE_FAST,
//	EFFECTSCALE_MAX
//}EFFECTSCALE;



//プロトタイプ宣言
void InitParticle(void);
void UninitParticle(void);
void UpdateParticle(void);
void DrawParticle(void);
void SetParticle(D3DXVECTOR3 pos, D3DXCOLOR col, float fRadius, int nLife);//posは「発射位置」、moveは「移動量」,damageは「弾の攻撃力」

#endif


