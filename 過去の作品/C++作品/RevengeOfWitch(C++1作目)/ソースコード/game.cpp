//=========================================================================================================================
//
//�V���P�P���F�I�u�W�F�N�g��Release���@�̕ύX[game.cpp]
//Author:ShinaTaiyo
//
//=========================================================================================================================

//=============================================================
//�C���N���[�h
//=============================================================
#include "game.h"
#include "bg.h"
#include "item.h"
#include "signboard.h"
#include "marker.h"
#include "manager.h"
#include "sound.h"
#include "fade.h"
#include "camera.h"
#include "input.h"
//=========================================================================================================================

//=============================================================
//�ÓI�����o�錾
//=============================================================
CPlayer* CGame::m_pPlayer = nullptr;
CStageManager* CGame::m_pStageManager = nullptr;
CBg3D* CGame::m_pBg3D = nullptr;
CField* CGame::m_pField = nullptr;
CTimer* CGame::m_pTimer = nullptr;
CDifficulty::DIFFICULTYTYPE CGame::m_DifficultyType = CDifficulty::DIFFICULTYTYPE_EASY;//��Փx
CEventManager* CGame::m_pEventManager = nullptr;
//=========================================================================================================================

//=============================================================
//�R���X�g���N�^
//=============================================================
CGame::CGame()
{
	m_pStageManager = nullptr;
	m_pPlayer = nullptr;
	m_pBg3D = nullptr;
	m_pField = nullptr;
	m_pTimer = nullptr;
	m_pEventManager = nullptr;
}
//=========================================================================================================================

//=============================================================
//�f�X�g���N�^
//=============================================================
CGame::~CGame()
{

}
//=========================================================================================================================

//=============================================================
//����������
//=============================================================
HRESULT CGame::Init()
{
	CScene::Init();//�V�[������������
	//============================
	//�X�e�[�W�}�l�[�W���[�̐���
	//============================
	m_pStageManager = CStageManager::Create();
	//======================================================

	switch (m_DifficultyType)
	{
	case CDifficulty::DIFFICULTYTYPE_EASY:
		m_pStageManager->LoadMapFilePass(CStageManager::WORLDTYPE00_EASY);
		m_pPlayer = CPlayer::Create(D3DXVECTOR3(-300.0f, 100.0f, 0.0f), NULL_VECTOR3, NULL_VECTOR3, ONE_VECTOR3);
		CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_BGM_STAGE1);
		break;
	case CDifficulty::DIFFICULTYTYPE_NORMAL:
		m_pStageManager->LoadMapFilePass(CStageManager::WORLDTYPE01_NORMAL);
		m_pPlayer = CPlayer::Create(D3DXVECTOR3(0.0f, 100.0f, 0.0f), NULL_VECTOR3, NULL_VECTOR3, ONE_VECTOR3);
		CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_BGM_GHOSTBUTTLE);
		break;
	case CDifficulty::DIFFICULTYTYPE_BOSSRUSH:
		m_pStageManager->LoadMapFilePass(CStageManager::WORLDTYPE02_BOSSRUSH);
		m_pPlayer = CPlayer::Create(D3DXVECTOR3(200.0f, 100.0f, 0.0f), NULL_VECTOR3, NULL_VECTOR3, ONE_VECTOR3);
		CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_BGM_GHOSTBUTTLE);
		break;
	default:
		break;
	}
	m_pStageManager->LoadMapTxt(0);   //�}�b�v���e�L�X�g�t�@�C������ǂݍ���
	
	//�^�C�}�[�̐���
	m_pTimer = CTimer::Create(0, D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f), 40.0f, 40.0f);
	return S_OK;
}
//=========================================================================================================================

//=============================================================
//�I������
//=============================================================
void CGame::Uninit()
{
	//===================================
	//3D�w�i�̔j��
	//===================================
	if (m_pBg3D != nullptr)
	{
		m_pBg3D->SetUseDeath(true);
		m_pBg3D->SetDeath();
		m_pBg3D = nullptr;
	}
	//====================================================================

	//===================================
	//�t�B�[���h�̔j��
	//===================================
	if (m_pField != nullptr)
	{
		m_pField->SetUseDeath(true);
		m_pField->SetDeath();
		m_pField = nullptr;
	}
	//====================================================================

	//===================================
    //�X�e�[�W�}�l�[�W���[�̔j��
    //===================================
	if (m_pStageManager != nullptr)
	{
		m_pStageManager->SetUseDeath(true);
		m_pStageManager->SetDeath();
		m_pStageManager = nullptr;
	}
	//====================================================================

	//===================================
	//�X�e�[�W�}�l�[�W���[�̔j��
	//===================================
	if (m_pEventManager != nullptr)
	{
		m_pEventManager->SetUseDeath(true);
		m_pEventManager->SetDeath();
		m_pEventManager = nullptr;
	}
	//====================================================================


	//============================================
	//�v���C���[�̔j��
	//============================================
	if (m_pPlayer != nullptr)
	{
		m_pPlayer->SetUseDeath(true);
		m_pPlayer->SetDeath();
		m_pPlayer = nullptr;
	}
	//=====================================================================

	//============================================
	//�^�C�}�[�̔j��
	//============================================
	if (m_pTimer != nullptr)
	{
		m_pTimer->SetUseDeath(true);
		m_pTimer->SetDeath();
		m_pTimer = nullptr;
	}
	//=====================================================================

	CManager::GetSound()->StopSound();

	CScene::Uninit();//�V�[���I������
}
//=========================================================================================================================

//=============================================================
//�X�V����
//=============================================================
void CGame::Update()
{
#ifdef _DEBUG
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN) == true || CManager::GetInputJoypad()->GetTrigger(CInputJoypad::JOYKEY_START) == true)
	{
		CManager::GetSceneFade()->SetSceneFade(CFade::FADEMODE_IN, CScene::MODE_RESULT);
	}
#else
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_1) == true)
	{
		CManager::GetSceneFade()->SetSceneFade(CFade::FADEMODE_IN, CScene::MODE_RESULT);
	}
#endif // !_DEBUG
}
//=========================================================================================================================