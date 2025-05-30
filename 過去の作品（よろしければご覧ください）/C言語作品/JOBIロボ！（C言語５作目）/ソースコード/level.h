//========================================================
//
//１２月２７日：レベル回収処理を行う。[Level.h]
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _LEVEL_H_
#define _LEVEL_H_

#include "main.h"

#define NUM_LEVEL (4)//レベルUIの種類数
#define LEVEL_DIGIT (2)//レベル数値の桁数
#define LEVELRADIUSSIZE (20.0f)//レベルの半径参照の大きさ
#define MAX_LEVELUPDISPLAY (10)//レベルアップ表示の最大数

typedef struct
{
	D3DXVECTOR3 pos;//位置
	int nType;//種類
	int nLevel;//現在のレベル
}Level;

typedef struct
{
	D3DXVECTOR3 pos;//位置
	float fPosTexU;//数字の表示用
}LevelNum;

typedef struct
{
	D3DXVECTOR3 pos;//位置
	D3DXCOLOR col;  //色
	bool bUse;      //使用状態
	int nLife;      //体力
	int nType;      //種類
	D3DXMATRIX mtxWorld;//マトリックスワールド
}LevelUpDisplay;



//プロトタイプ宣言
void InitLevel(void);
void UninitLevel(void);
void UpdateLevel(void);
void DrawLevel(void);

void InitLevelUpDisplay(void);
void UninitLevelUpDisplay(void);
void UpdateLevelUpDisplay(void);
void DrawLevelUpDisplay(void);
void SetLevelUpDisplay(D3DXVECTOR3 pos, D3DXCOLOR col,int nLife);

void InitLevelUpDisplay2D(void);
void UninitLevelUpDisplay2D(void);
void UpdateLevelUpDisplay2D(void);
void DrawLevelUpDisplay2D(void);
void SetLevelUpDisplay2D(D3DXVECTOR3 pos, D3DXCOLOR col, int nLife,int nType);

Level* GetLevel(void);//弾構造体の情報をゲットする
void AddLevel(int nType);//レベルを上げる。
#endif




