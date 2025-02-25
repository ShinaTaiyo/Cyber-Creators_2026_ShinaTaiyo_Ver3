//========================================================
//
//９月１１日：エフェクトもう一回見直す[score.h]
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _DAMAGE_H_
#define _DAMAGE_H_

#include "main.h"

typedef struct
{
	D3DXVECTOR3 pos;//位置
	D3DXVECTOR3 move;//移動量
	D3DXCOLOR col;//色
    int nLife;//寿命（どのくらいダメージ表記をするか）
	bool bUse;//ダメージ表記を使用しているかどうか
	float fDamageWrite;//表記するダメージ量
	int nDamagePosTexU;//ダメージ表記をするためのテクスチャの位置
	float a_col;//カラーのアルファ値になる値を持たせる
	float fRadius;//中心点から半径をとってポリゴンを配置。
}Damage;


//プロトタイプ宣言
void InitDamage(void);
void UninitDamage(void);
void UpdateDamage(void);
void DrawDamage(void);
void SetDamage(int nDamage,int nLife,D3DXVECTOR3 move,D3DXVECTOR3 pos,float fRadius);//posは「発射位置」、moveは「移動量」
Damage *GetDamage(void);

#endif

