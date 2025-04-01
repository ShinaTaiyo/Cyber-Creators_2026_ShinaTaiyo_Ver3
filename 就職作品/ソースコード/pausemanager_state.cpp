//==============================================================================================================
//
//�Q�O�Q�T�N�R���R�P���F�|�[�Y��������������[pausemanager_state.h]
//Author:ShinaTaiyo
//
//==============================================================================================================

//======================================================
//�C���N���[�h
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
//�R���|�[�l���g
//******************************************************

//======================================================
//�R���X�g���N�^
//======================================================
CPauseState_Component::CPauseState_Component(CPauseManager* pPauseManager)
{
	//���ɂȂ�
}
//=======================================================================================

//======================================================
//�f�X�g���N�^
//======================================================
CPauseState_Component::~CPauseState_Component()
{
	//���ɂȂ�
}
//=======================================================================================

//======================================================
//�X�V����
//======================================================
void CPauseState_Component::Update(CPauseManager* pPauseManager)
{
	//���ɂȂ�
}
//=======================================================================================

//******************************************************
//�ʏ�
//******************************************************

//======================================================
//�R���X�g���N�^
//======================================================
CPauseState_Normal::CPauseState_Normal(CPauseManager* pPauseManager) : CPauseState_Component(pPauseManager)
{
	m_pCommandUi[static_cast<int>(COMMANDTYPE::CONTINUE)] = CUi::Create(CUi::UITYPE::CONTINUETEXT_000, true, CObject2D::POLYGONTYPE::SENTERROLLING,
		200.0f, 100.0f, 1, false, D3DXVECTOR3(SCREEN_WIDTH / 2, 200.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));//�R���e�B�j���[�R�}���h����
	m_pCommandUi[static_cast<int>(COMMANDTYPE::EXIT)] = CUi::Create(CUi::UITYPE::EXITTEXT_000, true, CObject2D::POLYGONTYPE::SENTERROLLING,
		200.0f, 100.0f, 1, false, D3DXVECTOR3(SCREEN_WIDTH / 2, 400.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));//EXIT�R�}���h����
	m_pCommandUi[static_cast<int>(COMMANDTYPE::CAMERASENSITIVITY)] = m_pCommandUi[static_cast<int>(COMMANDTYPE::CAMERASENSITIVITY)] = CUi::Create(CUi::UITYPE::CAMERASENSITIVITYTEXT_000,
		true, CObject2D::POLYGONTYPE::SENTERROLLING, 200.0f, 100.0f, 1, false, D3DXVECTOR3(SCREEN_WIDTH / 2, 600.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));//EXIT�R�}���h����
	for (int nCnt = 0; nCnt < static_cast<int>(COMMANDTYPE::MAX); nCnt++)
	{//�S�R�}���h���ʐݒ�
		m_pCommandUi[nCnt]->SetUseDeath(false);//�S�ẴR�}���h�̎��S�t���O���g�p���Ȃ�
		m_pCommandUi[nCnt]->SetIsStopUpdatePause(false);//�|�[�Y���ɍX�V���~�߂Ȃ�
	}
}
//=======================================================================================

//======================================================
//�f�X�g���N�^
//======================================================
CPauseState_Normal::~CPauseState_Normal()
{
	for (int nCnt = 0; nCnt < static_cast<int>(COMMANDTYPE::MAX); nCnt++)
	{//�O�R�}���h���S�t���O�ݒ菈��
		if (m_pCommandUi[nCnt] != nullptr)
		{//�|�C���^�����݂��Ă�����
			m_pCommandUi[nCnt]->SetUseDeath(true); //�S�ẴR�}���h�̎��S�t���O���g�p����
			m_pCommandUi[nCnt]->SetDeath();        //���S�t���O��ݒ肷��
		}
	}
}
//=======================================================================================

//======================================================
//�X�V����
//======================================================
void CPauseState_Normal::Update(CPauseManager* pPauseManager)
{
	ChooseCommand();                //�R�}���h�I������
	ShowSelectedCommand();          //�I�������R�}���h��\�����鏈��
	ExecuteCommand(pPauseManager);  //�R�}���h��������
}
//=======================================================================================

//======================================================
//�R�}���h�I������
//======================================================
void CPauseState_Normal::ChooseCommand()
{
	CInputKeyboard* pInputKeyboard = CManager::GetInputKeyboard(); //�L�[���͏����擾
	CInputJoypad* pInputJoypad = CManager::GetInputJoypad();       //�W���C�p�b�h���͏����擾
	CDebugText* pDebugText = CManager::GetDebugText();             //�f�o�b�O�\�������擾
	int nNowChooseCoommand = m_nChooseCommandNum;                  //���ݑI��ł���R�}���h�ԍ����i�[
	if (pInputKeyboard->GetTrigger(DIK_S) || pInputJoypad->GetTrigger(CInputJoypad::JOYKEY::DOWN))
	{
		nNowChooseCoommand++;//���̃R�}���h�ɃC���N�������g
	}
	if (pInputKeyboard->GetTrigger(DIK_W) || pInputJoypad->GetTrigger(CInputJoypad::JOYKEY::UP))
	{
		nNowChooseCoommand--;//�O�̃R�}���h�ɃC���N�������g
	}
	nNowChooseCoommand = (nNowChooseCoommand + static_cast<int>(COMMANDTYPE::MAX)) % static_cast<int>(COMMANDTYPE::MAX);// �͈͐���
	m_nChooseCommandNum = nNowChooseCoommand;//�X�V

	pDebugText->PrintDebugText("�I�����Ă���|�[�Y�R�}���h�F%d\n", m_nChooseCommandNum);
}
//=======================================================================================

//======================================================
//�I���R�}���h��\�����鏈��
//======================================================
void CPauseState_Normal::ShowSelectedCommand()
{
	for (int nCnt = 0; nCnt < static_cast<int>(COMMANDTYPE::MAX); nCnt++)
	{
		if (nCnt == m_nChooseCommandNum)
		{
			m_pCommandUi[nCnt]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), false, 1.0f);//�I�𒆂̔ԍ��̃R�}���h�̐F�����ʏ�ɂ���
		}
		else
		{
			m_pCommandUi[nCnt]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f), false, 1.0f);//�I�����Ă��Ȃ��R�}���h�̐F�𔖂�����
		}
	}
}
//=======================================================================================

//======================================================
//�I���R�}���h�����s���鏈��
//======================================================
void CPauseState_Normal::ExecuteCommand(CPauseManager* pPauseManager)
{
	CInputKeyboard* pInputKeyboard = CManager::GetInputKeyboard(); //�L�[���͏����擾
	CInputJoypad* pInputJoypad = CManager::GetInputJoypad();       //�W���C�p�b�h���͏����擾
	CSceneFade* pSceneFade = CManager::GetSceneFade();             //�V�[���J�ڗp�t�F�[�h�����擾
	if (pInputKeyboard->GetTrigger(DIK_RETURN) || pInputJoypad->GetTrigger(CInputJoypad::JOYKEY::A))
	{//ENTER�L�[�A���̓W���C�p�b�h��A�{�^��
		switch (m_nChooseCommandNum)
		{
		case static_cast<int>(COMMANDTYPE::CONTINUE)://�I���R�}���h���u�R���e�B�j���[�v��������
			CGame::SetChengePause();//�|�[�Y��ԕύX�t���O��ON�ɂ��A�|�[�Y���I������			
			break;
		case static_cast<int>(COMMANDTYPE::EXIT)://�I���R�}���h���uEXIT(�^�C�g���ɖ߂�)�v��������
			pSceneFade->SetSceneFade(CFade::FADEMODE_IN, CScene::MODE_TITLE);//�^�C�g����ʂ֑J�ڂ���
			break;
		case static_cast<int>(COMMANDTYPE::CAMERASENSITIVITY)://�I���R�}���h���u�J�������x�����v��������
			pPauseManager->ChengeState(DBG_NEW CPauseState_CameraSensitivity(pPauseManager));
			break;
		default:
			break;
		}
	}
}
//=======================================================================================

//******************************************************
//�J�������x����
//******************************************************

//======================================================
//�R���X�g���N�^
//======================================================
CPauseState_CameraSensitivity::CPauseState_CameraSensitivity(CPauseManager* pPauseManager) : CPauseState_Component(pPauseManager)
{
	m_pSensitivityLevelStick = CUi::Create(CUi::UITYPE::LOCKON, true, CObject2D::POLYGONTYPE::SENTERROLLING, 200.0f, 50.0f, 1, false,
		D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	m_pSensitivityLevelStick->InitTextureOnly();            //�e�N�X�`�����g��Ȃ�
	m_pSensitivityLevelStick->SetUseDeath(false);           //���S�t���O���g�p���Ȃ�
	m_pSensitivityLevelStick->SetIsStopUpdatePause(false);  //�|�[�Y�����X�V����
	m_pNowSensitivityLevelVar = CUi::Create(CUi::UITYPE::LOCKON, true, CObject2D::POLYGONTYPE::SENTERROLLING, 20.0f, 80.0f, 1, false,
		D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	m_pNowSensitivityLevelVar->InitTextureOnly();           //�e�N�X�`�����g��Ȃ�
	m_pNowSensitivityLevelVar->SetUseDeath(false);          //���S�t���O���g�p���Ȃ�
	m_pNowSensitivityLevelVar->SetIsStopUpdatePause(false); //�|�[�Y�����X�V����

	m_pReturnTex = CUi::Create(CUi::UITYPE::RETURNTEXT_000, true, CObject2D::POLYGONTYPE::SENTERROLLING, 200.0f, 100.0f, 1, false,
		D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 120.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	m_pReturnTex->SetUseDeath(false);          //���S�t���O���g�p���Ȃ�
	m_pReturnTex->SetIsStopUpdatePause(false); //�|�[�Y�����X�V����

	m_nSensitivitLevel = CCamera::GetSensitivityLevel();//���݂̃J�����̊��x���x�����擾����
}
//=======================================================================================

//======================================================
//�f�X�g���N�^
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


	CCamera::SetSensitivityLevel(m_nSensitivitLevel);//�J�����̊��x��ݒ肷��
}
//=======================================================================================

//======================================================
//�X�V����
//======================================================
void CPauseState_CameraSensitivity::Update(CPauseManager* pPauseManager)
{
	if (m_pSensitivityLevelStick != nullptr&&m_pNowSensitivityLevelVar != nullptr)
	{
		ChooseCommand();//�R�}���h�I������
		switch (m_nChooseCommandNum)
		{
		case static_cast<int>(COMMANDTYPE::SENSITIVITY):
			//�g�p���Ȃ��R�}���h�̐F�𔖂�����
			m_pReturnTex->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), true, 0.5f);
			//�g�p����R�}���h�̐F�����ɖ߂�
			m_pNowSensitivityLevelVar->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), true,1.0f);
			m_pSensitivityLevelStick->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), true, 1.0f);
			//�J�������x����
			CameraSensitivity();
			break;
		case static_cast<int>(COMMANDTYPE::RETURN):
			//�g�p���Ȃ��R�}���h�̐F�𔖂�����
			m_pNowSensitivityLevelVar->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), true, 0.5f);
			m_pSensitivityLevelStick->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), true, 0.5f);
			//�g�p����R�}���h�̐F�����ɖ߂�
			m_pReturnTex->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), true, 1.0f);
			//�R�}���h�I����ʂɖ߂�
			Return(pPauseManager);
			break;
		default:
			break;
		}
	}
}
//=======================================================================================

//======================================================
//�R�}���h�I������
//======================================================
void CPauseState_CameraSensitivity::ChooseCommand()
{
	CInputKeyboard* pInputKeyboard = CManager::GetInputKeyboard(); //�L�[���͏����擾
	CInputJoypad* pInputJoypad = CManager::GetInputJoypad();       //�W���C�p�b�h���͏����擾
	CDebugText* pDebugText = CManager::GetDebugText();             //�f�o�b�O�\�������擾
	int nNowChooseCoommand = m_nChooseCommandNum;                  //���ݑI��ł���R�}���h�ԍ����i�[
	if (pInputKeyboard->GetTrigger(DIK_S) || pInputJoypad->GetTrigger(CInputJoypad::JOYKEY::DOWN))
	{
		nNowChooseCoommand++;//���̃R�}���h�ɃC���N�������g
	}
	if (pInputKeyboard->GetTrigger(DIK_W) || pInputJoypad->GetTrigger(CInputJoypad::JOYKEY::UP))
	{
		nNowChooseCoommand--;//�O�̃R�}���h�ɃC���N�������g
	}
	nNowChooseCoommand = (nNowChooseCoommand + static_cast<int>(COMMANDTYPE::MAX)) % static_cast<int>(COMMANDTYPE::MAX);// �͈͐���
	m_nChooseCommandNum = nNowChooseCoommand;//�X�V

	pDebugText->PrintDebugText("�I�����Ă���J�������x�����R�}���h�F%d\n", m_nChooseCommandNum);

}
//=======================================================================================

//======================================================
//�J�������x����
//======================================================
void CPauseState_CameraSensitivity::CameraSensitivity()
{
	CInputKeyboard* pInputKeyboard = CManager::GetInputKeyboard(); //�L�[���͏����擾
	CInputJoypad* pInputJoypad = CManager::GetInputJoypad();       //�W���C�p�b�h���͏����擾
	CDebugText* pDebugText = CManager::GetDebugText();             //�f�o�b�O�\�������擾
	int nNowSensitivityLevel = m_nSensitivitLevel;                 //���ݑI��ł��銴�x���x�����i�[
	const int& nMaxSensitivityLevel = CCamera::GetMaxSensitivityLevel();//�ő労�x���x�����擾
	if (pInputKeyboard->GetTrigger(DIK_D) || pInputJoypad->GetTrigger(CInputJoypad::JOYKEY::RIGHT))
	{
		nNowSensitivityLevel++;//���̃R�}���h�ɃC���N�������g
	}
	if (pInputKeyboard->GetTrigger(DIK_A) || pInputJoypad->GetTrigger(CInputJoypad::JOYKEY::LEFT))
	{
		nNowSensitivityLevel--;//�O�̃R�}���h�ɃC���N�������g
	}

	nNowSensitivityLevel = ((nNowSensitivityLevel + nMaxSensitivityLevel)) % nMaxSensitivityLevel; // �͈͐���
	m_nSensitivitLevel = nNowSensitivityLevel;                 // �X�V

	const D3DXVECTOR3& StickPos = m_pSensitivityLevelStick->GetPos();  //���x���X�e�B�b�N�̈ʒu
	const float fStickWidth = m_pSensitivityLevelStick->GetWidth();    //�X�e�B�b�N�̉���
	const D3DXVECTOR3& VarPos = m_pNowSensitivityLevelVar->GetPos();   //���x���o�[�̈ʒu
	float fDivisionStickWidth = fStickWidth / nMaxSensitivityLevel;                      //�X�e�B�b�N�̉���
	float fVarWidth = m_pNowSensitivityLevelVar->GetWidth() / 2;       //���x���o�[�̉���
	m_pNowSensitivityLevelVar->SetPos(D3DXVECTOR3(StickPos.x - fStickWidth / 2 + fVarWidth + fDivisionStickWidth * m_nSensitivitLevel, StickPos.y, 0.0f));

	pDebugText->PrintDebugText("�J�������x���x���F%d\n", m_nSensitivitLevel + 1);
}
//=======================================================================================

//======================================================
//�R�}���h�I����ʂɖ߂�
//======================================================
void CPauseState_CameraSensitivity::Return(CPauseManager* pPauseManager)
{
	CInputKeyboard* pInputKeyboard = CManager::GetInputKeyboard(); //�L�[���͏����擾
	CInputJoypad* pInputJoypad = CManager::GetInputJoypad();       //�W���C�p�b�h���͏����擾
	if (pInputKeyboard->GetTrigger(DIK_RETURN) || pInputJoypad->GetTrigger(CInputJoypad::JOYKEY::A))
	{//ENTER�L�[�A���̓W���C�p�b�h��A�{�^��
		pPauseManager->ChengeState(DBG_NEW CPauseState_Normal(pPauseManager));//�R�}���h�I����ʂɖ߂�
	}
}
//=======================================================================================