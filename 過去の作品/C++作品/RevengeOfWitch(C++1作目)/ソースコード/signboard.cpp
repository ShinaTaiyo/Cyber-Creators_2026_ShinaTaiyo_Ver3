//=====================================================================================================================
//
//�X���R���F���ĊŔ���������[signboard.cpp]
//Author:ShinaTaiyo
//
//=====================================================================================================================

//==================================================================
//�C���N���[�h
//==================================================================
#include "signboard.h"
#include "renderer.h"
#include "manager.h"
#include "objectXInfo.h"
#include "stagemanager.h"
#include "calculation.h"
#include "game.h"
#include "texture.h"
#include "scene.h"
//=====================================================================================================================

//==================================================================
//�ÓI�����o�錾
//==================================================================
const char* CSignBoard::m_apSIGNBOARD_TEXTURENAME[CSignBoard::SIGNBOARDMANUALTYPE_MAX] =
{
	"data\\TEXTURE\\SignBoard\\Manual_000_Move_NoSuccess.png",
	"data\\TEXTURE\\SignBoard\\Manual_001_Jump_NoSuccess.png",
	"data\\TEXTURE\\SignBoard\\Manual_002_Dash_NoSuccess.png",
	"data\\TEXTURE\\SignBoard\\Manual_003_WallJump_NoSuccess.png",
	"data\\TEXTURE\\SignBoard\\Manual_004_Shot_NoSuccess.png",
	"data\\TEXTURE\\SignBoard\\Manual_005_ChargeShot_NoSuccess.png",
	"data\\TEXTURE\\SignBoard\\Manual_006_RainShot_NoSuccess.png",
	"data\\TEXTURE\\SignBoard\\Manual_007_BurstShot_NoSuccess.png",
	"data\\TEXTURE\\SignBoard\\Manual_008_RapidShot_NoSuccess.png",
	"data\\TEXTURE\\SignBoard\\Manual_009_Magic_NoSuccess.png",
	"data\\TEXTURE\\SignBoard\\Manual_010_MagicChenge_NoSuccess.png",
	"data\\TEXTURE\\SignBoard\\Manual_011_DoubleJump_NoSuccess.png",
};

const char* CSignBoard::m_apSIGNBOARD_TEXTURENAME_SUCCESS[CSignBoard::SIGNBOARDMANUALTYPE_MAX] =
{
	"data\\TEXTURE\\SignBoard\\Manual_000_Move_Success.png",
	"data\\TEXTURE\\SignBoard\\Manual_001_Jump_Success.png",
	"data\\TEXTURE\\SignBoard\\Manual_002_Dash_Success.png",
	"data\\TEXTURE\\SignBoard\\Manual_003_WallJump_Success.png",
	"data\\TEXTURE\\SignBoard\\Manual_004_Shot_Success.png",
	"data\\TEXTURE\\SignBoard\\Manual_005_ChargeShot_Success.png",
	"data\\TEXTURE\\SignBoard\\Manual_006_RainShot_Success.png",
	"data\\TEXTURE\\SignBoard\\Manual_007_BurstShot_Success.png",
	"data\\TEXTURE\\SignBoard\\Manual_008_RapidShot_Success.png",
	"data\\TEXTURE\\SignBoard\\Manual_009_Magic_Success.png",
	"data\\TEXTURE\\SignBoard\\Manual_010_MagicChenge_Success.png",
	"data\\TEXTURE\\SignBoard\\Manual_011_DoubleJump_Success.png",
};

const char* CSignBoard::m_apSIGNBOARD_FILENAME[CSignBoard::SIGNBOARDTYPE_MAX] =
{
	"data\\MODEL\\SignBoard\\SignBoard_000.x",
};

//���ꂼ��̃T�u�^�C�v
int CSignBoard::m_SIGNBOARDSUBTYPENUM[CSignBoard::SIGNBOARDTYPE_MAX] =
{
	(int)(CSignBoard::SIGNBOARDMANUALTYPE_MAX)
};

//==================================================================
//�R���X�g���N�^
//==================================================================
CSignBoard::CSignBoard() : m_bSuccessJudge(false),m_ManualType(SIGNBOARDMANUALTYPE00_MOVE),m_SignBoardType(SIGNBOARDTYPE00_NORMAL),m_nSubType(0)
{

}
//=====================================================================================================================

//==================================================================
//�f�X�g���N�^
//==================================================================
CSignBoard::~CSignBoard()
{

}
//=====================================================================================================================

//==================================================================
//����������
//==================================================================
HRESULT CSignBoard::Init()
{
	CObjectX::Init();
	m_ManualType = SIGNBOARDMANUALTYPE00_MOVE;
	m_nSubType = 0;
	return S_OK;
}
//=====================================================================================================================

//==================================================================
//�I������
//==================================================================
void CSignBoard::Uninit()
{
	CObjectX::Uninit();
}
//=====================================================================================================================

//==================================================================
//�X�V����
//==================================================================
void CSignBoard::Update()
{
	CObjectX::Update();
	if (CScene::GetMode() == CScene::MODE_GAME)
	{
		float fLength = CCalculation::CalculationLength(D3DXVECTOR3(GetPos().x,0.0f, 0.0f),
			D3DXVECTOR3(CGame::GetPlayer()->GetPos().x,0.0f, 0.0f));//�v���C���[�Ƃ̋����𑪂�
		bool bUseCommand = CGame::GetPlayer()->GetJudgeCommand(CPlayer::PLAYERCOMMAND(m_ManualType));//�v���C���[���w�肳�ꂽ�R�}���h���g�p���Ă��邩�ǂ���
		CTexture* pTexture = CManager::GetTexture();

		if (fLength < GetSize().x / 2)
		{
			if (bUseCommand == true && m_bSuccessJudge == false)
			{
				m_bSuccessJudge = true;                                                                                             //���߂�����̔��肪����
				SetLockOnMatBindTexture(0, pTexture->GetAddress(pTexture->Regist(m_apSIGNBOARD_TEXTURENAME_SUCCESS[m_ManualType])));//�}�e���A���O�ԂɃe�N�X�`���𔽉f������
			}
		}
	}
}
//=====================================================================================================================

//==================================================================
//�`�揈��
//==================================================================
void CSignBoard::Draw()
{
	CObjectX::Draw();
}
//=====================================================================================================================

//==================================================================
//���S�t���O�ݒ菈��
//==================================================================
void CSignBoard::SetDeath()
{
	CObject::SetDeath();
}
//=====================================================================================================================

//==================================================================
//��������
//==================================================================
CSignBoard* CSignBoard::Create(SIGNBOARDTYPE Type, SIGNBOARDMANUALTYPE ManualType, D3DXVECTOR3 pos, D3DXVECTOR3 Scale, D3DXVECTOR3 Rot)
{
	CSignBoard* pSignBoard = DBG_NEW CSignBoard;   //���ĊŔ𐶐�
	CTexture* pTexture = CManager::GetTexture();
	bool bSuccess = pSignBoard->CObject::GetCreateSuccess();
	int nIdx = 0;//���f���̃C���f�b�N�X
	if (bSuccess == true)
	{
		if (pSignBoard != nullptr)
		{
			pSignBoard->Init();         //����������
			pSignBoard->m_nSubType = int(ManualType);//�T�u�^�C�v��ݒ�
			pSignBoard->SetUseDeath(true);                                                                  //���S�t���O�𔭓����邩�ǂ�����ݒ肷��
			pSignBoard->SetUseBlockCollision(false);
			pSignBoard->SetManualType(ManualType);                                                                //�����̎��
			pSignBoard->SetRot(Rot);                                                                        //������ݒ�
			pSignBoard->SetUseGravity(false);
			pSignBoard->CObjectX::SetPos(pos);                                                              //�I�u�W�F�N�g�Q�c�̈ʒu��ݒ�
			pSignBoard->SetPosOld(pos);                                                                     //1f�O�̈ʒu��ݒ�
			pSignBoard->SetSupportPos(pos);                                                                 //�ݒu�ʒu
			pSignBoard->CObject::SetType(CObject::TYPE_MARKER);                                             //�I�u�W�F�N�g�̎�ނ����߂�
			pSignBoard->CObjectX::SetScale(Scale);                                                          //�I�u�W�F�N�g�̊g�嗦
			pSignBoard->SetFormarScale(Scale);                                                              //���̊g�嗦��ݒ肷��
			pSignBoard->SetAutoSubLife(false);//�����G�ɑ̗͂����炷���ǂ���
			nIdx = CManager::GetObjectXInfo()->Regist(CObjectX::OBJECTXTYPE_MARKER,"data\\MODEL\\SignBoard\\SignBoard_001.x");
			pSignBoard->CObjectX::BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(CObjectX::OBJECTXTYPE_MARKER, nIdx),
				CManager::GetObjectXInfo()->GetBuffMat(CObjectX::OBJECTXTYPE_MARKER, nIdx),
				CManager::GetObjectXInfo()->GetdwNumMat(CObjectX::OBJECTXTYPE_MARKER, nIdx),
				CManager::GetObjectXInfo()->GetTexture(CObjectX::OBJECTXTYPE_MARKER, nIdx),
				CManager::GetObjectXInfo()->GetColorValue(CObjectX::OBJECTXTYPE_MARKER, nIdx));                             //���f���������蓖�Ă�

			pSignBoard->SetLockOnMatBindTexture(0, pTexture->GetAddress(pTexture->Regist(m_apSIGNBOARD_TEXTURENAME[ManualType])));//�}�e���A���O�ԂɃe�N�X�`���𔽉f������

			pSignBoard->CObjectX::SetObjXType(CObjectX::OBJECTXTYPE_SIGNBOARD);                 //�I�u�W�F�N�gX�̃^�C�v��ݒ�
			pSignBoard->SetSize();                                                           //X�I�u�W�F�N�g�̃T�C�Y��ݒ肷��
			pSignBoard->CObjectX::SetRotType(CObjectX::ROTTYPE::ROTTYPE_NORMAL);             //�����̎�ނ�ݒ�
			pSignBoard->SetManagerType((int)(CStageManager::MANAGEROBJECT_SIGNBOARD));       //�}�l�[�W���[�ŌĂяo�����̎�ނ�ݒ�

			pSignBoard->SummonTestEnemy();//�e�X�g�p�̓G����������
		}
		else
		{//�I�u�W�F�N�g�ɋ󂫂��Ȃ������̂Ŕj������
			delete pSignBoard;
			pSignBoard = nullptr;
			return nullptr;
		}
	}
	return pSignBoard;
}
//=====================================================================================================================

//==================================================================
//���ĊŔ������F�ړ��̏���
//==================================================================
void CSignBoard::ManualShotProcess()
{
	float fLength = CCalculation::CalculationLength(D3DXVECTOR3(GetPos().x,GetPos().y,0.0f), 
		D3DXVECTOR3(CGame::GetPlayer()->GetPos().x, CGame::GetPlayer()->GetPos().y, 0.0f));//�v���C���[�Ƃ̋����𑪂�
	bool bUseCommand = CGame::GetPlayer()->GetJudgeCommand(CPlayer::PLAYERCOMMAND00_MOVE);//�v���C���[���w�肳�ꂽ�R�}���h���g�p���Ă��邩�ǂ���
	CTexture* pTexture = CManager::GetTexture();

	if (fLength < GetSize().x / 2)
	{
		if (bUseCommand == true && m_bSuccessJudge == false)
		{
			m_bSuccessJudge = true;                                                                                             //���߂�����̔��肪����
			SetLockOnMatBindTexture(0, pTexture->GetAddress(pTexture->Regist(m_apSIGNBOARD_TEXTURENAME_SUCCESS[m_ManualType])));//�}�e���A���O�ԂɃe�N�X�`���𔽉f������
		}
	}
}
//=====================================================================================================================

//==================================================================
//�W�����v����
//==================================================================
void CSignBoard::ManualJumpProcess()
{
	float fLength = CCalculation::CalculationLength(D3DXVECTOR3(GetPos().x, GetPos().y, 0.0f),
		D3DXVECTOR3(CGame::GetPlayer()->GetPos().x, CGame::GetPlayer()->GetPos().y, 0.0f));//�v���C���[�Ƃ̋����𑪂�
	bool bUseCommand = CGame::GetPlayer()->GetJudgeCommand(CPlayer::PLAYERCOMMAND01_JUMP);//�v���C���[���w�肳�ꂽ�R�}���h���g�p���Ă��邩�ǂ���
	CTexture* pTexture = CManager::GetTexture();

	if (fLength < GetSize().x / 2)
	{
		if (bUseCommand == true && m_bSuccessJudge == false)
		{
			m_bSuccessJudge = true;                                                                                             //���߂�����̔��肪����
			SetLockOnMatBindTexture(0, pTexture->GetAddress(pTexture->Regist(m_apSIGNBOARD_TEXTURENAME_SUCCESS[m_ManualType])));//�}�e���A���O�ԂɃe�N�X�`���𔽉f������
		}
	}
}
//=====================================================================================================================

//==================================================================
//�_�b�V�������Ŕ̏���
//==================================================================
void CSignBoard::ManualDashProcess()
{
}
//=====================================================================================================================

//==================================================================
//�e�X�g�p�̓G���������鏈��
//==================================================================
void CSignBoard::SummonTestEnemy()
{

}
//=====================================================================================================================