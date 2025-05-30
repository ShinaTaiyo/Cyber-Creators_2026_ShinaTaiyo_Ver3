//========================================================
//
//プレイヤーの処理に関するヘッダーファイル
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"
#include "score.h"
#include "scrap.h"
//=======================================================================================================================================================================================================================
// マクロ定義
//=======================================================================================================================================================================================================================
#define MAX_PLAYERPARTS		(19)//プレイヤーパーツ数(武器を除く)
#define MAX_PLAYERPARTS2	(MAX_PLAYERPARTS + 1)//プレイヤーパーツ数(武器を含む) （プレイヤーのパーツ数(18) + 武器３つ(パーツ数 + 1) * MAX_WEAPON)
#define MAX_PLAYER_WEAPON	(3)//武器の種類
#define MAX_MOTION			(7)	//モーション数
#define MAX_KEY				(30)//モーションのキーの最大数
#define MAX_PLAYERMAT		(100)//各モデルのマテリアル数の最大数
#define MAX_LEVEL			(20)//レベルの最大値
#define FIRST_PLAYERSPEED	(12.0f)//プレイヤーの初速度
#define PLAYER_TXT			"data\\SetPlayer.txt"//プレイヤーのパーツ情報
#define WEAPON_TXT			"data\\SetWeapon.txt"//プレイヤーのパーツ情報
#define LEVEL_TXT			"data\\TEXTFILE\\Level.txt"
#define LEVELEDIT_TXT		"data\\TEXTFILE\\LevelEdit.txt"
#define PLAYERPARTS_MAT		(100)//マテリアルの最大数
#define COLLISION_WALL		(8.0f)//壁との当たり判定調整用

//=======================================================================================================================================================================================================================
// 状態
//=======================================================================================================================================================================================================================

//=================================================================================================
//プレイヤーの攻撃の番号（それぞれの攻撃にヒットストップの判定を付けたいので書いた。）
//=================================================================================================
typedef enum
{
	PLAYERATTACK00_SWORD = 0,//剣の攻撃の番号
	PLAYERATTACK01_BOOMERANG,//ブーメラン攻撃の番号
	PLAYERATTACK02_SPEAKER,  //スピーカー攻撃の番号
	PLAYERATTACK_MAX
}PLAYERATTACK;

typedef enum
{
	PLAYERSTATE_NUTORAL = 0,//ニュートラル
	PLAYERSTATE_MOVE,		//移動
	PLAYERSTATE_ATTACK,		//攻撃
	PLAYERSTATE_DAMAGE,		//ダメージ中
	PLAYERSTATE_HEALWALL,	//壁回復
	PLAYERSTATE_HEALGROUND,	//床回復
	PLAYERSTATE_DODGE,		//回避
	PLAYERSTATE_MAX
}PLAYERSTATE;

typedef enum
{
	PLAYERMODE_NORMAL = 0,	//普通モード
	PLAYERMODE_EDIT,		//エディットモード
	PLAYERMODE_MAX
}PLAYERMODE;

//=======================================================================================================================================================================================================================
// 構造体
//=======================================================================================================================================================================================================================

//================================================================
// 各モデルの構造体
//================================================================

//================================================================
// 剣の構造体
//================================================================
typedef struct
{
	int nLevel;				//レベル
	int nSwordScrap;		//剣スクラップ獲得数
	int nTotalSwordScrap;   //剣スクラップ合計獲得数
	int nMaxLevelUpPoint;	//次のレベルまでに必要なスクラップ数
	int nSwordPower;		//剣の攻撃力
	int nSwordAttackSpeed;	//剣の攻撃速度
}Sword;

//================================================================
// ブーメランの構造体
//================================================================
typedef struct
{
	int nLevel;				  //レベル
	int nBoomerangScrap;	  //ブーメランスクラップ獲得数
	int nTotalBoomerangScrap; //ブーメランスクラップ合計獲得数
	int nMaxLevelUpPoint;	  //次のレベルまでに必要なスクラップ数
	int nBoomerangPower;	  //ブーメランの攻撃力
	float fBoomerangRenge;	  //ブーメランの射程
	int nBoomerangPiece;	  //現在持っているブーメランの個数
	int nMaxBoomerangPiece;   //持てるブーメランの最大数
	bool bPossibleBoomerang;  //ブーメラン攻撃が可能かどうか　
	D3DXVECTOR3 BoomerangScale;//ブーメランの大きさ
}Boomerang;

//================================================================
// スピーカーの構造体
//================================================================
typedef struct
{
	int nLevel;				//レベル
	int nSpeakerScrap;		//スピーカースクラップ獲得数
	int nTotalSpeakerScrap; //スピーカースクラップ合計獲得数
	int nMaxLevelUpPoint;	//次のレベルまでに必要なスクラップ数
	float fSpeakerPower;	//スピーカーの攻撃力
	int nPowerConsumption;	//スピーカーの消費電力
}Speaker;

//================================
// プレイヤー武器
//================================
typedef struct
{
	D3DXVECTOR3 pos;//位置
	D3DXVECTOR3 rot;//向き
	int nType;//読み込むモデルの種類
	bool bUse;//使用しているかどうか
	D3DXVECTOR3 vtxMax;//最大頂点値
	D3DXVECTOR3 vtxMin;//最小頂点値
	D3DXVECTOR3 Size;//全体の大きさ
}Weapon;//パーツごとの動き

//=========================================
// キーの情報構造体
//=========================================
typedef struct
{
	D3DXVECTOR3 pos;//位置
	D3DXVECTOR3 rot;//向き
}PlayerKey;

//=========================================
// キーセットの情報構造体
//=========================================
typedef struct
{
	PlayerKey key[MAX_PLAYERPARTS2];
	int nMaxFrame;
}PlayerKeySet;

//=========================================
// モーションの情報構造体
//=========================================
typedef struct
{
	int nCntKey;//キー数カウンタ
	int nCntFrame;//フレーム数カウンタ
	int nMaxKey;//各モーションの最大キー数
	int nMotionLoop;//各モーションがループするかどうか
	PlayerKeySet keyset[MAX_PLAYERPARTS2];//パーツの情報
}PlayerMotionSet;

//================================
// プレイヤーパーツ
//================================
typedef struct
{
	D3DXVECTOR3 pos;//位置
	D3DXVECTOR3 rot;//向き
	D3DXVECTOR3 vtxMinModel;//位置の最小値
	D3DXVECTOR3 vtxMaxModel;//位置の最大値
	D3DCOLORVALUE DiffUse[MAX_PLAYERMAT];//色合い
	D3DCOLORVALUE FormarDiffUse[MAX_PLAYERMAT];//元の色合い

	BYTE* pVtxBuff;//頂点バッファへのポインタ
	DWORD sizeFVF;//頂点フォーマットのサイズ
	D3DXMATRIX mtxWorld;//ワールドマトリックス
	int nIdxModelParent;//親モデルのインデックス
	int nType;//種類
	bool bUse;//使用しているかどうか
	LPD3DXMESH pMesh;//メッシュ（頂点情報）へのポインタ
	LPD3DXBUFFER pBuffMat;//マテリアルへのポインタ
	DWORD dwNumMat;//マテリアルの数
	D3DXVECTOR3 PartsPos;//パーツの位置（ワールド座標）

}ModelParts;//パーツごとの動き

//================================================================
// プレイヤーの構造体
//================================================================
typedef struct
{
	D3DXVECTOR3 pos;//位置
	D3DXVECTOR3 posOld;//前の位置
	D3DXVECTOR3 rot;//向き
	D3DXVECTOR3 targetrot;//目的の向き
	D3DXVECTOR3 move;//移動量
	D3DXVECTOR3 vtxMax;//プレイヤー全体の最大頂点値
	D3DXVECTOR3 vtxMin;//プレイヤー全体の最小頂点値
	D3DXVECTOR3 size;//サイズ
	D3DXMATRIX mtxWorld;//ワールドマトリックス
	D3DCOLORVALUE DiffUse;//モデルの色
	bool bMotionFlag;//モーション発動フラグ
	int nMode;//モードの種類
	bool bUse;//使用しているかどうか
	int nIdxShadow;//影のインデックス
	int nIdxRaderDisplay;//レーダーディスプレイのインデックス
	float fWidth;//プレイヤーの幅
	float fHeight;//プレイヤーの高さ
	float fAutletLength;//コンセントとの距離
	bool bIsLanding;//地面にいるかどうか
	PLAYERSTATE State;//プレイヤーの状態を表す
	D3DXVECTOR3 Size;//プレイヤー全体の大きさ
	int nHp;//プレイヤーの体力
	Weapon aWeapon[MAX_PLAYER_WEAPON];//各武器の情報
	PlayerMotionSet aMotionSet[MAX_PLAYER_WEAPON][MAX_MOTION];//体のモーションの構造体の情報
	PlayerMotionSet aMotionSet2[MAX_PLAYER_WEAPON][MAX_MOTION];//脚のモーションの構造体の情報
	ModelParts aModel[MAX_PLAYERPARTS2][MAX_PLAYER_WEAPON];//各パーツの構造体の情報
	int nStateOld;//１f前の状態を保存する
	bool bMove;//移動状態判定
	bool bCompulsionAttack;//強制的に攻撃状態にする。
	int nAttackTime;//攻撃している時間を計測する
	bool bInvincible;//無敵状態にする
	int nInvincibleCnt;//無敵状態にする時間
	int nNumLandingModel;//どのモデルの上に乗っているか
	float fMoveX;//X方向の移動量
	float fMoveZ;//Z方向の移動量
	float fCombo;//コンボ倍率
	int nCntCombo;//コンボ数

	//====================================
	//ベクトル変数
	//====================================
	float fVXaim;
	float fVZaim;
	float fVaim;

	//====================================
	//弱い攻撃を受けた時の変数
	//====================================
	bool bWeakDamageReaction;//弱い攻撃を受けた時にtrueにして色を変えたりする
	int nWeakDamageReactionTime;//弱い攻撃を受けたときに、色を変える時間をカウントする

	//========================
	//リザルト関係
	//========================
	int nTotalGetScrap;   //スクラップ合計獲得数
	int nTotalDefeatEnemy;//敵合計撃破数

	//=================================
	//バッテリー減少関連
	//=================================
	int nMoveFrame;//動いたフレーム数をカウントする
	int nAttackCount;//攻撃した時にカウントをする
	int nDamageCountTime;//ダメージを受けている時間をカウントする
	bool bDamageState;//ダメージを受けている状態かどうか

	//=================================
	//バッテリー充電関連
	//=================================

	//壁充電
	bool bCompulsionHealWall;//強制的に壁回復（充電）状態にする
	int nHealWallTime;//壁回復をしている時間を計測する
	D3DXVECTOR3 HealWallSaveRot;//壁回復中の向きを保存する
	bool bChargingFlag;//充電中にtrueにする
	int nNumUseWallOutlet;//使用している壁コンセントのモデル番号を記録する

	//床充電
	bool bCompulsionHealGround;//強制的に床回復（充電）状態にする
	int nHealGroundTime;//床回復をしている時間を計測する
	D3DXVECTOR3 HealGroundSaveRot;//壁回復中の向きを保存する
	bool bChargingGroundFlag;//充電中にtrueにする
	int nNumUseGroundOutlet;//使用している壁コンセントのモデル番号を記録する

	//=================================
	//回避関連
	//=================================
	bool bCompulsionDodge;//強制的に回避状態にする
	bool bPossibleDodge;  //回避可能かどうか
	int nDodgeTime;       //回避している時間
	int nDodgeCoolTime;   //次回避可能になるまでのクールタイム

	//=================================
	// ちびロボ本体のステータス
	//=================================
	int nLevel;					//レベル
	int nTibiroboScrap;			//ちびロボスクラップ獲得数
	int nTotalTibiroboScrap;    //ちびロボスクラップ合計獲得数
	int nMaxLevelUpPoint;		//次のレベルまでに必要なスクラップ数
	int nBattery;				//ちびロボのバッテリー（体力）
	int nAllUpScrap;			//全てのステータス強化スクラップ獲得数
	int nTotalAllUpScrap;       //全てのステータス強化スクラップ合計獲得数
	int nWeapon;				//所持している武器
	float fSpeed;				//ちびロボの速さ
	float fSuctionScrapRange;   //スクラップ吸収範囲
	Sword aSwordInfo;			//剣のステータス
	Boomerang aBoomerangInfo;	//ブーメランのステータス
	Speaker aSpeakerInfo;		//スピーカーのステータス
}Player;

//=======================================================================================================================================================================================================================
// 各パーツのXファイル
//=======================================================================================================================================================================================================================
static const char* PLAYER_FILENAME[MAX_PLAYERPARTS] =
{
	"data\\PLAYER\\00_waist000.x",	//腰[0]
	"data\\PLAYER\\01_body000.x",	//体[1]
	"data\\PLAYER\\02_hedD000.x",	//頭下[2]
	"data\\PLAYER\\03_hedU000.x",	//頭上[3]
	"data\\PLAYER\\04_urmUL000.x",	//左上腕[4]
	"data\\PLAYER\\05_urmDL000.x",	//左腕[5]
	"data\\PLAYER\\06_handL000.x",	//左手[6]
	"data\\PLAYER\\07_urmUR000.x",	//右上腕[7]
	"data\\PLAYER\\08_urmDR000.x",	//右腕[8]
	"data\\PLAYER\\09_handR000.x",	//右手[9]
	"data\\PLAYER\\10_legUL000.x",	//左腿[10]
	"data\\PLAYER\\11_legDL000.x",	//左足[11]
	"data\\PLAYER\\12_legUR000.x",	//右腿[12]
	"data\\PLAYER\\13_legDR000.x",	//右足[13]
	"data\\PLAYER\\14_code000.x",	//コード[14]
	"data\\PLAYER\\14_code000.x",	//コード[15]
	"data\\PLAYER\\14_code000.x",	//コード[16]
	"data\\PLAYER\\15_plug000.x",	//プラグ[17]
	"data\\PLAYER\\b.x",			//透明(本物)[18]
};

//=======================================================================================================================================================================================================================
// 各武器のXファイル
//=======================================================================================================================================================================================================================
static const char* WEAPON_FILENAME[MAX_PLAYER_WEAPON] =
{
	"data\\PLAYER\\sword\\sword000.x",			//剣
	"data\\PLAYER\\boomerang\\boomerang000.x",	//ブーメラン
	"data\\PLAYER\\speaker\\speaker000.x",		//スピーカー
};

//=======================================================================================================================================================================================================================
// 各武器のXファイル
//=======================================================================================================================================================================================================================
static const char* MOTION_FILENAME[MAX_PLAYER_WEAPON] =
{
	"data\\PlayerMotion000.txt",//[0]剣
	"data\\PlayerMotion001.txt",//[1]ブーメラン
	"data\\PlayerMotion002.txt",//[2]スピーカー
};

//=======================================================================================================================================================================================================================
// プロトタイプ宣言
//=======================================================================================================================================================================================================================
void InitPlayer(void);//初期化処理
void UninitPlayer(void);//終了処理
void UpdatePlayer(void);//更新処理
void DrawPlayer(void);//描画処理
Player* GetPlayer(void);//構造体の情報の取得

//============================================
//プレイヤーパーツ、モーション関係
//============================================
void SetPlayerParts(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nIdx);//プレイヤーパーツの設定処理
void SetPlayerWeapon(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nIdx, int nCnt);//プレイヤーパーツの設定処理
void LoadPlayer(void);//プレイヤーパーツ情報の読み込み処理
void LoadWeapon(void);//武器パーツ情報の読み込み処理
void LoadMotion(int nWeapon);//モーションの読み込み処理
void PlayerMotion(int nWeapon);//体のモーション処理
void PlayerMotion2(int nWeapon);//脚のモーション処理

//===================================
//ゲーム関係
//===================================
void SetDamagePlayer(int nDamage);
bool bPlayerDeath(void);//プレイヤーが死んだかどうかの判定を返す
void AddScrapPlayer(SCRAPTYPE ScrapType,int nScrap);//スクラップを回収したときに呼ぶ関数。
void LevelPlayer(void);//プレイヤーのレベルに関する処理を行う。
void StatusPlayer(void);//プレイヤーのステータスに関する処理。
void CalculateMatrixPlayer(void);//プレイヤーのマトリックスを計算する関数
void AttackHitPlayer(int nSubBattery);//プレイヤーに攻撃がヒットした時に呼ぶ関数
void ResetPlayerPos(void);//プレイヤーの位置リセット処理

#endif
