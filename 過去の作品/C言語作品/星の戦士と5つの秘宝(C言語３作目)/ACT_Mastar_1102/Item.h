//========================================================
//
//８月３０日：エフェクトもう一回見直す[staging.h]
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _ITEM_H_
#define _ITEM_H

#include "main.h"
//プロトタイプ宣言
void InitItem(void);
void UninitItem(void);
void UpdateItem(void);
void DrawItem(void);

//構造体定義
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 move;
	bool bUse;//演出を使っているかどうか
	int type;//演出の種類]
	int HealPoint;//回復量
}Item;

typedef struct
{
	float fmoveBg;//背景を動かす速度を上げる。
	float fGameSpeed;//プレイヤーの速度を上げる。
}ItemEffect;


//列挙型定義
typedef enum
{
	ITEMTYPE_HEAL = 0,//体力回復
	ITEMTYPE_REAFORB,//草のオーブ
	ITEMTYPE_ICEORB,//氷のオーブ
	ITEMTYPE_FIREORB,//炎のオーブ
	ITEMTYPE_THUNDERORB,//雷のオーブ
	ITEMTYPE_LASTORB,//サイゴノオーブ
	ITEMTYPE_MAX
}ITEMTYPE;

void SetItem(ITEMTYPE type,D3DXVECTOR3 pos,int HealPoint);

Item* GetItem(void);

ItemEffect* GetItemEffect(void);
//Staging* GetStaging(void);
#endif

