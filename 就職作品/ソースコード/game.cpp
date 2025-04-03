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
//�ÓI�����o�錾
//=============================================================
CPlayer* CGame::m_pPlayer = nullptr;             //�v���C���[�ւ̃|�C���^
CStageManager* CGame::m_pStageManager = nullptr; //�X�e�[�W�}�l�[�W���[�ւ̃|�C���^
CPhaseManager* CGame::m_pPhaseManager = nullptr; //�t�F�[�Y�}�l�[�W���[�ւ̃|�C���^
CTutorial* CGame::m_pTutorial = nullptr;         //�`���[�g���A���ւ̃|�C���^
int CGame::s_nPhaseNum = 0;                      //�t�F�[�Y�ԍ�
CScore* CGame::s_pSCORE = nullptr;               //�X�R�A�ւ̃|�C���^
CCombo* CGame::s_pCOMBO = nullptr;               //�R���{�ւ̃|�C���^
bool CGame::s_bGameClear = false;                //�Q�[�����N���A�������ǂ���
bool CGame::s_bPAUSE = false;                    //�|�[�Y�����邩�ǂ���
bool CGame::s_bCHENGEPAUSE = false;                 //�|�[�Y���I��点��t���O
//=========================================================================================================================

//=============================================================
//�R���X�g���N�^
//=============================================================
CGame::CGame(bool bUseGamePad)
{
	m_pPlayer = nullptr;        //�v���C���[�ւ̃|�C���^��������
	m_pStageManager = nullptr;  //�X�e�[�W�}�l�[�W���[�ւ̃|�C���^��������
	m_pPhaseManager = nullptr;  //�t�F�[�Y�}�l�[�W���[�ւ̃|�C���^��������
	m_pTutorial = nullptr;      //�`���[�g���A���ւ̃|�C���^��������
	s_pSCORE = nullptr;         //�X�R�A�ւ̃|�C���^��������
	s_pCOMBO = nullptr;         //�R���{�ւ̃|�C���^��������
	s_nPhaseNum = 0;            //�t�F�[�Y�ԍ���������
	s_bGameClear = false;       //�Q�[�����N���A�������ǂ����i�����ݒ�̓N���A���Ă��Ȃ���ԁj
	s_bPAUSE = false;           //�|�[�Y�����邩�ǂ�����������
	m_pPauseManager = nullptr;  //�|�[�Y�}�l�[�W���[�ւ̃|�C���^��������
	s_bCHENGEPAUSE = false;     //�|�[�Y��ԕύX�t���O
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
#ifndef _DEBUG
	ShowCursor(FALSE);//�J�[�\�����\����
#endif // !_DEBUG

	CScene::Init();//�V�[������������
	CObjectX::SetCommonDraw(true);//�S�Ẳe��`�悷��

	CBg3D::Create(CBg3D::BG3DTYPE::SKY, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1200.0f, 1200.0f, 1200.0f));//3D�w�i�𐶐�

	m_pPhaseManager = CPhaseManager::Create();//�t�F�[�Y�}�l�[�W���[�𐶐�
	m_pPhaseManager->SetUseDeath(false);      //���S�t���O���g�p���Ȃ�

	m_pStageManager = CStageManager::Create();//�X�e�[�W�}�l�[�W���[�𐶐�
	m_pStageManager->SetUseDeath(false);      //���S�t���O���g�p���Ȃ�
	m_pStageManager->LoadMapTxt(0);           //�}�b�v�O��ǂݍ���

	m_pTutorial = CTutorial::Create();        //�`���[�g���A���𐶐�
	m_pTutorial->SetUseDeath(false);          //���S�t���O���g�p���Ȃ�

	s_pSCORE = CScore::Create(D3DXVECTOR3(SCREEN_WIDTH - CScore::GetNumberWidth(),CScore::GetNumberHeight(), 0.0f)); //�X�R�A�̐���
	s_pSCORE->SetUseDeath(false);                                                      //���S�t���O���g�p���Ȃ�

	s_pCOMBO = CCombo::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));//�R���{�𐶐�
	s_pCOMBO->SetUseDeath(false);//���S�t���O���g�p���Ȃ�

	if (s_bUSETUTORIAL == false)
	{//�`���[�g���A�����g�p���Ȃ��Ȃ�
		for (int nCnt = 0; nCnt < static_cast<int>(CTutorial::CHECK::MAX); nCnt++)
		{//�S�Ẵ`���[�g���A��������������
			m_pTutorial->SetSuccessCheck(static_cast<CTutorial::CHECK>(nCnt));
		}
	}
	m_pPlayer = CPlayer::Create(m_pStageManager->GetSpawnPoint(), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f));//�v���C���[�𐶐�
	m_pPlayer->SetUseDeath(false);//���S�t���O���g�p���Ȃ�

	CManager::GetSound()->PlaySoundB(CSound::SOUND_LABEL::BGM_FINALBATTLE);//BGM���Đ�
	return S_OK;
}
//=========================================================================================================================

//=============================================================
//�I������
//=============================================================
void CGame::Uninit()
{
#ifndef _DEBUG
	ShowCursor(TRUE);//�J�[�\�����\����
#endif

	//============================================
	//�v���C���[�̔j��
	//============================================
	if (m_pPlayer != nullptr)
	{
		m_pPlayer->SetUseDeath(true);//���S�t���O���g�p����
		m_pPlayer->SetDeath();       //���S�t���O��ݒ肷��
		m_pPlayer = nullptr;         //�v���C���[�̃|�C���^��������
	}
	//=====================================================================

	//============================================
	//�X�e�[�W�}�l�[�W���[�̔j��
	//============================================
	if (m_pStageManager != nullptr)
	{
		m_pStageManager->SetUseDeath(true);//���S�t���O���g�p����
		m_pStageManager->SetDeath();       //���S�t���O��ݒ肷�� 
		m_pStageManager = nullptr;         //�X�e�[�W�}�l�[�W���[�ւ̃|�C���^��������
	}
	//=====================================================================

	//============================================
	//�t�F�[�Y�}�l�[�W���[�̔j��
	//============================================
	if (m_pPhaseManager != nullptr)
	{
		m_pPhaseManager->SetUseDeath(true);//���S�t���O���g�p����
		m_pPhaseManager->SetDeath();       //���S�t���O��ݒ肷��
		m_pPhaseManager = nullptr;         //�t�F�[�Y�}�l�[�W���[�ւ̃|�C���^��������
	}
	//=====================================================================

    //============================================
	//�|�[�Y�}�l�[�W���[�̔j��
	//============================================
	if (m_pPauseManager != nullptr)
	{
		m_pPauseManager->SetUseDeath(true);//���S�t���O���g�p����
		m_pPauseManager->SetDeath();       //���S�t���O��ݒ肷��
		m_pPauseManager = nullptr;         //�t�F�[�Y�}�l�[�W���[�ւ̃|�C���^��������
	}
	//=====================================================================


	//============================================
	//�`���[�g���A���̔j��
	//============================================
	if (m_pTutorial != nullptr)
	{
		m_pTutorial->SetUseDeath(true);//���S�t���O���g�p����
		m_pTutorial->SetDeath();       //���S�t���O��ݒ肷��
		m_pTutorial = nullptr;         //�`���[�g���A���ւ̃|�C���^��������
	}
	//=====================================================================

	//============================================
	//�X�R�A�̔j��
	//============================================
	if (s_pSCORE != nullptr)
	{
		s_pSCORE->SetUseDeath(true); //���S�t���O���g�p����
		s_pSCORE->SetDeath();        //���S�t���O��ݒ肷��
		s_pSCORE = nullptr;          //�X�R�A�ւ̃|�C���^��������
	}
	//=====================================================================
	
	//============================================
	//�R���{�̔j��
	//============================================
	if (s_pCOMBO != nullptr)
	{
		s_pCOMBO->SetUseDeath(true); //���S�t���O���g�p����
		s_pCOMBO->SetDeath();        //���S�t���O��ݒ肷��
		s_pCOMBO = nullptr;          //�X�R�A�ւ̃|�C���^��������
	}
	//=====================================================================

	s_bPAUSE = false;//�|�[�Y���ɃQ�[�����[�h���I��点��Ƃ����|�[�Y��Ԃ���������

	CManager::GetSound()->Stop();//�S�ẴT�E���h���~

	CScene::Uninit();//�V�[���I������
}
//=========================================================================================================================

//=============================================================
//�X�V����
//=============================================================
void CGame::Update()
{
	if (!s_bPAUSE)
	{//�|�[�Y��Ԃ���Ȃ��Ƃ������Q�[���V�[������͉�ʑJ�ڏo���Ȃ�
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN) == true || CManager::GetInputJoypad()->GetTrigger(CInputJoypad::JOYKEY::START) == true)
		{//ENTER�L�[�A���̓X�^�[�g�{�^���������Ă�����
			CManager::GetSceneFade()->SetSceneFade(CFade::FADEMODE_IN, CScene::MODE_RESULT);//���U���g�V�[�����Ă�
		}
	}
	PauseProcess();//�|�[�Y����

	//�f�o�b�O�\��
	CManager::GetDebugText()->PrintDebugText("���݂̓G�̐��G%d\n", CEnemy::GetNumEnemy());
}
//=========================================================================================================================

//=============================================================
//�|�[�Y����
//=============================================================
void CGame::PauseProcess()
{
	CInputKeyboard* pInputKeyboard = CManager::GetInputKeyboard(); //�L�[���͏����擾����
	CInputJoypad* pInputJoypad = CManager::GetInputJoypad();       //�W���C�p�b�h���͏����擾����
	CDebugText* pDebugText = CManager::GetDebugText();             //�f�o�b�O�����擾����

	if (pInputKeyboard->GetTrigger(DIK_P) || pInputJoypad->GetTrigger(CInputJoypad::JOYKEY::BACK)|| s_bCHENGEPAUSE)
	{//P�L�[�������A���̓|�[�Y��ԕύX�t���O�𔭓�����
		s_bPAUSE = s_bPAUSE ? false : true;//�t���O��؂�ւ���
		if (s_bPAUSE)
		{//�|�[�Y��
			m_pPauseManager = CPauseManager::Create();    //�|�[�Y��ԂȂ̂ŁA�|�[�Y�}�l�[�W���[�𐶐�
			m_pPauseManager->SetUseDeath(false);          //���S�t���O���g�p���Ȃ�
			m_pPauseManager->SetIsStopUpdatePause(false); //�|�[�Y���ɍX�V���~�߂Ȃ�
		}
		else
		{//�|�[�Y������
			if (m_pPauseManager != nullptr)
			{//�j������
				m_pPauseManager->SetUseDeath(true);       //���S�t���O���g�p����
				m_pPauseManager->SetDeath();              //���S�t���O��ݒ肷��
				m_pPauseManager = nullptr;                //�t�F�[�Y�}�l�[�W���[�ւ̃|�C���^��������
			}
		}
	}

	s_bCHENGEPAUSE = false;//�|�[�Y��ԕύX�t���O���������i�O�����炢����Ȃ�����A���false�B�t���O�ݒ菈�����Ă񂾃t���[������true�ɂȂ�B)

	pDebugText->PrintDebugText("�|�[�Y�����ǂ���(P)�F%d\n", s_bPAUSE);
}
//=========================================================================================================================