//========================================================
//
//８月３０日：エフェクトもう一回見直す[player.cpp]
//Author:ShinaTaiyo
//
//=========================================================

#ifndef _PLAYER_H_//このマクロ定義がされていなかったら
#define _PLAYER_H_//２重インクルード防止のマクロ定義

//プレイヤーの状態
typedef enum
{
	PLAYERSTATE_APPER = 0,//出現状態（点滅）
    PLAYERSTATE_NORMAL,//通常状態
	PLAYERSTATE_DAMAGE,//ダメージ状態
	PLAYERSTATE_DEATH,//死亡状態
	PLAYERSTATE_MAX
}PLAYERSTATE;

//プレイヤー構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;//位置
	D3DXVECTOR3 rot;//向き
	D3DXVECTOR3 move;//移動量
	PLAYERSTATE state;//プレイヤーの状態
	int nCounterState;//状態遷移カウンター
	bool bDisp;//プレイヤーの表示のON、OFFをする。
	bool bInvincibility;//プレイヤーを無敵状態にするかどうか
	int nHitCount;//プレイヤーが被弾した回数をカウントする変数
	int nSkillUseCount;//プレイヤーがスキルを使用した回数をカウントする変数
	bool OperationFlag;//操作を可能にするフラグ
}Player;

//プロトタイプ宣言
void InitPlayer(void);
void UninitPlayer(void);
void UpDatePlayer(void);
void DrawPlayer(void);
Player* GetPlayer(void);
void HitPlayer(int nDamage);
#endif
