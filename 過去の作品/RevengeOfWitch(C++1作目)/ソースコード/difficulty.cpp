//=========================================================================================================================
//
//６月１７日：画面遷移を実装する[Difficulty.cpp]
//Author:ShinaTaiyo
//
//=========================================================================================================================

//=============================================================
//インクルード
//=============================================================
#include "difficulty.h"
#include "bg.h"
#include "manager.h"
#include "renderer.h"
#include "sound.h"
#include "fade.h"
#include "game.h"
#include "input.h"
//=========================================================================================================================

//=============================================================
//静的メンバ宣言
//=============================================================
int CDifficulty::s_nChooseDifficultyNum = 0;//難易度番号

//=============================================================
//コンストラクタ
//=============================================================
CDifficulty::CDifficulty() : m_pDifficultyUi()
{
	s_nChooseDifficultyNum = 0;
}
//=========================================================================================================================

//=============================================================
//デストラクタ
//=============================================================
CDifficulty::~CDifficulty()
{

}
//=========================================================================================================================

//=============================================================
//初期化処理
//=============================================================
HRESULT CDifficulty::Init()
{
	CScene::Init();//シーン初期化処理
	CBgDifficulty* pBgDifficulty = CBgDifficulty::Create(CBg::BG_DIFFICULTY);
	CUi* pUi = nullptr;
	pBgDifficulty->SetUseTexMove(true, D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR2(0.04f, 0.0f));

	//難易度「EASY」のテキストUIを生成
	m_pDifficultyUi[DIFFICULTYTYPE_EASY] = CUi::Create(CUi::UITYPE_EASY_TEXT, CObject2D::POLYGONTYPE01_SENTERROLLING, 200.0f, 100.0f,
		D3DXVECTOR3(SENTER_VECTOR3.x, 200.0f, 0.0f), NULL_VECTOR3, NULL_VECTOR3, NORMAL_COL);

	//難易度「NORMAL」のテキストUIを生成
	m_pDifficultyUi[DIFFICULTYTYPE_NORMAL] = CUi::Create(CUi::UITYPE_NORMAL_TEXT, CObject2D::POLYGONTYPE01_SENTERROLLING, 200.0f, 100.0f,
		D3DXVECTOR3(SENTER_VECTOR3.x, 200.0f * 2, 0.0f), NULL_VECTOR3, NULL_VECTOR3, NORMAL_COL);

	//難易度「BOSSRUSH」のテキストUIを生成
	m_pDifficultyUi[DIFFICULTYTYPE_BOSSRUSH] = CUi::Create(CUi::UITYPE_BOSSRUSH_TEXT, CObject2D::POLYGONTYPE01_SENTERROLLING, 200.0f, 100.0f,
		D3DXVECTOR3(SENTER_VECTOR3.x, 200.0f * 3, 0.0f), NULL_VECTOR3, NULL_VECTOR3, NORMAL_COL);


	//難易度選択のテキストUIを生成
	pUi = CUi::Create(CUi::UITYPE_CHOOSEDIFFICULTY_TEXT, CObject2D::POLYGONTYPE01_SENTERROLLING, 300.0f, 120.0f,
		D3DXVECTOR3(SENTER_VECTOR3.x, 75.0f, 0.0f), NULL_VECTOR3,NULL_VECTOR3, NORMAL_COL);
	pUi->SetUseDeath(true);

	//「スタートかEnterを押す」のテキストUIを生成
	pUi = CUi::Create(CUi::UITYPE_PRESSENTERORSTART_LOGO, CObject2D::POLYGONTYPE01_SENTERROLLING, 140.0f, 140.0f,
		D3DXVECTOR3(SENTER_VECTOR3.x,SCREEN_HEIGHT - 70.0f, 0.0f), NULL_VECTOR3, NULL_VECTOR3, NORMAL_COL);
	pUi->SetUseBlinking(true, 60, 0.0f);
	pUi->SetUseDeath(true);

	for (int nCnt = 0; nCnt < DIFFICULTYTYPE_MAX; nCnt++)
	{
		if (nCnt == s_nChooseDifficultyNum)
		{
			m_pDifficultyUi[nCnt]->SetUseFloating(0.0f, 0.1f, 2.0f, true);
			m_pDifficultyUi[nCnt]->SetColor(NORMAL_COL, false, 0.0f);
		}
		else
		{
			m_pDifficultyUi[nCnt]->SetUseFloating(0.0f, 0.0f, 0.0f, false);
			m_pDifficultyUi[nCnt]->SetMove(NULL_VECTOR3);
			m_pDifficultyUi[nCnt]->SetPos(m_pDifficultyUi[nCnt]->GetSupportPos());
			m_pDifficultyUi[nCnt]->SetColor(NORMAL_COL, true, 0.5f);
		}
	}

	CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_BGM_DIFFICULTY000);

	return S_OK;
}
//=========================================================================================================================

//=============================================================
//終了処理
//=============================================================
void CDifficulty::Uninit()
{
	CScene::Uninit();//シーン終了処理
	CManager::GetSound()->StopSound(CSound::SOUND_LABEL_BGM_DIFFICULTY000);

	for (int nCnt = 0; nCnt < DIFFICULTYTYPE_MAX; nCnt++)
	{
		if (m_pDifficultyUi[nCnt] != nullptr)
		{
			m_pDifficultyUi[nCnt]->SetUseDeath(true);
			m_pDifficultyUi[nCnt]->SetDeath();
			m_pDifficultyUi[nCnt] = nullptr;
		}
	}
}
//=========================================================================================================================

//=============================================================
//更新処理
//=============================================================
void CDifficulty::Update()
{
	CScene::Update();//シーン更新処理

	ChooseDifficulty();//難易度選択処理

	if (CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN) == true || CManager::GetInputJoypad()->GetTrigger(CInputJoypad::JOYKEY_START) == true)
	{
		CGame::SetDifficulty(DIFFICULTYTYPE(s_nChooseDifficultyNum));
		CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_DECISION_000);
		CManager::GetSceneFade()->SetSceneFade(CFade::FADEMODE_IN, CScene::MODE_GAME);
	}
}
//=========================================================================================================================

//=============================================================
//描画処理
//=============================================================
void CDifficulty::Draw()
{
	CScene::Draw();//シーン描画処理
}
//=========================================================================================================================

//=============================================================
//難易度選択処理
//=============================================================
void CDifficulty::ChooseDifficulty()
{
	bool bChoose = false;//選択しているかどうか
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_S) == true || CManager::GetInputJoypad()->GetTrigger(CInputJoypad::JOYKEY_DOWN) == true)
	{
		s_nChooseDifficultyNum++;

		if (s_nChooseDifficultyNum == DIFFICULTYTYPE_MAX)
		{
			s_nChooseDifficultyNum = 0;
		}
		bChoose = true;
	}
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_W) == true || CManager::GetInputJoypad()->GetTrigger(CInputJoypad::JOYKEY_UP) == true)
	{
		s_nChooseDifficultyNum--;

		if (s_nChooseDifficultyNum < 0)
		{
			s_nChooseDifficultyNum = DIFFICULTYTYPE_MAX - 1;
		}
		bChoose = true;
	}

	TextStaging(bChoose);//テキストUIの演出処理

}
//=========================================================================================================================

//=============================================================
//テキストUIの演出処理
//=============================================================
void CDifficulty::TextStaging(bool bChoose)
{
	if (bChoose == true)
	{
		CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_SELECT_000);
		for (int nCnt = 0; nCnt < DIFFICULTYTYPE_MAX; nCnt++)
		{
			if (nCnt == s_nChooseDifficultyNum)
			{
				m_pDifficultyUi[nCnt]->SetUseFloating(0.0f, 0.1f, 2.0f, true);
				m_pDifficultyUi[nCnt]->SetColor(NORMAL_COL, false, 0.0f);
			}
			else
			{
				m_pDifficultyUi[nCnt]->SetUseFloating(0.0f, 0.0f, 0.0f, false);
				m_pDifficultyUi[nCnt]->SetMove(NULL_VECTOR3);
				m_pDifficultyUi[nCnt]->SetPos(m_pDifficultyUi[nCnt]->GetSupportPos());
				m_pDifficultyUi[nCnt]->SetColor(NORMAL_COL, true, 0.5f);
			}
		}
	}
}
//=========================================================================================================================