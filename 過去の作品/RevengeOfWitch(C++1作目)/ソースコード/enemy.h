//====================================================
//
//７月１９日：敵のAI実装[enemy.h]
//Author:ShinaTaiyo
//
//====================================================

#ifndef _ENEMY_H_  
#define _ENEMY_H_

//======================
//インクルード
//======================
#include "main.h"
#include "objectX.h"
#include "aimodel.h"
//==========================================

//======================
//前方宣言
//======================
class CModel;

//==========================================
//エネミークラス
//==========================================
class CEnemy : public CObjectX
{
public:
	//===========================
	//敵の種類
	//===========================
	typedef enum
	{
		ENEMYTYPE00_SLIME = 0,
		ENEMYTYPE01_FIRESLIME,
		ENEMYTYPE02_SWORDSLIME,
		ENEMYTYPE03_HAMMERSLIME,
		ENEMYTYPE04_ORANGESLIME,
		ENEMYTYPE05_THUNDERSLIME,
		ENEMYTYPE06_SCANINGSLIME,
		ENEMYTYPE07_NEEDLESLIME,
		ENEMYTYPE08_ENGELSLIME,
		ENEMYTYPE09_DARKSLIME,
		ENEMYTYPE10_RAINBOWSLIME,
		ENEMYTYPE_MAX
	}ENEMYTYPE;
	//======================================================================
	CEnemy();                   //コンストラクタ
	~CEnemy() override;         //デストラクタ
	HRESULT Init() override;    //初期化処理
	void Uninit() override;     //終了処理
	void Update() override;     //更新処理
	void Draw() override;       //描画処理
	void SetDamage(int nDamage, int nHitStopTime) override;  //ダメージを与える処理
	void SetDeath() override;   //死亡フラグを設定
	static int GetNumFile() { return m_nNumFile; }//ファイルの数を取得する
	static CEnemy * Create(ENEMYTYPE type,int nLife,D3DXVECTOR3 pos, D3DXVECTOR3 move,D3DXVECTOR3 Scale,
		D3DXVECTOR3 MoveLemgthAIPos,D3DXVECTOR3 MoveLengthAIRot,D3DXVECTOR3 MoveLengthAIScale);//敵を生成

	//==============================================
	//敵のAIモデル
	//==============================================
	CAIModel* GetEnemyMoveRangeAIModel() { return m_pMoveRangeAI; }

private:
	static const char* m_ENEMY_FILENAME[ENEMYTYPE_MAX];           //ブロックのファイルネーム
	static const int m_nMAXENEMY01BULLET;
	static int m_nNumFile;                                        //読み込んだファイルの数

	//==============================
	//敵０の静的メンバ
	//==============================
	static constexpr float m_fENEMY00_MOVESTARTLENGTH = 500.0f;
	static constexpr float m_fENEMY00_NEARACTIONPATTERNLENGTH = 150.0f;//近くの時のパターンを発動する距離
	//==============================================================================================================

	//==============================
	//敵１の静的メンバ
	//==============================
	static constexpr float m_fENEMY01_NORMALSPEED = 2.0f;//敵１の普通の速度
	static const int m_nENEMY01_ACTION0_FRAME = 40;//敵１の行動０のフレーム数
	//==============================================================================================================
	
	//==============================
	//敵２の静的メンバ
	//==============================
	static constexpr float m_fENEMY02_JUMPPOWER = 16.0f;//敵２のジャンプ力
	static constexpr float m_fENEMY02_ACTION1_SPEED = 5.0f;//敵２のアクション２の速さ
	//==============================================================================================================

	//==============================
	//敵４の静的メンバ
	//==============================
	static constexpr float m_fENEMY04_ACTION2THREEWAYAIM = 0.8f;//アクション２の弾の発射角度間隔
	//==============================================================================================================

	//==============================
	//敵５の静的メンバ
	//==============================
	static const int m_nMAX_ENEMY05_BULLETNUM = 3;//弾の数
	//==============================================================================================================

	//==============================
	//当たり判定
	//==============================
	void Collision();
	//====================================================================

	void CalculationLength();//プレイヤーとの距離を計算する
	ENEMYTYPE m_type;       //敵の種類
	D3DXVECTOR3 m_Aim;      //ベクトル
	CObject* m_pLandingObj;   //乗っているオブジェクト
	float m_fTotalAim;      //総合したベクトル
	bool m_bUse;            //使用状態
	bool m_bIsJumping;      //ジャンプ中かどうか
	bool m_bIsWalling;      //壁に当たっているかどうか
	bool m_bAttack;         //攻撃をするかどうか
	int m_nCntTime;         //出現してからのフレーム数を測る

	int m_nCntAction;       //アクション回数カウント用
	int m_nPattern;         //パターン
	int m_nSubPattern;      //サブのパターン
	int m_nPatternTime;     //パターンに入ってからの時間をカウントする
	bool m_bPattern;        //行動パターンを実行するかどうか
	bool m_bAction;         //行動をするかどうか 

	float m_fLength;        //プレイヤーとの距離

	float m_fShotRot;       //バレットなどを発射する向き

	float m_fGravityPower;                                  //重力の大きさ

	bool m_bSpeedSwitch;    //移動方向を変えるフラグ

	void GravityProcess();                                  //重力をかける
	//=================================
	//敵の行動パターン
	//=================================
	void Enemy00Pattern();                                 //敵０のパターン
	void Enemy01Pattern();                                 //敵１のパターン
	void Enemy02Pattern();                                 //敵２のパターン
	void Enemy03Pattern();                                 //敵３のパターン
	void Enemy04Pattern();                                 //敵４のパターン
	void Enemy05Pattern();                                 //敵５のパターン
	void Enemy06Pattern();                                 //敵６のパターン
	void Enemy07Pattern();                                 //敵７のパターン
	void Enemy08Pattern();                                 //敵８のパターン
	void Enemy09Pattern();                                 //敵９のパターン
	void Enemy10Pattern();                                 //敵１０のパターン
	void ResetPattern();                                   //行動パターンのリセット
	void ApproachProcess(float fSpeed);                    //接近する処理
	//====================================================================================================

	//================================
	//敵のAI用モデル
	//================================
	CAIModel* m_pMoveRangeAI;//移動範囲を決めるAI
	void AdjustEnemyPos();//敵の位置調整を行う処理
	//====================================================================================================

	//================================
	//エディタ表示用
	//================================
	void DispInfo() override;//情報を表示する
	void ChengeLifeProcess();//体力を変える処理
	void ChengeMoveLengthAIScale();//移動範囲AIモデルの範囲を変える処理
	void ChengeMoveLengthAIScaleX();//移動範囲AIモデルの拡大率Xを変える処理
	void ChengeMoveLengthAIScaleY();//移動範囲AIモデルの拡大率Yを変える処理
	void ChengeMoveLengthAIScaleZ();//移動範囲AIモデルの拡大率Zを変える処理
	void ChengeMoveLengthAIPos();   //移動範囲AIモデルの位置を変える処理


	//====================================================================================================

};

#endif
