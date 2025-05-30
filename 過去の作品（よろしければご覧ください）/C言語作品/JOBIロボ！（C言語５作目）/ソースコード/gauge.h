//=========================================================
//
// UI用のゲージ表記処理
// Author : Shina Taiyo
//
//=========================================================
#ifndef _GAUGE_H_
#define _GAUGE_H_

#include "main.h"

#define NUM_GAUGEFRAME (4)             //ゲージのフレームの種類
#define MAX_GAUGEFRAME (4)             //ゲージフレームの最大数
#define MAX_GAUGE (NUM_GAUGEFRAME)     //ゲージの最大数（ゲージフレームと同じ数）

typedef enum
{
	SCRAPGAUGEFRAMETYPE00_TIBOROBO = 0,//ちびロボスクラップゲージフレーム
	SCRAPGAUGEFRAMETYPE01_SWORD,       //剣スクラップゲージフレーム
	SCRAPGAUGEFRAMETYPE02_BOOMERANG,   //ブーメランスクラップゲージフレーム
	SCRAPGAUGEFRAMETYPE03_SPEAKER,     //スピーカースクラップゲージフレーム
	SCRAPGAUGEFRAMETYPE_MAX
}SCRAPGAUGEFRAMETYPE;                  //スクラップゲージの種類

typedef struct
{
	bool bUse;                    //使用しているかどうか
	int nType;                    //ゲージフレームの分類を増やすケースに備え、intにしとく
	D3DXVECTOR3 pos;              //位置
	float fGaugeLength;           //ゲージの長さ
}GaugeFrame;

typedef struct
{
	D3DXVECTOR3 pos;              //位置
	float fGauge;                 //ゲージ表記
	float fGaugeLength;           //ゲージの長さ
	float fNextLevelGauge;        //次のレベルまでのゲージの値
	int nGaugeNum;              //ゲージの番号
}Gauge;


//=========================================================
// プロトタイプ宣言
//=========================================================
void InitGauge(void);	                             //初期化処理
void UninitGauge(void);	                             //終了処理
void UpdateGauge(void);	                             //更新処理
void DrawGauge(void);	                             //描画処理
void AddGauge(int nType,int nScrap,int nNextLvScrap);//ゲージを増やす処理
#endif
