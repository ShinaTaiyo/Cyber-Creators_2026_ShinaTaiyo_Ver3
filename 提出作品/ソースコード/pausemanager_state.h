//===============================================================================
//
//�Q�O�Q�T�N�R���R�P���F�|�[�Y��������������[pausemanager_state.h]
//Author:ShinaTaiyo
//
//===============================================================================

#ifndef _PAUSEMANAGER_STATE_H_  
#define _PAUSEMANAGER_STATE_H_

//========================================
//�C���N���[�h
//========================================
#include "main.h"
#include "pausemanager.h"
//===============================================================================

//========================================
//�N���X
//========================================

//�|�[�Y�X�e�[�g�N���X
class CPauseState_Component
{
public:
	CPauseState_Component(CPauseManager* pPauseManager);         //�R���X�g���N�^
	virtual ~CPauseState_Component();                            //�f�X�g���N�^
	virtual void Update(CPauseManager* pPauseManager);           //�X�V����
private:
};

//�|�[�Y�ʏ�X�e�[�g�N���X
class CPauseState_Normal : public CPauseState_Component
{
public:
	CPauseState_Normal(CPauseManager* pPauseManager);           //�R���X�g���N�^
	~CPauseState_Normal() override;                             //�f�X�g���N�^
	void Update(CPauseManager* pPauseManager) override;         //�X�V����
private:
	//===========================================
    //�񋓌^
    //===========================================
	enum class COMMANDTYPE
	{
		CONTINUE = 0,       //�Q�[���ɖ߂�
		EXIT,               //�^�C�g���ɖ߂�
		CAMERASENSITIVITY,  //�J�������x����
		MAX
	};//�R�}���h�񋓌^
	//=====================================================================================================================

	//===========================================
	//�ÓI�����o�ϐ�
	//===========================================

	//=====================================================================================================================

	//===========================================
	//�ϐ�
	//===========================================
	CUi* m_pCommandUi[static_cast<int>(COMMANDTYPE::MAX)] = {};//�R�}���hUI
	int m_nChooseCommandNum = 0;                               //�I��ł���R�}���h�̔ԍ�

	//=====================================================================================================================

	//===========================================
	//�֐�
	//===========================================
	void ChooseCommand();                                                  //�R�}���h�I��
	void ShowSelectedCommand();                                            //�I�������R�}���h��\�����鏈��
	void ExecuteCommand(CPauseManager * pPauseManager);                    //�R�}���h�𔭓����鏈��
	//=====================================================================================================================

};

//�J�������x�����N���X
class CPauseState_CameraSensitivity : public CPauseState_Component
{
public:
	CPauseState_CameraSensitivity(CPauseManager* pPauseManager);           //�R���X�g���N�^
	~CPauseState_CameraSensitivity() override;                             //�f�X�g���N�^
	void Update(CPauseManager* pPauseManager) override;         //�X�V����
private:
	//===========================================
    //�񋓌^
   //===========================================
	enum class COMMANDTYPE
	{
		SENSITIVITY = 0,       //�J�������x����������
		RETURN,                //�R�}���h�I����ʂɖ߂�
		MAX
	};//�R�}���h�񋓌^
	//=====================================================================================================================

	//===========================================
    //�ϐ�
    //===========================================

	CUi * m_pSensitivityLevelStick = nullptr;                   //���x���x����\���_
	CUi* m_pNowSensitivityLevelVar = nullptr;                   //���x���x����\���o�[
	CUi* m_pReturnTex = nullptr;                                //�߂�e�N�X�`��
	int m_nSensitivitLevel = 10;                                //���݂̊��x���x��
	int m_nChooseCommandNum = 0;                                //�I��ł���R�}���h�̔ԍ�
	//=====================================================================================================================

	//===========================================
	//�֐�
	//===========================================
	void ChooseCommand();                                 //�R�}���h�I������
	void CameraSensitivity();                             //�J�������x����
	void Return(CPauseManager * pPauseManager);           //�R�}���h�I����ʂɖ߂�
	//=====================================================================================================================
};

#endif