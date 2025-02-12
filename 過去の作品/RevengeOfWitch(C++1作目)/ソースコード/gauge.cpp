//==================================================================================================================================
//
//�U���P�U���F�Q�[�W������
//Author:ShinaTaiyo
//
//==================================================================================================================================

//===================================================================
//�C���N���[�h
//===================================================================
#include "gauge.h"
#include "texture.h"
//==================================================================================================================================

//===================================================================
//�R���X�g���N�^
//===================================================================
CGauge::CGauge() : CObject2D(7)
{
	m_nParam = 0;      //�p�����[�^
	m_nMaxParam = 0;   //�ő�p�����[�^
	m_type = (GAUGETYPE)(0);//�Q�[�W�̎��
}
//==================================================================================================================================

//===================================================================
//�f�X�g���N�^
//===================================================================
CGauge::~CGauge()
{

}
//==================================================================================================================================

//===================================================================
//����������
//===================================================================
HRESULT CGauge::Init()
{
	CObject2D::Init();        //2D�I�u�W�F�N�g������
	m_nParam = 0;             //�p�����[�^
	m_nMaxParam = 0;          //�ő�p�����[�^
	m_nParamOld = 0;          //��O�̃p�����[�^
	m_type = (GAUGETYPE)(0);  //�Q�[�W�̎��
	m_pDamageGauge = nullptr; //�_���[�W�Q�[�W
	return S_OK;
}
//==================================================================================================================================


//===================================================================
//�I������
//===================================================================
void CGauge::Uninit()
{
	CObject2D::Uninit();//2D�I�u�W�F�N�g�I��
}
//==================================================================================================================================

//===================================================================
//�X�V����
//===================================================================
void CGauge::Update()
{
	float fRatio;//���݂̃Q�[�W�̊���
	float fMaxWidth = GetMaxWidth();//2D�I�u�W�F�N�g�̉����̍ő�l���擾����

	if (m_nParam > m_nMaxParam)
	{//�p�����[�^���ő�l�𒴂��Ă�����A�ő�l�ɌŒ�B
		m_nParam = m_nMaxParam;
	}

	fRatio = (float)(m_nParam) / (float)(m_nMaxParam);

	SetWidth(fMaxWidth * fRatio);

	if (m_pDamageGauge != nullptr)
	{
		if (m_pDamageGauge->GetParam() <= 0)
		{
			m_pDamageGauge->SetDeath();
			m_pDamageGauge = nullptr;
		}
	}

	CObject2D::Update();
}
//==================================================================================================================================

//===================================================================
//�`�揈��
//===================================================================
void CGauge::Draw()
{
	CObject2D::Draw();//2D�I�u�W�F�N�g�`�揈��
}
//==================================================================================================================================

//===================================================================
//�p�����[�^�𑝉�������
//===================================================================
void CGauge::AddParam(int nParam)
{
	m_nParam += nParam;
}
//==================================================================================================================================

//===================================================================
//���S�t���O��ݒ�
//===================================================================
void CGauge::SetDeath()
{
	if (m_pDamageGauge != nullptr)
	{
		m_pDamageGauge->SetUseDeath(true);
		m_pDamageGauge->CObject::SetDeath();
		m_pDamageGauge = nullptr;
	}

	CObject::SetDeath();
}
//==================================================================================================================================

//===================================================================
//�p�����[�^��ݒ�
//===================================================================
void CGauge::SetParam(int nParam)
{
	m_nParamOld = m_nParam;//��O�̃p�����[�^��ݒ�
	m_nParam = nParam;
	int nDamage = m_nParamOld - m_nParam;//��O�̃p�����[�^���ׂ�
	float fRatio = (float)(m_nParam) / (float)(m_nMaxParam);//���݂̃Q�[�W�̊���
	float fMaxWidth = GetMaxWidth();//2D�I�u�W�F�N�g�̉����̍ő�l���擾����
	float fDamageRatio = 1.0f - fRatio;
	float fWidth = fMaxWidth * fRatio;
	float fSetWidth = fMaxWidth * fDamageRatio;
	SetWidth(fWidth);

	//if (m_pDamageGauge == nullptr && m_type == GAUGETYPE_BOSSHP)
	//{
	//	m_pDamageGauge = CDamageGauge::Create(GAUGETYPE_DAMAGE, nDamage,GetMaxParam(), GetMaxWidth(), GetHeight(),
	//		D3DXVECTOR3(GetPos().x + fWidth + (float)(nDamage / 2) - 5.0f, GetPos().y, 0.0f),0);
	//}
	//else if (m_pDamageGauge != nullptr && m_type == GAUGETYPE_BOSSHP)
	//{
	//	m_pDamageGauge->AddParam(nDamage);
	//}
}
//==================================================================================================================================

//===================================================================
//��������
//===================================================================
CGauge * CGauge::Create(GAUGETYPE type, int nParam, float fWidth, float fHeight, D3DXVECTOR3 pos)
{
	CGauge* pGauge = DBG_NEW CGauge;                             //�e�𐶐�
	bool bSuccess = pGauge->CObject::GetCreateSuccess();         //�����������������ǂ������擾����
	if (bSuccess == true)
	{//����������������
		if (pGauge != nullptr)
		{
			pGauge->Init();                                                                                 //����������
			pGauge->SetUseDeath(false);                                                                     //���S�t���O�𔭓����邩�ǂ�����ݒ肷��
			pGauge->m_type = type;                                                                          //�e�̎��
			pGauge->m_nParam = nParam;                                                                      //�p�����[�^
			pGauge->m_nMaxParam = nParam;                                                                   //�ő�p�����[�^
			pGauge->SetPos(pos);                                                                            //�I�u�W�F�N�g�Q�c�̈ʒu��ݒ�
			switch (type)
			{
			case GAUGETYPE_PLAYERHP:
				pGauge->SetInfo(1, 1, fWidth, fHeight, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), POLYGONTYPE03_LEFT, false);
				break;
			case GAUGETYPE_PLAYERMP:
				pGauge->SetInfo(1, 1, fWidth, fHeight, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), POLYGONTYPE03_LEFT, false);
				break;
			case GAUGETYPE_BOSSHP:
				pGauge->SetInfo(1, 1, fWidth, fHeight, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), POLYGONTYPE03_LEFT, false);
				break;
			case GAUGETYPE_DAMAGE:
				pGauge->SetInfo(1, 1, fWidth, fHeight, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), POLYGONTYPE03_LEFT, false);
				break;
			case GAUGETYPE_PLAYERUSAGEMP:
				pGauge->SetInfo(1, 1, fWidth, fHeight, D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), POLYGONTYPE03_LEFT, false);
				pGauge->SetUseBlinking(true, 30, 0.0f);//�_�ł�����
				break;
			default:
				break;
			}
			pGauge->CObject::SetType(CObject::TYPE_GAUGE);                                                  //�I�u�W�F�N�g�̎�ނ����߂�
		}
	}
	else
	{//�I�u�W�F�N�g�ɋ󂫂��Ȃ������̂Ŕj������
		delete pGauge;
		pGauge = nullptr;
	}

	return pGauge;
}
//==================================================================================================================================

//==========================================================================
//�R���X�g���N�^
//==========================================================================
CDamageGauge::CDamageGauge()
{

}
//==================================================================================================================================

//==========================================================================
//�f�X�g���N�^
//==========================================================================
CDamageGauge::~CDamageGauge()
{

}
//==================================================================================================================================

//==========================================================================
//����������
//==========================================================================
HRESULT CDamageGauge::Init()
{

	CGauge::Init();//�Q�[�W�̏���������
	m_nStartDamageParam = 0;
	m_nSubStartCnt = 0;//�Q�[�W�̌������J�n����܂ł̃J�E���g
	m_bSubGauge = false;//�Q�[�W�̌������J�n����J�E���g
	m_nStartParam = 0;//�_���[�W�\�����J�n���鎞�̃p�����[�^
	m_nFirstParam = 0;//�ŐV�̃p�����[�^
	m_nAddParamCnt = 0;//�p�����[�^�𑝂₵����
	return S_OK;
}
//==================================================================================================================================

//==========================================================================
//�I������
//==========================================================================
void CDamageGauge::Uninit()
{
	CGauge::Uninit();//�Q�[�W�̏I������
}
//==================================================================================================================================

//==========================================================================
//�X�V����
//==========================================================================
void CDamageGauge::Update()
{
	float fRatio = 0.0f;//���݂̃Q�[�W�̊���
	float fDamageRatio = 0.0f;//�_���[�W�̊���
	float fMaxWidth = GetMaxWidth();//2D�I�u�W�F�N�g�̉����̍ő�l���擾����
	float fFirstRatio = 0.0f;
	float fFirstWidth = 0.0f;
	D3DXVECTOR3 SupportPos = GetSupportPos();
	D3DXVECTOR3 Pos = GetPos();
	m_nSubStartCnt++;//�Q�[�W�����J�n�܂ł̃J�E���g������

	if (m_nSubStartCnt >= m_nSETSUBSTARTCNT && m_bSubGauge == false)
	{
		m_nSubStartCnt = 0;
		m_bSubGauge = true;
	}
	if (m_bSubGauge == true)
	{
		if (m_nSubStartCnt == 0)
		{
			SetPos(D3DXVECTOR3(SupportPos.x - GetWidth(), Pos.y, 0.0f));
			SetSupportPos(GetPos());
			SetInfo(1, 1, GetMaxWidth(), GetHeight(), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), POLYGONTYPE03_LEFT, false);
		}
		else
		{
			SubParam(5);
		}
	}

	fRatio = (float)(m_nParam) / (float)(GetMaxParam());
	SetWidth(fMaxWidth * fRatio);


	CObject2D::Update();
}
//==================================================================================================================================

//==========================================================================
//�`�揈��
//==========================================================================
void CDamageGauge::Draw()
{
	CGauge::Draw();
}
//==================================================================================================================================

//==========================================================================
//�p�����[�^��������
//==========================================================================
void CDamageGauge::AddParam(int nParam)
{
	m_nParam += nParam;
	m_nFirstParam = nParam;
	m_nSubStartCnt = 0;
}
//==================================================================================================================================

//==========================================================================
//�p�����[�^��ݒ�
//==========================================================================
void CDamageGauge::SetParam(int nParam)
{
	m_nAddParamCnt++;
	if (m_nAddParamCnt == 1)
	{
		m_nFirstParam = nParam;
	}
	m_nSubStartCnt = 0;//�_���[�W���X�V���ꂽ�̂ŁA�����J�n�̃J�E���g�����Z�b�g
	m_nParam = nParam;
}
//==================================================================================================================================

//==========================================================================
//���S�t���O�ݒ菈��
//==========================================================================
void CDamageGauge::SetDeath()
{
	
}
//==================================================================================================================================

//==========================================================================
//�_���[�W�Q�[�W�ݒ菈��
//==========================================================================
CDamageGauge* CDamageGauge::Create(CGauge::GAUGETYPE type, int nParam, int nMaxParam, float fWidth, float fHeight, D3DXVECTOR3 pos, int nStartDamageParam)
{
	CDamageGauge* pDamageGauge = DBG_NEW CDamageGauge;           //�_���[�W�Q�[�W�𐶐�
	bool bSuccess = pDamageGauge->CObject::GetCreateSuccess();         //�����������������ǂ������擾����
	if (bSuccess == true)
	{//����������������
		if (pDamageGauge != nullptr)
		{
			pDamageGauge->Init();                               //����������
			pDamageGauge->m_nStartDamageParam = nStartDamageParam;//�_���[�W�\�����J�n�������̃_���[�W��
			pDamageGauge->SetParam(nParam);
			pDamageGauge->SetUseDeath(false);                   //���S�t���O�𔭓����邩�ǂ�����ݒ肷��
			pDamageGauge->SetGaugeType(type);                   //�Q�[�W�̎��
			pDamageGauge->SetStartParam(nParam);                //�_���[�W�\�����J�n���鎞�̃p�����[�^
			pDamageGauge->SetMaxParam(nMaxParam);               //�ő�p�����[�^
			pDamageGauge->SetPos(pos);                          //�I�u�W�F�N�g�Q�c�̈ʒu��ݒ�
			pDamageGauge->SetSupportPos(pos);                   //�����ʒu��ݒ�
			switch (type)
			{
			case GAUGETYPE_PLAYERHP:
				pDamageGauge->SetInfo(1, 1, fWidth, fHeight, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), POLYGONTYPE04_RIGHT, false);
				break;
			case GAUGETYPE_PLAYERMP:
				pDamageGauge->SetInfo(1, 1, fWidth, fHeight, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), POLYGONTYPE04_RIGHT, false);
				break;
			case GAUGETYPE_BOSSHP:
				pDamageGauge->SetInfo(1, 1, fWidth, fHeight, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), POLYGONTYPE04_RIGHT, false);
				break;
			case GAUGETYPE_DAMAGE:
				pDamageGauge->SetInfo(1, 1, fWidth, fHeight, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), POLYGONTYPE04_RIGHT, false);
				break;
			default:
				break;
			}
			pDamageGauge->CObject::SetType(CObject::TYPE_GAUGE);                                                  //�I�u�W�F�N�g�̎�ނ����߂�
		}
	}
	else
	{//�I�u�W�F�N�g�ɋ󂫂��Ȃ������̂Ŕj������
		delete pDamageGauge;
		pDamageGauge = nullptr;
	}

	return pDamageGauge;
}
//==================================================================================================================================