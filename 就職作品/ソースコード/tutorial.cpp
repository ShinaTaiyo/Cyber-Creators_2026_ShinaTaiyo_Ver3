//===================================================================================================
//
//�P�Q���Q�S���F�`���[�g���A������������[tutorial.cpp]
//Author:ShinaTaiyo
//
//===================================================================================================

//======================================================
//�C���N���[�h
//======================================================
#include "tutorial.h"
//===================================================================================================

//======================================================
//�R���X�g���N�^
//======================================================
CTutorial::CTutorial(int nPri, bool bUseintPri, CObject::TYPE type, CObject::OBJECTTYPE ObjType) : CObject(nPri,bUseintPri,type,ObjType)
{
	m_bSuccess = false;//�`���[�g���A�������������������

	//�`���[�g���A���p�̃e�N�X�`������
	m_pTutorialTex = CUi::Create(CUi::UITYPE::TUTORIAL_TEX, false, CObject2D::POLYGONTYPE::SENTERROLLING, 300.0f, 300.0f, 1, false, D3DXVECTOR3(SCREEN_WIDTH - 150.0f, SCREEN_HEIGHT - 150.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	m_pTutorialTex->SetUseDeath(false);//���S�t���O���g�p���Ȃ�

	for (int nCnt = 0; nCnt < static_cast<int>(CHECK::MAX); nCnt++)
	{//���ꂼ��̃`���[�g���A���N���A�󋵂̃`�F�b�N�}�[�N��UI�𐶐�
		m_pCheckPoint[nCnt] = CUi::Create(CUi::UITYPE::CHECKMARK_000, false, CObject2D::POLYGONTYPE::SENTERROLLING, 30.0f, 30.0f, 1, false,
			D3DXVECTOR3(SCREEN_WIDTH - 20.0f, SCREEN_HEIGHT -225.0f + 27.0f * (nCnt), 0.0f),D3DXVECTOR3(0.0f, 0.0f, 0.0f), 
			D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		m_pCheckPoint[nCnt]->SetUseDraw(false); //�`�悵�Ȃ�
		m_pCheckPoint[nCnt]->SetUseDeath(false);//���S�t���O���g�p���Ȃ�
	}
}
//===================================================================================================

//======================================================
//�f�X�g���N�^
//======================================================
CTutorial::~CTutorial()
{

}
//===================================================================================================

//======================================================
//����������
//======================================================
HRESULT CTutorial::Init()
{
	CObject::Init();//�I�u�W�F�N�g�̏���������
	return S_OK;
}
//===================================================================================================

//======================================================
//�I������
//======================================================
void CTutorial::Uninit()
{
	CObject::Uninit();//�I�u�W�F�N�g�̏I������
}
//===================================================================================================

//======================================================
//�X�V����
//======================================================
void CTutorial::Update()
{
	CObject::Update();//�I�u�W�F�N�g�̍X�V����

	int nDrawSuccess = 0;//�`�F�b�N�}�[�N�̕`��i�e�`���[�g���A�����ڃN���A�j���������J�E���g
	for (int nCnt = 0; nCnt < static_cast<int>(CHECK::MAX); nCnt++)
	{
		if (m_pCheckPoint[nCnt]->GetUseDraw() == true && m_bSuccess == false)
		{//�`�悳��Ă�����J�E���g
			nDrawSuccess++;
		}
	}

	if (nDrawSuccess == static_cast<int>(CHECK::MAX) && m_bSuccess == false)
	{//�N���A�����`���[�g���A�����ő吔�ɒB������
		m_pTutorialTex->SetUseDraw(false);//�`���[�g���A���\���e�N�X�`���̕`����I�t�ɂ���
		SetDrawReset();//�`�F�b�N�}�[�N�̕`���S�ăI�t
	}
}
//===================================================================================================

//======================================================
//�`�揈��
//======================================================
void CTutorial::Draw()
{
	CObject::Draw();//�I�u�W�F�N�g�̕`�揈��
}
//===================================================================================================

//======================================================
//���S�t���O�ݒ菈��
//======================================================
void CTutorial::SetDeath()
{
	if (GetUseDeath() == true)
	{//���S�t���O���g�p����Ȃ�
		for (int nCnt = 0; nCnt < static_cast<int>(CHECK::MAX); nCnt++)
		{
			if (m_pCheckPoint[nCnt] != nullptr)
			{//�`�F�b�N�}�[�N��j��
				m_pCheckPoint[nCnt]->SetUseDeath(true);//���S�t���O���g�p����
				m_pCheckPoint[nCnt]->SetDeath();       //���S�t���O��ݒ肷��
				m_pCheckPoint[nCnt] = nullptr;         //�|�C���^��������
			}
		}

		if (m_pTutorialTex != nullptr)
		{//�`���[�g���A���\���e�N�X�`����j��
			m_pTutorialTex->SetUseDeath(true);//���S�t���O���g�p����
			m_pTutorialTex->SetDeath();       //���S�t���O��ݒ肷��
			m_pTutorialTex = nullptr;         //�|�C���^��������
		}
		CObject::SetDeath();//�I�u�W�F�N�g���S�t���O�ݒ菈��
	}
}
//===================================================================================================

//======================================================
//��������
//======================================================
CTutorial* CTutorial::Create()
{
	CTutorial* pTutorial = DBG_NEW CTutorial();//�`���[�g���A���𐶐�

	pTutorial->Init();                         //����������

	return pTutorial;
}
//===================================================================================================

//======================================================
//�S�Ẵ`�F�b�N�}�[�N�̕`������Z�b�g����
//======================================================
void CTutorial::SetDrawReset()
{
	if (m_bSuccess == false)
	{//�`���[�g���A�����������Ă��Ȃ�������
		for (int nCnt = 0; nCnt < static_cast<int>(CHECK::MAX); nCnt++)
		{//�S�Ă̊e�`���[�g���A���N���A�`�F�b�N�}�[�N�̕`����I�t�ɂ��A���S�Ƀ`���[�g���A�����I������
			m_pCheckPoint[nCnt]->SetUseDraw(false);//�`������Ȃ�
		}
		m_bSuccess = true;//�`���[�g���A���𐬌�
	}
}
//===================================================================================================