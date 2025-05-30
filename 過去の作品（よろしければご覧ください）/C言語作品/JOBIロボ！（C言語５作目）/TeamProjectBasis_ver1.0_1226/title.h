//=========================================================
//
// タイトル画面の処理に関するヘッダーファイル
// Author : Atsumu Kuboichi
//
//=========================================================
#ifndef _TITLE_H_
#define _TITLE_H

#include "main.h"

//=========================================================
// マクロ定義
//=========================================================
//++++++++++++++++++++++++++++++++
// タイトルロゴ
//++++++++++++++++++++++++++++++++
// タイトルロゴ基準
#define TITLELOGO_POS (D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f))			// ロゴ初期位置
#define TITLELOGO_SIZE (D3DXVECTOR2(1200.0f, 700.0f))											// ロゴ初期サイズ
// タイトルロゴ出現
#define TITLELOGO_ARR_TIME (60)																	// ロゴ出現時間
#define TITLELOGO_ARR_ROLL_POS D3DXVECTOR3(-SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0);	// ロゴ回転出現位置
#define TITLELOGO_ARR_ROLL_ROT D3DXVECTOR3(0.0f, 0.0f, (D3DX_PI * 2) * 3.0f)					// ロゴ回転出現回転数
// タイトルロゴ縮小
#define TITLELOGO_RED_START (120)																// ロゴ縮小開始時間
#define TITLELOGO_RED_TIME (60)																	// ロゴ縮小時間
#define TITLELOGO_RED_SCALE (0.5f)																// ロゴ縮小時間
#define TITLELOGO_RED_POS (D3DXVECTOR3(SCREEN_WIDTH * 0.25f, SCREEN_HEIGHT * 0.25f, 0.0f))		// ロゴ縮小位置
// タイトルロゴアニメーション全体
#define TITLELOGO_TIME (TITLELOGO_ARR_TIME + TITLELOGO_RED_START + TITLELOGO_RED_TIME)			// ロゴアニメーション時間

//++++++++++++++++++++++++++++++++
// タイトルメニュー
//++++++++++++++++++++++++++++++++
#define TITLEMENU_POS (D3DXVECTOR3(320.0f, 400.0f, 0.0f))		// タイトル位置最上段
#define TITLEMENU_SIZE (D3DXVECTOR2(300, 80.0f))				// タイトル位置最上段
#define TITLEMENU_COL_A (1.0f)
#define TITLEMENU_COL_NOTSELECT (D3DXCOLOR(1.0f, 1.0f, 1.0f, TITLEMENU_COL_A))
#define TITLEMENU_COL_SELECT (D3DXCOLOR(0.0f, 1.0f, 1.0f, TITLEMENU_COL_A))

#define TITLEMENU_TIME (30) 
#define TITLEMENU_FALL (10.0f)
//=========================================================
// 構造体定義
//=========================================================
//++++++++++++++++++++++++++++++++
// タイトルロゴ
//++++++++++++++++++++++++++++++++
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 postarget;	// 位置(目標)
	D3DXVECTOR3 rot;		// 向き
	D3DXVECTOR3 rottarget;	// 向き(目標)
	D3DXVECTOR2 size;		// 大きさ
	float fScale;			// 大きさ(倍率)
	float fScaletarget;		// 大きさ(目標)
	float fAngle;			// 頂点角度
	float fLeng;			// 原点からの頂点までのの長さ
	int nArrivalType;		// 出現タイプ
}TITLELOGO;

//++++++++++++++++++++++++++++++++
// タイトルメニュー
//++++++++++++++++++++++++++++++++
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR2 size;		// 大きさ
	D3DXCOLOR col;			// 色
	float fColaTarget;	// 色(目標)
	int nType;				// 種類
	bool bUse;				// 使用中
}TITLEMENU;
//=========================================================
// 列挙型定義
//=========================================================
//++++++++++++++++++++++++++++++++
// タイトルロゴメニュー
//++++++++++++++++++++++++++++++++
typedef enum
{
	TITLELOGO_NORMAL = 0,	// 無し
	TITLEROGO_ROLL,			// 回転
	TITLEROGO_MAX,
}TITLELOGO_ARRIVAL_TYPE;	// タイトル出現エフェクト

//++++++++++++++++++++++++++++++++
// タイトルメニュー
//++++++++++++++++++++++++++++++++
typedef enum
{
	TITLEMENU_START = 0,	// スタート
	TITLEMENU_RANKING,		// ランキング
	TITLEMENU_END,			// 終了
	TITLEMENU_MAX,
}TITLEMENU_TYPE;	// タイトル出現エフェクト

// タイトルメニューテクスチャ
static const char* cTITLEMENU_TEXTURE_FILENAME[TITLEMENU_MAX] = {
	"data\\TEXTURE\\STRAT000.png",
	"data\\TEXTURE\\RANKING000.png",
	"data\\TEXTURE\\EXIT000.png",
};


//=========================================================
// プロトタイプ宣言
//========================================================

// 全体
void InitTitle(void);		//初期化処理
void UninitTitle(void);		//終了処理
void UpdateTitle(void);		//更新処理
void DrawTitle(void);		//描画処理
void SelectTitle(void);	//選択処理
// ロゴ
void InitTitleLogo(void);	//初期化処理
void UninitTitleLogo(void);	//終了処理
void UpdateTitleLogo(void);	//更新処理
void DrawTitleLogo(void);	//描画処理
// メニュー
void InitTitleMenu(void);	//初期化処理
void UninitTitleMenu(void);	//終了処理
void UpdateTitleMenu(void);	//更新処理
void DrawTitleMenu(void);	//描画処理
void SelectTitleMenu(void);	//選択処理

#endif
