//==================================================================================================================
//
//�X���P�U���F�{�X�o���C�x���g�����[particle2d.cpp]
//Author:ShinaTaiyo
//
//==================================================================================================================

//===============================================================
//�C���N���[�h
//===============================================================
#include "particle2d.h"
#include "texture.h"
#include "manager.h"
#include "renderer.h"
//==================================================================================================================

//===============================================================
//�R���X�g���N�^
//===============================================================
CParticle2D::CParticle2D()
{

}
//==================================================================================================================

//===============================================================
//�f�X�g���N�^
//===============================================================
CParticle2D::~CParticle2D()
{
}

//===============================================================
//����������
//===============================================================
HRESULT CParticle2D::Init()
{
	CObject2D::Init();
	return S_OK;
}
//==================================================================================================================

//===============================================================
//�I������
//===============================================================
void CParticle2D::Uninit()
{
	CObject2D::Uninit();
}
//==================================================================================================================

//===============================================================
//�X�V����
//===============================================================
void CParticle2D::Update()
{
	CObject2D::Update();

	float fRatioLife = GetRatioLife();
	D3DXVECTOR2& Scale = GetScale();

	Scale = D3DXVECTOR2(1.0f, 1.0f) * fRatioLife;

}
//==================================================================================================================

//===============================================================
//�`�揈��
//===============================================================
void CParticle2D::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	//a�u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	CObject2D::Draw();
	//a�u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}
//==================================================================================================================

//===============================================================
//���S�t���O�ݒ菈��
//===============================================================
void CParticle2D::SetDeath()
{
	CObject::SetDeath();
}
//==================================================================================================================

//===============================================================
//��������
//===============================================================
CParticle2D* CParticle2D::Create(D3DXVECTOR3 pos, D3DXVECTOR3 Move, D3DXVECTOR3 Rot, CObject2D::POLYGONTYPE PolygonType, int nLife, float fWidth, float fHeight, D3DXCOLOR col)
{
	CParticle2D* pParticle2D = DBG_NEW CParticle2D;                           //�e�𐶐� 
	bool bSuccess = pParticle2D->CObject::GetCreateSuccess();         //�����������������ǂ������擾����
	if (bSuccess == true)
	{//����������������
		if (pParticle2D != nullptr)
		{
			pParticle2D->Init();                                                                                 //����������
			pParticle2D->SetUseDeath(true);                                                                      //���S�t���O�𔭓����邩�ǂ�����ݒ肷��
			pParticle2D->SetUseLife(true, nLife, nLife);                                                         //�̗͂��g�p����
			pParticle2D->SetUseLifeRatioColor(true);                                                             //�̗͂̊����ŐF������ς���
			pParticle2D->SetPos(pos);                                                                            //�I�u�W�F�N�g�Q�c�̈ʒu��ݒ�
			pParticle2D->SetMove(Move);                                                                          //�ړ��ʂ�ݒ肷��
			pParticle2D->SetRot(Rot);                                                                            //������ݒ肷��
			pParticle2D->SetUseScale(true);                                                                      //�g�嗦���g�p���邩�ǂ���
			pParticle2D->SetScale(D3DXVECTOR2(1.0f,1.0f));                                                       //�g�嗦��ݒ肷��
			pParticle2D->CObject::SetType(CObject::TYPE_PARTICLE2D);                                             //�I�u�W�F�N�g�̎�ނ����߂�
			pParticle2D->SetTextureIndex(CManager::GetTexture()->Regist("data\\TEXTURE\\shadow000.jpg"));        //�e�N�X�`�������蓖�Ă�
			pParticle2D->BindTexture(CManager::GetTexture()->GetAddress(pParticle2D->GetTextureIndex()));        //�e�N�X�`����ݒ肷��
			pParticle2D->SetInfo(1, 1, fWidth, fHeight, col, PolygonType, false);                          //�|���S������ݒ肷��
		}
	}
	else
	{//�I�u�W�F�N�g�ɋ󂫂��Ȃ������̂Ŕj������
		delete pParticle2D;
		pParticle2D = nullptr;
	}

	return pParticle2D;
}
//==================================================================================================================