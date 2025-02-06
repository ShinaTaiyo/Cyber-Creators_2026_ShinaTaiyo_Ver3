//===================================================================================================
//
//�X���P�O���F�^�C�g������������[ui.cpp]
//Author:ShinaTaiyo
//
//===================================================================================================

//====================================================
//�C���N���[�h
//====================================================
#include "ui.h"
#include "texture.h"
#include "renderer.h"
#include "manager.h"
#include "sound.h"
#include "calculation.h"
//===================================================================================================

//====================================================
//�ÓI�����o�錾
//====================================================
const string CUi::UI_FILENAME[int(CUi::UITYPE::MAX)] =
{
	"data\\TEXTURE\\UI\\LockOn_001.png",
	"data\\TEXTURE\\UI\\ActionModeWindow_000.png",
	"data\\TEXTURE\\UI\\ShotModeDisp_000.png",
	"data\\TEXTURE\\UI\\DiveModeDisp_000.png",
	"data\\TEXTURE\\UI\\PhaseText_000.png",
	"data\\TEXTURE\\UI\\StageText_000.png",
	"data\\TEXTURE\\UI\\Target_000.png",
	"data\\TEXTURE\\UI\\PossibleDiveText_000.png",
	"data\\TEXTURE\\UI\\Tutorial_000.png",
	"data\\TEXTURE\\UI\\CheckMark_000.png",
	"data\\TEXTURE\\UI\\Title_001.png",
	"data\\TEXTURE\\UI\\PressEnterOrStart_Logo.png",
	"data\\TEXTURE\\UI\\DiveGaugeFrame_000.png"
};//�e�N�X�`���t�@�C����

//====================================================
//�R���X�g���N�^
//====================================================
CUi::CUi(int nPri, bool bUseintPri, CObject::TYPE type, CObject::OBJECTTYPE ObjType) : CObject2D(nPri,bUseintPri,type,ObjType),
m_MoveType(UIMOVETYPE_NORMAL),m_Type(UITYPE::LOCKON),m_bUseUiEffect(false),m_nSetUiEffectLife(0),m_SetUiEffectColor(D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
{

}
//===================================================================================================

//====================================================
//�f�X�g���N�^
//====================================================
CUi::~CUi()
{

}
//===================================================================================================

//====================================================
//����������
//====================================================
HRESULT CUi::Init()
{
	CObject2D::Init();
	return S_OK;
}
//===================================================================================================

//====================================================
//�I������
//====================================================
void CUi::Uninit()
{
	CObject2D::Uninit();
}
//===================================================================================================

//====================================================
//�X�V����
//====================================================
void CUi::Update()
{
	CObject2D::Update();

	if (m_bUseUiEffect == true)
	{
		CUiEffect::Create(m_Type, GetPolygonType(), GetWidth(), GetHeight(), m_nSetUiEffectLife, GetPos(), D3DXVECTOR3(0.0f,0.0f,0.0f), GetRot(), m_SetUiEffectColor);
	}


	if (m_VecUiState.size() > 0)
	{//UI��Ԃ̓����z��̃f�[�^�������
		for (auto it = m_VecUiState.begin(); it != m_VecUiState.end(); ++it)
		{
			(*it)->Process(this);//���ꂼ��̏�Ԃ̏������J�n����
		}
	}
}
//===================================================================================================

//====================================================
//�`�揈��
//====================================================
void CUi::Draw()
{
	CObject2D::Draw();
}
//===================================================================================================

//====================================================
//���S�t���O�ݒ菈��
//====================================================
void CUi::SetDeath()
{
	if (GetUseDeath() == true)
	{
		for (auto it = m_VecUiState.begin(); it != m_VecUiState.end(); ++it)
		{
			if (*it != nullptr)
			{//�C�e���[�^���w������UI�X�e�[�g�ւ̃|�C���^��j�����Anullptr�ɂ���
				delete* it;
				*it = nullptr;
			}
		}
		
		m_VecUiState.clear();//�������̒��̏����N���A
		m_VecUiState.shrink_to_fit();//��������j��
	}
	CObject::SetDeath();
}
//===================================================================================================

//====================================================
//��������
//====================================================
CUi* CUi::Create(UITYPE type, CObject2D::POLYGONTYPE PolygonType, float fWidth, float fHeight, int nLife, bool bUseLife, D3DXVECTOR3 Pos, D3DXVECTOR3 Move, D3DXVECTOR3 Rot, D3DXCOLOR col)
{
	CUi* pUi = DBG_NEW CUi;   //�e�𐶐�
	bool bSuccess = pUi->CObject::GetCreateSuccess();
	CTexture* pTextureClass = CManager::GetTexture();           //�e�N�X�`���N���X���擾
	if (bSuccess == true)
	{
		if (pUi != nullptr)
		{
			pUi->SetUiType(type);                  //�w�i�̎�ނ�ݒ�
			pUi->SetUseLife(bUseLife, nLife, nLife);//�̗͐ݒ�
			pUi->Init();                                                     //����������
			pUi->SetMove(Move);//�ړ���
			pUi->SetRot(Rot);//����
			pUi->SetPos(Pos);//�ʒu
			pUi->SetSupportPos(Pos);//�x�_�ƂȂ�ʒu
			pUi->SetUseScale(true);//�g�嗦���g�p����
			pUi->CObject2D::SetAnimInfo(1, 1,true);//�|���S���ƃe�N�X�`������ݒ�
			pUi->SetWidth(fWidth);
			pUi->SetMaxWidth(fWidth);
			pUi->SetHeight(fHeight);
			pUi->SetMaxHeight(fHeight);
			pUi->SetPolygonType(PolygonType);
			pUi->SetColor(col,false,1.0f);
			pUi->SetUseDeath(true);                 //���S�t���O�𔭓����邩�ǂ�����ݒ肷��
			pUi->SetTextureIndex(pTextureClass->Regist(UI_FILENAME[int(type)]));
			pUi->CObject2D::BindTexture(pTextureClass->GetAddress(pUi->GetTextureIndex()));
			pUi->CObject::SetType(CObject::TYPE::UI);//�I�u�W�F�N�̎�ނ����߂�
		}
	}
	else
	{
		delete pUi;
		pUi = nullptr;
		return nullptr;
	}

	return pUi;
}
//===================================================================================================

//====================================================
//��������
//====================================================
void CUi::SetUiType(UITYPE type)
{
	m_Type = type;
	CTexture* pTextureClass = CManager::GetTexture();           //�e�N�X�`���N���X���擾
    SetTextureIndex(pTextureClass->Regist(UI_FILENAME[int(type)]));
    CObject2D::BindTexture(pTextureClass->GetAddress(GetTextureIndex()));

}
//===================================================================================================

//====================================================
//������Ԃɐݒ肷��
//====================================================
void CUi::SetNumericState(int nValue, float fWidth, float fHeight)
{
	for (auto it = m_VecUiState.begin(); it != m_VecUiState.end(); ++it)
	{
		if (CUiState::UISTATE::NUMERIC == (*it)->GetUiState())
		{
			return;//���ɏ�񂪑��݂��Ă���̂ŁA�ۑ������ɏI������
		}
	}
	m_VecUiState.push_back(DBG_NEW CUiState_Numeric(this, nValue, fWidth, fHeight));
}
//===================================================================================================

//====================================================
//UI�̏�Ԃ��擾����
//====================================================
CUiState* CUi::GetUiState(CUiState::UISTATE UiState)
{
	for (auto it = m_VecUiState.begin(); it != m_VecUiState.end(); ++it)
	{
		if (UiState == (*it)->GetUiState())
		{//�w�肵����Ԃ�����Ύ擾����
			return *it;
		}
	}
	return nullptr;
}
//===================================================================================================

//=======================================<<<<<<<<UI�G�t�F�N�g�N���X>>>>>>>>=========================================================================


//====================================================
//�R���X�g���N�^
//====================================================
CUiEffect::CUiEffect() : CUi(5)
{

}
//===================================================================================================

//====================================================
//�f�X�g���N�^
//====================================================
CUiEffect::~CUiEffect()
{
}
//===================================================================================================

//====================================================
//����������
//====================================================
HRESULT CUiEffect::Init()
{
	CUi::Init();
	return S_OK;
}
//===================================================================================================

//====================================================
//�I������
//====================================================
void CUiEffect::Uninit()
{
	CUi::Uninit();
}
//===================================================================================================

//====================================================
//�X�V����
//====================================================
void CUiEffect::Update()
{
	CUi::Update();
}
//===================================================================================================

//====================================================
//�`�揈��
//====================================================
void CUiEffect::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	//a�u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	CUi::Draw();
	//a�u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}
//===================================================================================================

//====================================================
//���S�t���O�ݒ菈��
//====================================================
void CUiEffect::SetDeath()
{
	CUi::SetDeath();
}
//===================================================================================================

//====================================================
//��������
//====================================================
CUiEffect* CUiEffect::Create(UITYPE type, CObject2D::POLYGONTYPE PolygonType, float fWidth, float fHeight, int nLife, D3DXVECTOR3 Pos, D3DXVECTOR3 Move, D3DXVECTOR3 Rot, D3DXCOLOR col)
{
	CUiEffect* pUiEffect = DBG_NEW CUiEffect;   //�e�𐶐�
	bool bSuccess = pUiEffect->CObject::GetCreateSuccess();
	CTexture* pTextureClass = CManager::GetTexture();           //�e�N�X�`���N���X���擾
	if (bSuccess == true)
	{
		if (pUiEffect != nullptr)
		{
			pUiEffect->SetUiType(type);                  //�w�i�̎�ނ�ݒ�
			pUiEffect->Init();                                                     //����������
			pUiEffect->SetMove(Move);//�ړ���
			pUiEffect->SetRot(Rot);//����
			pUiEffect->SetPos(Pos);//�ʒu
			pUiEffect->SetUseLife(true, nLife, nLife);//�̗͂��g�p����
			pUiEffect->SetSupportPos(Pos);//�x�_�ƂȂ�ʒu
			pUiEffect->SetUseLifeRatioColor(true);//�̗͂ɉ����ĐF������ς���
			pUiEffect->SetUseScale(true);//�g�嗦���g�p����
			pUiEffect->CObject2D::SetAnimInfo(1, 1,false);//�|���S���ƃe�N�X�`������ݒ�
			pUiEffect->SetWidth(fWidth);
			pUiEffect->SetMaxWidth(fWidth);
			pUiEffect->SetHeight(fHeight);
			pUiEffect->SetMaxHeight(fHeight);
			pUiEffect->SetPolygonType(PolygonType);
			pUiEffect->SetColor(col, false, 1.0f);
			pUiEffect->SetUseDeath(true);                 //���S�t���O�𔭓����邩�ǂ�����ݒ肷��
			pUiEffect->SetTextureIndex(pTextureClass->Regist(UI_FILENAME[int(type)]));
			pUiEffect->CObject2D::BindTexture(pTextureClass->GetAddress(pUiEffect->GetTextureIndex()));
			pUiEffect->CObject::SetType(CObject::TYPE::UI);//�I�u�W�F�N�̎�ނ����߂�
		}
	}
	else
	{
		delete pUiEffect;
		pUiEffect = nullptr;
		return nullptr;
	}

	return pUiEffect;
}
//===================================================================================================

//****************************************************
//UI�X�e�[�g�N���X
//****************************************************

//====================================================
//�R���X�g���N�^
//====================================================
CUiState::CUiState() : m_pUiState()
{

}
//===================================================================================================

//====================================================
//�f�X�g���N�^
//====================================================
CUiState::~CUiState()
{

}
//===================================================================================================

//====================================================
//����
//====================================================
void CUiState::Process(CUi* pUi)
{

}
//===================================================================================================

//****************************************************
//UI�X�e�[�g_����
//****************************************************

//====================================================
//�R���X�g���N�^
//====================================================
CUiState_Numeric::CUiState_Numeric(CUi* pUi, int nValue, float fWidth, float fHeight)
{
	SetUiState(CUiState::UISTATE::NUMERIC);
	m_nValue = nValue;
	m_fWidth = fWidth;//�����̊�l
	m_fHeight = fHeight;//�����̊�l
	int nDigit = CCalculation::CalculationDigit(m_nValue);
	if (m_nValue == 0)
	{//�������O�ɂ���킯�ɂ͂����Ȃ��̂ŁA�������P�Ƃ���
		nDigit = 1;
	}
	for (int nCnt = 0; nCnt < nDigit; nCnt++)
	{
		CNumber* pNumber = CNumber::Create(pUi->GetPos(), fWidth, fHeight);
		int nNum = CCalculation::getDigit(m_nValue, nCnt);
		pNumber->SetAnim(nNum);//�w�肵�����̐��l���擾����
		pNumber->SetUseDeath(false);//���S�t���O�𔭓������Ȃ�
		m_VecNum.push_back(pNumber);//Vector�ɕۑ�
	}
}
//===================================================================================================

//====================================================
//�f�X�g���N�^
//====================================================
CUiState_Numeric::~CUiState_Numeric()
{
	for (auto it : m_VecNum)
	{
		if (it != nullptr)
		{
			it->SetUseDeath(true);
			it->SetDeath();
			it = nullptr;
		}
	}

	m_VecNum.clear();
	m_VecNum.shrink_to_fit();
}
//===================================================================================================

//====================================================
//����
//====================================================
void CUiState_Numeric::Process(CUi* pUi)
{
	int nSize = m_VecNum.size();
	int nCnt = 0;
	for (auto it = m_VecNum.begin();it != m_VecNum.end();it++,nCnt++)
	{//���������ɕ��ב�����
		(*it)->SetPos(pUi->GetPos() + D3DXVECTOR3(pUi->GetWidth() / 2 + (*it)->GetWidth() / 2 + (*it)->GetWidth() * (nSize - 1) - ((*it)->GetWidth() * nCnt), 0.0f, 0.0f));
	}
}
//===================================================================================================

//====================================================
//���l��ݒ肷��
//====================================================
void CUiState_Numeric::SetValue(int nValue,CUi * pUi)
{
	//���l�̓��I�z��̒��g��S�Ĕj��
	for (auto it = m_VecNum.begin(); it != m_VecNum.end(); ++it)
	{
		if ((*it) != nullptr)
		{
			(*it)->SetUseDeath(true);
			(*it)->SetDeath();
			(*it) = nullptr;
		}
	}

	m_VecNum.clear();//�������̒��g��������

	m_nValue = nValue;//���l���i�[
	int nDigit = CCalculation::CalculationDigit(m_nValue);//�������v�Z

	if (m_nValue == 0)
	{//�������O�ɂ���킯�ɂ͂����Ȃ��̂ŁA�������P�Ƃ���
		nDigit = 1;
	}

	//���������l�̕\���𐶐������I�z��Ɋi�[����
	for (int nCnt = 0; nCnt < nDigit; nCnt++)
	{
		CNumber* pNumber = CNumber::Create(pUi->GetPos(),m_fWidth,m_fHeight);
		int nNum = CCalculation::getDigit(m_nValue, nCnt);//�w�肵�����̐��l���擾����
		pNumber->SetAnim(nNum);//�A�j���[�V�����p�^�[��
		pNumber->SetUseDeath(false);//���S�t���O�𔭓������Ȃ�
		m_VecNum.push_back(pNumber);//���I�z��ɕۑ�
	}
}
//===================================================================================================

//****************************************************
//UI�X�e�[�g_�Q�[�W�ێ�
//****************************************************

//====================================================
//�R���X�g���N�^
//====================================================
CUiState_Gauge::CUiState_Gauge(D3DXVECTOR3 GaugePos, D3DXCOLOR Col, CObject2D::POLYGONTYPE PolygonType, CGauge::GAUGETYPE GaugeType, float fMaxWidth, float fMaxHeight, int nValue, int nMaxValue)
{
	SetUiState(CUiState::UISTATE::GAUGE);//UI�̏�Ԃ�ݒ�
	m_pGauge = CGauge::Create(GaugeType, nMaxValue, fMaxWidth, fMaxHeight, GaugePos);//����
	m_pGauge->SetPolygonType(PolygonType);//�|���S���̎�ށi���S�_�j��ݒ�
	m_pGauge->SetParam(nValue);//�p�����[�^��ݒ�
	m_pGauge->SetColor(Col,false, 1.0f);//�F����
}
//===================================================================================================

//====================================================
//�f�X�g���N�^
//====================================================
CUiState_Gauge::~CUiState_Gauge()
{
	if (m_pGauge != nullptr)
	{//���S�t���O��ݒ肷��
		m_pGauge->SetUseDeath(true);
		m_pGauge->SetDeath();
		m_pGauge = nullptr;
	}
}
//===================================================================================================

//====================================================
//����
//====================================================
void CUiState_Gauge::Process(CUi* pUi)
{

}
//===================================================================================================