//======================================================================================================================
//
// �Q�O�Q�T�N�U���W���FOBB����[game.cpp]
//Author:ShinaTaiyo
//
//======================================================================================================================

//**********************************************************************************************************************
// �C���N���[�h
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
// �ÓI�����o�錾
//**********************************************************************************************************************
CPlayer* CGame::m_pPlayer = nullptr; // �v���C���[�ւ̃|�C���^
CStageManager* CGame::m_pStageManager = nullptr; // �X�e�[�W�}�l�[�W���[�ւ̃|�C���^
CPhaseManager* CGame::m_pPhaseManager = nullptr; // �t�F�[�Y�}�l�[�W���[�ւ̃|�C���^
CTutorial* CGame::m_pTutorial = nullptr; // �`���[�g���A���ւ̃|�C���^
int CGame::s_nPhaseNum = 0; // �t�F�[�Y�ԍ�
CScore* CGame::s_pSCORE = nullptr;  // �X�R�A�ւ̃|�C���^
CCombo* CGame::s_pCOMBO = nullptr;  // �R���{�ւ̃|�C���^
bool CGame::s_bGameClear = false;   // �Q�[�����N���A�������ǂ���
bool CGame::s_bPAUSE = false; // �|�[�Y�����邩�ǂ���
bool CGame::s_bCHENGEPAUSE = false; // �|�[�Y���I��点��t���O

//=====================================================================================================================
// 
// public�����o 
//
//=====================================================================================================================

//===================================================================================================================
// �R���X�g���N�^
//===================================================================================================================
CGame::CGame(bool bUseGamePad)
{
	m_pPlayer = nullptr;        // �v���C���[�ւ̃|�C���^��������
	m_pStageManager = nullptr;  // �X�e�[�W�}�l�[�W���[�ւ̃|�C���^��������
	m_pPhaseManager = nullptr;  // �t�F�[�Y�}�l�[�W���[�ւ̃|�C���^��������
	m_pTutorial = nullptr;      // �`���[�g���A���ւ̃|�C���^��������
	s_pSCORE = nullptr;         // �X�R�A�ւ̃|�C���^��������
	s_pCOMBO = nullptr;         // �R���{�ւ̃|�C���^��������
	s_nPhaseNum = 0;            // �t�F�[�Y�ԍ���������
	s_bGameClear = false;       // �Q�[�����N���A�������ǂ����i�����ݒ�̓N���A���Ă��Ȃ���ԁj
	s_bPAUSE = false;           // �|�[�Y�����邩�ǂ�����������
	m_pPauseManager = nullptr;  // �|�[�Y�}�l�[�W���[�ւ̃|�C���^��������
	s_bCHENGEPAUSE = false;     // �|�[�Y��ԕύX�t���O
	m_pAttackEnemy = nullptr;   // �G�̍U���ւ̃|�C���^
}

//===================================================================================================================
// �f�X�g���N�^
//===================================================================================================================
CGame::~CGame()
{
	// �����Ȃ�
}

//===================================================================================================================
// ����������
//===================================================================================================================
HRESULT CGame::Init()
{
#ifndef _DEBUG
	ShowCursor(FALSE);//�J�[�\�����\����
#endif // !_DEBUG

	CScene::Init();// �V�[������������
	CObjectX::SetCommonDraw(true); // �S�Ẳe��`�悷��

	// 3D�w�i�𐶐�
	CBg3D::Create(CBg3D::TYPE::SKY, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1200.0f, 1200.0f, 1200.0f));

	CCamera* pCamera = CManager::GetCamera();       // �J�����ւ̃|�C���^
	pCamera->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f)); // ������O�ɐݒ�
	CCameraState_Normal * pNormal = dynamic_cast<CCameraState_Normal*>(pCamera->GetCameraState()); // �J�������R�O�t���[������s�\�ɂ���
	
	// �J�����ʏ��Ԃ�����΁A��������s�\���Ԃ�ݒ�
	if (pNormal)
	{
		// �� �J�[�\���𒆉��ɌŒ肵�����A�������瓮�����ʂɉ����ăJ�����𓮂����Ă���̂ŁA�ŏ��ɃJ�[�\���𒆉��ɐݒ肵�܂��B
		// ��ʂ̒��S���W
		POINT center = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
		// �}�E�X�𒆉��Ƀ��Z�b�g
		SetCursorPos(static_cast<int>(center.x), static_cast<int>(center.y));
		pNormal->SetNoControlRotFrame(30);
	}

	m_pPhaseManager = CPhaseManager::Create(); // �t�F�[�Y�}�l�[�W���[�𐶐�
	m_pPhaseManager->SetUseDeath(false); // ���S�t���O���g�p���Ȃ�

	m_pStageManager = CStageManager::Create(); // �X�e�[�W�}�l�[�W���[�𐶐�
	m_pStageManager->SetUseDeath(false); // ���S�t���O���g�p���Ȃ�
	m_pStageManager->LoadMapTxt(0);      // �}�b�v�O��ǂݍ���

	m_pTutorial = CTutorial::Create(); // �`���[�g���A���𐶐�
	m_pTutorial->SetUseDeath(false);   // ���S�t���O���g�p���Ȃ�

	 // �X�R�A�̐���
	s_pSCORE = CScore::Create(D3DXVECTOR3(SCREEN_WIDTH - CScore::GetNumberWidth(),CScore::GetNumberHeight(), 0.0f));
	s_pSCORE->SetUseDeath(false); // ���S�t���O���g�p���Ȃ�

	s_pCOMBO = CCombo::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f)); // �R���{�𐶐�
	s_pCOMBO->SetUseDeath(false); // ���S�t���O���g�p���Ȃ�

	// �`���[�g���A�����g�p���Ȃ��Ȃ�
	if (s_bUSETUTORIAL == false)
	{
		// �S�Ẵ`���[�g���A��������������
		for (int nCnt = 0; nCnt < static_cast<int>(CTutorial::CHECK::MAX); nCnt++)
		{
			m_pTutorial->SetSuccessCheck(static_cast<CTutorial::CHECK>(nCnt));
		}
	}

	// �v���C���[�𐶐�
	m_pPlayer = CPlayer::Create(m_pStageManager->GetSpawnPoint(), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	m_pPlayer->SetUseDeath(false); // ���S�t���O���g�p���Ȃ�

	CManager::GetSound()->PlaySoundB(CSound::SOUND_LABEL::BGM_FINALBATTLE); // BGM���Đ�

	// OBB����m�F�p�I�u�W�F�N�g����
	{
		//for (int nCnt = 0; nCnt < 20; nCnt++)
		//{
		//	// �����l
		//	float fYaw = static_cast<float>(rand() % 100 - 50) * 1e-5f;   // Yaw
		//	float fPitch = static_cast<float>(rand() % 100 - 50) * 1e-5f; // Pitch
		//	float fRoll = static_cast<float>(rand() % 100 - 50) * 1e-5f;  // Roll

		//	// �G�̍U���𐶐�
		//	CAttackEnemy* pAttackEnemy = CAttackEnemy::Create(
		//		CAttack::ATTACKTYPE::MAGICSWORD, CAttack::TARGETTYPE::PLAYER, CAttack::COLLISIONTYPE::OBBTOOBB, false, true, 1, 6, 6000,
		//		D3DXVECTOR3(0.0f, 200.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(10.0f, 10.0f, 50.0f));
		//	pAttackEnemy->GetRotInfo().SetUseAddRot(true, D3DXVECTOR3(fPitch, fYaw, fRoll) * static_cast<float>(nCnt + 1)); // ��]������
		//	pAttackEnemy->SetCollisionRelease(false); // �������Ă��j������Ȃ�
		//}
	}
	return S_OK;
}

//===================================================================================================================
// �I������
//===================================================================================================================
void CGame::Uninit()
{
#ifndef _DEBUG
	ShowCursor(TRUE);//�J�[�\�����\����
#endif

	// �v���C���[�I��
	if (m_pPlayer != nullptr)
	{
		m_pPlayer->SetUseDeath(true); // ���S�t���O���g�p����
		m_pPlayer->SetDeath(); // ���S�t���O��ݒ肷��
		m_pPlayer = nullptr;   // �v���C���[�ւ̃|�C���^��������
	}

	// �X�e�[�W�}�l�[�W���[�I��
	if (m_pStageManager != nullptr)
	{
		m_pStageManager->SetUseDeath(true); // ���S�t���O���g�p����
		m_pStageManager->SetDeath(); // ���S�t���O��ݒ肷�� 
		m_pStageManager = nullptr;   // �X�e�[�W�}�l�[�W���[�ւ̃|�C���^��������
	}

	// �t�F�[�Y�}�l�[�W���[�I��
	if (m_pPhaseManager != nullptr)
	{
		m_pPhaseManager->SetUseDeath(true); // ���S�t���O���g�p����
		m_pPhaseManager->SetDeath(); // ���S�t���O��ݒ肷��
		m_pPhaseManager = nullptr;   // �t�F�[�Y�}�l�[�W���[�ւ̃|�C���^��������
	}

	// �|�[�Y�}�l�[�W���[�I��
	if (m_pPauseManager != nullptr)
	{
		m_pPauseManager->SetUseDeath(true); // ���S�t���O���g�p����
		m_pPauseManager->SetDeath(); // ���S�t���O��ݒ肷��
		m_pPauseManager = nullptr;   // �t�F�[�Y�}�l�[�W���[�ւ̃|�C���^��������
	}

	// �`���[�g���A���I��
	if (m_pTutorial != nullptr)
	{
		m_pTutorial->SetUseDeath(true); // ���S�t���O���g�p����
		m_pTutorial->SetDeath(); //���S�t���O��ݒ肷��
		m_pTutorial = nullptr;   //�`���[�g���A���ւ̃|�C���^��������
	}

	// �X�R�A�I��
	if (s_pSCORE != nullptr)
	{
		s_pSCORE->SetUseDeath(true); // ���S�t���O���g�p����
		s_pSCORE->SetDeath(); // ���S�t���O��ݒ肷��
		s_pSCORE = nullptr;   // �X�R�A�ւ̃|�C���^��������
	}
	
	// �R���{�I��
	if (s_pCOMBO != nullptr)
	{
		s_pCOMBO->SetUseDeath(true); // ���S�t���O���g�p����
		s_pCOMBO->SetDeath(); // ���S�t���O��ݒ肷��
		s_pCOMBO = nullptr;   // �X�R�A�ւ̃|�C���^��������
	}

	// �G�̍U���I��
	if (m_pAttackEnemy)
	{
		m_pAttackEnemy->SetUseDeath(true); // ���S�t���O���g�p����
		m_pAttackEnemy->SetDeath();     // ���S�t���O��ݒ肷��
		m_pAttackEnemy = nullptr;       // �G�̍U���ւ̃|�C���^��������
	}

	s_bPAUSE = false;//�|�[�Y���ɃQ�[�����[�h���I��点��Ƃ����|�[�Y��Ԃ���������

	CManager::GetSound()->Stop(); // �S�ẴT�E���h���~

	CScene::Uninit(); // �V�[���I������
}

//===================================================================================================================
// �X�V����
//===================================================================================================================
void CGame::Update()
{
	// �|�[�Y��Ԃ���Ȃ��Ƃ������Q�[���V�[������͉�ʑJ�ڏo���Ȃ�
	if (!s_bPAUSE)
	{
		// ENTER�L�[�A���̓X�^�[�g�{�^�������������ʑJ�ڊJ�n
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN) == true || CManager::GetInputJoypad()->GetTrigger(CInputJoypad::JOYKEY::START) == true)
		{
			CManager::GetSceneFade()->SetSceneFade(CFade::FADEMODE_IN, CScene::MODE_RESULT);
		}
	}

	PauseProcess(); // �|�[�Y����

	//�f�o�b�O�\��
	CManager::GetDebugText()->PrintDebugText("���݂̓G�̐��G%d\n", CEnemy::GetNumEnemy());
}

//===================================================================================================================
// �Q�[�����N���A�������ǂ����ݒ�
//===================================================================================================================
void CGame::SetGameClear(bool bClear)
{
	s_bGameClear = bClear;
}

//===================================================================================================================
// �Q�[�����N���A�������ǂ����擾
//===================================================================================================================
const bool& CGame::GetGameClear()
{
	return s_bGameClear;
}

//===================================================================================================================
// �v���C���[�擾
//===================================================================================================================
CPlayer* CGame::GetPlayer()
{
	return m_pPlayer;
}

//===================================================================================================================
// �t�F�[�Y�}�l�[�W���[�擾
//===================================================================================================================
CPhaseManager* CGame::GetPhaseManager()
{
	return m_pPhaseManager;
}

//===================================================================================================================
// �X�e�[�W�}�l�[�W���[�擾
//===================================================================================================================
CStageManager* CGame::GetStageManager()
{
	return m_pStageManager;
}

//===================================================================================================================
// �`���[�g���A���擾
//===================================================================================================================
CTutorial* CGame::GetTutorial()
{
	return m_pTutorial;
}

//===================================================================================================================
// �X�R�A�擾
//===================================================================================================================
CScore* CGame::GetScore()
{
	return s_pSCORE;
}

//===================================================================================================================
// �R���{�擾
//===================================================================================================================
CCombo* CGame::GetCombo()
{
	return s_pCOMBO;
}

//===================================================================================================================
// �|�[�Y��Ԃ��ǂ����擾
//===================================================================================================================
const bool& CGame::GetPauseFlag()
{
	return s_bPAUSE;
}

//===================================================================================================================
// �|�[�Y��ԕύX�t���O�ݒ�
//===================================================================================================================
void CGame::SetChengePause()
{
	s_bCHENGEPAUSE = true;
}

//=====================================================================================================================
// 
// private�����o 
//
//=====================================================================================================================

//===================================================================================================================
// �|�[�Y����
//===================================================================================================================
void CGame::PauseProcess()
{
	CInputKeyboard* pInputKeyboard = CManager::GetInputKeyboard(); // �L�[���͏����擾����
	CInputJoypad* pInputJoypad = CManager::GetInputJoypad(); // �W���C�p�b�h���͏����擾����
	CDebugText* pDebugText = CManager::GetDebugText(); // �f�o�b�O�����擾����

	//P�L�[�������A���̓|�[�Y��ԕύX�t���O�𔭓�������|�[�Y���̏������Ăяo������߂����肷��
	if (pInputKeyboard->GetTrigger(DIK_P) || pInputJoypad->GetTrigger(CInputJoypad::JOYKEY::BACK)|| s_bCHENGEPAUSE)
	{
		s_bPAUSE = s_bPAUSE ? false : true; // �t���O��؂�ւ���

		// �|�[�Y��
		if (s_bPAUSE)
		{
			m_pPauseManager = CPauseManager::Create();    // �|�[�Y��ԂȂ̂ŁA�|�[�Y�}�l�[�W���[�𐶐�
			m_pPauseManager->SetUseDeath(false);          // ���S�t���O���g�p���Ȃ�
			m_pPauseManager->SetIsStopUpdatePause(false); // �|�[�Y���ɍX�V���~�߂Ȃ�
		}
		// �|�[�Y������
		else
		{
			// �j������
			if (m_pPauseManager != nullptr)
			{
				m_pPauseManager->SetUseDeath(true); // ���S�t���O���g�p����
				m_pPauseManager->SetDeath(); // ���S�t���O��ݒ肷��
				m_pPauseManager = nullptr;   // �t�F�[�Y�}�l�[�W���[�ւ̃|�C���^��������
			}
		}
	}

	s_bCHENGEPAUSE = false; // �|�[�Y��ԕύX�t���O���������i�O�����炢����Ȃ�����A���false�B�t���O�ݒ菈�����Ă񂾃t���[������true�ɂȂ�B)

	// �f�o�b�O�\��
	pDebugText->PrintDebugText("�|�[�Y�����ǂ���(P)�F%d\n", s_bPAUSE);
}