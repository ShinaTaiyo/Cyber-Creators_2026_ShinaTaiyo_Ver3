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
CParticle2D::CParticle2D(int nPri, bool bUseintPri, CObject::TYPE type, CObject::OBJECTTYPE ObjType) : CObject2D(nPri,bUseintPri,type,ObjType)
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
	CObject2D::Init();//�I�u�W�F�N�g2D�̏�����
	return S_OK;
}
//==================================================================================================================

//===============================================================
//�I������
//===============================================================
void CParticle2D::Uninit()
{
	CObject2D::Uninit();//�I�u�W�F�N�g2D�̏I������
}
//==================================================================================================================

//===============================================================
//�X�V����
//===============================================================
void CParticle2D::Update()
{
	CObject2D::Update();//�I�u�W�F�N�g2D�̍X�V����

	float fRatioLife = GetRatioLife();//�̗͂̊���
	D3DXVECTOR2& Scale = GetScale();  //�g�嗦

	Scale = D3DXVECTOR2(1.0f, 1.0f) * fRatioLife;//�g�嗦��̗͂̊����Őݒ�

}
//==================================================================================================================

//===============================================================
//�`�揈��
//===============================================================
void CParticle2D::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();//�f�o�C�X�ւ̃|�C���^

	//a�u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	CObject2D::Draw();//�I�u�W�F�N�g2D�̕`�揈��

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
	CObject2D::SetDeath();//�I�u�W�F�N�g2D�̎��S�t���O�ݒ菈��
}
//==================================================================================================================

//===============================================================
//��������
//===============================================================
CParticle2D* CParticle2D::Create(D3DXVECTOR3 pos, D3DXVECTOR3 Move, D3DXVECTOR3 Rot, CObject2D::POLYGONTYPE PolygonType, int nLife, float fWidth, float fHeight, D3DXCOLOR col)
{
	CParticle2D* pParticle2D = DBG_NEW CParticle2D;                                                      //�p�[�e�B�N��2D�𐶐� 
	pParticle2D->Init();                                                                                 //����������
	pParticle2D->SetUseDeath(true);                                                                      //���S�t���O�𔭓����邩�ǂ�����ݒ肷��
	pParticle2D->SetUseLife(true, nLife, nLife);                                                         //�̗͂��g�p����
	pParticle2D->SetUseLifeRatioColor(true);                                                             //�̗͂̊����ŐF������ς���
	pParticle2D->SetPos(pos);                                                                            //�I�u�W�F�N�g�Q�c�̈ʒu��ݒ�
	pParticle2D->SetMove(Move);                                                                          //�ړ��ʂ�ݒ肷��
	pParticle2D->SetRot(Rot);                                                                            //������ݒ肷��
	pParticle2D->SetUseScale(true);                                                                      //�g�嗦���g�p���邩�ǂ���
	pParticle2D->SetScale(D3DXVECTOR2(1.0f,1.0f));                                                       //�g�嗦��ݒ肷��
	pParticle2D->CObject::SetType(CObject::TYPE::PARTICLE2D);                                            //�I�u�W�F�N�g�̎�ނ����߂�
	pParticle2D->SetTextureIndex(CManager::GetTexture()->Regist("data\\TEXTURE\\shadow000.jpg"));        //�e�N�X�`�������蓖�Ă�
	pParticle2D->BindTexture(CManager::GetTexture()->GetAddress(pParticle2D->GetTextureIndex()));        //�e�N�X�`����ݒ肷��
	pParticle2D->SetAnimInfo(1, 1,false);                                                                //�A�j���[�V��������ݒ肷��
	pParticle2D->SetWidth(fWidth);                                                                       //������ݒ�                                
	pParticle2D->SetMaxWidth(fWidth);		                                                             //�ő剡����ݒ�
	pParticle2D->SetColor(col, false, 1.0f);                                                             //�F������ݒ�
	pParticle2D->SetHeight(fHeight);		                                                             //������ݒ�
	pParticle2D->SetMaxHeight(fHeight);		                                                             //�ő卂����ݒ�
	pParticle2D->SetPolygonType(PolygonType);                                                            //�|���S���̎�ނ�ݒ�
	return pParticle2D;
}
//==================================================================================================================