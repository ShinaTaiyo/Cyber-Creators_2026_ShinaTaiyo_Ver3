//====================================================
//
//７月１日：魔法攻撃を作る[attack.h]
//Author:ShinaTaiyo
//
//====================================================

#ifndef _ATTACK_H_  
#define _ATTACK_H_

//======================
//インクルード
//======================
#include "main.h"
#include "objectX.h"
#include "effect.h"
//==========================================

//==========================================

//==========================================
//攻撃クラス
//==========================================
class CAttack : public CObjectX
{
public:
	//===========================
	//攻撃の種類
	//===========================
	typedef enum
	{
		ATTACKTYPE00_FIRE = 0,      //攻撃０：ファイア
		ATTACKTYPE01_METEO,         //攻撃１：メテオ
		ATTACKTYPE02_NEEDLE,        //攻撃２：ニードル
		ATTACKTYPE03_TORNADO,       //攻撃３：トルネード
		ATTACKTYPE04_BEAM,          //攻撃４：ビーム
		ATTACKTYPE05_ICE,           //攻撃５：アイス
		ATTACKTYPE06_THUNDER,       //攻撃６：サンダー
		ATTACKTYPE07_MAGICSWORD,    //攻撃７：マジックソード
		ATTACKTYPE08_VACUUMBLADE,   //攻撃８：真空刃
		ATTACKTYPE09_HAMMER,        //攻撃９：ハンマー
		ATTACKTYPE10_SHOCKWAVE,     //攻撃１０：衝撃波
		ATTACKTYPE11_EXPLOSION,     //攻撃１１：爆発
		ATTACKTYPE12_VINE,          //攻撃１２：つる
		ATTACKTYPE13_CROSSBOMB,     //攻撃１３：クロスボム
		ATTACKTYPE14_HELLFIRE,      //攻撃１４：業火
		ATTACKTYPE15_BLACKHOLE,     //攻撃１５：ブラックホール
		ATTACKTYPE16_PHYCOBURST,    //攻撃１６：サイコバースト
		ATTACKTYPE17_BLOCK,         //攻撃１７：ブロック
		ATTACKTYPE18_THORNCLUB,     //攻撃１８：トゲこん棒
		ATTACKTYPE19_SENTERTHUNDER, //攻撃１９：中心雷
		ATTACKTYPE_MAX
	}ATTACKTYPE;
	//======================================================================

	//===========================
	//当てる敵の種類
	//===========================
	typedef enum
	{
		TARGETTYPE_ENEMY = 0,      //敵に当てる
		TARGETTYPE_PLAYER,         //プレイヤーに当てる
		TARGETTYPE_MAX
	}TARGETTYPE;
	//======================================================================

	//===========================
	//当たり判定の種類
	//===========================
	typedef enum
	{
		COLLISIONTYPE00_SQUARE = 0,//正方形の当たり判定
		COLLISIONTYPE01_RECTANGLEXY,//正方形の斜めを含めた当たり判定
		COLLISIONTYPE02_TRUERECTANGLEXY,//ちゃんとした正方形XYの当たり判定
		COLLISIONTYPE03_BALL,           //円の当たり判定を行う
		COLLISIONTYPE_MAX
	}COLLISIONTYPE;
	//======================================================================

	CAttack();                  //コンストラクタ
	~CAttack() override;        //デストラクタ
	HRESULT Init() override;    //初期化処理
	void Uninit() override;     //終了処理
	void Update() override;     //更新処理
	void Draw() override;       //描画処理
	void SetDeath() override;   //死亡フラグ
	static int GetNumFile() { return m_nNumFile; }//ファイルの数を取得する
	static CAttack* Create(ATTACKTYPE type, int nDamage, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 Scale, TARGETTYPE TargetType, D3DXVECTOR3 Rot,
		COLLISIONTYPE CollisionType, D3DXVECTOR3 AddRot, int nHitStopTime);     //攻撃を生成
	void SetPower(int nPower) { m_nPower = nPower; }         //威力を設定
	void SetType(ATTACKTYPE type) { m_type = type; }         //タイプを設定
	void SetTargetType(TARGETTYPE TargetType) { m_TargetType = TargetType; }//ターゲットタイプを設定

	void SetCollisionType(COLLISIONTYPE CollisionType) { m_CollisionType = CollisionType; }//判定タイプを設定
	void SetCollisionRelease(bool bCollisionRelease) { m_bCollisionRelease = bCollisionRelease; }       //当たったときに消すかどうかを設定する
	void SetDoCollision(bool bCollision) { m_bDoCollision = bCollision; }                              //当たり判定をするかどうかを設定する
	void SetHitStopTime(int nHitStopTime) { m_nSetHitStopTime = nHitStopTime; }                        //ヒットストップ時間を設定する

	void SetUseThinColor(bool bUse);                        //色を薄くしていくかどうか

	void SetUseGravity(float fGravityPower);                //重力を使用するかどうか

	void SetUseEffect(CEffect::EFFECTTYPE type, bool bUse, D3DXCOLOR col, int nLife, D3DXVECTOR2 Size) {
		m_SetEffectType = type; m_bUseSetEffect = bUse; m_SetEffectColor = col; m_nSetEffectLife = nLife; m_SetEffectSize = Size;
	} //エフェクトを使用するかどうか

	bool GetbUse() { return m_bUse; }                      //使用状態を取得
	void SetDelayDeath(bool bDelayDeath) { m_bDelaySetDeath = bDelayDeath; }//死亡フラグを遅らせるかどうかを設定
	bool GetDelayDeath() { return m_bDelaySetDeath; }

	void SetPurposePos(D3DXVECTOR3 PurposePos) { m_PurposePos = PurposePos; }//目的地を設定する

	//=================================================
	//反射関係
	//=================================================
	void SetUseReflection(bool bUse) {m_bUseReflection = bUse;}//反射させるかどうか
	D3DXVECTOR3& GetPurposePos() { return m_PurposePos; }//目的地を取得する
	//===================================================================================

	//=================================================
	//攻撃エフェクト関係
	//=================================================
	void SetUseAttackEffect(bool bUse, int nLife, D3DXCOLOR col) { m_bUseAttackEffect = bUse; m_nSetAttackEffectLife = nLife; m_SetAttackEffectCol = col; }
	//===================================================================================

	//=================================================
	//サブタイプ
	//=================================================
	int& GetSubType() { return m_nSubType; }
	void SetSubType(int nSubType) { m_nSubType = nSubType; }
	//===================================================================================

	//=================================================
	//タイプ
	//=================================================
	ATTACKTYPE GetAttackType() { return m_type; }
	void SetAttackType(ATTACKTYPE type) { m_type = type; }
	//===================================================================================
protected:
	int& GetCntTime() { return m_nCntTime; }                //時間を取得する
	//============================================
	//パターン関係
	//============================================
	int& GetPattern() { return m_nPattern; }                //攻撃パターンを取得
	int& GetPatternTime() { return m_nPatternTime; }        //攻撃パターン時間
	bool& GetPatternFlag() { return m_bPatternFlag; }       //パターンのフラグ
	//======================================================================================

	static const char* m_ATTACK_FILENAME[ATTACKTYPE_MAX];   //攻撃のファイルネーム
	int m_nAttackCnt;                                       //攻撃回数
private:
	ATTACKTYPE m_type;                                      //攻撃の種類
	D3DXVECTOR3 m_PurposePos;                               //目的地代入用                            
	int m_nCntTime;                                         //生成されてからの時間を計測する
	int m_nSetHitStopTime;                                  //当たったときに対象に設定するヒットストップ時間
	int m_nPower;                                           //威力を設定
	bool m_bCollisionRelease;                               //当たったときに消すかどうか
	bool m_bUse;                                            //使用状態
	TARGETTYPE m_TargetType;                                //当てるオブジェクトの種類
	COLLISIONTYPE m_CollisionType;                          //当たり判定の種類
	static int m_nNumFile;                                  //読み込んだファイルの数
	bool m_bDoCollision;                                    //当たり判定をするかどうか
	int m_nSubType;                                         //動きを決めるためのタイプ

	bool m_bUseGravity;                                     //重力を使用するかどうか
	float m_fGravityPower;                                  //重力の大きさ

	bool m_bUseThinColor;                                   //色を薄くしていくかどうか

	bool m_bDelaySetDeath;                                  //死亡グラグ発動を遅くするかどうか

	//======================================
	//反射関係
	//======================================
	bool m_bUseReflection;
	//===================================================================================

	//======================================
	//エフェクト関係
	//======================================
	bool m_bUseSetEffect;                                   //エフェクトを使うかどうか
	int m_nSetEffectLife;                                   //設定するエフェクトの体力
	CEffect::EFFECTTYPE m_SetEffectType;                    //設定するエフェクトの種類
	D3DXCOLOR m_SetEffectColor;                             //設定するエフェクトの色合い
	D3DXVECTOR2 m_SetEffectSize;                            //設定するエフェクトの大きさ
	//==============================================================================================

	//======================================
	//パターン関係
	//======================================
	int m_nPattern;                                         //パターン
	int m_nPatternTime;                                     //それぞれのパターンでの時間
	bool m_bPatternFlag;                                    //一度だけ発動するフラグ
	//==============================================================================================

	//======================================
	//攻撃エフェクト関係
	//======================================
	bool m_bUseAttackEffect;//攻撃エフェクトを召喚するかどうか
	int m_nSetAttackEffectLife;//攻撃エフェクトの体力を設定する
	D3DXCOLOR m_SetAttackEffectCol;//攻撃エフェクトの色合いを設定する
	//==============================================================================================

	//======================================
	//当たり判定関係
	//======================================
	void Collision();                                       //当たり判定を取る
	void RectAngleCollisionXY();                            //XY方向の斜めを含めた判定を行う
	void TrueRectAngleCollisionXY();                        //正しいXY方向の回転矩形同士の当たり判定
	void BallCollision();                                   //球の当たり判定を行う
};
//==============================================================================================================================================

//===========================================================
//プレイヤー攻撃クラス
//===========================================================
class CAttackPlayer : public CAttack
{
public:
	CAttackPlayer();            //コンストラクタ
	~CAttackPlayer() override;  //デストラクタ
	HRESULT Init() override;    //初期化処理
	void Uninit() override;     //終了処理
	void Update() override;     //更新処理
	void Draw() override;       //描画処理
	void SetDeath() override;   //死亡フラグ設定処理
	int& GetVineDelBulletNum() { return m_nVineDelBulletNum; }
	void SetVineDelBulletNum(int nNum) { m_nVineDelBulletNum = nNum; }
	static CAttackPlayer* Create(ATTACKTYPE type, int nDamage, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 Scale,D3DXVECTOR3 Rot,
		COLLISIONTYPE CollisionType, D3DXVECTOR3 AddRot,bool bCollisionRelease,int nHitStopTime);//攻撃を生成
private:
	float m_fCorrectionRot;//調整用の向き
	D3DXVECTOR3 m_SaveScale;//拡大率保存用
	D3DXVECTOR3 m_TotalRot;     //総合的な向きを計算する

	bool m_bVineDelStart;       //つるを消す処理開始フラグ
	int m_nVineDelBulletNum;    //つるが消せる弾の数
	void VineProcess();         //つるの処理

	bool m_bCrossBombStaging;   //クロスボムの演出を発動するフラグ
	int m_nCrossBombStagingCnt; //クロスボムの演出の時間をカウントする
	void CrossBombProcess();    //クロスボムの処理

	int m_nBlackHoleDefeatCnt;  //ブラックホールで倒した敵の数をカウントする
	void BlackHoleProcess();    //ブラックホールの処理

	void PhycoBurstProcess();   //サイコバーストの処理
};
//==============================================================================================================================================

//===========================================================
//エネミー攻撃クラス
//===========================================================
class CAttackEnemy : public CAttack
{
public:
	CAttackEnemy();             //コンストラクタ
	~CAttackEnemy() override;   //デストラクタ
	HRESULT Init() override;    //初期化処理
	void Uninit() override;     //終了処理
	void Update() override;     //更新処理
	void Draw() override;       //描画処理
	void SetDeath() override;   //死亡フラグ設定処理
	static CAttackEnemy* Create(ATTACKTYPE type, int nDamage, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 Scale, D3DXVECTOR3 Rot,
		COLLISIONTYPE CollisionType, D3DXVECTOR3 AddRot, bool bCollisionRelease,int nHitStopTime);//攻撃を生成
private:

};
//==============================================================================================================================================


//===========================================================
//攻撃エフェクトクラス
//===========================================================
class CAttackEffect : public CAttack
{
public:
	CAttackEffect();             //コンストラクタ
	~CAttackEffect() override;   //デストラクタ
	HRESULT Init() override;     //初期化処理
	void Uninit() override;      //終了処理
	void Update() override;      //更新処理
	void Draw() override;        //描画処理
	void SetDeath() override;   //死亡フラグ設定処理
	static CAttackEffect* Create(ATTACKTYPE type, int nDamage, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 Scale, D3DXVECTOR3 Rot, D3DXVECTOR3 AddRot,D3DXCOLOR col);//攻撃を生成
private:
	D3DXVECTOR3 m_MaxScale;//拡大率最大値
	D3DXCOLOR m_Col;       //色合い
};
//==============================================================================================================================================

//===========================================================
//ミスターストロング攻撃クラス
//===========================================================
class CMrStrongAttack : public CAttack
{
public:
	CMrStrongAttack();             //コンストラクタ
	~CMrStrongAttack() override;   //デストラクタ
	HRESULT Init() override;     //初期化処理
	void Uninit() override;      //終了処理
	void Update() override;      //更新処理
	void Draw() override;        //描画処理
	void SetDeath() override;   //死亡フラグ設定処理
	static CMrStrongAttack* Create(ATTACKTYPE type, int nDamage, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 Scale, D3DXVECTOR3 Rot,
		COLLISIONTYPE CollisionType, D3DXVECTOR3 AddRot, bool bCollisionRelease, int nHitStopTime);//攻撃を生成
private:
	void BlockProcess();//ブロック攻撃の処理
};
//==============================================================================================================================================

//===========================================================
//ライリー攻撃クラス
//===========================================================
class CRileyAttack : public CAttack
{
public:
	typedef enum
	{
		ATTACKMOVE_RILEY_00_NONE = 0,
		ATTACKMOVE_RILEY_01_ACTION03,
		ATTACKMOVE_RILEY_MAX
	}ATTACKMOVE_RILEY;
	CRileyAttack();                    //コンストラクタ
	~CRileyAttack() override;          //デストラクタ
	HRESULT Init() override;     //初期化処理
	void Uninit() override;      //終了処理
	void Update() override;      //更新処理
	void Draw() override;        //描画処理
	void SetDeath() override;   //死亡フラグ設定処理
	static CRileyAttack* Create(ATTACKTYPE type, int nDamage, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 Scale, D3DXVECTOR3 Rot,
		COLLISIONTYPE CollisionType, D3DXVECTOR3 AddRot, bool bCollisionRelease, int nHitStopTime);//攻撃を生成
	void SetAttackMove(ATTACKMOVE_RILEY AttackMove) { m_AttackMove = AttackMove; }//動き方の種類を決める

	static const int m_nMAX_ACTION03_SUBTYPE = 3;//行動３の攻撃のサブタイプの最大数
private:
	ATTACKMOVE_RILEY m_AttackMove;
	void Action03_ExplosionProcess();
	void Action03_Process();
};
//==============================================================================================================================================

//===========================================================
//ドライヤ攻撃クラス
//===========================================================
class CDraiyaAttack : public CAttack
{
public:
	CDraiyaAttack();                    //コンストラクタ
	~CDraiyaAttack() override;          //デストラクタ
	HRESULT Init() override;     //初期化処理
	void Uninit() override;      //終了処理
	void Update() override;      //更新処理
	void Draw() override;        //描画処理
	void SetDeath() override;   //死亡フラグ設定処理
	static CDraiyaAttack* Create(ATTACKTYPE type, int nDamage, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 Scale, D3DXVECTOR3 Rot,
		COLLISIONTYPE CollisionType, D3DXVECTOR3 AddRot, bool bCollisionRelease, int nHitStopTime);//攻撃を生成
private:
};
//==============================================================================================================================================

//===========================================================
//ガブリエル攻撃クラス
//===========================================================
class CGabrielAttack : public CAttack
{
public:
	typedef enum
	{
		GABRIELATTACKMOVE_00 = 0,//挙動０
		GABRIELATTACKMOVE_01,    //挙動１
		GABRIELATTACKMOVE_02,    //挙動２
		GABRIELATTACKMOVE_03,    //挙動３
		GABRIELATTACKMOVE_04,    //挙動４
		GABRIELATTACKMOVE_05,    //挙動５
		GABRIELATTACKMOVE_MAX
	}GABRIELATTACKMOVE;

	CGabrielAttack();             //コンストラクタ
	~CGabrielAttack() override;   //デストラクタ
	HRESULT Init() override;     //初期化処理
	void Uninit() override;      //終了処理
	void Update() override;      //更新処理
	void Draw() override;        //描画処理
	void SetDeath() override;   //死亡フラグ設定処理
	static CGabrielAttack* Create(ATTACKTYPE type, int nDamage, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 Scale, D3DXVECTOR3 Rot,
		COLLISIONTYPE CollisionType, D3DXVECTOR3 AddRot, bool bCollisionRelease, int nHitStopTime);//攻撃を生成
private:
	static const int m_nMAX_SUMMON_METEO_TORNADO = 8;
	GABRIELATTACKMOVE m_AttackMove;

	void Attack01Process();//攻撃１の処理
};
//==============================================================================================================================================

#endif