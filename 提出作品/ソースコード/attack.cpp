//======================================================================================================================
//
// �Q�O�Q�T�N�S���P�X���F�R�[�h���Y��ɂ���[attack.cpp]
//Author:ShinaTaiyo
//
//======================================================================================================================

//**********************************************************************************************************************
// �C���N���[�h
//**********************************************************************************************************************

#include "attack.h"      // ���w�b�_
#include "damage.h"      // �_���[�W
#include "manager.h"     // �}�l�[�W���[
#include "enemy.h"       // �G
#include "player.h"      // �v���C���[
#include "gauge.h"       // �Q�[�W
#include "objectXInfo.h" // �I�u�W�F�N�gX���
#include "block.h"       // �u���b�N
#include "bgModel.h"     // �w�i���f��
#include "particle.h"    // �p�[�e�B�N��
#include "game.h"        // �Q�[��
#include "collision.h"   // �R���W����
#include "ui.h"          // UI
//**********************************************************************************************************************
// �ÓI�����o�錾
//**********************************************************************************************************************

// �U�����f���t�@�C����
const string CAttack::ATTACK_FILENAME[static_cast<int>(CAttack::ATTACKTYPE::MAX)] =
{
	"data\\MODEL\\Attack\\Fire_000.x",
	"data\\MODEL\\Attack\\Explosion_000.x",
	"data\\MODEL\\Attack\\MagicSword_000.x",
};

//**********************************************************************************************************************
// �U���N���X
//**********************************************************************************************************************

//=====================================================================================================================
// 
// public�����o 
//
//=====================================================================================================================

//=====================================================================================================================
// �R���X�g���N�^
//=====================================================================================================================
CAttack::CAttack(int nPower, int nSetHitStopTime, int nPri, bool bUseintPri, CObject::TYPE type, CObject::OBJECTTYPE ObjType) :
	CObjectX(nPri, bUseintPri, type, ObjType),
	m_Type{ ATTACKTYPE::BULLET },
	m_nPower{nPower}, 
	m_HitStop{ {nSetHitStopTime} },
	m_bCollisionRelease{true},
	m_CollisionType{CAttack::COLLISIONTYPE::NONE},
	m_TargetType{CAttack::TARGETTYPE::NONE},
	m_bHitOtherThanLiving{false},
	m_bAutoCollision{true},
	m_bCollisionSuccess{false},
	m_bExtrusionCollision{false},
    m_BoundInfo{},
	m_CollisionObjType{CObject::TYPE::NONE}
{

}

//=====================================================================================================================
// �f�X�g���N�^
//=====================================================================================================================
CAttack::~CAttack()
{

}

//=====================================================================================================================
// ����������
//=====================================================================================================================
HRESULT CAttack::Init()
{
	CObjectX::Init(); // �I�u�W�F�N�gX������
	return S_OK;
}

//=====================================================================================================================
// �I������
//=====================================================================================================================
void CAttack::Uninit()
{
	CObjectX::Uninit(); // �I�u�W�F�N�gX�I������
}

//=====================================================================================================================
// �X�V����
//=====================================================================================================================
void CAttack::Update()
{
	// === �ϐ������� ===
	m_bCollisionSuccess = false; // �����蔻�肪�����������ǂ������}�C�t���[�����Z�b�g

	// === ���� ===

	CObjectX::Update(); // �I�u�W�F�N�gX�X�V����

	// �����Ă�����̂Ƃ̓����蔻����s���i�v���C���[�A�G�A�{�X�Ȃǁj
	if (m_bAutoCollision == true)
	{
		Collision();
	}

	// �����Ă�����̈ȊO�i�w�i��u���b�N�j�Ƃ������蔻����s���i�I�t�̏ꍇ�A�����Ă��Ȃ����f�������蔲����j
	if (m_bHitOtherThanLiving == true)
	{
		CollisionBuilding();
	}
	// �����Ă�����̂Ƃ��������蔻����s���i���������������j
	else
	{
		// �����o��������s���i�����ɓ������Ă�������킯����Ȃ��j
		if (m_bExtrusionCollision == true)
		{
			ExtrusionCollisionProcess();

			// �o�E���h����Ȃ�o�E���h������
			if (m_BoundInfo.GetActive() == true)
			{
				m_BoundInfo.BoundProcess(this);
			}
		}
	}
}

//=====================================================================================================================
// �`�揈��
//=====================================================================================================================
void CAttack::Draw()
{
	CObjectX::Draw(); // �I�u�W�F�N�gX�`�揈��
}

//=====================================================================================================================
// ���S�t���O�ݒ菈��
//=====================================================================================================================
void CAttack::SetDeath()
{
	CObjectX::SetDeath(); // �I�u�W�F�N�gX���S�t���O����
}

//=====================================================================================================================
// ���莞�ɔj�����邩�ǂ����ݒ�
//=====================================================================================================================
void CAttack::SetCollisionRelease(bool bUse)
{
	m_bCollisionRelease = bUse;
}

//=====================================================================================================================
// ���莞�ɔj�����邩�ǂ����擾
//=====================================================================================================================
const bool& CAttack::GetCollisionRelease() const
{
	return m_bCollisionRelease;
}

//=====================================================================================================================
// ���G��ԏ����擾
//=====================================================================================================================
const CAttack::HitStop& CAttack::GetHitStop()
{
	return m_HitStop;
}

//=====================================================================================================================
// �З͎擾
//=====================================================================================================================
const int& CAttack::GetPower() const
{
	return m_nPower;
}

//=====================================================================================================================
// ����^�C�v�ݒ�
//=====================================================================================================================
void CAttack::SetCollisionType(COLLISIONTYPE Type)
{
	m_CollisionType = Type;
}

//=====================================================================================================================
// ����^�C�v�擾
//=====================================================================================================================
const CAttack::COLLISIONTYPE CAttack::GetCollisionType() const
{
	return m_CollisionType;
}

//=====================================================================================================================
// ������I�u�W�F�N�g�^�C�v�ݒ�
//=====================================================================================================================
void CAttack::SetTargetType(TARGETTYPE Type)
{
	m_TargetType = Type;
}

//=====================================================================================================================
// ������I�u�W�F�N�g�^�C�v�擾
//=====================================================================================================================
const CAttack::TARGETTYPE CAttack::GetTargetType() const
{
	return m_TargetType;
}

//=====================================================================================================================
// �����Ă���I�u�W�F�N�g�ȊO�ɂ������邩�ǂ����ݒ�
//=====================================================================================================================
void CAttack::SetHitOtherThanLibing(bool bHit)
{
	m_bHitOtherThanLiving = bHit;
}

//=====================================================================================================================
// �����Ă���I�u�W�F�N�g�ȊO�ɂ������邩�ǂ����擾
//=====================================================================================================================
const bool& CAttack::GetHitOtherThanLibing() const
{
	return m_bHitOtherThanLiving;
}

//=====================================================================================================================
// �����蔻����U���N���X�ɔC���āh�����I�Ɂh���肷�邩�ǂ����ݒ�
//=====================================================================================================================
void CAttack::SetAutoCollision(bool bAuto)
{
	m_bAutoCollision = bAuto;
}

//=====================================================================================================================
// �����蔻����U���N���X�ɔC���āh�����I�Ɂh���肷�邩�ǂ����擾
//=====================================================================================================================
const bool& CAttack::GetAutoCollision()
{
	return m_bAutoCollision;
}

//=====================================================================================================================
// �����o��������s�����ǂ����ݒ�
//=====================================================================================================================
void CAttack::SetExtrusionCollision(bool bUse)
{
	m_bExtrusionCollision = bUse;
}

//=====================================================================================================================
// �����o��������s�����ǂ����擾
//=====================================================================================================================
const bool& CAttack::GetExtrusionCollision()
{
	return m_bExtrusionCollision;
}

//=====================================================================================================================
// ���������I�u�W�F�N�g�̃^�C�v�擾
//=====================================================================================================================
const CObject::TYPE& CAttack::GetCollisionObjType()
{
	return m_CollisionObjType;
}

//=====================================================================================================================
// �o�E���h���擾
//=====================================================================================================================
CAttack::BoundInfo& CAttack::GetBoundInfo()
{
	return m_BoundInfo;
}

//=====================================================================================================================
// 
// protected�����o 
//
//=====================================================================================================================


//=====================================================================================================================
// �U���^�C�v�ݒ�
//=====================================================================================================================
void CAttack::SetAttackType(ATTACKTYPE AttackType)
{
	m_Type = AttackType;
}

//=====================================================================================================================
// �U���^�C�v�擾
//=====================================================================================================================
const CAttack::ATTACKTYPE& CAttack::GetAttackType() const
{
	return m_Type;
}

//=====================================================================================================================
// ���肪�����������ǂ���
//=====================================================================================================================
const bool& CAttack::GetCollisionSuccess() const
{
	return m_bCollisionSuccess;
}

//====================================================================================================================
// 
// private�����o 
//
//====================================================================================================================

//=====================================================================================================================
//�����蔻�菈��
//=====================================================================================================================
void CAttack::Collision()
{
	// === �U���𑼂̃I�u�W�F�N�g�ɔ��肳���邽�߃��X�g������ ===

	for (int nCntPri = 0; nCntPri < m_nMAXPRIORITY; nCntPri++)
	{
		CObject* pObj = GetTopObject(nCntPri); // ���X�g�̐擪�I�u�W�F�N�g���擾
		while (pObj != nullptr)
		{
			// === �ϐ� ===

		    bool bNowCollision = false; // �����������ǂ�����������
			CObject* pNext = pObj->GetNextObject(); // ���̃I�u�W�F�N�g���擾

			// === ���� ===

			// �I�u�W�F�N�g���G�ŁA�U�����_���I�u�W�F�N�g���u�G�v�������ꍇ
			if (pObj->GetType() == CObject::TYPE::ENEMY && m_TargetType == TARGETTYPE::ENEMY)
			{
				// === �ϐ� ===

				CEnemy* pEnemy = static_cast<CEnemy*>(pObj);//�I�u�W�F�N�g��G�ɃL���X�g

				// === ���� ===

				CollisionProcess(m_bCollisionSuccess, bNowCollision, pEnemy); // �G�Ƃ̓����蔻����s��

				// �����蔻�肪��L�̏����Ő������Ă�����
				if (m_bCollisionSuccess == true)
				{
					m_CollisionObjType = CObject::TYPE::ENEMY; // ���������I�u�W�F�N�g�̃^�C�v��G�Ƃ���

					// �G�̗̑͂��Ȃ��Ȃ��Ă�����U�����|�����I�u�W�F�N�g�̃^�C�v���u�G�v�ɐݒ�
					if (pEnemy->GetLifeInfo().GetLife() < 1)
					{
						pEnemy->SetDefeatAttack(m_Type); // �|�����I�u�W�F�N�g�̃^�C�v���u�G�v�Ƃ���
					}
				}
			}
			// �I�u�W�F�N�g���u�v���C���[�v�ōU�����_���I�u�W�F�N�g���u�v���C���[�v�������ꍇ
			else if (pObj->GetType() == CObject::TYPE::PLAYER && m_TargetType == TARGETTYPE::PLAYER)
			{
				// === �ϐ� ===

				CObjectX* pObjX = static_cast<CObjectX*>(pObj);//�I�u�W�F�N�g���I�u�W�F�N�gX�ɃL���X�g

				// === ���� ===

				CollisionProcess(m_bCollisionSuccess, bNowCollision, pObjX); // �����蔻��̏������s��

				// ��L�̏����œ����蔻�肪�������Ă�����
				if (m_bCollisionSuccess == true)
				{
					m_CollisionObjType = CObject::TYPE::PLAYER;// ���������I�u�W�F�N�g�̃^�C�v���u�v���C���[�v�Ƃ���
				}
			}

			pObj = pNext;//���X�g�����ɐi�߂�
		}
	}

	// === �����ɂ���Ĕj�����鏈�� ===

	// �����蔻�肪�������A�����������ɃI�u�W�F�N�g�������t���O��true�Ȃ�A���S�t���O�𔭓�����
	if (m_bCollisionSuccess == true && GetCollisionRelease() == true)
	{
		SetDeath();
	}

}

//=====================================================================================================================
//���̃I�u�W�F�N�g�ɂ����Ă鏈��
//=====================================================================================================================
void CAttack::CollisionBuilding()
{
	// === �ϐ� ===
	CObjectX::PosInfo& SelfPosInfo = GetPosInfo();     // �ʒu���
	CObjectX::SizeInfo& SelfSizeInfo = GetSizeInfo();  // �T�C�Y���
	const D3DXVECTOR3& SelfPos = SelfPosInfo.GetPos(); // �ʒu
	const D3DXVECTOR3& SelfVtxMax = SelfSizeInfo.GetVtxMax(); // �ő咸�_
	const D3DXVECTOR3& SelfVtxMin = SelfSizeInfo.GetVtxMin(); // �ŏ����_


	// === �����n�I�u�W�F�N�g�����X�g���猟������ ===
	for (int nCntPri = 0; nCntPri < m_nMAXPRIORITY; nCntPri++)
	{
		// === �ϐ� ===
		CObject* pObj = GetTopObject(nCntPri);//�擪�I�u�W�F�N�g���擾����

		// === ���� ===

		// �I�u�W�F�N�g�����݂��Ă�����
		while (pObj != nullptr)
		{
			// === �ϐ� ===

			bool bNowCollision = false; // ���ݓ����������ǂ���
			CObject* pNext = pObj->GetNextObject(); // ���̃I�u�W�F�N�g
			CObjectX* pObjX = nullptr; // �I�u�W�F�N�gX�ւ̃|�C���^

			// === ���� ===

			// �I�u�W�F�N�g�̃^�C�v���u�w�i���f���v���u�u���b�N�v�ȊO�Ȃ珈�����X�L�b�v
			if (pObj->GetType() != CObject::TYPE::BGMODEL && pObj->GetType() != CObject::TYPE::BLOCK)
			{
				pObj = pNext; // ���X�g�����ɐi�߂�
				continue;
			}

			pObjX = static_cast<CObjectX*>(pObj); // �I�u�W�F�N�g���u�I�u�W�F�N�gX�v�ɃL���X�g

			// �I�u�W�F�N�gX�����݂��Ȃ��̂ŏ������X�L�b�v
			if (pObjX == nullptr)
			{
				pObj = pNext;
				continue;
			}

			// === �ϐ� ===

			CObjectX::PosInfo&
				OtherPosInfo = pObjX->GetPosInfo();      // �ʒu���
			CObjectX::SizeInfo& 
				OtherSizeInfo = pObjX->GetSizeInfo();   // �T�C�Y���

			const D3DXVECTOR3&
				OtherPos = OtherPosInfo.GetPos(),        // �ʒu
				OtherVtxMax = OtherSizeInfo.GetVtxMax(), // �ő咸�_
				OtherVtxMin = OtherSizeInfo.GetVtxMin(); // �ŏ����_

			// === ���� ===

			// �����`�̓����蔻����s��
			if (CCollision::CollisionSquare(SelfPos,SelfVtxMax,SelfVtxMin,OtherPos,OtherVtxMax,OtherVtxMin))
			{
				m_bCollisionSuccess = true; // �����蔻�肪���������t���O���I���ɂ���
				m_CollisionObjType = pObj->GetType(); // ���������I�u�W�F�N�g�̃^�C�v���i�[����
			}

			pObj = pNext; // ���X�g�����ɐi�߂�
		}
	}

	// === �I�u�W�F�N�g���������� ===

	// �����蔻�肪�������A�����������ɏ����t���O��true�ɂȂ��Ă����玀�S�t���O�𔭓�����
	if (m_bCollisionSuccess == true && m_bCollisionRelease == true)
	{
		SetDeath();
	}

}

//=====================================================================================================================
// ���̃I�u�W�F�N�g�Ƃ̉����o��������s������
//=====================================================================================================================
void CAttack::ExtrusionCollisionProcess()
{
	// === �ϐ� ===

	CObjectX::CollisionInfo& CollisionInfo = GetCollisionInfo(); // �����蔻����
	CObjectX::CollisionInfo::State& CollisionState = CollisionInfo.GetState(); // ������
	CollisionState.SetLandingOld(CollisionState.GetLanding()); // 1f�O�ɃI�u�W�F�N�g���n�ʂɏ���Ă��邩�ǂ�����ݒ�
	CollisionState.SetWallingOld(CollisionState.GetWalling()); // 1f�O�ɃI�u�W�F�N�g���ǂɂ��邩�ǂ�����ݒ�
	CollisionInfo.GetSquareInfo().ResetPushOutFirstFlag();     // ���ꂼ��̎��̉����o������̗D��t���O�����Z�b�g
	CollisionState.ResetState(); // �����ԃ��Z�b�g

	// === �����o��������s���I�u�W�F�N�g�����X�g���猟�����A�����o���D��x�����߂� ===

	// ���X�g����
	for (int nCntPri = 0; nCntPri < CObject::m_nMAXPRIORITY; nCntPri++)
	{
		// === �ϐ� ===

		CObject* pObj = CObject::GetTopObject(nCntPri);//���X�g�̐擪�I�u�W�F�N�g���擾����

		// === ���� ===

		// �I�u�W�F�N�g�����݂���Ȃ�J��Ԃ�
		while (pObj != nullptr)
		{
			// === �ϐ� ===

			CObject* pNext = pObj->GetNextObject(); // ���̃I�u�W�F�N�g
			CObject::TYPE type = pObj->GetType();   // ��ނ̎擾�i�G�Ȃ瓖���蔻��j

			// === ���� ===

			// �I�u�W�F�N�g�̎�ނ��u�u���b�N�v�Ɓu�w�i���f���v�ȊO�Ȃ珈�����X�L�b�v
			if (type != CObject::TYPE::BLOCK && type != CObject::TYPE::BGMODEL)
			{
				// �I�u�W�F�N�g�����ɐi�߂�
				pObj = pNext;
				continue;
			}

			// === �ϐ� ===

			CObjectX* pObjX = static_cast<CObjectX*>(pObj); // �I�u�W�F�N�gX�Ƀ_�E���L���X�g

			// === ���� ===

			// �I�u�W�F�N�gX�����݂��Ă��Ȃ��Ȃ珈�����X�L�b�v
			if (pObjX == nullptr)
			{
				// �I�u�W�F�N�g�����ɐi�߂�
				pObj = pNext;
				continue;
			}

			CCollision::ExtrusionCollisionSquarePushOutFirstDecide(this, pObjX); // �����`�̉����o������̂��ꂼ��̎��̏����̗D��x�����߂�
			
			// �I�u�W�F�N�g�����ɐi�߂�
			pObj = pNext;
		}
	}

	// === �����o��������s���I�u�W�F�N�g�����X�g���猟�����A�����o������J�n ===

	// ���X�g����
	for (int nCntPri = 0; nCntPri < CObject::m_nMAXPRIORITY; nCntPri++)
	{
		// === �ϐ� ===

		CObject* pObj = CObject::GetTopObject(nCntPri); //���X�g�̐擪�I�u�W�F�N�g

		// === ���� ===

		// �I�u�W�F�N�g�����݂���Ȃ�J��Ԃ�
		while (pObj != nullptr)
		{
			// === �ϐ� ===

			CObject* pNext = pObj->GetNextObject(); // ���̃I�u�W�F�N�g���i�[
			CObject::TYPE type = pObj->GetType();   // ��ނ̎擾

			// === ���� ===

			// �I�u�W�F�N�g�̎�ނ��u�u���b�N�v�Ɓu�w�i���f���v�ȊO�Ȃ珈�����X�L�b�v
			if (type != CObject::TYPE::BLOCK && type != CObject::TYPE::BGMODEL)
			{
				// �I�u�W�F�N�g�����ɐi�߂�
				pObj = pNext;
				continue;
			}

			// === �ϐ� ===

			CObjectX* pObjX = static_cast<CObjectX*>(pObj); // �I�u�W�F�N�gX�Ƀ_�E���L���X�g

			// === ���� ===

			// �I�u�W�F�N�gX�����݂��Ă��Ȃ��Ȃ珈�����X�L�b�v
			if (pObjX == nullptr)
			{
				// �I�u�W�F�N�g�����ɐi�߂�
				pObj = pNext;
				continue;
			}

			CCollision::ResolveExtrusionCollisionSquare(this, pObjX); // �����`�̉����o�����������

			pObj = pNext;//�I�u�W�F�N�g�����ɐi�߂�
		}
	}
}

//=====================================================================================================================
//�����蔻�苤�ʏ���
//=====================================================================================================================
void CAttack::CollisionProcess(bool& bCollision, bool& bNowCollision, CObjectX* pObjX)
{
	// === �ϐ� ===

	// ����
	CObjectX::PosInfo& SelfPosInfo = GetPosInfo();     // �����̈ʒu���
	CObjectX::SizeInfo& SelfSizeInfo = GetSizeInfo();  // �����̃T�C�Y���
	const D3DXVECTOR3& SelfPos = SelfPosInfo.GetPos(); // �����̈ʒu
	const D3DXVECTOR3& SelfVtxMax = SelfSizeInfo.GetVtxMax(); // �����̍ő咸�_
	const D3DXVECTOR3& SelfVtxMin = SelfSizeInfo.GetVtxMin(); // �����̍ŏ����_
	
	// ����
	CObjectX::PosInfo& OtherPosInfo = pObjX->GetPosInfo();     // ����̈ʒu���
	CObjectX::SizeInfo& OtherSizeInfo = pObjX->GetSizeInfo();  // ����̃T�C�Y���
	const D3DXVECTOR3& OtherPos = OtherPosInfo.GetPos(); // ����̈ʒu
	const D3DXVECTOR3& OtherVtxMax = OtherSizeInfo.GetVtxMax(); // ����̍ő咸�_
	const D3DXVECTOR3& OtherVtxMin = OtherSizeInfo.GetVtxMin(); // ����̍ŏ����_

	// === �����蔻��̎�ނɂ���ă|�C���^�Ƃ��ēn���ꂽ�I�u�W�F�N�g�Ƃ̔�����@��ύX���ď��� ===

	// �����ނɂ���ĕύX
	switch (GetCollisionType())
	{
		// �����`�̓����蔻��
	case CAttack::COLLISIONTYPE::SQUARE:
		if (CCollision::CollisionSquare(SelfPos, SelfVtxMax, SelfVtxMin, OtherPos, OtherVtxMax, OtherVtxMin))
		{
			bCollision = true;    // ��������
			bNowCollision = true; // ���ݓ�������
		}
		break;
		// XZ�����̎΂߂̓����蔻��
	case CAttack::COLLISIONTYPE::RECTANGLE_XZ:
		if (CCollision::RectAngleCollisionXZ(this, pObjX))
		{
			bCollision = true;    // ��������
			bNowCollision = true; // ���ݓ�������
		}
		break;
	case CAttack::COLLISIONTYPE::OBBTOOBB:
		if (CCollision::OBBToOBB(this, pObjX))
		{
			bCollision = true;
			bNowCollision = true;
		}
		break;
	default:
		break;
	}

	// ���������Ȃ�_���[�W��^����
	if (bNowCollision == true)
	{
		pObjX->SetDamage(GetPower(), m_HitStop.nSetTime); // �����ɐݒ肳��Ă���I�u�W�F�N�g�Ƀ_���[�W��^����
		pObjX->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), m_HitStop.nSetTime + 1, false, false,false); // �����ɐݒ肳��Ă���I�u�W�F�N�g�̐F������Ԃ�����
	}

}

//*********************************************************************************************************************
// �v���C���[�U���N���X
//*********************************************************************************************************************

//=====================================================================================================================
// 
// public�����o 
//
//=====================================================================================================================

//=====================================================================================================================
// �R���X�g���N�^
//=====================================================================================================================
CAttackPlayer::CAttackPlayer(int nPower, int nSetHitStopTime) :
	CAttack(nPower,nSetHitStopTime)
{

}

//=====================================================================================================================
// �f�X�g���N�^
//=====================================================================================================================
CAttackPlayer::~CAttackPlayer()
{

}

//=====================================================================================================================
// ����������
//=====================================================================================================================
HRESULT CAttackPlayer::Init()
{
	CAttack::Init(); // �U���̏���������
	return S_OK;
}

//=====================================================================================================================
// �I������
//=====================================================================================================================
void CAttackPlayer::Uninit()
{
	CAttack::Uninit(); // �U���̏I������
}

//=====================================================================================================================
// �X�V����
//=====================================================================================================================
void CAttackPlayer::Update()
{
	CAttack::Update(); // �U���̍X�V����

	BulletCollisionProcess();//�e�������������̏���
}

//=====================================================================================================================
// �`�揈��
//=====================================================================================================================
void CAttackPlayer::Draw()
{
	CAttack::Draw(); // �U���̕`�揈��
}

//=====================================================================================================================
// ���S�t���O�ݒ菈��
//=====================================================================================================================
void CAttackPlayer::SetDeath()
{
	CAttack::SetDeath(); // �U���̎��S�t���O�ݒ菈��
}

//=====================================================================================================================
// ��������
//=====================================================================================================================
CAttackPlayer* CAttackPlayer::Create(ATTACKTYPE AttackType, TARGETTYPE TargetType, COLLISIONTYPE CollisionType,
	bool bHitOtherThanLiving, bool bAutoCollision, int nPower, int nSetHitStopTime, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, D3DXVECTOR3 Scale)
{
	// === �ϐ� ===

	CAttackPlayer* pAttackPlayer = DBG_NEW CAttackPlayer(nPower, nSetHitStopTime); // �v���C���[�̍U���ւ̃|�C���^

	// === ���ݒ菈�� ===

	pAttackPlayer->Init(); //����������
	pAttackPlayer->SetTargetType(TargetType);       // �^�[�Q�b�g�^�C�v��ݒ�
	pAttackPlayer->SetCollisionType(CollisionType); // ����^�C�v��ݒ�
	pAttackPlayer->SetType(CObject::TYPE::ATTACK);  // �I�u�W�F�N�g���Ƃ̃^�C�v��ݒ肷��
	pAttackPlayer->SetAttackType(AttackType);       // �U���̎�ނ�ݒ肷��
	pAttackPlayer->GetLifeInfo().SetLife(nLife);    // �̗͂�ݒ�
	pAttackPlayer->GetLifeInfo().SetMaxLife(nLife); // �ő�̗͂�ݒ�
	pAttackPlayer->GetPosInfo().SetPos(pos);        // �ʒu  
	pAttackPlayer->GetPosInfo().SetSupportPos(pos); // �x�_�ʒu
	pAttackPlayer->GetRotInfo().SetRot(rot);        // ����
	pAttackPlayer->GetMoveInfo().SetMove(move);     // �ړ���
	pAttackPlayer->GetSizeInfo().SetScale(Scale);   // �g�嗦
	pAttackPlayer->GetLifeInfo().SetAutoSubLife(true); // �̗͂��g�p����
	pAttackPlayer->GetLifeInfo().SetAutoDeath(true);   // �̗͂��O�ɂȂ����Ƃ��Ɏ����I�Ɏ��S�t���O�𔭓����� 
	pAttackPlayer->GetMoveInfo().SetUseInteria(false, CObjectX::GetNormalInertia()); // �������g�p���Ȃ�
	pAttackPlayer->GetMoveInfo().SetUseGravity(false,1.0f);    // �d�͂��g�p���Ȃ�
	pAttackPlayer->SetHitOtherThanLibing(bHitOtherThanLiving); // �w�i�I�u�W�F�N�g�Ƃ������蔻����s�����ǂ���
	pAttackPlayer->SetAutoCollision(bAutoCollision); //�U���̓����蔻����U���N���X�ɔC���邩�ǂ���

	// ���f�����ݒ�
	int nIdx = CManager::GetObjectXInfo()->Regist(ATTACK_FILENAME[static_cast<int>(AttackType)]); // �w��p�X�̃C���f�b�N�X�擾

	// ��L�Ŏ擾�����C���f�b�N�X�̃��f���������蓖�Ă�
	pAttackPlayer->BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(nIdx),
		CManager::GetObjectXInfo()->GetBuffMat(nIdx),
		CManager::GetObjectXInfo()->GetdwNumMat(nIdx),
		CManager::GetObjectXInfo()->GetTexture(nIdx),
		CManager::GetObjectXInfo()->GetColorValue(nIdx));

	pAttackPlayer->SetSize(); // ���蓖�Ă�ꂽ���f��������ɃT�C�Y��ݒ肷��

	return pAttackPlayer;
}

//=====================================================================================================================
// 
// private�����o 
//
//=====================================================================================================================

//=====================================================================================================================
// �e�������������̏���
//=====================================================================================================================
void CAttackPlayer::BulletCollisionProcess()
{
	// �Q�[���V�[���Ȃ�
	if (CScene::GetMode() == CScene::MODE::MODE_GAME)
	{
		// �����蔻�肪�������Ă��āA�U���̎�ނ��u�e�v��������
		if (GetCollisionSuccess() == true && GetAttackType() == ATTACKTYPE::BULLET)
		{
			// === �ϐ� ===

			CObjectX::PosInfo& PosInfo = GetPosInfo(); // �ʒu�����擾
			const D3DXVECTOR3& Pos = PosInfo.GetPos(); // �ʒu

			// === ���� ===

			// �G�ɓ������Ă�����A�p�[�e�B�N�����Փˈʒu�ɏ�������
			if (GetCollisionObjType() == CObject::TYPE::ENEMY)
			{
				// === �ϐ� ===
				CPlayer* pPlayer = CGame::GetPlayer(); // �v���C���[�ւ̃|�C���^���擾
				CUi* pDiveGaugeFrameUi = pPlayer->GetDiveGaugeFrame(); // �_�C�u�Q�[�W�t���[���ւ̃|�C���^

				// �Q�[�W�t���[���̃R���|�W�b�g�p�^�[���̃R���e�i���擾����
				CUIComposite_Container* pDiveGaugeFrameUiCompositeContainer = pDiveGaugeFrameUi->GetUiCompositeContainer(); 

				// �Q�[�W�̋@�\���擾����
				CUIComposite_Gauge* pDiveGaugeUi_CompositeGauge = pDiveGaugeFrameUiCompositeContainer->GetChildren<CUIComposite_Gauge>();

				// === ���� ===

				// �_�C�u�Q�[�W�����݂��Ă��Ȃ������珈�����X�L�b�v
				if (pDiveGaugeUi_CompositeGauge == nullptr)
				{
					return;
				}

				// === �ϐ� ===

				CGauge* pDiveGauge = pDiveGaugeUi_CompositeGauge->GetGauge(); // �_�C�u�Q�[�W���擾����
				CUi* pDivePossibleNum = pPlayer->GetDivePossibleNum(); // �_�C�u�\�񐔂�UI���擾

				// �_�C�u�\�񐔂̃R���|�W�b�g�p�^�[���̃R���e�i���擾����
				CUIComposite_Container* pDivePossibleNumCompositeContainer = pDivePossibleNum->GetUiCompositeContainer();

				// �_�C�u�\�񐔂̐��l�\���@�\���擾����
				CUIComposite_Numeric* pDivePossibleNum_Numeric = pDivePossibleNumCompositeContainer->GetChildren<CUIComposite_Numeric>();

				// === ���� ===

				// �擾����UI�̋@�\�Ɂu�����v���Ȃ������珈�����X�L�b�v
				if (pDivePossibleNum_Numeric == nullptr)
				{
					return;
				}

				// �ő�_�C�u�\�񐔂ɒB���Ă��Ȃ��ꍇ�̓_�C�u�\�񐔂��{�P����
				if (pDivePossibleNum_Numeric->GetValue() < CPlayer::GetMaxDiveNum())
				{
					pDiveGauge->SetParam(pDiveGauge->GetParam() + 1);
				}
				
				// �Փˈʒu�Ƀp�[�e�B�N������������
				CParticle::SummonParticle(CParticle::TYPE::TYPE00_NORMAL, 20, 30, 30.0f, 30.0f, 100, 10, true, Pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), true);
			}
			// ����ȊO�Ȃ�
			else
			{
				// �Փˈʒu�Ƀp�[�e�B�N������������
				CParticle::SummonParticle(CParticle::TYPE::TYPE00_NORMAL, 20, 30, 30.0f, 30.0f, 100, 10, false, Pos, D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), true);
			}
		}
	}
}

//*********************************************************************************************************************
// �G�U���N���X
//*********************************************************************************************************************

//=====================================================================================================================
// 
// public�����o 
//
//=====================================================================================================================

//=====================================================================================================================
// �R���X�g���N�^
//=====================================================================================================================
CAttackEnemy::CAttackEnemy(int nPower, int nSetHitStopTime) : CAttack(nPower,nSetHitStopTime)
{

}

//=====================================================================================================================
// �f�X�g���N�^
//=====================================================================================================================
CAttackEnemy::~CAttackEnemy()
{

}

//=====================================================================================================================
// ����������
//=====================================================================================================================
HRESULT CAttackEnemy::Init()
{
	CAttack::Init(); // �U���̏���������
	return S_OK;
}

//=====================================================================================================================
//�I������
//=====================================================================================================================
void CAttackEnemy::Uninit()
{
	CAttack::Uninit(); //�U���̏I������
}

//=====================================================================================================================
// �X�V����
//=====================================================================================================================
void CAttackEnemy::Update()
{
	CAttack::Update(); // �U���̍X�V����
}

//=====================================================================================================================
// �`�揈��
//=====================================================================================================================
void CAttackEnemy::Draw()
{
	CAttack::Draw(); // �U���̕`�揈��
}

//=====================================================================================================================
// ���S�t���O�ݒ菈��
//=====================================================================================================================
void CAttackEnemy::SetDeath()
{
	CAttack::SetDeath(); // �U���̎��S�t���O�ݒ菈��
}

//=====================================================================================================================
// �G�̍U���𐶐�
//=====================================================================================================================
CAttackEnemy* CAttackEnemy::Create(ATTACKTYPE AttackType, TARGETTYPE TargetType, COLLISIONTYPE CollisionType,
	bool bHitOtherThanLiving, bool bAutoCollision, int nPower, int nSetHitStopTime, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, D3DXVECTOR3 Scale)
{
	// === �ϐ� ===

	CAttackEnemy* pAttackEnemy = DBG_NEW CAttackEnemy(nPower, nSetHitStopTime); // �G�ւ̃|�C���^

	// === ���� ===

	pAttackEnemy->Init(); // ����������
	pAttackEnemy->SetType(CObject::TYPE::ATTACK); // �I�u�W�F�N�g���Ƃ̃^�C�v��ݒ肷��
	pAttackEnemy->SetAttackType(AttackType); // �U���̎�ނ�ݒ肷��
	pAttackEnemy->SetTargetType(TargetType); // �^�[�Q�b�g�^�C�v��ݒ肷��
	pAttackEnemy->SetCollisionType(CollisionType); // ����^�C�v��ݒ肷��
	pAttackEnemy->GetLifeInfo().SetLife(nLife);    // �̗͂�ݒ�
	pAttackEnemy->GetLifeInfo().SetMaxLife(nLife); // �ő�̗͂�ݒ�
	pAttackEnemy->GetLifeInfo().SetAutoSubLife(true); // �̗͂������I�Ɍ��炷
	pAttackEnemy->GetPosInfo().SetPos(pos); // �ʒu  
	pAttackEnemy->GetRotInfo().SetRot(rot); // ����
	pAttackEnemy->GetMoveInfo().SetMove(move);   // �ړ���
	pAttackEnemy->GetSizeInfo().SetScale(Scale); // �g�嗦
	pAttackEnemy->GetMoveInfo().SetUseInteria(false, CObjectX::GetNormalInertia()); // �������g�p���Ȃ�
	pAttackEnemy->GetLifeInfo().SetAutoDeath(true); // �̗͂��Ȃ��Ȃ������Ɏ����I�Ɏ��S�t���O�𔭓�����
	pAttackEnemy->GetMoveInfo().SetUseGravity(false, 1.0f);   // �d�͂��g�p����
	pAttackEnemy->SetHitOtherThanLibing(bHitOtherThanLiving); // �w�i�I�u�W�F�N�g�Ƃ������蔻��𔭓����邩�ǂ���
	pAttackEnemy->SetAutoCollision(bAutoCollision); // �U���N���X�ɔ����C���邩�ǂ���

	// ���f���p�X����C���f�b�N�X���擾
	int nIdx = CManager::GetObjectXInfo()->Regist(ATTACK_FILENAME[static_cast<int>(AttackType)]);

	// ��L�Ŏ擾�����C���f�b�N�X���烂�f���������蓖�Ă�
	pAttackEnemy->BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(nIdx),
		CManager::GetObjectXInfo()->GetBuffMat(nIdx),
		CManager::GetObjectXInfo()->GetdwNumMat(nIdx),
		CManager::GetObjectXInfo()->GetTexture(nIdx),
		CManager::GetObjectXInfo()->GetColorValue(nIdx));

	pAttackEnemy->SetSize(); // ���蓖�Ă�ꂽ���f��������ɃI�u�W�F�N�g�̃T�C�Y��ݒ肷��
	return pAttackEnemy;
}

//*********************************************************************************************************************
// �o�E���h�\����
//*********************************************************************************************************************

//=====================================================================================================================
// 
// public�����o 
//
//=====================================================================================================================

//=====================================================================================================================
// �o�E���h����
//=====================================================================================================================
void CAttack::BoundInfo::BoundProcess(CAttack* pAttack)
{
	// �n�ʂɏ���Ă�����
	if (pAttack->GetCollisionInfo().GetState().GetLanding() == true)
	{
		// ���˂�����
		pAttack->GetMoveInfo().SetMove(D3DXVECTOR3(pAttack->GetMoveInfo().GetMove().x, Power.y, pAttack->GetMoveInfo().GetMove().z));

		// �d�͂�ON�̏ꍇ�A�U�����g�Ƀo�E���h���̏d�͂�ݒ�
		if (bGravity == true)
		{
			pAttack->GetMoveInfo().SetUseGravity(true, fGravity);
		}
	}
}
