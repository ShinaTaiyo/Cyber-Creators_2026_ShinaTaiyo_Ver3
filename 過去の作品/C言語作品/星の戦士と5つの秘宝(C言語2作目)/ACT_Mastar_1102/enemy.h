//========================================================
//
//１０月１５日：新しい敵の追加[enemy.cpp]
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "main.h"

//マクロ定義
#define MAX_ENEMY (51)//敵の最大数
#define MAX_CIRCLEBULLET (20)//円形各散弾の弾数
#define MAX_CIRCLEBULLET_2 (10)//円形各散弾の弾数


//列挙型定義
//=======================================
//敵の状態
typedef enum
{
	ENEMYSTATE_NORMAL = 0,
	ENEMYSTATE_DAMAGE,
	ENEMYSTATE_MAX
}ENEMYSTATE;

//敵の種類
typedef enum
{
	ENEMYTYPE0_MINIPUNI = 0,
	ENEMYTYPE1_HONOPUNI,
	ENEMYTYPE2_SODOPUNI,
	ENEMYTYPE3_ICESLIME,
	ENEMYTYPE4_BILIBILISLIME,
	ENEMYTYPE5_SNIPERSLIME,
	ENEMYTYPE6_BOMUSLIME,
	ENEMYTYPE7_FLOWERSLIME,
	ENEMYTYPE8_CATSLIME,
	ENEMYTYPE9_STONESLIME,
	ENEMYTYPE10_WITCHSLIME,
	ENEMYTYPE_BANDY,
	ENEMYTYPE_BOSS1_STAND,
	ENEMYTYPE_BOSS1_RUN,
	ENEMYTYPE_BOSS1_ATTACK,
	ENEMYTYPE_BOSS2_STAND,
	ENEMYTYPE_BOSS2_RUN,
	ENEMYTYPE_BOSS2_ATTACK,
	ENEMYTYPE_BOSS3_STAND,
	ENEMYTYPE_BOSS3_RUN,
	ENEMYTYPE_BOSS3_ATTACK,
	ENEMYTYPE_BOSS4_STAND,
	ENEMYTYPE_BOSS4_RUN,
	ENEMYTYPE_BOSS4_ATTACK,
	ENEMYTYPE_FINALBOSS_STAND,
	ENEMYTYPE_FINALBOSS_RUN,
	ENEMYTYPE_FINALBOSS_ATTACK,
	ENEMYTYPE_MAX
}ENEMYTYPE;


//敵構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;//位置
	D3DXVECTOR3 posOld;;//1f前の位置
	D3DXVECTOR3 move;//移動量
	D3DXVECTOR3 rot;//向き
	ENEMYSTATE State;//状態
	int nType;//種類
	int nTypeOld;//1f前のタイプ
	int nCounterState;//状態管理カウンター
	int nEnemyCount;//敵の最大数をカウントする変数
	int nCountMoveInterval;//敵が動くまでの時間を計測
	int nMoveTime;//動く時間を計測
	int nAttarkIntervalCount;//敵が攻撃するまでの時間を計測する。
	int nApperCount;//敵がセットされてから出現するまでの時間を計測する
	int nAttackCount;//行動ごとの攻撃回数をカウントする
	int nCntAnim;//アニメーションカウンター
	int nPatternAnim;//アニメーションパターン
	int nCntJumpTime;//ジャンプする時間をカウントする
	float fLife;//体力
	float fMaxHp;//最大体力を保存する
	float fBossMove;//ボスを一定時間ごとに動かすフラグ。
	float fRotMove;//敵をランダムに動かす時の方向を決める変数
	float fEnemy5BulletCurve;//エネミー５の弾のカーブの値
	float fWidth;//敵の幅
	float fHeight;//敵の高さ
	float fGravity;//重力補正値
	float fXVaim;//自機狙いをするときの処理
	float fYVaim;//自機狙いをするときの処理
	float fVaim;//自機狙いをするときの処理
	bool bUse;//使用しているかどうか
	bool bIsLanding;//地面についているかどうかの処理
	bool bIsWalling;//壁に当たっているかどうかのフラグ
	bool bEnemyMoveX;//X方向のスクリーンセーバー
	bool bEnemyMoveY;//Y方向のスクリーンセーバー
	bool bSetEnemyUse;//敵をセットしたときにtrueになるフラグ
	bool bDefeatFlag;//敵をセットした後、敵を倒したらfalseになるフラグ
	bool bBossSummonFlag;//ボス召喚フラグ
	bool bBossMoveFlag;//フラグがtrueの時にボスを動かすフラグ。
	bool bAttackInterval;//敵が攻撃するインターバルをつけるフラグ
	bool bEnemyCountminusFlag;//敵がやられてから総数を減らすカウントを一回だけ発動させるフラグ
	bool bDirection;//左右どちらを向いているのか判定するフラグ
	bool bMoveWay;//動く方向を決めるフラグ
	bool bMovePattern;//その瞬間に動き方を２種類用意して決めるフラグ
	bool bSpecialAttack;//必殺技を使っているかどうかを決めるフラグ
	bool bSpecialStaging;//必殺技の演出をしたかどうかを判定するフラグ
	bool bGravity;//重力をかけるかどうかのフラグ
}Enemy;

typedef struct
{
	bool bResultFlag;
}Result;

typedef struct
{
	//bool bWaveFlag;//ウェーブ数のカウント
	int DefeatEnemyCount;//倒したエネミー数のカウント
}Defeat;

//プロトタイプ宣言
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
void SetEnemy(D3DXVECTOR3 pos, int ntype,float fLife,D3DXVECTOR3 move);//nTypeは敵の種類
void HitEnemy(int nCntEnemy, int nDamage);//「nCntEnemy」は、どの敵に？、「nDamage」は、どのくらいダメージを与える？
Enemy* GetEnemy(void);

Result* GetResult(void);

Defeat* GetDefeat(void);//ウェーブ数を管理する構造体をゲットする。

int DefeatEnemy(void);//倒した敵の数
#endif
