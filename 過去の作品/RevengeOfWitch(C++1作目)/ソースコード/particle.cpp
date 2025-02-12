//================================================================
//
//�U���Q�T���F���[�v�}�[�J�[����������[particle.cpp]
//Author:ShinaTaiyo
//
//================================================================

//==========================================
//�C���N���[�h
//==========================================
#include "main.h"
#include "particle.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
//================================================================

//==========================================
//�ÓI�����o�錾
//==========================================
const char* CParticle::m_apPARTICLE_FILENAME[CParticle::TYPE_MAX] =
{
	"data\\TEXTURE\\Shadow000.jpg",
	"data\\TEXTURE\\Particle\\FireParticle_000.png",
	"data\\TEXTURE\\Particle\\IceParticle_000.png",
	"data\\TEXTURE\\Particle\\ThunderParticle_000.png",
	"data\\TEXTURE\\Particle\\HealParticle_000.png",
	"data\\TEXTURE\\Particle\\Partile_Wing.png",
};

//==========================================
//�R���X�g���N�^
//==========================================
CParticle::CParticle() : CBillboard(4)
{

}
//================================================================

//==========================================
//�f�X�g���N�^
//==========================================
CParticle::~CParticle()
{

}
//================================================================

//==========================================
//����������
//==========================================
HRESULT CParticle::Init()
{
	CBillboard::Init();           //�r���{�[�h�̏�����

	m_bUse = false;              //�g�p���
	m_fReductionWidth = 0.0f;    //�����̏k���X�s�[�h
	m_fReductionHeight = 0.0f;   //�����̏k���X�s�[�h
	m_bBranding = false;         //�A���t�@�u�����f�B���O�����邩�ǂ���
	m_Type = CParticle::TYPE00_NORMAL;//�p�[�e�B�N���̎��
	m_fGravity = 0.0f;           //�d�͂̋���
	m_fAddSpeed = 0.0f;          //�����x
	m_SupportPos = NULL_VECTOR3; //�x�_�ƂȂ�ʒu
	return S_OK;
}
//================================================================

//==========================================
//�I������
//==========================================
void CParticle::Uninit()
{
	CBillboard::Uninit();//�r���{�[�h�̏I������
}
//================================================================

//==========================================
//�X�V����
//==========================================
void CParticle::Update()
{
	//=======================
	//�ϐ��錾
	//=======================
	D3DXVECTOR3& Pos = CBillboard::GetPos();
	D3DXVECTOR3& Move = GetMove();
	float& fWidth = GetWidth();
	float& fHeight = GetHeight();
	D3DXCOLOR col = GetColor();
	int& nLife = GetLife();//�̗�
	int& nMaxLife = GetMaxLife();//�ő�̗�
	float fAlpha = 0.0f;//�F�̓����x
	//===========================================================================================

	//=======================================
	//�X�V����
	//=======================================

	//������ON��������
	if (m_bAddSpeed == true)
	{
		Move *= m_fAddSpeed;
	}

	if (m_bGravity == true)
	{
		Move.y += m_fGravity;
	}

	fAlpha = (float)(nLife) / (float)(nMaxLife);//�F�̓����x��̗͂̊����Ō���
	fWidth -= m_fReductionWidth;                    //���������炷
	fHeight -= m_fReductionHeight;                  //���������炷
	SetColor(D3DXCOLOR(col.r, col.g, col.b, fAlpha));
	if (nLife <= 0)
	{//�̗͂��O�ɂȂ�����E�E�E
		m_bUse = false;
	}
	//===========================================================================================

	//========================================
	//�r���{�[�h�̍X�V����
	//========================================
	CBillboard::Update();
	//===========================================================================================

	//========================================
	//�`���[�W���鋓�����g�p���邩�ǂ���
	//========================================
	if (m_bUseChargePartilce == true)
	{
		if (Pos.x >= m_SupportPos.x - 20.0f && Pos.x <= m_SupportPos.x + 20.0f &&
			Pos.y >= m_SupportPos.y - 20.0f && Pos.y <= m_SupportPos.y + 20.0f &&
			Pos.z >= m_SupportPos.z - 20.0f && Pos.z <= m_SupportPos.z + 20.0f)
		{
			SetDeath();
		}
	}
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
//================================================================

//==========================================
//�`�揈��
//==========================================
void CParticle::Draw()
{
	//�����_���[�N���X�̏����擾
	CRenderer* pRenderer = CManager::GetRenderer();
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//================================================
	//�`��̒���
	//================================================
	//���C�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//Z�̔�r���@�ύX�B�i���f���������Ă��B��Ȃ��Ȃ�j
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	//Z�o�b�t�@�ɏ������܂Ȃ��i�d�Ȃ���Ɉ�a�����Ȃ��Ȃ�j
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	//�A���t�@�e�X�g��L��(�A���t�@�l���O���傫���ꍇ�ɕ`�悷��j
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//=======================================================================================================================================


	//a�u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	CBillboard::Draw();

	//a�u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	//================================================
	//�`��̒��������ɖ߂�
	//================================================

	//���C�g��L���ɖ߂�
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	////Z�̔�r���@�ύX�B
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	//Z�o�b�t�@�ɏ�������
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	//�A���t�@�e�X�g�����ɖ߂�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	//=======================================================================================================================================
}
//================================================================

//============================================
//���S�t���O��ݒ�
//============================================
void CParticle::SetDeath()
{
	CObject::SetDeath();
}
//================================================================

//============================================
//�����p�[�e�B�N���ɂ���
//============================================
void CParticle::SetAddSpeed(float fAddSpeed)
{
	m_bAddSpeed = true;//������Ԃɂ���
	m_fAddSpeed = fAddSpeed;//�����ʂ�ݒ肷��
}
//================================================================

//============================================
//�d�̓p�[�e�B�N���ɂ���
//============================================
void CParticle::SetGravity(float fGravity)
{
	m_bGravity = true;      //�d�͂�ON�ɂ���
	m_fGravity = fGravity;  //�d�͂�ݒ�
}
//================================================================

//==========================================
//�p�[�e�B�N���̐�������
//==========================================
CParticle* CParticle::Create(TYPE Type, int nLife, float fWidth, float fHeight, D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, bool bBrending)
{
	CParticle* pParticle = DBG_NEW CParticle;   //�G�t�F�N�g�𐶐�
	CTexture* pTexture = CManager::GetTexture();
	bool bSuccess = pParticle->CObject::GetCreateSuccess();
	if (bSuccess == true)
	{
		if (pParticle != nullptr)
		{
			pParticle->Init();                                                           //����������
			pParticle->SetUseDeath(true);                                                //���S�t���O�𔭓����邩�ǂ�����ݒ肷��
			pParticle->SetLife(nLife);                                                   //�G�t�F�N�g�̗̑�
			pParticle->SetMaxLife(nLife);                                                //�ő�̗�
			pParticle->m_bUse = true;                                                    //�g�p���
			pParticle->m_bBranding = bBrending;                                          //�A���t�@�u�����f�B���O�����邩�ǂ���
			pParticle->m_fReductionWidth = fWidth / (float)(nLife);                      //�����k���X�s�[�h
			pParticle->m_fReductionHeight = fHeight / (float)(nLife);                    //�����k���X�s�[�h
			pParticle->SetMove(move);                                                    //�ړ��ʂ̐ݒ�
			pParticle->m_Type = Type;                                                    //���
			pParticle->SetTextureIndex(pTexture->Regist(m_apPARTICLE_FILENAME[Type]));//�e�N�X�`�����蓖�Ăƃe�N�X�`���ԍ��̐ݒ�
			pParticle->bindTexture(pTexture->GetAddress(pParticle->GetTextureIndex()));  //�e�N�X�`�����Z�b�g����@
			pParticle->SetPos(pos);                                                      //�I�u�W�F�N�g�Q�c�̈ʒu��ݒ�
			pParticle->SetSupportPos(pos);                                                                 //�����ʒu��ݒ�
			pParticle->SetSize(fWidth, fHeight);                                         //�T�C�Y��ݒ肷��
			pParticle->SetColor(col);                                                    //�F������ݒ�
			pParticle->CObject::SetType(CObject::TYPE_PARTICLE);                         //�I�u�W�F�N�g�̎�ނ����߂�
			pParticle->SetAnimInfo(1, 1, col, false);                                    //�A�j���[�V��������ݒ�
		}
	}
	else
	{//�I�u�W�F�N�g�ɋ󂫂��Ȃ������̂Ŕj������
		delete pParticle;
		pParticle = nullptr;
		return nullptr;
	}

	return pParticle;
}
//==========================================================================================================================================

//==================================================================
//�����_�������Ɏˏo����p�[�e�B�N����������
//==================================================================
void CParticle::SummonParticle(TYPE type, int nNum, int nLife, float fWidth, float fHeight, int nRandSpeed, int nDivision, bool bRandumCol, D3DXVECTOR3 Pos, D3DXCOLOR col, bool bBreanding)
{
	D3DXVECTOR3 Speed = NULL_VECTOR3;//����
	D3DXCOLOR RandCol = {};          //�F����
	for (int nCnt = 0; nCnt < nNum; nCnt++)
	{
		Speed.x = float(rand() % nRandSpeed - (nRandSpeed / 2)) / nDivision;
		Speed.y = float(rand() % nRandSpeed - (nRandSpeed / 2)) / nDivision;
		Speed.z = float(rand() % nRandSpeed - (nRandSpeed / 2)) / nDivision;

		if (bRandumCol == false)
		{//�����_���ȐF�������I�t��������
			CParticle::Create(type, nLife, fWidth, fHeight, Pos, Speed, col, bBreanding);
		}
		else
		{//����ȊO
			RandCol.r = float(rand() % 100 + 1) / 100;
			RandCol.g = float(rand() % 100 + 1) / 100;
			RandCol.b = float(rand() % 100 + 1) / 100;
			CParticle::Create(type, nLife, fWidth, fHeight, Pos, Speed, RandCol, bBreanding);
		}
	}
}
//==========================================================================================================================================

//============================================================================================================
//�`���[�W����悤�ȋ����̃p�[�e�B�N�����o��
//============================================================================================================
void CParticle::SummonChargeParticle(TYPE type, int nNum, int nLife, float fInitialSpeed, float fWidth, float fHeight,
	int nSummonLength, int nRandSpeed, int nDivision, bool bRandumCol, D3DXVECTOR3 SupportPos, D3DXCOLOR col, bool bBreanding)
{
	float fVXaim = 0.0f;//X�����̃x�N�g��
	float fVYaim = 0.0f;//Y�����̃x�N�g��
	float fVZaim = 0.0f;//Z�����̃x�N�g��
	float fVLaim = 0.0f;//�����x�N�g��
	float fSpeed = 0.0f;//����
	float fLength = float(rand() % nSummonLength - (nSummonLength / 2));
	D3DXVECTOR3 Speed = NULL_VECTOR3;//����
	D3DXVECTOR3 SummonPos = NULL_VECTOR3;
	CParticle* pParticle = nullptr;
	for (int nCnt = 0; nCnt < nNum; nCnt++)
	{
		fSpeed = float(rand() % nRandSpeed) / nDivision + fInitialSpeed;
		SummonPos.x = SupportPos.x + float(rand() % nSummonLength - (nSummonLength / 2));
		SummonPos.y = SupportPos.y + float(rand() % nSummonLength - (nSummonLength / 2));
		SummonPos.z = SupportPos.z + float(rand() % nSummonLength - (nSummonLength / 2));
		//==========================
		//�R�����x�N�g�������
		//==========================
		fVXaim = SupportPos.x - SummonPos.x;
		fVYaim = SupportPos.y - SummonPos.y;
		fVZaim = SupportPos.z - SummonPos.z;

		fVLaim = sqrtf((fVXaim * fVXaim) + (fVYaim * fVYaim)
			+ (fVZaim * fVZaim));

		Speed.x = fVXaim / fVLaim * fSpeed;
		Speed.y = fVYaim / fVLaim * fSpeed;
		Speed.z = fVZaim / fVLaim * fSpeed;

		if (bRandumCol == true)
		{
			col.r = float(rand() % 100) / 100;
			col.g = float(rand() % 100) / 100;
			col.b = float(rand() % 100) / 100;
		}

	    pParticle = CParticle::Create(type, nLife, fWidth, fHeight, SummonPos, Speed, col, bBreanding);
		pParticle->m_SupportPos = SupportPos;
		pParticle->m_bUseChargePartilce = true;
	}
}
//==========================================================================================================================================