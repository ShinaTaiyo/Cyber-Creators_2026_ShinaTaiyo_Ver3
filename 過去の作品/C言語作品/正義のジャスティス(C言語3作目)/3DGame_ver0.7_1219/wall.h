//========================================================
//
//８月３０日：エフェクトもう一回見直す[title.h]
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _WALL_H_
#define _WALL_H_

#include "main.h"

#define MAX_WALL (5)//壁の最大枚数

typedef struct
{
	D3DXVECTOR3 pos;//位置
	D3DXVECTOR3 rot;//向き
	float fWidth;//X方向の大きさ
	float fHeight;//Y方向の大きさ
	bool bUse;//使用しているかどうか
}Wall;

//プロトタイプ宣言
void InitWall(void);
void UninitWall(void);
void UpdateWall(void);
void DrawWall(void);

void SetWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeiht);
Wall* GetWall(void);
#endif
