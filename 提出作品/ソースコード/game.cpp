//======================================================================================================================
//
// ２０２５年６月８日：OBB実装[game.cpp]
//Author:ShinaTaiyo
//
//======================================================================================================================

//**********************************************************************************************************************
// インクルード
//**********************************************************************************************************************
#include "game.h"
#include "manager.h"
#include "sound.h"
#include "fade.h"
#include "field.h"
#include "bg3d.h"
#include "enemy.h"
#include "input.h"
#include "attack.h"
#include "calculation.h"
#include "camera.h"
#include "particle.h"
#include "debugtext.h"
#include "phasemanager.h"
#include "stagemanager.h"
#include "tutorial.h"
#include "score.h"
#include "combo.h"
#include "player.h"
#include "pausemanager.h"

//**********************************************************************************************************************
// 静的メンバ宣言
//**********************************************************************************************************************
CPlayer* CGame::m_pPlayer = nullptr; // プレイヤーへのポインタ
CStageManager* CGame::m_pStageManager = nullptr; // ステージマネージャーへのポインタ
CPhaseManager* CGame::m_pPhaseManager = nullptr; // フェーズマネージャーへのポインタ
CTutorial* CGame::m_pTutorial = nullptr; // チュートリアルへのポインタ
int CGame::s_nPhaseNum = 0; // フェーズ番号
CScore* CGame::s_pSCORE = nullptr;  // スコアへのポインタ
CCombo* CGame::s_pCOMBO = nullptr;  // コンボへのポインタ
bool CGame::s_bGameClear = false;   // ゲームをクリアしたかどうか
bool CGame::s_bPAUSE = false; // ポーズをするかどうか
bool CGame::s_bCHENGEPAUSE = false; // ポーズを終わらせるフラグ

//=====================================================================================================================
// 
// publicメンバ 
//
//=====================================================================================================================

//===================================================================================================================
// コンストラクタ
//===================================================================================================================
CGame::CGame(bool bUseGamePad)
{
	m_pPlayer = nullptr;        // プレイヤーへのポインタを初期化
	m_pStageManager = nullptr;  // ステージマネージャーへのポインタを初期化
	m_pPhaseManager = nullptr;  // フェーズマネージャーへのポインタを初期化
	m_pTutorial = nullptr;      // チュートリアルへのポインタを初期化
	s_pSCORE = nullptr;         // スコアへのポインタを初期化
	s_pCOMBO = nullptr;         // コンボへのポインタを初期化
	s_nPhaseNum = 0;            // フェーズ番号を初期化
	s_bGameClear = false;       // ゲームをクリアしたかどうか（初期設定はクリアしていない状態）
	s_bPAUSE = false;           // ポーズをするかどうかを初期化
	m_pPauseManager = nullptr;  // ポーズマネージャーへのポインタを初期化
	s_bCHENGEPAUSE = false;     // ポーズ状態変更フラグ
	m_pAttackEnemy = nullptr;   // 敵の攻撃へのポインタ
}

//===================================================================================================================
// デストラクタ
//===================================================================================================================
CGame::~CGame()
{
	// 何もなし
}

//===================================================================================================================
// 初期化処理
//===================================================================================================================
HRESULT CGame::Init()
{
#ifndef _DEBUG
	ShowCursor(FALSE);//カーソルを非表示に
#endif // !_DEBUG

	CScene::Init();// シーン初期化処理
	CObjectX::SetCommonDraw(true); // 全ての影を描画する

	// 3D背景を生成
	CBg3D::Create(CBg3D::TYPE::SKY, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1200.0f, 1200.0f, 1200.0f));

	CCamera* pCamera = CManager::GetCamera();       // カメラへのポインタ
	pCamera->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f)); // 向きを前に設定
	CCameraState_Normal * pNormal = dynamic_cast<CCameraState_Normal*>(pCamera->GetCameraState()); // カメラを３０フレーム操作不可能にする
	
	// カメラ通常状態があれば、向き操作不可能時間を設定
	if (pNormal)
	{
		// ※ カーソルを中央に固定し続け、そこから動いた量に応じてカメラを動かしているので、最初にカーソルを中央に設定します。
		// 画面の中心座標
		POINT center = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
		// マウスを中央にリセット
		SetCursorPos(static_cast<int>(center.x), static_cast<int>(center.y));
		pNormal->SetNoControlRotFrame(30);
	}

	m_pPhaseManager = CPhaseManager::Create(); // フェーズマネージャーを生成
	m_pPhaseManager->SetUseDeath(false); // 死亡フラグを使用しない

	m_pStageManager = CStageManager::Create(); // ステージマネージャーを生成
	m_pStageManager->SetUseDeath(false); // 死亡フラグを使用しない
	m_pStageManager->LoadMapTxt(0);      // マップ０を読み込む

	m_pTutorial = CTutorial::Create(); // チュートリアルを生成
	m_pTutorial->SetUseDeath(false);   // 死亡フラグを使用しない

	 // スコアの生成
	s_pSCORE = CScore::Create(D3DXVECTOR3(SCREEN_WIDTH - CScore::GetNumberWidth(),CScore::GetNumberHeight(), 0.0f));
	s_pSCORE->SetUseDeath(false); // 死亡フラグを使用しない

	s_pCOMBO = CCombo::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f)); // コンボを生成
	s_pCOMBO->SetUseDeath(false); // 死亡フラグを使用しない

	// チュートリアルを使用しないなら
	if (s_bUSETUTORIAL == false)
	{
		// 全てのチュートリアルを完了させる
		for (int nCnt = 0; nCnt < static_cast<int>(CTutorial::CHECK::MAX); nCnt++)
		{
			m_pTutorial->SetSuccessCheck(static_cast<CTutorial::CHECK>(nCnt));
		}
	}

	// プレイヤーを生成
	m_pPlayer = CPlayer::Create(m_pStageManager->GetSpawnPoint(), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	m_pPlayer->SetUseDeath(false); // 死亡フラグを使用しない

	CManager::GetSound()->PlaySoundB(CSound::SOUND_LABEL::BGM_FINALBATTLE); // BGMを再生

	// OBB判定確認用オブジェクト召喚
	{
		//for (int nCnt = 0; nCnt < 20; nCnt++)
		//{
		//	// 乱数値
		//	float fYaw = static_cast<float>(rand() % 100 - 50) * 1e-5f;   // Yaw
		//	float fPitch = static_cast<float>(rand() % 100 - 50) * 1e-5f; // Pitch
		//	float fRoll = static_cast<float>(rand() % 100 - 50) * 1e-5f;  // Roll

		//	// 敵の攻撃を生成
		//	CAttackEnemy* pAttackEnemy = CAttackEnemy::Create(
		//		CAttack::ATTACKTYPE::MAGICSWORD, CAttack::TARGETTYPE::PLAYER, CAttack::COLLISIONTYPE::OBBTOOBB, false, true, 1, 6, 6000,
		//		D3DXVECTOR3(0.0f, 200.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(10.0f, 10.0f, 50.0f));
		//	pAttackEnemy->GetRotInfo().SetUseAddRot(true, D3DXVECTOR3(fPitch, fYaw, fRoll) * static_cast<float>(nCnt + 1)); // 回転させる
		//	pAttackEnemy->SetCollisionRelease(false); // 当たっても破棄されない
		//}
	}
	return S_OK;
}

//===================================================================================================================
// 終了処理
//===================================================================================================================
void CGame::Uninit()
{
#ifndef _DEBUG
	ShowCursor(TRUE);//カーソルを非表示に
#endif

	// プレイヤー終了
	if (m_pPlayer != nullptr)
	{
		m_pPlayer->SetUseDeath(true); // 死亡フラグを使用する
		m_pPlayer->SetDeath(); // 死亡フラグを設定する
		m_pPlayer = nullptr;   // プレイヤーへのポインタを初期化
	}

	// ステージマネージャー終了
	if (m_pStageManager != nullptr)
	{
		m_pStageManager->SetUseDeath(true); // 死亡フラグを使用する
		m_pStageManager->SetDeath(); // 死亡フラグを設定する 
		m_pStageManager = nullptr;   // ステージマネージャーへのポインタを初期化
	}

	// フェーズマネージャー終了
	if (m_pPhaseManager != nullptr)
	{
		m_pPhaseManager->SetUseDeath(true); // 死亡フラグを使用する
		m_pPhaseManager->SetDeath(); // 死亡フラグを設定する
		m_pPhaseManager = nullptr;   // フェーズマネージャーへのポインタを初期化
	}

	// ポーズマネージャー終了
	if (m_pPauseManager != nullptr)
	{
		m_pPauseManager->SetUseDeath(true); // 死亡フラグを使用する
		m_pPauseManager->SetDeath(); // 死亡フラグを設定する
		m_pPauseManager = nullptr;   // フェーズマネージャーへのポインタを初期化
	}

	// チュートリアル終了
	if (m_pTutorial != nullptr)
	{
		m_pTutorial->SetUseDeath(true); // 死亡フラグを使用する
		m_pTutorial->SetDeath(); //死亡フラグを設定する
		m_pTutorial = nullptr;   //チュートリアルへのポインタを初期化
	}

	// スコア終了
	if (s_pSCORE != nullptr)
	{
		s_pSCORE->SetUseDeath(true); // 死亡フラグを使用する
		s_pSCORE->SetDeath(); // 死亡フラグを設定する
		s_pSCORE = nullptr;   // スコアへのポインタを初期化
	}
	
	// コンボ終了
	if (s_pCOMBO != nullptr)
	{
		s_pCOMBO->SetUseDeath(true); // 死亡フラグを使用する
		s_pCOMBO->SetDeath(); // 死亡フラグを設定する
		s_pCOMBO = nullptr;   // スコアへのポインタを初期化
	}

	// 敵の攻撃終了
	if (m_pAttackEnemy)
	{
		m_pAttackEnemy->SetUseDeath(true); // 死亡フラグを使用する
		m_pAttackEnemy->SetDeath();     // 死亡フラグを設定する
		m_pAttackEnemy = nullptr;       // 敵の攻撃へのポインタを初期化
	}

	s_bPAUSE = false;//ポーズ中にゲームモードを終わらせるときもポーズ状態を解除する

	CManager::GetSound()->Stop(); // 全てのサウンドを停止

	CScene::Uninit(); // シーン終了処理
}

//===================================================================================================================
// 更新処理
//===================================================================================================================
void CGame::Update()
{
	// ポーズ状態じゃないときしかゲームシーンからは画面遷移出来ない
	if (!s_bPAUSE)
	{
		// ENTERキー、又はスタートボタンを押したら画面遷移開始
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN) == true || CManager::GetInputJoypad()->GetTrigger(CInputJoypad::JOYKEY::START) == true)
		{
			CManager::GetSceneFade()->SetSceneFade(CFade::FADEMODE_IN, CScene::MODE_RESULT);
		}
	}

	PauseProcess(); // ポーズ処理

	//デバッグ表示
	CManager::GetDebugText()->PrintDebugText("現在の敵の数；%d\n", CEnemy::GetNumEnemy());
}

//===================================================================================================================
// ゲームをクリアしたかどうか設定
//===================================================================================================================
void CGame::SetGameClear(bool bClear)
{
	s_bGameClear = bClear;
}

//===================================================================================================================
// ゲームをクリアしたかどうか取得
//===================================================================================================================
const bool& CGame::GetGameClear()
{
	return s_bGameClear;
}

//===================================================================================================================
// プレイヤー取得
//===================================================================================================================
CPlayer* CGame::GetPlayer()
{
	return m_pPlayer;
}

//===================================================================================================================
// フェーズマネージャー取得
//===================================================================================================================
CPhaseManager* CGame::GetPhaseManager()
{
	return m_pPhaseManager;
}

//===================================================================================================================
// ステージマネージャー取得
//===================================================================================================================
CStageManager* CGame::GetStageManager()
{
	return m_pStageManager;
}

//===================================================================================================================
// チュートリアル取得
//===================================================================================================================
CTutorial* CGame::GetTutorial()
{
	return m_pTutorial;
}

//===================================================================================================================
// スコア取得
//===================================================================================================================
CScore* CGame::GetScore()
{
	return s_pSCORE;
}

//===================================================================================================================
// コンボ取得
//===================================================================================================================
CCombo* CGame::GetCombo()
{
	return s_pCOMBO;
}

//===================================================================================================================
// ポーズ状態かどうか取得
//===================================================================================================================
const bool& CGame::GetPauseFlag()
{
	return s_bPAUSE;
}

//===================================================================================================================
// ポーズ状態変更フラグ設定
//===================================================================================================================
void CGame::SetChengePause()
{
	s_bCHENGEPAUSE = true;
}

//=====================================================================================================================
// 
// privateメンバ 
//
//=====================================================================================================================

//===================================================================================================================
// ポーズ処理
//===================================================================================================================
void CGame::PauseProcess()
{
	CInputKeyboard* pInputKeyboard = CManager::GetInputKeyboard(); // キー入力情報を取得する
	CInputJoypad* pInputJoypad = CManager::GetInputJoypad(); // ジョイパッド入力情報を取得する
	CDebugText* pDebugText = CManager::GetDebugText(); // デバッグ情報を取得する

	//Pキーを押す、又はポーズ状態変更フラグを発動したらポーズ注の処理を呼び出したり戻したりする
	if (pInputKeyboard->GetTrigger(DIK_P) || pInputJoypad->GetTrigger(CInputJoypad::JOYKEY::BACK)|| s_bCHENGEPAUSE)
	{
		s_bPAUSE = s_bPAUSE ? false : true; // フラグを切り替える

		// ポーズ時
		if (s_bPAUSE)
		{
			m_pPauseManager = CPauseManager::Create();    // ポーズ状態なので、ポーズマネージャーを生成
			m_pPauseManager->SetUseDeath(false);          // 死亡フラグを使用しない
			m_pPauseManager->SetIsStopUpdatePause(false); // ポーズ中に更新を止めない
		}
		// ポーズ解除時
		else
		{
			// 破棄する
			if (m_pPauseManager != nullptr)
			{
				m_pPauseManager->SetUseDeath(true); // 死亡フラグを使用する
				m_pPauseManager->SetDeath(); // 死亡フラグを設定する
				m_pPauseManager = nullptr;   // フェーズマネージャーへのポインタを初期化
			}
		}
	}

	s_bCHENGEPAUSE = false; // ポーズ状態変更フラグを初期化（外部からいじらない限り、常にfalse。フラグ設定処理を呼んだフレームだけtrueになる。)

	// デバッグ表示
	pDebugText->PrintDebugText("ポーズ中かどうか(P)：%d\n", s_bPAUSE);
}