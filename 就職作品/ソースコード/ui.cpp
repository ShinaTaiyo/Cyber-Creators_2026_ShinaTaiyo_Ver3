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
const string CUi::UI_FILENAME[static_cast<int>(CUi::UITYPE::MAX)] =
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
	"data\\TEXTURE\\UI\\DiveGaugeFrame_000.png",
	"data\\TEXTURE\\UI\\GameClear_000.png",
	"data\\TEXTURE\\UI\\Lose_000.png",
	"data\\TEXTURE\\UI\\Combo_000.png",
	"data\\TEXTURE\\UI\\PauseText_000.png",
	"data\\TEXTURE\\UI\\ExitText_000.png",
	"data\\TEXTURE\\UI\\ContinueText_000.png",
	"data\\TEXTURE\\UI\\CameraSensitivityText_000.png",
	"data\\TEXTURE\\UI\\ReturnText_000.png",
};//�e�N�X�`���t�@�C����

//====================================================
//�R���X�g���N�^
//====================================================
CUi::CUi(int nPri, bool bUseintPri, CObject::TYPE type, CObject::OBJECTTYPE ObjType) : CObject2D(nPri,bUseintPri,type,ObjType),
m_MoveType(UIMOVETYPE_NORMAL),m_Type(UITYPE::LOCKON),m_bUseUiEffect(false),m_nSetUiEffectLife(0),m_SetUiEffectColor(D3DXCOLOR(1.0f,1.0f,1.0f,1.0f)),m_pUiCompositeContainer(DBG_NEW CUIComposite_Container(this))
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
	CObject2D::Init();//�I�u�W�F�N�g2D����������
	return S_OK;
}
//===================================================================================================

//====================================================
//�I������
//====================================================
void CUi::Uninit()
{
	CObject2D::Uninit();//�I�u�W�F�N�g2D�I������

	if (m_pUiCompositeContainer != nullptr)
	{//UI�̋@�\��j��
		delete m_pUiCompositeContainer;
		m_pUiCompositeContainer = nullptr;
	}
}
//===================================================================================================

//====================================================
//�X�V����
//====================================================
void CUi::Update()
{
	CObject2D::Update();//�I�u�W�F�N�g2D�X�V����

	m_pUiCompositeContainer->Update(this);//�@�\�̍X�V����
}
//===================================================================================================

//====================================================
//�`�揈��
//====================================================
void CUi::Draw()
{
	CObject2D::Draw();//�I�u�W�F�N�g2D�`�揈��

	m_pUiCompositeContainer->Update(this);//�@�\�̕`�揈��
}
//===================================================================================================

//====================================================
//���S�t���O�ݒ菈��
//====================================================
void CUi::SetDeath()
{
	CObject2D::SetDeath();//�I�u�W�F�N�g���S�t���O�ݒ菈��
}
//===================================================================================================

//====================================================
//��������
//====================================================
CUi* CUi::Create(UITYPE type, bool bPauseUi, CObject2D::POLYGONTYPE PolygonType, float fWidth, float fHeight, int nLife, bool bUseLife, D3DXVECTOR3 Pos, D3DXVECTOR3 Move, D3DXVECTOR3 Rot, D3DXCOLOR col)
{
	CUi* pUi = nullptr;                                                              //UI�ւ̃|�C���^��������
	CTexture* pTextureClass = CManager::GetTexture();                                //�e�N�X�`�������擾
	if (bPauseUi)
	{//�|�[�Y�p��UI�ɂ���
		pUi = DBG_NEW CUi(0, false, TYPE::UI_PAUSE);
	}
	else
	{//���ʂ�UI�ɂ���
		pUi = DBG_NEW CUi();
	}
	pUi->SetUiType(type);                                                            //UI�̎�ނ�ݒ�
	pUi->SetUseLife(bUseLife, nLife, nLife);                                         //�̗͐ݒ�
	pUi->Init();                                                                     //����������
	pUi->SetMove(Move);                                                              //�ړ���
	pUi->SetRot(Rot);                                                                //����
	pUi->SetPos(Pos);                                                                //�ʒu
	pUi->SetSupportPos(Pos);                                                         //�x�_�ƂȂ�ʒu
	pUi->SetUseScale(true);                                                          //�g�嗦���g�p����
	pUi->CObject2D::SetAnimInfo(1, 1,true);                                          //�|���S���ƃe�N�X�`������ݒ�
	pUi->SetWidth(fWidth);                                                           //������ݒ�
	pUi->SetMaxWidth(fWidth);                                                        //�ő剡����ݒ�
	pUi->SetHeight(fHeight);                                                         //������ݒ�
	pUi->SetMaxHeight(fHeight);                                                      //�ő卂����ݒ�
	pUi->SetPolygonType(PolygonType);                                                //�|���S���̎�ނ�ݒ�
	pUi->SetColor(col,false,1.0f);                                                   //�F������ݒ�
	pUi->SetUseDeath(true);                                                          //���S�t���O�𔭓����邩�ǂ�����ݒ肷��
	pUi->SetTextureIndex(pTextureClass->Regist(UI_FILENAME[int(type)]));             //�e�N�X�`����o�^���A�e�N�X�`���ԍ����擾����
	pUi->CObject2D::BindTexture(pTextureClass->GetAddress(pUi->GetTextureIndex()));  //�e�N�X�`�������蓖�Ă�
	pUi->CObject::SetType(CObject::TYPE::UI);                                        //�I�u�W�F�N�̎�ނ����߂�
	return pUi;
}
//===================================================================================================

//====================================================
//��������
//====================================================
void CUi::SetUiType(UITYPE type)
{
	m_Type = type;//��ނ�ύX
	CTexture* pTextureClass = CManager::GetTexture();                     //�e�N�X�`���N���X���擾
    SetTextureIndex(pTextureClass->Regist(UI_FILENAME[int(type)]));       //�e�N�X�`����o�^���A�e�N�X�`���ԍ���ݒ�
    CObject2D::BindTexture(pTextureClass->GetAddress(GetTextureIndex())); //�e�N�X�`�������蓖�Ă�

}
//===================================================================================================