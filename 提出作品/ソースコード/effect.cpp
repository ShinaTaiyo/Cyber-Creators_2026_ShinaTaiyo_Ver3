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
const string CEffect::s_EffectFileName[static_cast<int>(CEffect::EFFECTTYPE::MAX)] =
{
	"data\\TEXTURE\\Shadow000.jpg",
};//�e�N�X�`���t�@�C���l�[��
//=================================================================

//==========================================
//�R���X�g���N�^
//==========================================
CEffect::CEffect(int nPri, bool bUseintPri, CObject::TYPE type, CObject::OBJECTTYPE ObjType) : CBillboard(nPri,bUseintPri,type,ObjType), m_Type(EFFECTTYPE::NORMAL),m_fReductionHeight(0.0f),m_fReductionWidth(0.0f)
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
	CBillboard::Init();                                    //�r���{�[�h�̏���������
								                           
	m_fReductionWidth = 0.0f;                              //�����̏k���X�s�[�h
	m_fReductionHeight = 0.0f;                             //�����̏k���X�s�[�h
	m_Type = EFFECTTYPE::NORMAL;                           //�G�t�F�N�g�̎��
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
	D3DXVECTOR3 Pos = CBillboard::GetPos();                               //�ʒu
	float fWidth = CBillboard::GetWidth();                                //����
	float fHeight = CBillboard::GetHeight();                              //����
	float fAlpha = 1.0f;                                                  //�F�̃A���t�@�l�����߂�
	int& nLife = GetLife();                                               //�̗�
	int& nMaxLife = GetMaxLife();                                         //�ő�̗�
	//===================================================
	
	fAlpha = (float)(nLife) / (float)(nMaxLife);                          //�F�̃A���t�@�l���ő�̗͂Ƃ̊����Ō��߂�
	fWidth -= m_fReductionWidth;                                          //���������炷
	fHeight -= m_fReductionHeight;                                        //���������炷
	SetPos(Pos);                                                          //�ʒu��ݒ�
	SetSize(fWidth, fHeight);                                             //�|���S���T�C�Y��ݒ�
	SetColor(D3DXCOLOR(GetColor().r, GetColor().g, GetColor().b, fAlpha));//�A���t�@�l

	CBillboard::Update();                                                 //�r���{�[�h�̍X�V����
	if (nLife == 0)
	{//�̗͂��O�ɂȂ�����
		SetDeath();//���S�t���O��ݒ肷��
	}
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
	CBillboard::SetDeath();
}
//=======================================================================================================================================

//==========================================
//�G�t�F�N�g�̐�������
//==========================================
CEffect* CEffect::Create(EFFECTTYPE type, int nLife, float fWidth, float fHeight, D3DXVECTOR3 pos, D3DXCOLOR col)
{
	CEffect* pEffect = DBG_NEW CEffect;   //�G�t�F�N�g�𐶐�
	CTexture* pTexture = CManager::GetTexture();
	if (pEffect != nullptr)
	{
		pEffect->Init();                                                              //����������
		pEffect->SetUseDeath(true);                                                   //���S�t���O�𔭓����邩�ǂ�����ݒ肷��
		pEffect->CBillboard::SetFormarSize(fWidth, fHeight);                          //�|���S���̌��̃T�C�Y��ݒ�
		pEffect->m_Type = type;                                                       //�G�t�F�N�g�̎��
		pEffect->SetLife(nLife);                                                      //�̗�
		pEffect->SetMaxLife(nLife);                                                   //�ő�̗�
		pEffect->m_fReductionWidth = fWidth / (float)(nLife);                         //�����k���X�s�[�h
		pEffect->m_fReductionHeight = fHeight / (float)(nLife);                       //�����k���X�s�[�h
		pEffect->SetTextureIndex(pTexture->Regist(s_EffectFileName[static_cast<int>(type)]));  //�e�N�X�`�����蓖�Ăƃe�N�X�`���ԍ��̐ݒ�
		pEffect->bindTexture(pTexture->GetAddress(pEffect->GetTextureIndex()));       //�e�N�X�`�����Z�b�g����@
		pEffect->SetPos(pos);                                                         //�I�u�W�F�N�g�Q�c�̈ʒu��ݒ�[
		pEffect->SetSupportPos(pos);                                                                 //�����ʒu��ݒ�
		pEffect->SetSize(fWidth, fHeight);                                            //�T�C�Y��ݒ肷��
		pEffect->SetColor(col);                                                       //�F������ݒ�
		pEffect->SetAnimInfo(1, 1, col, false);                                       //�A�j���[�V��������ݒ�
	}

	return pEffect;
}
//=======================================================================================================================================