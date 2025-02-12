//=====================================================================
//
//�V���P�U���F�g���b�v�����[trap.cpp]
//Author:ShinaTaiyo
//
//=====================================================================

//======================================
//�C���N���[�h
//======================================
#include "trap.h"
#include "main.h"
#include "game.h"
#include "edit.h"
#include "manager.h"
#include "renderer.h"
#include "particle.h"
#include "player.h"
#include "fade.h"
#include "block.h"
#include "objectXInfo.h"
#include "stagemanager.h"
#include "debugtext.h"
#include "input.h"
#include "bullet.h"
//=====================================================================

//======================================
//�ÓI�����o������
//======================================
int CTrap::m_nNumFile = CTrap::TRAPTYPE_MAX;  //�t�@�C���̐�
const int CTrap::m_nTRAPSUBTYPENUM[CTrap::TRAPTYPE_MAX] =
{ 5,//�g�Q�{�[��
  2,//��C
  1,//�g�Q����_
};
const char* CTrap::m_TRAPFILENAME[CTrap::TRAPTYPE_MAX] =
{ "data\\MODEL\\Trap\\Trap00_NeedleBall.x",
   "data\\MODEL\\Trap\\Trap01_Cannon.x",
   "data\\MODEL\\Trap\\Trap02_Thornclub.x",
};//�g���b�v�̃��f���̃p�X

//======================================
//�R���X�g���N�^
//======================================
CTrap::CTrap() : m_CollisionType(COLLISIONTYPE00_SQUARE)
{

}
//=====================================================================

//======================================
//�f�X�g���N�^
//======================================
CTrap::~CTrap()
{

}
//=====================================================================

//======================================
//����������
//======================================
HRESULT CTrap::Init()
{
	//=================================
	//X�I�u�W�F�N�g������
	//=================================
	CObjectX::Init();

	//=================================================================

	m_type = TRAPTYPE00_NEEDLEBALL;//���
	m_bUse = true;                 //�d�l���
	m_nCntTime = 0;                //�o�����Ă���̎���
	m_nSubType = 0;                //�T�u�^�C�v
	m_bSpeedSwitch = false;        //������ς���t���O
	m_bSpeedSwitch2 = false;       //��L���̂Q
	m_bUseMove = true;             //�ړ��ʂ��g�p���邩�ǂ���
	m_bTurn = false;               //�^�[��������ς���t���O

	m_nInterval = 1;               //�l�X�ȃC���^�[�o��
	m_fRotSpeed = 0.1f;            //��]�X�s�[�h
	m_fSpeed = 0.2f;               //����
	m_fStartRot = 0.0f;            //�J�n����
	m_fMoveLength = 100.0f;        //�ړ��͈�
	m_fTurnSpeed = 5.0f;           //�^�[������X�s�[�h
	m_nCntSetTime = 0;             //�l�X�Ȏ��Ԃ�ݒ�
	m_fAddSpeed = 0.0f;            //�����x
	return S_OK;
}
//=====================================================================

//======================================
//�I������
//======================================
void CTrap::Uninit()
{
	CObjectX::Uninit(); //X�I�u�W�F�N�g�̔j��
}
//=====================================================================

//======================================
//�X�V����
//======================================
void CTrap::Update()
{
	bool bStageManagerChoose = GetStageManagerChoose();//�X�e�[�W�}�l�[�W���[�ɑI������Ă��邩�ǂ������擾

	if (bStageManagerChoose == false)
	{//�X�e�[�W�}�l�[�W���[�ɑI������Ă��邩�ǂ������擾

		//==================================
		//X�I�u�W�F�N�g��������擾
		//==================================
		D3DXVECTOR3& Pos = CObjectX::GetPos();                      //�I�u�W�F�N�g�̈ʒu���擾
		D3DXVECTOR3& Rot = CObjectX::GetRot();                      //�I�u�W�F�N�g�̌������擾
		D3DXVECTOR3& Move = CObjectX::GetMove();                    //�I�u�W�F�N�g�̈ړ��ʂ��擾
		int& nLife = GetLife();                                     //�̗�
		//===========================================================

		//==================================
		//�g���b�v���Ƃ̃p�^�[��
		//==================================
		m_nCntTime++;//�o�����Ă���̎��Ԃ𑪂�

		//==================================
		//���W�X�V
		//==================================
		if (m_bUseMove == true)
		{
			Pos += Move;
		}
		//===========================================================

		//==================================
		//���ꂼ��̋���
		//==================================
		switch (m_type)
		{
		case TRAPTYPE00_NEEDLEBALL:
			NeedleBallProcess();
			break;
		case TRAPTYPE01_CANNON:
			CannonProcess();
			break;
		case TRAPTYPE02_THORNCLUB:
			ThornClubProcess();
			break;
		default:
			break;
		}
		//===========================================================

		//==================================
		//�����蔻����s��
		//==================================
		if (CScene::GetMode() == CScene::MODE_GAME)
		{
			switch (m_CollisionType)
			{
			case COLLISIONTYPE00_SQUARE:
				Collision();
				break;
			case COLLISIONTYPE01_RECTANGLESQUARE:

			default:
				break;
			}
		}
		//===========================================================

		//==================================
		//�g�p��ԊǗ�
		//==================================
		if (nLife <= 0)
		{//�̗͂��O�ɂȂ�����E�E�E
			m_bUse = false;
		}
		//===========================================================

		//==================================
		//�g���b�v�̌��ʔ����Ɣj��
		//==================================
		if (m_bUse == false)
		{
			SetDeath();
		}
		//===========================================================
	}
	else
	{//�X�e�[�W�}�l�[�W���[�ɑI������Ă��鎞��������\��

	}

	//==================================
    //X�I�u�W�F�N�g�X�V
    //==================================
	CObjectX::Update();
	//===========================================================

}
//=====================================================================

//======================================
//�`�揈��
//======================================
void CTrap::Draw()
{
	CObjectX::Draw();
}
//=====================================================================

//=======================================
//���S�t���O��ݒ�
//=======================================
void CTrap::SetDeath()
{
	CObject::SetDeath();
}
//=====================================================================

//======================================
//�g���b�v��������
//======================================
CTrap* CTrap::Create(TRAPTYPE type, D3DXVECTOR3 Pos, D3DXVECTOR3 Rot, D3DXVECTOR3 Scale, int nRotType, int nSubType)
{
	CTrap* pTrap = DBG_NEW CTrap;   //�G�𐶐�
	bool bSuccess = pTrap->CObject::GetCreateSuccess();
	int nIdx = 0;//���f���̃C���f�b�N�X
	if (bSuccess == true)
	{
		if (pTrap != nullptr)
		{
			pTrap->Init();         //����������
			pTrap->SetRot(Rot);                                                                                     //������ݒ�                    
			switch (type)
			{
			case TRAPTYPE02_THORNCLUB:
				pTrap->m_CollisionType = COLLISIONTYPE01_RECTANGLESQUARE;//�΂߂̔�����s��
				break;
			default:
				break;
			}

			pTrap->SetUseDeath(true);//���S�t���O�𔭓����邩�ǂ�����ݒ肷��
			pTrap->m_bUse = true;  //�g�p���
			pTrap->SetUseGravity(false);
			pTrap->m_type = type;  //�e�̎��
			pTrap->m_nSubType = nSubType;//�T�u�^�C�v
			pTrap->CObjectX::SetPos(Pos);               
			pTrap->SetUseBlockCollision(false);//�ʒu��ݒ�
			pTrap->SetLife(1);
			pTrap->SetMaxLife(1);
			pTrap->SetAutoSubLife(false);
			pTrap->SetPosOld(Pos);                                                                                  //1f�O�̈ʒu��ݒ�
			pTrap->SetSupportPos(Pos);                                                                              //�ݒu�ʒu
			pTrap->CObject::SetType(CObject::TYPE_TRAP);                                                            //�I�u�W�F�N�g�̎�ނ����߂�
			pTrap->CObjectX::SetScale(Scale);                                                                       //�I�u�W�F�N�g�̊g�嗦
			pTrap->SetFormarScale(Scale);                                                                           //���̊g�嗦��ݒ肷��
			nIdx = CManager::GetObjectXInfo()->Regist(CObjectX::OBJECTXTYPE_TRAP, m_TRAPFILENAME[(int)(type)]);
			pTrap->CObjectX::BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(CObjectX::OBJECTXTYPE_TRAP, nIdx),
				CManager::GetObjectXInfo()->GetBuffMat(CObjectX::OBJECTXTYPE_TRAP, nIdx),
				CManager::GetObjectXInfo()->GetdwNumMat(CObjectX::OBJECTXTYPE_TRAP, nIdx),
				CManager::GetObjectXInfo()->GetTexture(CObjectX::OBJECTXTYPE_TRAP, nIdx),
				CManager::GetObjectXInfo()->GetColorValue(CObjectX::OBJECTXTYPE_TRAP, nIdx));                       //���f���������蓖�Ă�

			pTrap->CObjectX::SetObjXType(CObjectX::OBJECTXTYPE_TRAP);                                               //�I�u�W�F�N�gX�̃^�C�v��ݒ�
			pTrap->CObjectX::SetTypeNum((int)(type));                                                               //�I�u�W�F�N�gX���Ƃ̃^�C�v�ԍ���ݒ�
			pTrap->SetSize();                                                                                       //X�I�u�W�F�N�g�̃T�C�Y��ݒ肷��
			pTrap->CObjectX::SetRotType(CObjectX::ROTTYPE::ROTTYPE_NORMAL);                                         //�����̎�ނ�ݒ�
			pTrap->SetManagerType((int)(CStageManager::MANAGEROBJECT_TRAP));                                        //�}�l�[�W���[�ŌĂяo�����̎�ނ�ݒ�
		}
		else
		{//�I�u�W�F�N�g�ɋ󂫂��Ȃ������̂Ŕj������
			delete pTrap;
			pTrap = nullptr;
			return nullptr;
		}
	}
	return pTrap;
}
//=====================================================================================================

//========================================================================================
//����ݒ肷��
//========================================================================================
void CTrap::SetInfo(float fStartRot, float fSpeed, float fRotSpeed, float fMoveLength, float fTurnSpeed, int nInterval, float fAddSpeed)
{
	m_fStartRot = fStartRot;     //��]�J�n�ʒu
	D3DXVECTOR3& Rot = GetRot();
	switch (GetTrapType())
	{
	case TRAPTYPE02_THORNCLUB:
		Rot.z = fStartRot;
		break;
	default:
		break;
	}
	m_fSpeed = fSpeed;           //����
	m_fRotSpeed = fRotSpeed;     //��]���x
	m_fMoveLength = fMoveLength; //�ړ�����
	m_fTurnSpeed = fTurnSpeed;   //�^�[������X�s�[�h
	m_nInterval = nInterval;     //�l�X�ȃC���^�[�o��
	m_fAddSpeed = fAddSpeed;     //�����x
}
//=====================================================================================================

//====================================================
//�����蔻����s��
//====================================================
void CTrap::Collision(void)
{

}
//=====================================================================================================

//===============================================================
//�g�Q�{�[���̏���
//===============================================================
void CTrap::NeedleBallProcess(void)
{
	D3DXVECTOR3& Pos = CObjectX::GetPos();                      //�I�u�W�F�N�g�̈ʒu���擾
	D3DXVECTOR3 SupportPos = CObjectX::GetSupportPos();         //�x�_�̈ʒu���擾
	D3DXVECTOR3& Rot = CObjectX::GetRot();                      //�I�u�W�F�N�g�̌������擾
	D3DXVECTOR3& Move = GetMove();                              //�ړ��ʂ��擾
    //�x�_�ƌ��ݒn�̋����𑪂�
	float fLength = sqrtf(powf(Pos.x - SupportPos.x, 2) + powf(Pos.y - SupportPos.y, 2) + powf(Pos.z - SupportPos.z, 2));

	if (m_nCntTime == 1)
	{//�ŏ��̌�����ݒ�
		Rot.z = m_fStartRot;
	}

	switch (m_nSubType)
	{
	case 0://��]�ړ�
		Rot += ONE_VECTOR3 * m_fRotSpeed;
		Pos.x = GetSupportPos().x + sinf(m_fStartRot + Rot.z) * m_fMoveLength;
		Pos.y = GetSupportPos().y + cosf(m_fStartRot + Rot.z) * m_fMoveLength;
		break;
	case 1://XY�ʈړ�
		Rot += ONE_VECTOR3 * -0.1f;
		if (m_bSpeedSwitch == false)
		{
			Move.x -= sinf(m_fStartRot) * m_fSpeed;
			//Move.y -= cosf(m_fStartRot) * m_fSpeed;
		}
		else
		{
			Move.x += sinf(m_fStartRot) * m_fSpeed;
			//Move.y += cosf(m_fStartRot) * m_fSpeed;
		}

		if (m_bSpeedSwitch2 == false)
		{
			Move.y -= cosf(m_fStartRot) * m_fSpeed;
		}
		else
		{
			Move.y += cosf(m_fStartRot) * m_fSpeed;
		}

		if (Move.x <= -m_fTurnSpeed)
		{
			m_bSpeedSwitch = true;
		}
		else if (Move.x >= m_fTurnSpeed)
		{
			m_bSpeedSwitch = false;
		}

		if (Move.y <= -m_fTurnSpeed)
		{
			m_bSpeedSwitch2 = true;
		}
		else if (Move.y >= m_fTurnSpeed)
		{
			m_bSpeedSwitch2 = false;
		}
		break;
	case 2://�����^�[��
		m_bUseMove = false;//�ړ��ʂ��g�p���Ȃ�

		Rot += ONE_VECTOR3 * -0.1f;
		if (m_bSpeedSwitch == false)
		{
			m_fSpeed -= m_fAddSpeed;

		}
		else
		{
			m_fSpeed += m_fAddSpeed;
		}

		Move.x = sinf(m_fStartRot) * m_fSpeed;
		Move.y = cosf(m_fStartRot) * m_fSpeed;

		if (fabsf(m_fSpeed) >= m_fTurnSpeed)
		{
			m_bSpeedSwitch = m_bSpeedSwitch ? false : true;
		}
		break;
	case 3://�^�[���ړ�

		Rot += ONE_VECTOR3 * 0.1f;
		//�ړ������𒴂�����
		if (fLength > m_fMoveLength)
		{
			m_bTurn = m_bTurn ? false : true;
		}
		//���������鏈��
		if (m_bTurn == false)
		{
			Move.x = cosf(m_fStartRot) * m_fSpeed;
			Move.y = sinf(m_fStartRot) * m_fSpeed;
		}
		else
		{
			Move.x = cosf(m_fStartRot) * -m_fSpeed;
			Move.y = sinf(m_fStartRot) * -m_fSpeed;
		}
		break;
	case 4:
		Rot += ONE_VECTOR3;
		break;
	default:
		break;
	}
}
//=====================================================================================================

//====================================================
//��C�̏���
//====================================================
void CTrap::CannonProcess(void)
{
	if (CScene::GetMode() == CScene::MODE_GAME)
	{
		float fVXaim = CGame::GetPlayer()->GetPos().x - GetPos().x;
		float fVYaim = CGame::GetPlayer()->GetPos().y - GetPos().y;
		float fVLaim = atan2f(fVXaim, fVYaim);
		D3DXVECTOR3& Scale = GetScale();
		D3DXVECTOR3& Rot = GetRot();
		CBullet* pBullet = nullptr;

		switch (m_nSubType)
		{
		case 0://���@�_���e
			Rot.z = -fVLaim;
			if (m_nCntTime % m_nInterval == 0)
			{
				pBullet = CBullet::Create(CBullet::BULLETTYPE_ENEMY,CBullet::BULLETTEX_ENEMY, 120, 1, 40.0f * Scale.x, 40.0f * Scale.y, GetPos(), D3DXVECTOR3(sinf(fVLaim) * m_fSpeed, cosf(fVLaim) * m_fSpeed, 0.0f));
				pBullet->SetBlockCollision(true);
			}
			break;
		case 1://������e
			if (m_nCntTime % m_nInterval == 0)
			{
			   pBullet = CBullet::Create(CBullet::BULLETTYPE_ENEMY, CBullet::BULLETTEX_ENEMY, 120, 1, 40.0f * Scale.x, 40.0f * Scale.y, GetPos(), D3DXVECTOR3(sinf(-Rot.z) * m_fSpeed, cosf(-Rot.z) * m_fSpeed, 0.0f));
			   pBullet->SetBlockCollision(true);
			}
			break;
		default:
			break;
		}
	}
}
//=====================================================================================================

//====================================================
//�g�Q����_�̏���
//====================================================
void CTrap::ThornClubProcess()
{
	D3DXVECTOR3& Rot = GetRot();
	Rot.z += m_fRotSpeed;

}
//=====================================================================================================

//====================================================
//���\������
//====================================================
void CTrap::DispInfo()
{
	D3DXVECTOR3& Pos = GetPos();//�ʒu���擾
	D3DXVECTOR3& SupportPos = GetSupportPos();//�x�_�ƂȂ�ʒu���擾
	if (GetStageManagerChoose() == true)
	{//�X�e�[�W�}�l�[�W���[���I�����Ă�����E�E�E

		//���x
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//�V�t�g�L�[�������Ȃ���E�E�E
			if (CManager::GetInputKeyboard()->GetTrigger(DIK_1) == true)
			{
				m_fSpeed -= 0.1f;

			}
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_1) == true)
		{
			m_fSpeed += 0.1f;

		}

		//��]���x
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//�V�t�g�L�[�������Ȃ���E�E�E
			if (CManager::GetInputKeyboard()->GetTrigger(DIK_2) == true)
			{
				m_fRotSpeed -= 0.01f;
			}
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_2) == true)
		{
			m_fRotSpeed += 0.01f;
		}

		//�ړ�����
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//�V�t�g�L�[�������Ȃ���E�E�E
			if (CManager::GetInputKeyboard()->GetTrigger(DIK_3) == true)
			{
				m_fMoveLength -= 10.0f;
			}
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_3) == true)
		{
			m_fMoveLength += 10.0f;
		}

		//��]�J�n�ʒu
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//�V�t�g�L�[�������Ȃ���E�E�E
			if (CManager::GetInputKeyboard()->GetTrigger(DIK_4) == true)
			{
				m_fStartRot -= 0.1f;
			}
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_4) == true)
		{
			m_fStartRot += 0.1f;
		}

		//U�^�[�����鑬�x
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//�V�t�g�L�[�������Ȃ���E�E�E
			if (CManager::GetInputKeyboard()->GetTrigger(DIK_5) == true)
			{
				m_fTurnSpeed -= 0.1f;
			}
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_5) == true)
		{
			m_fTurnSpeed += 0.1f;
		}

		//�C���^�[�o��
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//�V�t�g�L�[�������Ȃ���E�E�E
			if (CManager::GetInputKeyboard()->GetTrigger(DIK_6) == true)
			{
				m_nInterval -= 1;
			}
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_6) == true)
		{
			m_nInterval += 1;
		}

		//�����x
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//�V�t�g�L�[�������Ȃ���E�E�E
			if (CManager::GetInputKeyboard()->GetTrigger(DIK_7) == true)
			{
				m_fAddSpeed -= 0.05f;
			}
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_7) == true)
		{
			m_fAddSpeed += 0.05f;
		}

		if (m_nInterval <= 1)
		{
			m_nInterval = 1;
		}

		CObjectX::DispInfo();

		CManager::GetDebugText()->PrintDebugText("//=============================================\n");
		CManager::GetDebugText()->PrintDebugText("//�I���g���b�v���\n");
		CManager::GetDebugText()->PrintDebugText("//=============================================\n");
		CManager::GetDebugText()->PrintDebugText("�ړ����x(1)�F%f\n", m_fSpeed);
		CManager::GetDebugText()->PrintDebugText("��]���x(2)�F%f\n", m_fRotSpeed);
		CManager::GetDebugText()->PrintDebugText("�ړ�����(3)�F%f\n", m_fMoveLength);
		CManager::GetDebugText()->PrintDebugText("��]�J�n�ʒu(4)�F%f\n", m_fStartRot);
		CManager::GetDebugText()->PrintDebugText("�^�[�����x(5)�F%f\n", m_fTurnSpeed);
		CManager::GetDebugText()->PrintDebugText("�C���^�[�o��(6)�F%d\n", m_nInterval);
		CManager::GetDebugText()->PrintDebugText("�����x(7)�F%f\n", m_fAddSpeed);
		CManager::GetDebugText()->PrintDebugText("//=============================================\n");
	}
}
//=====================================================================================================