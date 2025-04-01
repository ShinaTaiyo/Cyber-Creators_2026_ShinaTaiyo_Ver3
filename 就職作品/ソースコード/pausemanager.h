//===============================================================================
//
//�Q�O�Q�T�N�R���R�P���F�|�[�Y��������������[pausemanager.h]
//Author:ShinaTaiyo
//
//===============================================================================

#ifndef _PAUSEMANAGER_H_  
#define _PAUSEMANAGER_H_

//========================================
//�C���N���[�h
//========================================
#include "main.h"
#include "ui.h"
#include "object.h"
//===============================================================================

//========================================
//�O���錾
//========================================
class CPauseState_Component;//�|�[�Y�X�e�[�g�N���X
//===============================================================================

//========================================
//�|�[�Y�}�l�[�W���[�N���X
//========================================
class CPauseManager : public CObject
{
public:
	//�ʏ�
	CPauseManager(int nPriority = 0, bool bUseintPriority = false, TYPE Type = TYPE::NONE, OBJECTTYPE ObjType = OBJECTTYPE::OBJECTTYPE_NONE);//�R���X�g���N�^
	~CPauseManager() override;        //�f�X�g���N�^
	HRESULT Init() override;          //����������
	void Uninit() override;           //�I������
	void Update() override;           //�X�V����
	void Draw() override;             //�`�揈��
	void SetDeath() override;         //���S�t���O�ݒ菈��
	static CPauseManager* Create();   //��������

	//��
	void ChengeState(CPauseState_Component* pState);//�X�e�[�g�ύX����

private:
	CPauseState_Component* m_pPauseState = nullptr;//�|�[�Y�X�e�[�g
};

#endif