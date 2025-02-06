//===================================================================
//
//６月３日：3Dオブジェクトを描画する[Camera.cpp]
//Author:ShinaTaiyo
//
//===================================================================

//===================================================================
//インクルード
//===================================================================
#include "camera.h"
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "object.h"
#include "particle.h"
#include "game.h"
#include "debugtext.h"
#include "edit.h"
#include "calculation.h"
#include "collision.h"
#include "object.h"
#include "objectX.h"
#include "title.h"
#include "tutorial.h"
//====================================================================================================

//====================================================================
//静的メンバ宣言
//====================================================================
const float CCamera::m_BESIDECAMERALENGTH = 570.0f;//ビサイドビューのカメラの距離
const float CCamera::s_fINITIAL_LENGTH = 200.0f;   //カメラとの最初の距離
//====================================================================================================

//====================================================================
//コンストラクタ
//====================================================================
CCamera::CCamera() : m_SupportPos(D3DXVECTOR3(0.0f,0.0f,0.0f)),m_fLength(s_fINITIAL_LENGTH), m_fTurningRotSpeed(0.0f),m_fTurningSpeedY(0.0f),m_PosV(D3DXVECTOR3(0.0f,0.0f,0.0f)),
m_PosR(D3DXVECTOR3(0.0f,0.0f,0.0f)),m_VecU(D3DXVECTOR3(0.0f,0.0f,0.0f)),m_Rot(D3DXVECTOR3(0.0f,0.0f,0.0f)),m_mtxProjection(),m_mtxView(),m_CameraType(CAMERATYPE_BIRD),m_DifferenceLength(D3DXVECTOR3(0.0f,0.0f,0.0f)),
m_ZoomSpeed(D3DXVECTOR3(0.0f,0.0f,0.0f)),m_nShakeFrame(0),m_ModeTime(0),m_fShakePower(0.0f),m_fAddLength(0.0f),m_AddPosR(D3DXVECTOR3(0.0f,0.0f,0.0f)),m_AddPosV(D3DXVECTOR3(0.0f,0.0f,0.0f))
,m_bCustom(false),m_State(CAMERASTATE::NORMAL),m_pCameraState(DBG_NEW CCameraState_Normal()),m_pCameraLengthState(DBG_NEW CCameraLengthState())
{

}
//====================================================================================================

//====================================================================
//デストラクタ
//====================================================================
CCamera::~CCamera()
{

}
//====================================================================================================

//====================================================================
//初期化処理
//====================================================================
HRESULT CCamera::Init()
{
	m_PosV = D3DXVECTOR3(0.0f, 200.0f,-350.0f);     //支点
	m_PosR = D3DXVECTOR3(0.0f,0.0f,0.0f);                          //注視点
	m_VecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);         //上方向ベクトル（法線）
	m_Rot = D3DXVECTOR3(-D3DX_PI * 0.5f,0.0f,0.0f);                           //向き
	m_mtxProjection = {};                           //プロジェクションマトリックス
	m_mtxView = {};                                 //ビューマトリックス
	m_CameraType = CAMERATYPE_BIRD;                 //カメラモードの種類
	m_nShakeFrame = 0;                              //カメラを揺らすフレーム数
	m_fShakePower = 0.0f;                           //カメラを揺らす力
	m_DifferenceLength = D3DXVECTOR3(0.0f,0.0f,0.0f);              //対象との距離
	m_ModeTime = 0;                                 //モードの時間
	m_ZoomSpeed = D3DXVECTOR3(0.0f,0.0f,0.0f);                     //ズームする速さ
	return S_OK;
}
//====================================================================================================

//====================================================================
//終了処理
//====================================================================
void CCamera::Uninit()
{
	if (m_pCameraState != nullptr)
	{
		delete m_pCameraState;
		m_pCameraState = nullptr;
	}

	if (m_pCameraLengthState != nullptr)
	{
		delete m_pCameraLengthState;
		m_pCameraLengthState = nullptr;
	}
}
//====================================================================================================


//====================================================================
//更新処理
//====================================================================
void CCamera::Update()
{
	m_pCameraState->Process(this);      //カメラの角度に関する処理
	m_pCameraLengthState->Process(this);//カメラの距離に関する処理
	//========================================
	//カメラの向きを補正する
	//========================================
	if (m_Rot.x < -D3DX_PI + 0.01f)
	{
		m_Rot.x = -D3DX_PI + 0.01f;
	}
	if (m_Rot.x > -0.01f)
	{
		m_Rot.x = -0.01f;
	}

	//向きが3.14と-3.14の境界をまたぐときの補正
	m_Rot.x = CCalculation::CorrectionRot(m_Rot.x);
	m_Rot.y = CCalculation::CorrectionRot(m_Rot.y);
	m_Rot.z = CCalculation::CorrectionRot(m_Rot.z);

	CManager::GetDebugText()->PrintDebugText("カメラの向き：%f %f %f\n", m_Rot.x, m_Rot.y, m_Rot.z);
	CManager::GetDebugText()->PrintDebugText("カメラの視点：%f %f %f\n", m_PosV.x, m_PosV.y, m_PosV.z);
	CManager::GetDebugText()->PrintDebugText("カメラの注視点：%f %f %f\n", m_PosR.x, m_PosR.y, m_PosR.z);
	CManager::GetDebugText()->PrintDebugText("カメラとの距離：%f\n",m_fAddLength);

	//カメラの通常の注視点を設定し続ける
	NormalCameraMove();

	if (CScene::GetMode() == CScene::MODE_GAME)
	{
		if (m_PosV.y < 0.0f)
		{//視点が０以下なら全ての影を描画しない
			CObjectX::SetCommonDraw(false);
		}
		else
		{
			CObjectX::SetCommonDraw(true);
		}
	}
	MakeTransparent();//カメラと中止点と重なったオブジェクトを透明にする処理

	//=================================================================================================================================

}
//====================================================================================================


//====================================================================
//カメラ設定処理
//====================================================================
void CCamera::SetCamera()
{
	LPDIRECT3DDEVICE9 pDevice;                       //デバイスへのポインタ
	pDevice = CManager::GetRenderer()->GetDevice();  //デバイスの取得

	//プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxProjection);

	if (m_CameraType == CAMERATYPE_BIRD || m_CameraType == CAMERATYPE_BESIDE)
	{
		//プロジェクションマトリックスを作成z
		D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
			D3DXToRadian(45.0f),
			(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
			10.0f,
			30000.0f);
	}
	else if (m_CameraType == CAMERATYPE_SIDE)
	{
		//平行東映（正射影）
		D3DXMatrixOrthoLH(
			&m_mtxProjection,
			(float)SCREEN_WIDTH / 2,
			(float)SCREEN_HEIGHT / 2,
			10.0f,
			2500.0f);
	}
	//プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	//ビューマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxView);

	//==========================
	//カメラを揺らす処理
	//==========================
	D3DXVECTOR3 adjust = D3DXVECTOR3(0.0f,0.0f,0.0f);//揺れの補正
	D3DXVECTOR3 ResultPosV = m_PosV;//結果的な支点
	D3DXVECTOR3 ResultPosR = m_PosR;//結果的な注視点
	if (m_nShakeFrame > 0)
	{
		m_nShakeFrame--;
		adjust.x += (float)(rand() % 100 - 50) * m_fShakePower;
		adjust.y += (float)(rand() % 100 - 50) * m_fShakePower;

		ResultPosV += adjust;
		ResultPosR += adjust;
	}
	//========================================================================================
	//ビューマトリックスの作成
	D3DXMatrixLookAtLH(&m_mtxView,
		&ResultPosV,
		&ResultPosR,
		&m_VecU);

	//ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW,&m_mtxView);

}
//====================================================================================================

//====================================================================
//カメラ設定処理
//====================================================================
void CCamera::SetShake(int nShakeFrame, float fShakePower)
{
	m_nShakeFrame = nShakeFrame;
	m_fShakePower = fShakePower;
}
//====================================================================================================

//====================================================================
//カメラの状態を変える
//====================================================================
void CCamera::ChengeState(CCameraState* pCameraState)
{
	delete m_pCameraState;
	m_pCameraState = pCameraState;
}
//====================================================================================================

//====================================================================
//カメラの距離の状態を変える
//====================================================================
void CCamera::ChengeLengthState(CCameraLengthState* pCameraLengthState)
{
	delete m_pCameraLengthState;
	m_pCameraLengthState = pCameraLengthState;
}
//====================================================================================================

//====================================================================
//普通のカメラの位置を設定し続ける
//====================================================================
void CCamera::NormalCameraMove()
{
	CObject* pManagerObject = nullptr;
	D3DXVECTOR3 RotVec = CCalculation::RadToVec(m_Rot);
	switch (CScene::GetMode())
	{
	case CScene::MODE_TITLE:
		m_PosR = CTitle::GetPlayer()->GetPosInfo().GetPos() + D3DXVECTOR3(100.0f,75.0f,0.0f);
		m_PosV = m_PosR + RotVec * m_fLength;
		break;
	case CScene::MODE_GAME://通常はプレイヤーの位置を軸に動く(カメラの向きを使い旋回する）
		if (CGame::GetPlayer() != nullptr)
		{
			m_PosR = CGame::GetPlayer()->GetPosInfo().GetPos() + D3DXVECTOR3(sinf(m_Rot.y + D3DX_PI * 0.5f) * 30.0f, 60.0f,cosf(m_Rot.y + D3DX_PI * 0.5f) * 30.0f) + m_AddPosR;
			m_PosV = m_PosR + RotVec * m_fLength;
		}
		break;
	case CScene::MODE_EDIT:
		m_PosV = m_PosR + RotVec * (m_fLength);
		break;
	default:
		break;
	}
}
//====================================================================================================

//====================================================================
//すごく近いオブジェクトを透明にする処理
//====================================================================
void CCamera::MakeTransparent()
{
	if (CScene::GetMode() != CScene::MODE_EDIT)
	{
		D3DXVECTOR3 Ray = m_PosR - m_PosV;
		D3DXVec3Normalize(&Ray, &Ray);
		D3DXVECTOR3 RayCollisionPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		float fLength = 0.0f;//レイが当たった場所の距離
		for (int nCntPri = 0; nCntPri < CObject::m_nMAXPRIORITY; nCntPri++)
		{
			CObject* pObj = CObject::GetTopObject(nCntPri);

			while (pObj != nullptr)
			{
				CObject* pNext = pObj->GetNextObject();
				CObject::TYPE Type = pObj->GetType();
				if (Type == CObject::TYPE::BGMODEL || Type == CObject::TYPE::BLOCK)
				{
					CObjectX* pObjX = static_cast<CObjectX*>(pObj);//オブジェクトXの機能を使う
					if (CCollision::RayIntersectsAABBCollisionPos(m_PosV, Ray, pObjX->GetPosInfo().GetPos() + pObjX->GetSizeInfo().GetVtxMin(), pObjX->GetPosInfo().GetPos() + pObjX->GetSizeInfo().GetVtxMax(), RayCollisionPos))
					{
						fLength = CCalculation::CalculationLength(m_PosV, RayCollisionPos);

						if (m_fLength > fLength)
						{//カメラの距離よりもオブジェクトとレイが当たった位置が近い場合は透明にする
							pObjX->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f), 3, true, true,false);
						}
					}
				}

				pObj = pNext;//リストを進める
			}
		}
	}
}
//====================================================================================================

//====================================================================
//ボスを倒したときのカメラの処理を行う
//====================================================================
void CCamera::BossDefeatCameraProcess()
{

}
//====================================================================================================

//====================================================================
//カメラの旋回処理を行う
//====================================================================
void CCamera::TurningCameraProcess()
{
	////旋回向き
	//m_Rot.y += m_fTurningRotSpeed;

	////支点Yの位置
	//m_PosV.y += m_fTurningSpeedY;

	////距離を更新する
	//m_fLength += m_fAddLength;

	////カメラの旋回処理
	//m_PosV.x = sinf(m_Rot.y) * m_fLength + m_PosR.x;
	//m_PosV.z = cosf(m_Rot.y) * m_fLength + m_PosR.z;
}
//===================================================================================================================================================

//※以降ステートクラス

//****************************************************************************************************
//カメラが後ろを向く状態クラス
//****************************************************************************************************

//=================================================================
//コンストラクタ
//=================================================================
CCameraState_TurnAround::CCameraState_TurnAround(D3DXVECTOR3 AimRot, float fAdjustTurnSpeed) : m_AimRot(AimRot),m_fAdjustTurnSpeed(fAdjustTurnSpeed)
{

}
//===================================================================================================================================================

//=================================================================
//デストラクタ
//=================================================================
CCameraState_TurnAround::~CCameraState_TurnAround()
{

}
//===================================================================================================================================================

//=================================================================
//処理
//=================================================================
void CCameraState_TurnAround::Process(CCamera* pCamera)
{
	const D3DXVECTOR3 & NowRot = pCamera->GetRot();

	//向きの差分を求める
	float fRotDiffX = m_AimRot.x - NowRot.x;
	float fRotDiffY = m_AimRot.y - NowRot.y;

	fRotDiffX = CCalculation::CorrectionRot(fRotDiffX);
	fRotDiffY = CCalculation::CorrectionRot(fRotDiffY);

	CManager::GetDebugText()->PrintDebugText("カメラの向きの差分:%f\n");

	//加算する向きの量を補正して求める
	float fAddRotX = fRotDiffX * m_fAdjustTurnSpeed;
	float fAddRotY = fRotDiffY * m_fAdjustTurnSpeed;
	fAddRotX = CCalculation::CorrectionRot(fAddRotX);
	fAddRotY = CCalculation::CorrectionRot(fAddRotY);
	pCamera->SetRot(D3DXVECTOR3(pCamera->GetRot().x + fAddRotX, pCamera->GetRot().y + fAddRotY, pCamera->GetRot().z));

	//差分の絶対値が0.01f以下なら
	if (fabsf(fAddRotX) < 0.005f && fabsf(fAddRotY) < 0.005f)//YawとPitchの向き加算量が0.01f以下になったらステートを戻す
	{
		pCamera->ChengeState(DBG_NEW CCameraState_Normal());
	}
}
//===================================================================================================================================================

//****************************************************************************************************
//普通の状態
//****************************************************************************************************

//=================================================================
//静的メンバ宣言
//=================================================================
const float CCameraState_Normal::s_fNORMAL_AROUNDROTSPEED = 0.02f;//カメラの通常回転速度
//===================================================================================================================================================


//=================================================================
//コンストラクタ
//=================================================================
CCameraState_Normal::CCameraState_Normal()
{

}
//===================================================================================================================================================

//=================================================================
//デストラクタ
//=================================================================
CCameraState_Normal::~CCameraState_Normal()
{

}
//===================================================================================================================================================

//=================================================================
//処理
//=================================================================
void CCameraState_Normal::Process(CCamera* pCamera)
{
	//========================================
    //カメラの向きを変える
    //========================================
	if (CScene::GetMode() != CScene::MODE_TITLE || CScene::GetMode() != CScene::MODE_RESULT)
	{
		if (CManager::GetInputKeyboard()->GetPress(DIK_Q))
		{
			pCamera->SetRot(pCamera->GetRot() + D3DXVECTOR3(0.0f, -s_fNORMAL_AROUNDROTSPEED, 0.0f));
		}
		if (CManager::GetInputKeyboard()->GetPress(DIK_E))
		{
			if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT))
			{
				pCamera->SetRot(D3DXVECTOR3(pCamera->GetRot().x, 0.0f, pCamera->GetRot().z));
			}
			else
			{
				pCamera->SetRot(pCamera->GetRot() + D3DXVECTOR3(0.0f, s_fNORMAL_AROUNDROTSPEED, 0.0f));
			}
		}

		if (CManager::GetInputJoypad()->GetPress(CInputJoypad::JOYKEY::LB))
		{
			pCamera->SetRot(pCamera->GetRot() + D3DXVECTOR3(0.0f, -s_fNORMAL_AROUNDROTSPEED, 0.0f));
		}
		if (CManager::GetInputJoypad()->GetPress(CInputJoypad::JOYKEY::RB))
		{
			pCamera->SetRot(pCamera->GetRot() + D3DXVECTOR3(0.0f, s_fNORMAL_AROUNDROTSPEED, 0.0f));
		}

		//===========================
		//Xボタンを押していたら
		//===========================
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//シフトキーを押しながら・・・
			if (CManager::GetInputKeyboard()->GetPress(DIK_X) == true)
			{
				pCamera->SetAddPosR(pCamera->GetAddPosR() + D3DXVECTOR3(0.0f, -5.0f, 0.0f));
			}
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_X) == true)
		{
			pCamera->SetAddPosR(pCamera->GetAddPosR() + D3DXVECTOR3(0.0f, 5.0f, 0.0f));
		}

		//=================================================
		//ゲームパッド、又はマウスでカメラの向きを変える
		//=================================================
		if (CManager::GetInputJoypad()->GetRStickPress(16) == true)
		{
			if (CScene::GetMode() == CScene::MODE::MODE_GAME)
			{
				CGame::GetTutorial()->SetSuccessCheck(CTutorial::CHECK::CAMERACONTROLL);
			}

			CManager::GetCamera()->SetRot(pCamera->GetRot() + D3DXVECTOR3(cosf(CManager::GetInputJoypad()->GetRStickAimRot() + D3DX_PI) * 0.04f,
				sinf(CManager::GetInputJoypad()->GetRStickAimRot()) * 0.04f, 0.0f));
		}
		float fAngle = 0.0f;
		if (CManager::GetInputMouse()->GetMouseMoveAngle(fAngle))
		{
			if (CScene::GetMode() == CScene::MODE::MODE_GAME)
			{
				CGame::GetTutorial()->SetSuccessCheck(CTutorial::CHECK::CAMERACONTROLL);
			}
			if (CScene::GetMode() != CScene::MODE_TITLE)
			{//タイトルではEnterしか押さないので、タイトルではマウスでカメラを動かさない
				CManager::GetCamera()->SetRot(pCamera->GetRot() + D3DXVECTOR3(cosf(fAngle) * 0.05f,
				   sinf(fAngle) * 0.05f, 0.0f));
			}
		}

		//===========================
		//Cボタンを押していたら
		//===========================
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//シフトキーを押しながら・・・
			if (CManager::GetInputKeyboard()->GetPress(DIK_C) == true)
			{
				pCamera->SetAddPosV(pCamera->GetAddPosV() + D3DXVECTOR3(0.0f, -5.0f, 0.0f));
			}
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_C) == true)
		{
			pCamera->SetAddPosV(pCamera->GetAddPosV() + D3DXVECTOR3(0.0f, 5.0f, 0.0f));
		}

		//===========================
		//Vボタンを押していたら
		//===========================
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//シフトキーを押しながら・・・
			if (CManager::GetInputKeyboard()->GetPress(DIK_V) == true)
			{
				pCamera->SetLength(pCamera->GetLength() - 5.0f);
			}
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_V) == true)
		{
			pCamera->SetLength(pCamera->GetLength() + 5.0f);
		}


		//========================================
		//カメラを揺らす
		//========================================
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_F1) == true)
		{
			pCamera->SetShake(50, 0.2f);
		}

		//========================================
		//後ろを向く
		//========================================
		if (CManager::GetInputJoypad()->GetTrigger(CInputJoypad::JOYKEY::Y))
		{
			if (CScene::GetMode() == CScene::MODE::MODE_GAME)
			{
				CGame::GetTutorial()->SetSuccessCheck(CTutorial::CHECK::TURNAROUND);
			}
			pCamera->ChengeState(DBG_NEW CCameraState_TurnAround(pCamera->GetRot() + D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), 0.15f));
		}
	}
}
//===================================================================================================================================================

//*****************************************************************
//カメラの距離を変える状態のクラス
//*****************************************************************

//=================================================================
//コンストラクタ
//=================================================================
CCameraLengthState::CCameraLengthState()
{

}
//===================================================================================================================================================

//=================================================================
//デストラクタ
//=================================================================
CCameraLengthState::~CCameraLengthState()
{

}
//===================================================================================================================================================

//=================================================================
//処理
//=================================================================
void CCameraLengthState::Process(CCamera* pCamera)
{

}
//===================================================================================================================================================

//*****************************************************************
//カメラの距離を目的の位置に徐々に変える状態のクラス
//*****************************************************************

//=================================================================
//コンストラクタ
//=================================================================
CCameraLengthState_Gradually::CCameraLengthState_Gradually(float fLength, float fAdjustAddSpeed, int nChengeFrame) : m_fAimLength(fLength),m_fAdjustAddSpeed(fAdjustAddSpeed),
m_nChengeLengthFrame(nChengeFrame),m_bNowAddLength(true),m_nChengeFrameCnt(0),m_bStartReturnLength(false)
{

}
//===================================================================================================================================================

//=================================================================
//デストラクタ
//=================================================================
CCameraLengthState_Gradually::~CCameraLengthState_Gradually()
{

}
//===================================================================================================================================================

//=================================================================
//処理
//=================================================================
void CCameraLengthState_Gradually::Process(CCamera* pCamera)
{
	if (m_bNowAddLength == true)
	{
		AddLengthProcess(pCamera);//カメラとの距離を増やす処理
	}
	else
	{
		MaintainLengthFrameCntProcess();//距離を維持する時間をカウントする処理
	}
	if (m_bStartReturnLength == true)
	{
		ReturnLengthProcess(pCamera);//カメラとの距離と状態を戻す処理
	}
}
//===================================================================================================================================================

//=================================================================
//カメラとの距離を増やす処理
//=================================================================
void CCameraLengthState_Gradually::AddLengthProcess(CCamera* pCamera)
{
	float fLengthDiff = m_fAimLength - pCamera->GetLength();
	float fAddLength = fLengthDiff * m_fAdjustAddSpeed;//加算する距離を求める
	pCamera->SetLength(pCamera->GetLength() + fAddLength);//距離を加算する

	if (fLengthDiff < 1.0f)
	{
		m_bNowAddLength = false;//距離を増やす状態をオフにする
	}
}
//===================================================================================================================================================

//=================================================================
//カメラとの距離を維持する処理
//=================================================================
void CCameraLengthState_Gradually::MaintainLengthFrameCntProcess()
{
	m_nChengeFrameCnt++;

	if (m_nChengeFrameCnt > m_nChengeLengthFrame)
	{//距離を維持する時間を超えたら
		m_bStartReturnLength = true;
	}
}
//===================================================================================================================================================

//=================================================================
//カメラの距離を元に戻す処理
//=================================================================
void CCameraLengthState_Gradually::ReturnLengthProcess(CCamera* pCamera)
{
	float fLengthDiff = CCamera::GetInitialLength() - pCamera->GetLength();
	float fSubLength = fLengthDiff * 0.1f;//加算する距離を求める
	pCamera->SetLength(pCamera->GetLength() + fSubLength);//距離を加算する

	if (fabsf(fLengthDiff) < 1.0f)//距離が絶対値で1.0fより下回ったら状態を戻す
	{//距離が元に戻ったので状態を解除
		pCamera->ChengeLengthState(DBG_NEW CCameraLengthState());
	}
}
//===================================================================================================================================================