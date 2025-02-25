//==============================================================================
//
//�U���Q�T���F���[�v�}�[�J�[����������[number3d.cpp
//Author:ShinaTaiyo
//
//==============================================================================

//=========================
//�C���N���[�h
//=========================
#include "number3d.h"
#include "manager.h"
#include "enemy.h"
#include "effect.h"
#include "sound.h"
#include "texture.h"
//=================================================

//=========================
//�ÓI�����o
//=========================

//========================================================

//=========================
//�R���X�g���N�^
//=========================
CNumber3D::CNumber3D(int nPri, bool bUseintPri, CObject::TYPE type, CObject::OBJECTTYPE ObjType) : CBillboard(nPri,bUseintPri,type,ObjType)
{

}
//=================================================

//=========================
//�f�X�g���N�^
//=========================
CNumber3D::~CNumber3D()
{

}
//=================================================

//=========================
//����������
//=========================
HRESULT CNumber3D::Init()
{
	//===========================
	//�r���{�[�h������
	//===========================
	CBillboard::Init();
	//=========================================
	return S_OK;
}
//=================================================

//=========================
//�I������
//=========================
void CNumber3D::Uninit()
{
	CBillboard::Uninit();
}
//=================================================

//=========================
//�X�V����
//=========================
void CNumber3D::Update()
{
	//========================================
	//�r���{�[�h�̍X�V����
	//========================================
	CBillboard::Update();
	//===========================================================================================
}
//=================================================

//=========================
//�`�揈��
//=========================
void CNumber3D::Draw()
{

}
//===========================================================================================

//=========================
//�ʘg�̕`�揈��
//=========================
void CNumber3D::ExtraDraw()
{
	CBillboard::Draw();
}
//===========================================================================================

//=========================
//���S�t���O��ݒ�
//========================
void CNumber3D::SetDeath()
{
	CBillboard::SetDeath();
}
//===========================================================================================

//=========================
//3D������������
//=========================
CNumber3D* CNumber3D::Create(float fWidth, float fHeight, D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col)
{
	CNumber3D* pNumber3D = DBG_NEW CNumber3D();                                                 //����3D�𐶐�
	CTexture* pTextureClass = CManager::GetTexture();                                           //�e�N�X�`���N���X���擾
	pNumber3D->Init();                                                                          //����������
	pNumber3D->SetUseDeath(false);                                                              //���S�t���O�𔭓����邩�ǂ�����ݒ肷��
	pNumber3D->SetTextureIndex(pTextureClass->Regist("data\\TEXTURE\\Number_000.png"));         //�e�N�X�`������o�^���A�e�N�X�`���ԍ���ݒ�
	pNumber3D->CBillboard::bindTexture(pTextureClass->GetAddress(pNumber3D->GetTextureIndex()));//�e�N�X�`�����Z�b�g����@
	pNumber3D->CBillboard::SetPos(pos);                                                         //�I�u�W�F�N�g�Q�c�̈ʒu��ݒ�
	pNumber3D->SetSupportPos(pos);                                                              //�����ʒu��ݒ�
	pNumber3D->CBillboard::SetSize(fWidth, fHeight);                                            //�|���S���̃T�C�Y��ݒ�
	pNumber3D->CObject::SetType(CObject::TYPE::NUMBER3D);                                       //�I�u�W�F�N�g�̎�ނ����߂�
	pNumber3D->CBillboard::SetFormarSize(fWidth, fHeight);                                      //�|���S���̌��̃T�C�Y��ݒ�
	pNumber3D->SetAnimInfo(10, 1,col, false);                                                   //�A�j���[�V��������ݒ�
	pNumber3D->SetMove(move);                                                                   //�ړ��ʂ�ݒ�
	return pNumber3D;
}
//=================================================