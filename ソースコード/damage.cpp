//=============================================================================================================================
//
//�P�Q���Q�O���F�G�̍U���̒���������[damage.cpp]
//Author:ShinaTaiyo
//
//=============================================================================================================================

//========================================================================
//�C���N���[�h
//========================================================================
#include "damage.h"
#include "calculation.h"
#include "manager.h"
#include "renderer.h"
#include "sound.h"
#include "number3d.h"
#include "camera.h"
//=============================================================================================================================

//========================================================================
//�R���X�g���N�^
//========================================================================
CDamage::CDamage() : m_nDamage(0), m_pNumber3D(), m_Pos(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
m_nDigit(0),m_nJumpCnt(m_nMAX_JUMPCNT),m_JumpMove(D3DXVECTOR3(0.0f,10.0f,0.0f)),m_bExeggration(false),m_fWidth(0.0f),
m_fHeight(0.0f)
{

}
//=============================================================================================================================

//========================================================================
//�f�X�g���N�^
//========================================================================
CDamage::~CDamage()
{

}
//=============================================================================================================================

//========================================================================
//����������
//========================================================================
HRESULT CDamage::Init()
{
	CObject::Init();
	return S_OK;
}
//=============================================================================================================================

//========================================================================
//�I������
//========================================================================
void CDamage::Uninit()
{
	CObject::Uninit();
}
//=============================================================================================================================

//========================================================================
//�X�V����
//========================================================================
void CDamage::Update()
{
	CObject::Update();
	if (m_pNumber3D[0]->GetPos().y < m_pNumber3D[0]->GetSupportPos().y)
	{
		m_nJumpCnt--;
		m_JumpMove.y /= 2;
		for (int nCnt = 0; nCnt < m_nMAX_DAMAGEDIGIT; nCnt++)
		{
			if (m_pNumber3D[nCnt] != nullptr)
			{
				m_pNumber3D[nCnt]->SetMove(D3DXVECTOR3(0.0f, m_JumpMove.y, 0.0f));
				m_pNumber3D[nCnt]->SetPos(m_pNumber3D[nCnt]->GetSupportPos());
			}
		}
	}

	if (m_nJumpCnt == 0)
	{
		SetDeath();
	}
}
//=============================================================================================================================

//========================================================================
//�`�揈��
//========================================================================
void CDamage::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	CObject::Draw();

	for (int nCnt = 0; nCnt < m_nDigit; nCnt++)
	{
		if (m_pNumber3D[nCnt] != nullptr)
		{
			m_pNumber3D[nCnt]->ExtraDraw();//�_���[�W�ʂ̌������`�悷��
		}
	}
}
//=============================================================================================================================

//========================================================================
//���S�t���O�ݒ菈��
//========================================================================
void CDamage::SetDeath()
{
	if (GetUseDeath() == true)
	{
		for (int nCnt = 0; nCnt < m_nMAX_DAMAGEDIGIT; nCnt++)
		{
			if (m_pNumber3D[nCnt] != nullptr)
			{
				m_pNumber3D[nCnt]->SetUseDeath(true);
				m_pNumber3D[nCnt]->SetDeath();
				m_pNumber3D[nCnt] = nullptr;
			}
		}
	}
	CObject::SetDeath();
}
//=============================================================================================================================

//========================================================================
//��������
//========================================================================
CDamage* CDamage::Create(int nDamage, D3DXVECTOR3 Pos, D3DXCOLOR col, float fWidth, float fHeight, bool bExeggration)
{
	CDamage* pDamage = DBG_NEW CDamage;                           //�e�𐶐� 
	bool bSuccess = pDamage->CObject::GetCreateSuccess();         //�����������������ǂ������擾����
	float fHalfWidth = fWidth * 0.5f;
	pDamage->m_fWidth = fHalfWidth;
	pDamage->m_fHeight = fHeight;
	if (bSuccess == true)
	{//����������������
		if (pDamage != nullptr)
		{
			if (nDamage > 999)
			{
				nDamage = 999;
			}
			pDamage->Init();                                                                                 //����������
			pDamage->SetUseDeath(true);                                                                     //���S�t���O�𔭓����邩�ǂ�����ݒ肷��
			pDamage->m_nDamage = nDamage;//�_���[�W��
			pDamage->m_nDigit = CCalculation::CalculationDigit(nDamage);//�������v�Z����
			pDamage->m_Pos = Pos;//�ʒu
			pDamage->CObject::SetType(CObject::TYPE::NONE);                                                  //�I�u�W�F�N�g�̎�ނ����߂�
			for (int nCnt = 0; nCnt < m_nMAX_DAMAGEDIGIT; nCnt++)
			{
				pDamage->m_pNumber3D[nCnt] = CNumber3D::Create(fWidth, fHeight, D3DXVECTOR3(Pos.x + sinf(CManager::GetCamera()->GetRot().y) * (fHalfWidth) * nCnt, Pos.y + fHeight,
					Pos.z + cosf(CManager::GetCamera()->GetRot().y) * (fHalfWidth) * nCnt),D3DXVECTOR3(0.0f,12.0f,0.0f), col);
				pDamage->m_pNumber3D[nCnt]->SetUseGravity(-0.5f);
			}
			pDamage->DamageDispProcess();
			pDamage->m_bExeggration = bExeggration;
		}
	}
	else
	{//�I�u�W�F�N�g�ɋ󂫂��Ȃ������̂Ŕj������
		delete pDamage;
		pDamage = nullptr;
	}

	return pDamage;

}
//=============================================================================================================================

//========================================================================
//�_���[�W��\�����鏈��
//========================================================================
void CDamage::DamageDispProcess()
{
	int nTempDamage = m_nDamage;
	int nTexPos = 0;
	for (int nCnt = 0; nCnt < m_nMAX_DAMAGEDIGIT; nCnt++)
	{
		nTexPos = nTempDamage % 10;
		nTempDamage /= 10;

		m_pNumber3D[nCnt]->SetAnim(nTexPos);
	}
}
//=============================================================================================================================