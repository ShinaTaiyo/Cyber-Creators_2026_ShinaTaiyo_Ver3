//=========================================================
//
// ボスの体力ゲージ表記処理
// Author : Shina Taiyo
//
//=========================================================
#ifndef _BOSSGAUGE_H_
#define _BOSSGAUGE_H_

#include "main.h"

//=========================================================
// マクロ定義
//=========================================================
#define NUM_BOSSGAUGEFRAME	(2)					//ゲージのフレームの種類
#define MAX_BOSSGAUGEFRAME	(NUM_BOSSGAUGEFRAME)//ゲージのフレームの最大数
#define BOSSGAUGE_POS_X		(760.0f)			//X座標
#define BOSSGAUGE_POS_Y		(25.0f)				//Y座標

//=========================================================
// 状態
//=========================================================
typedef enum
{
	BOSSGAUGEFRAME00_HIDE = 0,
	BOSSGAUGEFRAME01_APPER,
	BOSSGAUGEFRAME_MAX
}BOSSGAUGEFRAME;

//=========================================================
// 構造体
//=========================================================

//ゲージのフレーム
typedef struct
{
	bool bUse;				//使用しているかどうか
	int nType;				//ゲージフレームの分類を増やすケースに備え、intにしとく
	D3DXVECTOR3 pos;		//位置
	float fBossGaugeLength;	//ゲージの長さ
}BossGaugeFrame;

//体力ゲージ
typedef struct
{
	bool bUse;                  //使用しているかどうか
	D3DXVECTOR3 pos;            //位置
	float fBossGauge;           //ゲージ表記
	float fBossGaugeLength;     //ゲージの長さ
	float fNextLevelBossGauge;	//次のレベルまでのゲージの値
	int nBossGaugeNum;          //ゲージの番号
	int nEnemyNum;              //召喚した敵の番号
}BossGauge;

//=========================================================
// プロトタイプ宣言
//=========================================================
void InitBossGauge(void);			//初期化処理
void UninitBossGauge(void);			//終了処理
void UpdateBossGauge(void);			//更新処理
void DrawBossGauge(void);			//描画処理
void SetBossGauge(int nEnemyNum);	//ボスのゲージを出現させる処理

#endif
