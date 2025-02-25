//========================================================
//
//８月３０日：エフェクトもう一回見直す[enemy.cpp]
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
	ENEMYTYPE_DELBY = 0,
	ENEMYTYPE_PUNDY,
	ENEMYTYPE_SMILY,
	ENEMYTYPE_SQUDY,
	ENEMYTYPE_REAFY,
	ENEMYTYPE_HONOBY,
	ENEMYTYPE_MIZUBY,
	ENEMYTYPE_BOMUBY,
	ENEMYTYPE_WINBY,
	ENEMYTYPE_BOLBY,
	ENEMYTYPE_BANDY,
	ENEMYTYPE_BOSS1_STARWARRIOR,
	ENEMYTYPE_MINIBOMUBY,
	ENEMYTYPE_MAX
}ENEMYTYPE;


//敵構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;//位置
	D3DXVECTOR3 move;//移動量
	D3DXVECTOR3 rot;//向き
	int nType;//種類
	ENEMYSTATE State;//状態
	int nCounterState;//状態管理カウンター
	float fLife;//体力
	bool bUse;//使用しているかどうか
	int nEnemyCount;//敵の最大数をカウントする変数
	bool bEnemyMoveX;//X方向のスクリーンセーバー
	bool bEnemyMoveY;//Y方向のスクリーンセーバー
	bool bSetEnemyUse;//敵をセットしたときにtrueになるフラグ
	bool bDefeatFlag;//敵をセットした後、敵を倒したらfalseになるフラグ
	float fEnemy5BulletCurve;//エネミー５の弾のカーブの値

	bool bBossSummonFlag;//ボス召喚フラグ
	float fBossMove;//ボスを一定時間ごとに動かすフラグ。
	bool bBossMoveFlag;//フラグがtrueの時にボスを動かすフラグ。

	int nCountMoveInterval;//敵が動くまでの時間を計測
	int nMoveTime;//動く時間を計測

	float fRotMove;//敵をランダムに動かす時の方向を決める変数

	bool bAttackInterval;//敵が攻撃するインターバルをつけるフラグ

	int nAttarkIntervalCount;//敵が攻撃するまでの時間を計測する。

	int nApperCount;//敵がセットされてから出現するまでの時間を計測する

	bool bEnemyCountminusFlag;//敵がやられてから総数を減らすカウントを一回だけ発動させるフラグ
	//bool bResultFlag;
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
void HitEnemy(int nCntEnemy, float fDamage);//「nCntEnemy」は、どの敵に？、「nDamage」は、どのくらいダメージを与える？
Enemy* GetEnemy(void);

Result* GetResult(void);

Defeat* GetDefeat(void);//ウェーブ数を管理する構造体をゲットする。
#endif
