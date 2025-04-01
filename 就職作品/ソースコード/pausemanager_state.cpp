//==============================================================================================================
//
//２０２５年３月３１日：ポーズ処理を実装する[pausemanager_state.h]
//Author:ShinaTaiyo
//
//==============================================================================================================

//======================================================
//インクルード
//======================================================
#include "pausemanager_state.h"
#include "input.h"
#include "manager.h"
#include "debugtext.h"
#include "camera.h"
#include "fade.h"
#include "game.h"
//=======================================================================================

//******************************************************
//コンポーネント
//******************************************************

//======================================================
//コンストラクタ
//======================================================
CPauseState_Component::CPauseState_Component(CPauseManager* pPauseManager)
{
	//特になし
}
//=======================================================================================

//======================================================
//デストラクタ
//======================================================
CPauseState_Component::~CPauseState_Component()
{
	//特になし
}
//=======================================================================================

//======================================================
//更新処理
//======================================================
void CPauseState_Component::Update(CPauseManager* pPauseManager)
{
	//特になし
}
//=======================================================================================

//******************************************************
//通常
//******************************************************

//======================================================
//コンストラクタ
//======================================================
CPauseState_Normal::CPauseState_Normal(CPauseManager* pPauseManager) : CPauseState_Component(pPauseManager)
{
	m_pCommandUi[static_cast<int>(COMMANDTYPE::CONTINUE)] = CUi::Create(CUi::UITYPE::CONTINUETEXT_000, true, CObject2D::POLYGONTYPE::SENTERROLLING,
		200.0f, 100.0f, 1, false, D3DXVECTOR3(SCREEN_WIDTH / 2, 200.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));//コンティニューコマンド生成
	m_pCommandUi[static_cast<int>(COMMANDTYPE::EXIT)] = CUi::Create(CUi::UITYPE::EXITTEXT_000, true, CObject2D::POLYGONTYPE::SENTERROLLING,
		200.0f, 100.0f, 1, false, D3DXVECTOR3(SCREEN_WIDTH / 2, 400.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));//EXITコマンド生成
	m_pCommandUi[static_cast<int>(COMMANDTYPE::CAMERASENSITIVITY)] = m_pCommandUi[static_cast<int>(COMMANDTYPE::CAMERASENSITIVITY)] = CUi::Create(CUi::UITYPE::CAMERASENSITIVITYTEXT_000,
		true, CObject2D::POLYGONTYPE::SENTERROLLING, 200.0f, 100.0f, 1, false, D3DXVECTOR3(SCREEN_WIDTH / 2, 600.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));//EXITコマンド生成
	for (int nCnt = 0; nCnt < static_cast<int>(COMMANDTYPE::MAX); nCnt++)
	{//全コマンド共通設定
		m_pCommandUi[nCnt]->SetUseDeath(false);//全てのコマンドの死亡フラグを使用しない
		m_pCommandUi[nCnt]->SetIsStopUpdatePause(false);//ポーズ中に更新を止めない
	}
}
//=======================================================================================

//======================================================
//デストラクタ
//======================================================
CPauseState_Normal::~CPauseState_Normal()
{
	for (int nCnt = 0; nCnt < static_cast<int>(COMMANDTYPE::MAX); nCnt++)
	{//前コマンド死亡フラグ設定処理
		if (m_pCommandUi[nCnt] != nullptr)
		{//ポインタが存在していたら
			m_pCommandUi[nCnt]->SetUseDeath(true); //全てのコマンドの死亡フラグを使用する
			m_pCommandUi[nCnt]->SetDeath();        //死亡フラグを設定する
		}
	}
}
//=======================================================================================

//======================================================
//更新処理
//======================================================
void CPauseState_Normal::Update(CPauseManager* pPauseManager)
{
	ChooseCommand();                //コマンド選択処理
	ShowSelectedCommand();          //選択したコマンドを表示する処理
	ExecuteCommand(pPauseManager);  //コマンド発動処理
}
//=======================================================================================

//======================================================
//コマンド選択処理
//======================================================
void CPauseState_Normal::ChooseCommand()
{
	CInputKeyboard* pInputKeyboard = CManager::GetInputKeyboard(); //キー入力情報を取得
	CInputJoypad* pInputJoypad = CManager::GetInputJoypad();       //ジョイパッド入力情報を取得
	CDebugText* pDebugText = CManager::GetDebugText();             //デバッグ表示情報を取得
	int nNowChooseCoommand = m_nChooseCommandNum;                  //現在選んでいるコマンド番号を格納
	if (pInputKeyboard->GetTrigger(DIK_S) || pInputJoypad->GetTrigger(CInputJoypad::JOYKEY::DOWN))
	{
		nNowChooseCoommand++;//次のコマンドにインクリメント
	}
	if (pInputKeyboard->GetTrigger(DIK_W) || pInputJoypad->GetTrigger(CInputJoypad::JOYKEY::UP))
	{
		nNowChooseCoommand--;//前のコマンドにインクリメント
	}
	nNowChooseCoommand = (nNowChooseCoommand + static_cast<int>(COMMANDTYPE::MAX)) % static_cast<int>(COMMANDTYPE::MAX);// 範囲制限
	m_nChooseCommandNum = nNowChooseCoommand;//更新

	pDebugText->PrintDebugText("選択しているポーズコマンド：%d\n", m_nChooseCommandNum);
}
//=======================================================================================

//======================================================
//選択コマンドを表示する処理
//======================================================
void CPauseState_Normal::ShowSelectedCommand()
{
	for (int nCnt = 0; nCnt < static_cast<int>(COMMANDTYPE::MAX); nCnt++)
	{
		if (nCnt == m_nChooseCommandNum)
		{
			m_pCommandUi[nCnt]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), false, 1.0f);//選択中の番号のコマンドの色だけ通常にする
		}
		else
		{
			m_pCommandUi[nCnt]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f), false, 1.0f);//選択していないコマンドの色を薄くする
		}
	}
}
//=======================================================================================

//======================================================
//選択コマンドを実行する処理
//======================================================
void CPauseState_Normal::ExecuteCommand(CPauseManager* pPauseManager)
{
	CInputKeyboard* pInputKeyboard = CManager::GetInputKeyboard(); //キー入力情報を取得
	CInputJoypad* pInputJoypad = CManager::GetInputJoypad();       //ジョイパッド入力情報を取得
	CSceneFade* pSceneFade = CManager::GetSceneFade();             //シーン遷移用フェード情報を取得
	if (pInputKeyboard->GetTrigger(DIK_RETURN) || pInputJoypad->GetTrigger(CInputJoypad::JOYKEY::A))
	{//ENTERキー、又はジョイパッドのAボタン
		switch (m_nChooseCommandNum)
		{
		case static_cast<int>(COMMANDTYPE::CONTINUE)://選択コマンドが「コンティニュー」だったら
			CGame::SetChengePause();//ポーズ状態変更フラグをONにし、ポーズを終了する			
			break;
		case static_cast<int>(COMMANDTYPE::EXIT)://選択コマンドが「EXIT(タイトルに戻る)」だったら
			pSceneFade->SetSceneFade(CFade::FADEMODE_IN, CScene::MODE_TITLE);//タイトル画面へ遷移する
			break;
		case static_cast<int>(COMMANDTYPE::CAMERASENSITIVITY)://選択コマンドが「カメラ感度調整」だったら
			pPauseManager->ChengeState(DBG_NEW CPauseState_CameraSensitivity(pPauseManager));
			break;
		default:
			break;
		}
	}
}
//=======================================================================================

//******************************************************
//カメラ感度調整
//******************************************************

//======================================================
//コンストラクタ
//======================================================
CPauseState_CameraSensitivity::CPauseState_CameraSensitivity(CPauseManager* pPauseManager) : CPauseState_Component(pPauseManager)
{
	m_pSensitivityLevelStick = CUi::Create(CUi::UITYPE::LOCKON, true, CObject2D::POLYGONTYPE::SENTERROLLING, 200.0f, 50.0f, 1, false,
		D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	m_pSensitivityLevelStick->InitTextureOnly();            //テクスチャを使わない
	m_pSensitivityLevelStick->SetUseDeath(false);           //死亡フラグを使用しない
	m_pSensitivityLevelStick->SetIsStopUpdatePause(false);  //ポーズ中も更新する
	m_pNowSensitivityLevelVar = CUi::Create(CUi::UITYPE::LOCKON, true, CObject2D::POLYGONTYPE::SENTERROLLING, 20.0f, 80.0f, 1, false,
		D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	m_pNowSensitivityLevelVar->InitTextureOnly();           //テクスチャを使わない
	m_pNowSensitivityLevelVar->SetUseDeath(false);          //死亡フラグを使用しない
	m_pNowSensitivityLevelVar->SetIsStopUpdatePause(false); //ポーズ中も更新する

	m_pReturnTex = CUi::Create(CUi::UITYPE::RETURNTEXT_000, true, CObject2D::POLYGONTYPE::SENTERROLLING, 200.0f, 100.0f, 1, false,
		D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 120.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	m_pReturnTex->SetUseDeath(false);          //死亡フラグを使用しない
	m_pReturnTex->SetIsStopUpdatePause(false); //ポーズ中も更新する

	m_nSensitivitLevel = CCamera::GetSensitivityLevel();//現在のカメラの感度レベルを取得する
}
//=======================================================================================

//======================================================
//デストラクタ
//======================================================
CPauseState_CameraSensitivity::~CPauseState_CameraSensitivity()
{
	if (m_pSensitivityLevelStick != nullptr)
	{
		m_pSensitivityLevelStick->SetUseDeath(true);
		m_pSensitivityLevelStick->SetDeath();
		m_pSensitivityLevelStick = nullptr;
	}

	if (m_pNowSensitivityLevelVar != nullptr)
	{
		m_pNowSensitivityLevelVar->SetUseDeath(true);
		m_pNowSensitivityLevelVar->SetDeath();
		m_pNowSensitivityLevelVar = nullptr;
	}

	if (m_pReturnTex != nullptr)
	{
		m_pReturnTex->SetUseDeath(true);
		m_pReturnTex->SetDeath();
		m_pReturnTex = nullptr;
	}


	CCamera::SetSensitivityLevel(m_nSensitivitLevel);//カメラの感度を設定する
}
//=======================================================================================

//======================================================
//更新処理
//======================================================
void CPauseState_CameraSensitivity::Update(CPauseManager* pPauseManager)
{
	if (m_pSensitivityLevelStick != nullptr&&m_pNowSensitivityLevelVar != nullptr)
	{
		ChooseCommand();//コマンド選択処理
		switch (m_nChooseCommandNum)
		{
		case static_cast<int>(COMMANDTYPE::SENSITIVITY):
			//使用しないコマンドの色を薄くする
			m_pReturnTex->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), true, 0.5f);
			//使用するコマンドの色を元に戻す
			m_pNowSensitivityLevelVar->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), true,1.0f);
			m_pSensitivityLevelStick->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), true, 1.0f);
			//カメラ感度調整
			CameraSensitivity();
			break;
		case static_cast<int>(COMMANDTYPE::RETURN):
			//使用しないコマンドの色を薄くする
			m_pNowSensitivityLevelVar->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), true, 0.5f);
			m_pSensitivityLevelStick->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), true, 0.5f);
			//使用するコマンドの色を元に戻す
			m_pReturnTex->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), true, 1.0f);
			//コマンド選択画面に戻る
			Return(pPauseManager);
			break;
		default:
			break;
		}
	}
}
//=======================================================================================

//======================================================
//コマンド選択処理
//======================================================
void CPauseState_CameraSensitivity::ChooseCommand()
{
	CInputKeyboard* pInputKeyboard = CManager::GetInputKeyboard(); //キー入力情報を取得
	CInputJoypad* pInputJoypad = CManager::GetInputJoypad();       //ジョイパッド入力情報を取得
	CDebugText* pDebugText = CManager::GetDebugText();             //デバッグ表示情報を取得
	int nNowChooseCoommand = m_nChooseCommandNum;                  //現在選んでいるコマンド番号を格納
	if (pInputKeyboard->GetTrigger(DIK_S) || pInputJoypad->GetTrigger(CInputJoypad::JOYKEY::DOWN))
	{
		nNowChooseCoommand++;//次のコマンドにインクリメント
	}
	if (pInputKeyboard->GetTrigger(DIK_W) || pInputJoypad->GetTrigger(CInputJoypad::JOYKEY::UP))
	{
		nNowChooseCoommand--;//前のコマンドにインクリメント
	}
	nNowChooseCoommand = (nNowChooseCoommand + static_cast<int>(COMMANDTYPE::MAX)) % static_cast<int>(COMMANDTYPE::MAX);// 範囲制限
	m_nChooseCommandNum = nNowChooseCoommand;//更新

	pDebugText->PrintDebugText("選択しているカメラ感度調整コマンド：%d\n", m_nChooseCommandNum);

}
//=======================================================================================

//======================================================
//カメラ感度調整
//======================================================
void CPauseState_CameraSensitivity::CameraSensitivity()
{
	CInputKeyboard* pInputKeyboard = CManager::GetInputKeyboard(); //キー入力情報を取得
	CInputJoypad* pInputJoypad = CManager::GetInputJoypad();       //ジョイパッド入力情報を取得
	CDebugText* pDebugText = CManager::GetDebugText();             //デバッグ表示情報を取得
	int nNowSensitivityLevel = m_nSensitivitLevel;                 //現在選んでいる感度レベルを格納
	const int& nMaxSensitivityLevel = CCamera::GetMaxSensitivityLevel();//最大感度レベルを取得
	if (pInputKeyboard->GetTrigger(DIK_D) || pInputJoypad->GetTrigger(CInputJoypad::JOYKEY::RIGHT))
	{
		nNowSensitivityLevel++;//次のコマンドにインクリメント
	}
	if (pInputKeyboard->GetTrigger(DIK_A) || pInputJoypad->GetTrigger(CInputJoypad::JOYKEY::LEFT))
	{
		nNowSensitivityLevel--;//前のコマンドにインクリメント
	}

	nNowSensitivityLevel = ((nNowSensitivityLevel + nMaxSensitivityLevel)) % nMaxSensitivityLevel; // 範囲制限
	m_nSensitivitLevel = nNowSensitivityLevel;                 // 更新

	const D3DXVECTOR3& StickPos = m_pSensitivityLevelStick->GetPos();  //レベルスティックの位置
	const float fStickWidth = m_pSensitivityLevelStick->GetWidth();    //スティックの横幅
	const D3DXVECTOR3& VarPos = m_pNowSensitivityLevelVar->GetPos();   //レベルバーの位置
	float fDivisionStickWidth = fStickWidth / nMaxSensitivityLevel;                      //スティックの横幅
	float fVarWidth = m_pNowSensitivityLevelVar->GetWidth() / 2;       //レベルバーの横幅
	m_pNowSensitivityLevelVar->SetPos(D3DXVECTOR3(StickPos.x - fStickWidth / 2 + fVarWidth + fDivisionStickWidth * m_nSensitivitLevel, StickPos.y, 0.0f));

	pDebugText->PrintDebugText("カメラ感度レベル：%d\n", m_nSensitivitLevel + 1);
}
//=======================================================================================

//======================================================
//コマンド選択画面に戻る
//======================================================
void CPauseState_CameraSensitivity::Return(CPauseManager* pPauseManager)
{
	CInputKeyboard* pInputKeyboard = CManager::GetInputKeyboard(); //キー入力情報を取得
	CInputJoypad* pInputJoypad = CManager::GetInputJoypad();       //ジョイパッド入力情報を取得
	if (pInputKeyboard->GetTrigger(DIK_RETURN) || pInputJoypad->GetTrigger(CInputJoypad::JOYKEY::A))
	{//ENTERキー、又はジョイパッドのAボタン
		pPauseManager->ChengeState(DBG_NEW CPauseState_Normal(pPauseManager));//コマンド選択画面に戻る
	}
}
//=======================================================================================