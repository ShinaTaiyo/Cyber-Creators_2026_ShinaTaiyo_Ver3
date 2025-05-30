//================================================================================================================================================================================================================================
//
// ボスの処理に関するヘッダーファイル
// Author : Atsumu Kuboichi
//
//================================================================================================================================================================================================================================
#ifndef _BOSS_H_
#define _BOSS_H_

#include "main.h"
#include "player.h"
#include "attackmodel.h"

//================================================================================================================================================================================================================================
// マクロ定義
//================================================================================================================================================================================================================================
#define MAX_BOSSPARTS		(14)				//パーツの最大数
#define MAX_BOSSMOTION		(5)					//モーション数
#define MAX_BOSSKEYSET		(100)				//モーションのキーの最大数
#define MAX_BOSSMAT			(100)				//マテリアルの最大数
#define BOSSSET				"data\\BOSSSET.txt"	//ボスの情報txtファイル
#define NUM_BOSSATTACK		(10)                 //ボスの攻撃の種類
#define MAX_PURPOSEFLAG		(3)					//汎用フラグの最大数
#define BOSS_SAMMON_TIME	(120)				//ボス出現時間

//================================================================================================================================================================================================================================
// 構造体
//================================================================================================================================================================================================================================

//=========================================
// 状態
//=========================================
typedef enum
{
	BOSSSTATE_NEUTRAL,//ニュートラル
	BOSSSTATE_MOVE,//移動
	BOSSSTATE_ATTACK,//攻撃
	BOSSSTATE_MAX
}BOSSSTATE;

//=========================================
//攻撃の種類
//=========================================
typedef enum
{
	BOSSATTACK00_ATOMICPRESS = 0,  //攻撃０：アトミックプレス
	BOSSATTACK01_FLASHPUNCH,       //攻撃１：フラッシュパンチ
	BOSSATTACK02_THRUSTSTUB,       //攻撃２：スラストスタブ
	BOSSATTACK03_BIRDSTRIKE,       //攻撃３：バードストライク
	BOSSATTACK04_BOMBRAIN,         //攻撃４：ボムレイン
	BOSSATTACK05_QUELLTHEEARTH,    //攻撃５：クウェルジアース
	BOSSATTACK06_BURNINGCHICKEN,   //攻撃６：バーニングチキン
	BOSSATTACK07_SLANDERTORNADO,   //攻撃７：スレンダートルネード
	BOSSATTACK08_STARDUSTMETEO,    //攻撃８：スターダストメテオ
	BOSSATTACK09_COLLAPSELASER,    //攻撃９：コラプスレーザー
	BOSSATTACK_MAX
}BOSSATTACK;

//=========================================
// キーの情報構造体
//=========================================
typedef struct
{
	D3DXVECTOR3 pos;//位置
	D3DXVECTOR3 rot;//向き
}BossKey;

//=========================================
// キーセットの情報構造体
//=========================================
typedef struct
{
	BossKey key[MAX_BOSSPARTS];
	int nMaxFrame;
}BossKeySet;

//=========================================
// モーションの情報構造体
//=========================================
typedef struct
{
	int nCntKey;//キー数カウンタ
	int nCntFrame;//フレーム数カウンタ
	int nMaxKey;//各モーションの最大キー数
	int nLoop;//各モーションがループするかどうか
	BossKeySet keyset[MAX_BOSSKEYSET];//パーツの情報
}BossMotionSet;

//=========================================
// 各パーツの情報構造体
//=========================================
typedef struct
{
	LPD3DXMESH pMesh;						//メッシュの頂点情報へのポインタ
	LPD3DXBUFFER pBuffMat;					//マテリアルへのポインタ
	DWORD dwNumMat;							//マテリアルの数
	D3DXMATRIX mtxWorld;					//ワールドマトリックス
	D3DCOLORVALUE Diffuse[MAX_BOSSMAT];		//色合い
	D3DXVECTOR3 pos;						//位置
	D3DXVECTOR3 rot;						//向き
	D3DXVECTOR3 vtxMin;//それぞれの座標の敵の位置の最小値
	D3DXVECTOR3 vtxMax;//それぞれの座標の敵の位置の最大値
	D3DXVECTOR3 PartsPos;//パーツごとのマトリックスの位置
	int nParent;							//親の番号
}BossParts;//パーツごとの動き

//=========================================
// 敵の情報構造体
//=========================================
typedef struct
{
	D3DXMATRIX mtxWorld;//ワールドマトリックス
	D3DXVECTOR3 pos;//位置
	D3DXVECTOR3 posOld;//1F前の位置
	D3DXVECTOR3 move;//移動量
	D3DXVECTOR3 rot;//向き
	D3DXVECTOR3 vtxMin;//それぞれの座標の敵の位置の最小値
	D3DXVECTOR3 vtxMax;//それぞれの座標の敵の位置の最大値
	D3DXVECTOR3 Size;//敵の大きさ
	BOSSSTATE state;//状態
	int nIdxShadow;//影のインデックス
	int nIdxRaderDisplay;//レーダーディスプレイのインデックス
	int nNumParts;//パーツ数
	int nHp;//体力
	int nMaxHp;//体力最大値
	int nAttack;//攻撃力
	int nDefeatScore;//倒したときにもらえるスコア
	int nAttackTiming;//攻撃判定をするタイミングを測る
	int nNumLandingModel;//どのモデルの上に乗っているか
	int nPattern;//パターン
	int nCntAppearTime;//召喚されてからの時間
	int nRunAwayLottery;//ボスを低確率で逃げさせる変数
	int nCntSummonEnemy;//ボスが召喚した敵の数をカウントする
	int nTotalDamage;//ダメージ合計量
	int nDamageCnt;//ダメージ数をカウントする
	int nCntUseAttack;//攻撃をした回数をカウントする

	float fRotMove;//一部の処理で、この変数の値の方向に移動させる
	float fSpeed;//速さ
	float fVXaim;//X方向のベクトル
	float fVYaim;//Y方向のベクトル
	float fVZaim;//Z方向のベクトル
	float fVaim;//総合したベクトル
	float fVaimY1;//総合したY方向のベクトル
	float fLength;//距離
	float fMoveStartLength;//移動を開始する距離
	float fAttackStartLength;//攻撃を開始する距離
	bool bHitFlag;//攻撃がヒットしたかどうかを決める変数
	bool bUnderAttack;//攻撃中かどうかを判定する変数
	bool bUse;//使用しているかどうか
	bool bIsLanding;//地面にいるかどうか
	bool bBossSummon;//ボス召喚フラグ
	bool bBossAppear;//ボスが出現演出中かどうか
	bool bCurveLaser;//レーザーをカーブさせる向きを決める
	bool bRunAwayBoss;//フラグ発動時にボスを逃げさせる

	BossMotionSet aMotionset[MAX_BOSSMOTION];//各モーション情報
	BossParts aParts[MAX_BOSSPARTS];//各パーツ情報

	//=========================
	//攻撃の受け付け調整
	//=========================
	bool bHitStop[PLAYERATTACK_MAX];//この変数がtrueの間、攻撃を受け付けない（プレイヤーの攻撃の種類ごとに分ける。)
	int nCoolTime[PLAYERATTACK_MAX];//bHitStopをfalseにするまでのカウント（プレイヤーの攻撃の種類ごとに分ける。)
	bool bHitStopBoomerang[MAX_ATTACKMODEL];//ブーメランに対するヒットストップの判定をする
	int nCoolTimeBoomerang[MAX_ATTACKMODEL];//ブーメランに対するヒットストップのカウントをする
	
	//===============================
	//ボス攻撃処理用
	//===============================
	int nCntAttackTime;//攻撃をしている時間
	int nLotteryAttack;//攻撃を抽選する
	int nAttackCoolTime;//攻撃のクールタイム
	float fSaveAim;//ベクトルを保存する
	
	//======================================
	//３次元自機狙い弾の実装に使う変数
	//======================================
	float fVXaim3D;
	float fVYaim3D;
	float fVZaim3D;
	float fVLaim3D;

	D3DXVECTOR3 SaveMove3D;//移動量の保存をする　
}Boss;

typedef struct
{
	bool bUse;       //使用しているかどうか
	D3DXVECTOR3 pos; //位置
	D3DXVECTOR3 move;//移動量
	D3DXVECTOR3 rot; //向き
	float fAnimationPattern;//アニメーションパターン
	float fAnimationCnt;//アニメーションカウント
}GraspBoss;

//=========================================
// 各パーツXファイル
//=========================================
static const char* BOSSPARTS_FILENAME[MAX_BOSSPARTS] =
{
	"data\\ENEMY\\karaagekun\\00_body.x",
	"data\\ENEMY\\karaagekun\\01_head.x",
	"data\\ENEMY\\karaagekun\\04_armL.x",
	"data\\ENEMY\\karaagekun\\03_handR.x",
	"data\\ENEMY\\karaagekun\\02_armR.x",
	"data\\ENEMY\\karaagekun\\05_handL.x",
	"data\\ENEMY\\karaagekun\\06_footR.x",
	"data\\ENEMY\\karaagekun\\07_legR.x",
	"data\\ENEMY\\karaagekun\\08_footL.x",
	"data\\ENEMY\\karaagekun\\09_legL.x",
	"data\\ENEMY\\karaagekun\\10_wingR.x",
	"data\\ENEMY\\karaagekun\\11_wingL.x",
	"data\\ENEMY\\karaagekun\\12_weapon.x",
	"data\\ENEMY\\karaagekun\\13_shield.x",
};

//================================================================================================================================================================================================================================
// プロトタイプ宣言
//================================================================================================================================================================================================================================
void InitBoss(void);		            //初期化処理
void InitGraspBoss(void);               //ボス位置把握表示の初期化処理
void UninitBoss(void);		            //終了処理
void UninitGraspBoss(void);             //ボス位置把握表示の終了処理
void UpdateBoss(void);		            //更新処理
void UpdateGraspBoss(void);             //ボスの位置把握表示の更新処理
void DrawBoss(void);		            //描画処理
void DrawGraspBoss(void);               //ボスの位置把握表示の更新処理
void BossMotion(void);		            //モーション処理
void LoadBossParts(void);	            //パーツ情報読み込み処理
void LoadBossMotion(void);	            //モーション情報読み込み処理
void SetBoss(D3DXVECTOR3 pos, int nHp);	//ボスを召喚する
void SetGraspBoss(void);                //ボス位置把握表示の設定
void SetDamageBoss(int nDamage,PLAYERATTACK PlayerAttackNum,int nNumAttackModel);//ボスにダメージを与える
void HitStopBoss(void);                 //ボスへの攻撃のヒットストップ処理
void CaluculateMatrixBoss(void);        //ボスのマトリックスを計算する
Boss* GetBoss(void);                    //ボスの情報を取得する

//=============================
//ボスの攻撃処理関数
//=============================
void BossAttack0_AtomicPress(void);   //ボス攻撃０：アトミックプレス
void BossAttack1_FlashPunch(void);    //ボス攻撃１：フラッシュパンチ
void BossAttack2_ThrustStub(void);    //ボス攻撃２：スラストスタブ
void BossAttack3_BirdStrike(void);    //ボス攻撃３：バードストライク
void BossAttack4_BombRain(void);      //ボス攻撃４：ボムレイン
void BossAttack5_QuellTheEarth(void); //ボス攻撃５：クウェルジアース
void BossAttack6_BurningChicken(void);//ボス攻撃６：バーニングチキン
void BossAttack7_SlanderTornado(void);//ボス攻撃７：スレンダートルネード
void BossAttack8_StardustMeteo(void); //ボス攻撃８：スターダストメテオ
void BossAttack9_CollapseLaser(void); //ボス攻撃９：コラプスレーザー

void RunAwayBoss(void);               //ボスが逃げる処理

#endif
