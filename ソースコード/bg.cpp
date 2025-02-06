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
	//======================================================
    //�Q�c�I�u�W�F�N�g�̏�����
    //======================================================
	CObject2D::Init();
	//================================================================================================

	return S_OK;
}
//=========================================================================

//========================
//�I������
//========================
void CBg::Uninit()
{
	CObject2D::Uninit();
}
//=========================================================================

//========================
//�X�V����
//========================
void CBg::Update()
{
	//Rot.z += 0.01f;
	CObject2D::Update();

	if (m_bUseTexMove == true)
	{
		LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVertexBuffer();
		VERTEX_2D* pVtx;
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
	CObject2D::Draw();
}
//=========================================================================

//========================
//���S�t���O��ݒ�
//========================
void CBg::SetDeath()
{
	CObject::SetDeath();
}
//=========================================================================

//=============================
//�w�i�̐�������
//=============================
void CBg::Create(BG type)
{
	CBg* pBg = DBG_NEW CBg;   //�e�𐶐�
	bool bSuccess = pBg->CObject::GetCreateSuccess();
	CTexture* pTextureClass = CManager::GetTexture();           //�e�N�X�`���N���X���擾
	if (bSuccess == true)
	{
		if (pBg != nullptr)
		{
			pBg->Init();                                                     //����������
			pBg->CObject2D::SetAnimInfo(1, 1, false);//�|���S���ƃe�N�X�`������ݒ�
			pBg->SetColor(D3DXCOLOR(1.0f,1.0f, 1.0f, 1.0f), false, 1.0f);
			pBg->SetWidth(SCREEN_WIDTH / 2);
			pBg->SetMaxWidth(SCREEN_WIDTH / 2);
			pBg->SetHeight(SCREEN_HEIGHT / 2);
			pBg->SetMaxHeight(SCREEN_HEIGHT / 2);
			pBg->SetPolygonType(CObject2D::POLYGONTYPE::NORMAL);
			pBg->SetTextureIndex(pTextureClass->Regist(m_apBGFILE_NAME[type]));
			pBg->SetUseDeath(true);                 //���S�t���O�𔭓����邩�ǂ�����ݒ肷��
			pBg->CObject2D::BindTexture(pTextureClass->GetAddress(pBg->GetTextureIndex()));
			pBg->CObject2D::SetPos(D3DXVECTOR3(SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2,0.0f));//�|���S���ƃe�N�X�`������ݒ�
			pBg->m_Type = type;  //�e�̎��
			pBg->CObject::SetType(CObject::TYPE::BG);//�I�u�W�F�N�̎�ނ����߂�
		}
	}
	else
	{
		delete pBg;
		pBg = nullptr;
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
	CBg::Init();
	return S_OK;
}
//=========================================================================

//=====================================================
//�I������
//=====================================================
void CBgTitle::Uninit()
{
	CBg::Uninit();
}
//=========================================================================

//=====================================================
//�X�V����
//=====================================================
void CBgTitle::Update()
{
	CBg::Update();
}
//=========================================================================

//=====================================================
//�`�揈��
//=====================================================
void CBgTitle::Draw()
{
	CBg::Draw();
}
//=========================================================================

//=====================================================
//���S�t���O�ݒ菈��
//=====================================================
void CBgTitle::SetDeath()
{
	CObject::SetDeath();
}
//=========================================================================

//=====================================================
//��������
//=====================================================
CBgTitle* CBgTitle::Create(BG type)
{
	CBgTitle* pBgTitle = DBG_NEW CBgTitle;   //�e�𐶐�
	bool bSuccess = pBgTitle->CObject::GetCreateSuccess();
	CTexture* pTextureClass = CManager::GetTexture();           //�e�N�X�`���N���X���擾
	pBgTitle->Init();                                                     //����������
	pBgTitle->SetUseScale(true);//�g�嗦���g�p����
	pBgTitle->CObject2D::SetAnimInfo(1, 1,false);//�|���S���ƃe�N�X�`������ݒ�
	pBgTitle->SetWidth(SCREEN_WIDTH / 2);
	pBgTitle->SetMaxWidth(SCREEN_WIDTH / 2);
	pBgTitle->SetHeight(SCREEN_HEIGHT / 2);
	pBgTitle->SetMaxHeight(SCREEN_HEIGHT / 2);
	pBgTitle->SetPolygonType(CObject2D::POLYGONTYPE::NORMAL);
	pBgTitle->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), false, 1.0f);
	pBgTitle->SetTextureIndex(pTextureClass->Regist(m_apBGFILE_NAME[type]));
	pBgTitle->SetUseDeath(true);                 //���S�t���O�𔭓����邩�ǂ�����ݒ肷��
	pBgTitle->CObject2D::BindTexture(pTextureClass->GetAddress(pBgTitle->GetTextureIndex()));
	pBgTitle->CObject2D::SetPos(D3DXVECTOR3(SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2,0.0f));//�|���S���ƃe�N�X�`������ݒ�
	pBgTitle->SetBgType(type);                  //�w�i�̎�ނ�ݒ�
	pBgTitle->CObject::SetType(CObject::TYPE::BG);//�I�u�W�F�N�̎�ނ����߂�


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
	CBg::Init();
	return S_OK;
}
//=========================================================================

//=====================================================
//�I������
//=====================================================
void CBgDifficulty::Uninit()
{
	CBg::Uninit();
}
//=========================================================================

//=====================================================
//�X�V����
//=====================================================
void CBgDifficulty::Update()
{
	CBg::Update();
}
//=========================================================================

//=====================================================
//�`�揈��
//=====================================================
void CBgDifficulty::Draw()
{
	CBg::Draw();
}
//=========================================================================

//=====================================================
//���S�t���O�ݒ菈��
//=====================================================
void CBgDifficulty::SetDeath()
{
	CBg::SetDeath();
}
//=========================================================================

//=====================================================
//��������
//=====================================================
CBgDifficulty* CBgDifficulty::Create(BG type)
{
	CBgDifficulty* pBgDifficulty = DBG_NEW CBgDifficulty;   //�e�𐶐�
	bool bSuccess = pBgDifficulty->CObject::GetCreateSuccess();
	CTexture* pTextureClass = CManager::GetTexture();           //�e�N�X�`���N���X���擾
	if (bSuccess == true)
	{
		if (pBgDifficulty != nullptr)
		{
			pBgDifficulty->Init();                                                     //����������
			pBgDifficulty->SetUseScale(true);//�g�嗦���g�p����
			pBgDifficulty->CObject2D::SetAnimInfo(1, 1, false);//�|���S���ƃe�N�X�`������ݒ�
			pBgDifficulty->SetWidth(SCREEN_WIDTH);
			pBgDifficulty->SetMaxWidth(SCREEN_WIDTH);
			pBgDifficulty->SetHeight(SCREEN_HEIGHT);
			pBgDifficulty->SetMaxHeight(SCREEN_HEIGHT);
			pBgDifficulty->SetPolygonType(CObject2D::POLYGONTYPE::SENTERROLLING);

			pBgDifficulty->SetTextureIndex(pTextureClass->Regist(m_apBGFILE_NAME[type]));
			pBgDifficulty->SetUseDeath(true);                 //���S�t���O�𔭓����邩�ǂ�����ݒ肷��
			pBgDifficulty->CObject2D::BindTexture(pTextureClass->GetAddress(pBgDifficulty->GetTextureIndex()));
			pBgDifficulty->CObject2D::SetPos(D3DXVECTOR3(SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2,0.0f));//�|���S���ƃe�N�X�`������ݒ�
			pBgDifficulty->SetBgType(type);                  //�w�i�̎�ނ�ݒ�
			pBgDifficulty->CObject::SetType(CObject::TYPE::BG);//�I�u�W�F�N�̎�ނ����߂�
		}
	}
	else
	{
		delete pBgDifficulty;
		pBgDifficulty = nullptr;
		return nullptr;
	}

	return pBgDifficulty;
}
//=========================================================================