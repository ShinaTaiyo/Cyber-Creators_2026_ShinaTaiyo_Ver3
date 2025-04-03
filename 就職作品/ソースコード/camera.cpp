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
bool CCamera::s_bCAMERACONTROLLMOUSE = false;      //デバッグ時にカメラをマウスで動かすかどうか
int CCamera::s_nSENSITIVITYLEVEL = 10;              //カメラの感度レベル
//====================================================================================================

//====================================================================
//コンストラクタ
//====================================================================
CCamera::CCamera() : m_SupportPos(D3DXVECTOR3(0.0f,0.0f,0.0f)),m_fLength(s_fINITIAL_LENGTH), m_fTurningRotSpeed(0.0f),m_fTurningSpeedY(0.0f),m_PosV(D3DXVECTOR3(0.0f,0.0f,0.0f)),
m_PosR(D3DXVECTOR3(0.0f,0.0f,0.0f)),m_VecU(D3DXVECTOR3(0.0f,0.0f,0.0f)),m_Rot(D3DXVECTOR3(0.0f,0.0f,0.0f)),m_mtxProjection(),m_mtxView(),m_CameraType(CAMERATYPE_BIRD),m_DifferenceLength(D3DXVECTOR3(0.0f,0.0f,0.0f)),
m_ZoomSpeed(D3DXVECTOR3(0.0f,0.0f,0.0f)),m_nShakeFrame(0),m_ModeTime(0),m_fShakePower(0.0f),m_fAddLength(0.0f),m_AddPosR(D3DXVECTOR3(0.0f,0.0f,0.0f)),m_AddPosV(D3DXVECTOR3(0.0f,0.0f,0.0f))
,m_State(CAMERASTATE::NORMAL),m_pCameraState(DBG_NEW CCameraState_Normal()),m_pCameraLengthState(DBG_NEW CCameraLengthState())
{
#ifdef _DEBUG
	if (s_bDEBUGCAMERACONTROLLMOUSE)
	{//デバッグ時はカメラをマウスで操作するかどうかを決めることができいる
		s_bCAMERACONTROLLMOUSE = true;
	}
	else
	{//リリース時は必ずカメラをマウスで操作できるようにする
		s_bCAMERACONTROLLMOUSE = false;
	}
#else
	//必ずカメラをマウスで操作できるようにする
	s_bCAMERACONTROLLMOUSE = true;
#endif // _DEBUG

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
	m_PosV = D3DXVECTOR3(0.0f, 200.0f,-350.0f);       //支点
	m_PosR = D3DXVECTOR3(0.0f,0.0f,0.0f);             //注視点
	m_VecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);           //上方向ベクトル（法線）
	m_Rot = D3DXVECTOR3(-D3DX_PI * 0.5f,0.0f,0.0f);   //向き
	m_mtxProjection = {};                             //プロジェクションマトリックス
	m_mtxView = {};                                   //ビューマトリックス
	m_CameraType = CAMERATYPE_BIRD;                   //カメラモードの種類
	m_nShakeFrame = 0;                                //カメラを揺らすフレーム数
	m_fShakePower = 0.0f;                             //カメラを揺らす力
	m_DifferenceLength = D3DXVECTOR3(0.0f,0.0f,0.0f); //対象との距離
	m_ModeTime = 0;                                   //モードの時間
	m_ZoomSpeed = D3DXVECTOR3(0.0f,0.0f,0.0f);        //ズームする速さ

	fstream ReadingFile;//ファイル
	string ReadingBuff; //文字
	ReadingFile.open("data\\TEXTFILE\\CameraSensitivity.txt");//カメラ感度調整のファイルを読み込む

	if (ReadingFile.good())
	{//ファイルが正常に開けていたら
		while (ReadingFile >> ReadingBuff)
		{//文字列の読み込みが失敗するまで
			if (ReadingBuff == "CAMERASENSITIVITYLEVEL")
			{
				ReadingFile >> ReadingBuff;//イコール
				ReadingFile >> s_nSENSITIVITYLEVEL;//感度レベルを読み込む
			}
		}
	}
	ReadingFile.close();//ファイルを閉じる
	return S_OK;
}
//====================================================================================================

//====================================================================
//終了処理
//====================================================================
void CCamera::Uninit()
{
	if (m_pCameraState != nullptr)
	{//カメラの状態機械を破棄
		delete m_pCameraState;
		m_pCameraState = nullptr;
	}

	if (m_pCameraLengthState != nullptr)
	{//カメラの距離の状態機械を破棄
		delete m_pCameraLengthState;
		m_pCameraLengthState = nullptr;
	}

	fstream WritingFile;//書き出し用ファイル
	WritingFile.open("data\\TEXTFILE\\CameraSensitivity.txt", ios::out);

	if (WritingFile.good())
	{//ファイルを正常に開けたら
		WritingFile << "CAMERASENSITIVITYLEVEL = " << s_nSENSITIVITYLEVEL << endl;//カメラの感度レベルを書き出す
	}

	WritingFile.close();//ファイルを閉じる
}
//====================================================================================================


//====================================================================
//更新処理
//====================================================================
void CCamera::Update()
{
	m_pCameraState->Process(this);      //カメラの角度に関する処理
	m_pCameraLengthState->Process(this);//カメラの距離に関する処理
	CInputKeyboard* pInputKeyboard = CManager::GetInputKeyboard();//キー入力情報
	CDebugText* pDebugText = CManager::GetDebugText();//デバッグ情報

	pDebugText->PrintDebugText("カメラをマウスで操作するかどうか（Lシフトを押しながらM): %d\n", s_bCAMERACONTROLLMOUSE);

	if (pInputKeyboard->GetPress(DIK_LSHIFT))
	{
		if (pInputKeyboard->GetTrigger(DIK_M))
		{
			s_bCAMERACONTROLLMOUSE = s_bCAMERACONTROLLMOUSE ? false : true;//カメラをマウスで操作するかのフラグのONOFFを変える
		}
	}

	//Pitchは前側に範囲を制限
	if (m_Rot.x < -D3DX_PI + 0.01f)
	{
		m_Rot.x = -D3DX_PI + 0.01f;
	}
	if (m_Rot.x > -0.01f)
	{
		m_Rot.x = -0.01f;
	}

	

	//ジンバルロックを回避する
	m_Rot.x = CCalculation::CorrectionRot(m_Rot.x);
	m_Rot.y = CCalculation::CorrectionRot(m_Rot.y);
	m_Rot.z = CCalculation::CorrectionRot(m_Rot.z);

	//===============================================================================

	//デバッグ表示
	CManager::GetDebugText()->PrintDebugText("カメラの向き：%f %f %f\n", m_Rot.x, m_Rot.y, m_Rot.z);
	CManager::GetDebugText()->PrintDebugText("カメラの視点：%f %f %f\n", m_PosV.x, m_PosV.y, m_PosV.z);
	CManager::GetDebugText()->PrintDebugText("カメラの注視点：%f %f %f\n", m_PosR.x, m_PosR.y, m_PosR.z);
	CManager::GetDebugText()->PrintDebugText("カメラとの距離：%f\n",m_fAddLength);

	//カメラの通常の注視点を設定し続ける
	NormalCameraMove();

	if (CScene::GetMode() == CScene::MODE_GAME)
	{//シーンがゲームの時のみ行う処理
		if (m_PosV.y < 0.0f)
		{//視点が０以下なら全ての影を描画しない
			CObjectX::SetCommonDraw(false);
		}
		else
		{//それ以外なら全ての影を描画
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
		// プロジェクションマトリックスを作成（透視投影行列）→　遠くのオブジェクトを小さく見せる
		//１：行列、２：視野角、３：アスペクト比、４：近クリップ面、５：遠クリップ面
		//  近クリップ面：０、遠クリップ面：１
		// クリップ空間より外にあるものは描画しないようにしたり、トリミングが行われ、
		//描画コストが減る
		D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
			D3DXToRadian(45.0f),
			(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
			10.0f,
			30000.0f);
	}
	else if (m_CameraType == CAMERATYPE_SIDE)
	{
		//平行投影（正射影）
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
		m_nShakeFrame--;//設定されたカメラを揺らすフレーム数分揺らす

		//カメラを揺らす力が大きいほど強く揺らす
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
	m_nShakeFrame = nShakeFrame;//カメラを揺らすフレーム数
	m_fShakePower = fShakePower;//カメラを揺らす力
}
//====================================================================================================

//====================================================================
//カメラの状態を変える
//====================================================================
void CCamera::ChengeState(CCameraState* pCameraState)
{
	delete m_pCameraState;//カメラの状態を破棄
	m_pCameraState = pCameraState;//新しいカメラの状態
}
//====================================================================================================

//====================================================================
//カメラの距離の状態を変える
//====================================================================
void CCamera::ChengeLengthState(CCameraLengthState* pCameraLengthState)
{
	delete m_pCameraLengthState;//カメラの距離の状態を破棄
	m_pCameraLengthState = pCameraLengthState;//新しいカメラの距離の状態
}
//====================================================================================================

//====================================================================
//普通のカメラの位置を設定し続ける
//====================================================================
void CCamera::NormalCameraMove()
{
	D3DXVECTOR3 RotVec = CCalculation::RadToVec(m_Rot);
	switch (CScene::GetMode())
	{
	case CScene::MODE_TITLE://シーンがタイトルなら
		m_PosR = CTitle::GetPlayer()->GetPosInfo().GetPos() + D3DXVECTOR3(100.0f,75.0f,0.0f);//プレイヤーを左側に設定するためにカメラをプレイヤーの右側に設定
		m_PosV = m_PosR + RotVec * m_fLength;//注視点からの向きのベクトルに距離をかけて視点を設定
		break;
	case CScene::MODE_GAME://通常はプレイヤーの位置を軸に動く(カメラの向きを使い旋回する）
		if (CGame::GetPlayer() != nullptr)
		{
			m_PosR = CGame::GetPlayer()->GetPosInfo().GetPos() + D3DXVECTOR3(sinf(m_Rot.y + D3DX_PI * 0.5f) * 30.0f, 60.0f,cosf(m_Rot.y + D3DX_PI * 0.5f) * 30.0f) + m_AddPosR;//FPS風にカメラから見たキャラクターの位置を左にずらすため値を加算する
			m_PosV = m_PosR + RotVec * m_fLength;//注視点からの向きのベクトルに距離をかけて視点を設定
		}
		break;
	case CScene::MODE_EDIT://シーンがエディットだったら
		m_PosV = m_PosR + RotVec * (m_fLength);//注視点からの向きのベクトルに距離をかけて視点を設定
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
	{//シーンが「エディット」なら
		D3DXVECTOR3 Ray = m_PosR - m_PosV;                          //注視点と支店のベクトルを取り、レイとする
		D3DXVec3Normalize(&Ray, &Ray);                              //上記で求めたレイを正規化する
		D3DXVECTOR3 RayCollisionPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//レイが当たった位置代入用
		float fLength = 0.0f;                                       //レイが当たった場所の距離
		for (int nCntPri = 0; nCntPri < CObject::m_nMAXPRIORITY; nCntPri++)
		{//オブジェクトリストを検索する
			CObject* pObj = CObject::GetTopObject(nCntPri);//各リストの先頭オブジェクトを取得する

			while (pObj != nullptr)
			{//オブジェクトがnullptrになるまで検索する
				CObject* pNext = pObj->GetNextObject();//次のオブジェクトを取得する
				CObject::TYPE Type = pObj->GetType();//オブジェクトの種類を取得する
				if (Type == CObject::TYPE::BGMODEL || Type == CObject::TYPE::BLOCK)
				{
					CObjectX* pObjX = static_cast<CObjectX*>(pObj);//オブジェクトXにキャストする
					if (CCollision::RayIntersectsAABBCollisionPos(m_PosV, Ray, pObjX->GetPosInfo().GetPos() + pObjX->GetSizeInfo().GetVtxMin(), pObjX->GetPosInfo().GetPos() + pObjX->GetSizeInfo().GetVtxMax(), RayCollisionPos))
					{//レイが当たっていたら

						fLength = CCalculation::CalculationLength(m_PosV, RayCollisionPos);//レイが当たった位置と距離を求める

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
	const D3DXVECTOR3 & NowRot = pCamera->GetRot();//カメラの向きを取得する

	//向きの差分を求める
	float fRotDiffX = m_AimRot.x - NowRot.x;
	float fRotDiffY = m_AimRot.y - NowRot.y;

	//向きの差分を補正（ジンバルロックを回避)
	fRotDiffX = CCalculation::CorrectionRot(fRotDiffX);
	fRotDiffY = CCalculation::CorrectionRot(fRotDiffY);

	CManager::GetDebugText()->PrintDebugText("カメラの向きの差分:%f\n");//デバッグ表示

	//加算する向きの量を補正して求める
	float fAddRotX = fRotDiffX * m_fAdjustTurnSpeed;
	float fAddRotY = fRotDiffY * m_fAdjustTurnSpeed;

	//ジンバルロックを回避
	fAddRotX = CCalculation::CorrectionRot(fAddRotX);
	fAddRotY = CCalculation::CorrectionRot(fAddRotY);

	//向きを設定する
	pCamera->SetRot(D3DXVECTOR3(pCamera->GetRot().x + fAddRotX, pCamera->GetRot().y + fAddRotY, pCamera->GetRot().z));

	//差分の絶対値が0.01f以下なら
	if (fabsf(fAddRotX) < 0.005f && fabsf(fAddRotY) < 0.005f)//YawとPitchの向き加算量が0.01f以下になったらステートを戻す
	{//YawもPitchの加算する値がどちらも0.005f以下なら向きが変わり切ったと仮定し、状態を普通に戻す
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
	if (CScene::GetMode() != CScene::MODE_TITLE && CScene::GetMode() != CScene::MODE_RESULT)
	{//シーンがタイトルとリザルト以外なら
		if (CManager::GetInputKeyboard()->GetPress(DIK_Q))
		{//カメラを徐々に右に向かせる
			pCamera->SetRot(pCamera->GetRot() + D3DXVECTOR3(0.0f, -s_fNORMAL_AROUNDROTSPEED, 0.0f));
		}
		if (CManager::GetInputKeyboard()->GetPress(DIK_E))
		{//カメラを徐々に左に向かせる
			if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT))
			{//LSHIFTを押しながらEキーを押すとYawをリセット
				pCamera->SetRot(D3DXVECTOR3(pCamera->GetRot().x, 0.0f, pCamera->GetRot().z));
			}
			else
			{//普通にカメラを徐々に左に向かせる
				pCamera->SetRot(pCamera->GetRot() + D3DXVECTOR3(0.0f, s_fNORMAL_AROUNDROTSPEED, 0.0f));
			}
		}

		if (CManager::GetInputJoypad()->GetPress(CInputJoypad::JOYKEY::LB))
		{//コントローラーのLBボタンを押していたら
			pCamera->SetRot(pCamera->GetRot() + D3DXVECTOR3(0.0f, -s_fNORMAL_AROUNDROTSPEED, 0.0f));
		}
		if (CManager::GetInputJoypad()->GetPress(CInputJoypad::JOYKEY::RB))
		{//コントローラーのRBボタンを押していたら
			pCamera->SetRot(pCamera->GetRot() + D3DXVECTOR3(0.0f, s_fNORMAL_AROUNDROTSPEED, 0.0f));
		}

		//===========================
		//Xボタンを押していたら
		//===========================
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//シフトキーを押しながら・・・
			if (CManager::GetInputKeyboard()->GetPress(DIK_X) == true)
			{//注視点の加算量を減らす
				pCamera->SetAddPosR(pCamera->GetAddPosR() + D3DXVECTOR3(0.0f, -5.0f, 0.0f));
			}
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_X) == true)
		{//注視点の加算量を増やす
			pCamera->SetAddPosR(pCamera->GetAddPosR() + D3DXVECTOR3(0.0f, 5.0f, 0.0f));
		}
		float fMagnification = 1.0f / CCamera::GetMaxSensitivityLevel();//感度倍率を求める
		float fResultSensitivity = 0.0f;//結果的な感度
		//=================================================
		//ゲームパッド、又はマウスでカメラの向きを変える
		//=================================================
		if (CManager::GetInputJoypad()->GetRStickPress(16) == true)
		{//右スティックを入力していたら
			if (CScene::GetMode() == CScene::MODE::MODE_GAME)
			{//シーンが「ゲーム」だったら
				CGame::GetTutorial()->SetSuccessCheck(CTutorial::CHECK::CAMERACONTROLL);
			}
			fResultSensitivity = s_fMAX_STICKSENSITIVITY * (fMagnification * CCamera::GetSensitivityLevel());//感度レベルに応じて調整
			//スティックを押した方向にカメラの向きを向かせる
			CManager::GetCamera()->SetRot(pCamera->GetRot() + D3DXVECTOR3(cosf(CManager::GetInputJoypad()->GetRStickAimRot() + D3DX_PI) * fResultSensitivity,
				sinf(CManager::GetInputJoypad()->GetRStickAimRot()) * fResultSensitivity, 0.0f));
		}

		if (CCamera::GetUseCameraControllMouse())
		{//マウスでもカメラを操作するなら
			float fAngle = 0.0f;    //角度
			float fAddYaw = 0.0f;   //Yaw
			float fAddPitch = 0.0f; //Pitch
			if (CScene::GetMode() == CScene::MODE_GAME || CScene::GetMode() == CScene::MODE_EDIT)
			{//ゲームモードとエディットモードの時だけマウスを固定
				fResultSensitivity = s_fMAX_MOUSESENSITIVITY * (fMagnification * CCamera::GetSensitivityLevel());//感度レベルに応じて調整
				if (CManager::GetInputMouse()->GetMouseMoveAngle(fAddYaw, fAddPitch, fResultSensitivity))
				{//マウスが移動した方向からYawとPitchの加算量を取得する。マウスが動いていたらtrue
					if (CScene::GetMode() == CScene::MODE::MODE_GAME)
					{//シーンが「ゲーム」だったら
						CGame::GetTutorial()->SetSuccessCheck(CTutorial::CHECK::CAMERACONTROLL);//カメラを動かすチュートリアルを完了
					}
					//向きを加算する
					CManager::GetCamera()->SetRot(pCamera->GetRot() + D3DXVECTOR3(fAddPitch,
						fAddYaw, 0.0f));
				}
			}
		}

		//===========================
		//Cボタンを押していたら
		//===========================
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//シフトキーを押しながら・・・
			if (CManager::GetInputKeyboard()->GetPress(DIK_C) == true)
			{//Cキーを教えいたｒ
				pCamera->SetAddPosV(pCamera->GetAddPosV() + D3DXVECTOR3(0.0f, -5.0f, 0.0f));//視点の位置の加算量を減らす
			}
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_C) == true)
		{//シフトキーを押していなくてCキーを押していたら
			pCamera->SetAddPosV(pCamera->GetAddPosV() + D3DXVECTOR3(0.0f, 5.0f, 0.0f));//視点の位置の加算量を増やす
		}

		//===========================
		//Vボタンを押していたら
		//===========================
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//左シフトキーを押しながら・・・
			if (CManager::GetInputKeyboard()->GetPress(DIK_V) == true)
			{//Vキーを押していたら
				pCamera->SetLength(pCamera->GetLength() - 5.0f);//視点と注視点の距離を減らす
			}
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_V) == true)
		{//左シフトキーをおしていなく、Vキーを押していたら
			pCamera->SetLength(pCamera->GetLength() + 5.0f);//視点と注視点の距離を増やす
		}


		//========================================
		//カメラを揺らす
		//========================================
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_F1) == true)
		{//F1キーを押していたら
			pCamera->SetShake(50, 0.2f);//カメラを揺らす
		}

		//========================================
		//後ろを向く
		//========================================
		if (CScene::GetMode() == CScene::MODE::MODE_GAME)
		{//シーンが「ゲーム」だったら
			if (CManager::GetInputJoypad()->GetTrigger(CInputJoypad::JOYKEY::Y) || CManager::GetInputKeyboard()->GetTrigger(DIK_LCONTROL))
			{//コントローラーのYキーを押したかLコントロールキーを押していたら
				CGame::GetTutorial()->SetSuccessCheck(CTutorial::CHECK::TURNAROUND);//後ろを向くチュートリアルを完了

				pCamera->ChengeState(DBG_NEW CCameraState_TurnAround(pCamera->GetRot() + D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), 0.15f));//指定した向きを徐々に向く状態にし、現在の向きの後ろに設定
			}
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
	{//現在距離を加算しているなら
		AddLengthProcess(pCamera);//カメラとの距離を増やす処理
	}
	else
	{//現在距離を加算していないなら
		MaintainLengthFrameCntProcess();//距離を維持する時間をカウントする処理
	}
	if (m_bStartReturnLength == true)
	{//カメラとの距離を戻すなら
		ReturnLengthProcess(pCamera);//カメラとの距離と状態を戻す処理
	}
}
//===================================================================================================================================================

//=================================================================
//カメラとの距離を増やす処理
//=================================================================
void CCameraLengthState_Gradually::AddLengthProcess(CCamera* pCamera)
{
	float fLengthDiff = m_fAimLength - pCamera->GetLength();//目的の距離と現在のカメラの視点と注視点の距離の差分を求める
	float fAddLength = fLengthDiff * m_fAdjustAddSpeed;     //加算する距離を求める
	pCamera->SetLength(pCamera->GetLength() + fAddLength); //距離を加算する

	if (fLengthDiff < 1.0f)
	{//距離の差分が1.0f以下なら
		m_bNowAddLength = false;//距離を増やす状態をオフにする
	}
}
//===================================================================================================================================================

//=================================================================
//カメラとの距離を維持する処理
//=================================================================
void CCameraLengthState_Gradually::MaintainLengthFrameCntProcess()
{
	m_nChengeFrameCnt++;//変えたカメラの距離を維持するフレーム数をカウントする

	if (m_nChengeFrameCnt > m_nChengeLengthFrame)
	{//距離を維持する時間を超えたら
		m_bStartReturnLength = true;//カメラの距離を戻すフラグをtrueにする
	}
}
//===================================================================================================================================================

//=================================================================
//カメラの距離を元に戻す処理
//=================================================================
void CCameraLengthState_Gradually::ReturnLengthProcess(CCamera* pCamera)
{
	float fLengthDiff = CCamera::GetInitialLength() - pCamera->GetLength();//カメラの最初の距離と現在のカメラの距離の差分を求める
	float fSubLength = fLengthDiff * 0.1f;                                 //差分の0.1f倍減らす
	pCamera->SetLength(pCamera->GetLength() + fSubLength);                 //距離を加算する

	if (fabsf(fLengthDiff) < 1.0f)//距離が絶対値で1.0fより下回ったら状態を戻す
	{//距離が元に戻ったので状態を解除
		pCamera->ChengeLengthState(DBG_NEW CCameraLengthState());
	}
}
//===================================================================================================================================================