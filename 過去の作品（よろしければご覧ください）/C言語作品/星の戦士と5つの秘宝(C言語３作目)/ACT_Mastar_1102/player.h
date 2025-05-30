//========================================================
//
//８月３０日：エフェクトもう一回見直す[player.cpp]
//Author:ShinaTaiyo
//
//=========================================================

#ifndef _PLAYER_H_//このマクロ定義がされていなかったら
#define _PLAYER_H_//２重インクルード防止のマクロ定義

#define PLAYER_WIDTH (18.0f)//テクスチャの横幅
#define PLAYER_HEIGHT (40.0f)//テクスチャの縦幅


//プレイヤーの状態
typedef enum
{
	PLAYERSTATE_APPER = 0,//出現状態（点滅）
    PLAYERSTATE_NORMAL,//通常状態
	PLAYERSTATE_DAMAGE,//ダメージ状態
	PLAYERSTATE_DEATH,//死亡状態
	PLAYERSTATE_MAX
}PLAYERSTATE;

//プレイヤーのモーション
typedef enum
{
	PLAYERMOTION_STAND = 0,//待機モーション
	PLAYERMOTION_MOVE,//移動モーション
	PLAYERMOTION_PUNCH,//パンチモーション
	PLAYERMOTION_MAX
}PLAYERMOTION;

typedef enum
{
	INERTIA_NORMAL = 0,//普通の慣性
	INERTIA_ICE,//氷ブロックの上にいるときの慣性の強さ
	INERTIA_MAX
}INERTIA;


//プレイヤー構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;//位置
	D3DXVECTOR3 PosBeside;//横の中心点（当たり判定）
	D3DXVECTOR3 posOld;//前回の位置
	D3DXVECTOR3 rot;//向き
	PLAYERSTATE state;//プレイヤーの状態
	D3DXVECTOR3 move;//移動量
	int nCounterAnim;//カウンター
	int nPatternAnim;//パターン番号
	int nDirectionMove;//向き
	int nCounterState;//状態遷移カウンター
	int nMotionType;//使用しているモーションの種類
	int nDashCount;//ダッシュ状態にするためのコマンドをうつまでの時間をカウントする
	int nMotionTypeOld;//前に使用しているモーションを保存する
	int nMotionTime;//モーションを使用している時間。モーションをいつ終了するかの判定で使う
	int nCntDashButtonL;//左方向にダッシュするためのボタンを押した回数をカウント
	int nCntDashButtonR;//右方向にダッシュするためのボタンを押した回数をカウント
	int nInertia;//慣性を強める
	bool bIsJumping;//ジャンプ中かどうか
	bool bDisp;//プレイヤーの表示のON、OFFをする。
	bool bInvincibility;//プレイヤーを無敵状態にするかどうか
	bool bUse;//使用しているかどうか
	bool bIsLanding;//地面の上にいるかどうか
	int nHitCount;//プレイヤーが被弾した回数をカウントする変数
	int nSkillUseCount;//プレイヤーがスキルを使用した回数をカウントする変数
	bool OperationFlag;//操作を可能にするフラグ
	float fGravity;//重力をジャンプ時間が長いほど増やす。
	float fWidth;//幅
	float fHeight;//高さ
	bool bIsWalling;//壁に当たっているというフラグ
	bool g_bTouchingBlock;//ブロックに触れていると発動するフラグ
	bool bDirection;//左右どちらを向いているかを、フラグで判定する
	bool bUseMotion;//モーションを使用しているかどうかのフラグ
	bool bDashL;//左方向へのダッシュ状態かどうかのフラグ
	bool bDashR;//右方向へのダッシュ状態かどうかのフラグ
	bool bAutoMoveR;//右方向へ自動的に移動するかどうかのフラグ
	bool bAutoMoveL;//左方向へ自動的に移動するかどうかのフラグ
}Player;

//プロトタイプ宣言
void InitPlayer(void);
void UninitPlayer(void);
void UpDatePlayer(void);
void DrawPlayer(void);
Player* GetPlayer(void);
void HitPlayer(int nDamage);
void SetPlayer(D3DXVECTOR3 pos);
#endif
