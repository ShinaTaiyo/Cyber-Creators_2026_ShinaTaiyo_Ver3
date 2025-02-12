//======================================================================================================================================
//
//９月１５日：ボス登場演出を作る[eventmanager.cpp]
//Author:ShinaTaiyo
//
//======================================================================================================================================

//=====================================================================
//インクルード
//=====================================================================
#include "eventmanager.h"
#include "camera.h"
#include "boss.h"
#include "renderer.h"
#include "object.h"
#include "sound.h"
#include "fade.h"
#include "debugtext.h"
#include "manager.h"
#include "calculation.h"
#include "particle2d.h"
#include "ui.h"
//======================================================================================================================================

//=====================================================================
//コンストラクタ
//=====================================================================
CEventManager::CEventManager() : m_nEventPattern(0),m_nCntEventTime(0),m_NowEventType(EVENTTYPE_NONE),m_bEventProgress(false),
m_pMagicCircle(nullptr),m_pBossBattleText(nullptr),m_pDirectionFade(nullptr), m_pBossCutIn(nullptr),m_pBossNameUi(nullptr),
m_BossAppearType(BOSSAPPEAR_KINGSLIME)
{

}
//======================================================================================================================================

//=====================================================================
//デストラクタ
//=====================================================================
CEventManager::~CEventManager()
{

}
//======================================================================================================================================

//=====================================================================
//初期化処理
//=====================================================================
HRESULT CEventManager::Init()
{
	CObject::Init();
	return S_OK;
}
//======================================================================================================================================

//=====================================================================
//終了処理
//=====================================================================
void CEventManager::Uninit()
{
	CObject::Uninit();
}
//======================================================================================================================================

//=====================================================================
//更新処理
//=====================================================================
void CEventManager::Update()
{
	
	if (m_bEventProgress == true)
	{
		m_nCntEventTime++;
		switch (m_NowEventType)
		{
		case EVENTTYPE_BOSSAPPEAR:
			BossAppearEventProcess();
			break;
		default:
			break;
		}
	}

	CObject::Update();
}
//======================================================================================================================================

//=====================================================================
//描画処理
//=====================================================================
void CEventManager::Draw()
{
	CObject::Draw();
}
//======================================================================================================================================

//=====================================================================
//死亡フラグ設定処理
//=====================================================================
void CEventManager::SetDeath()
{

	CObject::SetDeath();
}
//======================================================================================================================================

//=====================================================================
//ボス登場イベントの処理
//=====================================================================
void CEventManager::BossAppearEventProcess()
{
	CObject* pObj = nullptr;
	CBoss* pBoss = nullptr;
	D3DXVECTOR3 BossPos = NULL_VECTOR3;
	CCamera* pCamera = CManager::GetCamera();//カメラへのポインタ

	float fCameraLength = 0.0f;    //カメラの注視点旋回距離
	float fCemeraSpeedY = 0.0f;    //カメラのY方向の移動量
	float fCameraAddLength = 0.0f; //カメラと注視点の距離の加算量
	float fCameraRotSpeed = 0.0f;  //カメラの回転速度

	float fRandRot = 0.0f;//乱数向き
	switch (m_nEventPattern)
	{
	case 0:
		if (m_nCntEventTime == 1)
		{
			CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_WARNING);
			m_pDirectionFade = CDirectionFade::Create();
			m_pDirectionFade->SetFade(CFade::FADEMODE_IN);
			m_pMagicCircle = CUi::Create(CUi::UITYPE_MAGICCIRCLE, CObject2D::POLYGONTYPE01_SENTERROLLING, 700.0f, 700.0f,
				SENTER_VECTOR3, NULL_VECTOR3, NULL_VECTOR3, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));//魔法陣を生成
			m_pMagicCircle->SetPolygonRotSpeed(0.1f);//魔法陣を回転させる
			m_pMagicCircle->SetUseBlinking(true, 30, 0.0f);//点滅させる
			m_pMagicCircle->SetUseDeath(false);//死亡フラグを発動させない

			m_pBossBattleText = CUi::Create(CUi::UITYPE_BOSSBATTLE_TEXT, CObject2D::POLYGONTYPE01_SENTERROLLING, 800.0f, 200.0f,
				D3DXVECTOR3(SCREEN_WIDTH + 300.0f, SENTER_VECTOR3.y, 0.0f),D3DXVECTOR3(-20.0f,0.0f,0.0f), NULL_VECTOR3, NORMAL_COL);
			m_pBossBattleText->SetPolygonRotSpeed(-0.4f);
			m_pBossBattleText->SetUseUiEffect(true, 10, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		}

		if (m_nCntEventTime < 120)
		{
			fRandRot = CCalculation::CalculationRandVecXY();
			CParticle2D::Create(SENTER_VECTOR3, D3DXVECTOR3(sinf(fRandRot) * 20.0f, cosf(fRandRot) * 20.0f, 0.0f), NULL_VECTOR3, CObject2D::POLYGONTYPE01_SENTERROLLING,
				60, 50.0f, 50.0f, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			if (m_pBossBattleText->GetPos().x >= SENTER_VECTOR3.x - 20.0f && m_pBossBattleText->GetPos().x <= SENTER_VECTOR3.x + 20.0f)
			{//中心にたどり着いたらゆっくり動かす
				m_pBossBattleText->SetPolygonRotSpeed(-0.01f);
				m_pBossBattleText->SetMove(D3DXVECTOR3(-0.1f, 0.0f, 0.0f));
			}
		}
		if (m_nCntEventTime == 120)
		{
			//再び動かす
			m_pBossBattleText->SetPolygonRotSpeed(-0.4f);
			m_pBossBattleText->SetMove(D3DXVECTOR3(-20.0f, 0.0f, 0.0f));

			m_pDirectionFade->SetFade(CFade::FADEMODE_OUT);
			if (m_pMagicCircle != nullptr)
			{
				m_pMagicCircle->SetUseDeath(true);
				m_pMagicCircle->SetDeath();
				m_pMagicCircle = nullptr;
			}
		}

		if (m_nCntEventTime == 180)
		{
			CManager::GetSound()->StopSound(CSound::SOUND_LABEL_SE_WARNING);
			NextPattern();
		}
		break;
	case 1:
		if (m_nCntEventTime == 1)
		{
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
						pBoss = (CBoss*)pObj;

						BossPos = pBoss->GetPos();//ボスの中心点を取得する
						break;
					}
					//リストを次に進める
					pObj = pNext;
				}
			}

			CManager::GetCamera()->SetCameraType(CCamera::CAMERATYPE_TURNINGXZ);
			CManager::GetCamera()->SetTurningInfo(1000.0f, -0.001f,BossPos,BossPos.y + 1500.0f, -0.01f, 0.1f);
		}

		if (m_nCntEventTime == 60)
		{
			NextPattern();//パターンを次に進める
		}
		break;
	case 2:
		fCemeraSpeedY = pCamera->GetTurningSpeedY();
		fCemeraSpeedY -= 0.01f * m_nCntEventTime;
		pCamera->SetTurningSpeedY(fCemeraSpeedY);//Yの移動量を加速させる

		pCamera->SetAddLength(-7.0f);//注視点との距離を離していく

		fCameraRotSpeed = pCamera->GetTurningRotSpeed();
		if (m_nCntEventTime % 3 == 0)
		{
			fCameraRotSpeed += 0.007f;
			pCamera->SetTurningRotSpeed(fCameraRotSpeed);
		}

		if (m_nCntEventTime == 89)
		{
			NextPattern();
		}
		break;
	case 3:
		pCamera->SetTurningSpeedY(-0.3f);
		pCamera->SetTurningRotSpeed(0.002f);
		pCamera->SetAddLength(-0.02f);

		if (m_nCntEventTime == 60)
		{
			NextPattern();
		}
		break;
	case 4:
		if (m_nCntEventTime == 1)
		{
			switch (m_BossAppearType)
			{
			case BOSSAPPEAR_KINGSLIME:
				m_pBossCutIn = CUi::Create(CUi::UITYPE_CUTIN_KINGSLIME, CObject2D::POLYGONTYPE01_SENTERROLLING, 500.0f, 500.0f,
					D3DXVECTOR3(0.0f - 250.0f, SENTER_VECTOR3.y + 200.0f, 0.0f), D3DXVECTOR3(30.0f, 0.0f, 0.0f), NULL_VECTOR3, NORMAL_COL);

				m_pBossNameUi = CUi::Create(CUi::UITYPE_KINGSLIME_TEXT, CObject2D::POLYGONTYPE01_SENTERROLLING, 1000.0f, 250.0f,
					D3DXVECTOR3(SCREEN_WIDTH + 500.0f, SENTER_VECTOR3.y - 200.0f, 0.0f), D3DXVECTOR3(-30.0f, 0.0f, 0.0f), NULL_VECTOR3, NORMAL_COL);
				break;
			case BOSSAPPEAR_DEATHENAGA:
				m_pBossCutIn = CUi::Create(CUi::UITYPE_CUTIN_DEATHENAGA, CObject2D::POLYGONTYPE01_SENTERROLLING, 500.0f, 500.0f,
					D3DXVECTOR3(0.0f - 250.0f, SENTER_VECTOR3.y + 200.0f, 0.0f), D3DXVECTOR3(30.0f, 0.0f, 0.0f), NULL_VECTOR3, NORMAL_COL);

				m_pBossNameUi = CUi::Create(CUi::UITYPE_DEATHENAGA_TEXT, CObject2D::POLYGONTYPE01_SENTERROLLING, 1000.0f, 250.0f,
					D3DXVECTOR3(SCREEN_WIDTH + 500.0f, SENTER_VECTOR3.y - 200.0f, 0.0f), D3DXVECTOR3(-30.0f, 0.0f, 0.0f), NULL_VECTOR3, NORMAL_COL);
				break;
			case BOSSAPPEAR_MRSTRONG:
				m_pBossCutIn = CUi::Create(CUi::UITYPE_CUTIN_MRSTRONG, CObject2D::POLYGONTYPE01_SENTERROLLING, 500.0f, 500.0f,
					D3DXVECTOR3(0.0f - 250.0f, SENTER_VECTOR3.y + 200.0f, 0.0f), D3DXVECTOR3(30.0f, 0.0f, 0.0f), NULL_VECTOR3, NORMAL_COL);

				m_pBossNameUi = CUi::Create(CUi::UITYPE_MRSTRONG_TEXT, CObject2D::POLYGONTYPE01_SENTERROLLING, 1000.0f, 250.0f,
					D3DXVECTOR3(SCREEN_WIDTH + 500.0f, SENTER_VECTOR3.y - 200.0f, 0.0f), D3DXVECTOR3(-30.0f, 0.0f, 0.0f), NULL_VECTOR3, NORMAL_COL);
				break;
			case BOSSAPPEAR_RILEY:
				m_pBossCutIn = CUi::Create(CUi::UITYPE_CUTIN_RILEY, CObject2D::POLYGONTYPE01_SENTERROLLING, 500.0f, 500.0f,
					D3DXVECTOR3(0.0f - 250.0f, SENTER_VECTOR3.y + 200.0f, 0.0f), D3DXVECTOR3(30.0f, 0.0f, 0.0f), NULL_VECTOR3, NORMAL_COL);

				m_pBossNameUi = CUi::Create(CUi::UITYPE_RILEY_TEXT, CObject2D::POLYGONTYPE01_SENTERROLLING, 1000.0f, 250.0f,
					D3DXVECTOR3(SCREEN_WIDTH + 500.0f, SENTER_VECTOR3.y - 200.0f, 0.0f), D3DXVECTOR3(-30.0f, 0.0f, 0.0f), NULL_VECTOR3, NORMAL_COL);
				break;
			case BOSSAPPEAR_DRAIYA:
				m_pBossCutIn = CUi::Create(CUi::UITYPE_CUTIN_DRAIYA, CObject2D::POLYGONTYPE01_SENTERROLLING, 500.0f, 500.0f,
					D3DXVECTOR3(0.0f - 250.0f, SENTER_VECTOR3.y + 200.0f, 0.0f), D3DXVECTOR3(30.0f, 0.0f, 0.0f), NULL_VECTOR3, NORMAL_COL);

				m_pBossNameUi = CUi::Create(CUi::UITYPE_DRAIYA_TEXT, CObject2D::POLYGONTYPE01_SENTERROLLING, 1000.0f, 250.0f,
					D3DXVECTOR3(SCREEN_WIDTH + 500.0f, SENTER_VECTOR3.y - 200.0f, 0.0f), D3DXVECTOR3(-30.0f, 0.0f, 0.0f), NULL_VECTOR3, NORMAL_COL);
				break;
			case BOSSAPPEAR_GABRIEL:
				m_pBossCutIn = CUi::Create(CUi::UITYPE_CUTIN_GABRIEL, CObject2D::POLYGONTYPE01_SENTERROLLING, 500.0f, 500.0f,
					D3DXVECTOR3(0.0f - 250.0f, SENTER_VECTOR3.y + 200.0f, 0.0f), D3DXVECTOR3(30.0f, 0.0f, 0.0f), NULL_VECTOR3, NORMAL_COL);

				m_pBossNameUi = CUi::Create(CUi::UITYPE_GABRIEL_TEXT, CObject2D::POLYGONTYPE01_SENTERROLLING, 1000.0f, 250.0f,
					D3DXVECTOR3(SCREEN_WIDTH + 500.0f, SENTER_VECTOR3.y - 200.0f, 0.0f), D3DXVECTOR3(-30.0f, 0.0f, 0.0f), NULL_VECTOR3, NORMAL_COL);
				break;
			default:
				break;
			}
		}

		if (m_pBossCutIn->GetPos().x >= SENTER_VECTOR3.x + 200.0f - 20.0f && m_pBossCutIn->GetPos().x <= SENTER_VECTOR3.x + 200.0f + 20.0f)
		{
			m_pBossCutIn->SetUseUiEffect(true, 90, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			m_pBossCutIn->SetMove(D3DXVECTOR3(1.0f, 0.0f, 0.0f));

			m_pBossNameUi->SetMove(D3DXVECTOR3(-1.0f, 0.0f, 0.0f));
			NextPattern();
		}
		break;
	case 5:
		if (m_nCntEventTime == 120)
		{
			m_pBossCutIn->SetMove(D3DXVECTOR3(30.0f, 0.0f, 0.0f));
			m_pBossNameUi->SetMove(D3DXVECTOR3(-30.0f, 0.0f, 0.0f));
			NextPattern();
		}
		break;
	case 6:
		if (m_nCntEventTime == 60)
		{//イベント終了
			ResetPattern();
			CManager::GetCamera()->SetCameraType(CCamera::CAMERATYPE_BOSSBATTLE);

			SetDeathOnly();//本体オブジェクト以外を消去
		}
		break;
	default:
		break;
	}

	CManager::GetDebugText()->PrintDebugText("現在のパターン：%d、現在のパターン時間：%d\n", m_nEventPattern, m_nCntEventTime);

}
//======================================================================================================================================

//=====================================================================
//本体以外のオブジェクトを全て消去する
//=====================================================================
void CEventManager::SetDeathOnly()
{
	if (m_pMagicCircle != nullptr)
	{
		m_pMagicCircle->SetUseDeath(true);
		m_pMagicCircle->SetDeath();
		m_pMagicCircle = nullptr;
	}

	if (m_pDirectionFade != nullptr)
	{
		m_pDirectionFade->SetUseDeath(true);
		m_pDirectionFade->SetDeath();
		m_pDirectionFade = nullptr;
	}

	if (m_pBossBattleText != nullptr)
	{
		m_pBossBattleText->SetUseDeath(true);
		m_pBossBattleText->SetDeath();
		m_pBossBattleText = nullptr;
	}

	if (m_pBossCutIn != nullptr)
	{
		m_pBossCutIn->SetUseDeath(true);
		m_pBossCutIn->SetDeath();
		m_pBossCutIn = nullptr;
	}

	if (m_pBossNameUi != nullptr)
	{
		m_pBossNameUi->SetUseDeath(true);
		m_pBossNameUi->SetDeath();
		m_pBossNameUi = nullptr;
	}
}
//======================================================================================================================================

//=====================================================================
//パターンを次に進める
//=====================================================================
void CEventManager::NextPattern()
{
	m_nCntEventTime = 0;
	m_nEventPattern++;
}
//======================================================================================================================================

//=====================================================================
//パターンをリセットする
//=====================================================================
void CEventManager::ResetPattern()
{
	m_nCntEventTime = 0;
	m_nEventPattern = 0;
	m_bEventProgress = false;
	m_NowEventType = EVENTTYPE_NONE;
}
//======================================================================================================================================

//=====================================================================
//イベントの生成
//=====================================================================
CEventManager* CEventManager::Create(EVENTTYPE type)
{
	CEventManager* pEventManager = DBG_NEW CEventManager;

	CEventManager::Init();//初期化処理
	m_NowEventType = type;//イベントの種類

	return pEventManager;
}
//======================================================================================================================================