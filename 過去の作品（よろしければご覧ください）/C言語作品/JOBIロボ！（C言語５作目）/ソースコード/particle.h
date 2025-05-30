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
#define Particle_SCALE (10.0f)

typedef struct
{
	D3DXVECTOR3 pos;//位置
	D3DXVECTOR3 move;//移動量
	D3DXVECTOR3 rot;//向き
	D3DXCOLOR col;//色
	int nType;//種類
	int nTexType;//パーティクルのテクスチャの種類
	int nLife;//体力
	int nMaxLife;//最大体力
	int nIdxShadow;//影のインデックス
	float fRWidth;//幅（半径）
	float fRHeight;//高さ（半径）
	float fScale;//エフェクトの大きさ
	float fRedution;//エフェクトの大きさを小さくするスピード
	float fRotMove;//動く向きの数値を決める変数
	bool bUse;//使用しているかどうか
	float fSpeed;//速さ
	int nAnimationPattern;
	int nAnimaitonCount;
	int nNumModel;
	//======================================
    //３次元自機狙い弾の実装に使う変数
    //======================================
	float fVXaim3D;
	float fVYaim3D;
	float fVZaim3D;
	float fVLaim3D;

	D3DXVECTOR3 SaveMove3D;//移動量の保存をする　
	//==============================================================================================
}Particle;

typedef enum
{
	PARTICLETYPE_NORMAL = 0,//普通に飛ぶ
	PARTICLETYPE_GRAVITY,//重力をかけて飛ぶ
	PARTICLETYPE_FIRE,   //火の粉のように、少しずつ上昇量が増える
	PARTICLETYPE_METEO,  //メテオが来る時に、位置を予告するパーティクル
	PARTICLETYPE_CHARGE, //パワーをチャージしている風
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
void SetParticle(int nType, int nLife, float fScale, float fRedution, D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRotMove,int nTexType,int nNumModel);

#endif



