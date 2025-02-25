//=============================================================================================
//
//�X���V���F�^�C�}�[����������[timer.cpp]
//Author:ShinaTaiyo
//
//=============================================================================================

//==============================================
//�C���N���[�h
//==============================================
#include "timer.h"
#include "manager.h"
#include "texture.h"
#include "fade.h"
#include "number.h"
//=============================================================================================

//==============================================
//�R���X�g���N�^
//==============================================
CTimer::CTimer()
{

}
//=============================================================================================

//==============================================
//�f�X�g���N�^
//==============================================
CTimer::~CTimer()
{

}
//=============================================================================================

//==============================================
//����������
//==============================================
HRESULT CTimer::Init()
{
	CObject::Init();
	m_nTime = m_nMAX_TIME;//�X�R�A
	for (int nCnt = 0; nCnt < m_nMAX_TIMERDIGIT; nCnt++)
	{
		m_pNumber[nCnt] = nullptr;//�����ւ̃|�C���^ 
	}
	return S_OK;
}
//=============================================================================================

//==============================================
//�I������
//==============================================
void CTimer::Uninit()
{
	CObject::Uninit();
}
//=============================================================================================

//==============================================
//�X�V����
//==============================================
void CTimer::Update()
{
	int nCntFrame = GetCntFrame();
	if (nCntFrame % 60 == 0)
	{//�������Ԃ����炷����
		SubTimer(-1);
	}

	if (GetUseDeath() == true)
	{
		int nC = 0;
	}

	CObject::Update();
}
//=============================================================================================

//==============================================
//�`�揈��
//==============================================
void CTimer::Draw()
{
	//CObject2D::Draw();
}
//=============================================================================================

//==============================================
//���S�t���O�ݒ菈��
//==============================================
void CTimer::SetDeath()
{
	for (int nCnt = 0; nCnt < m_nMAX_TIMERDIGIT; nCnt++)
	{
		if (m_pNumber[nCnt] != nullptr)
		{//�����̎��S�t���O�ݒ菈��
			m_pNumber[nCnt]->SetUseDeath(true);
			m_pNumber[nCnt]->SetDeath();
			m_pNumber[nCnt] = nullptr;
		}
	}

	CObject::SetDeath();
}
//=============================================================================================

//==============================================
//��������
//==============================================
CTimer* CTimer::Create(int nInitialTimer, D3DXVECTOR3 Pos, float fNumberWidth, float fNumberHeight)
{
	CTimer* pTimer = DBG_NEW CTimer;                           //�e�𐶐� 
	bool bSuccess = pTimer->CObject::GetCreateSuccess();         //�����������������ǂ������擾����
	if (bSuccess == true)
	{//����������������
		if (pTimer != nullptr)
		{
			pTimer->Init();                                                                                 //����������
			pTimer->SetUseDeath(false);                                                                     //���S�t���O�𔭓����邩�ǂ�����ݒ肷��
			pTimer->CObject::SetType(CObject::TYPE_TIMER);                                                  //�I�u�W�F�N�g�̎�ނ����߂�
			//pTimer->SetTextureIndex(CManager::GetTexture()->Regist(""));       //�e�N�X�`�������蓖�Ă�
			for (int nCnt = 0; nCnt < m_nMAX_TIMERDIGIT; nCnt++)
			{
				pTimer->m_pNumber[nCnt] = CNumber::Create(D3DXVECTOR3(Pos.x - fNumberWidth - fNumberWidth * nCnt,Pos.y + fNumberHeight, 0.0f), fNumberWidth, fNumberHeight);
			}
			//pTimer->BindTexture(CManager::GetTexture()->GetAddress(pTimer->GetTextureIndex()));            //�e�N�X�`����ݒ肷��
			pTimer->m_nTime = nInitialTimer;//�������ԏ����l
		}
	}
	else
	{//�I�u�W�F�N�g�ɋ󂫂��Ȃ������̂Ŕj������
		delete pTimer;
		pTimer = nullptr;
	}

	return pTimer;
}
//=============================================================================================

//==============================================
//���Ԃ𑝂₷����
//==============================================
void CTimer::AddTimer(int nTimer)
{
	m_nTime += nTimer;
	int nTempTimer = m_nTime;
	int nTexPos = 0;

	for (int nCnt = 0; nCnt < m_nMAX_TIMERDIGIT; nCnt++)
	{
		nTexPos = nTempTimer % 10;
		nTempTimer /= 10;

		m_pNumber[nCnt]->SetAnim(nTexPos);
	}
}
//=============================================================================================\

//==============================================
//���Ԃ����炷����
//==============================================
void CTimer::SubTimer(int nTimer)
{
	m_nTime -= nTimer;
	int nTempTimer = m_nTime;
	int nTexPos = 0;

	for (int nCnt = 0; nCnt < m_nMAX_TIMERDIGIT; nCnt++)
	{
		nTexPos = nTempTimer % 10;
		nTempTimer /= 10;

		m_pNumber[nCnt]->SetAnim(nTexPos);
	}

	//if (m_nTime == 0)
	//{
	//	CManager::GetSceneFade()->SetSceneFade(CFade::FADEMODE_IN, CScene::MODE_RESULT);
	//}
}
//=============================================================================================\