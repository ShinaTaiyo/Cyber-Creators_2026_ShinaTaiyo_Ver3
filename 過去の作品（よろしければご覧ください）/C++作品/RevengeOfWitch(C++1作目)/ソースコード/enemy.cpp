//=================================================
//
//�V���Q�T���F�G�̍s��AI������[enemy.cpp]
//Author:ShinaTaiyo
//
//=================================================

//==============================
//�C���N���[�h
//==============================
#include "enemy.h"
#include "manager.h"
#include "game.h"
#include "edit.h"
#include "camera.h"
#include "bullet.h"
#include "attack.h"
#include "particle.h"
#include "main.h"
#include "damage.h"
#include "sound.h"
#include "input.h"
#include "texture.h"
#include "debugtext.h"
#include "manager.h"
#include "objectXInfo.h"
#include "stagemanager.h"
#include "calculation.h"
//=================================================

//==============================
//�ÓI�����o������
//==============================
const int CEnemy::m_nMAXENEMY01BULLET = 8;       //�G�O�P�̒e�̔��ː�
int CEnemy::m_nNumFile = CEnemy::ENEMYTYPE_MAX;  //�t�@�C���̐�
const char* CEnemy::m_ENEMY_FILENAME[CEnemy::ENEMYTYPE_MAX] =
{ "data\\MODEL\\Enemy\\00_noobSlime.x",
"data\\MODEL\\Enemy\\01_angrySlime.x" ,
"data\\MODEL\\Enemy\\02_swordSlime.x" ,
"data\\MODEL\\Enemy\\03_hammerSlime.x",
"data\\MODEL\\Enemy\\04_HugeSlime.x",
"data\\MODEL\\Enemy\\05_ThunderSlime.x",
"data\\MODEL\\Enemy\\06_ScanningSlime.x",
"data\\MODEL\\Enemy\\07_NeedleSlime.x",
"data\\MODEL\\Enemy\\08_engelSlime.x",
"data\\MODEL\\Enemy\\09_darkSlime.x",
"data\\MODEL\\Enemy\\10_RainbowSlime.x",
}; //�G�̃��f���̃p�X
//=================================================

//==============================
//�R���X�g���N�^
//==============================
CEnemy::CEnemy()
{

}
//=================================================

//==============================
//�f�X�g���N�^
//==============================
CEnemy::~CEnemy()
{

}
//=================================================

//==============================
//����������
//==============================
HRESULT CEnemy::Init()
{
	//===========================
	//�I�u�W�F�N�gX������
	//===========================
	if (FAILED(CObjectX::Init()))
	{
		return E_FAIL;
	}
	//====================================
	
	//===========================
	//�N���X�̕ϐ�������
	//===========================
	m_nCntTime = 0;          //�o�����Ă���̎��Ԃ𑪂�
	m_Aim = NULL_VECTOR3;    //�x�N�g��
	m_fTotalAim = 0.0f;      //�����x�N�g��
	m_bUse = false;          //�g�p���
	m_bIsJumping = false;    //�W�����v�����ǂ���
	m_bIsWalling = false;    //�ǂɓ������Ă��邩�ǂ���
	m_bAttack = false;       //�U�������邩�ǂ���

	m_nPattern = 0;          //�p�^�[���ԍ�
	m_nSubPattern = 0;       //�T�u�̃p�^�[��
	m_nPatternTime = 0;      //�p�^�[���ɓ����Ă���̎���
	m_bPattern = false;      //�p�^�[�������s���邩�ǂ���
	m_bAction = false;       //�s�������邩�ǂ���
	m_nCntAction = 0;        //�A�N�V�����񐔃J�E���g�p

	m_fLength = 0.0f;        //�v���C���[�Ƃ̋���

	m_fGravityPower = 1.0f;  //�d�͂̑傫��

	m_fShotRot = 0.0f;       //�o���b�g�Ȃǂ𔭎˂������

	m_bSpeedSwitch = false;  //������ς���X�C�b�`

	m_pLandingObj = nullptr;//����Ă���I�u�W�F�N�g����p
	//==============================================================================================

	//====================================
	//�G��AI�p���f��
	//====================================
	m_pMoveRangeAI = nullptr;
	//==============================================================================================

	return S_OK;
}
//====================================================================================================================================================================

//==============================
//�I������
//==============================
void CEnemy::Uninit()
{
	CObjectX::Uninit();
	//SetDeath();
}
//=================================================

//==============================
//�X�V����
//==============================
void CEnemy::Update()
{
	//==================================
	//�I�u�W�F�N�gX��������擾
	//==================================
	D3DXVECTOR3 &pos = GetPos();                                  //���f���̈ʒu�̎擾
	D3DXVECTOR3 &PosOld = GetPosOld();                            //1f�O�̈ʒu��ݒ�
	D3DXVECTOR3& Move = GetMove();                                //�ړ��ʂ��擾
	D3DXVECTOR3 &Rot = GetRot();                                  //���f���̌����̎擾
	D3DXVECTOR3 Size = GetSize();                                 //�T�C�Y���擾
	bool& bIsLanding = GetLanding();                              //�n�ʂɂ��邩�ǂ���
	bool& bUseGravity = GetUseGravity();                          //
	int& nLife = GetLife();
	//===========================================================

	if (CScene::GetMode() == CScene::MODE_GAME)
	{
		GravityProcess();//�d�͂̏���
	}

	//==================================
	//�G���Ƃ̍U���p�^�[��
	//==================================
	m_nCntTime++;//�o�����Ă���̎��Ԃ𑪂�

	if (CScene::GetMode() == CScene::MODE_GAME)
	{
		CalculationLength();//�������v�Z����

	    //==========================================
	    //�s������
	    //==========================================
		if (m_fLength < 500.0f)
		{
			m_bAction = true;
		}
		else
		{
			m_bAction = false;
			//�s���p�^�[���̃��Z�b�g
			ResetPattern();
			Move.x = 0.0f;
			Rot = NULL_VECTOR3;
			if (m_type == ENEMYTYPE08_ENGELSLIME)
			{
				Move.y = 0.0f;
			}
		}
		//===========================================================================

		if (m_bAction == true)
		{
			switch (m_type)
			{
			case ENEMYTYPE00_SLIME://�X���C��
				Enemy00Pattern();
				break;
			case ENEMYTYPE01_FIRESLIME://�t�@�C�A�X���C��
				Enemy01Pattern();
				break;
			case ENEMYTYPE02_SWORDSLIME://�\�[�h�X���C��
				Enemy02Pattern();
				break;
			case ENEMYTYPE03_HAMMERSLIME://�n���}�[�X���C��
				Enemy03Pattern();
				break;
			case ENEMYTYPE04_ORANGESLIME://�I�����W�X���C��
				Enemy04Pattern();
				break;
			case ENEMYTYPE05_THUNDERSLIME://�T���_�[�X���C��
				Enemy05Pattern();
				break;
			case ENEMYTYPE06_SCANINGSLIME://�X�L���j���O�X���C��
				Enemy06Pattern();
				break;
			case ENEMYTYPE07_NEEDLESLIME://�j�[�h���X���C��
				Enemy07Pattern();
				break;
			case ENEMYTYPE08_ENGELSLIME://�G���W�F���X���C��
				Enemy08Pattern();
				break;
			case ENEMYTYPE09_DARKSLIME://�_�[�N�X���C��
				Enemy09Pattern();
				break;
			case ENEMYTYPE10_RAINBOWSLIME://���C���{�[�X���C��
				Enemy10Pattern();
				break;
			default:
				break;
			}
		}
	}
	//===========================================================

	//==================================
	//X�I�u�W�F�N�g�X�V
	//==================================
	CObjectX::Update();
	//===========================================================
	
	//==================================
	//�G�̈ʒu��������
	//==================================
	AdjustEnemyPos();
	//===========================================================


	//==================================
	//�����蔻��
	//==================================
	Collision();
	//===========================================================

	if (nLife < 1)
	{
		m_bUse = false;
	}

    //==================================
	//�G�̔j��
	//==================================
	if (m_bUse == false)
	{
		SetDeath();
	}
	//===========================================================
}
//==================================================================================================================

//==============================
//�`�揈��
//==============================
void CEnemy::Draw()
{
	CObjectX::Draw();

	if (CScene::GetMode() == CScene::MODE_EDIT)
	{
		if (m_pMoveRangeAI != nullptr)//�X�e�[�W�}�l�[�W���[�ɑI�΂�Ă�����
		{
			if (GetStageManagerChoose() == true)
			{
				m_pMoveRangeAI->SetUseDraw(true);
			}
			else
			{
				m_pMoveRangeAI->SetUseDraw(false);
			}
		}
	}
}
//==================================================================================================================

//==============================
//�_���[�W��^���鏈��
//==============================
void CEnemy::SetDamage(int nDamage, int nHitStopTime)
{
	bool& bHitStop = GetHitStop(); //�q�b�g�X�g�b�v��Ԃ��ǂ������擾
	int& nLife = GetLife();        //���݂̗̑͂��擾
	int& nHitStopTime2 = GetHitStopTime();//�q�b�g�X�g�b�v���Ԃ��擾

	if (bHitStop == false)
	{//�q�b�g�X�g�b�v��Ԃ���Ȃ����
		if (nHitStopTime > 0)
		{
			bHitStop = true;              //�q�b�g�X�g�b�v��Ԃɂ���
			nHitStopTime2 = nHitStopTime; //�q�b�g�X�g�b�v����
		}
		nLife -= nDamage;

		SetColor(COLORTYPE_RED, 10);
		CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_DAMAGE_000);
		CDamage::Create(nDamage, GetPos(), NORMAL_COL, 40.0f, 40.0f);
	}
}
//==================================================================================================================

//================================================================
//���S�t���O��ݒ�
//================================================================
void CEnemy::SetDeath()
{
	if (m_pMoveRangeAI != nullptr)
	{
		m_pMoveRangeAI->SetUseDeath(true);
		m_pMoveRangeAI->SetDeath();
		m_pMoveRangeAI = nullptr;
	}

	CObject::SetDeath();
}
//==================================================================================================================

//===============================
//�G�̐�������
//===============================
CEnemy* CEnemy::Create(ENEMYTYPE type, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 Scale,
	D3DXVECTOR3 MoveLemgthAIPos, D3DXVECTOR3 MoveLengthAIRot, D3DXVECTOR3 MoveLengthAIScale)
{
	CEnemy* pEnemy = DBG_NEW CEnemy;   //�G�𐶐�
	bool bSuccess = pEnemy->CObject::GetCreateSuccess();
	int nIdx = 0;//���f���̃C���f�b�N�X
	if (bSuccess == true)
	{
		if (pEnemy != nullptr)
		{
			pEnemy->Init();         //����������
			pEnemy->SetUseDeath(true);                                                   //���S�t���O�𔭓����邩�ǂ�����ݒ肷��
			pEnemy->m_bUse = true;  //�g�p���
			pEnemy->m_type = type;  //�G�̎��
			pEnemy->SetLife(nLife); //�G�̗̑�
			pEnemy->SetMaxLife(nLife);//�G�̍ő�̗�
			pEnemy->SetAutoSubLife(false);//�����I�ɑ̗͂����炷���ǂ���
			pEnemy->CObjectX::SetPos(pos);                                                                                  //�I�u�W�F�N�g�Q�c�̈ʒu��ݒ�
			pEnemy->SetPosOld(pos);               //1f�O�̈ʒu��ݒ�
			pEnemy->SetUseGravity(true);
			pEnemy->SetSupportPos(pos);           //�ݒu�ʒu
			pEnemy->CObject::SetType(CObject::TYPE_ENEMY);                                                                  //�I�u�W�F�N�g�̎�ނ����߂�
			pEnemy->CObjectX::SetScale(Scale);                                                                              //�I�u�W�F�N�g�̊g�嗦
			pEnemy->SetFormarScale(Scale);                                                                                  //���̊g�嗦��ݒ肷��
			nIdx = CManager::GetObjectXInfo()->Regist(CObjectX::OBJECTXTYPE_ENEMY, m_ENEMY_FILENAME[(int)(type)]);
			pEnemy->CObjectX::BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(CObjectX::OBJECTXTYPE_ENEMY,nIdx),
				CManager::GetObjectXInfo()->GetBuffMat(CObjectX::OBJECTXTYPE_ENEMY,nIdx),
				CManager::GetObjectXInfo()->GetdwNumMat(CObjectX::OBJECTXTYPE_ENEMY,nIdx),
				CManager::GetObjectXInfo()->GetTexture(CObjectX::OBJECTXTYPE_ENEMY,nIdx),
				CManager::GetObjectXInfo()->GetColorValue(CObjectX::OBJECTXTYPE_ENEMY,nIdx));                       //���f���������蓖�Ă�

			pEnemy->CObjectX::SetObjXType(CObjectX::OBJECTXTYPE_ENEMY);                            //�I�u�W�F�N�gX�̃^�C�v��ݒ�
			pEnemy->CObjectX::SetTypeNum((int)(type));                                             //�I�u�W�F�N�gX���Ƃ̃^�C�v�ԍ���ݒ�
			pEnemy->SetSize();                                                                     //X�I�u�W�F�N�g�̃T�C�Y��ݒ肷��
			pEnemy->CObjectX::SetRotType(CObjectX::ROTTYPE::ROTTYPE_NORMAL);                       //�����̎�ނ�ݒ�
			pEnemy->SetManagerType((int)(CStageManager::MANAGEROBJECT_ENEMY));        //�}�l�[�W���[�ŌĂяo�����̎�ނ�ݒ�

			//===========================================
			//AI���f���̐���
			//===========================================
			pEnemy->m_pMoveRangeAI = CAIModel::Create(CAIModel::AIMODELTYPE_SQUARE, MoveLemgthAIPos, MoveLengthAIRot, MoveLengthAIScale);
			//======================================================================================================================================
		}
		else
		{//�I�u�W�F�N�g�ɋ󂫂��Ȃ������̂Ŕj������
			delete pEnemy;
			pEnemy = nullptr;
			return nullptr;
		}
	}
	return pEnemy;
}
//===================================================================================================================================

//=======================================================
//�G�O�̃p�^�[��
//=======================================================
void CEnemy::Enemy00Pattern()
{
	D3DXVECTOR3& Pos = GetPos();                                  //���f���̈ʒu�̎擾
	D3DXVECTOR3& PosOld = GetPosOld();                            //1f�O�̈ʒu��ݒ�
	D3DXVECTOR3& Move = GetMove();                                //�ړ��ʂ��擾
	D3DXVECTOR3& Rot = GetRot();                                   //���f���̌����̎擾
	D3DXVECTOR3 Size = GetSize();                                 //�T�C�Y���擾
	m_fTotalAim = float(rand() % 628) / 100;//�R�U�O�x�̃x�N�g���̗���
	int nAIPattern = 0;                                           //AI�̖��������闐��
	int nPattern = 0;                                             //�p�^�[��

	//==========================================
	//�s���J�n
	//==========================================
	if (m_bAction == true)
	{
		if (m_bPattern == false)
		{
			if (m_fLength >= m_fENEMY00_NEARACTIONPATTERNLENGTH)
			{//�������������
				nAIPattern = 0;
				nPattern = rand() % 5;

				if (nPattern == 0)
				{//��m���ōU��
					m_nPattern = 1;
				}
				else if (nPattern >= 1 && nPattern <= 3)
				{//���m���Őڋ�
					m_nPattern = 0;
				}
				else
				{//��m���ŃW�����v
					m_nPattern = 2;
				}
			}
			else
			{//�������߂����
				nAIPattern = 1;
				nPattern = rand() % 6;

				if (nPattern >= 0 && nPattern <= 2)
				{//���m���ōU��
					m_nPattern = 1;
				}
				else if (nPattern >= 3 && nPattern <= 4)
				{//���m���ŃW�����v
					m_nPattern = 2;
				}
				else
				{//��m���Őڋ�
					m_nPattern = 0;
				}
			}
			m_bPattern = true;
		}

		if (m_bPattern == true)
		{
			m_nPatternTime++;
			switch (m_nPattern)
			{
			case 0:
				//�ڋ�
				if (Pos.x <= CGame::GetPlayer()->GetPos().x)
				{
					Move.x = 0.5f;
				}
				else if (Pos.x >= CGame::GetPlayer()->GetPos().x - 20.0f && Pos.x <= CGame::GetPlayer()->GetPos().x + 20.0f)
				{
					Move.x = 0.0f;
				}
				else
				{
					Move.x = -0.5f;
				}

				if (m_nPatternTime >= 70)
				{
					//�s���p�^�[���̃��Z�b�g
					ResetPattern();
				}
				break;
			case 1:
				//�U��
				Move.x = 0.0f;//�U�����͓����Ȃ�

				//if (m_nPatternTime == 1)
				//{
				//	CParticle::SummonParticle(CParticle::TYPE00_NORMAL, 50, 20, 40.0f, 40.0f, 200, 10, false, Pos, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), true);
				//}

				//if (m_nPatternTime % 25 == 0 && m_nPatternTime <= 50)
				//{//�o���b�g����
				//	CBullet::Create(CBullet::BULLETTYPE_ENEMY, CBullet::BULLETTEX_ENEMY, 300, 6, 20.0f, 20.0f, Pos,
				//		D3DXVECTOR3(sinf(m_fTotalAim) * 3.0f, cosf(m_fTotalAim) * 3.0f, 0.0f));
				//}

				if (m_nPatternTime >= 80)
				{
					//�s���p�^�[���̃��Z�b�g
					ResetPattern();
				}
				break;
			case 2:
				//�W�����v
				if (m_nPatternTime == 1)
				{
					SetUseInteria(false);
					if (Pos.x <= CGame::GetPlayer()->GetPos().x)
					{//�v���C���[���E���ɂ���
						Move.x = 1.0f;
						Move.y = 4.0f;
					}
					else
					{//�v���C���[�������ɂ���
						Move.x = -1.0f;
						Move.y = 4.0f;
					}
				}

				if (m_nPatternTime >= 45)
				{
					SetUseInteria(true);
					//�s���p�^�[���̃��Z�b�g
					ResetPattern();
				}
				break;
			default:
				break;
			}
		}
	}
	//===========================================================================

}
//===================================================================================================================================

//====================================================================
//�G�Ƃ̓����蔻��
//====================================================================
void CEnemy::Collision()
{

}
//===================================================================================================================================

//====================================================================
//�v���C���[�Ƃ̋������v�Z����
//====================================================================
void CEnemy::CalculationLength()
{
	D3DXVECTOR3 Pos = GetPos();//�ʒu���擾

	//�����v�Z�J�n
	m_fLength = sqrtf(powf(CGame::GetPlayer()->GetPos().x - Pos.x, 2) + powf(CGame::GetPlayer()->GetPos().y - Pos.y, 2) + 
		powf(CGame::GetPlayer()->GetPos().z - Pos.z, 2));
}
//===================================================================================================================================

//======================================================================
//�d�͂̏���
//======================================================================
void CEnemy::GravityProcess()
{
	D3DXVECTOR3& Pos = GetPos();                                  //���f���̈ʒu�̎擾
	D3DXVECTOR3& PosOld = GetPosOld();                            //1f�O�̈ʒu��ݒ�
	D3DXVECTOR3& Move = GetMove();                                //�ړ��ʂ��擾
	bool& bIsLanding = GetLanding();                              //�n�ʂɂ��邩�ǂ������擾
	bool& bUseGravity = GetUseGravity();                          //�d�͂��g�p���邩�ǂ������擾
	if (CScene::GetMode() == CScene::MODE_GAME)
	{
		if (bIsLanding == true)
		{
			Move.y = 0.0f;
		}
		if (bUseGravity == true)
		{
			Move.y += -m_fGravityPower;
		}
	}
}
//===================================================================================================================================

//=================================================================
//�G�P�̃p�^�[��
//=================================================================
void CEnemy::Enemy01Pattern()
{
	D3DXVECTOR3& Pos = GetPos();                                  //���f���̈ʒu�̎擾
	D3DXVECTOR3& PosOld = GetPosOld();                            //1f�O�̈ʒu��ݒ�
	D3DXVECTOR3& Move = GetMove();                                //�ړ��ʂ��擾
	D3DXVECTOR3& Rot = GetRot();                                   //���f���̌����̎擾
	D3DXVECTOR3 Size = GetSize();                                 //�T�C�Y���擾
	m_fTotalAim = float(rand() % 628) / 100;//�R�U�O�x�̃x�N�g���̗���
	int nAIPattern = 0;                                           //AI�̖��������闐��
	int nPattern = 0;                                             //�p�^�[��
	float fAddRot = float(rand() % 100 - 50) / 100;               //���Z�������
	CBullet* pBullet = nullptr;                                   //�o���b�g�ւ̃|�C���^
	//==========================================
	//�s���J�n
	//==========================================
	if (m_bAction == true)
	{
		if (m_bPattern == false)
		{
			//=============================
			//AI
			//=============================
			if (m_fLength >= 180.0f)
			{//�������������
				nAIPattern = 0;
				nPattern = rand() % 5;

				if (nPattern >= 0 && nPattern <= 1)
				{//���m���ŋ}�ڋ�
					m_nPattern = 0;
				}
				else if (nPattern >= 2 && nPattern <= 3)
				{//���m���ŉ~�`�e�U�e
					m_nPattern = 1;
				}
				else
				{//��m���Ńt�@�C�A�u���X
					m_nPattern = 2;
				}
			}
			else
			{//�������߂����
				nAIPattern = 1;
				nPattern = rand() % 5;

				if (nPattern == 0)
				{//��m���ŋ}�ڋ�
					m_nPattern = 0;
				}
				else if (nPattern == 1)
				{//��m���ŉ~�`�e�U�e
					m_nPattern = 1;
				}
				else
				{//���m���Ńt�@�C�A�u���X
					m_nPattern = 2;
				}
			}
			//================================================================================
			m_bPattern = true;
		}

		if (m_bPattern == true)
		{
			m_nPatternTime++;
			switch (m_nPattern)
			{
			case 0:
				//�}�ڋ�
				if (Pos.x <= CGame::GetPlayer()->GetPos().x)
				{
					Move.x = m_fENEMY01_NORMALSPEED;
				}
				else if (Pos.x >= CGame::GetPlayer()->GetPos().x - 20.0f && Pos.x <= CGame::GetPlayer()->GetPos().x + 20.0f)
				{
					Move.x = 0.0f;
				}
				else
				{
					Move.x = -m_fENEMY01_NORMALSPEED;
				}

				if (m_nPatternTime >= m_nENEMY01_ACTION0_FRAME)
				{
					//�s���p�^�[���̃��Z�b�g
					ResetPattern();
				}
				break;
			case 1:
				//�~�`�g�U�e
				Move.x = 0.0f;
				if (m_nPatternTime == 1)
				{
					CParticle::SummonParticle(CParticle::TYPE00_NORMAL, 50, 20, 40.0f, 40.0f, 200, 10, false, Pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), true);
					for (int nCnt = 0; nCnt < 4; nCnt++)
					{
						float fAngle = (2.0f / 4.0f) * (float)(nCnt);//�������𕪊����闐��
						CUi3D::Create(CUi3D::UI3DTYPE01_WARNING, 60, 60.0f, 60.0f, GetSenterPos(), D3DXVECTOR3(sinf(D3DX_PI * fAngle) * 20.0f, cosf(D3DX_PI * fAngle) * 20.0f, 0.0f));
					}
				}

					if (m_nPatternTime == 50)
					{
						for (int nCnt = 0; nCnt < 4; nCnt++)
						{
							float fAngle = (2.0f / 4.0f) * (float)(nCnt);//�������𕪊����闐��
							CBullet::Create(CBullet::BULLETTYPE_ENEMY, CBullet::BULLETTEX_ENEMY, 60, 8, 20.0f, 20.0f, Pos,
								D3DXVECTOR3(sinf(D3DX_PI * fAngle) * 5.0f, cosf(D3DX_PI * fAngle) * 5.0f, 0.0f));
						}
					}

					if (m_nPatternTime >= 90)
					{
						//�s���p�^�[���̃��Z�b�g
						ResetPattern();
					}
					break;
			case 2:
				//�t�@�C�A�u���X
				if (m_nPatternTime == 1)
				{
					CParticle::SummonParticle(CParticle::TYPE00_NORMAL, 50, 20, 40.0f, 40.0f, 200, 10, false, Pos, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), true);
				}

				//������������
				if (Pos.x <= CGame::GetPlayer()->GetPos().x)
				{//�v���C���[���E�ɂ����
					m_nPattern = 3;
				}
				else
				{//�v���C���[�����ɂ����
					m_nPattern = 4;
				}
				break;
			case 3:
				Move.x = 1.0f;
				if (m_nPatternTime == 5)
				{
					CUi3D::Create(CUi3D::UI3DTYPE01_WARNING, 60, 120.0f, 120.0f, GetSenterPos() + D3DXVECTOR3(sinf(D3DX_PI * 0.5f) * 60.0f, cosf(D3DX_PI * 0.5f) * 60.0f, 0.0f), NULL_VECTOR3);
				}
				if (m_nPatternTime >= 40 && m_nPatternTime <= 90)
				{
					pBullet = CBullet::Create(CBullet::BULLETTYPE_ENEMY, CBullet::BULLETTEX_ENEMY, 30, 8, 20.0f, 20.0f, Pos,
						D3DXVECTOR3(sinf(D3DX_PI * 0.5f + fAddRot) * 5.0f, cosf(D3DX_PI * 0.5f + fAddRot) * 5.0f, 0.0f));
					//�e�Ɋg��ʂ�t�^
					pBullet->SetScaling(0.02f);
				}

				if (m_nPatternTime >= 120)
				{
					//�s���p�^�[���̃��Z�b�g
					ResetPattern();
				}

				break;
			case 4:
				Move.x = -1.0f;
				if (m_nPatternTime == 5)
				{
					CUi3D::Create(CUi3D::UI3DTYPE01_WARNING, 60, 120.0f, 120.0f, GetSenterPos() + D3DXVECTOR3(sinf(D3DX_PI * 0.5f) * -60.0f, cosf(D3DX_PI * 0.5f) * -60.0f, 0.0f), NULL_VECTOR3);
				}
				if (m_nPatternTime >= 40 && m_nPatternTime <= 90)
				{
					pBullet = CBullet::Create(CBullet::BULLETTYPE_ENEMY, CBullet::BULLETTEX_ENEMY, 30, 8, 20.0f, 20.0f, Pos,
						D3DXVECTOR3(sinf(D3DX_PI * 0.5f + fAddRot) * -5.0f, cosf(D3DX_PI * 0.5f + fAddRot) * -5.0f, 0.0f));
					//�e�Ɋg��ʂ�t�^
					pBullet->SetScaling(0.02f);
				}
				if (m_nPatternTime >= 120)
				{
					//�s���p�^�[���̃��Z�b�g
					ResetPattern();
				}
				break;
			default:
				break;
				}
			}

	}
	//===========================================================================
}
//===================================================================================================================================

//===================================================================
//�G�Q�̃p�^�[��
//===================================================================
void CEnemy::Enemy02Pattern()
{
	D3DXVECTOR3& Pos = GetPos();                                  //���f���̈ʒu�̎擾
	D3DXVECTOR3& PosOld = GetPosOld();                            //1f�O�̈ʒu��ݒ�
	D3DXVECTOR3& Move = GetMove();                                //�ړ��ʂ��擾
	D3DXVECTOR3& Rot = GetRot();                                   //���f���̌����̎擾
	D3DXVECTOR3 Size = GetSize();                                 //�T�C�Y���擾
	m_fTotalAim = float(rand() % 628) / 100;                      //�R�U�O�x�̃x�N�g���̗���
	int nPattern = 0;                                             //�p�^�[��
	float fAddSpeed = float(rand() % 40 + 1) / 10;                //�����̕␳
	float fAddRot = float(rand() % 100 - 50) / 100;               //���Z�������
	CBullet* pBullet = nullptr;                                   //�o���b�g�ւ̃|�C���^

	float fVXaim = 0.0f;//X�����x�N�g��
	float fVYaim = 0.0f;//Y�����x�N�g��
	float fVLaim = 0.0f;//�����x�N�g��
	bool& bIsLanding = GetLanding();                              //�n�ʂɂ��邩�ǂ������擾

	int nRand = 0;      //����

	CAttackEnemy* pAttackEnemy = nullptr;//�G�̍U���擾�p

	float fAddAim = 0.0f;

	fVXaim = CGame::GetPlayer()->GetPos().x - Pos.x;
	fVYaim = (CGame::GetPlayer()->GetPos().y + CGame::GetPlayer()->GetSize().y * 0.5f) - Pos.y;//�v���C���[�̒��S��_��
	fVLaim = (float)(atan2(fVXaim, fVYaim));


	//==========================================
	//�s���J�n
	//==========================================
	if (m_bAction == true)
	{
		if (m_bPattern == false)
		{
			//=============================
			//AI
			//=============================
			if (m_fLength >= 220.0f)
			{//�������������
				nPattern = rand() % 5;

				if (nPattern >= 0 && nPattern <= 1)
				{//���m���Ŕ���������
					m_nPattern = 0;
				}
				else if (nPattern == 2)
				{//��m���Ŏ��@�_���e
					m_nPattern = 1;
				}
				else
				{//���m���ŃT�[�x���J�b�^�[
					m_nPattern = 2;
				}
			}
			else
			{//�������߂����
				nPattern = rand() % 6;

				if (nPattern >= 0 && nPattern <= 1)
				{//���m���Ŕ���������
					m_nPattern = 0;
				}
				else if (nPattern >= 2 && nPattern <= 4)
				{//���m���Ŏ��@�_��
					m_nPattern = 1;
				}
				else
				{//��m���ŃT�[�x���J�b�^�[
					m_nPattern = 2;
				}
			}
			//================================================================================
			m_bPattern = true;
		}

		if (m_bPattern == true)
		{
			m_nPatternTime++;
			switch (m_nPattern)
			{
			case 0://����������
				if (m_nPatternTime == 1)
				{
					CUi3D::Create(CUi3D::UI3DTYPE01_WARNING, 50, 100.0f, 100.0f, Pos, NULL_VECTOR3);
				}
				if (m_nPatternTime <= 100 && m_nPatternTime >= 40)
				{
					switch (m_nSubPattern)
					{
					case 0:
						if (bIsLanding == true)
						{
							SetUseInteria(false);//�������g�p���Ȃ�
							Move.x = 2.0f + fAddSpeed;
							if (Pos.x <= CGame::GetPlayer()->GetPos().x)
							{//�v���C���[���E���ɂ�����
								Move.x += 2.0f;
							}
							else
							{//�v���C���[�������ɂ�����
								Move.x -= 2.0f;
							}
							Move.y = m_fENEMY02_JUMPPOWER;
							m_nSubPattern++;
						}
						break;
					case 1:
						if (bIsLanding == true)
						{
							SetUseInteria(false);//�������g�p���Ȃ�
							Move.x = -2.0f - fAddSpeed;
							if (Pos.x <= CGame::GetPlayer()->GetPos().x)
							{//�v���C���[���E���ɂ�����
								Move.x += 2.0f;
							}
							else
							{//�v���C���[�������ɂ�����
								Move.x -= 2.0f;
							}
							Move.y = m_fENEMY02_JUMPPOWER;
							m_nSubPattern = 0;
						}
						break;
					default:
						break;
					}
				}
				else
				{//20f�������~�߂�
					Move.x = 0.0f;
				}

				if (m_nPatternTime >= 120)
				{
					SetUseInteria(true);//�������g�p����
					//�s���p�^�[���̃��Z�b�g
					ResetPattern();
				}
				break;
			case 1://���@�_��

				if (m_nPatternTime == 1)
				{
					m_nSubPattern = rand() % 2;
					CParticle::SummonParticle(CParticle::TYPE00_NORMAL, 50, 20, 40.0f, 40.0f, 200, 10, false, Pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), true);
				}

				switch (m_nSubPattern)
				{
				case 0://�E����X�^�[�g
					if (m_nPatternTime == 20)
					{
						SetInertia(0.1f);
						Move.x = m_fENEMY02_ACTION1_SPEED + fAddSpeed;
					}
					else if (m_nPatternTime == 50)
					{
						Move.x = -m_fENEMY02_ACTION1_SPEED - fAddSpeed;
					}
					else if (m_nPatternTime == 80)
					{
						Move.x = m_fENEMY02_ACTION1_SPEED / 2;
					}
					else if (m_nPatternTime == 110)
					{
						Move.y = m_fENEMY02_ACTION1_SPEED * 2;
					}
					break;
				case 1://������X�^�[�g
					if (m_nPatternTime == 20)
					{
						SetInertia(0.1f);
						Move.x = -m_fENEMY02_ACTION1_SPEED + fAddSpeed;
					}
					else if (m_nPatternTime == 50)
					{
						Move.x = m_fENEMY02_ACTION1_SPEED - fAddSpeed;
					}
					else if (m_nPatternTime == 80)
					{
						Move.x = -m_fENEMY02_ACTION1_SPEED / 2;
					}
					else if (m_nPatternTime == 110)
					{
						Move.y = m_fENEMY02_ACTION1_SPEED * 2;
					}
					break;
				default:
					break;
				}
				if (m_nPatternTime % 30 == 0 && m_nPatternTime <= 120)
				{
					nRand = rand() % 2;

					if (nRand == 0 && m_nCntAction < 2)
					{//�ő�Q�񌂂�
						m_nCntAction++;
						pBullet = CBullet::Create(CBullet::BULLETTYPE_ENEMY, CBullet::BULLETTEX_ENEMY, 300, 10, 20.0f, 20.0f, Pos,
							D3DXVECTOR3(sinf(fVLaim) * 5.0f, cosf(fVLaim) * 5.0f, 0.0f));

						pBullet->SetUseEffect(30, D3DXVECTOR2(40.0f, 40.0f), D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f));//�G�t�F�N�g���g�p����
					}
				}

				if (m_nPatternTime >= 150)
				{//�p�^�[�����Z�b�g
					ResetPattern();
				}

				break;
			case 2://�T�[�x���J�b�^�[
				if (m_nPatternTime == 15)
				{
					CParticle::SummonParticle(CParticle::TYPE00_NORMAL, 50, 20, 40.0f, 40.0f, 200, 10, false, Pos, D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), true);
					CUi3D::Create(CUi3D::UI3DTYPE01_WARNING, 60, 70.0f, 70.0f,CGame::GetPlayer()->GetPos(), NULL_VECTOR3);
				}

				if (m_nPatternTime == 81)
				{
					nRand = rand() % 2;

					if (nRand == 0)
					{//�m���ŃW�����v
						Move.y = 20.0f;
					}
				}

				if (m_nPatternTime == 95)
				{
					pAttackEnemy = CAttackEnemy::Create(CAttack::ATTACKTYPE08_VACUUMBLADE, 10, 120, Pos, D3DXVECTOR3(sinf(fVLaim) * 1.0f, cosf(fVLaim) * 1.0f, 0.0f), ONE_VECTOR3 * 2.0f,
						D3DXVECTOR3(0.0f, 0.0f,-fVLaim + D3DX_PI), CAttack::COLLISIONTYPE02_TRUERECTANGLEXY,NULL_VECTOR3, true, 20);

					pAttackEnemy->SetUseMultiSpeed(true,ONE_VECTOR3 * 1.05f);//�����x�ݒ�
				}

				if (m_nPatternTime == 100)
				{
					ResetPattern();
				}
				break;
			default:
				break;
			}
		}
	}
	//===========================================================================

}
//===================================================================================================================================

//===================================================================
//�s���p�^�[���̃��Z�b�g
//===================================================================
void CEnemy::ResetPattern()
{
	m_bPattern = false;
	m_nPattern = 0;
	m_nPatternTime = 0;
	m_nSubPattern = 0;
	m_nCntAction = 0;
}
//===================================================================================================================================

//===================================================================
//�G�R�̃p�^�[��
//===================================================================
void CEnemy::Enemy03Pattern()
{
	D3DXVECTOR3& Pos = GetPos();                                  //���f���̈ʒu�̎擾
	D3DXVECTOR3& PosOld = GetPosOld();                            //1f�O�̈ʒu��ݒ�
	D3DXVECTOR3& Move = GetMove();                                //�ړ��ʂ��擾
	D3DXVECTOR3& Rot = GetRot();                                   //���f���̌����̎擾
	D3DXVECTOR3 Size = GetSize();                                 //�T�C�Y���擾
	m_fTotalAim = float(rand() % 628) / 100;                      //�R�U�O�x�̃x�N�g���̗���
	int nPattern = 0;                                             //�p�^�[��
	float fAddSpeed = float(rand() % 40 + 1) / 10;                //�����̕␳
	float fRandRot = float(rand() % 628 + 1) / 100;               //��������
	CBullet* pBullet = nullptr;                                   //�o���b�g�ւ̃|�C���^
	CBullet* pParentBullet = nullptr;
	float fVXaim = 0.0f;//X�����x�N�g��
	float fVYaim = 0.0f;//Y�����x�N�g��
	float fVLaim = 0.0f;//�����x�N�g��
	D3DXMATRIX mtxWorld = GetMatrixWorld();
	CAttackEnemy* pAttackEnemy = nullptr;                         //�G�̍U���擾�p
	float fAddRot = 0.0f;                                         //���Z����
	float fAddAim = 0.0f;

	fAddRot = float(rand() % 25) / 100;

	//fVXaim = CGame::GetPlayer()->GetPos().x - Pos.x;
	//fVYaim = (CGame::GetPlayer()->GetPos().y + CGame::GetPlayer()->GetSize().y * 0.5f) - Pos.y;//�v���C���[�̒��S��_��
	//fVLaim = (float)(atan2(fVXaim, fVYaim));


	//==========================================
	//�s���J�n
	//==========================================
	if (m_bAction == true)
	{
		if (m_bPattern == false)
		{
			//=============================
			//AI
			//=============================
			if (m_fLength >= 220.0f)
			{//�������������
				nPattern = rand() % 7;

				if (nPattern >= 0 && nPattern <= 1)
				{//���m���Őڋ߂ł���
					m_nPattern = 0;
				}
				else if (nPattern == 2)
				{//��m���Ōジ����
					m_nPattern = 1;
				}
				else if (nPattern == 3)
				{
					m_nPattern = 2;
				}
				else
				{//���m���Ńu���X�X���E
					m_nPattern = 3;
				}
			}
			else
			{//�������߂����
				nPattern = rand() % 9;

				if (nPattern == 0)
				{//��m���Őڋ�
					m_nPattern = 0;
				}
				else if (nPattern >= 1 && nPattern <= 3)
				{//���m���Ōジ����
					m_nPattern = 1;
				}
				else if (nPattern >= 4 && nPattern <= 6)
				{//���m���Ńo���b�g�X���b�V��
					m_nPattern = 2;
				}
				else
				{//���m���Ńu���X�X���E
					m_nPattern = 3;
				}
			}
			//================================================================================
			m_bPattern = true;
		}

		if (m_bPattern == true)
		{
			m_nPatternTime++;
			switch (m_nPattern)
			{
			case 0://�ڋ�
				SetUseInteria(false);//�������g�p���Ȃ�
				ApproachProcess(0.5f);
				if (m_nPatternTime >= 100)
				{
					SetUseInteria(true);//�������g�p����
					//�s���p�^�[���̃��Z�b�g
					ResetPattern();
				}
				break;
			case 1://�ジ����
				SetUseInteria(false);//�������g�p���Ȃ�
				ApproachProcess(-3.0f);
				if (m_nPatternTime >= 35)
				{
					SetUseInteria(true);//�������g�p����

					//�s���p�^�[���̃��Z�b�g
					ResetPattern();
				}
				break;
			case 2://�o���b�g�X���b�V���i�}�g���b�N�X�o���b�g�j
				if (m_nPatternTime == 1)
				{
					CParticle::SummonParticle(CParticle::TYPE00_NORMAL, 50, 20, 40.0f, 40.0f, 200, 10, false, Pos, D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), true);

					for (int nCnt = 0; nCnt < 3; nCnt++)
					{
						pBullet = CBullet::Create(CBullet::BULLETTYPE_ENEMY, CBullet::BULLETTEX_ENEMY, 115, 3, 10.0f, 10.0f,NULL_VECTOR3,
							D3DXVECTOR3(sinf(0.0f) * (0.6f * (float)(nCnt + 1)), cosf(0.0f) * (0.6f * (float)(nCnt + 1)), 0.0f));
						pBullet->SetUseMtxChild(&GetMatrixWorld(),true);//�o���b�g���q�}�g���b�N�X�ɐݒ�
						pBullet->SetUseEffect(30, D3DXVECTOR2(40.0f, 40.0f), D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
					}
				}

				if (m_nPatternTime >= 50)
				{
					if (Pos.x <= CGame::GetPlayer()->GetPos().x)
					{//�v���C���[���E���ɂ�����
						m_nSubPattern = 1;
					}
					else
					{//�v���C���[�������ɂ�����
						m_nSubPattern = 2;
					}
				}

				switch (m_nSubPattern)
				{
				case 1:
					Rot.z += -0.03f;
					break;
				case 2:
					Rot.z += 0.03f;
					break;
				default:
					break;
				}

				if (m_nPatternTime >= 115)
				{
					Rot.z = 0.0f;
					SetUseInteria(true);//�������g�p����

					//�s���p�^�[���̃��Z�b�g
					ResetPattern();
				}
				break;
			case 3:
				//�u���X�X���E
				if (m_nPatternTime == 15)
				{
					CParticle::SummonParticle(CParticle::TYPE00_NORMAL, 50, 20, 40.0f, 40.0f, 200, 10, false, Pos, D3DXCOLOR(1.0f, 0.5f, 0.5f, 1.0f), true);

					if (Pos.x <= CGame::GetPlayer()->GetPos().x)
					{//�v���C���[���E�ɂ����
						m_nSubPattern = 0;
					}
					else
					{//�v���C���[�����ɂ����
						m_nSubPattern = 1;
					}
				}

				if (m_nPatternTime == 65)
				{//��񂾂��n���}�[�𓊂���
					switch (m_nSubPattern)
					{
					case 0:
						pAttackEnemy = CAttackEnemy::Create(CAttack::ATTACKTYPE09_HAMMER, 22, 160, Pos, D3DXVECTOR3(sinf(D3DX_PI * (0.25f - fAddRot)) * 10.0f, cosf(D3DX_PI * (0.25f - fAddRot)) * 10.0f, 0.0f), ONE_VECTOR3 * 3.0f, NULL_VECTOR3,
							CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, D3DXVECTOR3(0.0f, 0.0f, -0.4f), true, 10);
							pAttackEnemy->SetUseGravity(-0.2f);//�d�͂�ݒ�
							break;
					case 1:
						pAttackEnemy = CAttackEnemy::Create(CAttack::ATTACKTYPE09_HAMMER, 22, 160, Pos, D3DXVECTOR3(sinf(D3DX_PI * (-0.25f + fAddRot)) * 10.0f, cosf(D3DX_PI * (-0.25f + fAddRot)) * 10.0f, 0.0f), ONE_VECTOR3 * 3.0f, NULL_VECTOR3,
							CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, D3DXVECTOR3(0.0f, 0.0f, 0.4f), true, 10);
						pAttackEnemy->SetUseGravity(-0.2f);//�d�͂�ݒ�
						break;
					default:
						break;
					}
				}

				if (m_nPatternTime == 115)
				{
					SetUseInteria(true);//�������g�p����
					//�s���p�^�[���̃��Z�b�g
					ResetPattern();
				}

				break;
			default:
				break;
			}
		}
	}
	//===========================================================================

}
//===================================================================================================================================

//====================================================================
//�ڋ߂��鏈��
//====================================================================
void CEnemy::ApproachProcess(float fSpeed)
{
	D3DXVECTOR3& Pos = GetPos();                                  //���f���̈ʒu�̎擾
	D3DXVECTOR3& Move = GetMove();                                //�ړ��ʂ��擾

	if (Pos.x <= CGame::GetPlayer()->GetPos().x)
	{//�v���C���[���E���ɂ�����
		Move.x = fSpeed;
	}
	else
	{//�v���C���[�������ɂ�����
		Move.x = -fSpeed;
	}

	if (Pos.x >= CGame::GetPlayer()->GetPos().x - (GetSize().x / 2.0f) && Pos.x <= CGame::GetPlayer()->GetPos().x + (GetSize().x / 2.0f))
	{
		Move.x = 0.0f;
	}
}
//===================================================================================================================================

//====================================================================
//�G�̈ʒu�𒲐����鏈��
//====================================================================
void CEnemy::AdjustEnemyPos()
{
	if (m_pMoveRangeAI != nullptr)
	{//�ړ��͈͗p�̃��f�������݂�����E�E�E
		D3DXVECTOR3 &Pos = GetPos();
		D3DXVECTOR3& Move = GetMove();
		D3DXVECTOR3 VtxMax = GetVtxMax();
		D3DXVECTOR3 VtxMin = GetVtxMin();

		D3DXVECTOR3 AIPos = m_pMoveRangeAI->GetPos();
		D3DXVECTOR3 AIVtxMax = m_pMoveRangeAI->GetVtxMax();
		D3DXVECTOR3 AIVtxMin = m_pMoveRangeAI->GetVtxMin();


		if (AIPos.x + AIVtxMax.x < Pos.x + VtxMax.x)
		{//�ړ��͈͂��E�ɏo����
			Pos.x = AIPos.x + AIVtxMax.x - VtxMax.x;
			Move.x = 0.0f;
		}
		if (AIPos.x + AIVtxMin.x > Pos.x + VtxMin.x)
		{//�ړ��͈͂�荶�ɏo����
			Pos.x = AIPos.x + AIVtxMin.x - VtxMin.x;
			Move.x = 0.0f;
		}
	}
}
//===================================================================================================================================

//========================================================================
//���\������
//========================================================================
void CEnemy::DispInfo()
{
	D3DXVECTOR3 Size = GetSize();
	D3DXVECTOR3 Scale = GetScale();
	int nLife = GetLife();

	ChengeLifeProcess();//�̗͂�ύX����

	if (m_pMoveRangeAI != nullptr)
	{
		ChengeMoveLengthAIScale();//�ړ��͈͂̊g�嗦��ύX����

		ChengeMoveLengthAIPos();//�ړ��͈͂̈ʒu��ύX����

	}

	CObjectX::DispInfo();
}
//===================================================================================================================================

//========================================================================
//�̗͂�ς��鏈��
//========================================================================
void CEnemy::ChengeLifeProcess()
{
	int& nLife = GetLife();

	if (CManager::GetInputKeyboard()->GetPress(DIK_LCONTROL) == true)
	{//L�R���g���[���L�[�������Ȃ���
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//�V�t�g�L�[�������Ȃ���E�E�E
			if (CManager::GetInputKeyboard()->GetPress(DIK_1) == true)
			{
				nLife -= 5;
			}
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_1) == true)
		{
			nLife += 5;
		}
	}
	else
	{//L�R���g���[���L�[�������Ă��Ȃ�
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//�V�t�g�L�[�������Ȃ���E�E�E
			if (CManager::GetInputKeyboard()->GetTrigger(DIK_1) == true)
			{
				nLife -= 5;
			}
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_1) == true)
		{
			nLife += 5;
		}
	}

	CManager::GetDebugText()->PrintDebugText("�̗́i1) : %d\n",nLife);
}
//===================================================================================================================================

//========================================================================
//�ړ��͈�AI���f���͈̔͂�ς��鏈��
//========================================================================
void CEnemy::ChengeMoveLengthAIScale()
{
	D3DXVECTOR3& Scale = m_pMoveRangeAI->GetScale();

	ChengeMoveLengthAIScaleX();
	ChengeMoveLengthAIScaleY();
	ChengeMoveLengthAIScaleZ();

	CManager::GetDebugText()->PrintDebugText("�ړ��͈͂̊g�嗦�i2,3,4) : %f %f %f\n", Scale.x,Scale.y,Scale.z);
}
//===================================================================================================================================

//========================================================================
//�ړ��͈�AI���f����X�͈̔͂�ς��鏈��
//========================================================================
void CEnemy::ChengeMoveLengthAIScaleX()
{
	D3DXVECTOR3& Scale = m_pMoveRangeAI->GetScale();
	if (CManager::GetInputKeyboard()->GetPress(DIK_LCONTROL) == true)
	{//L�R���g���[���L�[�������Ȃ���
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//�V�t�g�L�[�������Ȃ���E�E�E
			if (CManager::GetInputKeyboard()->GetPress(DIK_2) == true)
			{
				Scale.x -= 0.1f;
			}
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_2) == true)
		{
			Scale.x += 0.1f;
		}
	}
	else
	{//L�R���g���[���L�[�������Ă��Ȃ�
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//�V�t�g�L�[�������Ȃ���E�E�E
			if (CManager::GetInputKeyboard()->GetTrigger(DIK_2) == true)
			{
				Scale.x -= 0.1f;
			}
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_2) == true)
		{
			Scale.x += 0.1f;
		}
	}
}
//===================================================================================================================================

//========================================================================
//�ړ��͈�AI���f����Y�͈̔͂�ς��鏈��
//========================================================================
void CEnemy::ChengeMoveLengthAIScaleY()
{
	D3DXVECTOR3& Scale = m_pMoveRangeAI->GetScale();

	if (CManager::GetInputKeyboard()->GetPress(DIK_LCONTROL) == true)
	{//L�R���g���[���L�[�������Ȃ���
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//�V�t�g�L�[�������Ȃ���E�E�E
			if (CManager::GetInputKeyboard()->GetPress(DIK_3) == true)
			{
				Scale.y -= 0.1f;
			}
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_3) == true)
		{
			Scale.y += 0.1f;
		}
	}
	else
	{//L�R���g���[���L�[�������Ă��Ȃ�
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//�V�t�g�L�[�������Ȃ���E�E�E
			if (CManager::GetInputKeyboard()->GetTrigger(DIK_3) == true)
			{
				Scale.y -= 0.1f;
			}
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_3) == true)
		{
			Scale.y += 0.1f;
		}
	}
}
//===================================================================================================================================

//========================================================================
//�ړ��͈�AI���f����Y�͈̔͂�ς��鏈��
//========================================================================
void CEnemy::ChengeMoveLengthAIScaleZ()
{
	D3DXVECTOR3& Scale = m_pMoveRangeAI->GetScale();

	if (CManager::GetInputKeyboard()->GetPress(DIK_LCONTROL) == true)
	{//L�R���g���[���L�[�������Ȃ���
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//�V�t�g�L�[�������Ȃ���E�E�E
			if (CManager::GetInputKeyboard()->GetPress(DIK_4) == true)
			{
				Scale.z -= 0.1f;
			}
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_4) == true)
		{
			Scale.z += 0.1f;
		}
	}
	else
	{//L�R���g���[���L�[�������Ă��Ȃ�
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//�V�t�g�L�[�������Ȃ���E�E�E
			if (CManager::GetInputKeyboard()->GetTrigger(DIK_4) == true)
			{
				Scale.z -= 0.1f;
			}
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_4) == true)
		{
			Scale.z += 0.1f;
		}
	}

}
//===================================================================================================================================

//========================================================================
//�ړ��͈�AI���f���̈ʒu��ς��鏈��
//========================================================================
void CEnemy::ChengeMoveLengthAIPos()
{
	D3DXVECTOR3& AIPos = m_pMoveRangeAI->GetPos();//�ړ��͈�AI�̈ʒu

	CCalculation::CaluclationMove(AIPos, 5.0f);

	CManager::GetDebugText()->PrintDebugText("�ړ��͈͂̈ʒu�iWASD) : %f %f %f\n", AIPos.x, AIPos.y, AIPos.z);
}
//===================================================================================================================================

//========================================================================
//�G�S�̃p�^�[��
//========================================================================
void CEnemy::Enemy04Pattern()
{
	D3DXVECTOR3& Pos = GetPos();                                  //���f���̈ʒu�̎擾
	D3DXVECTOR3& PosOld = GetPosOld();                            //1f�O�̈ʒu��ݒ�
	D3DXVECTOR3& Move = GetMove();                                //�ړ��ʂ��擾
	D3DXVECTOR3& Rot = GetRot();                                   //���f���̌����̎擾
	D3DXVECTOR3 Size = GetSize();                                 //�T�C�Y���擾
	int nPattern = 0;                                             //�p�^�[��
	float fAddSpeed = float(rand() % 40 + 1) / 10;                //�����̕␳
	float fRandRot = float(rand() % 628 + 1) / 100;               //��������
	CBullet* pBullet = nullptr;                                   //�o���b�g�ւ̃|�C���^
	float fVXaim = 0.0f;                                          //X�����x�N�g��
	float fVYaim = 0.0f;                                          //Y�����x�N�g��
	float fVLaim = 0.0f;                                          //�����x�N�g��
	D3DXMATRIX mtxWorld = GetMatrixWorld();                       //�}�g���b�N�X���[���h
	CAttackEnemy* pAttackEnemy = nullptr;                         //�G�̍U���擾�p
	float fAddRot = 0.0f;                                         //���Z����
	bool& bIsLanding = GetLanding();                              //�n�ʂɂ��邩�ǂ������擾

	fAddRot = float(rand() % 25) / 100;

	fVXaim = CGame::GetPlayer()->GetPos().x - Pos.x;
	fVYaim = (CGame::GetPlayer()->GetPos().y + CGame::GetPlayer()->GetSize().y * 0.5f) - Pos.y;//�v���C���[�̒��S��_��
	fVLaim = (float)(atan2(fVXaim, fVYaim));


	//==========================================
	//�s���J�n
	//==========================================
	if (m_bAction == true)
	{
		if (m_bPattern == false)
		{
			//=============================
			//AI
			//=============================
			if (m_fLength >= 250.0f)
			{//�������������
				nPattern = rand() % 5;

				if (nPattern >= 0 && nPattern <= 1)
				{//���m���ł̂��̂��ڋ�
					m_nPattern = 0;
				}
				else if (nPattern >= 2 && nPattern <= 3)
				{//���m���ŏՌ��g
					m_nPattern = 1;
				}
				else
				{//��m���Ńr�b�O�X���[�E�F�C
					m_nPattern = 2;
				}
			}
			else
			{//�������߂����
				nPattern = rand() % 5;

				if (nPattern == 0)
				{//��m���Őڋ�
					m_nPattern = 0;
				}
				else if (nPattern == 1)
				{//��m���ŏՌ��g
					m_nPattern = 1;
				}
				else
				{//���m���Ńr�b�O�X���[�E�F�C
					m_nPattern = 2;
				}
			}
			//================================================================================
			m_bPattern = true;
		}

		if (m_bPattern == true)
		{
			m_nPatternTime++;
			switch (m_nPattern)
			{
			case 0://�̂��̂��ڋ�
				if (m_nPatternTime % 30 == 0)
				{
					SetInertia(0.1f);
					SetUseInteria(true);//�������g�p����
					ApproachProcess(7.0f);
				}
				if (m_nPatternTime >= 100)
				{
					SetInertia(0.5f);
					//�s���p�^�[���̃��Z�b�g
					ResetPattern();
				}
				break;
			case 1://�Ռ��g

				if (m_nPatternTime % 10 == 0 && m_nPatternTime <= 30)
				{//�`���[�W
					CParticle::SummonChargeParticle(CParticle::TYPE00_NORMAL, 50, 100, 1.0f, 40.0f, 40.0f, 200, 20, 1, true, Pos, NORMAL_COL, true);
				}

				if (m_nPatternTime == 45)
				{
					Move.y = 20.0f;
					CUi3D::Create(CUi3D::UI3DTYPE01_WARNING, 60, 70.0f, 70.0f,Pos,D3DXVECTOR3(20.0f,0.0f,0.0f));
					CUi3D::Create(CUi3D::UI3DTYPE01_WARNING, 60, 70.0f, 70.0f,Pos,D3DXVECTOR3(-20.0f,0.0f,0.0f));
				}

				if (m_nPatternTime >= 50)
				{
					if (bIsLanding == true)
					{
						pAttackEnemy = CAttackEnemy::Create(CAttack::ATTACKTYPE10_SHOCKWAVE, 25, 300, Pos, D3DXVECTOR3(sinf(D3DX_PI * 0.5f) * 5.0f, cosf(D3DX_PI * 0.5f) * 5.0f, 0.0f),
							ONE_VECTOR3 * 3, NULL_VECTOR3, CAttack::COLLISIONTYPE02_TRUERECTANGLEXY,
							NULL_VECTOR3, false, 11);

						pAttackEnemy->SetUseMultiSpeed(true, ONE_VECTOR3 * (1.05f + 0.004f * (float)(m_nSubPattern)));

						pAttackEnemy = CAttackEnemy::Create(CAttack::ATTACKTYPE10_SHOCKWAVE, 25, 300, Pos, D3DXVECTOR3(sinf(D3DX_PI * 0.5f) * -5.0f, cosf(D3DX_PI * 0.5f) * -5.0f, 0.0f),
							ONE_VECTOR3 * 3, D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), CAttack::COLLISIONTYPE02_TRUERECTANGLEXY,
							NULL_VECTOR3, false, 11);

						pAttackEnemy->SetUseMultiSpeed(true, ONE_VECTOR3 * (1.05f + 0.004f * (float)(m_nSubPattern)));
						ResetPattern();
					}
				}


				break;
			case 2://�r�b�O�X���[�E�F�C
				if (m_nPatternTime == 30)
				{
					CParticle::SummonParticle(CParticle::TYPE00_NORMAL, 50, 20, 40.0f, 40.0f, 200, 10, false, Pos, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), true);
				}

				if (m_nPatternTime == 50)
				{
					for (int nCnt = 0; nCnt < 3; nCnt++)
					{
						CBullet::Create(CBullet::BULLETTYPE_ENEMY, CBullet::BULLETTEX_ENEMY, 100, 7, 60.0f, 60.0f, Pos, D3DXVECTOR3(sinf(fVLaim - m_fENEMY04_ACTION2THREEWAYAIM + m_fENEMY04_ACTION2THREEWAYAIM * (float)(nCnt)) * 5.0f,
							cosf(fVLaim - m_fENEMY04_ACTION2THREEWAYAIM + m_fENEMY04_ACTION2THREEWAYAIM * (float)(nCnt)) * 5.0f, 0.0f));
					}
				}

				if (m_nPatternTime == 90)
				{
					ResetPattern();
				}
				break;
			default:
				break;
			}
		}
	}
	//===========================================================================

}
//===================================================================================================================================

//=======================================================================
//�G�T�̃p�^�[��
//=======================================================================
void CEnemy::Enemy05Pattern()
{
	D3DXVECTOR3& Pos = GetPos();                                  //���f���̈ʒu�̎擾
	D3DXVECTOR3& PosOld = GetPosOld();                            //1f�O�̈ʒu��ݒ�
	D3DXVECTOR3& Move = GetMove();                                //�ړ��ʂ��擾
	D3DXVECTOR3& Rot = GetRot();                                   //���f���̌����̎擾
	D3DXVECTOR3 Size = GetSize();                                 //�T�C�Y���擾
	int nPattern = 0;                                             //�p�^�[��
	float fAddSpeed = float(rand() % 40 + 1) / 10;                //�����̕␳
	float fRandRot = float(rand() % 628 + 1) / 100;               //��������
	float fVXaim = 0.0f;                                          //X�����x�N�g��
	float fVYaim = 0.0f;                                          //Y�����x�N�g��
	float fVLaim = 0.0f;                                          //�����x�N�g��
	bool& bUseGravity = GetUseGravity();                          //�d�͂��g�p���邩�ǂ���
	D3DXMATRIX mtxWorld = GetMatrixWorld();                       //�}�g���b�N�X���[���h
	CBullet* pBullet = nullptr;                                   //�o���b�g�ւ̃|�C���^
	CAttackEnemy* pAttackEnemy = nullptr;                         //�G�̍U���擾�p
	float fAddRot = 0.0f;                                         //���Z����
	int nRand = 0;//����
	fAddRot = float(rand() % 25) / 100;

	fVXaim = CGame::GetPlayer()->GetPos().x - Pos.x;
	fVYaim = (CGame::GetPlayer()->GetPos().y + CGame::GetPlayer()->GetSize().y * 0.5f) - Pos.y;//�v���C���[�̒��S��_��
	fVLaim = (float)(atan2(fVXaim, fVYaim));


	//==========================================
	//�s���J�n
	//==========================================
	if (m_bAction == true)
	{
		if (m_bPattern == false)
		{
			//=============================
			//AI
			//=============================
			if (m_fLength >= 260.0f)
			{//�������������
				nPattern = rand() % 5;

				if (nPattern == 0)
				{//��m���ŃT���_�[�X�e�b�v
					m_nPattern = 0;
				}
				else if (nPattern >= 1 && nPattern <= 3)
				{//���m���ŃJ�[�u�e
					m_nPattern = 1;
				}
				else
				{//��m���Ńt�H�[�����X�p�[�N
					m_nPattern = 2;
				}
			}
			else
			{//�������߂����
				nPattern = rand() % 6;

				if (nPattern >= 0 && nPattern <= 1)
				{//���m���ŃT���_�[�X�e�b�v
					m_nPattern = 0;
				}
				else if (nPattern == 2)
				{//��m���ŃJ�[�u�e
					m_nPattern = 1;
				}
				else
				{//���m���Ńt�H�[�����X�p�[�N
					m_nPattern = 2;
				}
			}
			//================================================================================
			m_bPattern = true;
		}

		if (m_bPattern == true)
		{
			m_nPatternTime++;
			switch (m_nPattern)
			{
			case 0://�T���_�[�X�e�b�v
				if (m_nPatternTime >= 30 && m_nPatternTime <= 85)
				{
					bUseGravity = false;//�d�͂��g��Ȃ�
					SetUseInteria(false);//�������g��Ȃ�
					if (m_nPatternTime == 30)
					{
						CUi3D::Create(CUi3D::UI3DTYPE01_WARNING, 50, 200.0f, 200.0f, Pos, NULL_VECTOR3);
						CParticle::SummonParticle(CParticle::TYPE00_NORMAL, 50, 20, 40.0f, 40.0f, 200, 10, false, Pos, D3DXCOLOR(0.5f, 0.5f, 1.0f, 1.0f), true);
					}

					if (m_nPatternTime >= 75)
					{
						CEffect::Create(CEffect::EFFECTTYPE01_THUNDER, 100, 25.0f, 25.0f, Pos + D3DXVECTOR3(0.0f, GetSize().y * 0.5f, 0.0f), NORMAL_COL);
					}

					if (m_nPatternTime == 75)
					{
						m_fTotalAim = float(rand() % 314 - 157) / 100;                      //�R�U�O�x�̃x�N�g���̗���
						Move.x = sinf(m_fTotalAim) * 20.0f;
						Move.y = cosf(m_fTotalAim) * 20.0f;
					}
					if (m_nPatternTime == 85)
					{
						m_fTotalAim = float(rand() % 314 - 157) / 100;                      //�R�U�O�x�̃x�N�g���̗���
						Move.x = sinf(m_fTotalAim + D3DX_PI) * 20.0f;
						Move.y = cosf(m_fTotalAim + D3DX_PI) * 20.0f;
					}

					if (m_nPatternTime == 95)
					{
						Move.x = 0.0f;
					}
				}

				if (m_nPatternTime == 125)
				{
					bUseGravity = true;//�d�͂��g��Ȃ�
					SetUseInteria(true);//�������g��Ȃ�
					m_fGravityPower = GetNormalGravity();
					ResetPattern();
				}
				break;
			case 1://�J�[�u�e
				if (m_nPatternTime == 30)
				{
					CParticle::SummonParticle(CParticle::TYPE00_NORMAL, 50, 20, 40.0f, 40.0f, 200, 10, false, Pos, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), true);
				}

				if (m_nPatternTime == 95)
				{
					int nRand = rand() % 2;
					for (int nCnt = 0; nCnt < m_nMAX_ENEMY05_BULLETNUM; nCnt++)
					{
						float fRot = ((D3DX_PI * 2.0f) / m_nMAX_ENEMY05_BULLETNUM) * nCnt;
						pBullet = CBullet::Create(CBullet::BULLETTYPE_ENEMY, CBullet::BULLETTEX_ENEMY, 150, 15, 30.0f, 30.0f, Pos, NULL_VECTOR3);
						if (nRand == 0)
						{
							pBullet->SetUseCurve(fRot, 5.0f, 0.02f);
							pBullet->SetUseEffect(30, D3DXVECTOR2(30.0f, 30.0f), D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f));
						}
						else
						{
							pBullet->SetUseCurve(fRot, 5.0f, -0.02f);
							pBullet->SetUseEffect(30, D3DXVECTOR2(30.0f, 30.0f), D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f));
						}
					}
				}

				if (m_nPatternTime == 155)
				{
					ResetPattern();
				}
				break;
			case 2://�t�H�[�����X�p�[�N
				if (m_nPatternTime == 30)
				{
					CParticle::SummonParticle(CParticle::TYPE00_NORMAL, 50, 20, 40.0f, 40.0f, 200, 10, false, Pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), true);
					CUi3D::Create(CUi3D::UI3DTYPE01_WARNING, 50, 80.0f, 80.0f, Pos + D3DXVECTOR3(100.0f, 0.0f, 0.0f), NULL_VECTOR3);
					CUi3D::Create(CUi3D::UI3DTYPE01_WARNING, 50, 80.0f, 80.0f, Pos + D3DXVECTOR3(-100.0f, 0.0f, 0.0f), NULL_VECTOR3);
				}

				if (m_nPatternTime == 95)
				{
					pAttackEnemy = CAttackEnemy::Create(CAttack::ATTACKTYPE06_THUNDER, 10, 165, Pos + D3DXVECTOR3(100.0f, 600.0f, 0.0f),
						D3DXVECTOR3(sinf(D3DX_PI) * 15.0f, cosf(D3DX_PI) * 15.0f, 0.0f), D3DXVECTOR3(1.0f, 2.0f, 1.0f), NULL_VECTOR3, CAttack::COLLISIONTYPE02_TRUERECTANGLEXY,
						D3DXVECTOR3(0.0f,0.3f,0.0f), true, 25);

					pAttackEnemy->SetUseAddScale(D3DXVECTOR3(0.0f, 0.1f, 0.0f),true);

					pAttackEnemy = CAttackEnemy::Create(CAttack::ATTACKTYPE06_THUNDER, 10, 165, Pos + D3DXVECTOR3(-100.0f, 600.0f, 0.0f),
						D3DXVECTOR3(sinf(D3DX_PI) * 15.0f, cosf(D3DX_PI) * 15.0f, 0.0f), D3DXVECTOR3(1.0f, 2.0f, 1.0f), NULL_VECTOR3, CAttack::COLLISIONTYPE02_TRUERECTANGLEXY,
						D3DXVECTOR3(0.0f, 0.3f, 0.0f), true, 25);

					pAttackEnemy->SetUseAddScale(D3DXVECTOR3(0.0f, 0.1f, 0.0f),true);
				}

				if (m_nPatternTime == 145)
				{
					ResetPattern();
				}
				break;
			default:
				break;
			}
		}
	}
	//===========================================================================
}
//===================================================================================================================================

//============================================================================
//�G�U�̃p�^�[��
//============================================================================
void CEnemy::Enemy06Pattern()
{
	D3DXVECTOR3& Pos = GetPos();                                  //���f���̈ʒu�̎擾
	D3DXVECTOR3& PosOld = GetPosOld();                            //1f�O�̈ʒu��ݒ�
	D3DXVECTOR3& Move = GetMove();                                //�ړ��ʂ��擾
	D3DXVECTOR3& Rot = GetRot();                                   //���f���̌����̎擾
	D3DXVECTOR3 Size = GetSize();                                 //�T�C�Y���擾
	int nPattern = 0;                                             //�p�^�[��
	float fAddSpeed = float(rand() % 50 + 1) / 10;                //�����̕␳
	float fRandRot = float(rand() % 628 + 1) / 100;               //��������
	float fVXaim = 0.0f;                                          //X�����x�N�g��
	float fVYaim = 0.0f;                                          //Y�����x�N�g��
	float fVLaim = 0.0f;                                          //�����x�N�g��
	D3DXMATRIX mtxWorld = GetMatrixWorld();                       //�}�g���b�N�X���[���h
	CBullet* pBullet = nullptr;                                   //�o���b�g�ւ̃|�C���^
	CAttackEnemy* pAttackEnemy = nullptr;                         //�G�̍U���擾�p
	float fAddRot = 0.0f;                                         //���Z����
	bool& bIsLanding = GetLanding();                              //�n�ʂɂ��邩�ǂ������擾
	bool& bUseGravity = GetUseGravity();                          //�d�͂��g�p���邩�ǂ���

	fVXaim = CGame::GetPlayer()->GetPos().x - Pos.x;
	fVYaim = (CGame::GetPlayer()->GetPos().y + CGame::GetPlayer()->GetSize().y * 0.5f) - Pos.y;//�v���C���[�̒��S��_��
	fVLaim = (float)(atan2(fVXaim, fVYaim));


	//==========================================
	//�s���J�n
	//==========================================
	if (m_bAction == true)
	{
		if (m_bPattern == false)
		{
			//=============================
			//AI
			//=============================
			if (m_fLength >= 200.0f)
			{//�������������
				nPattern = rand() % 8;
				if (nPattern == 0)
				{//��m���ŃN�������X�e�b�v
					m_nPattern = 0;
				}
				else if (nPattern == 1)
				{//��m���Őڋ�
					m_nPattern = 1;
				}
				else if (nPattern >= 2 && nPattern <= 4)
				{//���m���ŃV���b�g�K��
					m_nPattern = 2;
				}
				else
				{//���m���Ńz�[�~���O�e
					m_nPattern = 3;
				}
			}
			else
			{//�������߂����
				nPattern = rand() % 8;
				if (nPattern >= 0 && m_nPattern <= 3)
				{//���m���ŃN�������X�e�b�v
					m_nPattern = 0;
				}
				else if (nPattern == 4)
				{//��m���Őڋ�
					m_nPattern = 1;
				}
				else if (nPattern == 5)
				{//��m���ŃV���b�g�K��
					m_nPattern = 2;
				}
				else
				{//���m���Ńz�[�~���O�e
					m_nPattern = 3;
				}
			}
			//================================================================================
			m_bPattern = true;
		}

		if (m_bPattern == true)
		{
			m_nPatternTime++;
			switch (m_nPattern)
			{
			case 0://�N�������X�e�b�v
				if (m_nPatternTime == 45)
				{
					CParticle::SummonParticle(CParticle::TYPE00_NORMAL, 50, 20, 40.0f, 40.0f, 200, 10, false, Pos, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), true);
				}

				if (m_nPatternTime >= 50)
				{
					SetUseInteria(false);
					if (bIsLanding == true)
					{
						m_nCntAction++;
						if (m_nCntAction < 3)
						{
							if (Pos.x <= CGame::GetPlayer()->GetPos().x)
							{//�v���C���[���E���ɂ�����
								Move.x = -5.0f;
								m_nSubPattern = 0;
							}
							else
							{//�v���C���[�������ɂ�����
								Move.x = 5.0f;
								m_nSubPattern = 1;
							}

							Move.y = 15.0f;
						}
					}

					switch (m_nSubPattern)
					{
					case 0:
						Rot.z += 0.4f;
						break;
					case 1:
						Rot.z -= 0.4f;
						break;
					default:
						break;
					}
				}

				if (m_nCntAction == 3)
				{
					Rot.z = 0.0f;
					SetUseInteria(true);
					ResetPattern();//�p�^�[�������Z�b�g
				}
				break;
			case 1://�ڋ�
				ApproachProcess(2.0f);//�ڋ�

				if (m_nPatternTime == 80)
				{
					ResetPattern();//�p�^�[�������Z�b�g
				}
				break;
			case 2://�V���b�g�K��
				if (m_nPatternTime == 15)
				{
					CParticle::SummonChargeParticle(CParticle::TYPE00_NORMAL, 50, 150,1.0f, 40.0f, 40.0f, 200, 200,20, true, Pos,D3DXCOLOR(1.0f,0.0f,1.0f,1.0f),true);

					for (int nCnt = 0; nCnt < 7; nCnt++)
					{
						fAddRot = float(rand() % 80 - 40) / 100;
						fAddSpeed = float(rand() % 50 + 1) / 10;
						CUi3D::Create(CUi3D::UI3DTYPE01_WARNING, 50, 50.0f, 50.0f, GetSenterPos(), D3DXVECTOR3(sinf(fVLaim + fAddRot)* (3.0f + fAddSpeed), cosf(fVLaim + fAddRot)* (3.0f + fAddSpeed), 0.0f));
					}
				}

				if (m_nPatternTime == 120)
				{
					for (int nCnt = 0; nCnt < 7; nCnt++)
					{
						fAddRot = float(rand() % 80 - 40) / 100;
						fAddSpeed = float(rand() % 50 + 1) / 10;
						CBullet::Create(CBullet::BULLETTYPE_ENEMY, CBullet::BULLETTEX_ENEMY, 200, 4, 20.0f, 20.0f, Pos,
							D3DXVECTOR3(sinf(fVLaim + fAddRot) * (10.0f + fAddSpeed), cosf(fVLaim + fAddRot) * (10.0f + fAddSpeed),0.0f));
					}
				}

				if (m_nPatternTime == 160)
				{
					ResetPattern();
				}

				break;
			case 3://�z�[�~���O�e
				if (m_nPatternTime == 15)
				{
					CParticle::SummonParticle(CParticle::TYPE00_NORMAL, 50, 20, 40.0f, 40.0f, 200, 10, false, Pos, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), true);
				}

				if (m_nPatternTime == 90)
				{
					pBullet = CBullet::Create(CBullet::BULLETTYPE_ENEMY, CBullet::BULLETTEX_ENEMY, 300, 5, 30.0f, 30.0f, Pos, NULL_VECTOR3);
					pBullet->SetUseHorming(5.0f);//�z�[�~���O���g�p����
					pBullet->SetUseEffect(60, D3DXVECTOR2(30.0f, 30.0f), D3DXCOLOR(0.0f, 1.0f, 0.5f, 1.0f));//�G�t�F�N�g���g�p����
				}

				if (m_nPatternTime == 130)
				{
					ResetPattern();
				}
				break;
			default:
				break;
			}
		}
	}
	//===========================================================================

}
//===================================================================================================================================

//===============================================================
//�G�V�̃p�^�[��
//===============================================================
void CEnemy::Enemy07Pattern()
{
	D3DXVECTOR3& Pos = GetPos();                                  //���f���̈ʒu�̎擾
	D3DXVECTOR3& PosOld = GetPosOld();                            //1f�O�̈ʒu��ݒ�
	D3DXVECTOR3& Move = GetMove();                                //�ړ��ʂ��擾
	D3DXVECTOR3& Rot = GetRot();                                  //���f���̌����̎擾
	D3DXVECTOR3 Size = GetSize();                                 //�T�C�Y���擾
	int nPattern = 0;                                             //�p�^�[��
	float fAddSpeed = float(rand() % 50 + 1) / 10;                //�����̕␳
	float fRandRot = float(rand() % 628 + 1) / 100;               //��������
	float fVXaim = 0.0f;                                          //X�����x�N�g��
	float fVYaim = 0.0f;                                          //Y�����x�N�g��
	float fVLaim = 0.0f;                                          //�����x�N�g��
	D3DXMATRIX mtxWorld = GetMatrixWorld();                       //�}�g���b�N�X���[���h
	CBullet* pBullet = nullptr;                                   //�o���b�g�ւ̃|�C���^
	CAttackEnemy* pAttackEnemy = nullptr;                         //�G�̍U���擾�p
	float fAddRot = 0.0f;                                         //���Z����
	bool& bUseGravity = GetUseGravity();                          //�d�͂��g�p���邩�ǂ���

	fVXaim = CGame::GetPlayer()->GetPos().x - Pos.x;
	fVYaim = (CGame::GetPlayer()->GetPos().y + CGame::GetPlayer()->GetSize().y * 0.5f) - Pos.y;//�v���C���[�̒��S��_��
	fVLaim = (float)(atan2(fVXaim, fVYaim));


	//==========================================
	//�s���J�n
	//==========================================
	if (m_bAction == true)
	{
		if (m_bPattern == false)
		{
			//=============================
			//AI
			//=============================
			if (m_fLength >= 200.0f)
			{//�������������
				nPattern = rand() % 8;
				if (nPattern >= 0 && nPattern <= 2)
				{
					m_nPattern = 0;
				}
				else if (nPattern >= 3 && nPattern <= 4)
				{
					m_nPattern = 1;
				}
				else if (nPattern == 5)
				{
					m_nPattern = 2;
				}
				else
				{
					m_nPattern = 3;
				}
			}
			else
			{//�������߂����
				nPattern = rand() % 8;
				if (nPattern >= 0 && nPattern <= 1)
				{
					m_nPattern = 0;
				}
				else if (nPattern >= 2 && nPattern <= 4)
				{
					m_nPattern = 1;
				}
				else if (nPattern >= 5 && nPattern <= 6)
				{
					m_nPattern = 2;
				}
				else
				{
					m_nPattern = 3;
				}
			}
			//================================================================================
			m_bPattern = true;
		}

		if (m_bPattern == true)
		{
			m_nPatternTime++;
			switch (m_nPattern)
			{
			case 0://������
				if (m_nPatternTime == 15)
				{
					CParticle::SummonParticle(CParticle::TYPE00_NORMAL, 50, 20, 40.0f, 40.0f, 200, 10, false, Pos, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), true);
				}

				if (m_nPatternTime == 30)
				{
					SetInertia(0.05f);
					ApproachProcess(10.0f);
				}

				if (m_nPatternTime == 80)
				{
					SetInertia(0.5f);
					ResetPattern();
				}
				break;
			case 1://�c�C���j�[�h��
				if (m_nPatternTime == 20)
				{
					CParticle::SummonParticle(CParticle::TYPE00_NORMAL, 50, 20, 40.0f, 40.0f, 200, 10, false, Pos, D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), true);
					CUi3D::Create(CUi3D::UI3DTYPE01_WARNING, 60, 100.0f, 100.0f, GetSenterPos() + D3DXVECTOR3(120.0f,0.0f,0.0f),NULL_VECTOR3);
					CUi3D::Create(CUi3D::UI3DTYPE01_WARNING, 60, 100.0f, 100.0f, GetSenterPos() + D3DXVECTOR3(-120.0f,0.0f,0.0f),NULL_VECTOR3);
				}

				if (m_nPatternTime == 50)
				{
					pAttackEnemy = CAttackEnemy::Create(CAttack::ATTACKTYPE02_NEEDLE, 10, 50, Pos, NULL_VECTOR3, D3DXVECTOR3(1.0f,0.0f,1.0f), D3DXVECTOR3(0.0f, 0.0f, D3DX_PI * 0.5f),
						CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, NULL_VECTOR3, true, 10);
					pAttackEnemy->SetUseAddScale(D3DXVECTOR3(0.0f, 0.05f, 0.0f),true);

					pAttackEnemy = CAttackEnemy::Create(CAttack::ATTACKTYPE02_NEEDLE, 10, 50, Pos, NULL_VECTOR3, D3DXVECTOR3(1.0f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, D3DX_PI * -0.5f),
						CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, NULL_VECTOR3, true, 10);
					pAttackEnemy->SetUseAddScale(D3DXVECTOR3(0.0f, 0.05f, 0.0f),true);
				}

				if (m_nPatternTime == 80)
				{
					ResetPattern();
				}
				break;
			case 2://�A�b�v�j�[�h��
				if (m_nPatternTime == 20)
				{
					CParticle::SummonParticle(CParticle::TYPE00_NORMAL, 50, 20, 40.0f, 40.0f, 200, 10, false, Pos, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), true);
					CUi3D::Create(CUi3D::UI3DTYPE01_WARNING, 60, 100.0f, 100.0f, GetSenterPos() + D3DXVECTOR3(0.0f, 100.0f, 0.0f), NULL_VECTOR3);

				}

				if (m_nPatternTime == 50)
				{
					pAttackEnemy = CAttackEnemy::Create(CAttack::ATTACKTYPE02_NEEDLE, 10, 50, Pos, NULL_VECTOR3, D3DXVECTOR3(1.5f, 0.0f, 1.5f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
						CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, NULL_VECTOR3, true, 10);
					pAttackEnemy->SetUseAddScale(D3DXVECTOR3(0.0f, 0.07f, 0.0f),true);

				}

				if (m_nPatternTime == 90)
				{
					ResetPattern();
				}
				break;
			case 3://�U�肩�Ԃ�V���b�g
				ApproachProcess(1.5f);
				if (m_nPatternTime == 20)
				{
					CParticle::SummonParticle(CParticle::TYPE00_NORMAL, 50, 20, 40.0f, 40.0f, 200, 10, false, Pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), true);
				}
				if (m_nPatternTime == 60)
				{
					if (Pos.x <= CGame::GetPlayer()->GetPos().x)
					{//�v���C���[���E���ɂ�����
						m_nSubPattern = 1;
					}
					else
					{//�v���C���[�������ɂ�����
						m_nSubPattern = 2;
					}
				}

				switch (m_nSubPattern)
				{
				case 1:
					m_fShotRot += 0.05f;
					break;
				case 2:
					m_fShotRot -= 0.05f;
					break;
				default:
					break;
				}

				if (m_nPatternTime >= 60)
				{
					if (m_nPatternTime % 10 == 0)
					{
						pBullet = CBullet::Create(CBullet::BULLETTYPE_ENEMY, CBullet::BULLETTEX_ENEMY, 100, 2, 20.0f, 20.0f, Pos, D3DXVECTOR3(sinf(m_fShotRot) * 5.0f, cosf(m_fShotRot) * 5.0f, 0.0f));
						pBullet->SetUseEffect(30, D3DXVECTOR2(40.0f, 40.0f), D3DXCOLOR(1.0f, 1.0f, 0.0f,1.0f));
					}
				}

				if (m_nPatternTime >= 120)
				{
					m_fShotRot = 0.0f;
					ResetPattern();//�p�^�[���̃��Z�b�g
				}
				break;
			default:
				break;
			}
		}
	}
	//===========================================================================
}
//===================================================================================================================================

//========================================================================
//�G�W�̃p�^�[��
//========================================================================
void CEnemy::Enemy08Pattern()
{
	D3DXVECTOR3& Pos = GetPos();                                  //���f���̈ʒu�̎擾
	D3DXVECTOR3& PosOld = GetPosOld();                            //1f�O�̈ʒu��ݒ�
	D3DXVECTOR3& Move = GetMove();                                //�ړ��ʂ��擾
	D3DXVECTOR3& Rot = GetRot();                                  //���f���̌����̎擾
	D3DXVECTOR3 Size = GetSize();                                 //�T�C�Y���擾
	int nPattern = 0;                                             //�p�^�[��
	float fVXaim = 0.0f;                                          //X�����x�N�g��
	float fVYaim = 0.0f;                                          //Y�����x�N�g��
	float fVLaim = 0.0f;                                          //�����x�N�g��
	D3DXMATRIX mtxWorld = GetMatrixWorld();                       //�}�g���b�N�X���[���h
	CBullet* pBullet = nullptr;                                   //�o���b�g�ւ̃|�C���^
	CAttackEnemy* pAttackEnemy = nullptr;                         //�G�̍U���擾�p
	float fAddRot = float(rand() % 314 + 1) / 100;                                         //���Z����
	bool& bUseGravity = GetUseGravity();                          //�d�͂��g�p���邩�ǂ���

	fVXaim = CGame::GetPlayer()->GetPos().x - Pos.x;
	fVYaim = (CGame::GetPlayer()->GetPos().y + CGame::GetPlayer()->GetSize().y * 0.5f) - Pos.y;//�v���C���[�̒��S��_��
	fVLaim = (float)(atan2(fVXaim, fVYaim));

	SetUseGravity(false);
	//==============================
    //�ړ����@�i�㉺�^���j
    //==============================
	bUseGravity = false;
	if (m_bSpeedSwitch == false)
	{
		Move.y += 0.1f;
	}
	else
	{
		Move.y -= 0.1f;
	}

	if (Move.y >= 4.0f)
	{
		m_bSpeedSwitch = true;
	}

	if (Move.y <= -4.0f)
	{
		m_bSpeedSwitch = false;
	}
	//====================================================================

	//==========================================
	//�s���J�n
	//==========================================
	if (m_bAction == true)
	{

		if (m_bPattern == false)
		{
			m_nPattern = rand() % 2;
			m_bPattern = true;
		}

		if (m_bPattern == true)
		{
			m_nPatternTime++;
			switch (m_nPattern)
			{
			case 0:
				if (m_nPatternTime == 1)
				{
					CParticle::SummonParticle(CParticle::TYPE00_NORMAL, 50, 20, 40.0f, 40.0f, 200, 10, false, Pos, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), true);
				}
				if (m_nPatternTime % 60 == 0)
				{
					for (int nCnt = 0; nCnt < 2; nCnt++)
					{
						float fSpeed = 2.0f;
						CBullet::Create(CBullet::BULLETTYPE_ENEMY, CBullet::BULLETTEX_ENEMY, 120, 15, 30.0f, 30.0f, Pos,
							D3DXVECTOR3(sinf(fVLaim) * (fSpeed * (1.0f + (float)(nCnt))), cosf(fVLaim) * (fSpeed * (1.0f + (float)(nCnt))),0.0f));
					}
				}
				if (m_nPatternTime == 125)
				{
					ResetPattern();
				}
				break;
			case 1:
				if (m_nPatternTime == 15)
				{
					CParticle::SummonParticle(CParticle::TYPE00_NORMAL, 50, 20, 40.0f, 40.0f, 200, 10, false, Pos, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), true);
				}

				if (m_nPatternTime % 40 == 0)
				{
					for (int nCnt = 0; nCnt < 8; nCnt++)
					{
						float fRot = ((D3DX_PI * 2.0f) / 8.0f) * (float)(nCnt);
						pBullet = CBullet::Create(CBullet::BULLETTYPE_ENEMY, CBullet::BULLETTEX_ENEMY, 180, 15, 30.0f, 30.0f, Pos,
							D3DXVECTOR3(sinf(fRot + fAddRot) * 0.5f, cosf(fRot + fAddRot) * 0.5f, 0.0f));
						pBullet->SetUseAddSpeed(true, true, 1.03f);
						pBullet->SetUseEffect(10, D3DXVECTOR2(30.0f, 30.0f), D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
					}
				}

				if (m_nPatternTime == 125)
				{
					ResetPattern();
				}
				break;
			default:
				break;
			}
		}
	}
	//===========================================================================

}
//===================================================================================================================================

//======================================================================
//�G�X�̃p�^�[��
//======================================================================
void CEnemy::Enemy09Pattern()
{
	D3DXVECTOR3& Pos = GetPos();                                  //���f���̈ʒu�̎擾
	D3DXVECTOR3& PosOld = GetPosOld();                            //1f�O�̈ʒu��ݒ�
	D3DXVECTOR3& Move = GetMove();                                //�ړ��ʂ��擾
	D3DXVECTOR3& Rot = GetRot();                                  //���f���̌����̎擾
	D3DXVECTOR3 Size = GetSize();                                 //�T�C�Y���擾
	int nPattern = 0;                                             //�p�^�[��
	float fAddSpeed = float(rand() % 50 + 1) / 10;                //�����̕␳
	float fRandRot = float(rand() % 628 + 1) / 100;               //��������
	float fVXaim = 0.0f;                                          //X�����x�N�g��
	float fVYaim = 0.0f;                                          //Y�����x�N�g��
	float fVLaim = 0.0f;                                          //�����x�N�g��
	D3DXMATRIX mtxWorld = GetMatrixWorld();                       //�}�g���b�N�X���[���h
	CBullet* pBullet = nullptr;                                   //�o���b�g�ւ̃|�C���^
	CAttackEnemy* pAttackEnemy = nullptr;                         //�G�̍U���擾�p
	float fAddRot = 0.0f;                                         //���Z����
	D3DXVECTOR3 SummonPos = NULL_VECTOR3;                         //�������g����������ʒu
	fVXaim = CGame::GetPlayer()->GetPos().x - Pos.x;
	fVYaim = (CGame::GetPlayer()->GetPos().y + CGame::GetPlayer()->GetSize().y * 0.5f) - Pos.y;//�v���C���[�̒��S��_��
	fVLaim = (float)(atan2(fVXaim, fVYaim));
	bool& bUseGravity = GetUseGravity();                          //�d�͂��g�p���邩�ǂ���

	fVXaim = CGame::GetPlayer()->GetPos().x - Pos.x;
	fVYaim = (CGame::GetPlayer()->GetPos().y + CGame::GetPlayer()->GetSize().y * 0.5f) - Pos.y;//�v���C���[�̒��S��_��
	fVLaim = (float)(atan2(fVXaim, fVYaim));


	//==========================================
	//�s���J�n
	//==========================================
	if (m_bAction == true)
	{
		if (m_bPattern == false)
		{
			//=============================
			//AI
			//=============================
			if (m_fLength >= 300.0f)
			{//�������������
				nPattern = rand() % 5;
				if (nPattern >= 0 && nPattern <= 1)
				{
					m_nPattern = 0;
				}
				else if (nPattern >= 2 && nPattern <= 3)
				{
					m_nPattern = 1;
				}
				else
				{
					m_nPattern = 2;
				}
			}
			else
			{//�������߂����
				nPattern = rand() % 5;
				if (nPattern == 0)
				{
					m_nPattern = 0;
				}
				else if (nPattern == 1)
				{
					m_nPattern = 1;
				}
				else
				{
					m_nPattern = 2;
				}
			}
			//================================================================================
			m_bPattern = true;
		}

		if (m_bPattern == true)
		{
			m_nPatternTime++;
			switch (m_nPattern)
			{
			case 0://�ڋ�
				if (m_nPatternTime == 15)
				{
					CParticle::SummonParticle(CParticle::TYPE00_NORMAL, 50, 20, 40.0f, 40.0f, 200, 10, false, Pos, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), true);
				}

				if (m_nPatternTime == 30)
				{
					SetUseInteria(false);
					ApproachProcess(3.0f);
				}

				if (m_nPatternTime == 80)
				{
					SetUseInteria(true);
					ResetPattern();
				}
				break;
			case 1://�e���|�[�g�N���b�V��
				switch (m_nSubPattern)
				{
				case 0:
					if (m_nPatternTime == 35)
					{
						CParticle::SummonChargeParticle(CParticle::TYPE00_NORMAL, 50, 150,1.0f, 40.0f, 40.0f, 100, 20, 1, true, Pos, NORMAL_COL, true);
						SetOriginalColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), 50);
						CUi3D::Create(CUi3D::UI3DTYPE01_WARNING, 50, 100.0f, 100.0f,CGame::GetPlayer()->GetSenterPos(), NULL_VECTOR3);
					}

					if (m_nPatternTime == 85)
					{
						SummonPos.x = float(rand() % 400 - 200);
						SummonPos.y = float(rand() % 400 - 200);

						Pos = CGame::GetPlayer()->GetPos() + SummonPos;
						CParticle::SummonParticle(CParticle::TYPE00_NORMAL, 50, 20, 40.0f, 40.0f, 20, 1, false, Pos, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), true);
						m_nSubPattern++;
						m_nPatternTime = 0;
					}
					break;
				case 1:
					Move = NULL_VECTOR3;//�ړ��ʂ��Ȃ���
					bUseGravity = false;//�d�͂��Ȃ���

					if (m_nPatternTime == 90)
					{
						pAttackEnemy = CAttackEnemy::Create(CAttack::ATTACKTYPE11_EXPLOSION, 10, 45, Pos, NULL_VECTOR3, NULL_VECTOR3, NULL_VECTOR3,
							CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, NULL_VECTOR3, false, 15);
						pAttackEnemy->SetUseAddScale(D3DXVECTOR3(0.3f,0.3f,0.3f),true);
						pAttackEnemy->SetUseThinColor(true);//�F��̗͂ɂ���ĕς��邩�ǂ���
						m_nSubPattern++;
						m_nPatternTime = 0;
					}
					break;
				case 2:
					if (m_nPatternTime == 60)
					{
						bUseGravity = true;
						ResetPattern();
					}
					break;
				default:
					break;
				}
				break;
			case 2://�e���|�[�g�V���b�g
				switch (m_nSubPattern)
				{
				case 0:
					if (m_nPatternTime == 35)
					{
						CParticle::SummonChargeParticle(CParticle::TYPE00_NORMAL, 50, 150,1.0f, 40.0f, 40.0f, 200, 20, 1, true, Pos, NORMAL_COL, true);
						SetOriginalColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), 50);
						CUi3D::Create(CUi3D::UI3DTYPE01_WARNING, 60, 120.0f, 120.0f, GetSenterPos(),NULL_VECTOR3);
					}

					if (m_nPatternTime == 85)
					{
						SummonPos.x = float(rand() % 400 - 200);
						SummonPos.y = float(rand() % 400 - 200);

						Pos = CGame::GetPlayer()->GetPos() + SummonPos;
						CParticle::SummonParticle(CParticle::TYPE00_NORMAL, 50, 20, 40.0f, 40.0f, 20, 1, false, Pos, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), true);
						m_nSubPattern++;
						m_nPatternTime = 0;
					}
					break;
				case 1:
					Move = NULL_VECTOR3;//�ړ��ʂ��Ȃ���
					bUseGravity = false;//�d�͂��Ȃ���
					if (m_nSubPattern == 2)
					{
						CUi3D::Create(CUi3D::UI3DTYPE01_WARNING, 60, 60.0f, 60.0f, GetSenterPos(), D3DXVECTOR3(sinf(fVLaim) * 20.0f, cosf(fVLaim) * 20.0f, 0.0f));
					}
					if (m_nPatternTime == 90)
					{
						pBullet = CBullet::Create(CBullet::BULLETTYPE_ENEMY, CBullet::BULLETTEX_ENEMY, 120, 11, 40.0f, 40.0f, Pos, D3DXVECTOR3(sinf(fVLaim) * 10.0f, cosf(fVLaim) * 10.0f, 0.0f));
						pBullet->SetUseAddSpeed(true, true, 1.02f);
						pBullet->SetUseEffect(60, D3DXVECTOR2(40.0f, 40.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
						m_nSubPattern++;
						m_nPatternTime = 0;
					}
					break;
				case 2:
					if (m_nPatternTime == 60)
					{
						bUseGravity = true;
						ResetPattern();
					}
					break;
				default:
					break;
				}
				break;

			default:
				break;
			}
		}
	}
	//===========================================================================

}
//===================================================================================================================================

//======================================================================
//�G�P�O�̃p�^�[��
//======================================================================
void CEnemy::Enemy10Pattern()
{
	D3DXVECTOR3& Pos = GetPos();                                  //���f���̈ʒu�̎擾
	D3DXVECTOR3& PosOld = GetPosOld();                            //1f�O�̈ʒu��ݒ�
	D3DXVECTOR3& Move = GetMove();                                //�ړ��ʂ��擾
	D3DXVECTOR3& Rot = GetRot();                                  //���f���̌����̎擾
	D3DXVECTOR3 Size = GetSize();                                 //�T�C�Y���擾
	int nPattern = 0;                                             //�p�^�[��
	float fAddSpeed = float(rand() % 50 + 1) / 10;                //�����̕␳
	float fRandRot = float(rand() % 628 + 1) / 100;               //��������
	float fVXaim = 0.0f;                                          //X�����x�N�g��
	float fVYaim = 0.0f;                                          //Y�����x�N�g��
	float fVLaim = 0.0f;                                          //�����x�N�g��
	D3DXMATRIX mtxWorld = GetMatrixWorld();                       //�}�g���b�N�X���[���h
	CBullet* pBullet = nullptr;                                   //�o���b�g�ւ̃|�C���^
	CAttackEnemy* pAttackEnemy = nullptr;                         //�G�̍U���擾�p
	float fAddRot = 0.0f;                                         //���Z����
	D3DXVECTOR3 SummonPos = NULL_VECTOR3;                         //�������g����������ʒu
	bool& bIsLanding = GetLanding();                              //�n�ʂɂ��邩�ǂ������擾
	bool& bUseGravity = GetUseGravity();                          //�d�͂��g�p���邩�ǂ���

	fVXaim = CGame::GetPlayer()->GetPos().x - Pos.x;
	fVYaim = (CGame::GetPlayer()->GetPos().y + CGame::GetPlayer()->GetSize().y * 0.5f) - Pos.y;//�v���C���[�̒��S��_��
	fVLaim = (float)(atan2(fVXaim, fVYaim));


	//==========================================
	//�s���J�n
	//==========================================
	if (m_bAction == true)
	{
		if (m_bPattern == false)
		{
			//=============================
			//AI
			//=============================
			m_nPattern = rand() % 5;
			m_bPattern = true;
		}

		if (m_bPattern == true)
		{
			m_nPatternTime++;
			switch (m_nPattern)
			{
			case 0://���p�ړ�
				switch (m_nSubPattern)
				{
				case 0:
					if (m_nPatternTime == 15)
					{
						CParticle::SummonParticle(CParticle::TYPE00_NORMAL, 50, 20, 40.0f, 40.0f, 200, 10, false, Pos, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), true);
					}

					if (m_nPatternTime == 45)
					{
						bUseGravity = false;
						SetUseInteria(false);
						ApproachProcess(10.0f);
						m_nSubPattern++;
						m_nPatternTime = 0;
					}
					break;
				case 1:
					if (m_nPatternTime == 30)
					{
						Move.x = 0.0f;
						m_nSubPattern++;
						m_nPatternTime = 0;
						if (Pos.y <= CGame::GetPlayer()->GetPos().y)
						{
							Move.y = 10.0f;
						}
						else
						{
							Move.y = -10.0f;
						}
					}
					CBullet::Create(CBullet::BULLETTYPE_ENEMY, CBullet::BULLETTEX_ENEMY, 30, 10, 40.0f, 40.0f, Pos + D3DXVECTOR3(0.0f, Size.y / 2.0f, 0.0f),
						D3DXVECTOR3(sinf(0.0f) * 5.0f, cosf(0.0f) * 5.0f, 0.0f));
					CBullet::Create(CBullet::BULLETTYPE_ENEMY, CBullet::BULLETTEX_ENEMY, 30, 10, 40.0f, 40.0f, Pos + D3DXVECTOR3(0.0f, Size.y / 2.0f, 0.0f),
						D3DXVECTOR3(sinf(0.0f) * -5.0f, cosf(0.0f) * -5.0f, 0.0f));
					break;
				case 2:
					CBullet::Create(CBullet::BULLETTYPE_ENEMY, CBullet::BULLETTEX_ENEMY, 30, 10, 40.0f, 40.0f, Pos + D3DXVECTOR3(0.0f, Size.y / 2.0f, 0.0f),
						D3DXVECTOR3(sinf(D3DX_PI * 0.5f) * 5.0f, cosf(D3DX_PI * 0.5f) * 5.0f, 0.0f));
					CBullet::Create(CBullet::BULLETTYPE_ENEMY, CBullet::BULLETTEX_ENEMY, 30, 10, 40.0f, 40.0f, Pos + D3DXVECTOR3(0.0f, Size.y / 2.0f, 0.0f),
						D3DXVECTOR3(sinf(D3DX_PI * 0.5f) * -5.0f, cosf(D3DX_PI * 0.5f) * -5.0f, 0.0f));

					if (m_nPatternTime == 30)
					{
						bUseGravity = true;
						SetUseInteria(true);
						ResetPattern();
					}
					break;
				default:
					break;
				}
				break;
			case 1://���e�I�u���X�g
				if (bIsLanding == true)
				{
					Move.y = 20.0f;
					m_nCntAction++;

					for (int nCnt = 0; nCnt < 5; nCnt++)
					{
						SummonPos.x = float(rand() % 200 - 100) + Pos.x;
						SummonPos.y = float(rand() % 200 - 100) + Pos.y;
						fVXaim = CGame::GetPlayer()->GetPos().x - SummonPos.x;
						fVYaim = (CGame::GetPlayer()->GetPos().y + CGame::GetPlayer()->GetSize().y * 0.5f) - SummonPos.y;//�v���C���[�̒��S��_��
						fVLaim = (float)(atan2(fVXaim, fVYaim));

						CParticle::SummonParticle(CParticle::TYPE00_NORMAL, 50, 20, 40.0f, 40.0f, 200, 10, false, SummonPos, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), true);
						pAttackEnemy = CAttackEnemy::Create(CAttack::ATTACKTYPE01_METEO, 15, 180, SummonPos,
							D3DXVECTOR3(sinf(fVLaim) * 1.0f, cosf(fVLaim) * 1.0f, 0.0f), ONE_VECTOR3 * 2.0f, NULL_VECTOR3,
							CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, ONE_VECTOR3 * 0.1f, true, 30);
						pAttackEnemy->SetUseMultiSpeed(true, ONE_VECTOR3 * 1.03f);
					}

				}

				if (m_nCntAction == 5)
				{
					ResetPattern();
				}
				break;
			case 2://�X�[�p�[�J�[�u�e
				if (m_nPatternTime == 25)
				{
					CParticle::SummonParticle(CParticle::TYPE00_NORMAL, 50, 20, 40.0f, 40.0f, 200, 10, false, Pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), true);
				}

				if (m_nPatternTime == 65)
				{
					for (int nCnt = 0; nCnt < 8; nCnt++)
					{
						float fRot = ((D3DX_PI * 2.0f) / 8.0f) * nCnt;
						pBullet = CBullet::Create(CBullet::BULLETTYPE_ENEMY, CBullet::BULLETTEX_ENEMY, 100, 15, 30.0f, 30.0f, Pos, NULL_VECTOR3);
						pBullet->SetUseCurve(fRot + fRandRot, 10.0f, 0.03f);
						pBullet->SetUseEffect(30, D3DXVECTOR2(30.0f, 30.0f), D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f));
					}

					for (int nCnt = 0; nCnt < 8; nCnt++)
					{
						float fRot = ((D3DX_PI * 2.0f) / 8.0f) * nCnt;
						pBullet = CBullet::Create(CBullet::BULLETTYPE_ENEMY, CBullet::BULLETTEX_ENEMY, 100, 15, 30.0f, 30.0f, Pos, NULL_VECTOR3);
						pBullet->SetUseCurve(fRot + fRandRot, 10.0f, -0.03f);
						pBullet->SetUseEffect(30, D3DXVECTOR2(30.0f,30.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
					}
				}

				if (m_nPatternTime == 100)
				{
					bUseGravity = true;
					SetUseInteria(true);
					ResetPattern();
				}
				break;
			case 3://�G�N�X�g���[���v���X
				if (m_nPatternTime == 25)
				{
					CParticle::SummonParticle(CParticle::TYPE00_NORMAL, 50, 20, 40.0f, 40.0f, 200, 10, false, Pos, D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), true);
				}

				if (m_nPatternTime >= 26)
				{
					ApproachProcess(3.0f);
					if (bIsLanding == true)
					{
						CManager::GetCamera()->SetShake(20, 0.1f);
						m_nSubPattern++;
						if (m_nSubPattern <= 10)
						{
							pAttackEnemy = CAttackEnemy::Create(CAttack::ATTACKTYPE10_SHOCKWAVE, 25, 300, Pos, D3DXVECTOR3(sinf(D3DX_PI * 0.5f) * 1.0f, cosf(D3DX_PI * 0.5f) * 1.0f, 0.0f),
								ONE_VECTOR3 * (float)(m_nSubPattern * 0.4f), NULL_VECTOR3, CAttack::COLLISIONTYPE02_TRUERECTANGLEXY,
								NULL_VECTOR3, true, 11);

							pAttackEnemy->SetUseMultiSpeed(true, ONE_VECTOR3 * (1.05f + 0.004f * (float(m_nSubPattern))));

							pAttackEnemy = CAttackEnemy::Create(CAttack::ATTACKTYPE10_SHOCKWAVE, 25, 300, Pos, D3DXVECTOR3(sinf(D3DX_PI * 0.5f) * -1.0f, cosf(D3DX_PI * 0.5f) * -1.0f, 0.0f),
								ONE_VECTOR3 * (float)(m_nSubPattern * 0.4f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), CAttack::COLLISIONTYPE02_TRUERECTANGLEXY,
								NULL_VECTOR3, true, 11);

							pAttackEnemy->SetUseMultiSpeed(true, ONE_VECTOR3* (1.05f + 0.004f * (float(m_nSubPattern))));
						}
						else
						{
							m_nCntAction++;
							Move.y = 5.0f + 5.0f * (float)(m_nCntAction);
							m_nSubPattern = 0;
						}
					}

					if (m_nCntAction == 5)
					{
						CParticle::SummonParticle(CParticle::TYPE00_NORMAL, 100, 20, 40.0f, 40.0f, 300, 10, false, Pos, D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), true);

						for (int nCnt = 0; nCnt < 10; nCnt++)
						{
							fAddRot = float(rand() % 150 - 75);
							pBullet = CBullet::Create(CBullet::BULLETTYPE_ENEMY, CBullet::BULLETTEX_ENEMY, 120, 12, 30.0f, 30.0f, Pos,
								D3DXVECTOR3(sinf(0.0f + fAddRot) * 10.0f, cosf(0.0f + fAddRot) * 10.0f, 0.0f));
							pBullet->SetUseGravity(-0.3f);
						}
						ResetPattern();
					}
				}
				break;
			case 4://�N���X�T���_�[
				switch (m_nSubPattern)
				{
				case 0:
					if (m_nPatternTime == 25)
					{
						SetUseInteria(false);
						CParticle::SummonParticle(CParticle::TYPE00_NORMAL, 50, 60, 40.0f, 40.0f, 200, 10, false, Pos, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), true);
					}

					if (m_nPatternTime == 50)
					{
						bUseGravity = false;
						Move.y = 10.0f;
					}

					if (m_nPatternTime == 80)
					{
						m_nSubPattern++;
						m_nPatternTime = 0;
					}
					break;
				case 1:
					Rot.z = fVLaim;
					Move = NULL_VECTOR3;//�������~�߂�
					if (m_nPatternTime == 80)
					{
						Move.x = sinf(fVLaim) * 20.0f;
						Move.y = cosf(fVLaim) * 20.0f;
						m_nSubPattern++;
						m_nPatternTime = 0;
					}
					break;
				case 2:
					if (bIsLanding == true)
					{
						Move = NULL_VECTOR3;
						m_nPatternTime = 0;
						m_nSubPattern++;
					}
					break;
				case 3:
					if (m_nPatternTime % 10 == 0)
					{
						m_nCntAction++;

						if (m_nCntAction == 1)
						{
							pAttackEnemy = CAttackEnemy::Create(CAttack::ATTACKTYPE11_EXPLOSION, 2,50, Pos, NULL_VECTOR3, NULL_VECTOR3, NULL_VECTOR3,
								CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, ONE_VECTOR3 * 0.2f, false, 5);
							pAttackEnemy->SetUseAddScale(ONE_VECTOR3 * 0.3f,true);
							pAttackEnemy->SetOriginalColor(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f),50);
							pAttackEnemy->SetUseThinColor(true);
						}

						pAttackEnemy = CAttackEnemy::Create(CAttack::ATTACKTYPE06_THUNDER, 30, 150, Pos + D3DXVECTOR3(90.0f * (float)(m_nCntAction + 1), 300.0f, 0.0f),
							NULL_VECTOR3, ONE_VECTOR3, NULL_VECTOR3, CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, D3DXVECTOR3(0.0f, 0.3f, 0.0f), true, 10);
						pAttackEnemy->SetUseAddScale(D3DXVECTOR3(0.0f, 0.1f, 0.0f),true);

						pAttackEnemy = CAttackEnemy::Create(CAttack::ATTACKTYPE06_THUNDER, 30, 150, Pos + D3DXVECTOR3(-90.0f * (float)(m_nCntAction + 1), 300.0f, 0.0f),
							NULL_VECTOR3, ONE_VECTOR3, NULL_VECTOR3, CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, D3DXVECTOR3(0.0f, 0.3f, 0.0f), true, 10);
						pAttackEnemy->SetUseAddScale(D3DXVECTOR3(0.0f, 0.1f, 0.0f),true);
					}

					if (m_nCntAction == 10)
					{
						Rot.z = 0.0f;
						SetUseInteria(true);
						bUseGravity = true;
						ResetPattern();
					}
				default:
					break;
				}
			break;
			default:
				break;
			}
		}
	}
	//===========================================================================

}
//===================================================================================================================================