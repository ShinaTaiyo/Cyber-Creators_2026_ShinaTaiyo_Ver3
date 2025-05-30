//========================================================
//
//ミッションの処理に関するヘッダーファイル
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _MISSION_H_
#define _MISSION_H_

#include "main.h"

//=========================================================
//マクロ定義
//=========================================================
#define MAX_MISSION				(5)		//ミッション表示の最大数
#define MISSION_POS_Y			(180.0f)//ミッション表示のY座標
#define MISSION_DISPLAY_TIME	(5)		//ミッション表示時間(秒)
#define MISSION_MOVE			(15.0f)	//表示中移動量
#define NUM_CANDY				(5)		//飴の数
#define NEED_KILLENEMY			(150)	//必要な敵撃破数	

//=========================================================
//各ミッション
//=========================================================
typedef enum
{
	MISSION00_COLLECTCANDY = 0,//飴を集めろ
	MISSION01_DEFEATENEMY30,   //敵を３０体倒せ
	MISSION02_DONTFALLBELOW70, //体力５０％を下回るな
	MISSION03_BOSSDEFEAT70SEC, //ボスを１００秒以内に倒せ
	MISSION04_HEALBIND_3,      //回復３回以内縛り
	MISSION_MENU_MAX
}MISSION_MENU;

//=========================================================
//ミッション0構造体
//=========================================================
typedef struct
{
	bool bMissionClear;	//ミッションをクリアしたかどうか
	bool bMissionUISet;	//ミッションUIが表示済みかどうか
	bool bMissionReward;//ミッションの報酬を受け取ったかどうか
	int nGetCandyCount;	//キャンディ獲得量
}Mission0;

//=========================================================
//ミッション1構造体
//=========================================================
typedef struct
{
	bool bMissionClear;		//ミッションをクリアしたかどうか
	bool bMissionUISet;	//ミッションUIが表示済みかどうか
	bool bMissionReward;	//ミッションの報酬を受け取ったかどうか
	int nDefeatEnemyCount;	//敵を倒した量
}Mission1;

//=========================================================
//ミッション2構造体
//=========================================================
typedef struct
{
	bool bMissionClear;	//ミッションをクリアしたかどうか
	bool bMissionUISet;	//ミッションUIが表示済みかどうか
	int nKeepHp;		//プレイヤーの現在の体力を保存する
	bool bTimeUpFlag;	//時間になったら、一回だけ処理を行う
}Mission2;

//=========================================================
//ミッション3構造体
//=========================================================
typedef struct
{
	bool bMissionClear;			//ミッションをクリアしたかどうか
	bool bMissionUISet;	//ミッションUIが表示済みかどうか
	bool bMissionReward;		//ミッションの報酬を受け取ったかどうか
	int nAfterBossApperCntTime;	//ボスが出現してからの時間
}Mission3;

//=========================================================
//ミッション4構造体
//=========================================================
typedef struct
{
	bool bMissionClear;	//ミッションをクリアしたかどうか
	bool bMissionUISet;	//ミッションUIが表示済みかどうか
	int nHealCount;		//回復した回数をカウントする
	int nHealTime;		//回復をしている時間をカウントする
}Mission4;

//=========================================================
//ミッション全体の構造体
//=========================================================
typedef struct
{
	Mission0 aMission0;//飴を集めろ
	Mission1 aMission1;//敵を３０体倒せ
	Mission2 aMission2;//体力５０％を下回るな
	Mission3 aMission3;//ボスを１００秒以内に倒せ
	Mission4 aMission4;//回復３回以内縛り
}Mission;

//=========================================================
//ミッションクリア表示の構造体
//=========================================================
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	int nDisplayTime;	//表示時間
	int nType;			//種類
	float fWidth;		//横幅
	float fHeight;		//高さ
	bool bUse;			//使用しているかどうか
}MissionUI;

//=========================================================
//各種類のテクスチャファイル
//=========================================================
static const char* MISSION_TEXTURE[MAX_MISSION] =
{
	"data\\TEXTURE\\Mission000.png",//飴を集めろ	
	"data\\TEXTURE\\Mission001.png",//敵を３０体倒せ	
	"data\\TEXTURE\\Mission002.png",//体力５０％を下回るな	
	"data\\TEXTURE\\Mission003.png",//ボスを１００秒以内に倒せ	
	"data\\TEXTURE\\Mission004.png",//回復３回以内縛り	
};

//=========================================================
//プロトタイプ宣言
//=========================================================
void InitMission(void);		//初期化処理
void UninitMission(void);	//終了処理
void UpdateMission(void);	//更新処理
void DrawMission(void);		//描画処理
Mission* GetMission(void);	//情報構造体の取得
void SetMission(int nType, float fWidth, float fHeight);//設定処理
void JudgeClear(void);		//ミッションのクリア判定処理

#endif
