//========================================================
//
//８月３０日：エフェクトもう一回見直す[explosion.cpp]
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

#include "main.h"

typedef enum
{
	EXPLOSIONTYPE_NORMAL=0,//普通の爆発
	EXPLOSIONTYPE_NOVA,//プレイヤーのスキル「ノヴァブラスト」の爆発
	EXPLOSIONTYPE_E6,//エネミー６の攻撃の爆発
	EXPLOSIONTYPE_MAX
}EXPLOSION;//

//プロトタイプ宣言
void InitExplosion(void);
void UninitExplosion(void);
void UpdateExplosion(void);
void DrawExplosion(void);
void SetExplosion(D3DXVECTOR3 pos, D3DXCOLOR col,int nType ,float fWidth,float fHeight);

#endif