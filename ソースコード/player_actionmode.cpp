//===================================================================================================================
//
//１１月１０日：プレイヤーのアクションモードクラスを作る[player_actionmode.cpp]
//Author:ShinaTaiyo
//
//===================================================================================================================

//===============================================================
//インクルード
//===============================================================
#include "player_actionmode.h"
#include "texture.h"
#include "calculation.h"
#include "debugtext.h"
#include "manager.h"
#include "collision.h"
#include "effect.h"
#include "enemy.h"
#include "meshorbit.h"
#include "attack.h"
#include "input.h"
#include "lockon.h"
#include "wire.h"
#include "player.h"
#include "game.h"
#include "tutorial.h"
#include "particle.h"
#include "sound.h"
#include "wire_head.h"
#include "camera.h"
//===================================================================================================================

//******************************************************************************************************************************************************
//プレイヤー移動クラス
//******************************************************************************************************************************************************

//=====================================================================================================
//コンストラクタ
//=====================================================================================================
CPlayerMove::CPlayerMove() : m_bIsLanding(false), m_bDodge(false)
{

}
//======================================================================================================================================================

//=====================================================================================================
//デストラクタ
//=====================================================================================================
CPlayerMove::~CPlayerMove()
{

}
//======================================================================================================================================================

//=====================================================================================================
//移動処理
//=====================================================================================================
void CPlayerMove::MoveProcess(CPlayer* pPlayer)
{
	if (m_bDodge == false)
	{//回避中なら絶対に通常移動はさせない
		const D3DXVECTOR3& Move = pPlayer->GetMoveInfo().GetMove();
		D3DXVECTOR3 AddMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		bool bMove = false;//移動しているかどうか
		float fRotAim = 0.0f;

		bMove = CCalculation::CaluclationMove(true, AddMove, 10.0f, CCalculation::MOVEAIM_XZ, fRotAim);
		pPlayer->GetRotInfo().SetRot(D3DXVECTOR3(pPlayer->GetRotInfo().GetRot().x, fRotAim, pPlayer->GetRotInfo().GetRot().z));

		pPlayer->GetMoveInfo().SetUseInteria(true, CObjectX::GetNormalInertia());
		pPlayer->GetMoveInfo().SetUseGravity(true, CObjectX::GetNormalGravity());
		if (bMove == true)
		{
			pPlayer->SetNextMotion(1);//移動モーションを次のモーションに指定する
			pPlayer->GetMoveInfo().SetMove(AddMove + D3DXVECTOR3(0.0f, Move.y, 0.0f));
			CGame::GetTutorial()->SetSuccessCheck(CTutorial::CHECK::MOVE);
		}
		else
		{
			pPlayer->SetNextMotion(0);//動いていないので、初期モーションのニュートラルに設定
		}
	}
}
//======================================================================================================================================================

//=====================================================================================================
//ジャンプ処理
//=====================================================================================================
void CPlayerMove::JumpProcess(CPlayer* pPlayer)
{
	if (pPlayer->GetLanding())
	{//地面にいるならジャンプ
		pPlayer->GetMoveInfo().SetUseGravity(true, CObjectX::GetNormalGravity());
		if (CManager::GetInputJoypad()->GetTrigger(CInputJoypad::JOYKEY::A) || CManager::GetInputKeyboard()->GetTrigger(DIK_SPACE))
		{
			CGame::GetTutorial()->SetSuccessCheck(CTutorial::CHECK::JUMP);
			pPlayer->GetMoveInfo().SetMove(D3DXVECTOR3(pPlayer->GetMoveInfo().GetMove().x, 20.0f, pPlayer->GetMoveInfo().GetMove().z));
		}
	}
}
//======================================================================================================================================================

//=====================================================================================================
//回避処理
//=====================================================================================================
void CPlayerMove::DodgeProcess(CPlayer* pPlayer)
{
	CInputJoypad* pInput = CManager::GetInputJoypad();
	if ((pInput->GetLT_Trigger() || CManager::GetInputKeyboard()->GetTrigger(DIK_LSHIFT)) && m_bDodge == false)
	{
		m_bDodge = true;
		pPlayer->GetMoveInfo().SetMove(D3DXVECTOR3(pPlayer->GetMoveInfo().GetMove().x * s_fACCELL_PARAM, pPlayer->GetMoveInfo().GetMove().y, pPlayer->GetMoveInfo().GetMove().z * s_fACCELL_PARAM));
		pPlayer->GetMoveInfo().SetUseInteria(true, 0.1f);
		CGame::GetTutorial()->SetSuccessCheck(CTutorial::CHECK::DASH);
	}
	float fAverageSpeed = (fabsf(pPlayer->GetMoveInfo().GetMove().x) + fabsf(pPlayer->GetMoveInfo().GetMove().z)) / 2;

	if (fAverageSpeed < 0.51f && m_bDodge == true)
	{
		m_bDodge = false;
		pPlayer->GetMoveInfo().SetUseInteria(true, CObjectX::GetNormalInertia());
	}
}

//======================================================================================================================================================

//******************************************************************************************************************************************************
//プレイヤー普通移動クラス
//******************************************************************************************************************************************************

//=====================================================================================================
//コンストラクタ
//=====================================================================================================
CPlayerMove_Normal::CPlayerMove_Normal()
{

}
//======================================================================================================================================================

//=====================================================================================================
//デストラクタ
//=====================================================================================================
CPlayerMove_Normal::~CPlayerMove_Normal()
{
}
//======================================================================================================================================================

//=====================================================================================================
//移動処理
//=====================================================================================================
void CPlayerMove_Normal::MoveProcess(CPlayer* pPlayer)
{
	CPlayerMove::MoveProcess(pPlayer);

	CPlayerMove::DodgeProcess(pPlayer);

	CPlayerMove::JumpProcess(pPlayer);
}
//======================================================================================================================================================


//******************************************************************************************************************************************************
//プレイヤーダイブ移動クラス
//******************************************************************************************************************************************************

//=====================================================================================================
//コンストラクタ
//=====================================================================================================
CPlayerMove_PrepDive::CPlayerMove_PrepDive(CPlayer* pPlayer)
{

}
//======================================================================================================================================================

//=====================================================================================================
//デストラクタ
//=====================================================================================================
CPlayerMove_PrepDive::~CPlayerMove_PrepDive()
{

}
//======================================================================================================================================================

//=====================================================================================================
//移動処理
//=====================================================================================================
void CPlayerMove_PrepDive::MoveProcess(CPlayer* pPlayer)
{
	CPlayerMove::MoveProcess(pPlayer);//通常移動

	CPlayerMove::DodgeProcess(pPlayer);//回避移動

	CPlayerMove::JumpProcess(pPlayer);//ジャンプ移動
}
//======================================================================================================================================================

//******************************************************************************************************************************************************
//プレイヤーダイブ移動クラス
//******************************************************************************************************************************************************

//=====================================================================================================
//静的メンバ宣言
//=====================================================================================================

//=====================================================================================================
//コンストラクタ
//=====================================================================================================
CPlayerMove_Dive::CPlayerMove_Dive() : m_DiveMove(D3DXVECTOR3(0.0f,0.0f,0.0f))
{

}
//======================================================================================================================================================

//=====================================================================================================
//デストラクタ
//=====================================================================================================
CPlayerMove_Dive::~CPlayerMove_Dive()
{

}
//======================================================================================================================================================

//=====================================================================================================
//移動処理
//=====================================================================================================
void CPlayerMove_Dive::MoveProcess(CPlayer* pPlayer)
{
	CWireHead* pWireHead = pPlayer->GetWire()->GetWireHead();
	bool bInput = CManager::GetInputJoypad()->GetRT_Press();

	if (CManager::GetInputJoypad()->GetRT_Press() || CManager::GetInputMouse()->GetMouseLeftClickPress())
	{
		bInput = true;
	}

	pPlayer->GetMoveInfo().SetMove(CCalculation::Calculation3DVec(pPlayer->GetPosInfo().GetPos(),pWireHead->GetPosInfo().GetPos(),s_fDIVEMOVE));//目的地に達するまで狙い続ける

	if (pPlayer->IsDamaged())
	{//ダイブ移動中にダメージを受けたら射撃モードの初期状態に戻す（ダイブ攻撃は強力な攻撃なので、ノーリスクで突撃させたくないから）
		pPlayer->SetInitialActionMode(CPlayer::ACTIONMODE::SHOT);
	}

	//CCamera* pCamera = CManager::GetCamera();
	//if (CCalculation::CalculationLength(pPlayer->GetPosInfo().GetPos(), pWireHead->GetPosInfo().GetPos()) < s_fCOLLISIONDIVEMOVELENGTH)
	//{//ダイブ時に判定したら移動モードと攻撃モードを通常に戻す
	//	if(bInput == true && pWireHead->GetCollisionObjType() != CObject::TYPE::ENEMY)
	//	{//引っ付き→ダイブ（もしワイヤーヘッドが衝突したオブジェクトが敵の場合、敵には引っ付きたくないので、攻撃に移行する
	//		//pPlayer->SetRot(pPlayer->GetRot());
	//		CGame::GetTutorial()->SetSuccessCheck(CTutorial::CHECK::STUCKWALL);
	//		pPlayer->ChengeMoveMode(DBG_NEW CPlayerMove_Stuck(pPlayer));
	//		pPlayer->ChengeAttackMode(DBG_NEW CPlayerAttack_Dont());
	//	}
	//	else
	//	{//攻撃→射撃モード
	//		pPlayer->ChengeAttackMode(DBG_NEW CPlayerAttack_Dive());
	//		pPlayer->GetWire()->SetUseDraw(false);
	//		pPlayer->ChengeMoveMode(DBG_NEW CPlayerMove_PrepDive(pPlayer));
	//		pPlayer->GetRotInfo().SetRot(D3DXVECTOR3(0.0f, pCamera->GetRot().y, 0.0f));
	//	}
	//}
}
//======================================================================================================================================================

//******************************************************************************************************************************************************
//引っ付き移動クラス
//******************************************************************************************************************************************************

//=====================================================================================================
//コンストラクタ（ワイヤー発射開始フラグは、デフォルトで発射可能なのでtrueで初期化)
//=====================================================================================================
CPlayerMove_Stuck::CPlayerMove_Stuck(CPlayer* pPlayer) : m_NowPos(pPlayer->GetPosInfo().GetPos()),m_bStartWireShot(true)
{
	CCamera* pCamera = CManager::GetCamera();
	CWireHead* pWireHead = pPlayer->GetWire()->GetWireHead();
	pPlayer->GetPosInfo().SetPos(pPlayer->GetPosInfo().GetPos() - pPlayer->GetMoveInfo().GetMove());
	//pCamera->SetRot(D3DXVECTOR3(-pWireHead->GetRot().x,pWireHead->GetRot().y + D3DX_PI,0.0f));//カメラの向きを固定したワイヤーヘッドの逆側に！

	//==========================
	//カメラの向きを求める
	//==========================
	D3DXVECTOR3 ComRot = pWireHead->GetPosInfo().GetPos() - pPlayer->GetPosInfo().GetPos();
	D3DXVec3Normalize(&ComRot, &ComRot);
	float fYaw = atan2f(ComRot.x, ComRot.z);
	float fPitch = atan2f(ComRot.y, sqrtf(powf(ComRot.x, 2) + powf(ComRot.z, 2)));
	D3DXVECTOR3 ResultRot = D3DXVECTOR3(-fPitch - D3DX_PI * 0.5f, fYaw, 0.0f);//カメラの向きを調整する（前方向を基準にする）

	CSound* pSound = CManager::GetSound();

	pSound->PlaySoundB(CSound::SOUND_LABEL::SE_LANDING_000);

	if (pCamera->GetRot().x > ResultRot.x - 0.5f && pCamera->GetRot().x < ResultRot.x + 0.5f &&
		pCamera->GetRot().y > ResultRot.y - 0.5f && pCamera->GetRot().y < ResultRot.y + 0.5f)
	{//現在のカメラの向きが目的の向きに近かったらダイブ先に合わせる
		//CParticle::SummonParticle(CParticle::TYPE::TYPE00_NORMAL, 10, 60, 30.0f, 30.0f, 100, 10, false, pPlayer->GetSenterPos(), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), true);
		pCamera->ChengeState(DBG_NEW CCameraState_TurnAround(D3DXVECTOR3(-pWireHead->GetRotInfo().GetRot().x, pWireHead->GetRotInfo().GetRot().y + D3DX_PI, 0.0f), 0.15f));
	}
	//==============================================================================================
}
//======================================================================================================================================================

//=====================================================================================================
//デストラクタ
//=====================================================================================================
CPlayerMove_Stuck::~CPlayerMove_Stuck()
{

}
//======================================================================================================================================================

//=====================================================================================================
//移動処理
//=====================================================================================================
void CPlayerMove_Stuck::MoveProcess(CPlayer* pPlayer)
{
	pPlayer->GetMoveInfo().SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//*変数宣言
	CCamera* pCamera = CManager::GetCamera(); // カメラへのポインタ
	CWireHead* pWireHead = pPlayer->GetWire()->GetWireHead(); // ワイヤーヘッドへのポインタ
	D3DXVECTOR3 WireHeadRot = pWireHead->GetRotInfo().GetRot(); // ワイヤーヘッドの向き（オイラー角）
	CWire* pWire = pPlayer->GetWire();
	CLockon* pLockon = pPlayer->GetLockOn();//ロックオンへのポインタ
	CInputJoypad* pInputJoypad = CManager::GetInputJoypad();//ジョイパッド入力情報を取得
	CInputMouse* pInputMouse = CManager::GetInputMouse();//マウス入力情報を取得
	pWireHead->GetPosInfo().SetPos(pPlayer->GetPosInfo().GetPos());//ダイブ準備中なのでワイヤーヘッドをプレイヤーの位置に固定

	if (pInputJoypad->GetTrigger(CInputJoypad::JOYKEY::B) || pInputMouse->GetMouseMiddleClickTrigger())
	{//引っ付きながら射撃する
		m_bStartWireShot = m_bStartWireShot ? false : true;//フラグのONOFFを変える
		if (m_bStartWireShot == false)
		{//m_bStartWireShotをtrueにしたときにステートが変わらないようにするため、falseの時だけステートを変える
			pPlayer->ChengeAttackMode(DBG_NEW CPlayerAttack_StackShot(pPlayer));
		}
	}

	if ((pInputJoypad->GetRT_Trigger() || pInputMouse->GetMouseLeftClickTrigger()) && pLockon->GetSuccessRayCollision() && m_bStartWireShot)
	{//ワイヤー発射移動モードにチェンジ
		pPlayer->ChengeAttackMode(DBG_NEW CPlayerAttack_Dont());//強制的に攻撃不可にする（ダイブ発動までの準備をするため）
	}

}
//======================================================================================================================================================

//******************************************************************************************************************************************************
//プレイヤー移動できないクラス
//******************************************************************************************************************************************************

//=====================================================================================================
//コンストラクタ
//=====================================================================================================
CPlayerMove_Dont::CPlayerMove_Dont()
{

}
//======================================================================================================================================================

//=====================================================================================================
//デストラクタ
//=====================================================================================================
CPlayerMove_Dont::~CPlayerMove_Dont()
{

}
//======================================================================================================================================================

//=====================================================================================================
//移動処理
//=====================================================================================================
void CPlayerMove_Dont::MoveProcess(CPlayer* pPlayer)
{
	pPlayer->GetMoveInfo().SetMove(D3DXVECTOR3(0.0f, pPlayer->GetMoveInfo().GetMove().y, 0.0f));
}
//======================================================================================================================================================

//******************************************************************************************************************************************************
//プレイヤー攻撃できるクラス
//******************************************************************************************************************************************************

//=====================================================================================================
//静的メンバ宣言
//=====================================================================================================
const float CPlayerAttack_Shot::s_fNORMAL_SHOTSPEED = 50.0f;

//=====================================================================================================
//コンストラクタ
//=====================================================================================================
CPlayerAttack_Shot::CPlayerAttack_Shot()
{

}
//======================================================================================================================================================

//=====================================================================================================
//デストラクタ
//=====================================================================================================
CPlayerAttack_Shot::~CPlayerAttack_Shot()
{

}
//======================================================================================================================================================

//=====================================================================================================
//攻撃処理
//=====================================================================================================
void CPlayerAttack_Shot::AttackProcess(CPlayer* pPlayer)
{
	CLockon* pLockon = pPlayer->GetLockOn();
	D3DXVECTOR3 ShotPos = pPlayer->GetPosInfo().GetPos() + D3DXVECTOR3(0.0f, pPlayer->GetSizeInfo().GetVtxMax().y, 0.0f);
	D3DXVECTOR3 Move = CCalculation::Calculation3DVec(ShotPos, pLockon->GetNearRayColObjPos(), s_fNORMAL_SHOTSPEED);
	CAttackPlayer* pAttackPlayer = nullptr;//プレイヤー攻撃へのポインタ
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_J) == true || CManager::GetInputJoypad()->GetRT_Repeat(4) == true ||
		CManager::GetInputMouse()->GetMouseLeftClickRepeat(4) == true)
	{
		pAttackPlayer = CAttackPlayer::Create(CAttack::ATTACKTYPE::BULLET,CAttack::TARGETTYPE::ENEMY,CAttack::COLLISIONTYPE::SQUARE,true,true,3,0,45,ShotPos, pPlayer->GetRotInfo().GetRot(), Move, D3DXVECTOR3(1.0f, 1.0f, 1.0f));
		pAttackPlayer->GetMoveInfo().SetUseInteria(false, CObjectX::GetNormalInertia());
		pAttackPlayer->GetLifeInfo().SetAutoSubLife(true);
		pAttackPlayer->SetHitOtherThanLibing(true);

		CManager::GetSound()->PlaySoundB(CSound::SOUND_LABEL::SE_SHOT_001);//射撃効果音を出す
		CGame::GetTutorial()->SetSuccessCheck(CTutorial::CHECK::SHOT);		
	}

	if (CManager::GetInputJoypad()->GetRT_Press())
	{
		pPlayer->SetNextMotion(2);//攻撃ボタンを押している限り、次のモーションは攻撃モーションになる
	}
}
//======================================================================================================================================================

//******************************************************************************************************************************************************
//プレイヤー攻撃できないクラス
//******************************************************************************************************************************************************

//=====================================================================================================
//コンストラクタ
//=====================================================================================================
CPlayerAttack_Dont::CPlayerAttack_Dont()
{

}
//======================================================================================================================================================

//=====================================================================================================
//デストラクタ
//=====================================================================================================
CPlayerAttack_Dont::~CPlayerAttack_Dont()
{

}
//======================================================================================================================================================

//=====================================================================================================
//攻撃処理
//=====================================================================================================
void CPlayerAttack_Dont::AttackProcess(CPlayer* pPlayer)
{
	//
}
//======================================================================================================================================================

//******************************************************************************************************************************************************
//プレイヤーダイブ攻撃クラス
//******************************************************************************************************************************************************

//=====================================================================================================
//コンストラクタ
//=====================================================================================================
CPlayerAttack_Dive::CPlayerAttack_Dive()
{

}
//======================================================================================================================================================

//=====================================================================================================
//デストラクタ
//=====================================================================================================
CPlayerAttack_Dive::~CPlayerAttack_Dive()
{

}
//======================================================================================================================================================

//=====================================================================================================
//攻撃処理
//=====================================================================================================
void CPlayerAttack_Dive::AttackProcess(CPlayer* pPlayer)
{
	CUiState_Gauge* pUiState_Gauge = dynamic_cast<CUiState_Gauge*>(CGame::GetPlayer()->GetDiveGaugeFrame()->GetUiState(CUiState::UISTATE::GAUGE));//UIのゲージ情報を取得
	if (pUiState_Gauge != nullptr)
	{
		CGauge* pDiveGauge = pUiState_Gauge->GetGauge();//ダイブゲージを取得する

		CUi* pDivePossibleNum = pPlayer->GetDivePossibleNum();//ダイブ可能回数のUIを取得する
		CWireHead* pWireHead = pPlayer->GetWire()->GetWireHead();//ワイヤーの頭を取得する
		CUiState_Numeric* pUiState_Numeric = dynamic_cast<CUiState_Numeric*>(pDivePossibleNum->GetUiState(CUiState::UISTATE::NUMERIC));//UIの数字情報を取得する
		if (pUiState_Numeric->GetValue() > 0)
		{//ダイブゲージがたまっていたら爆発攻撃を発動
			CAttackPlayer* pAttackPlayer = CAttackPlayer::Create(CAttack::ATTACKTYPE::EXPLOSION, CAttack::TARGETTYPE::ENEMY, CAttack::COLLISIONTYPE::SQUARE, false, true, 50, 30, 100, pPlayer->GetPosInfo().GetPos(), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.1f, 0.1f, 0.1f),
				D3DXVECTOR3(1.0f, 1.0f, 1.0f));
			pAttackPlayer->GetSizeInfo().SetUseAddScale(D3DXVECTOR3(0.4f, 0.4f, 0.4f), true);
			pAttackPlayer->SetColor(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), 2, false, false, false);
			pAttackPlayer->GetLifeInfo().SetUseRatioLifeAlpha(true);
			pAttackPlayer->SetCollisionRelease(false);
			CGame::GetTutorial()->SetSuccessCheck(CTutorial::CHECK::TAKEDIVE);
			pUiState_Numeric->SetValue(pUiState_Numeric->GetValue() - 1, pDivePossibleNum);
			CManager::GetSound()->PlaySoundB(CSound::SOUND_LABEL::SE_EXPLOSION_000);
			//目的の向きまで少しづつ動かす（カメラの前は-D3DX_PI * 0.5f,プレイヤーはデフォルトの向きが異なるので、Rot.y + D3DX_PI)
			CManager::GetCamera()->ChengeState(DBG_NEW CCameraState_TurnAround(D3DXVECTOR3(-D3DX_PI * 0.5f, pPlayer->GetRotInfo().GetRot().y + D3DX_PI, 0.0f), 0.1f));

			//爆発を見せたいので、カメラと注視点の距離を一定時間変える
			CManager::GetCamera()->ChengeLengthState(DBG_NEW CCameraLengthState_Gradually(300.0f, 0.1f, 60));
		}
		pWireHead->GetDrawInfo().SetUseDraw(false);//ワイヤーの頭の描画をオフにする
		pPlayer->ChengeAttackMode(DBG_NEW CPlayerAttack_Dont());//爆発し終わったので、攻撃状態を「なし」に戻す
	}

}
//======================================================================================================================================================

//******************************************************************************************************************************************************
//プレイヤーワイヤー発射中クラス
//******************************************************************************************************************************************************

//******************************************************************************************************************************************************
//攻撃状態：引っ付き射撃
//******************************************************************************************************************************************************

//=====================================================================================================
//コンストラクタ
//=====================================================================================================
CPlayerAttack_StackShot::CPlayerAttack_StackShot(CPlayer* pPlayer) : m_bDelayModeChengeFrame(true)
{
	//CLockon* pLockon = pPlayer->GetLockOn();//ロックオンターゲットを取得
	//pLockon->ChengeTexture(CLockon::TYPE::STUCKSHOT);//引っ付き射撃用のターゲットテクスチャに変える
}
//======================================================================================================================================================

//=====================================================================================================
//デストラクタ
//=====================================================================================================
CPlayerAttack_StackShot::~CPlayerAttack_StackShot()
{

}
//======================================================================================================================================================

//=====================================================================================================
//攻撃処理
//=====================================================================================================
void CPlayerAttack_StackShot::AttackProcess(CPlayer* pPlayer)
{
	//CLockon* pLockon = pPlayer->GetLockOn();
	//D3DXVECTOR3 ShotPos = pPlayer->GetPosInfo().GetPos() + D3DXVECTOR3(0.0f, pPlayer->GetSizeInfo().GetVtxMax().y, 0.0f);
	//D3DXVECTOR3 Move = CCalculation::Calculation3DVec(ShotPos, pLockon->GetNearRayColObjPos(),s_fNORMAL_SHOTSPEED);
	//CAttackPlayer* pAttackPlayer = nullptr;//プレイヤー攻撃へのポインタ
	//CInputJoypad* pInputJoypad = CManager::GetInputJoypad();//ジョイパッド入力情報へのポインタ
	//CInputMouse* pInputMouse = CManager::GetInputMouse();   //マウス入力情報へのポインタ
	//if (pInputJoypad->GetRT_Repeat(s_nSHOT_FREQUENCY) == true ||
	//	pInputMouse->GetMouseLeftClickRepeat(s_nSHOT_FREQUENCY) == true)
	//{
	//	pAttackPlayer = CAttackPlayer::Create(CAttack::ATTACKTYPE::BULLET, CAttack::TARGETTYPE::ENEMY, CAttack::COLLISIONTYPE::SQUARE, true, true, 3, 0, 45, ShotPos, pPlayer->GetRotInfo().GetRot(), Move, D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	//	pAttackPlayer->GetMoveInfo().SetUseInteria(false, CObjectX::GetNormalInertia());
	//	pAttackPlayer->GetLifeInfo().SetAutoSubLife(true);
	//	pAttackPlayer->SetHitOtherThanLibing(true);

	//	CManager::GetSound()->PlaySoundB(CSound::SOUND_LABEL::SE_SHOT_001);//射撃効果音を出す
	//	CGame::GetTutorial()->SetSuccessCheck(CTutorial::CHECK::SHOT);
	//}
	//if (pInputJoypad->GetRT_Press() || pInputMouse->GetMouseLeftClickPress())
	//{
	//	pPlayer->SetNextMotion(2);//攻撃ボタンを押している限り、次のモーションは攻撃モーションになる
	//}

	//if ((pInputJoypad->GetTrigger(CInputJoypad::JOYKEY::B) || pInputMouse->GetMouseMiddleClickTrigger()) && m_bDelayModeChengeFrame == false)
	//{//スタック移動モードでこの攻撃モードにJOYKEY_Bボタンで変えているので、この処理に最初に移行したときにJOYKEY_Bボタンが発動してしまうので、1フレーム遅らせる
	//	pPlayer->ChengeAttackMode(DBG_NEW CPlayerAttack_Dont());//攻撃しないモードに再び変える
	//	pLockon->ChengeTexture(CLockon::TYPE::DIVE);//ダイブ用のターゲットテクスチャに戻す
	//}

	//if (m_bDelayModeChengeFrame == true)
	//{//1フレーム上のモードチェンジの入力を遅らせたので役目は終了。falseに戻す
	//	m_bDelayModeChengeFrame = false;
	//}

}
//======================================================================================================================================================