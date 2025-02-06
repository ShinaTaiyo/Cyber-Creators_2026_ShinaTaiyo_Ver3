//======================================================================================
//
//１１月１０日：プレイヤーアクションモードクラスを作る[player_actionmode.h]
//Author:ShinaTaiyo
//
//======================================================================================

//============================================
//２重インクルード防止
//============================================
#ifndef _PLAYER_ACTIONMODE_H_
#define _PLAYER_ACTIONMODE_H_
//======================================================================================

//============================================
//インクルード
//============================================
#include "main.h"
#include "object2d.h"
//======================================================================================

//============================================
//前方宣言
//============================================
class CPlayer;

//スーパークラス
class CPlayerMove
{
public:
	CPlayerMove();                             //コンストラクタ
	virtual ~CPlayerMove();                    //デストラクタ
	virtual void MoveProcess(CPlayer* pPlayer);//移動処理
	void JumpProcess(CPlayer* pPlayer);         //ジャンプ処理
	void DodgeProcess(CPlayer* pPlayer);//回避処理
	const bool& GetDodge() const { return m_bDodge; }
	void SetDodge(bool bDodge) { m_bDodge = bDodge; }
	const bool& GetLanding() const { return m_bIsLanding; }
	void SetLanding(bool bLanding) { m_bIsLanding = bLanding; }
private:
	//*静的メンバ宣言
	static constexpr float s_fACCELL_PARAM = 7.0f;

	//*変数宣言
	bool m_bIsLanding;//地面にいるかどうか
	bool m_bDodge;//回避しているかどうか
};

//通常移動クラス
class CPlayerMove_Normal : public CPlayerMove
{
public:
	CPlayerMove_Normal();                    //コンストラクタ
	~CPlayerMove_Normal() override;                   //デストラクタ
	void MoveProcess(CPlayer* pPlayer) override;//移動処理

};

//ダイブ移動クラス
class CPlayerMove_Dive : public CPlayerMove
{
public:
	CPlayerMove_Dive();                      //コンストラクタ
	~CPlayerMove_Dive() override;                     //デストラクタ
	void MoveProcess(CPlayer* pPlayer) override;//移動処理
	void SetDiveMove(D3DXVECTOR3 Move) { m_DiveMove = Move; }//ダイブ時の移動量を設定
private:
	static const float s_fCOLLISIONDIVEMOVELENGTH;//ダイブ移動がワイヤーヘッドと当たる距離
	D3DXVECTOR3 m_DiveMove;//ダイブの移動量
};

//ダイブ準備クラス
class CPlayerMove_PrepDive : public CPlayerMove
{
public:
	CPlayerMove_PrepDive(CPlayer * pPlayer);                      //コンストラクタ
	~CPlayerMove_PrepDive() override;            //デストラクタ
	void MoveProcess(CPlayer* pPlayer) override; //移動処理
};

//引っ付きクラス
class CPlayerMove_Stuck : public CPlayerMove
{
public: 
	CPlayerMove_Stuck(CPlayer * pPlayer);                         //コンストラクタ
	~CPlayerMove_Stuck();                        //デストラクタ
	void MoveProcess(CPlayer* pPlayer) override; //移動処理
private:
	D3DXVECTOR3 m_NowPos;//カメラの位置のモーションに使う
};

//移動できないクラス
class CPlayerMove_Dont : public CPlayerMove
{
public:
	CPlayerMove_Dont();//コンストラクタ
	~CPlayerMove_Dont() override;//デストラクタ
	void MoveProcess(CPlayer* pPlayer) override;//移動処理
private:
};
//======================================================================================



//==============================================
//攻撃クラス
//==============================================

//スーパークラス
class CPlayerAttack
{
public:
	CPlayerAttack() {};//コンストラクタ
	virtual ~CPlayerAttack() {};//デストラクタ
	virtual void AttackProcess(CPlayer* pPlayer) {};//攻撃処理
};

//射撃クラス
class CPlayerAttack_Shot : public CPlayerAttack
{
public:
	CPlayerAttack_Shot();//コンストラクタ
	~CPlayerAttack_Shot() override;//デストラクタ
	void AttackProcess(CPlayer* pPlayer) override;//攻撃処理
private:
	static const float s_fNORMAL_SHOTSPEED;//通常の射撃速度
};

//ダイブクラス
class CPlayerAttack_Dive : public CPlayerAttack
{
public:
	CPlayerAttack_Dive();//コンストラクタ
	~CPlayerAttack_Dive();//デストラクタ
	void AttackProcess(CPlayer* pPlayer) override;//攻撃処理
};

//攻撃できないクラス
class CPlayerAttack_Dont : public CPlayerAttack
{
public:
	CPlayerAttack_Dont();//コンストラクタ
	~CPlayerAttack_Dont() override;//デストラクタ
	void AttackProcess(CPlayer* pPlayer) override;//攻撃処理
};
//======================================================================================

//==============================================
//エフェクトクラス
//==============================================

//スーパークラス
class CPlayerEffect
{
public:
	CPlayerEffect();//コンストラクタ
	virtual ~CPlayerEffect();//デストラクタ
	virtual void EffectProcess(CPlayer * pPlayer);//エフェクト処理
};

//ダイブエフェクトクラス
class CPlayerEffect_Dive : public CPlayerEffect
{
public:
	CPlayerEffect_Dive();//コンストラクタ
	~CPlayerEffect_Dive() override;//デストラクタ
	void EffectProcess(CPlayer* pPlayer) override;//エフェクト処理
};

//エフェクトなしクラス
class CPlayerEffect_None : public CPlayerEffect
{
public:
	CPlayerEffect_None() {}; //コンストラクタ
	~CPlayerEffect_None() {}; //デストラクタ
	void EffectProcess(CPlayer* pPlayer) override {};//エフェクト処理
};

//==============================================
//ワイヤー発射中判定クラス
//==============================================

//スーパークラス
class CPlayerWireShot
{
public:
	CPlayerWireShot();//コンストラクタ
	virtual ~CPlayerWireShot();//デストラクタ
	virtual void WireShotProcess(CPlayer * pPlayer);//ワイヤー発射処理
	static void StartWireShotProcess(CPlayer* pPlayer);//ワイヤーの発射を開始する
};

//発射するクラス
class CPlayerWireShot_Do : public CPlayerWireShot
{
public:
	CPlayerWireShot_Do();//コンストラクタ
	~CPlayerWireShot_Do() override;//デストラクタ
	void WireShotProcess(CPlayer* pPlayer) override;//ワイヤー発射処理
private:
	void FrightenedEnemy(CPlayer* pPlayer);//敵を怯えさせる処理
	void DecisionCameraRot(CPlayer* pPlayer);//カメラの向きを決める処理
};

//発射しないクラス
class CPlayerWireShot_Dont : public CPlayerWireShot
{
public:
	CPlayerWireShot_Dont();//コンストラクタ
	~CPlayerWireShot_Dont() override;//デストラクタ
	void WireShotProcess(CPlayer* pPlayer) override;//ワイヤー発射処理
};
#endif