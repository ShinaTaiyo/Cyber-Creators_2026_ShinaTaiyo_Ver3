////========================================================
////
////８月３０日：エフェクトもう一回見直す[BOSS.cpp]
////Author:ShinaTaiyo
////
////=========================================================
//#ifndef _BOSS_H_
//#define _BOSS_H_
//
//#include "main.h"
//
////マクロ定義
//#define MAX_BOSS (50)//敵の最大数
//#define MAX_CIRCLEBULLET (20)//円形各散弾の弾数
//#define MAX_CIRCLEBULLET_2 (10)//円形各散弾の弾数
//
//
////列挙型定義
////=======================================
////敵の状態
//typedef enum
//{
//	BOSSSTATE_NORMAL = 0,
//	BOSSSTATE_DAMAGE,
//	BOSSSTATE_MAX
//}BOSSSTATE;
//
////敵の種類
//typedef enum
//{
//	BOSSTYPE_DELBY = 0,
//	BOSSTYPE_PUNDY,
//	BOSSTYPE_SMILY,
//	BOSSTYPE_SQUDY,
//	BOSSTYPE_REAFY,
//	BOSSTYPE_HONOBY,
//	BOSSTYPE_MIZUBY,
//	BOSSTYPE_BOMUBY,
//	BOSSTYPE_WINBY,
//	BOSSTYPE_BOLBY,
//	BOSSTYPE_BANDY,
//	BOSSTYPE_MAX
//}BOSSTYPE;
//
//
////敵構造体の定義
//typedef struct//========================================================
////
////８月３０日：エフェクトもう一回見直す[enemy.cpp]
////Author:ShinaTaiyo
////
////=========================================================
//#ifndef _ENEMY_H_
//#define _ENEMY_H_
//
//#include "main.h"
//
////マクロ定義
//#define MAX_ENEMY (50)//敵の最大数
//#define MAX_CIRCLEBULLET (20)//円形各散弾の弾数
//#define MAX_CIRCLEBULLET_2 (10)//円形各散弾の弾数
//
//
////列挙型定義
////=======================================
////敵の状態
//typedef enum
//{
//	ENEMYSTATE_NORMAL = 0,
//	ENEMYSTATE_DAMAGE,
//	ENEMYSTATE_MAX
//}ENEMYSTATE;
//
////敵の種類
//typedef enum
//{
//	ENEMYTYPE_DELBY = 0,
//	ENEMYTYPE_PUNDY,
//	ENEMYTYPE_SMILY,
//	ENEMYTYPE_SQUDY,
//	ENEMYTYPE_REAFY,
//	ENEMYTYPE_HONOBY,
//	ENEMYTYPE_MIZUBY,
//	ENEMYTYPE_BOMUBY,
//	ENEMYTYPE_WINBY,
//	ENEMYTYPE_BOLBY,
//	ENEMYTYPE_BANDY,
//	ENEMYTYPE_MAX
//}ENEMYTYPE;
//
//
////敵構造体の定義
//typedef struct
//{
//	D3DXVECTOR3 pos;//位置
//	D3DXVECTOR3 move;//移動量
//	D3DXVECTOR3 rot;//向き
//	int nType;//種類
//	ENEMYSTATE State;//状態
//	int nCounterState;//状態管理カウンター
//	int nLife;//体力
//	bool bUse;//使用しているかどうか
//	int nEnemyCount;//敵の最大数をカウントする変数
//	bool bEnemyMoveX;//X方向のスクリーンセーバー
//	bool bEnemyMoveY;//Y方向のスクリーンセーバー
//	bool bSetEnemyUse;//敵をセットしたときにtrueになるフラグ
//	bool bDefeatFlag;//敵をセットした後、敵を倒したらfalseになるフラグ
//	float fEnemy5BulletCurve;//エネミー５の弾のカーブの値
//	//bool bResultFlag;
//}Enemy;
//
//typedef struct
//{
//	bool bResultFlag;
//}Result;
//
//typedef struct
//{
//	//bool bWaveFlag;//ウェーブ数のカウント
//	int DefeatEnemyCount;//倒したエネミー数のカウント
//}Wave;
//
////プロトタイプ宣言
//void InitEnemy(void);
//void UninitEnemy(void);
//void UpdateEnemy(void);
//void DrawEnemy(void);
//void SetEnemy(D3DXVECTOR3 pos, int ntype,int nLife,D3DXVECTOR3 move);//nTypeは敵の種類
//void HitEnemy(int nCntEnemy, int nDamage);//「nCntEnemy」は、どの敵に？、「nDamage」は、どのくらいダメージを与える？
//Enemy* GetEnemy(void);
//
//Result* GetResult(void);
//
//Wave* GetWave(void);//ウェーブ数を管理する構造体をゲットする。
//#endif
//
//{
//	D3DXVECTOR3 pos;//位置
//	D3DXVECTOR3 move;//移動量
//	D3DXVECTOR3 rot;//向き
//	int nType;//種類
//	BOSSSTATE State;//状態
//	int nCounterState;//状態管理カウンター
//	int nLife;//体力
//	bool bUse;//使用しているかどうか
//	int nBOSSCount;//敵の最大数をカウントする変数
//	bool bBOSSMoveX;//X方向のスクリーンセーバー
//	bool bBOSSMoveY;//Y方向のスクリーンセーバー
//	bool bSetBOSSUse;//敵をセットしたときにtrueになるフラグ
//	bool bDefeatFlag;//敵をセットした後、敵を倒したらfalseになるフラグ
//	float fBOSS5BulletCurve;//エネミー５の弾のカーブの値
//	//bool bResultFlag;
//}BOSS;
//
//typedef struct
//{
//	bool bResultFlag;
//}Result;
//
//typedef struct
//{
//	//bool bWaveFlag;//ウェーブ数のカウント
//	int DefeatBOSSCount;//倒したエネミー数のカウント
//}Wave;
//
////プロトタイプ宣言
//void InitBOSS(void);
//void UninitBOSS(void);
//void UpdateBOSS(void);
//void DrawBOSS(void);
//void SetBOSS(D3DXVECTOR3 pos, int ntype, int nLife, D3DXVECTOR3 move);//nTypeは敵の種類
//void HitBOSS(int nCntBOSS, int nDamage);//「nCntBOSS」は、どの敵に？、「nDamage」は、どのくらいダメージを与える？
//BOSS* GetBOSS(void);
//
//Result* GetResult(void);
//
//Wave* GetWave(void);//ウェーブ数を管理する構造体をゲットする。
//#endif
//
