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
#include "boss.h"
#include "input.h"
#include "object.h"
#include "game.h"
#include "edit.h"
//====================================================================================================

//====================================================================
//静的メンバ宣言
//====================================================================
const float CCamera::m_BESIDECAMERALENGTH = 570.0f;//ビサイドビューのカメラの距離
//====================================================================================================

//====================================================================
//コンストラクタ
//====================================================================
CCamera::CCamera() : m_SupportPos(NULL_VECTOR3),m_fLength(0.0f), m_fTurningRotSpeed(0.0f),m_fTurningSpeedY(0.0f)
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
	m_PosV = D3DXVECTOR3(0.0f, 200.0f,-400.0f);     //支点
	m_PosR = NULL_VECTOR3;                          //注視点
	m_VecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);         //上方向ベクトル（法線）
	m_Rot = NULL_VECTOR3;                           //向き
	m_mtxProjection = {};                           //プロジェクションマトリックス
	m_mtxView = {};                                 //ビューマトリックス
	m_CameraType = (CAMERATYPE)(1);                 //カメラモードの種類
	m_nShakeFrame = 0;                              //カメラを揺らすフレーム数
	m_fShakePower = 0.0f;                           //カメラを揺らす力
	m_DifferenceLength = NULL_VECTOR3;              //対象との距離
	m_ModeTime = 0;                                 //モードの時間
	m_ZoomSpeed = NULL_VECTOR3;                     //ズームする速さ
	return S_OK;
}
//====================================================================================================

//====================================================================
//終了処理
//====================================================================
void CCamera::Uninit()
{

}
//====================================================================================================


//====================================================================
//更新処理
//====================================================================
void CCamera::Update()
{
	//========================================
	//カメラの位置を変える
	//========================================
	if (CManager::GetInputKeyboard()->GetPress(DIK_E) == true)
	{
		m_Rot.y += 0.02f;
	}
	else if (CManager::GetInputKeyboard()->GetPress(DIK_Q) == true)
	{
		m_Rot.y -= 0.02f;
	}

	//==========================oo==============
	//カメラを揺らす
	//========================================
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_F1) == true)
	{
		SetShake(50, 0.2f);
	}

	//カメラの通常の注視点を設定し続ける
	NormalCameraSetPosR();

	//========================================
	//カメラモードによって視点を変える
	//========================================
#ifdef _DEBUG
	//if (CManager::GetInputKeyboard()->GetTrigger(DIK_Z) == true)
	//{
	//	m_ModeTime = 0;
	//	//カメラタイプの種類を変える
	//	if (m_CameraType == CAMERATYPE_BIRD)
	//	{
	//		m_CameraType = CAMERATYPE_BESIDE;
	//	}
	//	else if (m_CameraType == CAMERATYPE_BESIDE)
	//	{
	//		m_CameraType = CAMERATYPE_SIDE;
	//	}
	//	else if (m_CameraType == CAMERATYPE_SIDE)
	//	{
	//		m_CameraType = CAMERATYPE_BIRD;
	//	}
	//}
#endif // _DEBUG

	if (m_CameraType == CAMERATYPE_BESIDE)
	{
		m_PosV = D3DXVECTOR3(sinf(m_Rot.y) * -m_BESIDECAMERALENGTH + m_PosR.x, m_PosR.y + 50.0f, cosf(m_Rot.y) * -m_BESIDECAMERALENGTH + m_PosR.z);
		//m_PosV = D3DXVECTOR3(-400.0f,)
	}
	else if (m_CameraType == CAMERATYPE_BOSSBATTLE)
	{
		m_Rot.y = 0.0f;
		m_PosV = D3DXVECTOR3(sinf(m_Rot.y) * -m_BESIDECAMERALENGTH + m_PosR.x, m_PosR.y + 50.0f, cosf(m_Rot.y) * -m_BESIDECAMERALENGTH + m_PosR.z);
	}
	else if (m_CameraType == CAMERATYPE_SIDE)
	{
		m_PosV = D3DXVECTOR3(sinf(m_Rot.y) * -100.0f + m_PosR.x, m_PosR.y, cosf(m_Rot.y) * -300.0f + m_PosR.z);
	}
	else if (m_CameraType == CAMERATYPE_BOSSDEFEAT)
	{
		BossDefeatCameraProcess();
	}
	else if (m_CameraType == CAMERATYPE_TURNINGXZ)
	{
		TurningCameraProcess();
	}
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

	if (m_CameraType == CAMERATYPE_BIRD || m_CameraType == CAMERATYPE_BESIDE || m_CameraType == CAMERATYPE_BOSSDEFEAT || m_CameraType == CAMERATYPE_BOSSBATTLE ||
		m_CameraType == CAMERATYPE_TURNINGXZ)
	{
		//プロジェクションマトリックスを作成z
		D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
			D3DXToRadian(45.0f),
			(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
			10.0f,
			20000.0f);
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
	D3DXVECTOR3 adjust = NULL_VECTOR3;//揺れの補正
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
//普通のカメラの位置を設定し続ける
//====================================================================
void CCamera::NormalCameraSetPosR()
{
	D3DXVECTOR3 EditPos = NULL_VECTOR3;

	////===================================================
 //   //カメラの種類によって、注視点の位置を変える
 //   //===================================================
	//if (CManager::GetCamera()->GetCameraType() == CCamera::CAMERATYPE_BIRD)
	//{
	//	CManager::GetCamera()->SetPosR(D3DXVECTOR3(GetPos().x, 0.0f, GetPos().z));
	//}
	//else if (CManager::GetCamera()->GetCameraType() == CCamera::CAMERATYPE_BESIDE)
	//{
	//	CManager::GetCamera()->SetPosR(D3DXVECTOR3(GetPos().x, 150.0f, GetPos().z));
	//	if (pos.y >= 150.0f)
	//	{
	//		m_bReturnCamera = false;
	//		CManager::GetCamera()->SetPosR(GetPos());
	//	}
	//	else
	//	{
	//		m_bReturnCamera = true;
	//	}
	//}
	//else if (CManager::GetCamera()->GetCameraType() == CCamera::CAMERATYPE_SIDE)
	//{
	//	CManager::GetCamera()->SetPosR(D3DXVECTOR3(GetPos().x, 100.0f, GetPos().z));
	//}
	//========================================================================================================================
	if (CManager::GetCamera()->GetCameraType() == CCamera::CAMERATYPE_BESIDE)
	{
		if (CScene::GetMode() == CScene::MODE_GAME)
		{//ゲームモード
			if (CGame::GetPlayer() != nullptr)
			{
				if (CGame::GetPlayer()->GetPos().y <= 150.0f)
				{//現在いるY座標が150f以下だったら
					m_PosR.x = CGame::GetPlayer()->GetPos().x;
					m_PosR.y = 150.0f;
					m_PosR.z = CGame::GetPlayer()->GetPos().z;
				}
				else
				{//それ以上なら
					m_PosR = CGame::GetPlayer()->GetPos();
				}

				//m_PosR = D3DXVECTOR3(0.0f,150.0f,0.0f);
			}
		}
		else if (CScene::GetMode() == CScene::MODE_EDIT)
		{//エディットモード
			if (CEdit::GetStageManager()->GetFocusType() == CStageManager::FOCUSTYPE_NORMAL)
			{
				EditPos = CEdit::GetStageManager()->GetPos();
			}
			else
			{
				EditPos = CEdit::GetStageManager()->GetWarpPos();
			}

			if (EditPos.y <= 150.0f)
			{
				m_PosR.y = 150.0f;
			}
			else
			{
				m_PosR.y = EditPos.y;
			}
			m_PosR.x = EditPos.x;
		}
	}
	else if (m_CameraType == CAMERATYPE_BOSSBATTLE)
	{
		m_PosR = D3DXVECTOR3(0.0f,150.0f,0.0f);
	}
}
//====================================================================================================

//====================================================================
//ボスを倒したときのカメラの処理を行う
//====================================================================
void CCamera::BossDefeatCameraProcess()
{
	float fVXaim = 0.0f;//X方向ベクトル
	float fVYaim = 0.0f;//Y方向ベクトル
	float fVZaim = 0.0f;//Z方向ベクトル
	float fVLaim = 0.0f;//総合ベクトル
	float fLength = 0.0f;
	CObject* pObj = nullptr;
	D3DXVECTOR3 ComparisonPos = NULL_VECTOR3;

	m_ModeTime++;
	//m_PosR = D3DXVECTOR3(0.0f,100.0f,0.0f);
	//==================================
	//アイテムの当たり判定
	//==================================
	for (int nCntPriority = 0; nCntPriority < CObject::m_nMAXPRIORITY; nCntPriority++)
	{
		pObj = CObject::GetTopObject(nCntPriority);//トップオブジェクトを取得
		while (pObj != nullptr)
		{
			//次のオブジェクトを格納
			CObject* pNext = pObj->GetNextObject();

			//種類の取得（ボスならカメラズーム）
			CObject::TYPE type = pObj->GetType();

			if (type == CObject::TYPE_BOSS)
			{
				ComparisonPos = ((CBoss*)pObj)->GetPos();
				if (m_ModeTime == 1)
				{
					m_DifferenceLength = ComparisonPos - m_PosV;
					m_ZoomSpeed;


					//==========================
					//３次元ベクトルを取る
					//==========================
					fVXaim = ComparisonPos.x - m_PosV.x;
					fVYaim = ComparisonPos.y - m_PosV.y;
					fVZaim = ComparisonPos.z - m_PosV.z;



					fVLaim = sqrtf((fVXaim * fVXaim) + (fVYaim * fVYaim)
						+ (fVZaim * fVZaim));

					fLength = fVLaim / CBoss::m_nMAX_DEFEATSTAGINGTIME;
					m_ZoomSpeed.x = fVXaim / fVLaim * fLength;
					m_ZoomSpeed.y = fVYaim / fVLaim * fLength;
					m_ZoomSpeed.z = fVZaim / fVLaim * fLength;

				}

				m_PosR = ComparisonPos;//ボスに注視点を固定
				m_PosV += m_ZoomSpeed;
				break;
			}
			//リストを次に進める
			pObj = pNext;
		}
	}
}
//====================================================================================================

//====================================================================
//カメラの旋回処理を行う
//====================================================================
void CCamera::TurningCameraProcess()
{
	//旋回向き
	m_Rot.y += m_fTurningRotSpeed;

	//支点Yの位置
	m_PosV.y += m_fTurningSpeedY;

	//距離を更新する
	m_fLength += m_fAddLength;

	//カメラの旋回処理
	m_PosV.x = sinf(m_Rot.y) * m_fLength + m_PosR.x;
	m_PosV.z = cosf(m_Rot.y) * m_fLength + m_PosR.z;
}
//====================================================================================================