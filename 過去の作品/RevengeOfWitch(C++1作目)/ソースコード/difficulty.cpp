//=========================================================================================================================
//
//�U���P�V���F��ʑJ�ڂ���������[Difficulty.cpp]
//Author:ShinaTaiyo
//
//=========================================================================================================================

//=============================================================
//�C���N���[�h
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
//�ÓI�����o�錾
//=============================================================
int CDifficulty::s_nChooseDifficultyNum = 0;//��Փx�ԍ�

//=============================================================
//�R���X�g���N�^
//=============================================================
CDifficulty::CDifficulty() : m_pDifficultyUi()
{
	s_nChooseDifficultyNum = 0;
}
//=========================================================================================================================

//=============================================================
//�f�X�g���N�^
//=============================================================
CDifficulty::~CDifficulty()
{

}
//=========================================================================================================================

//=============================================================
//����������
//=============================================================
HRESULT CDifficulty::Init()
{
	CScene::Init();//�V�[������������
	CBgDifficulty* pBgDifficulty = CBgDifficulty::Create(CBg::BG_DIFFICULTY);
	CUi* pUi = nullptr;
	pBgDifficulty->SetUseTexMove(true, D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR2(0.04f, 0.0f));

	//��Փx�uEASY�v�̃e�L�X�gUI�𐶐�
	m_pDifficultyUi[DIFFICULTYTYPE_EASY] = CUi::Create(CUi::UITYPE_EASY_TEXT, CObject2D::POLYGONTYPE01_SENTERROLLING, 200.0f, 100.0f,
		D3DXVECTOR3(SENTER_VECTOR3.x, 200.0f, 0.0f), NULL_VECTOR3, NULL_VECTOR3, NORMAL_COL);

	//��Փx�uNORMAL�v�̃e�L�X�gUI�𐶐�
	m_pDifficultyUi[DIFFICULTYTYPE_NORMAL] = CUi::Create(CUi::UITYPE_NORMAL_TEXT, CObject2D::POLYGONTYPE01_SENTERROLLING, 200.0f, 100.0f,
		D3DXVECTOR3(SENTER_VECTOR3.x, 200.0f * 2, 0.0f), NULL_VECTOR3, NULL_VECTOR3, NORMAL_COL);

	//��Փx�uBOSSRUSH�v�̃e�L�X�gUI�𐶐�
	m_pDifficultyUi[DIFFICULTYTYPE_BOSSRUSH] = CUi::Create(CUi::UITYPE_BOSSRUSH_TEXT, CObject2D::POLYGONTYPE01_SENTERROLLING, 200.0f, 100.0f,
		D3DXVECTOR3(SENTER_VECTOR3.x, 200.0f * 3, 0.0f), NULL_VECTOR3, NULL_VECTOR3, NORMAL_COL);


	//��Փx�I���̃e�L�X�gUI�𐶐�
	pUi = CUi::Create(CUi::UITYPE_CHOOSEDIFFICULTY_TEXT, CObject2D::POLYGONTYPE01_SENTERROLLING, 300.0f, 120.0f,
		D3DXVECTOR3(SENTER_VECTOR3.x, 75.0f, 0.0f), NULL_VECTOR3,NULL_VECTOR3, NORMAL_COL);
	pUi->SetUseDeath(true);

	//�u�X�^�[�g��Enter�������v�̃e�L�X�gUI�𐶐�
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
//�I������
//=============================================================
void CDifficulty::Uninit()
{
	CScene::Uninit();//�V�[���I������
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
//�X�V����
//=============================================================
void CDifficulty::Update()
{
	CScene::Update();//�V�[���X�V����

	ChooseDifficulty();//��Փx�I������

	if (CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN) == true || CManager::GetInputJoypad()->GetTrigger(CInputJoypad::JOYKEY_START) == true)
	{
		CGame::SetDifficulty(DIFFICULTYTYPE(s_nChooseDifficultyNum));
		CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_DECISION_000);
		CManager::GetSceneFade()->SetSceneFade(CFade::FADEMODE_IN, CScene::MODE_GAME);
	}
}
//=========================================================================================================================

//=============================================================
//�`�揈��
//=============================================================
void CDifficulty::Draw()
{
	CScene::Draw();//�V�[���`�揈��
}
//=========================================================================================================================

//=============================================================
//��Փx�I������
//=============================================================
void CDifficulty::ChooseDifficulty()
{
	bool bChoose = false;//�I�����Ă��邩�ǂ���
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

	TextStaging(bChoose);//�e�L�X�gUI�̉��o����

}
//=========================================================================================================================

//=============================================================
//�e�L�X�gUI�̉��o����
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