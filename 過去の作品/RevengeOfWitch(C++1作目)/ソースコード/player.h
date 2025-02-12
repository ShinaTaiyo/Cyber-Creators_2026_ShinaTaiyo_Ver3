//====================================================
//
//７月４日：チャージ攻撃を実装する[player.h]
//Author:ShinaTaiyo
//
//====================================================

#ifndef _PLAYER_H_  
#define _PLAYER_H_

//======================
//インクルード
//======================
#include "main.h"
#include "object2d.h"
#include "objectX.h"
#include "modelparts.h"
#include "Ui3D.h"
#include "meshorbit.h"
#include "ui.h"
#include "magic.h"
//==========================================

//======================
//前方宣言
//======================
class CInputKeyboard;
class CInputJoypad;
class CGauge;
class CNumber;
class CScore;
//==========================================

//===========================================
//プレイヤー3Dクラス
//===========================================
class CPlayer : public CObjectX
{
public:

	//===========================
    //アクションの種類列挙型
    //===========================
	typedef enum
	{
		ACTIONTYPE00_DOUBLEJUMP = 0,//2段ジャンプ
		ACTIONTYPE01_DODGE,         //回避
		ACTIONTYPE02_CHARGEATTACK,  //カウンター
		ACTIONTYPE03_WALLJUMP,      //壁ジャンプ
		ACTIONTYPE_MAX
	}ACTIONTYPE;
	//========================================================================================

	//===========================
	//バレット技種類列挙型
	//===========================
	typedef enum
	{
		BULLETACTION00_CHARGE = 0,//チャージショット
		BULLETACTION01_RAINSHOT,  //レインショット
		BULLETACTION02_BURSTSHOT, //バーストショット
		BULLETACTION03_RAPIDSHOT, //ラピッドショット
		BULLETACTION_MAX
	}BULLETACTION;
	//========================================================================================

	//==========================
	//コマンド発動列挙型
	//==========================
	typedef enum
	{
		PLAYERCOMMAND00_MOVE = 0,    //動いたかどうかを判定
		PLAYERCOMMAND01_JUMP,        //ジャンプしたかどうかを判定
		PLAYERCOMMAND02_DASH,        //ダッシュしたかどうかを判定
		PLAYERCOMMAND03_WALLJUMP,    //壁ジャンプをしたかどうかを判定
		PLAYERCOMMAND04_SHOT,        //ショットをしたかどうかを判定
		PLAYERCOMMAND05_CHARGESHOT,  //チャージショットをしたかどうかを判定
		PLAYERCOMMAND06_RAINSHOT,    //レインショットをしたかどうかを判定
		PLAYERCOMMAND07_BURSTSHOT,   //バーストショットをしたかどうかを判定
		PLAYERCOMMAND08_RAPIDSHOT,   //ラピッドショットを下かどうかを判定
		PLAYERCOMMAND09_MAGIC,       //魔法を使用したかどうかを判定
		PLAYERCOMMAND10_MAGICCHENGE, //魔法を変更したかどうかを判定
		PLAYERCOMMAND11_DOUBLEJUMP,  //２段ジャンプをしたかどうかを判定
		PLAYERCOMMAND_MAX,
	}PLAYERCOMMAND;

	CPlayer();                  //コンストラクタ
	~CPlayer();                 //デストラクタ
	HRESULT Init() override;    //初期化処理
	void Uninit() override;     //終了処理
	void ExtraUninit() override;//別枠の終了処理
	void Update() override;     //更新処理
	void Draw() override;       //描画処理
	void SetDeath() override;   //死亡フラグを設定
	static void Unload();       //モデルの情報の破棄
	static CPlayer * Create(D3DXVECTOR3 pos,D3DXVECTOR3 rot,D3DXVECTOR3 move,D3DXVECTOR3 Scale);
	static int GetNumFile() { return m_nNumFile; }                                                                 //ファイル数取得用
	void SetAddMove(D3DXVECTOR3 move) { m_AddMove += move; }                                                       //加算する移動量を設定する
	void StopMove() { m_bMove = false; }                                                                           //加算する移動量を設定する
	void NotSetMove(bool bX, bool bY, bool bZ);                                                                    //反映しない移動量を設定する
	void SetComplusionPos(D3DXVECTOR3 pos);                                                                        //加算する位置を設定する
	void ReleasePlayer();//プレイヤーを破棄する
	void SetSlideState(bool bState) { m_State.SlideState.bState = bState; }                                        //滑る状態を設定

	//========================================
	//ステータス関連
	//========================================
	void SetDamage(int nDamage, int nHitStopTime) override;                                                        //ダメージを与える
	int& GetHp() { return m_Status.nHp; }                                                                          //体力を取得する
	void SetMp(int nMp);                                                                                           //MPを設定する
	void SetDecelerationState(bool bState) { m_State.DecelerationState.bState = bState; m_State.DecelerationState.nStateCnt = m_nDECELERATIONCNT; }//減速状態を設定
	void AddPlayerAtkBuff(float fAdd) { m_Status.fAtkBuff += fAdd; }          //攻撃バフを加算する
	float GetPlayerAtkBuff() { return m_Status.fAtkBuff; }                    //攻撃バフを取得する
	//===================================================================================================================================================================

	//=======================================
	//アクション関連
	//=======================================
	void SetUnlockAction(ACTIONTYPE type) { m_bUnlockAction[type] = true; }                                        //選んだアクションを開放する
	void SetUnlockBulletAction(BULLETACTION type) { m_bUnlockBulletAction[type] = true; }                          //弾の技を開放する
    //===================================================================================================================================================================

	//=======================================
	//スコア関連
	//=======================================
	CScore* GetScore() { return m_pScore; }//プレイヤーが持っているスコアを取得
	//===================================================================================================================================================================

	//=======================================
	//コマンド関連
	//=======================================
	bool GetJudgeCommand(PLAYERCOMMAND Command) { return m_bJudgeCommand[Command];}
	//===================================================================================================================================================================

	//=======================================
	//魔法関連
	//=======================================
	void SetUnlockMagic(int nMagicNum,D3DXVECTOR3 Pos,bool bUseSignBoard);
	int GetNumPlayerMagic() { return m_nMagicNum; }//プレイヤーの魔法の総数を取得する
	bool* GetUnlockMagic() { return &m_bUnlockMagic[0];}//全ての魔法が開放済みかどうかを取得する
	//===================================================================================================================================================================

	//=======================================
	//リスポーン関係
	//=======================================
	void SetReSpawnPos(D3DXVECTOR3 Pos) { m_ReSpawnPos = Pos; }//プレイヤーのリスポーン位置を設定する
	void ActivePlayerRespawn();                                //プレイヤーのリスポーン処理を発動する
	void ResetRespawnCnt() { m_nRespawnCnt = 0; }
	//===================================================================================================================================================================

	//=======================================
	//慣性関係
	//=======================================
	void SetPlayerSlideInertia(float fInertia);//滑らせるための慣性を設定する
	//===================================================================================================================================================================
	
	//=======================================
	//体力関連
	//=======================================
	void SetHeal(int nHeal, D3DXCOLOR col, float fWidth, float fHeight) override;                  //回復量を与える()
	//===================================================================================================================================================================

	//=======================================
	//壁ジャンプ関係
	//=======================================
	void SetWallJumpBindTime(int nTime) { m_nWallJumpBindTime = nTime; }//壁ジャンプ抑制時間を設定する
	//===================================================================================================================================================================

	//=======================================
	//顔関係
	//=======================================
	typedef enum
	{
		PLAYERREACTION_NORMAL = 0,//リアクション-普通
		PLAYERREACTION_DAMAGE,    //リアクション-ダメージ
		PLAYERREACTION_GLAD,      //リアクション-嬉しい
		PLAYERREACTION_MAX
	}PLAYERREACTION;
	void SetPlayerReaction(PLAYERREACTION ReactionType, int nReactionTime);//プレイヤーのリアクションを設定する
	//===================================================================================================================================================================
private:   
	static const float m_fNORMALJUMPPOWER;                    //通常のジャンプ力
	static const float m_fNORMALSPEED;        
	static const float m_fGRAVITYPOWER;                       //重力の大きさ
	static const int m_nMAX_PLAYERPARTS = 11;                 //プレイヤーのパーツの最大数
	static const int m_nMAX_KEYSET = 30;                      //プレイヤーのキーの最大数
	static const int m_nMAX_MOTION = 5;                       //モーション最大数
	static const int m_nDECELERATIONCNT = 20;                 //減速状態の時間
	static const int m_nMAX_MAGICNUM = 10;                                  //持てる魔法の最大数

	//===============================
	//バレット関係
	//===============================
	static const int m_nRAINSHOTBULLET = 12;                  //レインショットの基本的なバレット数
	static const int m_nRAINSHOT_POWER = 5;                   //レインショットの威力
	static const int m_nBURSTSHOTBULLET = 12;                 //バーストショットの基本的なバレット数
	static const int m_nBURSTSHOT_POWER = 5;                  //バーストショットの威力
	static const int m_nNORMALSHOT_POWER = 5;               //ノーマルショットの威力
	static const int m_nRAPIDSHOT_POWER = 30;                 //ラピッドショットの威力
	//===============================
	//各種魔法消費MP
	//===============================
	static const int m_nPLAYERUSAGEMP[CMagicDisp::MAGICDISP_MAX];//それぞれの魔法の消費MP
	static const int m_nQUICKHEALONEMP = 40;//一回のヒールの回復に使うMP
	//===========================================================================================================

	//===============================
	//各魔法威力
	//===============================
	static const int m_nFIREBALL_POWER = 50;                  //ファイアボールの威力
	static const int m_nBEAMWHIP_POWER = 120;                 //ビームウィップの威力
	static const int m_nFLOWERDICE_POWER = 60;                //フラワードアイスの威力
	static const int m_nTHUNDERBURST_POWER = 3;               //サンダーバーストの威力
	static const int m_nMAGICSWORD_POWER = 125;               //マジックソードの威力
	static const int m_nPLANTSHIELD_POWER = 20;               //プラントシールドの威力
	static const int m_nCROSSBOMB_POWER = 70;                 //クロスボムの威力
	static const int m_nBLACKHOLE_POWER = 200;                //ブラックホールの威力
	static const int m_nPHYCOBURST_POWER = 60;                //サイコバーストの威力
	//==========================================================================================================

	//==============================
	//ステータス関係
	//==============================
	static const int m_nINIT_HP = 20; //初期HP
	static const int m_nINIT_MP = 150;//初期MP
	static constexpr float m_fINIT_ATTACKBUFF = 1.0f;//初期攻撃倍率
	//==========================================================================================================

	//==============================
	//ダメージ関係
	//==============================
	int DamageResult(int nDamage) { return (int)(nDamage * m_Status.fAtkBuff); }//攻撃バフ倍率を参照したダメージ結果を返す
    //==========================================================================================================

	void BlockCollision();                                    //ブロックとの当たり判定を行う
	void DecelerationProcess();                               //減速状態の処理
	typedef struct
	{
		int nHp;             //体力
		int nMaxHp;          //最大体力
		int nMp;             //マジックポイント
		int nMaxMp;          //最大MP
		float fAtkBuff;      //攻撃バフ倍率
	}Status;


	//滑る状態
	typedef struct
	{
		bool bState;
		int nStateCnt;
	}Slide;

	typedef struct
	{
		bool bState;
		int nStateCnt;
	}Deceleration;

	//状態異常
	typedef struct
	{
		Slide SlideState;
		Deceleration DecelerationState;
	}State;

	State m_State;                                                          //状態構造体

	static LPD3DXMESH m_pMeshTemp;                                          //メッシュ（頂点情報）へのポインタ（モデルの種類数分）
	static LPD3DXBUFFER m_pBuffMatTemp;                                     //マテリアルへのポインタ
	static DWORD m_dwNumMatTemp;                                            //マテリアルの数
	static LPDIRECT3DTEXTURE9 m_pTextureTemp[CObjectX::m_nMAX_MAT];         //テクスチャへのポインタ
	static int m_nNumFile;                                                  //ファイル数を保存する
	float m_fRot;                                                           //補正用向き
	int m_nUseMagicTime;                                                    //魔法使用時間をカウントする
	int m_nNextUseMagic;                                                    //次に魔法が使用可能になるまでの時間
	int m_nUseMagicNum;                                                     //どの魔法を使用しているか[
	int m_nCntLanding;                                                      //地面にいる時間をカウントする
	bool m_bPossibleMagic;                                                  //次の魔法が使用可能かどうか
	bool m_bMove;                                                           //移動中かどうか
	bool m_bJustStopMove;                                                   //移動を一瞬止めるフラグ
	bool m_bCompulsionPos;                                                  //位置強制状態
	D3DXVECTOR3 m_CompulsionPos;                                            //加算する位置
	D3DXVECTOR3 m_SaveMove;                                                 //保存する移動量
	D3DXVECTOR3 m_AddMove;                                                  //加算移動量

	float m_fSpeed;                                                         //移動速度
	float m_fJump;                                                          //ジャンプ力
	float m_fCorrectionInertia;                                             //慣性の補正
	int m_nCollisionCheck = 0;                                              //判定確認
	Status m_Status;                                                        //ステータス
	CNumber* m_pNumber;                                                     //番号

	int m_nStopMoveCnt;                                                     //動きを止める時間をカウントする

	//====================================
	//コマンド関係
	//====================================
	bool m_bJudgeCommand[PLAYERCOMMAND_MAX];//プレイヤーがどのコマンドを発動したかを判定
	void ResetJudgeCommand();               //コマンド発動状態をリセット
	//===============================================================================================================

	//====================================
	//ゲージ関係
	//====================================
	CGauge* m_pHpGauge;                                                     //体力ゲージ
	CGauge* m_pMpGauge;                                                     //MPゲージ
	CGauge* m_pUsageMpGauge;                                                //MP消費量を表すゲージ
	CUi* m_pGaugeFrame;                                                     //ゲージ用フレームを生成
	//================================================================================================================

	//====================================
	//顔関係
	//====================================
	CUi* m_pUiReaction;//顔のUI
	int m_nCntChengeUiReactionTime;
	bool m_bChengeUiReactionFlag;
	void UiReactionProcess();
	//=================================================================================================================

	//====================================
	//魔法関係
	//====================================
	CMagicDisp* m_pMagicDisp;                                               //魔法表示
	CMagicWindow* m_pMagicWindow;                                           //魔法表示ウインドウ
	int m_nNowMagicType;                                                    //現在の魔法タイプ
	int m_nMagicNum;                                                        //持っている魔法の数
	int m_nMagicSlot[m_nMAX_MAGICNUM];                                      //現在の魔法の順番
	bool m_bUnlockMagic[CMagicDisp::MAGICDISP_MAX];                         //それぞれの魔法が開放済みかどうか
	//===============================================================================================================

	void NormalAttack();                                                    //通常攻撃
	void MagicAttack();                                                     //魔法攻撃
	void ChengeMagic();                                                     //魔法変更処理 

	void FireBallProcess();                                                 //ファイアボールの処理
	void BeamProcess();                                                     //ビームの処理
	void FlowerdIceProcess();                                               //フラワードアイスの処理
	void ThunderBurstProcess();                                             //サンダーバーストの処理
	void PsychoBladeProcess();                                              //サイコブレイドの処理
	void PlantShieldProcess();                                              //プラントシールドの処理

	void QuickHealProcess();                                                //クイックヒールの処理
	void QucikHealStaging();                                                //クイックヒールの演出

	void CrossBombProcess();                                                //クロスボム

	void BlackHoleProcess();                                                //ブラックホールの処理

	CUi3D* m_pLockOnUi;                                                     //ロックオンUIへのポインタ
	void PhycoBurstProcess();                                               //サイコバーストの処理

	void ResetUseMagic();                                                   //魔法使用状態をリセットする処理

	//===============================================================================================================

	//====================================
	//アクション関係
	//====================================

	static const int m_nMAX_JAMPPUSHCNT = 13;                               //一回のジャンプでボタンを押せる最大カウント数
	static const int m_nMAX_JAMPNUM = 2;                                    //空中でジャンプ出来る回数
	static const int m_nREVIVALDODGETIME = 30;                             //回避コマンドが復活するまでの時間
	static const int m_nDODGETIME = 20;                                     //回避状態を維持する時間
	static const int m_nREVIVALCOUNTERTIME = 120;                           //カウンターコマンドが復活するまでの時間
	static const int m_nCOUNTERTIME = 20;                                   //カウンター状態維持する時間
	static const int m_nMAX_CHARGEATTACKLEVEL = 5;                          //チャージ攻撃の段階
	static const int m_nMAX_INCINCIBLETIME = 100;                           //無敵状態の時間の最大数
	static constexpr float m_fMAX_GRAVITYPOWER = -15.0f;                    //重力の最大数
	static const int m_nMAX_CHARGETIME = 30;                                    //チャージ時間
	static const int m_nCHARGEATTACKPOWER[m_nMAX_CHARGEATTACKLEVEL];        //チャージ攻撃のそれぞれの段階の威力
	//普通のジャンプ
	int m_nJumpPushCnt;                                                     //ジャンプボタンを押している時間をカウントする

	//２段ジャンプ
	int m_nJumpCnt;                                                         //ジャンプ回数をカウントする

	//回避
	typedef struct
	{
		int nRevivalCnt;                                                 //回避コマンドが復活するまでの時間をカウントする                                    
		bool bPossible;                                                  //回避可能かどうか
		bool bState;                                                     //回避状態かどうか
		int nStateCnt;                                                   //回避状態のカウントをする
	}Dodge;

	//チャージ攻撃
	typedef struct
	{
		int nChargeCnt;                                                     //チャージ時間をカウントする
		int nPower;                                                         //威力
		int nChargeLevel;                                                   //チャージ段階
		bool bState;                                                        //チャージ状態かどうか
	}ChargeAttack;

	//回復
	typedef struct
	{
		int nHealCnt;//ヒール状態になってからのカウント
		bool bState; //ヒール状態かどうか
		bool bLastHeal;//回復に必要なMPが足りなくなった時用のフラグ
	}Heal;

	Dodge m_Dodge;                                                          //回避アクション構造体
	ChargeAttack m_ChargeAttack;                                            //チャージ攻撃構造体
	Heal m_Heal;                                                            //回復構造体
	//その他
	bool m_bUnlockAction[ACTIONTYPE_MAX];                                   //アクションが解放されたかどうか

	//関数

	//重力
	void GravityProcess() override;                                         //重力の処理

	void AdjustJump();                                                      //調整ジャンプ
	void DoubleJump();                                                      //２段ジャンプ
	void DodgeAction(float fMoveX,float fMoveY);                            //回避

	void ChargeAttack();                                                    //チャージ攻撃
	void ChargeAttackParticle();                                            //チャージ攻撃のパーティクル召喚処理
	void MaxChargeProcess();                                                //チャージ攻撃のチャージ率がMaxになったときの処理

	void WallJumpProcess();                                                 //壁ジャンプの処理
	void InvincibleProcess();                                               //無敵時間の処理
	void SizeSet();                                                         //モデルパーツ同士の頂点の最大最小を比べ、サイズを設定する
	//========================================================================================

	//=====================================
	//軌跡関係
	//=====================================
	CMeshOrbit* m_pMeshOrbit;                                                //軌跡へのポインタ
	void MeshOrbitProcess();                                                 //軌跡の処理
	//========================================================================================

	//=====================================
    //スコア関係
	//=====================================
	CScore* m_pScore;//スコアへのポインタ
    //========================================================================================

	//=====================================
	//バレット技関係
	//=====================================
	bool m_bUnlockBulletAction[BULLETACTION_MAX];                            //弾の技を開放しているかどうか
	void ChargeShotProcess();                                                //チャージショットの処理を行う
	void RainShotProcess();                                                  //レインショットの処理を行う
	void BurstShotProcess();                                                 //バーストショットの処理を行う
	void RapidShotProcess();                                                 //ラピッドショットの処理を行う
	bool CheckUnlockBulletAction();                                          //バレットアクションを開放しているかどうかのチェックを行う
	//=====================================
	//ブロック関係
	//=====================================

	//=====================================
	//カメラ関係
	//=====================================
	bool m_bReturnCamera;                                                    //カメラを戻すフラグ

	bool m_bStartFade;                                                       //フェードスタート

	//=====================================
	//無敵状態関係
	//=====================================
	bool m_bInvincible;                                                      //無敵状態かどうか
	int m_nCntInvincible;                                                    //無敵状態の時間をカウントする 

	//=====================================
	//モデルパーツ関係
	//=====================================
	CModelParts * m_apModelParts[m_nMAX_PLAYERPARTS];                        //プレイヤーのパーツ数分の
	void LoadModelParts();                                                   //モデルパーツのロード

	//=====================================
	//モーション関係
	//=====================================

	//キーデータ
	struct KEY
	{
		//位置
		D3DXVECTOR3 Pos;
		//向き
		D3DXVECTOR3 Rot;
	};

	//キー情報
	struct KEYSET
	{
		int nFrame;//フレーム数
		KEY aKey[m_nMAX_PLAYERPARTS];//プレイヤーのパーツ分モーションする
	};

	//モーション情報
	struct MOTION
	{
		bool bLoop;   //ループするかどうか
		int nCntKey;  //キーカウント
		int nCntFrame;//フレームカウント
		int nNumKey;  //キー総数
		KEYSET aKeySet[m_nMAX_KEYSET];//キー情報
	};

	//モーション名列挙型
	typedef enum
	{
		MOTIONTYPE00_NUTRAL = 0,//ニュートラル
		MOTIONTYPE01_RUNNING,   //走る
		MOTIONTYPE02_ATTACK,    //攻撃
		MOTIONTYPE03_JUMP,      //ジャンプ
		MOTIONTYPE04_LANDING,   //着地
		MOTIONTYPE_MAX
	}MOTIONTYPE;

	int m_nNowMotion;                         //モーションカウンター
	int m_nMotionOld;                         //1f前のモーション
	int m_nNextMotion;                        //次のモーション
	int m_nCntFrame;                          //現在のフレーム数
	MOTION m_aMotion[m_nMAX_MOTION];          //モーション
	bool m_bChengeMotion;                     //モーションを次に変えるかどうか
	int m_nTargetMotion;                      //次に変えるモーション番号                    
	bool m_bPriorityMotion[MOTIONTYPE_MAX];   //優先させるモーション


	void LoadMotion();                //モーション情報をロードする
	void MotionProcess();             //モーション処理
	void MotionPriorityProcess();     //モーションの優先度を管理する

	//=====================================
	//その他
	//=====================================
	void DispPlayerInfo();            //プレイヤーの情報を表示する

	//=====================================
	//慣性関係
	//=====================================
	void PlayerInertiaProcess();//プレイヤーの慣性の処理
	float m_fSlideInertia;//滑らせるための慣性
	float m_fReturnInertia;//戻すための慣性
	//=================================================================================

	//=====================================
	//リスポーン関係
	//=====================================
	D3DXVECTOR3 m_ReSpawnPos;//リスポーンする位置
	int m_nRespawnCnt;
	//=================================================================================

	//=====================================
	//壁ジャンプ関係
	//=====================================
	bool m_bPossibleWallJump;//壁ジャンプが可能かどうか
	int m_nWallJumpBindTime; //壁ジャンプが不可能な時間をカウントする
	void WallJumpBindProcess();//壁ジャンプを束縛する処理
	void WallJumpParticleProcess();//壁ジャンプの演出用パーティクルを召喚
	//=================================================================================

	//=====================================
	//当たり判定関係
	//=====================================
	void CollisionEnemy();//敵との当たり判定
	void CollisionItem(); //アイテムとの当たり判定
	void CollisionTrap(); //トラップとの当たり判定
	//=================================================================================


};

#endif
