//========================================================
//
//１２月２７日：敵レーダー回収処理を行う。[RADER.h]
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _RADER_H_
#define _RADER_H_

#include "main.h"

#define MAX_RADERDISPLAY (5000)//レーダーで表示する最大数
#define NUM_RADERDISPLAY (5)//レーダーで表示する種類数
typedef enum
{
	RADERDISPLAY00_PLAYER = 0,//プレイヤーの位置を表示
	RADERDISPLAY01_ENEMY,     //敵の位置を表示
	RADERDISPLAY02_BOSS,      //ボスの位置を表示
	RADERDISPLAY03_OUTLET,    //充電器の位置を表示
	RADERDISPLAY04_EDIT,      //エディタの位置を表示
	RADERDISPLAY_MAX
}RADERDISPLAYTYPE;

typedef struct
{
	D3DXVECTOR3 pos;//位置
	D3DXVECTOR3 rot;//向き
	D3DXMATRIX mtxWorld;

}Rader;//レーダー

typedef struct
{
	D3DXVECTOR3 UserPos;//レーダーディスプレイを使用している者の位置
	D3DXVECTOR3 pos;//位置
	D3DXVECTOR3 rot;//向き
	int nType;//種類
	bool bUse;//使用しているかどうか
	float size;//大きさ
	float fAngle;//アングル
	float fLength;//長さ
	float fWidth;//横
	float fHeight;//縦
}RaderDisplay;//レーダーディスプレイ

//プロトタイプ宣言

//====================
//レーダー
//====================
void InitRader(void);
void UninitRader(void);
void UpdateRader(void);
void DrawRader(void);

//================================

//=========================
//レーダーディスプレイ
//=========================
void InitRaderDisplay(void);
void UninitRaderDisplay(void);
void UpdateRaderDisplay(void);
void DrawRaderDisplay(void);
int SetRaderDisplay(int nType);
void SetPositionRaderDisPlay(int nIdxRaderDisPlay, D3DXVECTOR3 pos,float fRotZ);
void KillRaderDisplay(int nIdxRaderDisplay);
//============================================================================


//========================
//総合
//========================
void InitRaderCom(void);
void UninitRaderCom(void);
void UpdateRaderCom(void);
void DrawRaderCom(void);
//======================================

#endif
