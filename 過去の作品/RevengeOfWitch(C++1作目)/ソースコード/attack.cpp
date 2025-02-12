//=================================================
//
//�V���Q�T���F�G��AI������[attack.cpp]
//Author:ShinaTaiyo
//
//=================================================

//==============================
//�C���N���[�h
//==============================
#include "attack.h"
#include "manager.h"
#include "game.h"
#include "edit.h"
#include "bullet.h"
#include "particle.h"
#include "emitter.h"
#include "texture.h"
#include "manager.h"
#include "objectXInfo.h"
#include "input.h"
#include "sound.h"
#include "billboard.h"
#include "calculation.h"
#include "collision.h"
#include "player.h"
#include "camera.h"
#include "boss.h"
#include <math.h>
//=================================================
//==============================
//�ÓI�����o������
//==============================
int CAttack::m_nNumFile = CAttack::ATTACKTYPE_MAX;                      //�t�@�C���̐�
const char* CAttack::m_ATTACK_FILENAME[CAttack::ATTACKTYPE_MAX] =
{ "data\\MODEL\\Attack\\Attack00_Fire.x",
 "data\\MODEL\\Attack\\Attack01_Meteo.x",
 "data\\MODEL\\Attack\\Attack02_Needle.x",
 "data\\MODEL\\Attack\\Attack03_Tornado.x",
 "data\\MODEL\\Attack\\Attack04_Beam.x",
 "data\\MODEL\\Attack\\Attack05_Ice.x",
 "data\\MODEL\\Attack\\Attack06_Thunder.x",
 "data\\MODEL\\Attack\\Attack07_MagicSword.x",
 "data\\MODEL\\Attack\\Attack08_VacuumBlade_2.x",
 "data\\MODEL\\Attack\\Attack09_Hammer.x",
 "data\\MODEL\\Attack\\Attack10_ShockWave.x",
 "data\\MODEL\\Attack\\Attack11_Explosion.x",
 "data\\MODEL\\Attack\\Attack12_Vine.x",
 "data\\MODEL\\Attack\\Attack13_CrossBomb.x",
 "data\\MODEL\\Attack\\Attack14_HellFire.x",
 "data\\MODEL\\Attack\\Attack15_BlackHole.x",
 "data\\MODEL\\Attack\\Attack16_PhycoBurst.x",
 "data\\MODEL\\Attack\\Attack17_Block.x",
 "data\\MODEL\\Attack\\Attack18_Thornclub.x",
 "data\\MODEL\\Attack\\Attack19_Senter_Thunder.x",
};
//=================================================

//==============================
//�R���X�g���N�^
//==============================
CAttack::CAttack() : m_CollisionType(COLLISIONTYPE00_SQUARE),m_nSetEffectLife(0),m_SetEffectType(CEffect::EFFECTTYPE00_NORMAL),
m_TargetType(TARGETTYPE_ENEMY),m_bCollisionRelease(false),m_bDelaySetDeath(true),m_bDoCollision(false),m_bPatternFlag(false),
m_bUse(true),m_bUseAttackEffect(false),m_bUseGravity(false),m_bUseReflection(false),m_bUseSetEffect(false),m_bUseThinColor(false),m_fGravityPower(0.0f),m_nAttackCnt(0),
m_nCntTime(0),m_nPattern(0),m_nPatternTime(0),m_nPower(0),m_nSetAttackEffectLife(0),m_nSetHitStopTime(0),m_nSubType(0),m_type(ATTACKTYPE00_FIRE)
{

}
//=================================================

//==============================
//�f�X�g���N�^
//==============================
CAttack::~CAttack()
{

}
//=================================================

//==============================
//����������
//==============================
HRESULT CAttack::Init()
{
	//===========================
	//�QD�I�u�W�F�N�g������
	//===========================
	if (FAILED(CObjectX::Init()))
	{
		return E_FAIL;
	}
	//====================================

	////===========================
	////������
	////===========================
	//m_bUse = true;                            //�g�p���
	//m_CollisionType = COLLISIONTYPE00_SQUARE; //�����蔻��̎��
	//m_nPower = 0;                             //�_���[�W��
	//m_nCntTime = 0;                           //��������Ă���̎��Ԃ��v������
	//m_bCollisionRelease = false;              //���������Ƃ��ɏ������ǂ���
	//m_nAttackCnt = 0;                         //�U���񐔂��J�E���g����
	//m_nSetHitStopTime = 0;                    //���������Ƃ��Ɋ��蓖�Ă�q�b�g�X�g�b�v����
	//m_bDoCollision = false;                   //�����蔻������邩�ǂ���
	//m_bCollisionPossible = true;              //�����蔻�肪�\���ǂ���

	//m_bUseThinColor = false;                  //�F�𔖂����Ă������ǂ���

	//m_bDelaySetDeath = true;                 //���S�t���O������x�����邩�ǂ���

	//m_PurposePos = NULL_VECTOR3;              //�ړI�n����p

	////====================================
	////���ˊ֌W
	////====================================
	//m_bUseReflection = false;
	////===============================================================================

	////====================================
	////�G�t�F�N�g�֌W
	////====================================
	//m_bUseSetEffect = false;//�G�t�F�N�g���g�p���邩�ǂ���
	//m_nSetEffectLife = 0;//�ݒ肷��G�t�F�N�g�̗̑�
	//m_SetEffectSize = D3DXVECTOR2(0.0f, 0.0f);//�ݒ肷��G�t�F�N�g�̃T�C�Y
	//m_SetEffectType = (CEffect::EFFECTTYPE)(0);//�ݒ肷��G�t�F�N�g�̎��
	//m_SetEffectColor = NORMAL_COL * 0.0f;//�ݒ肷��G�t�F�N�g�̐F����
	////===============================================================================

	////====================================
 //   //�U���G�t�F�N�g�֌W
	////====================================
	//m_bUseAttackEffect = false;//�U���G�t�F�N�g���g�p���邩�ǂ���
	//m_nSetAttackEffectLife = 0;//�ݒ肷��U���G�t�F�N�g�̗̑�
	//m_SetAttackEffectCol = NORMAL_COL;//�ݒ肷��U���G�t�F�N�g�̐F����
	////===============================================================================

	////====================================
	////�p�^�[���֌W
	////====================================
	//m_nPattern = 0;                     //�U���̃p�^�[��
	//m_nPatternTime = 0;                 //�U���p�^�[������
	//m_bPatternFlag = false;             //�p�^�[���̃t���O
	////====================================

	if (GetUseInitialSound() == true)
	{
		switch (m_type)
		{
		case ATTACKTYPE00_FIRE:
			CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_FIRE);
			break;
		case ATTACKTYPE04_BEAM:
			CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PUNCH);
			break;
		case ATTACKTYPE06_THUNDER:
			CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_THUNDER);
			break;
		case ATTACKTYPE12_VINE:
			CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_VINE);
			break;
		case ATTACKTYPE13_CROSSBOMB:
			CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_FIRE_002);
			break;
		case ATTACKTYPE16_PHYCOBURST:
			CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PHYCOBURST_000);
			break;
		default:
			break;
		}
	}
	//====================================
	//�p�^�[���֌W
	//====================================
	m_nSubType = 0;//�����������߂�Ƃ��ȂǂɎg��
	//===========================================================
	return S_OK;
}
//=================================================

//==============================
//�I������
//==============================
void CAttack::Uninit()
{
	CObjectX::Uninit();
	//SetDeath();
	////Release();//�I�u�W�F�N�g�j��
}
//=================================================

//==============================
//�X�V����
//==============================
void CAttack::Update()
{
	m_nCntTime++;                                                 //��������Ă���̎��Ԃ��v������
	m_nPatternTime++;                                             //���ꂼ��̓����̎��Ԃ��v������
	//==================================
	//�I�u�W�F�N�gX��������擾
	//==================================
	D3DXVECTOR3 PosOld = GetPosOld();                             //1f�O�̈ʒu��ݒ�
	D3DXVECTOR3& Move = GetMove();                                //�ړ��ʂ�ݒ�
	D3DXVECTOR3 Size = GetSize();                                 //�T�C�Y���擾
	D3DXVECTOR3& Scale = GetScale();                               //�g�嗦���擾
	D3DXVECTOR3 & Pos = CObjectX::GetPos();                         //�ʒu���擾
	D3DXVECTOR3 & Rot = GetRot();                                   //�������擾
	D3DXVECTOR3 ParticlePos = CObjectX::GetPos();
	int& nLife = GetLife();                                       //�̗͂��擾
	int nMaxLife = GetMaxLife();                                  //�ő�̗͂��擾
	float fVXaim = 0.0f;
	float fVYaim = 0.0f;
	float fVLaim = 0.0f;
	float fRatioLife = 0.0f;
	float fRot = (float)(rand() % 60 - 30) / 100;
	float fRandAddSpeed = (float)(rand() % 1000) / 1000;
	bool bCollision = false;//�������Ă��邩�ǂ���
	D3DXVECTOR3 RandMove = NULL_VECTOR3;                          //�����ړ���   
	CParticle* pParticle = nullptr;                               //�p�[�e�B�N���擾�p  
	//===========================================================

	//==================================
	//�G�t�F�N�g���I���ɂȂ��Ă�����
	//==================================
	if (m_bUseSetEffect == true)
	{
		CEffect::Create(m_SetEffectType, m_nSetEffectLife, m_SetEffectSize.x, m_SetEffectSize.y, Pos, m_SetEffectColor);
	}

	//============================================
	//�F�𔖂����Ă����t���O��ON�ɂȂ��Ă�����
	//============================================
	if (m_bUseThinColor == true)
	{
		fRatioLife = (float)(nLife) / (float)(nMaxLife);
		SetColorAlphaOnly(fRatioLife);//�F�̃A���t�@�l��ݒ�
	}
	//==========================================================

	//==================================
	//�d�͂�ON�ɂȂ��Ă�����
	//==================================
	if (m_bUseGravity == true)
	{
		Move.y += m_fGravityPower;
	}
	//==========================================================

	//==================================
	//X�I�u�W�F�N�g�X�V
	//==================================
	CObjectX::Update();
	//===========================================================

	//==================================
	//�U���G�t�F�N�g�֌W
	//==================================
	if (m_bUseAttackEffect == true)
	{
		if (GetUseParent() == false)
		{
			CAttackEffect::Create(m_type, 10, m_nSetAttackEffectLife, Pos, NULL_VECTOR3, Scale, Rot, NULL_VECTOR3, m_SetAttackEffectCol);
		}
		else
		{
			CAttackEffect::Create(m_type, 10, m_nSetAttackEffectLife,GetOffSetPos(), NULL_VECTOR3, Scale, GetParentRot(), NULL_VECTOR3, m_SetAttackEffectCol);
		}
	}

	//==================================
	//���ˊ֌W
	//==================================
	if (m_bUseReflection == true)
	{
		if ((GetSuccessBlockCollisionX() == true))
		{//X�x�N�g���𔽓]������
			Move.x *= -1.0f;
		}
		if (GetSuccessBlockCollisionY() == true || GetPos().y + GetVtxMin().y < 0.0f || GetPos().y + GetVtxMax().y > 400.0f)
		{//Y�x�N�g���𔽓]������
			Move.y *= -1.0f;
		}
	}
	//===========================================================
	
	//==================================
	//�����蔻����s��
	//==================================
	if (CScene::GetMode() == CScene::MODE_GAME)
	{
		if (m_bDoCollision == true)
		{
			if (GetCntTime() > 1)
			{
				switch (m_CollisionType)
				{
				case COLLISIONTYPE00_SQUARE:
					Collision();
					break;
				case COLLISIONTYPE01_RECTANGLEXY:
					RectAngleCollisionXY();
					break;
				case COLLISIONTYPE02_TRUERECTANGLEXY:
					TrueRectAngleCollisionXY();
					break;
				case COLLISIONTYPE03_BALL:
					BallCollision();
					break;
				default:
					break;
				}
			}
		}
	}
	//===========================================================

	//���S����
	if (nLife < 1)
	{
		m_bUse = false;
	}


	//==================================
	//�U���̔j��
	//==================================
	if (m_bDelaySetDeath == false)
	{
		if (m_bUse == false)
		{
			switch (m_type)
			{
			case ATTACKTYPE05_ICE:
				for (int nCnt = 0; nCnt < 30; nCnt++)
				{
					RandMove.x = float(rand() % 150 - 75) / 5;
					RandMove.y = float(rand() % 150 - 75) / 5;
					RandMove.z = float(rand() % 150 - 75) / 5;
					pParticle = CParticle::Create(CParticle::TYPE02_ICE, 50, 80.0f, 80.0f, Pos, RandMove, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), true);
					pParticle->SetGravity(-0.4f);
					pParticle = nullptr;
				}
				CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_ICE000);
				break;
			case ATTACKTYPE13_CROSSBOMB:
				CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_EXPLOSION_000);
				break;
			default:
				break;
			}

			SetDeath();//���S�t���O��ݒ�
			//Uninit();//���S�t���O��ݒ�
		}
	}
	if (m_bDelaySetDeath == true && m_bUse == false)
	{//���S�t���O��x�点��t���O�������ɃI�t�ɂ���i����ɂ��Atrue�̃I�u�W�F�N�g��1f�x�点�邱�Ƃ��ł��ACollision�ɁAdeath�����������ꍇ�̏����������Ȃ��ėǂ��B���e�͈́B�j
		m_bDelaySetDeath = false;
	}
	//===========================================================
}
//=================================================

//==============================
//�`�揈��
//==============================
void CAttack::Draw()
{
	CObjectX::Draw();
}
//=================================================

//==========================================================================
//���S�t���O��ݒ�
//==========================================================================
void CAttack::SetDeath()
{
	CObject::SetDeath();
}
//==========================================================================================


//==========================================================================
//�d�͂�ݒ�
//==========================================================================
void CAttack::SetUseGravity(float fGravityPower)
{
	m_bUseGravity = true;
	m_fGravityPower = fGravityPower;
}
//===================================================================================================================================================

//==========================================================================
//��������
//==========================================================================
CAttack* CAttack::Create(ATTACKTYPE type, int nDamage, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 Scale, TARGETTYPE TargetType, D3DXVECTOR3 Rot,
	COLLISIONTYPE CollisionType, D3DXVECTOR3 AddRot, int nHitStopTime)
{
	CAttack* pAttack = DBG_NEW CAttack;   //�G�𐶐�
	bool bSuccess = pAttack->CObject::GetCreateSuccess();
	int nIdx = 0;//���f���̃C���f�b�N�X�擾�p
	if (bSuccess == true)
	{
		if (pAttack != nullptr)
		{
			pAttack->m_type = type;  //�e�̎��
			pAttack->Init();         //����������
			pAttack->SetHitStopTime(nHitStopTime);//���������Ƃ��̃q�b�g�X�g�b�v���Ԃ�ݒ肷��
			pAttack->SetUseBlockCollision(false);
			pAttack->SetUseInteria(false);//�������g�p���Ȃ�
			pAttack->m_bUse = true;  //�g�p���
			pAttack->SetMove(move);  //�ړ��ʂ̐ݒ�
			pAttack->SetLife(nLife); //�̗͂�ݒ�
			pAttack->SetMaxLife(nLife);//�ő�̗͂�ݒ�
			pAttack->SetAutoSubLife(true);              //�����I�ɑ̗͂����炷���ǂ���
			pAttack->m_TargetType = TargetType;//�N�ɓ��Ă邩��ݒ�
			pAttack->m_CollisionType = CollisionType;//�����蔻��̎��
			pAttack->m_nPower = nDamage;               //�_���[�W��
			pAttack->SetUseAddRot(true,AddRot);                 //���Z��������̗�
			pAttack->SetDoCollision(true);              //�����蔻������邩�ǂ�����ݒ�
			pAttack->CObjectX::SetPos(pos);             //�I�u�W�F�N�gX�̈ʒu��ݒ�
			pAttack->SetRot(Rot);                       //������ݒ�
			pAttack->SetPosOld(pos);                                                                                         //1f�O�̈ʒu��ݒ�
			pAttack->SetSupportPos(pos);           
			pAttack->CObject::SetType(CObject::TYPE_ATTACK);                                                                  //�I�u�W�F�N�g�̎�ނ����߂�
			pAttack->SetScale(Scale);                                                                              //�I�u�W�F�N�g�̊g�嗦
			pAttack->SetFormarScale(Scale);                                       //���̊g�嗦��ݒ肷��
			pAttack->SetUseDeath(true);//���S�t���O�𔭓����邩�ǂ�����ݒ肷��
			nIdx = CManager::GetObjectXInfo()->Regist(CObjectX::OBJECTXTYPE_ATTACK, m_ATTACK_FILENAME[(int)(type)]);
			pAttack->CObjectX::BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(CObjectX::OBJECTXTYPE_ATTACK,nIdx),
				CManager::GetObjectXInfo()->GetBuffMat(CObjectX::OBJECTXTYPE_ATTACK, nIdx),
				CManager::GetObjectXInfo()->GetdwNumMat(CObjectX::OBJECTXTYPE_ATTACK, nIdx),
				CManager::GetObjectXInfo()->GetTexture(CObjectX::OBJECTXTYPE_ATTACK, nIdx),
				CManager::GetObjectXInfo()->GetColorValue(CObjectX::OBJECTXTYPE_ATTACK, nIdx));                       //���f���������蓖�Ă�

			pAttack->CObjectX::SetObjXType(CObjectX::OBJECTXTYPE_ATTACK);                            //�I�u�W�F�N�gX�̃^�C�v��ݒ�
			pAttack->CObjectX::SetTypeNum((int)(type));                                             //�I�u�W�F�N�gX���Ƃ̃^�C�v�ԍ���ݒ�
			pAttack->SetSize();                                                                     //X�I�u�W�F�N�g�̃T�C�Y��ݒ肷��
			pAttack->CObjectX::SetRotType(CObjectX::ROTTYPE::ROTTYPE_NORMAL);                       //�����̎�ނ�ݒ�
		}
		else
		{//�I�u�W�F�N�g�ɋ󂫂��Ȃ������̂Ŕj������
			delete pAttack;
			pAttack = nullptr;
			return nullptr;
		}
	}
	return pAttack;
}
//===================================================================================================================================================

//==========================================================================
//�F�𔖂����Ă������ǂ��������߂�
//==========================================================================
void CAttack::SetUseThinColor(bool bUse)
{
	m_bUseThinColor = bUse;
}
//===================================================================================================================================================

//=====================================================================
//�����蔻������
//=====================================================================
void CAttack::Collision()
{
	//==================================
	//�I�u�W�F�N�gX��������擾
	//==================================
	D3DXVECTOR3 Pos = GetPos();                      //�I�u�W�F�N�g�̈ʒu���擾
	D3DXVECTOR3 Size = GetSize();                    //�I�u�W�F�N�g�̃T�C�Y���擾
	D3DXVECTOR3 VtxMax = GetVtxMax();
	D3DXVECTOR3 VtxMin = GetVtxMin();
	D3DXVECTOR3 ComparisonPos = NULL_VECTOR3;                  //�v���C���[�̈ʒu����p
	D3DXVECTOR3 ComparisonVtxMax = NULL_VECTOR3;               //����Ώۂ̍ő咸�_
	D3DXVECTOR3 ComparisonVtxMin = NULL_VECTOR3;               //����Ώۂ̍ŏ����_
	CPlayer* pPlayer = nullptr;                                //�v���C���[�N���X�擾�p
	CEnemy* pEnemy = nullptr;                                  //�G�N���X�擾�p 
	CBoss* pBoss = nullptr;                                    //�{�X�擾�p
	CObject* pObj = nullptr;
	//===========================================================

	float fSpeedX = 0.0f;
	float fSpeedY = 0.0f;
	float fSpeedZ = 0.0f;

	//==================================
	//�U���̓����蔻��
	//==================================
	if (m_TargetType == TARGETTYPE_PLAYER)
	{//�^�[�Q�b�g���v���C���[�Ȃ�
		ComparisonPos = CGame::GetPlayer()->GetPos();                   //�ʒu���擾
		ComparisonVtxMax = CGame::GetPlayer()->GetVtxMax();             //�ő咸�_���擾
		ComparisonVtxMin = CGame::GetPlayer()->GetVtxMin();             //�ŏ����_���擾
		if (Pos.x + VtxMin.x / 2 <= ComparisonPos.x + ComparisonVtxMax.x &&
			Pos.x + VtxMax.x / 2 >= ComparisonPos.x + ComparisonVtxMin.x &&
			Pos.y + VtxMin.y <= ComparisonPos.y + ComparisonVtxMax.y &&
			Pos.y + VtxMax.y >= ComparisonPos.y + ComparisonVtxMin.y &&
			Pos.z + VtxMin.z / 2 <= ComparisonPos.z + ComparisonVtxMax.x &&
			Pos.z + VtxMax.z / 2 >= ComparisonPos.z + ComparisonVtxMin.x)
		{//�U���ɓ���������
			CGame::GetPlayer()->SetDamage(m_nPower,1);
			if (m_bCollisionRelease == true)
			{
				m_bUse = false;//�g�p��Ԃ��I�t�ɂ���
			}
		}
	}
	else if (m_TargetType == TARGETTYPE_ENEMY)
	{//�^�[�Q�b�g���G�Ȃ�
		for (int nCntPriority = 0; nCntPriority < m_nMAXPRIORITY; nCntPriority++)
		{

			pObj = GetTopObject(nCntPriority);//�g�b�v�I�u�W�F�N�g���擾
			while (pObj != nullptr)
			{
				//���̃I�u�W�F�N�g���i�[
				CObject* pNext = pObj->GetNextObject();

				//��ނ̎擾�i�G�Ȃ瓖���蔻��j
				CObject::TYPE type = pObj->GetType();
				switch (type)
				{
				case CObject::TYPE_ENEMY:
					pEnemy = (CEnemy*)pObj;                             //�I�u�W�F�N�g��G�̌^�Ƀ_�E���L���X�g
					ComparisonPos = pEnemy->GetPos();                   //�ʒu���擾
					ComparisonVtxMax = pEnemy->GetVtxMax();             //�ő咸�_���擾
					ComparisonVtxMin = pEnemy->GetVtxMin();             //�ŏ����_���擾
					break;
				case CObject::TYPE_BOSS:
					pBoss = (CBoss*)pObj;                               //�I�u�W�F�N�g���{�X�̌^�Ƀ_�E���L���X�g
					ComparisonPos = pBoss->GetPos();                   //�ʒu���擾
					ComparisonVtxMax = pBoss->GetVtxMax();             //�ő咸�_���擾
					ComparisonVtxMin = pBoss->GetVtxMin();             //�ŏ����_���擾
					break;
				default:
					pObj = pNext;
					continue;
					break;
				}

				if (Pos.x + VtxMin.x / 2 <= ComparisonPos.x + ComparisonVtxMax.x &&
					Pos.x + VtxMax.x / 2 >= ComparisonPos.x + ComparisonVtxMin.x &&
					Pos.y + VtxMin.y <= ComparisonPos.y + ComparisonVtxMax.y &&
					Pos.y + VtxMax.y >= ComparisonPos.y + ComparisonVtxMin.y &&
					Pos.z + VtxMin.z / 2 <= ComparisonPos.z + ComparisonVtxMax.x &&
					Pos.z + VtxMax.z / 2 >= ComparisonPos.z + ComparisonVtxMin.x)
				{//�U���ɓ���������
					switch (type)
					{
					case CObject::TYPE_ENEMY:
						if (pEnemy->GetHitStop() == false)
						{
							pEnemy->SetDamage(m_nPower,m_nSetHitStopTime);//�_���[�W��^����
							if (m_bCollisionRelease == true)
							{
								m_bUse = false;//�g�p��Ԃ��I�t�ɂ���
							}
						}
						break;
					case CObject::TYPE_BOSS:
						if (pBoss->GetHitStop() == false)
						{
							pBoss->SetDamage(m_nPower,m_nSetHitStopTime);
							if (m_bCollisionRelease == true)
							{
								m_bUse = false;//�g�p��Ԃ��I�t�ɂ���
							}
						}
						break;
					default:
						continue;
						break;
					}
				}
				//���X�g�����ɐi�߂�
				pObj = pNext;
			}
		}
	}

}
//===================================================================================================================================================

//======================================================================
//XY�����̎΂߂��܂߂�������s��
//======================================================================
void CAttack::RectAngleCollisionXY()
{
	D3DXVECTOR3 FourVtxPos[4] = {};   //�S���_�̈ʒu
	D3DXVECTOR3 FourVtxRotPos[4] = {};//��]�����S���_�̈ʒu
	
	D3DXVECTOR3 Pos = GetPos();//�ʒu
	D3DXVECTOR3 ComparisonPos = NULL_VECTOR3;//��r�Ώۂ̈ʒu
	D3DXVECTOR3 ComparisonSize = NULL_VECTOR3;//��r�Ώۂ̃T�C�Y
	D3DXVECTOR3 ComparisonVtxMax = NULL_VECTOR3;
	D3DXVECTOR3 ComparisonVtxMin = NULL_VECTOR3;
	D3DXVECTOR3 Rot = GetRot();//����
	D3DXVECTOR3 VtxMax = GetVtxMax();//�ő咸�_
	D3DXVECTOR3 VtxMin = GetVtxMin();//�ŏ����_
	float fSpeedX = 0.0f;
	float fSpeedY = 0.0f;
	float fSpeedZ = 0.0f;

	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		ComparisonPos = CGame::GetPlayer()->GetPos();
		ComparisonSize = CGame::GetPlayer()->GetSize();
		ComparisonVtxMax = CGame::GetPlayer()->GetVtxMax();
		ComparisonVtxMin = CGame::GetPlayer()->GetVtxMin();
		switch (nCnt)
		{
		case 0://����
			ComparisonPos.x += ComparisonVtxMin.x;
			ComparisonPos.y += ComparisonVtxMax.y;
			break;
		case 1://�E��
			ComparisonPos.x += ComparisonVtxMax.x;
			ComparisonPos.y += ComparisonVtxMax.y;
			break;
		case 2://����
			ComparisonPos.x += ComparisonVtxMin.x;
			break;
		case 3://�E��
			ComparisonPos.x += ComparisonVtxMax.x;
			break;
		default:
			break;
		}

		//=================================
		//�v�Z�Ɏg���S���_�̈ʒu
		//=================================
		if (CScene::GetMode() == CScene::MODE_GAME)
		{
			//���㒸�_
			FourVtxPos[0] = D3DXVECTOR3(Pos.x + VtxMin.x, Pos.y + VtxMax.y, 0.0f);

			//�E�㒸�_
			FourVtxPos[1] = D3DXVECTOR3(Pos.x + VtxMax.x, Pos.y + VtxMax.y, 0.0f);

			//�������_
			FourVtxPos[2] = D3DXVECTOR3(Pos.x + VtxMin.x, Pos.y + VtxMin.y, 0.0f);

			//�E�����_
			FourVtxPos[3] = D3DXVECTOR3(Pos.x + VtxMax.x, Pos.y + VtxMin.y, 0.0f);

			//=============================================================================================================================================================================

			//==================================
			//���S�_�ƂS���_�̋��������߂�
			//==================================
			float fLength[4] = {};
			for (int nCnVtx = 0; nCnVtx < 4; nCnVtx++)
			{
				fLength[nCnVtx] = sqrtf(powf((FourVtxPos[nCnVtx].x - Pos.x), 2) + powf((FourVtxPos[nCnVtx].y - Pos.y), 2));
			}
			//=============================================================================================================================================================================

			//==================================
			//���S�_����S���_�̊p�x�����߂�
			//==================================
			float fVaim[4] = {};
			for (int nCntVtx = 0; nCntVtx < 4; nCntVtx++)
			{
				fVaim[nCntVtx] = atan2f(FourVtxPos[nCntVtx].x - Pos.x, FourVtxPos[nCntVtx].y - Pos.y);
			}
			//=============================================================================================================================================================================

			//==================================
			//��]���_�̈ʒu�����߂�
			//==================================
			for (int nCntVtx = 0; nCntVtx < 4; nCntVtx++)
			{
				//��������킹�邽�߁ARot��-��t����
				FourVtxRotPos[nCntVtx] = D3DXVECTOR3(sinf(-Rot.z + fVaim[nCntVtx]) * fLength[nCntVtx] + Pos.x, cosf(-Rot.z + fVaim[nCntVtx]) * fLength[nCntVtx] + Pos.y, 0.0f);
			}
			//=============================================================================================================================================================================

			//===============================================================
			//��`�̊e�ӂ̎n�_�ƏI�_�̃x�N�g����p�ӂ���i�x�N�g��A�j
			//===============================================================
			float fVecAX[4] = {};
			float fVecAY[4] = {};

			fVecAX[0] = FourVtxRotPos[1].x - FourVtxRotPos[0].x;
			fVecAX[1] = FourVtxRotPos[3].x - FourVtxRotPos[1].x;
			fVecAX[2] = FourVtxRotPos[2].x - FourVtxRotPos[3].x;
			fVecAX[3] = FourVtxRotPos[0].x - FourVtxRotPos[2].x;

			fVecAY[0] = FourVtxRotPos[1].y - FourVtxRotPos[0].y;
			fVecAY[1] = FourVtxRotPos[3].y - FourVtxRotPos[1].y;
			fVecAY[2] = FourVtxRotPos[2].y - FourVtxRotPos[3].y;
			fVecAY[3] = FourVtxRotPos[0].y - FourVtxRotPos[2].y;

			float fCheckAim[4] = {};
			for (int nCntVtx = 0; nCntVtx < 4; nCntVtx++)
			{
				fCheckAim[nCntVtx] = atan2f(fVecAX[nCntVtx], fVecAY[nCntVtx]);
			}
			//=============================================================================================================================================================================

			//=============================================================================================
			//�^�[�Q�b�g�ƂȂ�_�i�v���C���[�A�G�l�~�[�Ȃǁj�ƂS���_�̃x�N�g����p�ӂ���i�x�N�g��B�j
			//=============================================================================================
			float fVecBX[4] = {};
			float fVecBY[4] = {};


			fVecBX[0] = ComparisonPos.x - FourVtxRotPos[0].x;
			fVecBX[1] = ComparisonPos.x - FourVtxRotPos[1].x;
			fVecBX[2] = ComparisonPos.x - FourVtxRotPos[3].x;
			fVecBX[3] = ComparisonPos.x - FourVtxRotPos[2].x;

			fVecBY[0] = ComparisonPos.y - FourVtxRotPos[0].y;
			fVecBY[1] = ComparisonPos.y - FourVtxRotPos[1].y;
			fVecBY[2] = ComparisonPos.y - FourVtxRotPos[3].y;
			fVecBY[3] = ComparisonPos.y - FourVtxRotPos[2].y;

			float fCheckAim2[4] = {};

			for (int nCntVtx = 0; nCntVtx < 4; nCntVtx++)
			{
				fCheckAim2[nCntVtx] = atan2f(fVecBX[nCntVtx], fVecBY[nCntVtx]);
			}

			//========================================================
			//���ō�����x�N�g���̊O�ς��v�Z����
			//========================================================
			float fCross[4] = {};
			for (int nCntVtx = 0; nCntVtx < 4; nCntVtx++)
			{
				fCross[nCntVtx] = (fVecAX[nCntVtx] * fVecBY[nCntVtx]) - (fVecBX[nCntVtx] * fVecAY[nCntVtx]);
			}

			//========================================================
			//�㉺���������
			//========================================================
			int nCntCheck = 0;//���肪���������Ă��邩�m�F����

			for (int nCntVtx = 0; nCntVtx < 4; nCntVtx++)
			{
				if (fCross[nCntVtx] <= 0.0f)
				{//�l�����Ȃ��`�̓����ɂ���B
					nCntCheck++;
				}
			}

			if (nCntCheck >= 4)
			{//�S�Ă̊O�ς̒l�����̒l�i��`�̒��ɂ���
				if (ComparisonPos.z + ComparisonSize.z / 2 >= Pos.z + VtxMin.z &&
					ComparisonPos.z - ComparisonSize.z / 2 <= Pos.z + VtxMax.z && m_TargetType == TARGETTYPE_PLAYER)
				{//��`���ɓ������Ƃ��̃v���C���[�ƃ��f���̒��S�_�Ƃ̋����������o��
					if (m_bCollisionRelease == true)
					{
						m_bUse = false;//�g�p��Ԃ��I�t�ɂ���
					}
					CGame::GetPlayer()->SetDamage(m_nPower,m_nSetHitStopTime);
					for (int nCnt = 0; nCnt < 20; nCnt++)
					{
						fSpeedX = float(rand() % 200 - 100) / 10;
						fSpeedY = float(rand() % 200 - 100) / 10;
						fSpeedZ = float(rand() % 200 - 100) / 10;
						CParticle::Create(CParticle::TYPE01_FIRE, 30, 20.0f, 20.0f, CGame::GetPlayer()->GetPos(), D3DXVECTOR3(fSpeedX, fSpeedY, fSpeedZ), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), true);
					}
					break;
				}
			}
		}
	}
}

//======================================================================
//������XY�����̉�]��`���m�̓����蔻��
//======================================================================
void CAttack::TrueRectAngleCollisionXY()
{

	//�I�u�W�F�N�g
	D3DXVECTOR3 Pos = GetPos();//�ʒu
	D3DXVECTOR3 Rot = GetRot();//����
	D3DXVECTOR3 VtxMax = GetVtxMax();//�ő咸�_
	D3DXVECTOR3 VtxMin = GetVtxMin();//�ŏ����_
	if (GetUseParent() == true)
	{//�e��������
		Pos = GetOffSetPos();//�I�t�Z�b�g�̈ʒu���擾

		Rot = GetParentRot() + GetRot();

		float fSpeedX = float(rand() % 200 - 100) / 10;
		float fSpeedY = float(rand() % 200 - 100) / 10;
		float fSpeedZ = float(rand() % 200 - 100) / 10;
		CParticle::Create(CParticle::TYPE01_FIRE, 30, 20.0f, 20.0f, Pos, D3DXVECTOR3(fSpeedX, fSpeedY, fSpeedZ), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), true);

	}

	D3DXVECTOR2 FourVtxPos[4] = {};   //�S���_�̈ʒu
	D3DXVECTOR2 FourVtxRotPos[4] = {};//��]�����S���_�̈ʒu
	D3DXVECTOR3 ComparisonPos = NULL_VECTOR3;//��r�Ώۂ̈ʒu
	D3DXVECTOR2 FourComparisonVtxPos[4] = {};    //��r�p�S���_
	D3DXVECTOR3 ComparisonSize = NULL_VECTOR3;//��r�Ώۂ̃T�C�Y
	D3DXVECTOR3 ComparisonVtxMax = NULL_VECTOR3;
	D3DXVECTOR3 ComparisonVtxMin = NULL_VECTOR3;
	CObject* pObj = nullptr;//�I�u�W�F�N�g�i�[�p
	bool bCollision = true;
	bool bEnd = false;        //���[�v���I��点��t���O
	CEnemy* pEnemy = nullptr;//�G�ւ̃|�C���^
	CBoss* pBoss = nullptr;  //�{�X�ւ̃|�C���^
	CObjectX* pObjX = nullptr;//�I�u�W�F�N�gX�ւ̃|�C���^
	CBullet* pBullet = nullptr;//�e�ւ̃|�C���^

	if (m_TargetType == TARGETTYPE_PLAYER)
	{
		bCollision = CCollision::RectAngleCollisionXY(Pos, VtxMax, VtxMin, Rot,
			CGame::GetPlayer()->GetPos(), CGame::GetPlayer()->GetVtxMax(), CGame::GetPlayer()->GetVtxMin(), CGame::GetPlayer()->GetRot());

		if (bCollision == true)
		{
			CGame::GetPlayer()->SetDamage(1, 10);
			if (m_bCollisionRelease == true)
			{
				SetDeath();
			}
		}
	}
	else if (m_TargetType == TARGETTYPE_ENEMY)
	{
		for (int nCntPriority = 0; nCntPriority < m_nMAXPRIORITY; nCntPriority++)
		{

			pObj = GetTopObject(nCntPriority);//�g�b�v�I�u�W�F�N�g���擾
			while (pObj != nullptr)
			{
				//���̃I�u�W�F�N�g���i�[
				CObject* pNext = pObj->GetNextObject();

				//��ނ̎擾�i�G�Ȃ瓖���蔻��j
				CObject::TYPE type = pObj->GetType();
				bCollision = true;

				if (type == CObject::TYPE_ENEMY || type == CObject::TYPE_BOSS)
				{
					pObjX = (CObjectX*)pObj;

					bCollision = CCollision::RectAngleCollisionXY(Pos, VtxMax, VtxMin, Rot,
						pObjX->GetPos(), pObjX->GetVtxMax(), pObjX->GetVtxMin(), pObjX->GetRot());

					if (bCollision == true)
					{
						pObjX->SetDamage(m_nPower, m_nSetHitStopTime);
						if (m_bCollisionRelease == true)
						{
							//SetDeath();
							//SetDelayDeath(true);
							m_bUse = false;
						}
					}
				}
				else if (type == CObject::TYPE_BULLET)
				{
					if (m_type == ATTACKTYPE12_VINE)
					{
						pBullet = (CBullet*)pObj;
						bCollision = CCollision::RectAngleCollisionXY(Pos, VtxMax, VtxMin, Rot,
							pBullet->GetPos(),D3DXVECTOR3(pBullet->GetWidth(),pBullet->GetHeight(),pBullet->GetWidth()) * 0.5f,
							D3DXVECTOR3(-pBullet->GetWidth(), -pBullet->GetHeight(), -pBullet->GetWidth()) * 0.5f,NULL_VECTOR3);

						if (bCollision == true && pBullet->GetBulletType() == CBullet::BULLETTYPE_ENEMY)
						{
							pBullet->SetDeath();
							if (m_bCollisionRelease == true)
							{
								SetDeath();
							}
						}
					}
				}


				if (bEnd == true)
				{//���[�v�I���i���������������U�������肳�ꂽ�Ƃ�
					break;
				}
				//���X�g�����ɐi�߂�
				pObj = pNext;
			}
		}
	}

}
//===================================================================================================================================================

//=====================================================================
//���̓����蔻����s��
//=====================================================================
void CAttack::BallCollision()
{
	CObject* pObj = nullptr;//�I�u�W�F�N�g�ւ̃|�C���^
	CObjectX* pObjX = nullptr;//�I�u�W�F�N�gX�ւ̃|�C���^
	bool bCollision = false;//�����������ǂ���
	for (int nCntPriority = 0; nCntPriority < m_nMAXPRIORITY; nCntPriority++)
	{
		pObj = GetTopObject(nCntPriority);//�g�b�v�I�u�W�F�N�g���擾
		while (pObj != nullptr)
		{
			CObject* pNext = pObj->GetNextObject();
			CObject::TYPE type = pObj->GetType();
			if (type == CObject::TYPE_PLAYER || type == CObject::TYPE_ENEMY || type == CObject::TYPE_BOSS)
			{
				pObjX = (CObjectX*)pObj;

				bCollision = CCollision::CollisionBall(GetSenterPos(), GetSize(), pObjX->GetSenterPos(), pObjX->GetSize());

				if (bCollision == true)
				{
					switch (m_TargetType)
					{
					case TARGETTYPE_ENEMY:
						if (type == CObject::TYPE_BOSS || type == CObject::TYPE_ENEMY)
						{
							pObjX->SetDamage(m_nPower, m_nSetHitStopTime);
						}
						break;
					case TARGETTYPE_PLAYER:
						if (type == CObject::TYPE_PLAYER)
						{
							pObjX->SetDamage(m_nPower, m_nSetHitStopTime);
						}
						break;
					default:
						break;
					}
				}
			}
			else
			{
				pObj = pNext;
				continue;
			}
			pObj = pNext;//���̐i�߂�
		}
	}
}
//===================================================================================================================================================

//===================================================================<<<�v���C���[�U���N���X>>>===========================================================

//=====================================================================
//�R���X�g���N�^
//=====================================================================
CAttackPlayer::CAttackPlayer()
{

}
//===================================================================================================================================================

//=====================================================================
//�f�X�g���N�^
//=====================================================================
CAttackPlayer::~CAttackPlayer()
{
}
//===================================================================================================================================================

//=====================================================================
//����������
//=====================================================================
HRESULT CAttackPlayer::Init()
{
	CAttack::Init();               //�U���̏�����
	m_fCorrectionRot = 0.0f;       //�␳�p����
	m_nVineDelBulletNum = 0;       //�邪�����e�̐�

	m_SaveScale = NULL_VECTOR3;    //�g�嗦�ۑ��p
	m_TotalRot = NULL_VECTOR3;     //�����I����
	m_bVineDelStart = false;       //������������J�n�t���O

	m_bCrossBombStaging = false;   //�N���X�{���̉��o�t���O
	m_nCrossBombStagingCnt = 0;    //�N���X�{���̉��o���Ԃ��J�E���g����

	m_nBlackHoleDefeatCnt = 0;     //�u���b�N�z�[���œ|�����G�̐����J�E���g����
	return S_OK;
}
//===================================================================================================================================================

//=====================================================================
//�I������
//=====================================================================
void CAttackPlayer::Uninit()
{
	CAttack::Uninit();//�U���̏I������
}
//===================================================================================================================================================

//=====================================================================
//�X�V����
//=====================================================================
void CAttackPlayer::Update()
{
	if (CScene::GetMode() == CScene::MODE_GAME)
	{
		D3DXVECTOR3 Size = GetSize();                                 //�T�C�Y���擾
		D3DXVECTOR3 ParticlePos = CObjectX::GetPos();
		D3DXVECTOR3 & Rot = GetRot();                                   //�������擾
		D3DXVECTOR3 & Move = GetMove();                                //�ړ��ʂ�ݒ�
		D3DXVECTOR3 & Pos = GetPos();                                   //�ʒu���擾
		D3DXVECTOR3 & Scale = GetScale();
		CEmitter* pEmitter = nullptr;                                 //�G�~�b�^�[
		CParticle* pParticle = nullptr;                               //�p�[�e�B�N���擾�p
		float fRot = (float)(rand() % 60 - 30) / 100;
		float fRandAddSpeed = (float)(rand() % 1000) / 1000;
		float fRatioLife = (float)(GetLife()) / (float)(GetMaxLife());//�ő�̗͂Ƃ̊��������߂�
		bool bTurn = false;
		int& nCntTime = GetCntTime();
		int& nPattern = GetPattern();
		switch (GetAttackType())
		{
		case ATTACKTYPE00_FIRE:
			ParticlePos.x += (float)(rand() % (int)(Size.x) - (int)(Size.x / 2)) * 2;
			ParticlePos.y += (float)(rand() % (int)(Size.y) - (int)(Size.y / 2));
			pParticle = CParticle::Create(CParticle::TYPE00_NORMAL, 50, 40.0f, 40.0f,ParticlePos, D3DXVECTOR3(sinf(D3DX_PI * 0.5f + fRot) * -Move.x, cosf(D3DX_PI * 0.5f + fRot) * -Move.x, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), true);
			pParticle->SetAddSpeed(fRandAddSpeed);//�����ʂ�ݒ�
			pParticle = nullptr;
			break;
		case ATTACKTYPE04_BEAM:
			if (CScene::GetMode() == CScene::MODE_GAME)
			{
				Pos = CGame::GetPlayer()->GetPos();
			}
			pParticle = CParticle::Create(CParticle::TYPE01_FIRE,60, 30.0f, 30.0f, ParticlePos, D3DXVECTOR3(sinf(-Rot.z + fRot) * 10.0f, cosf(-Rot.z + fRot) * 10.0f, 0.0f),
				NORMAL_COL, true);
			pParticle = nullptr;
			break;
		case ATTACKTYPE05_ICE:
			Scale += ONE_VECTOR3 * 0.1f;
			break;
		case ATTACKTYPE06_THUNDER:
			if (fRatioLife > 0.5f)
			{
				Scale.y += 0.3f;
			}

			D3DXVECTOR3(0.0f, -0.37f, 0.0f);
			if (nCntTime % 30 == 0)
			{
			    pEmitter = CEmitter::Create((int)(CEffect::EFFECTTYPE01_THUNDER), 60, 20, 20.0f, 20.0f, 40.0f, Pos + D3DXVECTOR3(0.0f, Size.y / 2, 0.0f), 
					D3DXVECTOR3(0.0f, -12.0f, 0.0f),D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				pEmitter->SetUseRotate(true, 20.0f, NULL_VECTOR3, D3DXVECTOR3(0.0f, -0.37f, 0.0f));
			}
			else if (nCntTime % 15 == 0)
			{
				pEmitter = CEmitter::Create((int)(CEffect::EFFECTTYPE01_THUNDER), 60, 20, 20.0f, 20.0f, 40.0f, Pos + D3DXVECTOR3(0.0f, Size.y / 2, 0.0f),
					D3DXVECTOR3(0.0f, -12.0f, 0.0f),D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				pEmitter->SetUseRotate(true, 20.0f, NULL_VECTOR3, D3DXVECTOR3(0.0f, 0.37f, 0.0f));
			}
			Rot.y += 0.4f;
			break;
		case ATTACKTYPE07_MAGICSWORD:
			Pos = CGame::GetPlayer()->GetPos();
			Pos.y += CGame::GetPlayer()->GetSize().y / 2;
			//�v���C���[�̌����ɂ���čU��������ς���

			if (CGame::GetPlayer()->GetRot().y >= D3DX_PI)
			{
				Rot.z = -m_TotalRot.z;
				bTurn = true;
			}
			else
			{
				Rot.z = m_TotalRot.z;
				bTurn = false;
			}


			switch (nPattern)
			{
			case 0:
				SetDoCollision(false);
				m_TotalRot = NULL_VECTOR3;
				if (nCntTime == 1)
				{
					CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_CLAW);
				}
				if ((CManager::GetInputKeyboard()->GetTrigger(DIK_M) == true || CManager::GetInputJoypad()->GetTrigger(CInputJoypad::JOYKEY_B) == true)&&
					nCntTime >= 3)
				{
					nPattern++;
					m_fCorrectionRot = 0.3f;
				}
				break;
			case 1:
				SetDoCollision(true);
				m_TotalRot.z += m_fCorrectionRot;
				CAttackEffect::Create(CAttack::ATTACKTYPE07_MAGICSWORD, 0, 30, Pos, NULL_VECTOR3, ONE_VECTOR3 * 2, Rot, NULL_VECTOR3, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
				if (m_TotalRot.z >= D3DX_PI - (D3DX_PI * 0.5f))
				{
					if (m_nAttackCnt == 0)
					{
						CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_SLASH_000);
						if (bTurn == true)
						{
							CAttackPlayer::Create(CAttack::ATTACKTYPE08_VACUUMBLADE, 35, 100, Pos, D3DXVECTOR3(sinf(D3DX_PI * 0.5f) * 10.0f, 0.0f, cosf(D3DX_PI * 0.5f) * 10.0f), ONE_VECTOR3 * 2.0f,D3DXVECTOR3(0.0f,0.0f,D3DX_PI * 0.5f),
								CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, NULL_VECTOR3, true, 10);
						}
						else
						{
							CAttackPlayer::Create(CAttack::ATTACKTYPE08_VACUUMBLADE, 35, 100, Pos, D3DXVECTOR3(sinf(D3DX_PI * -0.5f) * 10.0f, 0.0f, cosf(D3DX_PI * -0.5f) * 10.0f), ONE_VECTOR3 * 2.0f, D3DXVECTOR3(0.0f, 0.0f, D3DX_PI * -0.5f),
								CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, NULL_VECTOR3, true, 10);
						}
						m_nAttackCnt++;
					}
					m_fCorrectionRot += (0.0f - m_fCorrectionRot) * 0.15f;
				}

				if (m_fCorrectionRot <= 0.001f)
				{
					m_nAttackCnt = 0;
					nPattern = 0;
					nCntTime = 0;
				}
				break;
			default:
				break;
			}
			break;
		case ATTACKTYPE08_VACUUMBLADE:
			CAttackEffect::Create(CAttack::ATTACKTYPE08_VACUUMBLADE, 0, 30, Pos, NULL_VECTOR3, ONE_VECTOR3 * 2, Rot, NULL_VECTOR3, NORMAL_COL);
			break;
		case ATTACKTYPE12_VINE:
			VineProcess();
			break;
		case ATTACKTYPE13_CROSSBOMB:
			CrossBombProcess();
			break;
		case ATTACKTYPE15_BLACKHOLE:
			BlackHoleProcess();
			break;
		case ATTACKTYPE16_PHYCOBURST:
			PhycoBurstProcess();
			break;
		}
		CAttack::Update();//�U���̍X�V����
	}
}
//===================================================================================================================================================

//=====================================================================
//�`�揈��
//=====================================================================
void CAttackPlayer::Draw()
{
	CAttack::Draw();
}
//===================================================================================================================================================

//=====================================================================
//���S�t���O�ݒ菈��
//=====================================================================
void CAttackPlayer::SetDeath()
{
	CAttack::SetDeath();
}
//===================================================================================================================================================

//=====================================================================
//��������
//=====================================================================
CAttackPlayer* CAttackPlayer::Create(ATTACKTYPE type, int nDamage, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 Scale, D3DXVECTOR3 Rot,
	COLLISIONTYPE CollisionType, D3DXVECTOR3 AddRot, bool bCollisionRelease, int nHitStopTime)
{
	CAttackPlayer* pAttackPlayer = DBG_NEW CAttackPlayer;   //�G�𐶐�
	bool bSuccess = pAttackPlayer->CObject::GetCreateSuccess();
	int nIdx = 0;//���f���̃C���f�b�N�X
	if (bSuccess == true)
	{
		if (pAttackPlayer != nullptr)
		{
			pAttackPlayer->SetAttackType(type);  //�U���̎��
			pAttackPlayer->Init();         //����������
			pAttackPlayer->SetHitStopTime(nHitStopTime);          //���������Ƃ��Ɋ��蓖�Ă�q�b�g�X�g�b�v���Ԃ�ݒ肷��
			pAttackPlayer->SetUseBlockCollision(false);
			pAttackPlayer->SetCollisionRelease(bCollisionRelease);//�����������ɏ������ǂ�����ݒ肷��
			pAttackPlayer->SetDoCollision(true);                  //�����蔻������邩�ǂ�����ݒ�
			pAttackPlayer->SetUseInteria(false);//�������g�p���Ȃ�
			pAttackPlayer->SetFormarScale(Scale);                                       //���̊g�嗦��ݒ肷��
			pAttackPlayer->SetMove(move);  //�ړ���
			pAttackPlayer->SetLife(nLife);                    //�̗͂�ݒ�
			pAttackPlayer->SetMaxLife(nLife);                 //�ő�̗͂�ݒ�
			pAttackPlayer->SetAutoSubLife(true);              //�����I�ɑ̗͂����炷���ǂ���
			pAttackPlayer->SetTargetType(CAttack::TARGETTYPE_ENEMY);//�N�ɓ��Ă邩��ݒ�
			pAttackPlayer->SetCollisionType(CollisionType);   //�����蔻��̎��
			pAttackPlayer->SetPower(nDamage);                 //�З�
			pAttackPlayer->SetUseAddRot(true, AddRot);                 //���Z��������̗�
			pAttackPlayer->CObjectX::SetPos(pos);             //�I�u�W�F�N�gX�̈ʒu��ݒ�
			pAttackPlayer->SetRot(Rot);                       //������ݒ�
			pAttackPlayer->SetUseDeath(true);                 //���S�t���O�𔭓����邩�ǂ�����ݒ肷��
			pAttackPlayer->SetPosOld(pos);                                                                                         //1f�O�̈ʒu��ݒ�
			pAttackPlayer->SetSupportPos(pos);                                                                                     //�ݒu�ʒu
			pAttackPlayer->CObject::SetType(CObject::TYPE_ATTACK);                                                                  //�I�u�W�F�N�g�̎�ނ����߂�
			pAttackPlayer->CObjectX::SetScale(Scale);                                                                              //�I�u�W�F�N�g�̊g�嗦
			nIdx = CManager::GetObjectXInfo()->Regist(CObjectX::OBJECTXTYPE_ATTACK, CAttack::m_ATTACK_FILENAME[(int)(type)]);

			pAttackPlayer->CObjectX::BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(CObjectX::OBJECTXTYPE_ATTACK, nIdx),
				CManager::GetObjectXInfo()->GetBuffMat(CObjectX::OBJECTXTYPE_ATTACK, nIdx),
				CManager::GetObjectXInfo()->GetdwNumMat(CObjectX::OBJECTXTYPE_ATTACK, nIdx),
				CManager::GetObjectXInfo()->GetTexture(CObjectX::OBJECTXTYPE_ATTACK, nIdx),
				CManager::GetObjectXInfo()->GetColorValue(CObjectX::OBJECTXTYPE_ATTACK, nIdx));                       //���f���������蓖�Ă�

			pAttackPlayer->CObjectX::SetObjXType(CObjectX::OBJECTXTYPE_ATTACK);                            //�I�u�W�F�N�gX�̃^�C�v��ݒ�
			pAttackPlayer->CObjectX::SetTypeNum((int)(type));                                             //�I�u�W�F�N�gX���Ƃ̃^�C�v�ԍ���ݒ�
			pAttackPlayer->SetSize();                                                                     //X�I�u�W�F�N�g�̃T�C�Y��ݒ肷��
			pAttackPlayer->CObjectX::SetRotType(CObjectX::ROTTYPE::ROTTYPE_NORMAL);                       //�����̎�ނ�ݒ�
		}
		else
		{//�I�u�W�F�N�g�ɋ󂫂��Ȃ������̂Ŕj������
			delete pAttackPlayer;
			pAttackPlayer = nullptr;
			return nullptr;
		}
	}
	return pAttackPlayer;
}
//===================================================================================================================================================

//=====================================================================
//��̏���
//=====================================================================
void CAttackPlayer::VineProcess()
{
	int& nCntTime = GetCntTime();
	D3DXVECTOR3& Scale = GetScale();
	float fRatioLife = 0.0f;
	if (nCntTime <= 30)
	{
		Scale.y += 0.01f;
	}
	else
	{
		if (nCntTime <= 45)
		{
			Scale.y += 0.2f;
		}

	}

	if (m_nVineDelBulletNum <= 0 && m_bVineDelStart == false)
	{//������e�����O�ɂȂ�����
		SetLife(10);//�̗͂��P�O�ɐݒ肵�A���̏����𔭓�
		m_bVineDelStart = true;//��̏����J�n
	}

	if (GetLife() == 10)
	{
		m_SaveScale = Scale;
	}

	if (GetLife() < 10)
	{//�̗͂��P�O��艺�ɂȂ�����g�嗦�������Ă���
		fRatioLife = (float)(GetLife()) / 10;
		Scale.y = m_SaveScale.y * fRatioLife;
	}
}
//===================================================================================================================================================

//=====================================================================
//�N���X�{���̏���
//=====================================================================
void CAttackPlayer::CrossBombProcess()
{
	D3DXVECTOR3 Pos = GetPos();             //�ʒu
	D3DXVECTOR3& Scale = GetScale();        //�g�嗦
	CAttackPlayer* pAttackPlayer = nullptr; //�U���v���C���[

	if (GetLife() % 20 == 0)
	{//���o����
		m_bCrossBombStaging = true;
	}

	if (m_bCrossBombStaging == true)
	{//�傫�����ď����������Ȃ鋓�����J��Ԃ�

		SetColor(CObjectX::COLORTYPE_RED, 2);

		m_nCrossBombStagingCnt++;
		if (m_nCrossBombStagingCnt < 8)
		{//�g��
			Scale += ONE_VECTOR3 * 0.05f;
		}
		else
		{//�k��
			Scale -= ONE_VECTOR3 * 0.05f;
		}

		if (m_nCrossBombStagingCnt >= 10)
		{//���o�t���O���I�t�ɂ���
			m_bCrossBombStaging = false;
			m_nCrossBombStagingCnt = 0;
		}
	}

	if (GetDelayDeath() == false)
	{
		pAttackPlayer = CAttackPlayer::Create(CAttack::ATTACKTYPE14_HELLFIRE, 70,100,NULL_VECTOR3, NULL_VECTOR3,D3DXVECTOR3(2.0f,8.0f,2.0f), NULL_VECTOR3, CAttack::COLLISIONTYPE02_TRUERECTANGLEXY,
			D3DXVECTOR3(0.0f, 0.4f, 0.0f), false, 10);

		//�I�t�Z�b�g��ݒ�
		pAttackPlayer->SetParentInfo(Pos, D3DXVECTOR3(0.0f, 0.0f,D3DX_PI * 0.5f), ONE_VECTOR3,true);

		pAttackPlayer = CAttackPlayer::Create(CAttack::ATTACKTYPE14_HELLFIRE, 70,100, NULL_VECTOR3, NULL_VECTOR3, D3DXVECTOR3(2.0f, 8.0f, 2.0f), NULL_VECTOR3, CAttack::COLLISIONTYPE02_TRUERECTANGLEXY,
			D3DXVECTOR3(0.0f, 0.4f, 0.0f), false, 10);

		//�I�t�Z�b�g��ݒ�
		pAttackPlayer->SetParentInfo(Pos,NULL_VECTOR3, ONE_VECTOR3, true);

	}
}
//===================================================================================================================================================

//=====================================================================
//�u���b�N�z�[���̏���
//=====================================================================
void CAttackPlayer::BlackHoleProcess()
{
	CObject* pObj = nullptr;
	CBullet* pBullet = nullptr;
	int& nCntTime = GetCntTime();//�o������

	D3DXVECTOR3 MyPos = GetPos();            //�U���̈ʒu
	D3DXVECTOR3 MyVtxMax = GetVtxMax();      //�U���̍ő咸�_
	D3DXVECTOR3 MyVtxMin = GetVtxMin();      //�U���̍ŏ����_
	D3DXVECTOR3& MyScale = GetScale();       //�U���̊g�嗦

	D3DXVECTOR3 ComparisonPos = NULL_VECTOR3;//��r�p�ʒu
	D3DXVECTOR3 ComparisonVtxMax = NULL_VECTOR3;//��r�p�ő咸�_
	D3DXVECTOR3 ComparisonVtxMin = NULL_VECTOR3;//��r�p�ŏ����_

	for (int nCntPriority = 0; nCntPriority < m_nMAXPRIORITY; nCntPriority++)
	{
		pObj = GetTopObject(nCntPriority);//�g�b�v�I�u�W�F�N�g���擾
		while (pObj != nullptr)
		{
			//���̃I�u�W�F�N�g���i�[
			CObject* pNext = pObj->GetNextObject();

			//��ނ̎擾�i�G�Ȃ瓖���蔻��j
			CObject::TYPE type = pObj->GetType();

			if (type == CObject::TYPE_BULLET)
			{
				pBullet = static_cast<CBullet*>(pObj);//�o���b�g�ɃL���X�g
				ComparisonPos = pBullet->GetPos();
				ComparisonVtxMax = D3DXVECTOR3(pBullet->GetWidth(), pBullet->GetHeight(), pBullet->GetWidth());
				ComparisonVtxMin = D3DXVECTOR3(-pBullet->GetWidth(), -pBullet->GetHeight(), -pBullet->GetWidth());
			}
			else
			{
				pObj = pNext;//���X�g�����ɐi�߂�
				continue;
			}
			pBullet->SetPos(ComparisonPos + CCalculation::Calculation3DVec(ComparisonPos, GetPos(), 20.0f));//�e�������I�Ƀu���b�N�z�[���Ɍ����Ĉړ�������
			if (MyPos.x + MyVtxMin.x <= ComparisonPos.x + ComparisonVtxMax.x &&
				MyPos.x + MyVtxMax.x >= ComparisonPos.x + ComparisonVtxMin.x &&
				MyPos.y + MyVtxMin.y <= ComparisonPos.y + ComparisonVtxMax.y &&
				MyPos.y + MyVtxMax.y >= ComparisonPos.y + ComparisonVtxMin.y &&
				MyPos.z + MyVtxMin.z <= ComparisonPos.z + ComparisonVtxMax.x &&
				MyPos.z + MyVtxMax.z >= ComparisonPos.z + ComparisonVtxMin.x)
			{//���[�v�}�[�J�[�ɓ���������
				pBullet->SetUseDeath(true);
				pBullet->SetDeath();
				SetScale(GetScale() + ONE_VECTOR3 * 0.01f);//�u���b�N�z�[���̊g�嗦���グ��
				m_nBlackHoleDefeatCnt++;//�u���b�N�z�[���ŋz�����񂾒e�̐����C���N�������g
			}
			//���X�g�����ɐi�߂�
			pObj = pNext;
		}
	}

	if (GetLife() == 1)
	{
		for (int nCntDefeat = 0; nCntDefeat < m_nBlackHoleDefeatCnt; nCntDefeat++)
		{//�e���z�����ނقǔh��ȃp�[�e�B�N���ɂȂ�
			CParticle::SummonParticle(CParticle::TYPE03_THUNDER,1,100, 40.0f, 40.0f, 200, 10, true, MyPos, NORMAL_COL, true);
		}	
		//�e���z�����ނقǈЗ̓A�b�v
	    CAttackPlayer* pAttackPlayer = CAttackPlayer::Create(CAttack::ATTACKTYPE11_EXPLOSION, m_nBlackHoleDefeatCnt * 2,m_nBlackHoleDefeatCnt, GetPos(), NULL_VECTOR3, ONE_VECTOR3,
				NULL_VECTOR3, CAttack::COLLISIONTYPE00_SQUARE, ONE_VECTOR3 * 0.1f, false, 20);//��������
		pAttackPlayer->SetUseAddScale(ONE_VECTOR3 * 0.01f * static_cast<float>(m_nBlackHoleDefeatCnt), true);//�z�����ނقǊg�呬�x�㏸
		pAttackPlayer->SetUseAttackEffect(true, 120, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));//�U���ɋO�Ղ�����
	}


}
//===================================================================================================================================================

//=====================================================================
//�T�C�R�o�[�X�g�̏���
//=====================================================================
void CAttackPlayer::PhycoBurstProcess()
{
	int nLife = GetLife();            //�̗͂��擾
	int nMaxLife = GetMaxLife();      //�ő�̗͂��擾
	int nDiffrenceLife = nMaxLife - nLife;//�ő�̗͂Ƃ̍��������߂�
	float fRatio = 0.0f;              //����
	CEmitter* pEmitter = nullptr;     //�G�~�b�^�[�ւ̃|�C���^
	D3DXVECTOR3 Pos = GetPos();       //�ʒu���擾
	D3DXVECTOR3 Move = NULL_VECTOR3;  //�ړ���
	D3DXVECTOR3& Scale = GetScale();   //�g�嗦���擾
	D3DXVECTOR3& Rot = GetRot();      //�������擾
	D3DXCOLOR col = {};               //�F����
	D3DXVECTOR3& AddRot = GetAddRot();//�����̉��Z�ʂ��擾
	//�I�u�W�F�N�g�̐F�����𔖂����Ă���
	fRatio = (float)(nLife) / (float)(nMaxLife);
	SetColorAlphaOnly(fRatio);

	//�g�嗦����Z����
	float fScale = 0.012f * (float)(nDiffrenceLife);
	Scale += ONE_VECTOR3 * fScale;

	AddRot += AddRot * 0.1f;

	if (nDiffrenceLife == 1)
	{//�ŏ��ɃG�~�b�^�[������

		for (int nCnt = 0; nCnt < 10; nCnt++)
		{
			//��ԕ��������߂�
			Move.x = float(rand() % 100 - 50) / 100;
			Move.y = float(rand() % 100 - 50) / 100;
			Move.z = float(rand() % 100 - 50) / 100;

			//�����I�u�W�F�N�g�̐F����
			col.r = float(rand() % 100) / 100;
			col.g = float(rand() % 100) / 100;
			col.b = float(rand() % 100) / 100;
			col.a = 1.0f;

			pEmitter = CEmitter::Create(CEffect::EFFECTTYPE00_NORMAL, nMaxLife - (nMaxLife - nLife), 20, 0.0f, 40.0f, 40.0f,NULL_VECTOR3, Move, col);
			pEmitter->SetUseMtxChild(&GetMatrixWorld(),true);
		}
	}

	if (nLife < 2)
	{
		SetSummonObjectDeath(true);//���������I�u�W�F�N�g�������t���O�𔭓�
	}
}
//===================================================================================================================================================

//===================================================================<<<�G�l�~�[�U���N���X>>>===========================================================

//=====================================================================
//�R���X�g���N�^
//=====================================================================
CAttackEnemy::CAttackEnemy()
{

}
//===================================================================================================================================================

//=====================================================================
//�f�X�g���N�^
//=====================================================================
CAttackEnemy::~CAttackEnemy()
{

}
//===================================================================================================================================================

//=====================================================================
//����������
//=====================================================================
HRESULT CAttackEnemy::Init()
{
	CAttack::Init();//�U���̏���������
	return S_OK;
}
//===================================================================================================================================================

//=====================================================================
//�I������
//=====================================================================
void CAttackEnemy::Uninit()
{
	CAttack::Uninit();//�U���̏I������
}
//===================================================================================================================================================

//=====================================================================
//�X�V����
//=====================================================================
void CAttackEnemy::Update()
{

	D3DXVECTOR3& PosOld = GetPosOld();                             //1f�O�̈ʒu��ݒ�
	D3DXVECTOR3 Size = GetSize();                                 //�T�C�Y���擾
	D3DXVECTOR3& Scale = GetScale();                               //�g�嗦���擾
	D3DXVECTOR3& Pos = CObjectX::GetPos();                         //�ʒu���擾
	D3DXVECTOR3& Rot = GetRot();                                   //�������擾
	CParticle* pParticle = nullptr;                                //�p�[�e�B�N���ւ̃|�C���^
	float fRandRot = 0.0f;                                         //��������
	switch (GetAttackType())
	{
	case ATTACKTYPE02_NEEDLE:
		break;
	case ATTACKTYPE05_ICE:
		if (Scale.x <= 4.0f)
		{
			Scale += (ONE_VECTOR3 * 0.1f);
		}
		break;
	case ATTACKTYPE10_SHOCKWAVE:
		fRandRot = float(rand() % 628) / 100;
		pParticle = CParticle::Create(CParticle::TYPE00_NORMAL, 45, 20.0f, 20.0f, Pos,
			D3DXVECTOR3(sinf(fRandRot) * 10.0f, 10.0f, cosf(fRandRot) * 10.0f), NORMAL_COL, true);

		pParticle->SetUseGravity(-0.8f);
		break;
	}

	CAttack::Update();//�U���̍X�V����
}
//===================================================================================================================================================

//=====================================================================
//�`�揈��
//=====================================================================
void CAttackEnemy::Draw()
{
	CAttack::Draw();//�U���̕`�揈��
}
//===================================================================================================================================================

//=====================================================================
//���S�t���O�ݒ菈��
//=====================================================================
void CAttackEnemy::SetDeath()
{
	CAttack::SetDeath();
}
//===================================================================================================================================================

//=====================================================================
//��������
//=====================================================================
CAttackEnemy* CAttackEnemy::Create(ATTACKTYPE type, int nDamage, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 Scale, D3DXVECTOR3 Rot,
	COLLISIONTYPE CollisionType, D3DXVECTOR3 AddRot, bool bCollisionRelease, int nHitStopTime)
{
	CAttackEnemy* pAttackEnemy = DBG_NEW CAttackEnemy;   //�G�𐶐�
	bool bSuccess = pAttackEnemy->CObject::GetCreateSuccess();
	int nIdx = 0;//���f���̃C���f�b�N�X
	if (bSuccess == true)
	{
		if (pAttackEnemy != nullptr)
		{
			pAttackEnemy->SetAttackType(type);  //�U���̎��
			pAttackEnemy->Init();         //����������
			pAttackEnemy->SetHitStopTime(nHitStopTime);          //���������Ƃ��Ɋ��蓖�Ă�q�b�g�X�g�b�v���Ԃ�ݒ肷��
			pAttackEnemy->SetUseBlockCollision(false);
			pAttackEnemy->SetCollisionRelease(bCollisionRelease);//�����������ɏ������ǂ�����ݒ肷��
			pAttackEnemy->SetUseInteria(false);//�������g�p���Ȃ�
			pAttackEnemy->SetMove(move);  //�ړ���
			pAttackEnemy->SetDoCollision(true);                  //�����蔻������邩�ǂ�����ݒ�
			pAttackEnemy->SetLife(nLife);                    //�̗͂�ݒ�
			pAttackEnemy->SetMaxLife(nLife);                 //�ő�̗͂�ݒ�
			pAttackEnemy->SetAutoSubLife(true);              //�����I�ɑ̗͂����炷���ǂ���
			pAttackEnemy->SetTargetType(CAttack::TARGETTYPE_PLAYER);//�N�ɓ��Ă邩��ݒ�
			pAttackEnemy->SetCollisionType(CollisionType);   //�����蔻��̎��
			pAttackEnemy->SetPower(nDamage);                 //�З�
			pAttackEnemy->SetUseAddRot(true,AddRot);                 //���Z��������̗�
			pAttackEnemy->CObjectX::SetPos(pos);             //�I�u�W�F�N�gX�̈ʒu��ݒ�
			pAttackEnemy->SetRot(Rot);                       //������ݒ�
			pAttackEnemy->SetPosOld(pos);                                                                                         //1f�O�̈ʒu��ݒ�
			pAttackEnemy->SetSupportPos(pos);                                                                                     //�ݒu�ʒu
			pAttackEnemy->CObject::SetType(CObject::TYPE_ATTACK);                                                                  //�I�u�W�F�N�g�̎�ނ����߂�
			pAttackEnemy->CObjectX::SetScale(Scale);                                                                              //�I�u�W�F�N�g�̊g�嗦
			pAttackEnemy->SetFormarScale(Scale);                                       //���̊g�嗦��ݒ肷��
			pAttackEnemy->SetUseDeath(true);                 //���S�t���O�𔭓����邩�ǂ�����ݒ肷��
			nIdx = CManager::GetObjectXInfo()->Regist(CObjectX::OBJECTXTYPE_ATTACK, CAttack::m_ATTACK_FILENAME[(int)(type)]);
			pAttackEnemy->CObjectX::BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(CObjectX::OBJECTXTYPE_ATTACK, nIdx),
				CManager::GetObjectXInfo()->GetBuffMat(CObjectX::OBJECTXTYPE_ATTACK, nIdx),
				CManager::GetObjectXInfo()->GetdwNumMat(CObjectX::OBJECTXTYPE_ATTACK, nIdx),
				CManager::GetObjectXInfo()->GetTexture(CObjectX::OBJECTXTYPE_ATTACK, nIdx),
				CManager::GetObjectXInfo()->GetColorValue(CObjectX::OBJECTXTYPE_ATTACK, nIdx));                       //���f���������蓖�Ă�

			pAttackEnemy->CObjectX::SetObjXType(CObjectX::OBJECTXTYPE_ATTACK);                            //�I�u�W�F�N�gX�̃^�C�v��ݒ�
			pAttackEnemy->CObjectX::SetTypeNum((int)(type));                                             //�I�u�W�F�N�gX���Ƃ̃^�C�v�ԍ���ݒ�
			pAttackEnemy->SetSize();                                                                     //X�I�u�W�F�N�g�̃T�C�Y��ݒ肷��
			pAttackEnemy->CObjectX::SetRotType(CObjectX::ROTTYPE::ROTTYPE_NORMAL);                       //�����̎�ނ�ݒ�
		}
		else
		{//�I�u�W�F�N�g�ɋ󂫂��Ȃ������̂Ŕj������
			delete pAttackEnemy;
			pAttackEnemy = nullptr;
			return nullptr;
		}
	}
	return pAttackEnemy;
}
//===================================================================================================================================================

//================================================================<<�U���G�t�F�N�g�N���X>>==================================================================

//==============================================================================
//�R���X�g���N�^
//==============================================================================
CAttackEffect::CAttackEffect()
{

}
//===================================================================================================================================================

//==============================================================================
//�f�X�g���N�^
//==============================================================================
CAttackEffect::~CAttackEffect()
{

}
//===================================================================================================================================================

//==============================================================================
//����������
//==============================================================================
HRESULT CAttackEffect::Init()
{
	m_MaxScale = NULL_VECTOR3;//�g�嗦�̍ő�l
	m_Col = {};               //�F����
	CAttack::Init();          //�U���̏���������
	return S_OK;
}
//===================================================================================================================================================

//==============================================================================
//�I������
//==============================================================================
void CAttackEffect::Uninit()
{
	CAttack::Uninit();//�U���̏I������
}
//===================================================================================================================================================

//==============================================================================
//�X�V����
//==============================================================================
void CAttackEffect::Update()
{
	float fRatioLife = (float)(GetLife()) / (float)(GetMaxLife());//�c��̗͂̊��������߂�
	m_Col.a = fRatioLife;//�A���t�@�l��ݒ�
	//D3DXVECTOR3 Scale = NULL_VECTOR3;
	//Scale = m_MaxScale * fRatioLife;
	SetOriginalColor(m_Col, 2);//�F������ݒ肷��
	CAttack::Update();//�U���̍X�V����
}
//===================================================================================================================================================

//==============================================================================
//�`�揈��
//==============================================================================
void CAttackEffect::Draw()
{
	CAttack::Draw();//�U���̕`�揈��
}
//===================================================================================================================================================

//=====================================================================
//���S�t���O�ݒ菈��
//=====================================================================
void CAttackEffect::SetDeath()
{
	CAttack::SetDeath();
}
//===================================================================================================================================================

//==============================================================================
//��������
//==============================================================================
CAttackEffect* CAttackEffect::Create(ATTACKTYPE type, int nDamage, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 Scale, D3DXVECTOR3 Rot, D3DXVECTOR3 AddRot, D3DXCOLOR col)
{
	CAttackEffect* pAttackEffect = DBG_NEW CAttackEffect;   //�G�𐶐�
	bool bSuccess = pAttackEffect->CObject::GetCreateSuccess();
	int nIdx = 0;//���f���̃C���f�b�N�X
	if (bSuccess == true)
	{
		if (pAttackEffect != nullptr)
		{
			pAttackEffect->SetAttackType(type);  //�U���̎��
			pAttackEffect->SetUseInitialSound(false);//���������ɃT�E���h���Đ����邩�ǂ���
			pAttackEffect->Init();         //����������
			pAttackEffect->SetUseShadow(false);//�e���g�p���Ȃ�
			pAttackEffect->SetUseBlockCollision(false);
			pAttackEffect->SetUseInteria(false);//�������g�p���Ȃ�
			pAttackEffect->m_MaxScale = Scale;//�g�嗦�̍ő�l
			pAttackEffect->m_Col = col;
			pAttackEffect->SetOriginalColor(col, 2);          //�F������ݒ�     
			pAttackEffect->SetMove(move);   //�ړ���
			pAttackEffect->SetDoCollision(false);                  //�����蔻������邩�ǂ�����ݒ�
			pAttackEffect->SetLife(nLife);                    //�̗͂�ݒ�
			pAttackEffect->SetMaxLife(nLife);                 //�ő�̗͂�ݒ�
			pAttackEffect->SetAutoSubLife(true);              //�����I�ɑ̗͂����炷���ǂ���
			pAttackEffect->SetTargetType(CAttack::TARGETTYPE_PLAYER);//�N�ɓ��Ă邩��ݒ�
			pAttackEffect->SetPower(nDamage);                 //�З�
			pAttackEffect->SetUseAddRot(true, AddRot);                 //���Z��������̗�
			pAttackEffect->CObjectX::SetPos(pos);             //�I�u�W�F�N�gX�̈ʒu��ݒ�
			pAttackEffect->SetRot(Rot);                       //������ݒ�
			pAttackEffect->SetPosOld(pos);                                                                                         //1f�O�̈ʒu��ݒ�
			pAttackEffect->SetSupportPos(pos);                                                                                     //�ݒu�ʒu
			pAttackEffect->CObject::SetType(CObject::TYPE_ATTACK);                                                                  //�I�u�W�F�N�g�̎�ނ����߂�
			pAttackEffect->CObjectX::SetScale(Scale);                                                                              //�I�u�W�F�N�g�̊g�嗦
			pAttackEffect->SetFormarScale(Scale);                                       //���̊g�嗦��ݒ肷��
			pAttackEffect->SetUseDeath(true);                 //���S�t���O�𔭓����邩�ǂ�����ݒ肷��
			nIdx = CManager::GetObjectXInfo()->Regist(CObjectX::OBJECTXTYPE_ATTACK, CAttack::m_ATTACK_FILENAME[(int)(type)]);
			pAttackEffect->CObjectX::BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(CObjectX::OBJECTXTYPE_ATTACK, nIdx),
				CManager::GetObjectXInfo()->GetBuffMat(CObjectX::OBJECTXTYPE_ATTACK, nIdx),
				CManager::GetObjectXInfo()->GetdwNumMat(CObjectX::OBJECTXTYPE_ATTACK, nIdx),
				CManager::GetObjectXInfo()->GetTexture(CObjectX::OBJECTXTYPE_ATTACK, nIdx),
				CManager::GetObjectXInfo()->GetColorValue(CObjectX::OBJECTXTYPE_ATTACK, nIdx));                       //���f���������蓖�Ă�

			pAttackEffect->CObjectX::SetObjXType(CObjectX::OBJECTXTYPE_ATTACK);                            //�I�u�W�F�N�gX�̃^�C�v��ݒ�
			pAttackEffect->CObjectX::SetTypeNum((int)(type));                                             //�I�u�W�F�N�gX���Ƃ̃^�C�v�ԍ���ݒ�
			pAttackEffect->SetSize();                                                                     //X�I�u�W�F�N�g�̃T�C�Y��ݒ肷��
			pAttackEffect->CObjectX::SetRotType(CObjectX::ROTTYPE::ROTTYPE_NORMAL);                       //�����̎�ނ�ݒ�
		}
		else
		{//�I�u�W�F�N�g�ɋ󂫂��Ȃ������̂Ŕj������
			delete pAttackEffect;
			pAttackEffect = nullptr;
			return nullptr;
		}
	}
	return pAttackEffect;
}
//===================================================================================================================================================

//==============================================================================
//�R���X�g���N�^
//==============================================================================
CMrStrongAttack::CMrStrongAttack()
{

}
//===================================================================================================================================================

//==============================================================================
//�f�X�g���N�^
//==============================================================================
CMrStrongAttack::~CMrStrongAttack()
{

}
//===================================================================================================================================================

//==============================================================================
//����������
//==============================================================================
HRESULT CMrStrongAttack::Init()
{
	CAttack::Init();
	return S_OK;
}
//===================================================================================================================================================

//==============================================================================
//�I������
//==============================================================================
void CMrStrongAttack::Uninit()
{
	CAttack::Uninit();
}
//===================================================================================================================================================

//==============================================================================
//�X�V����
//==============================================================================
void CMrStrongAttack::Update()
{
	switch (GetAttackType())
	{
	case ATTACKTYPE17_BLOCK:
		BlockProcess();
		break;
	default:
		break;
	}

	CAttack::Update();
}
//===================================================================================================================================================

//==============================================================================
//�`�揈��
//==============================================================================
void CMrStrongAttack::Draw()
{
	CAttack::Draw();
}
//===================================================================================================================================================

//==============================================================================
//�`�揈��
//==============================================================================
void CMrStrongAttack::SetDeath()
{
	CAttack::SetDeath();
}
//===================================================================================================================================================

//==============================================================================
//��������
//==============================================================================
CMrStrongAttack* CMrStrongAttack::Create(ATTACKTYPE type, int nDamage, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 Scale, D3DXVECTOR3 Rot,
	COLLISIONTYPE CollisionType, D3DXVECTOR3 AddRot, bool bCollisionRelease, int nHitStopTime)
{
	CMrStrongAttack* pMrStrongAttack = DBG_NEW CMrStrongAttack;   //�G�𐶐�
	bool bSuccess = pMrStrongAttack->CObject::GetCreateSuccess();
	int nIdx = 0;//���f���̃C���f�b�N�X
	if (bSuccess == true)
	{
		if (pMrStrongAttack != nullptr)
		{
			pMrStrongAttack->SetAttackType(type);  //�U���̎��
			pMrStrongAttack->Init();         //����������
			pMrStrongAttack->SetHitStopTime(nHitStopTime);          //���������Ƃ��Ɋ��蓖�Ă�q�b�g�X�g�b�v���Ԃ�ݒ肷��
			pMrStrongAttack->SetUseBlockCollision(false);
			pMrStrongAttack->SetCollisionRelease(bCollisionRelease);//�����������ɏ������ǂ�����ݒ肷��
			pMrStrongAttack->SetUseInteria(false);//�������g�p���Ȃ�
			pMrStrongAttack->SetMove(move);  //�ړ���
			pMrStrongAttack->SetDoCollision(true);                  //�����蔻������邩�ǂ�����ݒ�
			pMrStrongAttack->SetLife(nLife);                    //�̗͂�ݒ�
			pMrStrongAttack->SetMaxLife(nLife);                 //�ő�̗͂�ݒ�
			pMrStrongAttack->SetAutoSubLife(true);              //�����I�ɑ̗͂����炷���ǂ���
			pMrStrongAttack->SetTargetType(CAttack::TARGETTYPE_PLAYER);//�N�ɓ��Ă邩��ݒ�
			pMrStrongAttack->SetCollisionType(CollisionType);   //�����蔻��̎��
			pMrStrongAttack->SetPower(nDamage);                 //�З�
			pMrStrongAttack->SetUseAddRot(true, AddRot);                 //���Z��������̗�
			pMrStrongAttack->CObjectX::SetPos(pos);             //�I�u�W�F�N�gX�̈ʒu��ݒ�
			pMrStrongAttack->SetRot(Rot);                       //������ݒ�
			pMrStrongAttack->SetPosOld(pos);                                                                                         //1f�O�̈ʒu��ݒ�
			pMrStrongAttack->SetSupportPos(pos);                                                                                     //�ݒu�ʒu
			pMrStrongAttack->CObject::SetType(CObject::TYPE_ATTACK);                                                                  //�I�u�W�F�N�g�̎�ނ����߂�
			pMrStrongAttack->CObjectX::SetScale(Scale);                                                                              //�I�u�W�F�N�g�̊g�嗦
			pMrStrongAttack->SetFormarScale(Scale);                                       //���̊g�嗦��ݒ肷��
			pMrStrongAttack->SetUseDeath(true);                 //���S�t���O�𔭓����邩�ǂ�����ݒ肷��
			nIdx = CManager::GetObjectXInfo()->Regist(CObjectX::OBJECTXTYPE_ATTACK, CAttack::m_ATTACK_FILENAME[(int)(type)]);
			pMrStrongAttack->CObjectX::BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(CObjectX::OBJECTXTYPE_ATTACK, nIdx),
				CManager::GetObjectXInfo()->GetBuffMat(CObjectX::OBJECTXTYPE_ATTACK, nIdx),
				CManager::GetObjectXInfo()->GetdwNumMat(CObjectX::OBJECTXTYPE_ATTACK, nIdx),
				CManager::GetObjectXInfo()->GetTexture(CObjectX::OBJECTXTYPE_ATTACK, nIdx),
				CManager::GetObjectXInfo()->GetColorValue(CObjectX::OBJECTXTYPE_ATTACK, nIdx));                       //���f���������蓖�Ă�

			pMrStrongAttack->CObjectX::SetObjXType(CObjectX::OBJECTXTYPE_ATTACK);                            //�I�u�W�F�N�gX�̃^�C�v��ݒ�
			pMrStrongAttack->CObjectX::SetTypeNum((int)(type));                                             //�I�u�W�F�N�gX���Ƃ̃^�C�v�ԍ���ݒ�
			pMrStrongAttack->SetSize();                                                                     //X�I�u�W�F�N�g�̃T�C�Y��ݒ肷��
			pMrStrongAttack->CObjectX::SetRotType(CObjectX::ROTTYPE::ROTTYPE_NORMAL);                       //�����̎�ނ�ݒ�
		}
		else
		{//�I�u�W�F�N�g�ɋ󂫂��Ȃ������̂Ŕj������
			delete pMrStrongAttack;
			pMrStrongAttack = nullptr;
			return nullptr;
		}
	}
	return pMrStrongAttack;
}
//===================================================================================================================================================

//==============================================================================
//�u���b�N�U���̏���
//==============================================================================
void CMrStrongAttack::BlockProcess()
{
	float fVXaim = 0.0f;
	float fVYaim = 0.0f;
	float fVLaim = 0.0f;
	D3DXVECTOR3& Pos = GetPos();
	D3DXVECTOR3& Move = GetMove();
	D3DXVECTOR3 PurposePos = GetPurposePos();
	int& nPattern = GetPattern();//�p�^�[��
	int& nPatternTime = GetPatternTime();//�p�^�[������
	switch (nPattern)
	{
	case 0:
		fVXaim = PurposePos.x - Pos.x;
		fVYaim = PurposePos.y - Pos.y;
		fVLaim = atan2f(fVXaim, fVYaim);

		Move = D3DXVECTOR3(sinf(fVLaim) * 10.0f, cosf(fVLaim) * 10.0f, 0.0f);

		if (Pos.x > PurposePos.x - 10.0f && Pos.x < PurposePos.x + 10.0f &&
			Pos.y > PurposePos.y - 10.0f && Pos.y < PurposePos.y + 10.0f)
		{
			Pos = PurposePos;
			Move = NULL_VECTOR3;
			nPattern++;
			nPatternTime = 0;
		}
		break;
	case 1:
		if (nPatternTime == 60)
		{
			nPattern++;
			nPatternTime = 0;
		}
		break;
	case 2:
		if (nPatternTime == 1)
		{
			Move.y = 3.0f;
		}
		
		if (nPatternTime == 2)
		{//����������
			SetUseMultiSpeed(true, D3DXVECTOR3(1.0f, 1.03f, 1.0f));
			nPattern++;
			nPatternTime = 0;
		}
		break;
	case 3:
		if (Pos.y >= 1000.0f)
		{
			Pos.z = 0.0f;
			SetUseMultiSpeed(false, NULL_VECTOR3);
			//SetUseAddSpeed(true, D3DXVECTOR3(0.0f, 0.01f, 0.0f));
			Move.y = -20.0f;

			nPattern++;
			nPatternTime = 0;
		}
		break;
	case 4:
		CAttackEffect::Create(CAttack::ATTACKTYPE17_BLOCK, 0, 20, Pos, NULL_VECTOR3, GetScale(), GetRot(), NULL_VECTOR3, D3DXCOLOR(1.0f, 0.0f, 1.0f,1.0f));
		break;
	default:
		break;
	}	
}
//===================================================================================================================================================

//==============================================================================
//�R���X�g���N�^
//==============================================================================
CRileyAttack::CRileyAttack()
{

}
//===================================================================================================================================================

//==============================================================================
//�f�X�g���N�^
//==============================================================================
CRileyAttack::~CRileyAttack()
{

}
//===================================================================================================================================================

//==============================================================================
//����������
//==============================================================================
HRESULT CRileyAttack::Init()
{
	CAttack::Init();
	m_AttackMove = ATTACKMOVE_RILEY_00_NONE;
	return S_OK;
}
//===================================================================================================================================================

//==============================================================================
//�I������
//==============================================================================
void CRileyAttack::Uninit()
{
	CAttack::Uninit();
}
//===================================================================================================================================================

//==============================================================================
//�X�V����
//==============================================================================
void CRileyAttack::Update()
{
	switch (m_AttackMove)
	{
	case ATTACKMOVE_RILEY_01_ACTION03:
		Action03_Process();
		break;
	default:
		break;
	}

	CAttack::Update();
}
//===================================================================================================================================================

//==============================================================================
//�`�揈��
//==============================================================================
void CRileyAttack::Draw()
{
	CAttack::Draw();
}
//===================================================================================================================================================

//==============================================================================
//��������
//==============================================================================
CRileyAttack* CRileyAttack::Create(ATTACKTYPE type, int nDamage, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 Scale, D3DXVECTOR3 Rot,
	COLLISIONTYPE CollisionType, D3DXVECTOR3 AddRot, bool bCollisionRelease, int nHitStopTime)
{
	CRileyAttack* pRileyAttack = DBG_NEW CRileyAttack;   //�G�𐶐�
	bool bSuccess = pRileyAttack->CObject::GetCreateSuccess();
	int nIdx = 0;//���f���̃C���f�b�N�X
	if (bSuccess == true)
	{
		if (pRileyAttack != nullptr)
		{
			pRileyAttack->SetAttackType(type);  //�U���̎��
			pRileyAttack->Init();         //����������
			pRileyAttack->SetUseBlockCollision(false);
			pRileyAttack->SetHitStopTime(nHitStopTime);          //���������Ƃ��Ɋ��蓖�Ă�q�b�g�X�g�b�v���Ԃ�ݒ肷��
			pRileyAttack->SetCollisionRelease(bCollisionRelease);//�����������ɏ������ǂ�����ݒ肷��
			pRileyAttack->SetUseInteria(false);//�������g�p���Ȃ�
			pRileyAttack->SetMove(move);  //�ړ���
			pRileyAttack->SetDoCollision(true);                  //�����蔻������邩�ǂ�����ݒ�
			pRileyAttack->SetLife(nLife);                    //�̗͂�ݒ�
			pRileyAttack->SetMaxLife(nLife);                 //�ő�̗͂�ݒ�
			pRileyAttack->SetAutoSubLife(true);              //�����I�ɑ̗͂����炷���ǂ���
			pRileyAttack->SetTargetType(CAttack::TARGETTYPE_PLAYER);//�N�ɓ��Ă邩��ݒ�
			pRileyAttack->SetCollisionType(CollisionType);   //�����蔻��̎��
			pRileyAttack->SetPower(nDamage);                 //�З�
			pRileyAttack->SetUseAddRot(true, AddRot);                 //���Z��������̗�
			pRileyAttack->CObjectX::SetPos(pos);             //�I�u�W�F�N�gX�̈ʒu��ݒ�
			pRileyAttack->SetRot(Rot);                       //������ݒ�
			pRileyAttack->SetPosOld(pos);                                                                                         //1f�O�̈ʒu��ݒ�
			pRileyAttack->SetSupportPos(pos);                                                                                     //�ݒu�ʒu
			pRileyAttack->CObject::SetType(CObject::TYPE_ATTACK);                                                                  //�I�u�W�F�N�g�̎�ނ����߂�
			pRileyAttack->CObjectX::SetScale(Scale);                                                                              //�I�u�W�F�N�g�̊g�嗦
			pRileyAttack->SetFormarScale(Scale);                                       //���̊g�嗦��ݒ肷��
			pRileyAttack->SetUseDeath(true);                 //���S�t���O�𔭓����邩�ǂ�����ݒ肷��
			nIdx = CManager::GetObjectXInfo()->Regist(CObjectX::OBJECTXTYPE_ATTACK, CAttack::m_ATTACK_FILENAME[(int)(type)]);
			pRileyAttack->CObjectX::BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(CObjectX::OBJECTXTYPE_ATTACK, nIdx),
				CManager::GetObjectXInfo()->GetBuffMat(CObjectX::OBJECTXTYPE_ATTACK, nIdx),
				CManager::GetObjectXInfo()->GetdwNumMat(CObjectX::OBJECTXTYPE_ATTACK, nIdx),
				CManager::GetObjectXInfo()->GetTexture(CObjectX::OBJECTXTYPE_ATTACK, nIdx),
				CManager::GetObjectXInfo()->GetColorValue(CObjectX::OBJECTXTYPE_ATTACK, nIdx));                       //���f���������蓖�Ă�

			pRileyAttack->CObjectX::SetObjXType(CObjectX::OBJECTXTYPE_ATTACK);                            //�I�u�W�F�N�gX�̃^�C�v��ݒ�
			pRileyAttack->CObjectX::SetTypeNum((int)(type));                                             //�I�u�W�F�N�gX���Ƃ̃^�C�v�ԍ���ݒ�
			pRileyAttack->SetSize();                                                                     //X�I�u�W�F�N�g�̃T�C�Y��ݒ肷��
			pRileyAttack->CObjectX::SetRotType(CObjectX::ROTTYPE::ROTTYPE_NORMAL);                       //�����̎�ނ�ݒ�
		}
		else
		{//�I�u�W�F�N�g�ɋ󂫂��Ȃ������̂Ŕj������
			delete pRileyAttack;
			pRileyAttack = nullptr;
			return nullptr;
		}
	}
	return pRileyAttack;
}
//===================================================================================================================================================

//==============================================================================
//��������
//==============================================================================
void CRileyAttack::Action03_ExplosionProcess()
{
	int nLife = GetLife();
	int nMaxLife = GetMaxLife();
	float fRatioLife = 1.0f - float(nLife) / float(nMaxLife);
	D3DXVECTOR3& Move = GetMove();
	bool& bPatternFlag = GetPatternFlag();

	int& nPattern = GetPattern();
	int& nPatternTime = GetPatternTime();
	float fRandRot = 0.0f;//�p�x�v�Z�p
	switch (GetSubType())
	{
	case 0:
		SetOriginalColor(D3DXCOLOR(fRatioLife, 0.0f, 0.0f, 1.0f), 2);
		switch (nPattern)
		{
		case 0:
			if (fRatioLife > 0.5f)
			{
				CParticle::SummonChargeParticle(CParticle::TYPE00_NORMAL, 40, 45, 3.0f, 40.0f, 40.0f, 150, 100, 20, true, GetSenterPos(), NORMAL_COL, true);
				nPattern++;
				nPatternTime = 0;
			}
		case 1:
			if (fRatioLife > 0.7f)
			{
				nPattern++;
				nPatternTime = 0;
				SetUseEffect(CEffect::EFFECTTYPE00_NORMAL, true, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), 30, D3DXVECTOR2(50.0f, 50.0f));
			}
			break;
		case 2:
			fRandRot = CCalculation::CalculationXYaim(GetSenterPos(), CGame::GetPlayer()->GetSenterPos());
			Move.x = sinf(fRandRot) * 25.0f;
			Move.y = cosf(fRandRot) * 25.0f;
			nPattern++;
			nPatternTime = 0;
			break;
		default:
			break;
		}
		break;
	case 1:
		SetOriginalColor(D3DXCOLOR(0.0f, fRatioLife, 0.0f, 1.0f), 2);
		switch (nPattern)
		{
		case 0:
			if (fRatioLife > 0.5f)
			{
				CParticle::SummonChargeParticle(CParticle::TYPE00_NORMAL, 40, 45, 3.0f, 40.0f, 40.0f, 150, 100, 20, true, GetSenterPos(), NORMAL_COL, true);
				nPattern++;
				nPatternTime = 0;
			}
		case 1:
			if (fRatioLife > 0.7f)
			{//�F���Z���Ȃ��Ă�����
				nPattern++;
				nPatternTime = 0;
				SetUseEffect(CEffect::EFFECTTYPE00_NORMAL, true, D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), 30, D3DXVECTOR2(50.0f, 50.0f));
			}
			break;
		case 2:
			if (GetSenterPos().x < CGame::GetPlayer()->GetSenterPos().x)
			{
				Move.x = 6.0f;
			}
			else
			{
				Move.x = -6.0f;
			}
			Move.y = -8.0f;
			SetUseReflection(true);
			nPattern++;
			nPatternTime = 0;
			break;
		default:
			break;
		}
		break;
	case 2:
		SetOriginalColor(D3DXCOLOR(0.0f, 0.0f, fRatioLife, 1.0f), 2);
		switch (nPattern)
		{
		case 0:
			if (fRatioLife > 0.5f)
			{
				CParticle::SummonChargeParticle(CParticle::TYPE00_NORMAL, 40, 45, 3.0f, 40.0f, 40.0f, 150, 100, 20, true, GetSenterPos(), NORMAL_COL, true);
				nPattern++;
				nPatternTime = 0;
			}
		case 1:
			if (fRatioLife > 0.7f)
			{//�F���Z���Ȃ��Ă�����
				nPattern++;
				nPatternTime = 0;
			}
			break;
		case 2:
			for (int nCnt = 0; nCnt < 3; nCnt++)
			{
				fRandRot = CCalculation::CalculationRandVecXY();
				CBullet::Create(CBullet::BULLETTYPE_ENEMY, CBullet::BULLETTEX_ENEMY, 140, 10, 30.0f, 30.0f, GetSenterPos(),
					D3DXVECTOR3(sinf(fRandRot) * 4.0f, cosf(fRandRot) * 4.0f, 0.0f));
			}
			nPattern++;
			nPatternTime = 0;
			SetUseDeath(true);
			SetDeath();
			break;
		default:
			break;
		}
		break;
		break;
	default:
		break;
	}
}
//===================================================================================================================================================

//==============================================================================
//�`�揈��
//==============================================================================
void CRileyAttack::SetDeath()
{
	CAttack::SetDeath();
}
//===================================================================================================================================================

//==============================================================================
//�s���R�̏���
//==============================================================================
void CRileyAttack::Action03_Process()
{
	switch (GetAttackType())
	{
	case ATTACKTYPE11_EXPLOSION:
		Action03_ExplosionProcess();
		break;
	default:
		break;
	}
}
//===================================================================================================================================================


//==========================================================�������h���C���U���N���X������=====================================================================================================================

//==============================================================================
//�R���X�g���N�^
//==============================================================================
CDraiyaAttack::CDraiyaAttack()
{

}
//===================================================================================================================================================

//==============================================================================
//�f�X�g���N�^
//==============================================================================
CDraiyaAttack::~CDraiyaAttack()
{

}
//===================================================================================================================================================

//==============================================================================
//����������
//==============================================================================
HRESULT CDraiyaAttack::Init()
{
	CAttack::Init();
	return S_OK;
}
//===================================================================================================================================================

//==============================================================================
//�I������
//==============================================================================
void CDraiyaAttack::Uninit()
{
	CAttack::Uninit();
}
//===================================================================================================================================================

//==============================================================================
//�X�V����
//==============================================================================
void CDraiyaAttack::Update()
{
	CAttack::Update();
}
//===================================================================================================================================================

//==============================================================================
//�`�揈��
//==============================================================================
void CDraiyaAttack::Draw()
{
	CAttack::Draw();
}
//===================================================================================================================================================

//==============================================================================
//���S�t���O�ݒ菈��
//==============================================================================
void CDraiyaAttack::SetDeath()
{
	CAttack::SetDeath();
}
//===================================================================================================================================================

//==============================================================================
//��������
//==============================================================================
CDraiyaAttack* CDraiyaAttack::Create(ATTACKTYPE type, int nDamage, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 Scale, D3DXVECTOR3 Rot, COLLISIONTYPE CollisionType, D3DXVECTOR3 AddRot, bool bCollisionRelease, int nHitStopTime)
{
	CDraiyaAttack* pDraiyaAttack = DBG_NEW CDraiyaAttack;   //�G�𐶐�
	bool bSuccess = pDraiyaAttack->CObject::GetCreateSuccess();
	int nIdx = 0;//���f���̃C���f�b�N�X
	if (bSuccess == true)
	{
		if (pDraiyaAttack != nullptr)
		{
			pDraiyaAttack->SetAttackType(type);  //�U���̎��
			pDraiyaAttack->Init();         //����������
			pDraiyaAttack->SetUseBlockCollision(false);
			pDraiyaAttack->SetHitStopTime(nHitStopTime);          //���������Ƃ��Ɋ��蓖�Ă�q�b�g�X�g�b�v���Ԃ�ݒ肷��
			pDraiyaAttack->SetCollisionRelease(bCollisionRelease);//�����������ɏ������ǂ�����ݒ肷��
			pDraiyaAttack->SetUseInteria(false);//�������g�p���Ȃ�
			pDraiyaAttack->SetMove(move);  //�ړ���
			pDraiyaAttack->SetDoCollision(true);                  //�����蔻������邩�ǂ�����ݒ�
			pDraiyaAttack->SetLife(nLife);                    //�̗͂�ݒ�
			pDraiyaAttack->SetMaxLife(nLife);                 //�ő�̗͂�ݒ�
			pDraiyaAttack->SetAutoSubLife(true);              //�����I�ɑ̗͂����炷���ǂ���
			pDraiyaAttack->SetTargetType(CAttack::TARGETTYPE_PLAYER);//�N�ɓ��Ă邩��ݒ�
			pDraiyaAttack->SetCollisionType(CollisionType);   //�����蔻��̎��
			pDraiyaAttack->SetPower(nDamage);                 //�З�
			pDraiyaAttack->SetUseAddRot(true, AddRot);                 //���Z��������̗�
			pDraiyaAttack->CObjectX::SetPos(pos);             //�I�u�W�F�N�gX�̈ʒu��ݒ�
			pDraiyaAttack->SetRot(Rot);                       //������ݒ�
			pDraiyaAttack->SetPosOld(pos);                                                                                         //1f�O�̈ʒu��ݒ�
			pDraiyaAttack->SetSupportPos(pos);                                                                                     //�ݒu�ʒu
			pDraiyaAttack->CObject::SetType(CObject::TYPE_ATTACK);                                                                  //�I�u�W�F�N�g�̎�ނ����߂�
			pDraiyaAttack->CObjectX::SetScale(Scale);                                                                              //�I�u�W�F�N�g�̊g�嗦
			pDraiyaAttack->SetFormarScale(Scale);                                       //���̊g�嗦��ݒ肷��
			pDraiyaAttack->SetUseDeath(true);                 //���S�t���O�𔭓����邩�ǂ�����ݒ肷��
			nIdx = CManager::GetObjectXInfo()->Regist(CObjectX::OBJECTXTYPE_ATTACK, CAttack::m_ATTACK_FILENAME[(int)(type)]);
			pDraiyaAttack->CObjectX::BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(CObjectX::OBJECTXTYPE_ATTACK, nIdx),
				CManager::GetObjectXInfo()->GetBuffMat(CObjectX::OBJECTXTYPE_ATTACK, nIdx),
				CManager::GetObjectXInfo()->GetdwNumMat(CObjectX::OBJECTXTYPE_ATTACK, nIdx),
				CManager::GetObjectXInfo()->GetTexture(CObjectX::OBJECTXTYPE_ATTACK, nIdx),
				CManager::GetObjectXInfo()->GetColorValue(CObjectX::OBJECTXTYPE_ATTACK, nIdx));                       //���f���������蓖�Ă�

			pDraiyaAttack->CObjectX::SetObjXType(CObjectX::OBJECTXTYPE_ATTACK);                            //�I�u�W�F�N�gX�̃^�C�v��ݒ�
			pDraiyaAttack->CObjectX::SetTypeNum((int)(type));                                             //�I�u�W�F�N�gX���Ƃ̃^�C�v�ԍ���ݒ�
			pDraiyaAttack->SetSize();                                                                     //X�I�u�W�F�N�g�̃T�C�Y��ݒ肷��
			pDraiyaAttack->CObjectX::SetRotType(CObjectX::ROTTYPE::ROTTYPE_NORMAL);                       //�����̎�ނ�ݒ�
		}
		else
		{//�I�u�W�F�N�g�ɋ󂫂��Ȃ������̂Ŕj������
			delete pDraiyaAttack;
			pDraiyaAttack = nullptr;
			return nullptr;
		}
	}
	return pDraiyaAttack;
}
//===================================================================================================================================================

//==========================================================�������K�u���G���U���N���X������=====================================================================================================================

//==============================================================================
//�R���X�g���N�^
//==============================================================================
CGabrielAttack::CGabrielAttack() : m_AttackMove(GABRIELATTACKMOVE_00)
{

}
//===================================================================================================================================================

//==============================================================================
//�f�X�g���N�^
//==============================================================================
CGabrielAttack::~CGabrielAttack()
{

}
//===================================================================================================================================================

//==============================================================================
//����������
//==============================================================================
HRESULT CGabrielAttack::Init()
{
	CAttack::Init();
	return S_OK;
}
//===================================================================================================================================================

//==============================================================================
//�I������
//==============================================================================
void CGabrielAttack::Uninit()
{
	CAttack::Uninit();
}
//===================================================================================================================================================

//==============================================================================
//�X�V����
//==============================================================================
void CGabrielAttack::Update()
{
	CAttack::Update();
}
//===================================================================================================================================================

//==============================================================================
//�`�揈��
//==============================================================================
void CGabrielAttack::Draw()
{
	CAttack::Draw();
}
//===================================================================================================================================================

//==============================================================================
//���S�t���O�ݒ菈��
//==============================================================================
void CGabrielAttack::SetDeath()
{
	CAttack::SetDeath();
}
//===================================================================================================================================================

//==============================================================================
//��������
//==============================================================================
CGabrielAttack* CGabrielAttack::Create(ATTACKTYPE type, int nDamage, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 Scale, D3DXVECTOR3 Rot, COLLISIONTYPE CollisionType, D3DXVECTOR3 AddRot, bool bCollisionRelease, int nHitStopTime)
{
	CGabrielAttack* pGabrielAttack = DBG_NEW CGabrielAttack;   //�G�𐶐�
	bool bSuccess = pGabrielAttack->CObject::GetCreateSuccess();
	int nIdx = 0;//���f���̃C���f�b�N�X
	if (bSuccess == true)
	{
		if (pGabrielAttack != nullptr)
		{
			pGabrielAttack->SetAttackType(type);  //�U���̎��
			pGabrielAttack->Init();         //����������
			pGabrielAttack->SetUseBlockCollision(false);
			pGabrielAttack->SetHitStopTime(nHitStopTime);          //���������Ƃ��Ɋ��蓖�Ă�q�b�g�X�g�b�v���Ԃ�ݒ肷��
			pGabrielAttack->SetCollisionRelease(bCollisionRelease);//�����������ɏ������ǂ�����ݒ肷��
			pGabrielAttack->SetUseInteria(false);//�������g�p���Ȃ�
			pGabrielAttack->SetMove(move);  //�ړ���
			pGabrielAttack->SetDoCollision(true);                  //�����蔻������邩�ǂ�����ݒ�
			pGabrielAttack->SetLife(nLife);                    //�̗͂�ݒ�
			pGabrielAttack->SetMaxLife(nLife);                 //�ő�̗͂�ݒ�
			pGabrielAttack->SetAutoSubLife(true);              //�����I�ɑ̗͂����炷���ǂ���
			pGabrielAttack->SetTargetType(CAttack::TARGETTYPE_PLAYER);//�N�ɓ��Ă邩��ݒ�
			pGabrielAttack->SetCollisionType(CollisionType);   //�����蔻��̎��
			pGabrielAttack->SetPower(nDamage);                 //�З�
			pGabrielAttack->SetUseAddRot(true, AddRot);                 //���Z��������̗�
			pGabrielAttack->CObjectX::SetPos(pos);             //�I�u�W�F�N�gX�̈ʒu��ݒ�
			pGabrielAttack->SetRot(Rot);                       //������ݒ�
			pGabrielAttack->SetPosOld(pos);                                                                                         //1f�O�̈ʒu��ݒ�
			pGabrielAttack->SetSupportPos(pos);                                                                                     //�ݒu�ʒu
			pGabrielAttack->CObject::SetType(CObject::TYPE_ATTACK);                                                                  //�I�u�W�F�N�g�̎�ނ����߂�
			pGabrielAttack->CObjectX::SetScale(Scale);                                                                              //�I�u�W�F�N�g�̊g�嗦
			pGabrielAttack->SetFormarScale(Scale);                                       //���̊g�嗦��ݒ肷��
			pGabrielAttack->SetUseDeath(true);                 //���S�t���O�𔭓����邩�ǂ�����ݒ肷��
			nIdx = CManager::GetObjectXInfo()->Regist(CObjectX::OBJECTXTYPE_ATTACK, CAttack::m_ATTACK_FILENAME[(int)(type)]);
			pGabrielAttack->CObjectX::BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(CObjectX::OBJECTXTYPE_ATTACK, nIdx),
				CManager::GetObjectXInfo()->GetBuffMat(CObjectX::OBJECTXTYPE_ATTACK, nIdx),
				CManager::GetObjectXInfo()->GetdwNumMat(CObjectX::OBJECTXTYPE_ATTACK, nIdx),
				CManager::GetObjectXInfo()->GetTexture(CObjectX::OBJECTXTYPE_ATTACK, nIdx),
				CManager::GetObjectXInfo()->GetColorValue(CObjectX::OBJECTXTYPE_ATTACK, nIdx));                       //���f���������蓖�Ă�

			pGabrielAttack->CObjectX::SetObjXType(CObjectX::OBJECTXTYPE_ATTACK);                            //�I�u�W�F�N�gX�̃^�C�v��ݒ�
			pGabrielAttack->CObjectX::SetTypeNum((int)(type));                                             //�I�u�W�F�N�gX���Ƃ̃^�C�v�ԍ���ݒ�
			pGabrielAttack->SetSize();                                                                     //X�I�u�W�F�N�g�̃T�C�Y��ݒ肷��
			pGabrielAttack->CObjectX::SetRotType(CObjectX::ROTTYPE::ROTTYPE_NORMAL);                       //�����̎�ނ�ݒ�
		}
		else
		{//�I�u�W�F�N�g�ɋ󂫂��Ȃ������̂Ŕj������
			delete pGabrielAttack;
			pGabrielAttack = nullptr;
			return nullptr;
		}
	}
	return pGabrielAttack;
}
//===================================================================================================================================================

//==============================================================================
//�U���P�̑S�Ă̏���
//==============================================================================
void CGabrielAttack::Attack01Process()
{
	//D3DXVECTOR3 Pos = GetPos();

	//switch (GetAttackType())
	//{
	//case ATTACKTYPE01_METEO:
	//	if (Pos.y <= 0.0f)
	//	{
	//		for (int nCnt = 0; nCnt < m_nMAX_SUMMON_METEO_TORNADO; nCnt++)
	//		{
	//			
	//		}
	//	}
	//	break;
	//default:
	//	break;
	//}
}
//===================================================================================================================================================