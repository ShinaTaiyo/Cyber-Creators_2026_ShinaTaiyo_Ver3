//========================================================
//
// カメラの処理に関するヘッダーファイル
// Author : ShinaTaiyo
//
//=========================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"

//=========================================================
// マクロ定義
//=========================================================
#define TITLE_POSV_Y	(50.0f)
#define TITLE_POSV_Z	(10.0f)
#define GAME_POSV_Y		(150.0f)
#define GAME_POSV_Z		(700.0f)
#define BOSSAPPEAR_TIME	(300)	//ボス出現演出時間
#define BOSS_SETPOS_Y	(300.0f)//ボス出現演出開始時の位置(Y座標)

//=========================================================
// カメラ構造体
//=========================================================
typedef struct
{
	D3DXVECTOR3 PosV;//支点
	D3DXVECTOR3 PosR;//注視点
	D3DXVECTOR3 VecU;//上方向ベクトル
	D3DXVECTOR3 rot;//向き
	D3DXVECTOR3 move;//移動量
	D3DXVECTOR3 DecayRot;//カメラの向きを変える動きを減衰させる処理;
	D3DXVECTOR3 center;//カメラとプレイヤー間の中間点
	float fXVaim;//X方向ベクトル
	float fZVaim;//Z方向ベクトル
	float fLength;//距離
	float fRot;//どの方向を向いているか
	int nMode;//カメラモードを設定する
	int nModeOld;//1f前のカメラモード
	int nCntModeTime;//そのカメラモードになってからの時間
	bool bBossAppear;//ボスが出現したら使う
	D3DXMATRIX mtxProjection;//プロジェクションマトリックス
	D3DXMATRIX mtxView;//ビューマトリックス

	//======================================
	//３次元自機狙い
	//======================================
	float fVXaim3D;
	float fVYaim3D;
	float fVZaim3D;
	float fVLaim3D;

}Camera;

typedef enum
{
	CAMERAMODE_REVOLUTION = 0,//公転モード
	CAMERAMODE_ROTATION,//自転モード
	CAMERAMODE_BIRDSEYE,//俯瞰視点モード
	CAMERAMODE_BOSSAPPEAR,//ボス出現モード
	CAMERAMODE_MAX//カメラモード最大数
}CAMERAMODE;

//=========================================================
// プロトタイプ宣言
//=========================================================
void InitCamera(void);
void UninitCamera(void);
void UpdateCamera(void);
void SetCamera(void);
Camera* GetCamera(void);

#endif
