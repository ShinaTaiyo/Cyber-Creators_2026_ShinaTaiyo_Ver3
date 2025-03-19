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
CPlayer* CGame::m_pPlayer = nullptr;             //プレイヤーへのポインタ
CStageManager* CGame::m_pStageManager = nullptr; //ステージマネージャーへのポインタ
CPhaseManager* CGame::m_pPhaseManager = nullptr; //フェーズマネージャーへのポインタ
CTutorial* CGame::m_pTutorial = nullptr;         //チュートリアルへのポインタ
int CGame::s_nPhaseNum = 0;                      //フェーズ番号
CScore* CGame::s_pSCORE = nullptr;               //スコアへのポインタ
CCombo* CGame::s_pCOMBO = nullptr;               //コンボへのポインタ
bool CGame::s_bGameClear = false;                //ゲームをクリアしたかどうか
//=========================================================================================================================

//=============================================================
//コンストラクタ
//=============================================================
CGame::CGame(bool bUseGamePad)
{
	m_pPlayer = nullptr;        //プレイヤーへのポインタを初期化
	m_pStageManager = nullptr;  //ステージマネージャーへのポインタを初期化
	m_pPhaseManager = nullptr;  //フェーズマネージャーへのポインタを初期化
	m_pTutorial = nullptr;      //チュートリアルへのポインタを初期化
	s_pSCORE = nullptr;         //スコアへのポインタを初期化
	s_pCOMBO = nullptr;         //コンボへのポインタを初期化
	s_nPhaseNum = 0;            //フェーズ番号を初期化
	s_bGameClear = false;       //ゲームをクリアしたかどうか（初期設定はクリアしていない状態）
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
#ifndef _DEBUG
	ShowCursor(FALSE);//カーソルを非表示に
#endif // !_DEBUG

	CScene::Init();//シーン初期化処理
	CObjectX::SetCommonDraw(true);//全ての影を描画する

	CBg3D::Create(CBg3D::BG3DTYPE::SKY, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1200.0f, 1200.0f, 1200.0f));//3D背景を生成

	m_pPhaseManager = CPhaseManager::Create();//フェーズマネージャーを生成
	m_pPhaseManager->SetUseDeath(false);      //死亡フラグを使用しない

	m_pStageManager = CStageManager::Create();//ステージマネージャーを生成
	m_pStageManager->SetUseDeath(false);      //死亡フラグを使用しない
	m_pStageManager->LoadMapTxt(0);           //マップ０を読み込む

	m_pTutorial = CTutorial::Create();        //チュートリアルを生成
	m_pTutorial->SetUseDeath(false);          //死亡フラグを使用しない

	s_pSCORE = CScore::Create(D3DXVECTOR3(SCREEN_WIDTH - CScore::GetNumberWidth(),CScore::GetNumberHeight(), 0.0f)); //スコアの生成
	s_pSCORE->SetUseDeath(false);                                                      //死亡フラグを使用しない

	s_pCOMBO = CCombo::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));//コンボを生成
	s_pCOMBO->SetUseDeath(false);//死亡フラグを使用しない

	if (s_bUSETUTORIAL == false)
	{//チュートリアルを使用しないなら
		for (int nCnt = 0; nCnt < static_cast<int>(CTutorial::CHECK::MAX); nCnt++)
		{//全てのチュートリアルを完了させる
			m_pTutorial->SetSuccessCheck(static_cast<CTutorial::CHECK>(nCnt));
		}
	}
	m_pPlayer = CPlayer::Create(D3DXVECTOR3(0.0f,200.0f,0.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f));//プレイヤーを生成
	m_pPlayer->SetUseDeath(false);//死亡フラグを使用しない

	CManager::GetSound()->PlaySoundB(CSound::SOUND_LABEL::BGM_FINALBATTLE);//BGMを再生
	return S_OK;
}
//=========================================================================================================================

//=============================================================
//終了処理
//=============================================================
void CGame::Uninit()
{
#ifndef _DEBUG
	ShowCursor(TRUE);//カーソルを非表示に
#endif

	//============================================
	//プレイヤーの破棄
	//============================================
	if (m_pPlayer != nullptr)
	{
		m_pPlayer->SetUseDeath(true);//死亡フラグを使用する
		m_pPlayer->SetDeath();       //死亡フラグを設定する
		m_pPlayer = nullptr;         //プレイヤーのポインタを初期化
	}
	//=====================================================================

	//============================================
	//ステージマネージャーの破棄
	//============================================
	if (m_pStageManager != nullptr)
	{
		m_pStageManager->SetUseDeath(true);//死亡フラグを使用する
		m_pStageManager->SetDeath();       //死亡フラグを設定する 
		m_pStageManager = nullptr;         //ステージマネージャーへのポインタを初期化
	}
	//=====================================================================

	//============================================
	//フェーズマネージャーの破棄
	//============================================
	if (m_pPhaseManager != nullptr)
	{
		m_pPhaseManager->SetUseDeath(true);//死亡フラグを使用する
		m_pPhaseManager->SetDeath();       //死亡フラグを設定する
		m_pPhaseManager = nullptr;         //フェーズマネージャーへのポインタを初期化
	}
	//=====================================================================

	//============================================
	//チュートリアルの破棄
	//============================================
	if (m_pTutorial != nullptr)
	{
		m_pTutorial->SetUseDeath(true);//死亡フラグを使用する
		m_pTutorial->SetDeath();       //死亡フラグを設定する
		m_pTutorial = nullptr;         //チュートリアルへのポインタを初期化
	}
	//=====================================================================

	//============================================
	//スコアの破棄
	//============================================
	if (s_pSCORE != nullptr)
	{
		s_pSCORE->SetUseDeath(true); //死亡フラグを使用する
		s_pSCORE->SetDeath();        //死亡フラグを設定する
		s_pSCORE = nullptr;          //スコアへのポインタを初期化
	}
	//=====================================================================
	
	//============================================
	//コンボの破棄
	//============================================
	if (s_pCOMBO != nullptr)
	{
		s_pCOMBO->SetUseDeath(true); //死亡フラグを使用する
		s_pCOMBO->SetDeath();        //死亡フラグを設定する
		s_pCOMBO = nullptr;          //スコアへのポインタを初期化
	}
	//=====================================================================

	CManager::GetSound()->Stop();//全てのサウンドを停止

	CScene::Uninit();//シーン終了処理
}
//=========================================================================================================================

//=============================================================
//更新処理
//=============================================================
void CGame::Update()
{
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN) == true || CManager::GetInputJoypad()->GetTrigger(CInputJoypad::JOYKEY::START) == true)
	{//ENTERキー、又はスタートボタンを押していたら
		CManager::GetSceneFade()->SetSceneFade(CFade::FADEMODE_IN, CScene::MODE_RESULT);//リザルトシーンを呼ぶ
	}

	//デバッグ表示
	CManager::GetDebugText()->PrintDebugText("現在の敵の数；%d\n", CEnemy::GetNumEnemy());
}
//=========================================================================================================================