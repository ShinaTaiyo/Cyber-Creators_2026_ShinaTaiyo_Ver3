//========================================================
//
//１２月２７日：バッテリー回収処理を行う。[Battery.h]
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _Battery_H_
#define _Battery_H_

#include "main.h"

#define MAX_BATTERY (5000)//バッテリーの最大数
#define BATTERYDIGIT (4)//バッテリー数値の桁数
#define BATTERYRADIUSSIZE (20.0f)//バッテリーの半径参照の大きさ
#define MAX_OUTLETDISP (30)//充電器位置表示の最大数
#define OUTLETDISP_WIDTH (30.0f)
#define OUTLETDISP_HEIGHT (30.0f)
#define NUM_BATTERY (2)//バッテリーの種類数

typedef enum
{
	BATTERYTYPE00_NOWBATTERY = 0,//現在のバッテリー
	BATTERYTYPE01_MAXBATTERY,    //バッテリーの最大値
	BATTERYTYPE_MAX
}BATTERYTYPE;

typedef struct
{
	D3DXVECTOR3 pos;//位置
	float fPosTexU;//数字の表示用
}BatteryNum;

typedef struct
{
	D3DXVECTOR3 pos;//位置
	int nType;//種類
	int nBattery;//現在のバッテリー
	bool bCautionBattery;//バッテリーが少ない時に警告を出す変数
	BatteryNum aBatteryNum[BATTERYDIGIT];//バッテリー数値の構造体
    int nAnimationPattern;//アニメーションパターン
	int nAnimationCnt;      //アニメーションカウント
	bool bAnimationTurn;    //アニメーションを逆にする
}Battery;

typedef struct
{
	D3DXVECTOR3 pos;//位置
	bool bUse;//使用状態
	D3DXVECTOR3 rot;//向き
	D3DXMATRIX mtxWorld;//マトリックスワールド
	int nNumModel;//何番目のモデルが使用しているか
}OutletDisp;


//プロトタイプ宣言

//============================
//バッテリー表示
//============================
void InitBattery(void);
void UninitBattery(void);
void UpdateBattery(void);
void DrawBattery(void);
Battery* GetBattery(void);//弾構造体の情報をゲットする
void SubBattery(int nSubBattery);//バッテリーを上げる。
//================================================================

//=============================
//充電器の位置の表示
//=============================
void InitOutletDisp(void);
void UninitOutletDisp(void);
void UpdateOutletDisp(void);
void DrawOutletDisp(void);
void SetOutletDisp(D3DXVECTOR3 pos,int nNumModel);

//=============================
//最大バッテリー表示
//=============================
void InitMaxBattery(void);
void UninitMaxBattery(void);
void UpdateMaxBattery(void);
void DrawMaxBattery(void);

#endif





