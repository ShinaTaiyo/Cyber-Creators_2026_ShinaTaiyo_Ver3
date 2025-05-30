//========================================================
//
//１２月２７日：スクラップ回収処理を行う。[scrap.h]
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _SCRAP_H_
#define _SCRAP_H_

#include "main.h"

#define MAX_SCRAP (5000)//スクラップの最大数
#define NUM_SCRAP (5)//攻撃ビルボードの種類数（一応種類後から増えた時用に、このマクロは使っておく。）
#define GENERALLYSCRAPLIFE (600)//全てのスクラップの体力
#define SCRAPRADIUSSIZE (20.0f)//スクラップの半径参照の大きさ
typedef enum
{
	SCRAPTYPE00_TIBIROBO = 0,//ちびロボ強化スクラップ
	SCRAPTYPE01_SWORD,       //剣攻撃強化スクラップ
	SCRAPTYPE02_BOOMERANG,   //ブーメラン強化スクラップ
	SCRAPTYPE03_SPEAKER,     //スピーカー強化スクラップ
	SCRAPTYPE04_ALL,          //全ステータス強化スクラップ
	SCRAPTYPE_MAX
}SCRAPTYPE;

typedef enum
{
	SCRAPMOVETYPE_DIFFUSION = 0,//拡散させるときの移動のしかた
	SCRAPMOVETYPE_NORMAL,        //普通の移動の仕方
	SCRAPMOVETYPE_MAX
}SCRAPMOVETYPE;

typedef struct
{
	D3DXVECTOR3 pos;//位置
	D3DXVECTOR3 move;//移動量
	D3DXVECTOR3 rot;//向き

	D3DXVECTOR3 DecayMove;//減衰させる移動量
	int nType;//種類
	int nLife;//体力
	int nIdxShadow;//影のインデックス
	float fRWidth;//幅（半径）
	float fRHeight;//高さ（半径）
	float fLength;//距離
	bool bUse;//使用しているかどうか
	int nAnimationPattern;
	int nAnimationCnt;

	float fXaim;//X方向のベクトル
	float fYaim;//Y方向のベクトル
	float fZaim;//Z方向のベクトル

	float fVXZaim;//XとZのベクトル
	float fVYZaim;//YとZのベクトル

	//==============================
	//三次元自機狙い変数
	//==============================
	float fVXaim3D;
	float fVYaim3D;
	float fVZaim3D;
	float fVLaim3D;
	//==============================================================

	SCRAPMOVETYPE ScrapMoveType;//スクラップの移動の仕方
}Scrap;

typedef struct
{
	D3DXVECTOR3 pos;//位置
	float fPosTexU;//数字の表示用
}ScrapUI;



//プロトタイプ宣言
void InitScrap(void);
void UninitScrap(void);
void UpdateScrap(void);
void DrawScrap(void);
void SetScrap(int nType, int nLife,float fRWidth, float fRHeight, D3DXVECTOR3 pos, D3DXVECTOR3 rot,D3DXVECTOR3 DiffusionMove);
Scrap* GetScrap(void);//弾構造体の情報をゲットする

#endif