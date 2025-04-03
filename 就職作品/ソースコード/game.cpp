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
bool CGame::s_bPAUSE = false;                    //ポーズをするかどうか
bool CGame::s_bCHENGEPAUSE = false;                 //ポーズを終わらせるフラグ
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
	s_bPAUSE = false;           //ポーズをするかどうかを初期化
	m_pPauseManager = nullptr;  //ポーズマネージャーへのポインタを初期化
	s_bCHENGEPAUSE = false;     //ポーズ状態変更フラグ
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
	m_pPlayer = CPlayer::Create(m_pStageManager->GetSpawnPoint(), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f));//プレイヤーを生成
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
	//ポーズマネージャーの破棄
	//============================================
	if (m_pPauseManager != nullptr)
	{
		m_pPauseManager->SetUseDeath(true);//死亡フラグを使用する
		m_pPauseManager->SetDeath();       //死亡フラグを設定する
		m_pPauseManager = nullptr;         //フェーズマネージャーへのポインタを初期化
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

	s_bPAUSE = false;//ポーズ中にゲームモードを終わらせるときもポーズ状態を解除する

	CManager::GetSound()->Stop();//全てのサウンドを停止

	CScene::Uninit();//シーン終了処理
}
//=========================================================================================================================

//=============================================================
//更新処理
//=============================================================
void CGame::Update()
{
	if (!s_bPAUSE)
	{//ポーズ状態じゃないときしかゲームシーンからは画面遷移出来ない
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN) == true || CManager::GetInputJoypad()->GetTrigger(CInputJoypad::JOYKEY::START) == true)
		{//ENTERキー、又はスタートボタンを押していたら
			CManager::GetSceneFade()->SetSceneFade(CFade::FADEMODE_IN, CScene::MODE_RESULT);//リザルトシーンを呼ぶ
		}
	}
	PauseProcess();//ポーズ処理

	//デバッグ表示
	CManager::GetDebugText()->PrintDebugText("現在の敵の数；%d\n", CEnemy::GetNumEnemy());
}
//=========================================================================================================================

//=============================================================
//ポーズ処理
//=============================================================
void CGame::PauseProcess()
{
	CInputKeyboard* pInputKeyboard = CManager::GetInputKeyboard(); //キー入力情報を取得する
	CInputJoypad* pInputJoypad = CManager::GetInputJoypad();       //ジョイパッド入力情報を取得する
	CDebugText* pDebugText = CManager::GetDebugText();             //デバッグ情報を取得する

	if (pInputKeyboard->GetTrigger(DIK_P) || pInputJoypad->GetTrigger(CInputJoypad::JOYKEY::BACK)|| s_bCHENGEPAUSE)
	{//Pキーを押す、又はポーズ状態変更フラグを発動する
		s_bPAUSE = s_bPAUSE ? false : true;//フラグを切り替える
		if (s_bPAUSE)
		{//ポーズ時
			m_pPauseManager = CPauseManager::Create();    //ポーズ状態なので、ポーズマネージャーを生成
			m_pPauseManager->SetUseDeath(false);          //死亡フラグを使用しない
			m_pPauseManager->SetIsStopUpdatePause(false); //ポーズ中に更新を止めない
		}
		else
		{//ポーズ解除時
			if (m_pPauseManager != nullptr)
			{//破棄する
				m_pPauseManager->SetUseDeath(true);       //死亡フラグを使用する
				m_pPauseManager->SetDeath();              //死亡フラグを設定する
				m_pPauseManager = nullptr;                //フェーズマネージャーへのポインタを初期化
			}
		}
	}

	s_bCHENGEPAUSE = false;//ポーズ状態変更フラグを初期化（外部からいじらない限り、常にfalse。フラグ設定処理を呼んだフレームだけtrueになる。)

	pDebugText->PrintDebugText("ポーズ中かどうか(P)：%d\n", s_bPAUSE);
}
//=========================================================================================================================