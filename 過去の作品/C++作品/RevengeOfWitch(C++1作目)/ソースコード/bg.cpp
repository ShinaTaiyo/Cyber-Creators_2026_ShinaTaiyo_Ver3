//=========================================
//
//�T���Q�Q���F�N���X��[bg.cpp]
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
const char* CBg::m_apBGFILE_NAME[CBg::BG_MAX] =
{
	"data\\TEXTURE\\Title\\Title_001.png",
	"data\\TEXTURE\\Bg\\Wall_000.jpg",
	"data\\TEXTURE\\Result_BG.png"
};//�w�i�̃t�@�C����

//========================
//�R���X�g���N�^
//========================
CBg::CBg() : CObject2D(0), m_bUseTexMove(false),m_PosTex(D3DXVECTOR2(0.0f,0.0f)), m_MoveTex(D3DXVECTOR2(0.0f,0.0f))
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
	m_fPosTexU = 0.0f;//�e�N�X�`�����W

	//==========================
	//��]�n
	//==========================
	m_fRotationPower = 0.0f;//��]��
	//======================================================

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
	D3DXVECTOR3& Rot = GetRot();

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
			pBg->CObject2D::SetInfo(1, 1, SCREEN_WIDTH,SCREEN_HEIGHT,NORMAL_COL,POLYGONTYPE01_SENTERROLLING,true);//�|���S���ƃe�N�X�`������ݒ�
			pBg->SetTextureIndex(pTextureClass->Regist(m_apBGFILE_NAME[type]));
			pBg->SetUseDeath(true);                 //���S�t���O�𔭓����邩�ǂ�����ݒ肷��
			pBg->CObject2D::BindTexture(pTextureClass->GetAddress(pBg->GetTextureIndex()));
			pBg->CObject2D::SetPos(SENTER_VECTOR3);//�|���S���ƃe�N�X�`������ݒ�
			pBg->m_Type = type;  //�e�̎��
			pBg->CObject::SetType(CObject::TYPE_BG);//�I�u�W�F�N�̎�ނ����߂�
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
CBgTitle::CBgTitle() : m_bRotationSwitch(false),m_nRotationNum(0),m_pUi(),m_TitleUiRotSpeed(0.0f),m_TitleUiScale(D3DXVECTOR2(0.0f,0.0f))
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
	float& fRotationPower = GetRotationPower();
	fRotationPower += (0.0f - fRotationPower) * 0.25f;//��]�͂�����������

	m_TitleUiScale.x += fRotationPower / 1.5f;
	m_TitleUiScale.y += fRotationPower / 1.5f;
	if (fRotationPower <= 0.00002f)
	{
		m_bRotationSwitch = m_bRotationSwitch ? false : true;
		m_TitleUiScale = D3DXVECTOR2(1.0f, 1.0f);//�g�嗦�����Z�b�g
		if (m_nRotationNum == 0)
		{
			fRotationPower = 0.05f;
		}
		else
		{
			fRotationPower = 0.1f;
		}
		m_nRotationNum++;
	}



	if (m_bRotationSwitch == false)
	{//�E�ɉ�]
		m_TitleUiRotSpeed = fRotationPower;
	}
	else
	{//���ɉ�]
		m_TitleUiRotSpeed = -fRotationPower;
	}

	//m_pUi[TITLEUI00_TITLELOGO]->SetScale(m_TitleUiScale);
	//m_pUi[TITLEUI00_TITLELOGO]->SetPolygonRotSpeed(m_TitleUiRotSpeed);
	m_pUi[TITLEUI01_PLAYERILLUST]->SetScale(m_TitleUiScale);
	m_pUi[TITLEUI01_PLAYERILLUST]->SetPolygonRotSpeed(m_TitleUiRotSpeed);
	m_pUi[TITLEUI02_SLIMEILLUST]->SetScale(m_TitleUiScale);
	m_pUi[TITLEUI02_SLIMEILLUST]->SetPolygonRotSpeed(m_TitleUiRotSpeed);

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

	//�^�C�g���w�iUI�̏I������
	for (int nCnt = 0; nCnt < TITLEUI_MAX; nCnt++)
	{
		if (m_pUi[nCnt] != nullptr)
		{
			m_pUi[nCnt]->SetUseDeath(true);
			m_pUi[nCnt]->SetDeath();
			m_pUi[nCnt] = nullptr;
		}
	}
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
	if (bSuccess == true)
	{
		if (pBgTitle != nullptr)
		{
			pBgTitle->Init();                                                     //����������
			pBgTitle->SetUseScale(true);//�g�嗦���g�p����
			pBgTitle->CObject2D::SetInfo(1, 1, SCREEN_WIDTH, SCREEN_HEIGHT, NORMAL_COL, POLYGONTYPE01_SENTERROLLING, true);//�|���S���ƃe�N�X�`������ݒ�
			pBgTitle->SetTextureIndex(pTextureClass->Regist(m_apBGFILE_NAME[type]));
			pBgTitle->SetUseDeath(true);                 //���S�t���O�𔭓����邩�ǂ�����ݒ肷��
			pBgTitle->CObject2D::BindTexture(pTextureClass->GetAddress(pBgTitle->GetTextureIndex()));
			pBgTitle->CObject2D::SetPos(SENTER_VECTOR3);//�|���S���ƃe�N�X�`������ݒ�
			pBgTitle->SetBgType(type);                  //�w�i�̎�ނ�ݒ�
			pBgTitle->CObject::SetType(CObject::TYPE_BG);//�I�u�W�F�N�̎�ނ����߂�

			pBgTitle->m_pUi[CBgTitle::TITLEUI03_MAGICCIRCLE] = CUi::Create(CUi::UITYPE_MAGICCIRCLE, CObject2D::POLYGONTYPE01_SENTERROLLING,
				700.0f, 700.0f, SENTER_VECTOR3, NULL_VECTOR3, NULL_VECTOR3, NORMAL_COL);
			pBgTitle->m_pUi[CBgTitle::TITLEUI03_MAGICCIRCLE]->SetPolygonRotSpeed(0.02f);

			pBgTitle->m_pUi[CBgTitle::TITLEUI00_TITLELOGO] = CUi::Create(CUi::UITYPE_TITLELOGO, CObject2D::POLYGONTYPE01_SENTERROLLING,
				SCREEN_WIDTH - 200.0f, 200.0f, SENTER_VECTOR3 + D3DXVECTOR3(0.0f, -250.0f, 0.0f), NULL_VECTOR3, NULL_VECTOR3, NORMAL_COL);
			pBgTitle->m_pUi[CBgTitle::TITLEUI00_TITLELOGO]->SetUseFloating(0.0f,0.1f,2.0f, true);

			pBgTitle->m_pUi[CBgTitle::TITLEUI01_PLAYERILLUST] = CUi::Create(CUi::UITYPE_PLAYER2DILUST, CObject2D::POLYGONTYPE01_SENTERROLLING,
				200.0f, 200.0f, SENTER_VECTOR3 + D3DXVECTOR3(-400.0f,150.0f, 0.0f), NULL_VECTOR3, NULL_VECTOR3, NORMAL_COL);

			pBgTitle->m_pUi[CBgTitle::TITLEUI02_SLIMEILLUST] = CUi::Create(CUi::UITYPE_SLIMEILUST, CObject2D::POLYGONTYPE01_SENTERROLLING,
				100.0f, 100.0f, SENTER_VECTOR3 + D3DXVECTOR3(400.0f,150.0f, 0.0f), NULL_VECTOR3, NULL_VECTOR3, NORMAL_COL);
		}
	}
	else
	{
		delete pBgTitle;
		pBgTitle = nullptr;
		return nullptr;
	}

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
			pBgDifficulty->CObject2D::SetInfo(1, 1, SCREEN_WIDTH, SCREEN_HEIGHT, NORMAL_COL, POLYGONTYPE01_SENTERROLLING, true);//�|���S���ƃe�N�X�`������ݒ�
			pBgDifficulty->SetTextureIndex(pTextureClass->Regist(m_apBGFILE_NAME[type]));
			pBgDifficulty->SetUseDeath(true);                 //���S�t���O�𔭓����邩�ǂ�����ݒ肷��
			pBgDifficulty->CObject2D::BindTexture(pTextureClass->GetAddress(pBgDifficulty->GetTextureIndex()));
			pBgDifficulty->CObject2D::SetPos(SENTER_VECTOR3);//�|���S���ƃe�N�X�`������ݒ�
			pBgDifficulty->SetBgType(type);                  //�w�i�̎�ނ�ݒ�
			pBgDifficulty->CObject::SetType(CObject::TYPE_BG);//�I�u�W�F�N�̎�ނ����߂�
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