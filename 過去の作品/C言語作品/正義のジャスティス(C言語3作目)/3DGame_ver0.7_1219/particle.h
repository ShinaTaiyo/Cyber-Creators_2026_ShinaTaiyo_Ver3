//========================================================
//
//１１月１３日：３Ｄ空間でエフェクトを発射する[Particle.h]
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _Particle_H_
#define _Particle_H_

#include "main.h"

#define MAX_PARTICLE (50000)//パーティクルの最大数
#define NUM_PARTICLE (2)//パーティクルの種類数

typedef struct
{
	D3DXVECTOR3 pos;//位置
	D3DXVECTOR3 move;//移動量
	D3DXVECTOR3 rot;//向き
	D3DXCOLOR col;//色
	int nType;//種類
	int nTexType;//パーティクルのテクスチャの種類
	int nLife;//体力
	int nIdxShadow;//影のインデックス
	float fRWidth;//幅（半径）
	float fRHeight;//高さ（半径）
	float fScale;//エフェクトの大きさ
	float fRedution;//エフェクトの大きさを小さくするスピード
	float fRotMove;//動く向きの数値を決める変数
	bool bUse;//使用しているかどうか

	int nAnimationPattern;
	int nAnimaitonCount;
}Particle;

typedef enum
{
	PARTICLETYPE_NORMAL = 0,//普通に飛ぶ
	PARTICLETYPE_GRAVITY,//重力をかけて飛ぶ
	PARTICLETYPE_MAX
}PARTICLETYPE;

typedef enum
{
 PARTICLETEX_NORMAL,//普通のパーティクル
 PARTICLETEX_CHAINFIRE,//主人公の技：チェインファイア
 PARTICLETEX_MAX
}PARTICLETEX;

//プロトタイプ宣言
void InitParticle(void);
void UninitParticle(void);
void UpdateParticle(void);
void DrawParticle(void);
void SetParticle(int nType, int nLife, float fScale, float fRedution, D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRotMove,int nTexType);

#endif



