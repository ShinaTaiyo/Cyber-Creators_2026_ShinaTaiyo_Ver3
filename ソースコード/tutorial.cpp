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
	m_bSuccess = false;
	m_pTutorialTex = CUi::Create(CUi::UITYPE::TUTORIAL_TEX, CObject2D::POLYGONTYPE::SENTERROLLING, 300.0f, 300.0f, 1, false, D3DXVECTOR3(SCREEN_WIDTH - 150.0f, SCREEN_HEIGHT - 150.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	m_pTutorialTex->SetUseDeath(false);

	for (int nCnt = 0; nCnt < static_cast<int>(CHECK::MAX); nCnt++)
	{
		m_pCheckPoint[nCnt] = CUi::Create(CUi::UITYPE::CHECKMARK_000, CObject2D::POLYGONTYPE::SENTERROLLING, 30.0f, 30.0f, 1, false, 
			D3DXVECTOR3(SCREEN_WIDTH - 20.0f, SCREEN_HEIGHT -225.0f + 27.0f * (nCnt), 0.0f),D3DXVECTOR3(0.0f, 0.0f, 0.0f), 
			D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		m_pCheckPoint[nCnt]->SetUseDraw(false);
		m_pCheckPoint[nCnt]->SetUseDeath(false);
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
	CObject::Init();
	return S_OK;
}
//===================================================================================================

//======================================================
//�I������
//======================================================
void CTutorial::Uninit()
{
	CObject::Uninit();
}
//===================================================================================================

//======================================================
//�X�V����
//======================================================
void CTutorial::Update()
{
	CObject::Update();

	int nDrawSuccess = 0;
	for (int nCnt = 0; nCnt < static_cast<int>(CHECK::MAX); nCnt++)
	{
		if (m_pCheckPoint[nCnt]->GetUseDraw() == true && m_bSuccess == false)
		{
			nDrawSuccess++;
		}
	}

	if (nDrawSuccess == static_cast<int>(CHECK::MAX) && m_bSuccess == false)
	{
		m_pTutorialTex->SetUseDraw(false);
		SetDrawReset();
	}
}
//===================================================================================================

//======================================================
//�`�揈��
//======================================================
void CTutorial::Draw()
{
	CObject::Draw();
}
//===================================================================================================

//======================================================
//���S�t���O�ݒ菈��
//======================================================
void CTutorial::SetDeath()
{
	if (GetUseDeath() == true)
	{
		for (int nCnt = 0; nCnt < static_cast<int>(CHECK::MAX); nCnt++)
		{
			if (m_pCheckPoint[nCnt] != nullptr)
			{
				m_pCheckPoint[nCnt]->SetUseDeath(true);
				m_pCheckPoint[nCnt]->SetDeath();
				m_pCheckPoint[nCnt] = nullptr;
			}
		}

		if (m_pTutorialTex != nullptr)
		{
			m_pTutorialTex->SetUseDeath(true);
			m_pTutorialTex->SetDeath();
			m_pTutorialTex = nullptr;
		}
		CObject::SetDeath();
	}
}
//===================================================================================================

//======================================================
//��������
//======================================================
CTutorial* CTutorial::Create()
{
	CTutorial* pTutorial = DBG_NEW CTutorial();

	pTutorial->Init();

	return pTutorial;
}
//===================================================================================================

//======================================================
//�S�Ẵ`�F�b�N�}�[�N�̕`������Z�b�g����
//======================================================
void CTutorial::SetDrawReset()
{
	if (m_bSuccess == false)
	{
		for (int nCnt = 0; nCnt < static_cast<int>(CHECK::MAX); nCnt++)
		{
			m_pCheckPoint[nCnt]->SetUseDraw(false);
		}
		m_bSuccess = true;
	}
}
//===================================================================================================