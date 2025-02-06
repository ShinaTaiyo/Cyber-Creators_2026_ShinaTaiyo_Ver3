//=========================================================================================================================
//
//７月１１日：オブジェクトのRelease方法の変更[game.cpp]
//Author:ShinaTaiyo
//
//=========================================================================================================================

//=============================================================
//インクルード
//=============================================================
#include "game.h"
#include "bg.h"
#include "manager.h"
#include "sound.h"
#include "fade.h"
#include "camera.h"
#include "field.h"
#include "bg3d.h"
#include "enemy.h"
#include "input.h"
#include "attack.h"
#include "calculation.h"
#include "particle.h"
#include "debugtext.h"
#include "wire.h"
//=========================================================================================================================

//=============================================================
//静的メンバ宣言
//=============================================================
CPlayer* CGame::m_pPlayer = nullptr;
CStageManager* CGame::m_pStageManager = nullptr;
CPhaseManager* CGame::m_pPhaseManager = nullptr;
CTutorial* CGame::m_pTutorial = nullptr;
int CGame::s_nPhaseNum = 0;
bool CGame::s_bUseGamePad = false;
//=========================================================================================================================

//=============================================================
//コンストラクタ
//=============================================================
CGame::CGame(bool bUseGamePad)
{
	m_pPlayer = nullptr;
	m_pStageManager = nullptr;
	m_pPhaseManager = nullptr;
	m_pTutorial = nullptr;
	s_nPhaseNum = 0;
	s_bUseGamePad = bUseGamePad;
}
//=========================================================================================================================

//=============================================================
//デストラクタ
//=============================================================
CGame::~CGame()
{

}
//=========================================================================================================================

//=============================================================
//初期化処理
//=============================================================
HRESULT CGame::Init()
{
	CScene::Init();//シーン初期化処理
	CObjectX::SetCommonDraw(true);

	CBg3D::Create(CBg3D::BG3DTYPE::SKY, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1200.0f, 1200.0f, 1200.0f));

	m_pPhaseManager = CPhaseManager::Create();//フェーズマネージャーを生成
	m_pPhaseManager->SetUseDeath(false);

	m_pStageManager = CStageManager::Create();//ステージマネージャーを生成
	m_pStageManager->SetUseDeath(false);

	m_pStageManager->LoadMapTxt(0);

	m_pTutorial = CTutorial::Create();
	m_pTutorial->SetUseDeath(false);

	m_pPlayer = CPlayer::Create(D3DXVECTOR3(0.0f,200.0f,0.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	m_pPlayer->SetUseDeath(false);
	m_pPlayer->SetVecAxis(D3DXVECTOR3(0.0f, 1.0f, 0.0f));

	CManager::GetSound()->PlaySoundB(CSound::SOUND_LABEL::BGM_FINALBATTLE);
	return S_OK;
}
//=========================================================================================================================

//=============================================================
//終了処理
//=============================================================
void CGame::Uninit()
{
	//============================================
	//プレイヤーの破棄
	//============================================
	if (m_pPlayer != nullptr)
	{
		m_pPlayer->SetUseDeath(true);
		m_pPlayer->SetDeath();
		m_pPlayer = nullptr;
	}
	//=====================================================================

	//============================================
	//ステージマネージャーの破棄
	//============================================
	if (m_pStageManager != nullptr)
	{
		m_pStageManager->SetUseDeath(true);
		m_pStageManager->SetDeath();
		m_pStageManager = nullptr;
	}
	//=====================================================================

	//============================================
	//フェーズマネージャーの破棄
	//============================================
	if (m_pPhaseManager != nullptr)
	{
		m_pPhaseManager->SetUseDeath(true);
		m_pPhaseManager->SetDeath();
		m_pPhaseManager = nullptr;
	}
	//=====================================================================

	//============================================
	//チュートリアルの破棄
	//============================================
	if (m_pTutorial != nullptr)
	{
		m_pTutorial->SetUseDeath(true);
		m_pTutorial->SetDeath();
		m_pTutorial = nullptr;
	}
	//=====================================================================

	CManager::GetSound()->Stop();

	CScene::Uninit();//シーン終了処理
}
//=========================================================================================================================

//=============================================================
//更新処理
//=============================================================
void CGame::Update()
{
#ifdef _DEBUG
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN) == true || CManager::GetInputJoypad()->GetTrigger(CInputJoypad::JOYKEY::START) == true)
	{
		CManager::GetSceneFade()->SetSceneFade(CFade::FADEMODE_IN, CScene::MODE_RESULT);
	}
	CManager::GetDebugText()->PrintDebugText("現在の敵の数；%d\n", CEnemy::GetNumEnemy());
#else
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_1) == true || CManager::GetInputJoypad()->GetTrigger(CInputJoypad::JOYKEY::START) == true)
	{
		CManager::GetSceneFade()->SetSceneFade(CFade::FADEMODE_IN, CScene::MODE_RESULT);
	}
#endif // !_DEBUG
}
//=========================================================================================================================