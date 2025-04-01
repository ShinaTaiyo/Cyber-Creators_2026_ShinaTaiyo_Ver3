//======================================================================================================================
//
//�P�O���Q�S���F�U���p�̃N���X�����[attack.cpp]
//Author:ShinaTaiyo
//
//======================================================================================================================

//==================================================================
//�C���N���[�h
//==================================================================
#include "attack.h"
#include "damage.h"
#include "manager.h"
#include "enemy.h"
#include "player.h"
#include"gauge.h"
#include "objectXInfo.h"
#include "block.h"
#include "bgModel.h"
#include "particle.h"
#include "game.h"
#include "collision.h"
//======================================================================================================================

//==================================================================
//�O���錾
//==================================================================
const string CAttack::ATTACK_FILENAME[static_cast<int>(CAttack::ATTACKTYPE::MAX)] =
{
	"data\\MODEL\\Attack\\Fire_000.x",
	"data\\MODEL\\Attack\\Explosion_000.x",
	"data\\MODEL\\Attack\\MagicSword_000.x",
};
//======================================================================================================================

//==================================================================
//�R���X�g���N�^
//==================================================================
CAttack::CAttack(int nPower, int nSetHitStopTime, int nPri, bool bUseintPri, CObject::TYPE type, CObject::OBJECTTYPE ObjType) : CObjectX(nPri, bUseintPri, type, ObjType),
m_Type(ATTACKTYPE::BULLET), m_nPower(nPower), m_HitStop({0,nSetHitStopTime}),m_bCollisionRelease(true),m_CollisionType(CAttack::COLLISIONTYPE::NONE),
m_TargetType(CAttack::TARGETTYPE::NONE),m_bHitOtherThanLiving(false),m_bAutoCollision(true),m_bCollisionSuccess(false),m_bExtrusionCollision(false),
m_BoundInfo(),m_CollisionObjType(CObject::TYPE::NONE)
{

}
//======================================================================================================================

//==================================================================
//�f�X�g���N�^
//==================================================================
CAttack::~CAttack()
{

}
//======================================================================================================================

//==================================================================
//����������
//==================================================================
HRESULT CAttack::Init()
{
	CObjectX::Init();//�I�u�W�F�N�gX������
	return S_OK;
}
//======================================================================================================================

//==================================================================
//�I������
//==================================================================
void CAttack::Uninit()
{
	CObjectX::Uninit();//�I�u�W�F�N�gX�I������
}
//======================================================================================================================

//==================================================================
//�X�V����
//==================================================================
void CAttack::Update()
{
	CObjectX::Update();//�I�u�W�F�N�gX�X�V����

	m_bCollisionSuccess = false;//�����蔻�肪�����������ǂ������}�C�t���[�����Z�b�g

	if (m_bAutoCollision == true)
	{//�����Ă�����̂Ƃ̓����蔻����s���i�v���C���[�A�G�A�{�X�Ȃǁj
		Collision();
	}

	if (m_bHitOtherThanLiving == true)
	{//�����Ă�����̈ȊO�i�w�i��u���b�N�j�Ƃ������蔻����s��
		HitOtherCollisionProcess();
	}
	else
	{//�����Ă�����̂Ƃ��������蔻����s���i���������������j
		if (m_bExtrusionCollision == true)
		{//�����o��������s���i�����ɓ������Ă�������킯����Ȃ��j
			ExtrusionCollisionProcess();

			if (m_BoundInfo.GetActive() == true)
			{//�o�E���h����Ȃ�o�E���h������
				m_BoundInfo.BoundProcess(this);
			}
		}
	}
}
//======================================================================================================================

//==================================================================
//�`�揈��
//==================================================================
void CAttack::Draw()
{
	CObjectX::Draw();//�I�u�W�F�N�gX�`�揈��
}
//======================================================================================================================

//==================================================================
//���S�t���O�ݒ菈��
//==================================================================
void CAttack::SetDeath()
{
	CObjectX::SetDeath();//�I������
}
//======================================================================================================================

//==================================================================
//�����蔻�菈��
//==================================================================
void CAttack::Collision()
{
	for (int nCntPri = 0; nCntPri < m_nMAXPRIORITY; nCntPri++)
	{//�S�Ẵ��X�g������
		CObject* pObj = GetTopObject(nCntPri);//���X�g�̐擪�I�u�W�F�N�g���擾
		while (pObj != nullptr)
		{
		    bool bNowCollision = false;//�����������ǂ�����������
			CObject* pNext = pObj->GetNextObject();//���̃I�u�W�F�N�g���擾

			if (pObj->GetType() == CObject::TYPE::ENEMY && m_TargetType == TARGETTYPE::ENEMY)
			{//�I�u�W�F�N�g���G�ŁA�U�����_���I�u�W�F�N�g���u�G�v�������ꍇ
				CEnemy* pEnemy = static_cast<CEnemy*>(pObj);//�I�u�W�F�N�g��G�ɃL���X�g
				CollisionProcess(m_bCollisionSuccess, bNowCollision, pEnemy);//�����蔻��̏���
				if (m_bCollisionSuccess == true)
				{//�����蔻�肪��L�̏����Ő������Ă�����
					m_CollisionObjType = CObject::TYPE::ENEMY;//���������I�u�W�F�N�g�̃^�C�v���i�[����
					if (pEnemy->GetLifeInfo().GetLife() < 1)
					{//�G�̗̑͂��Ȃ��Ȃ��Ă�����U�����|�����I�u�W�F�N�g�̃^�C�v���u�G�v�ɐݒ�
						pEnemy->SetDefeatAttack(m_Type);
					}
				}
			}
			else if (pObj->GetType() == CObject::TYPE::PLAYER && m_TargetType == TARGETTYPE::PLAYER)
			{//�I�u�W�F�N�g���u�v���C���[�v�ōU�����_���I�u�W�F�N�g���u�v���C���[�v�������ꍇ
				CObjectX* pObjX = static_cast<CObjectX*>(pObj);//�I�u�W�F�N�g���I�u�W�F�N�gX�ɃL���X�g
				CollisionProcess(m_bCollisionSuccess, bNowCollision, pObjX);//�����蔻��̏������s��

				if (m_bCollisionSuccess == true)
				{//��L�̏����œ����蔻�肪�������Ă�����
					m_CollisionObjType = CObject::TYPE::PLAYER;//���������I�u�W�F�N�g�̃^�C�v���i�[����
				}
			}

			pObj = pNext;//���X�g�����ɐi�߂�
		}
	}

	if (m_bCollisionSuccess == true && GetCollisionRelease() == true)
	{//�����蔻�肪�������A�����������ɃI�u�W�F�N�g�������t���O��true�Ȃ�A���S�t���O�𔭓�����
		SetDeath();
	}

}
//======================================================================================================================

//==================================================================
//���̃I�u�W�F�N�g�ɂ����Ă鏈��
//==================================================================
void CAttack::HitOtherCollisionProcess()
{
	for (int nCntPri = 0; nCntPri < m_nMAXPRIORITY; nCntPri++)
	{//�S�Ẵ��X�g����������
		CObject* pObj = GetTopObject(nCntPri);//�擪�I�u�W�F�N�g���擾����
		while (pObj != nullptr)
		{//�I�u�W�F�N�g��nullptr�ɂȂ�܂ŌJ��Ԃ�
			bool bNowCollision = false;//���ݓ����������ǂ�����������
			CObject* pNext = pObj->GetNextObject();//���X�g�̎��̃I�u�W�F�N�g���擾

			if (pObj->GetType() == CObject::TYPE::BGMODEL || pObj->GetType() == CObject::TYPE::BLOCK)
			{//�I�u�W�F�N�g�̃^�C�v���u�w�i���f���v���u�u���b�N�v�Ȃ�
				CObjectX* pObjX = static_cast<CObjectX*>(pObj);//�I�u�W�F�N�g���u�I�u�W�F�N�gX�v�ɃL���X�g

				if (CCollision::CollisionSquare(GetPosInfo().GetPos(), GetSizeInfo().GetVtxMax(), GetSizeInfo().GetVtxMin(), pObjX->GetPosInfo().GetPos(), pObjX->GetSizeInfo().GetVtxMax(), pObjX->GetSizeInfo().GetVtxMin()))
				{//�����`�̓����蔻����s��
					m_bCollisionSuccess = true;//�����蔻�肪���������t���O���I���ɂ���
					m_CollisionObjType = pObj->GetType();//���������I�u�W�F�N�g�̃^�C�v���i�[����
				}
			}

			pObj = pNext;//���X�g�����ɐi�߂�
		}
	}

	if (m_bCollisionSuccess == true && m_bCollisionRelease == true)
	{//�����蔻�肪�������A�����������ɏ����t���O��true�ɂȂ��Ă����玀�S�t���O�𔭓�����
		SetDeath();
	}

}
//======================================================================================================================

//==================================================================
//���̃I�u�W�F�N�g�Ƃ̉����o��������s������
//==================================================================
void CAttack::ExtrusionCollisionProcess()
{
	CObjectX::CollisionInfo& CollisionInfo = GetCollisionInfo();//�����蔻������擾����
	CObjectX::CollisionInfo::State& CollisionState = CollisionInfo.GetState();//�t���O�����Z�b�g
	CollisionState.SetLandingOld(CollisionState.GetLanding());     //1f�O�ɃI�u�W�F�N�g���n�ʂɏ���Ă��邩�ǂ�����ݒ�
	CollisionState.SetWallingOld(CollisionState.GetWalling());     //1f�O�ɃI�u�W�F�N�g���ǂɂ��邩�ǂ�����ݒ�
	CollisionState.ResetState();                                   //�X�e�[�g
	CollisionInfo.GetSquareInfo().ResetPushOutFirstFlag();         //���ꂼ��̎��̉����o������̗D��t���O�����Z�b�g

	for (int nCntPri = 0; nCntPri < CObject::m_nMAXPRIORITY; nCntPri++)
	{//�S�Ẵ��X�g����������
		CObject* pObj = CObject::GetTopObject(nCntPri);//���X�g�̐擪�I�u�W�F�N�g���擾����

		while (pObj != nullptr)
		{//�I�u�W�F�N�g��nullptr�ɂȂ�܂ŌJ��Ԃ�
			//���̃I�u�W�F�N�g���i�[
			CObject* pNext = pObj->GetNextObject();//���̃I�u�W�F�N�g���擾����

			//��ނ̎擾�i�G�Ȃ瓖���蔻��j
			CObject::TYPE type = pObj->GetType();

			if (type == CObject::TYPE::BLOCK || type == CObject::TYPE::BGMODEL)
			{//�I�u�W�F�N�g�̎�ނ��u�u���b�N�v�܂��́u�w�i���f���v�Ȃ�
				CObjectX* pObjX = static_cast<CObjectX*>(pObj);//�I�u�W�F�N�gX�Ƀ_�E���L���X�g
				CCollision::ExtrusionCollisionSquarePushOutFirstDecide(this, pObjX);//�����`�̉����o������̂��ꂼ��̎��̏����̗D��x�����߂�
			}
			//�I�u�W�F�N�g�����ɐi�߂�
			pObj = pNext;
		}
	}
	//=======================================================================================

	//============================================================
	//�����o������J�n
	//============================================================
	for (int nCntPri = 0; nCntPri < CObject::m_nMAXPRIORITY; nCntPri++)
	{//�S�Ẵ��X�g����������
		CObject* pObj = CObject::GetTopObject(nCntPri);//���X�g�̐擪�I�u�W�F�N�g���擾����

		while (pObj != nullptr)
		{//�I�u�W�F�N�g��nullptr�ɂȂ�܂ŌJ��Ԃ�
			//���̃I�u�W�F�N�g���i�[
			CObject* pNext = pObj->GetNextObject();

			//��ނ̎擾�i�G�Ȃ瓖���蔻��j
			CObject::TYPE type = pObj->GetType();

			if (type == CObject::TYPE::BLOCK || type == CObject::TYPE::BGMODEL)
			{//�I�u�W�F�N�g�̎�ނ��u�u���b�N�v�܂��́u�w�i���f���v�Ȃ�
				CObjectX* pObjX = static_cast<CObjectX*>(pObj);//�I�u�W�F�N�gX�Ƀ_�E���L���X�g

				CCollision::ResolveExtrusionCollisionSquare(this, pObjX);//�����`�̉����o�����������
			}

			pObj = pNext;//�I�u�W�F�N�g�����ɐi�߂�
		}
	}
	//=======================================================================================
}
//======================================================================================================================

//==================================================================
//�����蔻�苤�ʏ���
//==================================================================
void CAttack::CollisionProcess(bool& bCollision, bool& bNowCollision, CObjectX* pObjX)
{
	switch (GetCollisionType())
	{//�����蔻��̎�ނɂ���ĕς���
	case CAttack::COLLISIONTYPE::SQUARE://�����`�̓����蔻��
		if (CCollision::CollisionSquare(GetPosInfo().GetPos(), GetSizeInfo().GetVtxMax(), GetSizeInfo().GetVtxMin(), pObjX->GetPosInfo().GetPos(), pObjX->GetSizeInfo().GetVtxMax(), pObjX->GetSizeInfo().GetVtxMin()) == true)
		{//�����`�̓����蔻����s��
			bCollision = true;//��������
			bNowCollision = true;//���ݓ�������
		}
		break;
	case CAttack::COLLISIONTYPE::RECTANGLE_XZ://XZ�����̎΂߂̓����蔻��
		if (CCollision::RectAngleCollisionXZ(this, pObjX) == true)
		{//XZ�����̎΂߂̓����蔻����s��
			bCollision = true;//��������
			bNowCollision = true;//���ݓ�������
		}
		break;
	default:
		break;
	}

	if (bNowCollision == true)
	{//����������
		pObjX->SetDamage(GetPower(), m_HitStop.nSetTime);//�����ɐݒ肳��Ă���I�u�W�F�N�g�Ƀ_���[�W��^����
		pObjX->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), m_HitStop.nSetTime, false, false,false);//�����ɐݒ肳��Ă���I�u�W�F�N�g�̐F������Ԃ�����
	}

}
//======================================================================================================================

//=============================================================================�v���C���[�U���N���X=============================================================================

//==================================================================
//�R���X�g���N�^
//==================================================================
CAttackPlayer::CAttackPlayer(int nPower, int nSetHitStopTime) : CAttack(nPower,nSetHitStopTime)
{

}
//======================================================================================================================

//==================================================================
//�f�X�g���N�^
//==================================================================
CAttackPlayer::~CAttackPlayer()
{

}
//======================================================================================================================

//==================================================================
//����������
//==================================================================
HRESULT CAttackPlayer::Init()
{
	CAttack::Init();//�U���̏���������
	return S_OK;
}
//======================================================================================================================

//==================================================================
//�I������
//==================================================================
void CAttackPlayer::Uninit()
{
	CAttack::Uninit();//�U���̏I������
}
//======================================================================================================================

//==================================================================
//�X�V����
//==================================================================
void CAttackPlayer::Update()
{
	CAttack::Update();//�U���̍X�V����

	BulletCollisionProcess();//�e�������������̏���
}
//======================================================================================================================

//==================================================================
//�`�揈��
//==================================================================
void CAttackPlayer::Draw()
{
	CAttack::Draw();//�U���̕`�揈��
}
//======================================================================================================================

//==================================================================
//���S�t���O�ݒ菈��
//==================================================================
void CAttackPlayer::SetDeath()
{
	CAttack::SetDeath();//�U���̎��S�t���O�ݒ菈��
}
//======================================================================================================================

//==================================================================
//��������
//==================================================================
CAttackPlayer* CAttackPlayer::Create(ATTACKTYPE AttackType, TARGETTYPE TargetType, COLLISIONTYPE CollisionType,
	bool bHitOtherThanLiving, bool bAutoCollision, int nPower, int nSetHitStopTime, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, D3DXVECTOR3 Scale)
{
	CAttackPlayer* pAttackPlayer = nullptr;                                         //�v���C���[�̍U���ւ̃|�C���^��������
	pAttackPlayer = DBG_NEW CAttackPlayer(nPower,nSetHitStopTime);                  //����
	pAttackPlayer->Init();                                                          //����������
	pAttackPlayer->SetTargetType(TargetType);                                       //�^�[�Q�b�g�^�C�v��ݒ�
	pAttackPlayer->SetCollisionType(CollisionType);                                 //����^�C�v��ݒ�
	pAttackPlayer->SetType(CObject::TYPE::ATTACK);                                  //�I�u�W�F�N�g���Ƃ̃^�C�v��ݒ肷��
	pAttackPlayer->SetAttackType(AttackType);                                       //�U���̎�ނ�ݒ肷��
	pAttackPlayer->GetLifeInfo().SetLife(nLife);                                    //�̗͂�ݒ�
	pAttackPlayer->GetLifeInfo().SetMaxLife(nLife);                                 //�ő�̗͂�ݒ�
	pAttackPlayer->GetPosInfo().SetPos(pos);                                        //�ʒu  
	pAttackPlayer->GetPosInfo().SetSupportPos(pos);                                 //�x�_�ʒu
	pAttackPlayer->GetRotInfo().SetRot(rot);                                        //����
	pAttackPlayer->GetMoveInfo().SetMove(move);                                     //�ړ���
	pAttackPlayer->GetSizeInfo().SetScale(Scale);                                   //�g�嗦
	pAttackPlayer->GetLifeInfo().SetAutoSubLife(true);                              //�̗͂��g�p����
	pAttackPlayer->GetLifeInfo().SetAutoDeath(true);                                //�̗͂��O�ɂȂ����Ƃ��Ɏ����I�Ɏ��S�t���O�𔭓����� 
	pAttackPlayer->GetMoveInfo().SetUseInteria(false, CObjectX::GetNormalInertia());//�������g�p���Ȃ�
	pAttackPlayer->GetMoveInfo().SetUseGravity(false,1.0f);                         //�d�͂��g�p���Ȃ�
	pAttackPlayer->SetHitOtherThanLibing(bHitOtherThanLiving);                      //�w�i�I�u�W�F�N�g�Ƃ������蔻����s�����ǂ���
	pAttackPlayer->SetAutoCollision(bAutoCollision);                                //�U���̓����蔻����U���N���X�ɔC���邩�ǂ���

	//���f�����ݒ�
	int nIdx = CManager::GetObjectXInfo()->Regist(ATTACK_FILENAME[static_cast<int>(AttackType)]);

	//���f���������蓖�Ă�
	pAttackPlayer->BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(nIdx),
		CManager::GetObjectXInfo()->GetBuffMat(nIdx),
		CManager::GetObjectXInfo()->GetdwNumMat(nIdx),
		CManager::GetObjectXInfo()->GetTexture(nIdx),
		CManager::GetObjectXInfo()->GetColorValue(nIdx));

	pAttackPlayer->SetSize();//�T�C�Y��ݒ肷��
	return pAttackPlayer;
}
//======================================================================================================================

//==================================================================
//�e�������������̏���
//==================================================================
void CAttackPlayer::BulletCollisionProcess()
{
	if (CScene::GetMode() == CScene::MODE::MODE_GAME)
	{//�Q�[���V�[���Ȃ�
		if (GetCollisionSuccess() == true && GetAttackType() == ATTACKTYPE::BULLET)
		{//�����蔻�肪�������Ă��āA�U���̎�ނ��u�e�v��������
			CObjectX::PosInfo& PosInfo = GetPosInfo();//�ʒu�����擾
			const D3DXVECTOR3& Pos = PosInfo.GetPos();//�ʒu
			//�p�[�e�B�N�����Փˈʒu�ɏ�������
			if (GetCollisionObjType() == CObject::TYPE::ENEMY)
			{//�G�ɓ������Ă�����
				CPlayer* pPlayer = CGame::GetPlayer();//�v���C���[�ւ̃|�C���^���擾
				CUi* pDiveGaugeFrameUi = pPlayer->GetDiveGaugeFrame();//�_�C�u�Q�[�W�t���[���ւ̃|�C���^
				//�_�C�u�Q�[�W�̋@�\�̎擾
				CUIComposite_Container* pDiveGaugeFrameUiCompositeContainer = pDiveGaugeFrameUi->GetUiCompositeContainer();                //�Q�[�W�t���[���̃R���|�W�b�g�p�^�[���̃R���e�i���擾����
				CUIComposite_Gauge* pDiveGaugeUi_CompositeGauge = pDiveGaugeFrameUiCompositeContainer->GetChildren<CUIComposite_Gauge>();  //�Q�[�W�̋@�\���擾����
				if (pDiveGaugeUi_CompositeGauge != nullptr)
				{
					CGauge* pDiveGauge = pDiveGaugeUi_CompositeGauge->GetGauge();//�_�C�u�Q�[�W���擾����

					CUi* pDivePossibleNum = pPlayer->GetDivePossibleNum();//�_�C�u�\�񐔂�UI���擾

					//�_�C�u�\�񐔂�UI�̃R���|�W�b�g�p�^�[������@�\���擾
					CUIComposite_Container* pDivePossibleNumCompositeContainer = pDivePossibleNum->GetUiCompositeContainer();//�_�C�u�\�񐔂̃R���|�W�b�g�p�^�[���̃R���e�i���擾����
					CUIComposite_Numeric* pDivePossibleNum_Numeric = pDivePossibleNumCompositeContainer->GetChildren<CUIComposite_Numeric>();//�_�C�u�\�񐔂̐��l�\���@�\���擾����

					if (pDivePossibleNum_Numeric != nullptr)
					{//�擾����UI�̋@�\��������������
						if (pDivePossibleNum_Numeric->GetValue() < CPlayer::GetMaxDiveNum())
						{//�ő�_�C�u�\�񐔂ɒB���Ă��Ȃ��ꍇ�̓_�C�u�\�񐔂��{�P����
							pDiveGauge->SetParam(pDiveGauge->GetParam() + 1);
						}
					}
				}
				//�p�[�e�B�N������������
				CParticle::SummonParticle(CParticle::TYPE::TYPE00_NORMAL, 20, 30, 30.0f, 30.0f, 100, 10, true, Pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), true);
			}
			else
			{//����ȊO�Ȃ�

				//�p�[�e�B�N������������
				CParticle::SummonParticle(CParticle::TYPE::TYPE00_NORMAL, 20, 30, 30.0f, 30.0f, 100, 10, false, Pos, D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), true);
			}
		}
	}
}
//======================================================================================================================

//=============================================================================�G�U���N���X=============================================================================

//==================================================================
//�R���X�g���N�^
//==================================================================
CAttackEnemy::CAttackEnemy(int nPower, int nSetHitStopTime) : CAttack(nPower,nSetHitStopTime)
{

}
//======================================================================================================================

//==================================================================
//�f�X�g���N�^
//==================================================================
CAttackEnemy::~CAttackEnemy()
{

}
//======================================================================================================================

//==================================================================
//����������
//==================================================================
HRESULT CAttackEnemy::Init()
{
	CAttack::Init();//�U���̏���������
	return S_OK;
}
//======================================================================================================================

//==================================================================
//�I������
//==================================================================
void CAttackEnemy::Uninit()
{
	CAttack::Uninit();//�U���̏I������
}
//======================================================================================================================

//==================================================================
//�X�V����
//==================================================================
void CAttackEnemy::Update()
{
	CAttack::Update();//�U���̍X�V����
}
//======================================================================================================================

//==================================================================
//�`�揈��
//==================================================================
void CAttackEnemy::Draw()
{
	CAttack::Draw();//�U���̕`�揈��
}
//======================================================================================================================

//==================================================================
//���S�t���O�ݒ菈��
//==================================================================
void CAttackEnemy::SetDeath()
{
	CAttack::SetDeath();//�U���̎��S�t���O�ݒ菈��
}
//======================================================================================================================

//==================================================================
//�G�̍U���𐶐�
//==================================================================
CAttackEnemy* CAttackEnemy::Create(ATTACKTYPE AttackType, TARGETTYPE TargetType, COLLISIONTYPE CollisionType,
	bool bHitOtherThanLiving, bool bAutoCollision, int nPower, int nSetHitStopTime, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, D3DXVECTOR3 Scale)
{
	CAttackEnemy* pAttackEnemy = DBG_NEW CAttackEnemy(nPower, nSetHitStopTime);

	pAttackEnemy->Init();                                       //����������
	pAttackEnemy->SetType(CObject::TYPE::ATTACK);               //�I�u�W�F�N�g���Ƃ̃^�C�v��ݒ肷��
	pAttackEnemy->SetAttackType(AttackType);                    //�U���̎�ނ�ݒ肷��
	pAttackEnemy->SetTargetType(TargetType);                    //�^�[�Q�b�g�^�C�v��ݒ肷��
	pAttackEnemy->SetCollisionType(CollisionType);              //����^�C�v��ݒ肷��
	pAttackEnemy->GetLifeInfo().SetLife(nLife);                 //�̗͂�ݒ�
	pAttackEnemy->GetLifeInfo().SetMaxLife(nLife);              //�ő�̗͂�ݒ�
	pAttackEnemy->GetLifeInfo().SetAutoSubLife(true);           //�̗͂������I�Ɍ��炷
	pAttackEnemy->GetPosInfo().SetPos(pos);                     //�ʒu  
	pAttackEnemy->GetRotInfo().SetRot(rot);                     //����
	pAttackEnemy->GetMoveInfo().SetMove(move);                  //�ړ���
	pAttackEnemy->GetSizeInfo().SetScale(Scale);                //�g�嗦
	pAttackEnemy->GetMoveInfo().SetUseInteria(false, CObjectX::GetNormalInertia());//�������g�p���Ȃ�
	pAttackEnemy->GetLifeInfo().SetAutoDeath(true);                                //�̗͂��Ȃ��Ȃ������Ɏ����I�Ɏ��S�t���O�𔭓�����
	pAttackEnemy->GetMoveInfo().SetUseGravity(false, 1.0f);                        //�d�͂��g�p����
	pAttackEnemy->SetHitOtherThanLibing(bHitOtherThanLiving);                      //�w�i�I�u�W�F�N�g�Ƃ������蔻��𔭓����邩�ǂ���
	pAttackEnemy->SetAutoCollision(bAutoCollision);                                //�U���N���X�ɔ����C���邩�ǂ���
	//���f�����ݒ�
	int nIdx = CManager::GetObjectXInfo()->Regist(ATTACK_FILENAME[static_cast<int>(AttackType)]);

	//���f���������蓖�Ă�
	pAttackEnemy->BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(nIdx),
		CManager::GetObjectXInfo()->GetBuffMat(nIdx),
		CManager::GetObjectXInfo()->GetdwNumMat(nIdx),
		CManager::GetObjectXInfo()->GetTexture(nIdx),
		CManager::GetObjectXInfo()->GetColorValue(nIdx));
	pAttackEnemy->SetSize();//�I�u�W�F�N�g�̃T�C�Y��ݒ肷��
	return pAttackEnemy;
}
//======================================================================================================================

//==================================================================
//�o�E���h����
//==================================================================
void CAttack::BoundInfo::BoundProcess(CAttack* pAttack)
{
	if (pAttack->GetCollisionInfo().GetState().GetLanding() == true)
	{//�n�ʂɏ���Ă�����

		//���˂�����
		pAttack->GetMoveInfo().SetMove(D3DXVECTOR3(pAttack->GetMoveInfo().GetMove().x, Power.y, pAttack->GetMoveInfo().GetMove().z));

		if (bGravity == true)
		{//�d�͂�ON�ɂ���ꍇ
			pAttack->GetMoveInfo().SetUseGravity(true, fGravity);
		}
	}
}
//======================================================================================================================