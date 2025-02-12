//================================================================
//
//�V���X���F�G�~�b�^�[����������[Emitter.cpp]
//Author:ShinaTaiyo
//
//================================================================

//==========================================
//�C���N���[�h
//==========================================
#include "main.h"
#include "Emitter.h"
#include "manager.h"
#include "renderer.h"
#include "effect.h"
#include "texture.h"
//================================================================

//==========================================
//�ÓI�����o������
//==========================================

//==========================================
//�R���X�g���N�^
//==========================================
CEmitter::CEmitter() : CBillboard(4)
{

}
//================================================================

//==========================================
//�f�X�g���N�^
//==========================================
CEmitter::~CEmitter()
{

}
//================================================================

//==========================================
//����������
//==========================================
HRESULT CEmitter::Init()
{
	CBillboard::Init();             //2D�I�u�W�F�N�g�̏�����
					
	//�G�~�b�^�[���
	m_bUse = false;                //�g�p���
	m_Rot = NULL_VECTOR3;          //����
	m_AddRot = NULL_VECTOR3;       //���Z��]��
	m_fLength = 0.0f;              //�����v�Z�p

	//�G�t�F�N�g���
	m_fSetEffectWidth = 0.0f;      //�ݒ肷��G�t�F�N�g�̉���
	m_fSetEffectHeight = 0.0f;     //�ݒ肷��G�t�F�N�g�̍���
	m_nSetEffectLife = 0;          //�ݒ肷��G�t�F�N�g�̗̑�
	m_SetEffectColor = {};         //�ݒ肷��G�t�F�N�g�̐F����
	m_nEffectType = 0;             //�ݒ肷��G�t�F�N�g�̎��
	return S_OK;
}
//================================================================

//==========================================
//�I������
//==========================================
void CEmitter::Uninit()
{
	CBillboard::Uninit();//�r���{�[�h�̏I������
	//SetDeath();//���S�t���O�ݒ�
}
//================================================================

//==========================================
//�X�V����
//==========================================
void CEmitter::Update()
{
	//=======================
	//�ϐ��錾
	//=======================
	D3DXVECTOR3 & Pos = CBillboard::GetPos();
	D3DXVECTOR3 SummonPos = Pos;//�G�t�F�N�g�����ʒu
	float & fWidth = CBillboard::GetWidth();
	float & fHeight = CBillboard::GetHeight();
	float fAlpha = 1.0f;//�F�̃A���t�@�l�����߂�
	int& nLife = GetLife();//�̗�
	int& nMaxLife = GetMaxLife();//�ő�̗�
	//===================================================

	if (m_bUseRotate == true)
	{//��]���g�p����Ȃ�
		m_Rot += m_AddRot;
		SummonPos = Pos + D3DXVECTOR3(sinf(m_Rot.y) * m_fLength, Pos.y, cosf(m_Rot.y) * m_fLength);
	}

	if (GetUseMtxChild() == false)
	{//�q������Ȃ��ꍇ
		//�G�t�F�N�g�𐶐�
		CEffect::Create((CEffect::EFFECTTYPE)(m_nEffectType), m_nSetEffectLife, m_fSetEffectWidth, m_fSetEffectHeight, SummonPos, m_SetEffectColor);
	}
	else
	{//�q�ǂ��̏ꍇ
		//�G�t�F�N�g�𐶐�
		CEffect::Create((CEffect::EFFECTTYPE)(m_nEffectType), m_nSetEffectLife, m_fSetEffectWidth, m_fSetEffectHeight, GetMtxTransPos(), m_SetEffectColor);
	}
	if (nLife <= 0)
	{//�̗͂��O�ɂȂ�����E�E�E
		m_bUse = false;
	}

	SetSize(fWidth, fHeight);

	//========================================
	//�r���{�[�h�̍X�V����
	//========================================
	CBillboard::Update();
	//===========================================================================================


	//=======================================
	//�g�p��Ԃ��I�t�ɂȂ�����E�E�E
	//=======================================
	if (m_bUse == false)
	{
		//Uninit();
		SetDeath();
	}
	//===========================================================================================

}
//===========================================================================================

//======================================================
//�`�揈��
//======================================================
void CEmitter::Draw()
{
	CBillboard::Draw();
}
//===========================================================================================

//======================================================
//���S�t���O��ݒ�
//======================================================
void CEmitter::SetDeath()
{
	CObject::SetDeath();
}
//===========================================================================================

//==========================================
//�G�t�F�N�g�̐�������
//==========================================
CEmitter * CEmitter::Create(int nEffectType, int nLife, int nSetLife, float fLength, float fWidth, float fHeight, D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col)
{
	CEmitter* pEmitter = DBG_NEW CEmitter;   //�G�t�F�N�g�𐶐�
	CTexture* pTexture = CManager::GetTexture();
	bool bSuccess = pEmitter->CObject::GetCreateSuccess();
	if (bSuccess == true)
	{
		if (pEmitter != nullptr)
		{
		
			pEmitter->Init();                                                              //����������
			pEmitter->m_nEffectType = nEffectType;                                         //�ݒ肷��G�t�F�N�g�̎��
			pEmitter->SetUseDeath(true);                                                   //���S�t���O�𔭓����邩�ǂ�����ݒ肷��
			pEmitter->SetLife(nLife);                                                      //�̗�
			pEmitter->SetMaxLife(nLife);                                                   //�ő�̗�
			pEmitter->m_fLength = fLength;                                                 //����
			pEmitter->SetUseDraw(true);                                                   //�`�悷�邩�ǂ�����ݒ�
			pEmitter->m_bUse = true;                                                       //�g�p���
			pEmitter->SetMove(move);                                                       //�ړ���
			pEmitter->m_nSetEffectLife = nSetLife;                                         //�ݒ肷��G�t�F�N�g�̗̑�
			pEmitter->m_fSetEffectWidth = fWidth;                                          //�ݒ肷��G�t�F�N�g�̉���
			pEmitter->m_fSetEffectHeight = fHeight;                                        //�ݒ肷��G�t�F�N�g�̍���
			pEmitter->m_SetEffectColor = col;                                              //�ݒ肷��G�t�F�N�g�̐F����
			pEmitter->SetPos(pos);                                                         //�I�u�W�F�N�g�Q�c�̈ʒu��ݒ�
			pEmitter->SetSupportPos(pos);                                                                 //�����ʒu��ݒ�
			pEmitter->SetColor(col);                                                       //�F������ݒ�
			pEmitter->CObject::SetType(CObject::TYPE_EMITTER);                             //�I�u�W�F�N�g�̎�ނ����߂�
			pEmitter->SetAnimInfo(1, 1, col, false);                                       //�A�j���[�V��������ݒ�
		}
	}
	else
	{//�I�u�W�F�N�g�ɋ󂫂��Ȃ������̂Ŕj������
		delete pEmitter;
		pEmitter = nullptr;
		return nullptr;
	}

	return pEmitter;

}
//================================================================