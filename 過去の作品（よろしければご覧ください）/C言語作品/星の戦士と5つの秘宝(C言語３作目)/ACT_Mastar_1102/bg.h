//========================================================
//
//８月３０日：エフェクトもう一回見直す[bg.cpp]
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _BG_H_
#define _BG_H

typedef enum
{
	BG_CENTRALCIRCLE,//セントラルサークル背景
	BG_STAGE1,//ステージ１背景
	BG_STAGE2,//ステージ２背景
	BG_STAGE3,//ステージ３背景
	BG_STAGE4,//ステージ４背景
	BG_LASTSTAGE,//ラストステージ背景
	BG_MAX
}BG;

//プロトタイプ宣言
void InitBG(void);
void UninitBG(void);
void UpDateBG(void);
void DrawBG(void);

#endif
