//================================================================
//
// 敵の処理に関するヘッダーファイル
// Author : Shina Taiyo
//
//================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "main.h"
#include "player.h"
#include "attackmodel.h"

//================================================================
// マクロ定義
//================================================================
#define MAX_ENEMYPARTS			(20)//プレイヤーのパーツの最大数
#define MAX_ENEMYMOTION			(5)	//モーション数
#define MAX_ENEMYKEY			(100)//モーションのキーの最大数
#define NUM_ENEMY				(5)//種類
#define MAX_ENEMY				(50)//最大数
#define ENEMY_SCALE				(10.0f)
#define MAX_ENEMYTEXTURE		(32)
#define MAX_DELAY				(50)//全体的に攻撃速度を下げる
#define MAX_ENEMYMAT			(100)
#define ENEMY000SET				"data\\ENEMY000SET.txt"//レッドブル
#define ENEMY001SET				"data\\ENEMY001SET.txt"//ミルタンク
#define ENEMY002SET				"data\\ENEMY002SET.txt"//ミックチュジューチュ
#define ENEMY003SET				"data\\ENEMY003SET.txt"//モンスター
#define ENEMY004SET				"data\\ENEMY004SET.txt"//カップ麵
#define ENEMY004_NUM_ATTACK2	(6)//中ボスのサンダーバリアの出現数

//================================================================
// 構造体
//================================================================

//=========================================
// 敵の状態
//=========================================
typedef enum
{
	ENEMYSTATE_NEUTRAL,//ニュートラル
	ENEMYSTATE_MOVE,//移動
	ENEMYSTATE_ATTACK,//攻撃
	ENEMYSTATE_MAX
}ENEMYSTATE;

//=========================================
// キーの情報構造体
//=========================================
typedef struct
{
	D3DXVECTOR3 pos;//位置
	D3DXVECTOR3 rot;//向き
}EnemyKey;

//=========================================
// キーセットの情報構造体
//=========================================
typedef struct
{
	EnemyKey key[MAX_ENEMYPARTS];
	int nMaxFrame;
}EnemyKeySet;

//=========================================
// モーションの情報構造体
//=========================================
typedef struct
{
	int nCntEnemyKey;//キー数カウンタ
	int nCntFrame;//フレーム数カウンタ
	int nMaxEnemyKey;//各モーションの最大キー数
	int nMotionLoop;//各モーションがループするかどうか
	EnemyKeySet keyset[MAX_ENEMYKEY];//パーツの情報
}EnemyMotionSet;

//=========================================
// 各パーツの情報構造体
//=========================================
typedef struct
{
	D3DXVECTOR3 pos;//位置
	D3DXVECTOR3 rot;//向き
	D3DXVECTOR3 PartsPos;//現在のパーツごとの位置
	int nType;
	D3DXMATRIX mtxWorld;//ワールドマトリックス
	int nIdxModelParent;//親モデルのインデックス
	bool bUse;//使用しているかどうか
	LPD3DXMESH pMesh;//メッシュ（頂点情報）へのポインタ
	LPD3DXBUFFER pBuffMat;//マテリアルへのポインタ
	DWORD dwNumMat;//マテリアルの数
	LPDIRECT3DTEXTURE9 pTexture[MAX_ENEMYMAT];//テクスチャへのポインタ
	D3DCOLORVALUE Diffuse[MAX_ENEMYMAT];//色合い
	D3DCOLORVALUE FormarDiffuse[MAX_ENEMYMAT];//モデルの色


}EnemyParts;//パーツごとの動き

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
	int nType;//種類
	int nIdxShadow;//影のインデックス
	int nIdxRaderDisplay;//レーダーディスプレイのインデックス
	bool bUse;//使用しているかどうか
	bool bIsLanding;//地面にいるかどうか
	int nHp;//体力
	int nMaxHp;//体力最大値
	int nAttack;//攻撃力
	int nDefeatScore;//倒したときにもらえるスコア
	int nAttackTiming;//攻撃判定をするタイミングを測る
	int nNumLandingModel;//どのモデルの上に乗っているか
	float fSpeed;//速さ
	float fFormarSpeed;//元の速さ
	float fVXaim;//X方向のベクトル
	float fVYaim;//Y方向のベクトル
	float fVZaim;//Z方向のベクトル
	float fVaim;//総合したベクトル
	float fVaimY1;//総合したY方向のベクトル
	float fPenduram;//振り子のように弾を発射させる変数
	bool bPenduramFlag;//振り子の向きを変える
	bool bHitFlag;//攻撃がヒットしたかどうかを決める変数4
	bool bUnderAttack;//攻撃中かどうかを判定する変数
	bool bDefeatDropScrap;//倒された時にスクラップをドロップするかどうか
	bool bSummonByBoss;//ボスによって召喚されたかどうか
	float fLength;//距離
	D3DXVECTOR3 Size;//敵の大きさ
	int nCntEnemyFrame;//モージョンフレーム数カウンタ
	ENEMYSTATE state;//状態
	EnemyMotionSet motionset[NUM_ENEMY][MAX_ENEMYMOTION];//各モーション情報
	EnemyParts parts[NUM_ENEMY][MAX_ENEMYPARTS];//各パーツ情報
	float fMoveStartLength;//移動を開始する距離
	float fAttackStartLength;//攻撃を開始する距離

	//==============================
	//敵の表示オプションの変数
	//==============================
	bool bUseEnemyDisplay;//敵のオプションの表示をするかどうか
	float fOptionDispLength;//敵オプションの表示をスタートする距離

    //==================================
	//ダメージ状態処理
	//==================================
	bool bDamageState;//ダメージ状態	
	int nDamageStateCount;//ダメージ状態になってからのフレームをカウントする　
	bool bDamageColor;//ダメージ状態の色にするかどうかのフラグ
	
	//==================================
	//状態の強制
	//==================================
	bool bCompulsionAttack;//強制的に攻撃状態にする
	bool bCompulsionNeutoral;//強制敵にニュートラル状態にする

	//=========================
	//攻撃の受け付け調整
    //=========================
	bool bHitStop[PLAYERATTACK_MAX];//この変数がtrueの間、攻撃を受け付けない（プレイヤーの攻撃の種類ごとに分ける。)(剣とスピーカー）
	int nCoolTime[PLAYERATTACK_MAX];//bHitStopをfalseにするまでのカウント（プレイヤーの攻撃の種類ごとに分ける。（剣とスピーカー）
	bool bHitStopBoomerang[MAX_ATTACKMODEL];//ブーメランのアタックモデルに対するヒットストップ判定をする
	int nCoolTimeBoomerang[MAX_ATTACKMODEL];//ブーメランのアタックモデルに対するヒットストップ時間を判定する。
	bool bWeakWeapon[PLAYERATTACK_MAX];//どの武器に弱いかを決める。
	//=========================
	//中ボス用変数
	//=========================
	int nLotteryAttack;//どの攻撃をするか抽選を行う。

	//======================================
	//３次元自機狙い弾の実装に使う変数
	//======================================
	float fVXaim3D;
	float fVYaim3D;
	float fVZaim3D;
	float fVLaim3D;

	D3DXVECTOR3 SaveMove3D;//移動量の保存をする　
}Enemy;

//=========================================
// 各種類の体力
//=========================================
static int ENEMY_HP[NUM_ENEMY] =
{
	45,
	45,
	45,
	45,
	120,
};

//=========================================
// 各種類の攻撃力
//=========================================
static int ENEMY_ATTACK[NUM_ENEMY] =
{
	5,
	30,
	20,
	60,
	70,
};

//=========================================
// 各種類の獲得スコア
//=========================================
static int ENEMY_SCORE[NUM_ENEMY] =
{
	500,
	500,
	500,
	500,
	1000,
};

//=========================================
// 各種類のスピード
//=========================================
static float ENEMY_SPEED[NUM_ENEMY] =
{
	5.0f,
	2.5f,
	3.5f,
	4.5f,
	1.6f,
};

//=========================================
// 各種類のパーツ数
//=========================================
static int ENEMY_NUMPARTS[NUM_ENEMY] =
{
	15,
	15,
	14,
	9,
	13,
};

//=========================================
//移動を開始する距離
//=========================================
static float ENEMY_MOVESTART[NUM_ENEMY]
{
	150.0f,
	550.0f,
	550.0f,
	150.0f,
	250.0f,
};

//=========================================
//攻撃を開始する距離
//=========================================
static float ENEMY_ATTACKSTART[NUM_ENEMY]
{
	60.0f,
	550.0f,
	550.0f,
	60.0f,
	100.0f,
};

static float ENEMYDISPLENGTH[NUM_ENEMY]
{
	300.0f,
	600.0f,
	600.0f,
	300.0f,
	450.0f,
};

//=========================================
// 各敵情報ファイル
//=========================================
static const char* ENEMY_FILE[NUM_ENEMY] =
{
	ENEMY000SET,
	ENEMY001SET,
	ENEMY002SET,
	ENEMY003SET,
	ENEMY004SET,
};

//=========================================
// レッドブルの各パーツXファイル
//=========================================
static const char* ENEMY000_FILENAME[15] =
{
	"data\\ENEMY\\redbull\\body.x",
	"data\\ENEMY\\redbull\\armUR.x",
	"data\\ENEMY\\redbull\\armDR.x",
	"data\\ENEMY\\redbull\\handR.x",
	"data\\ENEMY\\redbull\\armUL.x",
	"data\\ENEMY\\redbull\\armDL.x",
	"data\\ENEMY\\redbull\\handL.x",
	"data\\ENEMY\\redbull\\legUR.x",
	"data\\ENEMY\\redbull\\legDR.x",
	"data\\ENEMY\\redbull\\footR.x",
	"data\\ENEMY\\redbull\\legUL.x",
	"data\\ENEMY\\redbull\\legDL.x",
	"data\\ENEMY\\redbull\\footL.x",
	"data\\ENEMY\\redbull\\wingR.x",
	"data\\ENEMY\\redbull\\wingL.x",
};

//=========================================
//ミルタンクの各パーツXファイル
//=========================================
static const char* ENEMY001_FILENAME[15] =
{
	"data\\ENEMY\\milltank\\00_box.x",
	"data\\ENEMY\\milltank\\01_hopper.x",
	"data\\ENEMY\\milltank\\02_drawer.x",
	"data\\ENEMY\\milltank\\03_handle.x",
	"data\\ENEMY\\milltank\\04_tireL.x",
	"data\\ENEMY\\milltank\\05_tireR.x",
	"data\\ENEMY\\milltank\\06_turret.x",
	"data\\ENEMY\\milltank\\07_body.x",
	"data\\ENEMY\\milltank\\08_hed.x",
	"data\\ENEMY\\milltank\\09_armUL.x",
	"data\\ENEMY\\milltank\\10_armUR.x",
	"data\\ENEMY\\milltank\\11_armDL.x",
	"data\\ENEMY\\milltank\\12_armDR.x",
	"data\\ENEMY\\milltank\\13_handL.x",
	"data\\ENEMY\\milltank\\14_handR.x",
};

//=========================================
// ミックチュジューチュの各パーツXファイル
//=========================================
static const char* ENEMY002_FILENAME[14] =
{
	"data\\ENEMY\\mixjuice\\00_BodyD.x",
	"data\\ENEMY\\mixjuice\\01_BodyU.x",
	"data\\ENEMY\\mixjuice\\02_WingR.x",
	"data\\ENEMY\\mixjuice\\03_WingL.x",
	"data\\ENEMY\\mixjuice\\04_ArmR.x",
	"data\\ENEMY\\mixjuice\\05_HandR.x",
	"data\\ENEMY\\mixjuice\\06_ArmL.x",
	"data\\ENEMY\\mixjuice\\07_HandL.x",
	"data\\ENEMY\\mixjuice\\08_ThighR.x",
	"data\\ENEMY\\mixjuice\\09_FootR.x",
	"data\\ENEMY\\mixjuice\\10_ThighL.x",
	"data\\ENEMY\\mixjuice\\11_FootL.x",
	"data\\ENEMY\\mixjuice\\12_Tale.x",
	"data\\ENEMY\\mixjuice\\13_Cannon.x",
};

//=========================================
// モンスターの各パーツXファイル
//=========================================
static const char* ENEMY003_FILENAME[9] =
{
	"data\\ENEMY\\monster\\00_body.x",
	"data\\ENEMY\\monster\\01_RightArm.x",
	"data\\ENEMY\\monster\\02_RightHand.x",
	"data\\ENEMY\\monster\\03_LeftArm.x",
	"data\\ENEMY\\monster\\04_LeftHand.x",
	"data\\ENEMY\\monster\\05_RightThigh.x",
	"data\\ENEMY\\monster\\06_RightFoot.x",
	"data\\ENEMY\\monster\\07_LeftThigh.x",
	"data\\ENEMY\\monster\\08_LeftFoot.x",
};

//=========================================
// カップメンの各パーツXファイル
//=========================================
static const char* ENEMY004_FILENAME[13] =
{
	"data\\ENEMY\\CupNoodle\\00_body000.x",
	"data\\ENEMY\\CupNoodle\\01_lid000.x",
	"data\\ENEMY\\CupNoodle\\02_eye000.x",
	"data\\ENEMY\\CupNoodle\\02_eye000.x",
	"data\\ENEMY\\CupNoodle\\b.x",
	"data\\ENEMY\\CupNoodle\\03_noodles000.x",
	"data\\ENEMY\\CupNoodle\\03_noodles000.x",
	"data\\ENEMY\\CupNoodle\\03_noodles000.x",
	"data\\ENEMY\\CupNoodle\\03_noodles000.x",
	"data\\ENEMY\\CupNoodle\\03_noodles000.x",
	"data\\ENEMY\\CupNoodle\\03_noodles000.x",
	"data\\ENEMY\\CupNoodle\\03_noodles000.x",
	"data\\ENEMY\\CupNoodle\\03_noodles000.x",
};

//=========================================
// プロトタイプ宣言
//=========================================
void InitEnemy(void);//初期化処理
void UninitEnemy(void);//終了処理
void UpdateEnemy(void);//更新処理
void DrawEnemy(void);//描画処理
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType,bool bSummonByBoss);//設定処理
void SetDamageEnemy(int nDamage, int nNumEnemy,PLAYERATTACK PlayerAttackNum,int nNumAttackModel);
Enemy* GetEnemy(void);//敵構造体の情報を取得する
void LoadEnemy(void);//敵情報の読み込み処理
void LoadEnemyMotion(int nType);//モーションの読み込み処理
void EnemyMotion(int nCnt);//モーション処理
void CalculateMatrixEnemy(void);//敵のマトリックスを計算する処理
void KillEnemy(void);//敵をキルする関数

#endif
