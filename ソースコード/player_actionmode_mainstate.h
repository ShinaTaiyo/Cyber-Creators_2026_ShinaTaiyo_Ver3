//======================================================================================
//
//２月１７日：プレイヤーアクションモードのメイン状態クラスを作る[player_actionmode_mainstate.h]
//Author:ShinaTaiyo
//
//======================================================================================

//============================================
//インクルード
//============================================
#include "main.h"

//============================================
//前方宣言
//============================================
class CPlayer;
//======================================================================================

//============================================
//２重インクルード防止
//============================================
#ifndef _PLAYER_ACTIONMODE_MAINSTATE_H_
#define _PLAYER_ACTIONMODE_MAINSTATE_H_
//======================================================================================

//プレイヤーアクションメインステートクラス
class CPlayerActionMode
{
public:
	CPlayerActionMode();                    //コンストラクタ
	virtual ~CPlayerActionMode();           //デストラクタ
	virtual void Process(CPlayer * pPlayer);//処理（何もなし）
};

//プレイヤーアクション：射撃移動クラス
class CPlayerActionMode_ShotMove : public CPlayerActionMode
{
public:
	CPlayerActionMode_ShotMove(CPlayer * pPlayer);//コンストラクタ
	~CPlayerActionMode_ShotMove() override;       //デストラクタ
	void Process(CPlayer* pPlayer) override;      //処理
};

//プレイヤーアクション：ダイブ準備クラス
class CPlayerActionMode_PrepDive : public CPlayerActionMode
{
public:
	CPlayerActionMode_PrepDive(CPlayer* pPlayer);//コンストラクタ
	~CPlayerActionMode_PrepDive() override;      //デストラクタ
	void Process(CPlayer* pPlayer) override;     //処理
};

//プレイヤーアクション：ワイヤー発射クラス
class CPlayerActionMode_WireShot : public CPlayerActionMode
{
public:
	CPlayerActionMode_WireShot(CPlayer* pPlayer);        //コンストラクタ
	~CPlayerActionMode_WireShot() override;              //デストラクタ
	void Process(CPlayer* pPlayer) override;             //処理
private:
	static constexpr float s_fWIREHEAD_SHOTSPEED = 60.0f;//ワイヤーの頭を発射する速度
	void FrightenedEnemy(CPlayer* pPlayer);              //敵を怯えさせる処理
	void DecisionCameraRot(CPlayer* pPlayer);            //カメラの向きを決める処理
};

//プレイヤーアクション：ダイブ移動クラス
class CPlayerActionMode_DiveMove : public CPlayerActionMode
{
public:
	CPlayerActionMode_DiveMove(D3DXVECTOR3 Move,CPlayer * pPlayer);//コンストラクタ
	~CPlayerActionMode_DiveMove() override;                        //デストラクタ
	void Process(CPlayer* pPlayer) override;                       //処理
private:
	//================
	//静的メンバ宣言
	//================
	static constexpr float s_fCOLLISIONSTARTLENGTH = 120.0f;       //プレイヤーがワイヤーの頭と衝突する距離
	//===========================================================================================================
};

//プレイヤーアクション：ダイブアタッククラス
class CPlayerActionMode_DiveAttack : public CPlayerActionMode
{
public:
	CPlayerActionMode_DiveAttack(CPlayer* pPlayer);//コンストラクタ
	~CPlayerActionMode_DiveAttack() override;      //デストラクタ
	void Process(CPlayer* pPlayer) override;       //処理
};

//プレイヤーアクション：引っ付きクラス
class CPlayerActionMode_Stuck : public CPlayerActionMode
{
public:
	CPlayerActionMode_Stuck(CPlayer* pPlayer);//コンストラクタ
	~CPlayerActionMode_Stuck() override;      //デストラクタ
	void Process(CPlayer* pPlayer) override;  //処理
private:
	bool m_bStartWireShot;                    //ワイヤーの発射を開始するかどうか
};

#endif