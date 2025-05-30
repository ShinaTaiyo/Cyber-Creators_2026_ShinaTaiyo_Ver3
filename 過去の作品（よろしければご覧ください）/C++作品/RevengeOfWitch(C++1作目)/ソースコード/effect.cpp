//================================================================
//
//�T���Q�V���F��ՂÂ���i�G�t�F�N�g����������j[Effect.cpp]
//Author:ShinaTaiyo
//
//================================================================

//==========================================
//�C���N���[�h
//==========================================
#include "main.h"
#include "effect.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
//================================================================

//==========================================
//�ÓI�����o������
//==========================================
LPDIRECT3DTEXTURE9 CEffect::m_pTextureTemp = nullptr;//�e�N�X�`��
const char* CEffect::m_aEFFECT_FILENAME[CEffect::EFFECTTYPE_MAX] =
{
	"data\\TEXTURE\\Shadow000.jpg",
	"data\\TEXTURE\\Particle\\ThunderParticle_000.png",
};//�e�N�X�`���t�@�C���l�[��
//=================================================================

//==========================================
//�R���X�g���N�^
//==========================================
CEffect::CEffect() : CBillboard(4), m_Type(EFFECTTYPE00_NORMAL), m_bUse(false), m_fReductionHeight(0.0f),m_fReductionWidth(0.0f)
{

}
//================================================================

//==========================================
//�f�X�g���N�^
//==========================================
CEffect::~CEffect()
{

}
//================================================================

//==========================================
//����������
//==========================================
HRESULT CEffect::Init()
{
	CBillboard::Init();           //2D�I�u�W�F�N�g�̏�����

	m_bUse = false;              //�g�p���
	m_fReductionWidth = 0.0f;    //�����̏k���X�s�[�h
	m_fReductionHeight = 0.0f;   //�����̏k���X�s�[�h
	m_Type = EFFECTTYPE00_NORMAL;//�G�t�F�N�g�̎��
	return S_OK;
}
//================================================================

//==========================================
//�I������
//==========================================
void CEffect::Uninit()
{
	CBillboard::Uninit();//�r���{�[�h�̏I������
}
//================================================================

//==========================================
//�X�V����
//==========================================
void CEffect::Update()
{
	//=======================
    //�ϐ��錾
    //=======================
	D3DXVECTOR3 Pos = CBillboard::GetPos();
	float fWidth = CBillboard::GetWidth();
	float fHeight = CBillboard::GetHeight();
	float fAlpha = 1.0f;//�F�̃A���t�@�l�����߂�
	int& nLife = GetLife();//�̗�
	int& nMaxLife = GetMaxLife();//�ő�̗�
	//===================================================
	
	fAlpha = (float)(nLife) / (float)(nMaxLife);//�F�̃A���t�@�l���ő�̗͂Ƃ̊����Ō��߂�
	fWidth -= m_fReductionWidth;   //���������炷
	fHeight -= m_fReductionHeight; //���������炷
	SetPos(Pos);                   //�ʒu��ݒ�
	SetSize(fWidth, fHeight);      //�|���S���T�C�Y��ݒ�

	if (nLife <= 0)
	{//�̗͂��O�ɂȂ�����E�E�E
		m_bUse = false;
	}

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
//================================================================

//==========================================
//�`�揈��
//==========================================
void CEffect::Draw()
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
//=======================================================================================================================================

//=========================================================================
//���S�t���O��ݒ�
//=========================================================================
void CEffect::SetDeath()
{
	CObject::SetDeath();
  
}
//=======================================================================================================================================

//==========================================
//�e�N�X�`���̃A�����[�h����
//==========================================
void CEffect::Unload()
{
	if (m_pTextureTemp != nullptr)
	{
		m_pTextureTemp->Release();
		m_pTextureTemp = nullptr;
	}
}
//================================================================

//==========================================
//�G�t�F�N�g�̐�������
//==========================================
void CEffect::Create(EFFECTTYPE type, int nLife, float fWidth, float fHeight, D3DXVECTOR3 pos, D3DXCOLOR col)
{
	CEffect* pEffect = DBG_NEW CEffect;   //�G�t�F�N�g�𐶐�
	CTexture* pTexture = CManager::GetTexture();
	bool bSuccess = pEffect->CObject::GetCreateSuccess();
	if (bSuccess == true)
	{
		if (pEffect != nullptr)
		{
			pEffect->Init();                                                              //����������
			pEffect->SetUseDeath(true);                                                   //���S�t���O�𔭓����邩�ǂ�����ݒ肷��
			pEffect->CBillboard::SetFormarSize(fWidth, fHeight);                          //�|���S���̌��̃T�C�Y��ݒ�
			pEffect->m_Type = type;                                                       //�G�t�F�N�g�̎��
			pEffect->m_bUse = true;                                                       //�g�p���
			pEffect->SetLife(nLife);                                                      //�̗�
			pEffect->SetMaxLife(nLife);                                                   //�ő�̗�
			pEffect->m_fReductionWidth = fWidth / (float)(nLife);                         //�����k���X�s�[�h
			pEffect->m_fReductionHeight = fHeight / (float)(nLife);                       //�����k���X�s�[�h
			pEffect->SetTextureIndex(pTexture->Regist(m_aEFFECT_FILENAME[(int)(type)]));  //�e�N�X�`�����蓖�Ăƃe�N�X�`���ԍ��̐ݒ�
			pEffect->bindTexture(pTexture->GetAddress(pEffect->GetTextureIndex()));       //�e�N�X�`�����Z�b�g����@
			pEffect->SetPos(pos);                                                         //�I�u�W�F�N�g�Q�c�̈ʒu��ݒ�[
			pEffect->SetSupportPos(pos);                                                                 //�����ʒu��ݒ�
			pEffect->SetSize(fWidth, fHeight);                                            //�T�C�Y��ݒ肷��
			pEffect->SetColor(col);                                                       //�F������ݒ�
			pEffect->CObject::SetType(CObject::TYPE_EFFECT);                              //�I�u�W�F�N�g�̎�ނ����߂�
			pEffect->SetAnimInfo(1, 1, col, false);                                       //�A�j���[�V��������ݒ�
		}
	}
	else
	{//�I�u�W�F�N�g�ɋ󂫂��Ȃ������̂Ŕj������
		delete pEffect;
		pEffect = nullptr;
	}

}
//================================================================