//=========================================
//
//�P�O���P�W���F�V����GitProject�쐬[bg.cpp]
//Author:ShinaTaiyo
//
//=========================================

//========================
//�C���N���[�h
//========================
#include "bg.h"
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "object2d.h"
#include "object.h"
#include "texture.h"
//=========================================================================

//========================
//�ÓI�����o
//========================
const std::string CBg::m_apBGFILE_NAME[CBg::BG_MAX] =
{
	"data\\TEXTURE\\UI\\Title_000.png",
	"data\\TEXTURE\\Bg\\Wall_000.jpg",
	"data\\TEXTURE\\Bg\\Result_BG.png"
};//�w�i�̃t�@�C����

//========================
//�R���X�g���N�^
//========================
CBg::CBg(int nPri, bool bUseintPri, CObject::TYPE type, CObject::OBJECTTYPE ObjType) : CObject2D(nPri,bUseintPri,type,ObjType), m_bUseTexMove(false),m_PosTex(D3DXVECTOR2(0.0f,0.0f)), m_MoveTex(D3DXVECTOR2(0.0f,0.0f)),m_Type(BG_TITLE),m_fPosTexU(0.0f),m_fRotationPower(0.0f)
{

}
//=========================================================================

//========================
//�f�X�g���N�^
//========================
CBg::~CBg()
{

}
//=========================================================================

//========================
//����������
//========================
HRESULT CBg::Init()
{
	CObject2D::Init();//2D�I�u�W�F�N�g�̏�����
	return S_OK;
}
//=========================================================================

//========================
//�I������
//========================
void CBg::Uninit()
{
	CObject2D::Uninit();//2D�I�u�W�F�N�g�̏I������
}
//=========================================================================

//========================
//�X�V����
//========================
void CBg::Update()
{
	CObject2D::Update();//�I�u�W�F�N�g2D�̍X�V����

	if (m_bUseTexMove == true)
	{//�e�N�X�`�����W���ړ������邩�ǂ���
		LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVertexBuffer();
		VERTEX_2D* pVtx;//2D���_���ւ̃|�C���^
		//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f) + m_PosTex;
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f) + m_PosTex;
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f) + m_PosTex;
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f) + m_PosTex;

		//�e�N�X�`�����W���ړ�������
		m_PosTex += m_MoveTex;

		//���_�o�b�t�@���A�����b�N����
		pVtxBuff->Unlock();
	}
}
//=========================================================================

//========================
//�`�揈��
//========================
void CBg::Draw()
{
	CObject2D::Draw();//�I�u�W�F�N�g2D�̕`�揈��
}
//=========================================================================

//========================
//���S�t���O��ݒ�
//========================
void CBg::SetDeath()
{
	CObject2D::SetDeath();//�I�u�W�F�N�g2D�̎��S�t���O�ݒ菈��
}
//=========================================================================

//=============================
//�w�i�̐�������
//=============================
void CBg::Create(BG type)
{
	CBg* pBg = DBG_NEW CBg;                                     //�w�i�𐶐�
	CTexture* pTextureClass = CManager::GetTexture();           //�e�N�X�`���N���X���擾
	if (pBg != nullptr)
	{//�w�i�����݂��Ă�����
		pBg->Init();                                                                   //����������a
		pBg->CObject2D::SetAnimInfo(1, 1, false);                                      //�|���S���ƃe�N�X�`������ݒ�
		pBg->SetColor(D3DXCOLOR(1.0f,1.0f, 1.0f, 1.0f), false, 1.0f);                  //�F������ݒ�
		pBg->SetWidth(SCREEN_WIDTH / 2);                                               //�|���S���̉�����ݒ�
		pBg->SetMaxWidth(SCREEN_WIDTH / 2);                                            //�|���S���̍ő剡����ݒ�
		pBg->SetHeight(SCREEN_HEIGHT / 2);                                             //�|���S���̍�����ݒ�
		pBg->SetMaxHeight(SCREEN_HEIGHT / 2);                                          //�|���S���̍ő卂����ݒ�
		pBg->SetPolygonType(CObject2D::POLYGONTYPE::NORMAL);                           //�|���S���̎�ނ�ݒ�
		pBg->SetTextureIndex(pTextureClass->Regist(m_apBGFILE_NAME[type]));            //�e�N�X�`���ԍ��̐ݒ�
		pBg->SetUseDeath(true);                                                        //���S�t���O�𔭓����邩�ǂ�����ݒ肷��
		pBg->CObject2D::BindTexture(pTextureClass->GetAddress(pBg->GetTextureIndex()));//�e�N�X�`�����̐ݒ�
		pBg->CObject2D::SetPos(D3DXVECTOR3(SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2,0.0f));  //�|���S���ƃe�N�X�`������ݒ�
		pBg->m_Type = type;                                                            //�w�i�̎��
		pBg->CObject::SetType(CObject::TYPE::BG);                                      //�I�u�W�F�N�g�̎�ނ����߂�
	}
}
//=========================================================================

//======================================================<<<<<<<<�^�C�g���w�i�N���X>>>>>>>>>>=========================================

//=====================================================
//�R���X�g���N�^
//=====================================================
CBgTitle::CBgTitle()
{

}
//=========================================================================

//=====================================================
//�f�X�g���N�^
//=====================================================
CBgTitle::~CBgTitle()
{

}
//=========================================================================

//=====================================================
//����������
//=====================================================
HRESULT CBgTitle::Init()
{
	CBg::Init();//�w�i�̏�����
	return S_OK;
}
//=========================================================================

//=====================================================
//�I������
//=====================================================
void CBgTitle::Uninit()
{
	CBg::Uninit();//�w�i�̏I������
}
//=========================================================================

//=====================================================
//�X�V����
//=====================================================
void CBgTitle::Update()
{
	CBg::Update();//�w�i�̍X�V����
}
//=========================================================================

//=====================================================
//�`�揈��
//=====================================================
void CBgTitle::Draw()
{
	CBg::Draw();//�w�i�̕`�揈��
}
//=========================================================================

//=====================================================
//���S�t���O�ݒ菈��
//=====================================================
void CBgTitle::SetDeath()
{
	CBg::SetDeath();//�w�i�̎��S�t���O�ݒ菈��
}
//=========================================================================

//=====================================================
//��������
//=====================================================
CBgTitle* CBgTitle::Create(BG type)
{
	CBgTitle* pBgTitle = DBG_NEW CBgTitle;                                                   //�^�C�g���w�i�𐶐�
	CTexture* pTextureClass = CManager::GetTexture();                                        //�e�N�X�`���N���X���擾
	pBgTitle->Init();                                                                        //����������
	pBgTitle->SetUseScale(true);                                                             //�g�嗦���g�p����
	pBgTitle->CObject2D::SetAnimInfo(1, 1,false);                                            //�|���S���ƃe�N�X�`������ݒ�
	pBgTitle->SetWidth(SCREEN_WIDTH / 2);                                                    //������ݒ�
	pBgTitle->SetMaxWidth(SCREEN_WIDTH / 2);                                                 //�ő剡����ݒ�
	pBgTitle->SetHeight(SCREEN_HEIGHT / 2);                                                  //������ݒ�
	pBgTitle->SetMaxHeight(SCREEN_HEIGHT / 2);                                               //�ő卂����ݒ�
	pBgTitle->SetPolygonType(CObject2D::POLYGONTYPE::NORMAL);                                //�|���S���̎�ނ�ݒ�
	pBgTitle->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), false, 1.0f);                      //�F������ݒ�
	pBgTitle->SetTextureIndex(pTextureClass->Regist(m_apBGFILE_NAME[type]));                 //�e�N�X�`���ԍ��̐ݒ�
	pBgTitle->SetUseDeath(true);                                                             //���S�t���O�𔭓����邩�ǂ�����ݒ肷��
	pBgTitle->CObject2D::BindTexture(pTextureClass->GetAddress(pBgTitle->GetTextureIndex()));//�e�N�X�`�����̐ݒ�
	pBgTitle->CObject2D::SetPos(D3DXVECTOR3(SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2,0.0f));       //�|���S���ƃe�N�X�`������ݒ�
	pBgTitle->SetBgType(type);                                                               //�w�i�̎�ނ�ݒ�
	pBgTitle->CObject::SetType(CObject::TYPE::BG);                                           //�I�u�W�F�N�̎�ނ����߂�


	return pBgTitle;
}
//=========================================================================

//======================================================<<<<<<<<��Փx�I��w�i�N���X>>>>>>>>>>=========================================

//=====================================================
//�R���X�g���N�^
//=====================================================
CBgDifficulty::CBgDifficulty()
{

}
//=========================================================================

//=====================================================
//�f�X�g���N�^
//=====================================================
CBgDifficulty::~CBgDifficulty()
{

}
//=========================================================================

//=====================================================
//����������
//=====================================================
HRESULT CBgDifficulty::Init()
{
	CBg::Init();//�w�i�̏���������
	return S_OK;
}
//=========================================================================

//=====================================================
//�I������
//=====================================================
void CBgDifficulty::Uninit()
{
	CBg::Uninit();//�w�i�̏I������
}
//=========================================================================

//=====================================================
//�X�V����
//=====================================================
void CBgDifficulty::Update()
{
	CBg::Update();//�w�i�̍X�V����
}
//=========================================================================

//=====================================================
//�`�揈��
//=====================================================
void CBgDifficulty::Draw()
{
	CBg::Draw();//�w�i�̕`�揈��
}
//=========================================================================

//=====================================================
//���S�t���O�ݒ菈��
//=====================================================
void CBgDifficulty::SetDeath()
{
	CBg::SetDeath();//�w�i�̎��S�t���O�ݒ菈��
}
//=========================================================================

//=====================================================
//��������
//=====================================================
CBgDifficulty* CBgDifficulty::Create(BG type)
{
	CBgDifficulty* pBgDifficulty = DBG_NEW CBgDifficulty;                                                    //��Փx�I���̔w�i�𐶐�
	CTexture* pTextureClass = CManager::GetTexture();                                                        //�e�N�X�`���N���X���擾
	if (pBgDifficulty != nullptr)								                                             
	{//��Փx�I��w�i�����݂�����							                                                 
		pBgDifficulty->Init();                                                                               //����������
		pBgDifficulty->SetUseScale(true);                                                                    //�g�嗦���g�p����
		pBgDifficulty->CObject2D::SetAnimInfo(1, 1, false);                                                  //�|���S���ƃe�N�X�`������ݒ�
		pBgDifficulty->SetWidth(SCREEN_WIDTH);                                                               //������ݒ�
		pBgDifficulty->SetMaxWidth(SCREEN_WIDTH);                                                            //�ő剡����ݒ�
		pBgDifficulty->SetHeight(SCREEN_HEIGHT);                                                             //������ݒ�
		pBgDifficulty->SetMaxHeight(SCREEN_HEIGHT);                                                          //�ő卂����ݒ�
		pBgDifficulty->SetPolygonType(CObject2D::POLYGONTYPE::SENTERROLLING);                                //�|���S���̎�ނ�ݒ�

		pBgDifficulty->SetTextureIndex(pTextureClass->Regist(m_apBGFILE_NAME[type]));                        //�e�N�X�`���ԍ���ݒ�
		pBgDifficulty->SetUseDeath(true);                                                                    //���S�t���O�𔭓������邩�ǂ�����ݒ�
		pBgDifficulty->CObject2D::BindTexture(pTextureClass->GetAddress(pBgDifficulty->GetTextureIndex()));  //�e�N�X�`������ݒ�
		pBgDifficulty->CObject2D::SetPos(D3DXVECTOR3(SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2,0.0f));              //�|���S���ƃe�N�X�`������ݒ�
		pBgDifficulty->SetBgType(type);                                                                      //�w�i�̎�ނ�ݒ�
		pBgDifficulty->CObject::SetType(CObject::TYPE::BG);                                                  //�I�u�W�F�N�g�̎�ނ����߂�
	}
	return pBgDifficulty;
}
//=========================================================================