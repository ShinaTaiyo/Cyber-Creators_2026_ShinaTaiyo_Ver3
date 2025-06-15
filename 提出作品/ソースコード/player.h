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
#include "character.h"
#include "lockon.h"
#include "gauge.h"
#include "player_actionmode.h"
#include "player_actionmode_mainstate.h"
#include "wire.h"
//==========================================

//======================
//前方宣言
//======================
class CPlayerAbnormalState;
class CPlayerAbnormalState_KnockBack;
class CUi; // UI

//===========================================
//プレイヤー3Dクラス
//===========================================
class CPlayer : public CCharacter
{
public:
	//================================================
    //アクションモード列挙型
    //================================================
	enum class ACTIONMODE
	{
		SHOT = 0,
		DIVE,
		MAX
	};
	//===============================================================================================

	CPlayer(CPlayerMove* pPlayerMove, CPlayerAttack* pPlayerAttack,
		int nPri = 0, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::PLAYER, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_X);//コンストラクタ
	~CPlayer();                                                                                    //デストラクタ
	HRESULT Init() override;                                                                       //初期化処理
	void Uninit() override;                                                                        //終了処理
	void Update() override;                                                                        //更新処理
	void Draw() override;                                                                          //描画処理
	void SetDeath() override;                                                                      //死亡フラグを設定
	static CPlayer* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, D3DXVECTOR3 Scale); //生成処理
	CLockon* GetLockOn() { return m_pLockOn; }                                                     //ロックオンのインスタンスを取得

	//================================================
	//当たり判定
	//================================================
	void SetSuccessCollision(bool bSuccess) { m_bCollision = bSuccess; }    //当たり判定が成功したかどうかを設定
	const bool& GetCollisionSuccess() const { return m_bCollision; }        //当たり判定が成功したかどうかを取得
	//===============================================================================================

	//================================================
	//モードディスプ
	//================================================
	void SetModeDisp(CUi* pModeDisp);                 //モード表示を設定
	CUi* GetModeDisp() { return m_pModeDisp; }        //モード表示を取得
	//===============================================================================================

	//================================================
	//メッシュ
	//================================================
	CWire* GetWire() { return m_pWire; }              //ワイヤーを取得
	//===============================================================================================

	//================================================
	//ゲージ
	//================================================
	CUi* GetDiveGaugeFrame() { return m_pDiveGaugeFrame; }   //ダイブゲージのフレーム
	CUi* GetDivePossibleNum() { return m_pDivePossibleNum; } //ダイブ回数表示用UI
	//===============================================================================================
	
	//================================================
	//モードチェンジ
	//================================================
	void ChengeMoveMode(CPlayerMove* pPlayerMove);                      //移動モードを変える
	void ChengeAttackMode(CPlayerAttack* pPlayerAttack);                //攻撃モードを変える
	void ChengeActionMode(CPlayerActionMode* pPlayerActionMode);        //プレイヤーのアクションモードを変更する
	void SetInitialActionMode(ACTIONMODE ActionMode);                   //アクションモードを設定する
	//===============================================================================================

	//================================================
	//状態異常チェンジ
	//================================================
	void ChengeAbnormalState(CPlayerAbnormalState* pAbnormalState);//状態異常を変える
	//===============================================================================================

	//=============================
	//体力系
	//=============================
	void SetDamage(int nDamage, int nHitStopTime) override; //ダメージを与える
	const bool& IsDamaged() { return m_bDamage; }           //ダメージを受けたかどうかを受け取る（プレイヤーの更新の最後でfalseになるので、ちゃんと取得できる）
	//===============================================================================================

	//================================================
	//静的メンバ取得
	//================================================
	static const int& GetMaxDiveNum() { return s_nMaxDiveNum; }//最大ダイブ可能回数を取得する
	//===============================================================================================
private:
	// == 静的メンバ変数 ===

	static constexpr float m_fNORMALATTACK_SPEED = 10.0f;   //通常攻撃の移動量を設定
	static const int 
		s_nNORMAL_MAXLIFE, // プレイヤーの通常最大体力
        s_nMaxDiveNum;     // プレイヤーの最大ダイブ数

	// === メンバ変数 ===

	float m_fRotAim;  // 目的の向き
	bool 
		m_bCollision, // 当たり判定が成功したかどうか
	    m_bDamage;    // ダメージを受けたかどうか									            	         
	ACTIONMODE m_NowActionMode; // 現在のアクションモード
	CLockon* m_pLockOn; // ロックオンカーソル
	CUi* m_pModeDisp; // モード表示UI
	CWire* m_pWire;   // ワイヤー
	CGauge* m_pHpGauge; //体力ゲージ
	CUi* m_pDivePossibleNum; // ダイブ可能回数UI
	CUi* m_pDiveGaugeFrame;  // ダイブゲージのフレーム				         
	CPlayerMove* m_pMove;   // 移動処理
	CPlayerAttack* m_pAttack; // 攻撃処理
	CPlayerActionMode* m_pPlayerActionMode; // プレイヤーアクションモード(メインのステートクラス)								     
	CPlayerAbnormalState* m_pAbnormalState; // 状態異常

	// === メンバ関数 ===

	void AdjustRot(); // 向き調整処理
	void ActionModeChengeProcess(); // アクションモードを変更する
	void DiveGaugeMaxEffect(); // ダイブゲージがマックスになった時にエフェクトを出す
	void CollisionProcess();   // 当たり判定処理全般
};

//===================================================
//状態異常ステート
//===================================================
class CPlayerAbnormalState
{
public:
	CPlayerAbnormalState();                   //コンストラクタ
	virtual ~CPlayerAbnormalState();          //デストラクタ
	virtual void Process(CPlayer * pPlayer);  //処理
};
//===============================================================================================

//===================================================
//状態異常：ぶっ飛ばし
//===================================================
class CPlayerAbnormalState_KnockBack : public CPlayerAbnormalState
{
public:
	CPlayerAbnormalState_KnockBack(CPlayer * pPlayer,D3DXVECTOR3 KnockBackMove,float fInertia);   //コンストラクタ
	~CPlayerAbnormalState_KnockBack() override;                                                   //デストラクタ
	void Process(CPlayer* pPlayer) override;                                                      //処理
private:																					      
	D3DXVECTOR3 m_KnockBackMove;                                                                  //ノックバックする移動量
	float m_fInertia;                                                                             //減衰度
};
//===============================================================================================
#endif