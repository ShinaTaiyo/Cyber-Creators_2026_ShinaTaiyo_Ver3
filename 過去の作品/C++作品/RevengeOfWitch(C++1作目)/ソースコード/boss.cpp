//=================================================
//
//�V���Q���F�{�X������[boss.cpp]
//Author:ShinaTaiyo
//
//=================================================

//==============================
//�C���N���[�h
//==============================
#include "boss.h"
#include "manager.h"
#include "game.h"
#include "aimodel.h"
#include "edit.h"
#include "bullet.h"
#include "particle.h"
#include "texture.h"
#include "emitter.h"
#include "manager.h"
#include "objectXInfo.h"
#include "debugtext.h"
#include "stagemanager.h"
#include "eventmanager.h"
#include "calculation.h"
#include "player.h"
#include "attack.h"
#include "Ui3D.h"
#include "camera.h"
#include "marker.h"
#include "sound.h"
#include "damage.h"
#include "item.h"
//=================================================


//==============================
//�ÓI�����o������
//==============================

int CBoss::m_nNumFile = CBoss::BOSSTYPE_MAX; //�t�@�C���̐�
const char* CBoss::m_BOSS_FILENAME[CBoss::BOSSTYPE_MAX] =
{ "data\\MODEL\\Enemy\\Boss\\Boss00_KingSlime.x",
  "data\\MODEL\\Enemy\\Boss\\Boss01_DeathEnaga.x",
  "data\\MODEL\\Enemy\\Boss\\Boss02_MrStrong.x",
  "data\\MODEL\\Enemy\\Boss\\Boss03_Riley.x",
  "data\\MODEL\\Enemy\\Boss\\Boss04_Draiya.x",
  "data\\MODEL\\Enemy\\Boss\\Boss05_Gabriel.x",
};

//�f�X�G�i�K�̈ړ���ʒu
const D3DXVECTOR3 CDeathEnaga::m_MOVEPOINT[CDeathEnaga::m_nMAX_MOVEPOINT] =
{
	D3DXVECTOR3(-300.0f,250.0f,0.0f),D3DXVECTOR3(0.0f,250.0f,0.0f),D3DXVECTOR3(300.0f,250.0f,0.0f),
	D3DXVECTOR3(-300.0f,0.0f,0.0f),D3DXVECTOR3(0.0f,0.0f,0.0f),D3DXVECTOR3(300.0f,0.0f,0.0f),
};
//=================================================

//==============================
//�R���X�g���N�^
//==============================
CBoss::CBoss()
{

}
//=================================================

//==============================
//�f�X�g���N�^
//==============================
CBoss::~CBoss()
{

}
//=================================================

//==============================
//����������
//==============================
HRESULT CBoss::Init()
{
	//===========================
	//X�I�u�W�F�N�g������
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
	m_bUse = true;           //�g�p���
	m_pGauge = nullptr;      //�Q�[�W
	m_PurposePos = NULL_VECTOR3;//�ړI�̈ʒu

	//===========================
	//�s���p�^�[���֌W
	//===========================
	m_nPattern = 0;          //�p�^�[��
	m_nSubPattern = 0;       //�T�u�p�^�[���i�s���p�^�[���̒��ōX�ɕ����j
	m_nPatternTime = 0;      //���̃p�^�[���ɂȂ��Ă���̎���
	m_nSkill = 0;            //���݂̃X�L��
	m_bAction = false;       //�s�����Ă��邩�ǂ���
	m_nActionNum = 0;        //�s����
	m_nDefeatStagingTime = 0;//�|��鉉�o�̎���

	m_fSaveRot = 0.0f;       //������ۑ����Ă���
	m_bInsanity = false;     //�������Ă��邩�ǂ���
	m_bSpecialAttack = false;//�K�E�Z���g�����ǂ���
	m_nTurnCnt = 0;           //�^�[�������J�E���g
	//====================================

	//===========================
	//�ړ��p�^�[���֌W
	//===========================
	m_bUseMovePattern = true; //�ړ��p�^�[�����g���Ă��邩�ǂ���
	m_nMovePatternNum = 0;    //���݂̈ړ��p�^�[���̔ԍ�
	m_nMovePatternFrame = 0;//�ړ��p�^�[�����g���Ă���t���[����
	m_nMaxMovePatternFrame = 0;//�ړ��p�^�[���̍ő�t���[����
	//=======================================================================================

	SetUseGravity(true);

	CManager::GetSound()->StopSound();
	CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_BGM_BOSS);
	return S_OK;
}
//=================================================

//==============================
//�I������
//==============================
void CBoss::Uninit()
{
	CObjectX::Uninit();
	//SetDeath();//���S�t���O�ݒ�
}
//=================================================

//==============================
//�X�V����
//==============================
void CBoss::Update()
{
	if (CScene::GetMode() == CScene::MODE_GAME && CManager::GetEventManager()->GetEventType() != CEventManager::EVENTTYPE_BOSSAPPEAR)
	{
		m_nCntTime++;
		//==================================
		//�I�u�W�F�N�gX��������擾
		//==================================
		D3DXVECTOR3 &pos = GetPos();                                    //���f���̈ʒu�̎擾
		D3DXVECTOR3& Move = GetMove();                                  //�ړ��ʂ��擾
		D3DXVECTOR3 PosOld = GetPosOld();                               //1f�O�̈ʒu��ݒ�
		D3DXVECTOR3 & Rot = GetRot();                                   //���f���̌����̎擾
		D3DXVECTOR3 Size = GetSize();                                   //�T�C�Y���擾
		D3DXVECTOR3 Pos = CObjectX::GetPos();       
		D3DXVECTOR3 RandMove = NULL_VECTOR3;                            //�����ړ���  
		CParticle* pParticle = nullptr;                                 //�p�[�e�B�N���擾�p
		CObject* pLandingObj = nullptr;                                 //���ݏ���Ă���I�u�W�F�N�g�i�[�p	
		int& nLife = GetLife();                                         //�̗͂��擾
		int nLandingBlockNum = 0;                                       //�ǂ̃u���b�N�̏�ɏ���Ă��邩
		int nLandingBlockPriority = 0;                                  //����Ă���u���b�N�̃v���C�I���e�B
		bool bIsLiftLanding = false;                                    //���t�g�̏�ɏ���Ă��邩�ǂ���
		bool bDodgeState = false;                                       //�����Ԃ��ǂ���
		bool& bIsLanding = GetLanding();                                //�n�ʂɂ��邩�ǂ���
		bool& bIsJumping = GetJumping();                                //�W�����v���Ă��邩�ǂ���
		bool& bIsWalling = GetWalling();                                //�ǂɂ������Ă��邩�ǂ���
		bool& bUseGravity = GetUseGravity();                            //�d�͂��g�p���邩�ǂ���
		bool& bUseBlockCollision = GetUseBlockCollision();              //�u���b�N�Ƃ̓����蔻����g�p���邩�ǂ���
		bool bSuccessBlockCollisionX = false;                           //�u���b�N��X�����̓����蔻�肪�����������ǂ���
		bool bSuccessBlockCollisionY = false;                           //�u���b�N��Y�����̓����蔻�肪�����������ǂ���
		//===========================================================

		//=====================================
		//���_�ɓ��B�����Ƃ��̈ʒu�̕␳
		//=====================================
		AdjusOriginPosProcess();
		//===========================================================

		if (bUseGravity == true)
		{

			if (CGame::GetPlayer()->GetHp() < 1)
			{//�v���C���[�̗̑͂��Ȃ��Ȃ������߁A���S�t���O��ON�ɂ���
				SetUseDeath(true);
			}

			GravityProcess();
		}

		//==================================
		//�{�X�̍s��
		//==================================
		if (nLife > 0)
		{
			if (m_bUseMovePattern == true)
			{//�ړ��p�^�[�����Ă�
				MovePattern();
			}
			else
			{//�ړ��p�^�[�����Ă΂�Ă��Ȃ��Ƃ���
				ActionPattern();
			}
		}
		//===========================================================

		//==================================
		//X�I�u�W�F�N�g�X�V
		//==================================
		CObjectX::Update();
		//===========================================================


		//==================================
		//�����蔻��
		//==================================
		Collision();
		//===========================================================

		//==================================
		//�{�X�̔j��
		//==================================
		if (nLife <= 0)
		{
			Move = NULL_VECTOR3; //�������~�߂�
			m_nDefeatStagingTime++;//�|�����o�̎��Ԃ��v������
			DefeatStaging();//�������o
			if (m_nDefeatStagingTime >= m_nMAX_DEFEATSTAGINGTIME)
			{
				for (int nCnt = 0; nCnt < 120; nCnt++)
				{
					RandMove.x = float(rand() % 150 - 75) / 5;
					RandMove.y = float(rand() % 150 - 75) / 5;
					RandMove.z = float(rand() % 150 - 75) / 5;
					pParticle = CParticle::Create(CParticle::TYPE00_NORMAL, 120, 100.0f, 100.0f, Pos, RandMove, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), true);
					pParticle->SetGravity(-0.3f);//�d�͂�ݒ�
					pParticle = nullptr;
				}
				if (CDifficulty::GetDifficultyNum() != CDifficulty::DIFFICULTYTYPE_BOSSRUSH)
				{
					//��V
					CItem::Create(CItem::ITEMTYPE03_CROWN, D3DXVECTOR3(0.0f, 150.0f, 0.0f), D3DXVECTOR3(0.2f, 0.0f, 0.0f), ONE_VECTOR3 * 2.0f, 0);
				}
				else
				{
					BossRushProcess();
				}
				//�J���������ɖ߂�
				CManager::GetCamera()->SetCameraType(CCamera::CAMERATYPE_BESIDE);
				SetDeath();
			}
		}
		//===========================================================
	}
	else if(CScene::GetMode() == CScene::MODE_EDIT)
	{
		//==================================
		//X�I�u�W�F�N�g�X�V
		//==================================
		CObjectX::Update();
		//===========================================================
	}
}
//=================================================

//==============================
//�`�揈��
//==============================
void CBoss::Draw()
{
	CObjectX::Draw();

}
//=====================================================================================================================

//===============================================================
//���S�t���O��ݒ�
//===============================================================
void CBoss::SetDeath()
{
	if (GetUseDeath() == true)
	{
		if (m_pGauge != nullptr)
		{
			m_pGauge->SetUseDeath(true);
			m_pGauge->SetDeath();
			m_pGauge = nullptr;
		}
		CObject::SetDeath();
	}
}
//=====================================================================================================================

//===============================================================
//�_���[�W��^���鏈��
//===============================================================
void CBoss::SetDamage(int nDamage, int nHitStopTime)
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

		if (m_pGauge != nullptr)
		{
			m_pGauge->SetParam(nLife);
		}
		CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_DAMAGE_000);
		CDamage::Create(nDamage, GetPos(), NORMAL_COL, 40.0f, 40.0f);
	}
}
//=====================================================================================================================

//===============================
//�{�X�̐�������
//===============================
CBoss* CBoss::Create(BOSSTYPE type, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 Scale)
{
	CBoss* pBoss = DBG_NEW CBoss;   //�{�X�𐶐�
	bool bSuccess = pBoss->CObject::GetCreateSuccess();
	int nIdx = 0;//���f���̃C���f�b�N�X
	if (bSuccess == true)
	{
		if (pBoss != nullptr)
		{
			pBoss->Init();         //����������
			pBoss->SetUseDeath(true);                 //���S�t���O�𔭓����邩�ǂ�����ݒ肷��
			pBoss->m_bUse = true;  //�g�p���
			pBoss->m_type = type;  //�{�X�̎��
			pBoss->SetLife(nLife);                     //�̗�
			pBoss->SetMaxLife(nLife);//�ő�̗�
			pBoss->SetAutoSubLife(false);              //�����I�ɑ̗͂����炷���ǂ���
			pBoss->CObjectX::SetPos(pos);                                                                                  //�I�u�W�F�N�g�Q�c�̈ʒu��ݒ�
			pBoss->SetPosOld(pos);                                                                                         //1f�O�̈ʒu��ݒ�
			pBoss->SetSupportPos(pos);                                                                                     //�ݒu�ʒu
			pBoss->CObject::SetType(CObject::TYPE_BOSS);                                                                   //�I�u�W�F�N�g�̎�ނ����߂�
			pBoss->CObjectX::SetScale(Scale);                                                                              //�I�u�W�F�N�g�̊g�嗦
			nIdx = CManager::GetObjectXInfo()->Regist(CObjectX::OBJECTXTYPE_BOSS, m_BOSS_FILENAME[(int)(type)]);
			pBoss->CObjectX::BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(CObjectX::OBJECTXTYPE_BOSS, nIdx),
				CManager::GetObjectXInfo()->GetBuffMat(CObjectX::OBJECTXTYPE_BOSS, nIdx),
				CManager::GetObjectXInfo()->GetdwNumMat(CObjectX::OBJECTXTYPE_BOSS, nIdx),
				CManager::GetObjectXInfo()->GetTexture(CObjectX::OBJECTXTYPE_BOSS, nIdx),
				CManager::GetObjectXInfo()->GetColorValue(CObjectX::OBJECTXTYPE_BOSS, nIdx));                       //���f���������蓖�Ă�
			pBoss->SetBossHpGauge(nLife);                                                                              //�{�X�̗̑̓Q�[�W
			pBoss->CObjectX::SetObjXType(CObjectX::OBJECTXTYPE_BOSS);                                                      //�I�u�W�F�N�gX�̃^�C�v��ݒ�
			pBoss->CObjectX::SetTypeNum((int)(type));                                                                      //�I�u�W�F�N�gX���Ƃ̃^�C�v�ԍ���ݒ�
			pBoss->SetSize();                                                                                              //X�I�u�W�F�N�g�̃T�C�Y��ݒ肷��
			pBoss->CObjectX::SetRotType(CObjectX::ROTTYPE::ROTTYPE_NORMAL);                                                //�����̎�ނ�ݒ�
			pBoss->SetManagerType((int)(CStageManager::MANAGEROBJECT_BOSS));                                               //�}�l�[�W���[�ŌĂяo�����̎�ނ�ݒ�
		}
		else
		{//�I�u�W�F�N�g�ɋ󂫂��Ȃ������̂Ŕj������
			delete pBoss;
			pBoss = nullptr;
			return nullptr;
		}
	}
	return pBoss;
}
//===================================================================================================================================================

//====================================================
//�T�C�Y�𒲐�����
//====================================================
void CBoss::AdjustSize()
{

}
//===================================================================================================================================================

//====================================================
//�{�X��ݒ肷��
//====================================================
CObject * CBoss::SetCreateBoss(BOSSTYPE type, int nLife, D3DXVECTOR3 Pos, D3DXVECTOR3 Scale)
{
	CObject* pObj = nullptr;
	switch (type)
	{
	case BOSSTYPE00_KINGSLIME:
		pObj = CKingSlime::Create(CBoss::BOSSTYPE00_KINGSLIME, nLife, Pos, Scale);
		if (CScene::GetMode() == CScene::MODE_GAME)
		{
			CManager::GetEventManager()->SetBossAppearEvent(CEventManager::BOSSAPPEAR_KINGSLIME);
		}
		break;
	case BOSSTYPE01_DEATHENAGA:
		pObj = CDeathEnaga::Create(CBoss::BOSSTYPE01_DEATHENAGA, nLife, Pos, Scale);
		if (CScene::GetMode() == CScene::MODE_GAME)
		{
			CManager::GetEventManager()->SetBossAppearEvent(CEventManager::BOSSAPPEAR_DEATHENAGA);
		}
		break;
	case BOSSTYPE02_MRSTRONG:
		pObj = CMrStrong::Create(CBoss::BOSSTYPE02_MRSTRONG, nLife, Pos, Scale);
		if (CScene::GetMode() == CScene::MODE_GAME)
		{
			CManager::GetEventManager()->SetBossAppearEvent(CEventManager::BOSSAPPEAR_MRSTRONG);
		}
		break;
	case BOSSTYPE03_SNOWDEVIL_RILEY:
		pObj = CRiley::Create(CBoss::BOSSTYPE03_SNOWDEVIL_RILEY, nLife, Pos, Scale);
		if (CScene::GetMode() == CScene::MODE_GAME)
		{
			CManager::GetEventManager()->SetBossAppearEvent(CEventManager::BOSSAPPEAR_RILEY);
		}
		break;
	case BOSSTYPE04_DRAIYA:
		pObj = CDraiya::Create(CBoss::BOSSTYPE04_DRAIYA, nLife, Pos, Scale);
		if (CScene::GetMode() == CScene::MODE_GAME)
		{
			CManager::GetEventManager()->SetBossAppearEvent(CEventManager::BOSSAPPEAR_DRAIYA);
		}
		break;
	case BOSSTYPE05_GABRIEL:
		pObj = CGabriel::Create(CBoss::BOSSTYPE05_GABRIEL, nLife, Pos, Scale);
		if (CScene::GetMode() == CScene::MODE_GAME)
		{
			//CManager::GetEventManager()->SetBossAppearEvent(CEventManager::BOSSAPPEAR_GABRIEL);
		}
		break;
	default:
		break;
	}
    return pObj;
}
//===================================================================================================================================================

//====================================================
//�{�X�̗̑̓Q�[�W��ݒ肷��
//====================================================
void CBoss::SetBossHpGauge(int nLife)
{
	m_pGauge = CGauge::Create(CGauge::GAUGETYPE_BOSSHP, nLife, 700.0f, 10.0f, D3DXVECTOR3(SCREEN_WIDTH - 850.0f, 30.0f, 0.0f));
}
//===================================================================================================================================================

//====================================================
//�������o
//====================================================
void CBoss::DefeatStaging()
{
	D3DXVECTOR3 Pos = GetPos();//�ʒu
	D3DXVECTOR3 SummonPos = NULL_VECTOR3;//�����ʒu
	float fVXaim = 0.0f;//X�����x�N�g��
	float fVYaim = 0.0f;//Y�����x�N�g��
	float fVZaim = 0.0f;//Z�����x�N�g��
	float fVLaim = 0.0f;//�����x�N�g��
	float fRandSpeed = 0.0f;
	float fRatioColor = 0.0f;//�F�̊���
	CParticle* pParticle = nullptr;//�p�[�e�B�N���擾�p
	CManager::GetCamera()->SetCameraType(CCamera::CAMERATYPE_BOSSDEFEAT);

	SummonPos.x = Pos.x + float(rand() % 300 - 150);
	SummonPos.y = Pos.y + float(rand() % 300 - 150);
	SummonPos.z = Pos.z + float(rand() % 300 - 150);
	fRandSpeed = float(rand() % 300 + 1) / 10;
	//==========================
	//�R�����x�N�g�������
	//==========================
	fVXaim = Pos.x - SummonPos.x;
	fVYaim = Pos.y - SummonPos.y;
	fVZaim = Pos.z - SummonPos.z;

	D3DXVECTOR3 BulletMove = NULL_VECTOR3;

	fVLaim = sqrtf((fVXaim * fVXaim) + (fVYaim * fVYaim)
		+ (fVZaim * fVZaim));

	BulletMove.x = fVXaim / fVLaim * fRandSpeed;
	BulletMove.y = fVYaim / fVLaim * fRandSpeed;
	BulletMove.z = fVZaim / fVLaim * fRandSpeed;

	pParticle = CParticle::Create(CParticle::TYPE00_NORMAL, 30, 40.0f, 40.0f, SummonPos, BulletMove, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), true);
	pParticle->SetAddSpeed(0.99f);//�����ʂ�ݒ�
	pParticle = nullptr;
	//==================================================================================================================


	fRatioColor = (float)(m_nDefeatStagingTime) / (float)(m_nMAX_DEFEATSTAGINGTIME);
	SetOriginalColor(D3DXCOLOR(fRatioColor, 0.0f, 0.0f, 1.0f), 2);
}

//===============================
//�{�X�̍s��
//===============================
void CBoss::ActionPattern()
{
	m_nPatternTime++;
}
//===================================================================================================================================================

//===============================
//�ړ��p�^�[��
//===============================
void CBoss::MovePattern()
{
	m_nMovePatternFrame++;//�ړ��p�^�[�����g�p���Ă���t���[�������J�E���g����
}
//===================================================================================================================================================

//===============================
//���Z�b�g�p�^�[��
//===============================
void CBoss::ResetPattern()
{
	m_nActionNum = 0;
	m_nPattern = 0;
	m_nSubPattern = 0;
	m_nPatternTime = 0;
	m_bAction = false;
	m_bUseMovePattern = true;
}
//===================================================================================================================================================

//===========================================================
//�{�X���b�V���̏���
//===========================================================
void CBoss::BossRushProcess()
{
	switch (m_type)
	{
	case BOSSTYPE00_KINGSLIME:
		CMarker::Create(CMarker::MARKERTYPE00_WARP, D3DXVECTOR3(0.0f, 100.0f, 0.0f), ONE_VECTOR3, NULL_VECTOR3, 1);
		break;
	case BOSSTYPE01_DEATHENAGA:
		CMarker::Create(CMarker::MARKERTYPE00_WARP, D3DXVECTOR3(0.0f, 100.0f, 0.0f), ONE_VECTOR3, NULL_VECTOR3, 2);
		break;
	case BOSSTYPE02_MRSTRONG:
		CMarker::Create(CMarker::MARKERTYPE00_WARP, D3DXVECTOR3(0.0f, 100.0f, 0.0f), ONE_VECTOR3, NULL_VECTOR3, 3);
		break;
	case BOSSTYPE03_SNOWDEVIL_RILEY:
		CMarker::Create(CMarker::MARKERTYPE00_WARP, D3DXVECTOR3(0.0f, 100.0f, 0.0f), ONE_VECTOR3, NULL_VECTOR3, 4);
		break;
	case BOSSTYPE04_DRAIYA:
		CItem::Create(CItem::ITEMTYPE03_CROWN, D3DXVECTOR3(0.0f, 100.0f, 0.0f), NULL_VECTOR3, ONE_VECTOR3, 0);
		break;
	default:
		break;
	}
}
//===================================================================================================================================================

//===========================================================
//�����蔻��
//===========================================================
void CBoss::Collision()
{

}
//===================================================================================================================================================

//==========================================================
//���\������
//==========================================================
void CBoss::DispInfo()
{
	int& nMaxLife = GetMaxLife();
	CObjectX::DispInfo();
}
//===================================================================================================================================================

//==========================================================
//�ړ��p�^�[�������߂鏈��
//==========================================================
void CBoss::DecideMovePattern()
{

}
//===================================================================================================================================================

//============================================<<<�L���O�X���C���N���X>>>=============================================================================


//============================================================
//�R���X�g���N�^
//============================================================
CKingSlime::CKingSlime()
{

}
//===================================================================================================================================================

//============================================================
//�f�X�g���N�^
//============================================================
CKingSlime::~CKingSlime()
{

}
//===================================================================================================================================================

//============================================================
//����������
//============================================================
HRESULT CKingSlime::Init()
{
	CBoss::Init();
	return S_OK;
}
//===================================================================================================================================================

//============================================================
//�I������
//============================================================
void CKingSlime::Uninit()
{
	CBoss::Uninit();//�{�X�̏I������
}
//===================================================================================================================================================

//============================================================
//�X�V����
//============================================================
void CKingSlime::Update()
{
	CBoss::Update();//�{�X�̍X�V����
}
//===================================================================================================================================================

//============================================================
//�`�揈��
//============================================================
void CKingSlime::Draw()
{
	int nCntTime = GetCntTime();
	CBoss::Draw();
}
//===================================================================================================================================================

//============================================================
//���S�t���O��ݒ�
//============================================================
void CKingSlime::SetDeath()
{
	CBoss::SetDeath();
}
//===================================================================================================================================================

//============================================================
//��������
//============================================================
CKingSlime* CKingSlime::Create(BOSSTYPE type, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 Scale)
{
	CKingSlime* pKingSlime = DBG_NEW CKingSlime;   //�{�X�𐶐�
	bool bSuccess = pKingSlime->CObject::GetCreateSuccess();
	int nIdx = 0;//���f���̃C���f�b�N�X
	if (bSuccess == true)
	{
		if (pKingSlime != nullptr)
		{
			pKingSlime->Init();                             //����������
			pKingSlime->SetUseDeath(true);                  //���S�t���O�𔭓����邩�ǂ�����ݒ肷��
			pKingSlime->SetType(type);                      //�{�X�̎��
			pKingSlime->SetLife(nLife);                     //�̗�
			pKingSlime->SetMaxLife(nLife);//�ő�̗�
			pKingSlime->SetAutoSubLife(false);              //�����I�ɑ̗͂����炷���ǂ���
			pKingSlime->CObjectX::SetPos(pos);                                                                                  //�I�u�W�F�N�g�Q�c�̈ʒu��ݒ�
			pKingSlime->SetPosOld(pos);                                                                                         //1f�O�̈ʒu��ݒ�
			pKingSlime->SetSupportPos(pos);                                                                                     //�ݒu�ʒu
			pKingSlime->CObject::SetType(CObject::TYPE_BOSS);                                                                   //�I�u�W�F�N�g�̎�ނ����߂�
			pKingSlime->CObjectX::SetScale(Scale);                                                                              //�I�u�W�F�N�g�̊g�嗦
			pKingSlime->SetFormarScale(Scale);                                                                                  //���̊g�嗦��ݒ肷��
			nIdx = CManager::GetObjectXInfo()->Regist(CObjectX::OBJECTXTYPE_BOSS, m_BOSS_FILENAME[(int)(type)]);
			pKingSlime->CObjectX::BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(CObjectX::OBJECTXTYPE_BOSS, nIdx),
				CManager::GetObjectXInfo()->GetBuffMat(CObjectX::OBJECTXTYPE_BOSS, nIdx),
				CManager::GetObjectXInfo()->GetdwNumMat(CObjectX::OBJECTXTYPE_BOSS, nIdx),
				CManager::GetObjectXInfo()->GetTexture(CObjectX::OBJECTXTYPE_BOSS, nIdx),
				CManager::GetObjectXInfo()->GetColorValue(CObjectX::OBJECTXTYPE_BOSS, nIdx));                                   //���f���������蓖�Ă�
			pKingSlime->SetBossHpGauge(nLife);                                                                                  //�̗̓Q�[�W��ݒ肷��
			pKingSlime->CObjectX::SetObjXType(CObjectX::OBJECTXTYPE_BOSS);                                                      //�I�u�W�F�N�gX�̃^�C�v��ݒ�
			pKingSlime->CObjectX::SetTypeNum((int)(type));                                                                      //�I�u�W�F�N�gX���Ƃ̃^�C�v�ԍ���ݒ�
			pKingSlime->SetSize();                                                                                              //X�I�u�W�F�N�g�̃T�C�Y��ݒ肷��
			pKingSlime->CObjectX::SetRotType(CObjectX::ROTTYPE::ROTTYPE_NORMAL);                                                //�����̎�ނ�ݒ�
			pKingSlime->SetManagerType((int)(CStageManager::MANAGEROBJECT_BOSS));                                               //�}�l�[�W���[�ŌĂяo�����̎�ނ�ݒ�
		}
		else
		{//�I�u�W�F�N�g�ɋ󂫂��Ȃ������̂Ŕj������
			delete pKingSlime;
			pKingSlime = nullptr;
			return nullptr;
		}
	}
	return pKingSlime;
}
//===================================================================================================================================================

//============================================================
//�L���O�X���C���̍s���p�^�[��
//============================================================
void CKingSlime::ActionPattern()
{
	bool& bAction = GetAction();
	int& nSkill = GetSkill();

	if (bAction == false)
	{
		nSkill = rand() % 5;
		bAction = true;
	}

	if (bAction == true)
	{
		CBoss::ActionPattern();

		switch (nSkill)
		{
		case 0:
			Action0();
			break;
		case 1:
			Action1();
			break;
		case 2:
			Action2();
			break;
		case 3:
			Action3();
			break;
		case 4:
			Action4();
			break;
		default:
			break;
		}
	}

}
//===================================================================================================================================================

//============================================================
//�L���O�X���C���̃A�N�V�����O
//============================================================
void CKingSlime::Action0()
{
	int& nPatternTime = GetPatternTime();                           //�p�^�[�����Ԃ��擾����
	bool& bAction = GetAction();                                    //�s�����Ă��邩�ǂ������擾
	int& nPattern = GetPattern();                                   //���݂̃p�^�[�����擾

	D3DXVECTOR3& Pos = GetPos();
	D3DXVECTOR3& Move = GetMove();                                  //�ړ��ʂ��擾
	D3DXVECTOR3& PlayerPos = CGame::GetPlayer()->GetPos();
	switch (nPattern)
	{
	case 0:
		if (PlayerPos.x >= Pos.x)
		{
			Move.x = 2.2f;
		}
		else
		{
			Move.x = -2.2f;
		}

		if (nPatternTime == 60)
		{
			CParticle::SummonChargeParticle(CParticle::TYPE00_NORMAL, 60, 60, 2.0f, 30.0f, 30.0f, 200, 50, 10,true, Pos, NORMAL_COL, true);
		}

		if (nPatternTime >= 120)
		{
			nPattern++;
			nPatternTime = 0;
		}
		break;
	case 1:
		if (PlayerPos.x >= Pos.x)
		{
			nPattern++;
		}
		else
		{
			nPattern += 2;
		}
		nPatternTime = 0;
		break;
	case 2:
		Move.x = 10.0f;
		if (nPatternTime % 20 == 0)
		{
			CBullet::Create(CBullet::BULLETTYPE_ENEMY, CBullet::BULLETTEX_ENEMY, 90, 7, 20.0f, 20.0f, Pos, D3DXVECTOR3(sinf(0.0f) * 5.0f, cosf(0.0f) * 5.0f, 0.0f));
		}

		if (nPatternTime >= 50)
		{
			ResetPattern();
		}
		break;
	case 3:
		Move.x = -10.0f;
		if (nPatternTime % 20 == 0)
		{
			CBullet::Create(CBullet::BULLETTYPE_ENEMY, CBullet::BULLETTEX_ENEMY, 90, 7, 20.0f, 20.0f, Pos, D3DXVECTOR3(sinf(0.0f) * 5.0f, cosf(0.0f) * 5.0f, 0.0f));
		}

		if (nPatternTime >= 50)
		{
			ResetPattern();
		}
		break;
	default:
		break;
	}
}
//===================================================================================================================================================

//============================================================
//�L���O�X���C���̃A�N�V�����P
//============================================================
void CKingSlime::Action1()
{
	int& nPatternTime = GetPatternTime();                           //�p�^�[�����Ԃ��擾����
	bool& bAction = GetAction();                                    //�s�����Ă��邩�ǂ������擾
	bool& bIsLanding = GetLanding();                                //�n�ʂɂ��邩�ǂ������擾
	int& nPattern = GetPattern();                                   //���݂̃p�^�[�����擾
	int& nActionNum = GetActionNum();                               //�A�N�V�����񐔂��J�E���g
	D3DXVECTOR3& Pos = GetPos();
	D3DXVECTOR3& Move = GetMove();                                  //�ړ��ʂ��擾
	D3DXVECTOR3& PlayerPos = CGame::GetPlayer()->GetPos();
	float fRandRot = 0.0f;
	CBullet* pBullet = nullptr;
	switch (nPattern)
	{
	case 0:
		if (nPatternTime == 1)
		{
			CParticle::SummonChargeParticle(CParticle::TYPE00_NORMAL,100,100, 5.0f, 30.0f, 30.0f, 250, 50, 10,true, Pos, NORMAL_COL, true);
		}

		if (nPatternTime == 50)
		{
			nPattern++;
		}
		break;
	case 1:
		if (bIsLanding == true)
		{
			Pos = D3DXVECTOR3(PlayerPos.x, PlayerPos.y + 300.0f, PlayerPos.z);
			CParticle::SummonChargeParticle(CParticle::TYPE00_NORMAL, 40, 45, 5.0f, 30.0f, 30.0f, 150, 100, 10, false, Pos, NORMAL_COL, true);
			nPattern++;
		}
		break;
	case 2:
		CParticle::SummonParticle(CParticle::TYPE00_NORMAL, 40, 45, 30.0f, 30.0f, 100, 10, false,D3DXVECTOR3(Pos.x,0.0f,Pos.z), NORMAL_COL, true);
		nActionNum++;
		nPattern = 1;
		if (nActionNum == 5)
		{
			ResetPattern();
		}
		break;
	default:
		break;
	}
}
//===================================================================================================================================================

//============================================================
//�L���O�X���C���̃A�N�V�����Q
//============================================================
void CKingSlime::Action2()
{
	int& nPatternTime = GetPatternTime();                           //�p�^�[�����Ԃ��擾����
	bool& bAction = GetAction();                                    //�s�����Ă��邩�ǂ������擾
	int& nPattern = GetPattern();                                   //���݂̃p�^�[�����擾
	int& nActionNum = GetActionNum();                               //�A�N�V�����񐔂��J�E���g

	D3DXVECTOR3& Pos = GetPos();
	D3DXVECTOR3& Move = GetMove();                                  //�ړ��ʂ��擾
	D3DXVECTOR3& PlayerPos = CGame::GetPlayer()->GetPos();
	float fRandRot = 0.0f;
	D3DXVECTOR3 SummonPos = NULL_VECTOR3;
	CParticle* pParticle = nullptr;//�p�[�e�B�N���擾�p
	float fVXaim = 0.0f;//X�����x�N�g��
	float fVYaim = 0.0f;//Y�����x�N�g��
	float fVZaim = 0.0f;//Z�����x�N�g��
	float fVLaim = 0.0f;//�����x�N�g��
	switch (nPattern)
	{
	case 0:
		if (nPatternTime >= 100)
		{
			nPattern++;
			nPatternTime = 0;
		}
		else
		{
			SummonPos.x = Pos.x + float(rand() % 300 - 150);
			SummonPos.y = Pos.y + float(rand() % 300 - 150);
			SummonPos.z = Pos.z + float(rand() % 300 - 150);

			//==========================
			//�R�����x�N�g�������
			//==========================
			fVXaim = Pos.x - SummonPos.x;
			fVYaim = Pos.y - SummonPos.y;
			fVZaim = Pos.z - SummonPos.z;

			D3DXVECTOR3 BulletMove = NULL_VECTOR3;

			fVLaim = sqrtf((fVXaim * fVXaim) + (fVYaim * fVYaim)
				+ (fVZaim * fVZaim));

			BulletMove.x = fVXaim / fVLaim * 10.0f;
			BulletMove.y = fVYaim / fVLaim * 10.0f;
			BulletMove.z = fVZaim / fVLaim * 10.0f;

			pParticle = CParticle::Create(CParticle::TYPE00_NORMAL, 60, 40.0f, 40.0f, SummonPos, BulletMove, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), true);
			pParticle->SetAddSpeed(1.01f);//�����ʂ�ݒ�
			pParticle = nullptr;
			//==================================================================================================================

		}
		break;
	case 1:

		Pos.y += 0.1f;
		Move.y += 30.0f;
		nPattern++;
		nPatternTime = 0;
		break;
	case 2:
		if (Pos.y <= 0.0f)
		{
			nPattern++;
			nPatternTime = 0;
			CManager::GetCamera()->SetShake(50, 0.3f);
		}
		break;
	case 3:
		if (nPatternTime % 10 == 0 && nPatternTime <= 140)
		{
			SummonPos.x = Pos.x + float(rand() % 1200 - 600);
			SummonPos.y = Pos.y + 700.0f;
			SummonPos.z = Pos.z;
			CAttackEnemy::Create(CAttack::ATTACKTYPE01_METEO, 5, 120, SummonPos,
				D3DXVECTOR3(sinf(D3DX_PI) * 10.0f, cosf(D3DX_PI) * 10.0f, 0.0f), ONE_VECTOR3 * 2.0f, NULL_VECTOR3, CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, D3DXVECTOR3(0.1f, 0.1f, 0.1f), true, 10);
		}

		if (nPatternTime >= 240)
		{
			ResetPattern();
		}
		break;
	}
}
//===================================================================================================================================================

//============================================================
//�L���O�X���C���̃A�N�V�����R
//============================================================
void CKingSlime::Action3()
{
	int& nPatternTime = GetPatternTime();                           //�p�^�[�����Ԃ��擾����
	bool& bAction = GetAction();                                    //�s�����Ă��邩�ǂ������擾
	int& nPattern = GetPattern();                                   //���݂̃p�^�[�����擾
	int& nActionNum = GetActionNum();                               //�A�N�V�����񐔂��J�E���g

	D3DXVECTOR3& Move = GetMove();                                  //�ړ��ʂ��擾
	D3DXVECTOR3& Pos = GetPos();
	D3DXVECTOR3& PlayerPos = CGame::GetPlayer()->GetPos();
	D3DXVECTOR3& Rot = GetRot();
	D3DXVECTOR3 GoalPos = D3DXVECTOR3(0.0f, 0.0f, 500.0f);
	D3DXVECTOR3 BulletMove = NULL_VECTOR3;
	float fVXaim = 0.0f;//X�����x�N�g��
	float fVYaim = 0.0f;//Y�����x�N�g��
	float fVZaim = 0.0f;//Z�����x�N�g��
	float fVLaim = 0.0f;//�����x�N�g��
	switch (nPattern)
	{
	case 0:
		if (nPatternTime >= 50)
		{
			Rot.y = D3DX_PI;
			nPattern++;
			nPatternTime = 0;
		}
		break;
	case 1:
		//==========================
		//�ړI�̈ʒu�܂ł̂R�����x�N�g�������
		//==========================
		fVXaim = GoalPos.x - Pos.x;
		fVYaim = GoalPos.y - Pos.y;
		fVZaim = GoalPos.z - Pos.z;

		fVLaim = sqrtf((fVXaim * fVXaim) + (fVYaim * fVYaim)
			+ (fVZaim * fVZaim));

		Move.x = fVXaim / fVLaim * 5.0f;
		Move.y = fVYaim / fVLaim * 5.0f;
		Move.z = fVZaim / fVLaim * 5.0f;

		if (Pos.x >= GoalPos.x - 20.0f && Pos.x <= GoalPos.x + 20.0f &&
			Pos.y >= GoalPos.y - 20.0f && Pos.y <= GoalPos.y + 20.0f &&
			Pos.z >= GoalPos.z - 20.0f && Pos.z <= GoalPos.z + 20.0f)
		{
			Pos = GoalPos;
			Rot.y = 0.0f;
			nPattern++;
			nPatternTime = 0;
		}
		//==================================================================================================================
		break;
	case 2:
		//==========================
		//�ړI�̈ʒu�܂ł̂R�����x�N�g�������
		//==========================
		fVXaim = PlayerPos.x - Pos.x;
		fVYaim = PlayerPos.y - Pos.y;
		fVZaim = PlayerPos.z - Pos.z;

		fVLaim = sqrtf((fVXaim * fVXaim) + (fVYaim * fVYaim)
			+ (fVZaim * fVZaim));

		BulletMove.x = fVXaim / fVLaim * 15.0f;
		BulletMove.y = fVYaim / fVLaim * 15.0f;
		BulletMove.z = fVZaim / fVLaim * 15.0f;

		if (nPatternTime % 3 == 0)
		{
			CBullet::Create(CBullet::BULLETTYPE_ENEMY, CBullet::BULLETTEX_ENEMY, 120, 13, 60.0f, 60.0f, Pos, BulletMove);
		}

		if (nPatternTime >= 180)
		{
			nPattern++;
			nPatternTime = 0;
		}
		//==================================================================================================================
		break;
	case 3:
		if (nPatternTime == 1)
		{
			Move.y = 50.0f;
		}

		if (Pos.y >= 300.0f)
		{

			if (nPatternTime < 180)
			{
				Move.y = 0.0f;
				Pos.y = 300.0f;
				//==========================
				//�ړI�̈ʒu�܂ł̂R�����x�N�g�������
				//==========================
				fVXaim = PlayerPos.x - Pos.x;
				fVYaim = PlayerPos.y - Pos.y;
				fVZaim = PlayerPos.z - Pos.z;

				fVLaim = sqrtf((fVXaim * fVXaim) + (fVYaim * fVYaim)
					+ (fVZaim * fVZaim));

				Rot.x = fVXaim / fVLaim * 20.0f;
				Rot.y = fVYaim / fVLaim * 20.0f;
				Rot.z = fVZaim / fVLaim * 20.0f;
			}
		}
		if (nPatternTime >= 180)
		{
			Move = Rot;

			if (Pos.z >= -20.0f && Pos.z <= 20.0f)
			{
				Move = NULL_VECTOR3;
				nPattern++;
				nPatternTime = 0;
				Pos.z = 0.0f;

				CAttackEnemy::Create(CAttack::ATTACKTYPE03_TORNADO, 11, 250, Pos, D3DXVECTOR3(sinf(D3DX_PI * 0.5f) * 7.0f, cosf(D3DX_PI * 0.5f) * 7.0f, 0.0f), ONE_VECTOR3 * 0.8f,
					NULL_VECTOR3, CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, D3DXVECTOR3(0.0f, 0.3f, 0.0f), true, 10);
				CAttackEnemy::Create(CAttack::ATTACKTYPE03_TORNADO, 11, 250, Pos, D3DXVECTOR3(sinf(D3DX_PI * -0.5f) * 7.0f, cosf(D3DX_PI * -0.5f) * 7.0f, 0.0f), ONE_VECTOR3 * 0.8f,
					NULL_VECTOR3, CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, D3DXVECTOR3(0.0f, -0.3f, 0.0f), true, 10);

			}
		}
		//==================================================================================================================
		break;
	case 4:
		if (nPatternTime >= 90)
		{
			ResetPattern();
			Rot = NULL_VECTOR3;
		}
		break;
	default:
		break;
	}
}
//===================================================================================================================================================

//============================================================
//�L���O�X���C���̃A�N�V�����S
//============================================================
void CKingSlime::Action4()
{
	int& nPatternTime = GetPatternTime();                           //�p�^�[�����Ԃ��擾����
	bool& bAction = GetAction();                                    //�s�����Ă��邩�ǂ������擾
	int& nPattern = GetPattern();                                   //���݂̃p�^�[�����擾
	int& nActionNum = GetActionNum();                               //�A�N�V�����񐔂��J�E���g

	D3DXVECTOR3& Pos = GetPos();
	D3DXVECTOR3& PlayerPos = CGame::GetPlayer()->GetPos();
	float fRandRot = 0.0f;
	float fRot = 0.0f;
	D3DXVECTOR3 SummonPos = NULL_VECTOR3;
	D3DXVECTOR3 RandMove = NULL_VECTOR3;
	CParticle* pParticle = nullptr;//�p�[�e�B�N���擾�p
	CAttackEnemy* pAttackEnemy = nullptr;//�G�U���擾�p
	switch (nPattern)
	{
	case 0:
		if (nPatternTime >= 60)
		{
			for (int nCnt = 0; nCnt < 50; nCnt++)
			{
				RandMove.x = float(rand() % 150 - 75) / 5;
				RandMove.y = float(rand() % 150 - 75) / 5;
				RandMove.z = float(rand() % 150 - 75) / 5;
				pParticle = CParticle::Create(CParticle::TYPE00_NORMAL, 120, 60.0f, 60.0f, Pos, RandMove, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), true);
				pParticle->SetGravity(-0.2f);//�d�͂�ݒ�
				pParticle = nullptr;
			}

			nPattern++;
			nPatternTime = 0;
		}
		break;
	case 1:
		if (nPatternTime >= 60)
		{
			nPatternTime = 0;
			nPattern++;
		}
		break;
	case 2:
		if (nPatternTime == 1)
		{
			for (int nCnt = 0; nCnt < 6; nCnt++)
			{
				fRot = (D3DX_PI / 5) * nCnt;
				pAttackEnemy = CAttackEnemy::Create(CAttack::ATTACKTYPE02_NEEDLE, 8, 55, Pos, NULL_VECTOR3, D3DXVECTOR3(1.0f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, -D3DX_PI * 0.5f + fRot),
					CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, NULL_VECTOR3, true, 10);

				pAttackEnemy->SetUseAddScale(D3DXVECTOR3(0.0f, 0.1f, 0.0f),true);
			}
		}
		if (nPatternTime >= 80)
		{
			ResetPattern();
		}
		break;
	default:
		break;
	}
}
//================================================================================================================================

//=========================================================
//�ړ��p�^�[��
//=========================================================
void CKingSlime::MovePattern()
{
	CBoss::MovePattern();//�ړ��p�^�[���̋��ʏ���
	int nMovePatternFrame = GetMovePatternFrame();
	bool& bUseMovePattern = GetUseMovePattern();//�ړ��p�^�[�����g�p���Ă��邩�ǂ���
	D3DXVECTOR3& Move = GetMove();
	
	Move.x = 0.0f;//�ړ��p�^�[����Ԃ̂Ƃ��ɓ������~�߂�
	if (nMovePatternFrame > m_nMAX_MOVEPATTERNFRAME)
	{
		bUseMovePattern = false;
		nMovePatternFrame = 0;
	}
}
//================================================================================================================================

//============================================<<<�f�X�G�i�K�N���X>>>=============================================================================

//========================================================
//�R���X�g���N�^
//========================================================
CDeathEnaga::CDeathEnaga()
{

}
//================================================================================================================================

//========================================================
//�f�X�g���N�^
//========================================================
CDeathEnaga::~CDeathEnaga()
{

}
//================================================================================================================================

//========================================================
//����������
//========================================================
HRESULT CDeathEnaga::Init()
{
	CBoss::Init();            //�{�X�̏���������
	m_nPurposeMovePoint = 0;  //�ړ���̔ԍ�
	m_fMoveLength = 0.0f;     //�ړ����鋗��

	for (int nCnt = 0; nCnt < m_nMAX_TWINTORNADO; nCnt++)
	{
		m_apTwinTornado[nCnt] = nullptr;
	}
	return S_OK;
}
//================================================================================================================================

//========================================================
//�I������
//========================================================
void CDeathEnaga::Uninit()
{
	CBoss::Uninit();
}
//================================================================================================================================

//========================================================
//�X�V����
//========================================================
void CDeathEnaga::Update()
{
	CBoss::Update();
}
//================================================================================================================================

//========================================================
//�`�揈��
//========================================================
void CDeathEnaga::Draw()
{
	int nCntTime = GetCntTime();
	CBoss::Draw();
}
//================================================================================================================================

//========================================================
//���S�t���O��ݒ�
//========================================================
void CDeathEnaga::SetDeath()
{
	for (int nCnt = 0; nCnt < m_nMAX_TWINTORNADO; nCnt++)
	{
		if (m_apTwinTornado[nCnt] != nullptr)
		{
			m_apTwinTornado[nCnt]->SetUseDeath(true);
			m_apTwinTornado[nCnt]->SetDeath();
			m_apTwinTornado[nCnt] = nullptr;
		}
	}

	CBoss::SetDeath();
}
//================================================================================================================================

//========================================================
//��������
//========================================================
CDeathEnaga* CDeathEnaga::Create(BOSSTYPE type, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 Scale)
{
	CDeathEnaga* pDeathEnaga = DBG_NEW CDeathEnaga;   //�{�X�𐶐�
	bool bSuccess = pDeathEnaga->CObject::GetCreateSuccess();
	int nIdx = 0;//���f���̃C���f�b�N�X
	if (bSuccess == true)
	{
		if (pDeathEnaga != nullptr)
		{
			pDeathEnaga->Init();                             //����������
			pDeathEnaga->SetUseDeath(true);                  //���S�t���O�𔭓����邩�ǂ�����ݒ肷��
			pDeathEnaga->SetType(type);                      //�{�X�̎��
			pDeathEnaga->SetLife(nLife);                     //�̗�
			pDeathEnaga->SetUseGravity(false);               //�d�͂��g��Ȃ�
			pDeathEnaga->SetUseBlockCollision(false);        //�u���b�N�Ƃ̓����蔻����g��Ȃ�
			pDeathEnaga->SetMaxLife(nLife);                  //�ő�̗�
			pDeathEnaga->SetAutoSubLife(false);              //�����I�ɑ̗͂����炷���ǂ���
			pDeathEnaga->CObjectX::SetPos(pos);                                                                                  //�I�u�W�F�N�g�Q�c�̈ʒu��ݒ�
			pDeathEnaga->SetPosOld(pos);                                                                                         //1f�O�̈ʒu��ݒ�
			pDeathEnaga->SetSupportPos(pos);                                                                                     //�ݒu�ʒu
			pDeathEnaga->CObject::SetType(CObject::TYPE_BOSS);                                                                   //�I�u�W�F�N�g�̎�ނ����߂�
			pDeathEnaga->CObjectX::SetScale(Scale);                                                                              //�I�u�W�F�N�g�̊g�嗦
			pDeathEnaga->SetFormarScale(Scale);                                                                                  //���̊g�嗦��ݒ肷��
			nIdx = CManager::GetObjectXInfo()->Regist(CObjectX::OBJECTXTYPE_BOSS, m_BOSS_FILENAME[(int)(type)]);
			pDeathEnaga->CObjectX::BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(CObjectX::OBJECTXTYPE_BOSS, nIdx),
				CManager::GetObjectXInfo()->GetBuffMat(CObjectX::OBJECTXTYPE_BOSS, nIdx),
				CManager::GetObjectXInfo()->GetdwNumMat(CObjectX::OBJECTXTYPE_BOSS, nIdx),
				CManager::GetObjectXInfo()->GetTexture(CObjectX::OBJECTXTYPE_BOSS, nIdx),
				CManager::GetObjectXInfo()->GetColorValue(CObjectX::OBJECTXTYPE_BOSS, nIdx));                                   //���f���������蓖�Ă�
			pDeathEnaga->SetBossHpGauge(nLife);                                                                                  //�̗̓Q�[�W��ݒ肷��
			pDeathEnaga->CObjectX::SetObjXType(CObjectX::OBJECTXTYPE_BOSS);                                                      //�I�u�W�F�N�gX�̃^�C�v��ݒ�
			pDeathEnaga->CObjectX::SetTypeNum((int)(type));                                                                      //�I�u�W�F�N�gX���Ƃ̃^�C�v�ԍ���ݒ�
			pDeathEnaga->SetSize();                                                                                              //X�I�u�W�F�N�g�̃T�C�Y��ݒ肷��
			pDeathEnaga->CObjectX::SetRotType(CObjectX::ROTTYPE::ROTTYPE_NORMAL);                                                //�����̎�ނ�ݒ�
			pDeathEnaga->SetManagerType((int)(CStageManager::MANAGEROBJECT_BOSS));                                               //�}�l�[�W���[�ŌĂяo�����̎�ނ�ݒ�
		}
		else
		{//�I�u�W�F�N�g�ɋ󂫂��Ȃ������̂Ŕj������
			delete pDeathEnaga;
			pDeathEnaga = nullptr;
			return nullptr;
		}
	}
	return pDeathEnaga;
}
//================================================================================================================================

//========================================================
//�ړ��p�^�[��
//========================================================
void CDeathEnaga::MovePattern()
{
	int& nMovePatternFrame = GetMovePatternFrame();
	int& nMaxMovePatternFrame = GetMaxMovePatternFrame();//�ړ��p�^�[���̍ő�t���[����
	bool& bUseMovePattern = GetUseMovePattern();//�ړ��p�^�[�����g�p���Ă��邩�ǂ���
	D3DXVECTOR3& Move = GetMove();
	D3DXVECTOR3 Pos = GetPos();
	float fLength = 0.0f;
	float fVLaim = 0.0f;//�����x�N�g��
	float fVXaim = 0.0f;//X�����x�N�g��
	float fVYaim = 0.0f;//Y�����x�N�g��
	CEmitter* pEmitter = nullptr;
	CBoss::MovePattern();//�ړ��p�^�[���̋��ʏ���

	int nNotPurpose = m_nPurposeMovePoint;//�O��ړ������ړI�n�̔ԍ����L�^����

	if (nMovePatternFrame == 1)
	{//�ړ��p�^�[���J�n���̏���
		nMaxMovePatternFrame = 100;//�ړ��p�^�[���̍ő�t���[������ݒ�

		while (1)
		{
			m_nPurposeMovePoint = rand() % m_nMAX_MOVEPOINT;//�ړ����ݒ肷��
			if (m_nPurposeMovePoint != nNotPurpose)
			{//�ړ��ꏊ�����Ȃ��悤�ɂ���
				break;
			}
		}

		//�ړ��ʒu�܂ł̋����𑪂�
		fLength = sqrtf(powf(m_MOVEPOINT[m_nPurposeMovePoint].x - Pos.x, 2) +
			powf(m_MOVEPOINT[m_nPurposeMovePoint].y - Pos.y, 2));

		m_fMoveLength = fLength / (float)(nMaxMovePatternFrame);//�ړ�������t���[�����Ŋ���A1f������̈ړ��ʂ����߂�

		//���[�v��ɃG�~�b�^�[����������
		pEmitter = CEmitter::Create((int)(CEffect::EFFECTTYPE00_NORMAL), nMaxMovePatternFrame, 80, 80.0f, 40.0f, 40.0f,
			m_MOVEPOINT[m_nPurposeMovePoint], NULL_VECTOR3, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		pEmitter->SetUseCurve(0.0f, 10.0f, 0.2f);
	}

	//XY�x�N�g�������
	fVXaim = m_MOVEPOINT[m_nPurposeMovePoint].x - Pos.x;
	fVYaim = m_MOVEPOINT[m_nPurposeMovePoint].y - Pos.y;

	//�����x�N�g�������߂�
	fVLaim = atan2f(fVXaim, fVYaim);

	//�ړI�̈ʒu�܂ňړ�������
	Move.x = sinf(fVLaim) * m_fMoveLength;
	Move.y = cosf(fVLaim) * m_fMoveLength;

	//�ړ��p�^�[�����I��点��
	if (nMovePatternFrame > nMaxMovePatternFrame)
	{
		bUseMovePattern = false;
		nMovePatternFrame = 0;
		Move = NULL_VECTOR3;
	}
}
//================================================================================================================================

//========================================================
//�s���p�^�[��
//========================================================
void CDeathEnaga::ActionPattern()
{
	bool& bAction = GetAction();    //�s�����Ă��邩�ǂ���
	int& nSkill = GetSkill();       //�X�L��
	bool& bInsanity = GetInsanity();//�������Ă��邩�ǂ���
	int nLife = GetLife();          //�̗�
	int& nCntTurn = GetTurnCnt();   //�^�[�������J�E���g
	int nMaxLife = GetMaxLife();    //�ő�̗�
	float fRatioLife = 0.0f;        //�̗͂̊���

	fRatioLife = float(nLife) / float(nMaxLife);

	if (fRatioLife < 0.5f)
	{//�̗͂��T�O������������甭����Ԃɂ���
		bInsanity = true;
	}

	if (bAction == false)
	{
		if (bInsanity == false)
		{//������Ԃ���Ȃ����
			nSkill = rand() % 3;
		}
		else
		{//������ԂȂ�
			nCntTurn++;//��������^�[�������J�E���g
			nSkill = rand() % 5;
			if (nCntTurn == 1)
			{//�������Ă���ŏ��̃^�[���ɕK�E�Z�𔭓��I
				nSkill = 4;
			}
		}
		bAction = true;
	}

	if (bAction == true)
	{
		CBoss::ActionPattern();
		switch (nSkill)
		{
		case 0:
			Action0();
			break;
		case 1:
			Action1();
			break;
		case 2:
			Action2();
			break;
		case 3:
			Action3();
			break;
		case 4:
			Action4();
			break;
		default:
			break;
		}
	}

}
//================================================================================================================================

//========================================================
//�s���O
//========================================================
void CDeathEnaga::Action0()
{
	int& nPatternTime = GetPatternTime();                           //�p�^�[�����Ԃ��擾����
	bool& bAction = GetAction();                                    //�s�����Ă��邩�ǂ������擾
	int& nPattern = GetPattern();                                   //���݂̃p�^�[�����擾
	int& nActionNum = GetActionNum();                               //�A�N�V�����񐔂��J�E���g

	D3DXVECTOR3& Move = GetMove();                                 //�ړ���
	D3DXVECTOR3 SenterPos = GetSenterPos();                              //�ʒu
	CBullet* pBullet = nullptr;                                    //�o���b�g�ւ̃|�C���^
	float fVLaim = 0.0f;
	float fVXaim = 0.0f;
	float fVYaim = 0.0f;

	D3DXVECTOR3 PlayerPos = CGame::GetPlayer()->GetPos() + D3DXVECTOR3(0.0f, CGame::GetPlayer()->GetSize().y / 2.0f, 0.0f);//�v���C���[�̐^��
	fVXaim = PlayerPos.x - SenterPos.x;
	fVYaim = PlayerPos.y - SenterPos.y;

	fVLaim = atan2f(fVXaim, fVYaim);

	switch (nPattern)
	{
	case 0:
		if (nPatternTime % 25 == 0 && nPatternTime <= 75)
		{
		    CBullet::Create(CBullet::BULLETTYPE_ENEMY, CBullet::BULLETTEX_ENEMY, 120, 1, 40.0f, 40.0f, SenterPos,
				D3DXVECTOR3(sinf(fVLaim) * 5.0f, cosf(fVLaim) * 5.0f, 0.0f));

			if (nPatternTime == 75)
			{
				CParticle::SummonChargeParticle(CParticle::TYPE00_NORMAL, 80, 120, 5.0f, 30.0f, 30.0f, 200, 100, 10, false, SenterPos, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f)
					, true);
			}
		}

		if (nPatternTime == 100)
		{
			for (int nCnt = 0; nCnt < 3; nCnt++)
			{
				pBullet = CBullet::Create(CBullet::BULLETTYPE_ENEMY, CBullet::BULLETTEX_ENEMY, 120, 1, 40.0f, 40.0f, SenterPos,
					D3DXVECTOR3(sinf(fVLaim - 0.6f + (0.6f * (float)(nCnt))) * 2.0f, 
						cosf(fVLaim - 0.6f + (0.6f * (float)(nCnt))) * 2.0f, 0.0f));

				pBullet->SetUseAddSpeed(true, true, 1.05f);
				pBullet->SetUseEffect(30, D3DXVECTOR2(40.0f, 40.0f), D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f));
			}
			nPatternTime = 0;//���ԃ��Z�b�g
			nPattern++;      //�p�^�[����i�߂�
		}
		break;
	case 1:
		if (nPatternTime == 50)
		{
			ResetPattern();
		}
		break;
	default:
		break;
	}

}
//================================================================================================================================

//========================================================
//�s���P
//========================================================
void CDeathEnaga::Action1()
{
	int& nPatternTime = GetPatternTime();         //�p�^�[�����Ԃ��擾����
	bool& bAction = GetAction();                  //�s�����Ă��邩�ǂ������擾
	int& nPattern = GetPattern();                 //���݂̃p�^�[�����擾
	int& nActionNum = GetActionNum();             //�A�N�V�����񐔂��J�E���g

	D3DXVECTOR3& Pos = GetPos();                  //�ʒu���擾
	D3DXVECTOR3& Move = GetMove();                //�ړ��ʂ��擾
	CBullet* pBullet = nullptr;                   //�o���b�g�ւ̃|�C���^

	float fVXaim = 0.0f;                          //X�x�N�g��
	float fVYaim = 0.0f;                          //Y�x�N�g��
	float fVLaim = 0.0f;            			  //�����x�N�g��

	int nRand = rand() % 2;                       //����
	switch (nPattern)
	{
	case 0://���߂�
		if (nPatternTime == 30)
		{//�ԃ`���[�W
			CParticle::SummonChargeParticle(CParticle::TYPE00_NORMAL, 180, 120, 5.0f, 30.0f, 30.0f, 300, 100, 10, false,
				Pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), true);

			nPatternTime = 0;
			nPattern++;
		}
		break;
	case 1://���ߊ���
		if (nPatternTime == 30)
		{
			nPatternTime = 0;
			nPattern++;
		}
		break;
	case 2://�o���b�g����
		for (int nCnt = 0; nCnt < 2; nCnt++)
		{
			float fRot = (2.0f / 2.0f) * float(nCnt);
			pBullet = CBullet::Create(CBullet::BULLETTYPE_ENEMY, CBullet::BULLETTEX_ENEMY, 180, 1, 40.0f, 40.0f,
				Pos + D3DXVECTOR3(0.0f, GetSize().y / 2.0f, 0.0f), NULL_VECTOR3);

			if (nRand  == 0)
			{
				pBullet->SetUseLengthCurve(true, D3DX_PI * fRot, 0.02f, 3.0f);
			}
			else
			{
				pBullet->SetUseLengthCurve(true, D3DX_PI * fRot, -0.02f, 3.0f);
			}
		}
		nPattern++;
		nPatternTime = 0;
		break;
	case 3://�ːi
		if (nPatternTime == 1)
		{
			Pos = D3DXVECTOR3(-500.0f,CGame::GetPlayer()->GetPos().y, 0.0f);
		}

		if (nPatternTime >= 40)
		{
			Move.x = 30.0f;
		}

		if (nPatternTime == 70)
		{
			Move.x = 0.0f;
			nPatternTime = 0;
			nPattern++;
		}
		break;
	case 4://�ːi
		if (nPatternTime == 1)
		{
			Pos = D3DXVECTOR3(500.0f, CGame::GetPlayer()->GetPos().y, 0.0f);
		}
		if (nPatternTime >= 30)
		{
			Move.x = -30.0f;
		}

		if (nPatternTime == 70)
		{
			Pos = NULL_VECTOR3 + D3DXVECTOR3(0.0f, 250.0f, 0.0f);
			CParticle::SummonParticle(CParticle::TYPE00_NORMAL, 40, 45, 30.0f, 30.0f, 100, 10, false,Pos, NORMAL_COL, true);
			ResetPattern();
		}
		break;
	default:
		break;
	}
}
//================================================================================================================================

//========================================================
//�s���Q
//========================================================
void CDeathEnaga::Action2()
{
	int& nPatternTime = GetPatternTime();         //�p�^�[�����Ԃ��擾����
	bool& bAction = GetAction();                  //�s�����Ă��邩�ǂ������擾
	int& nPattern = GetPattern();                 //���݂̃p�^�[�����擾
	int& nActionNum = GetActionNum();             //�A�N�V�����񐔂��J�E���g

	D3DXVECTOR3& Pos = GetPos();                  //�ʒu���擾
	D3DXVECTOR3& Move = GetMove();                //�ړ��ʂ��擾
	CBullet* pBullet = nullptr;                   //�o���b�g�ւ̃|�C���^

	switch (nPattern)
	{
	case 0://���߂�
		if (nPatternTime == 30)
		{//�`���[�W
			CParticle::SummonChargeParticle(CParticle::TYPE00_NORMAL, 180, 120, 5.0f, 30.0f, 30.0f, 300, 100, 10, false,
				Pos, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), true);

			nPatternTime = 0;
			nPattern++;
		}
		break;
	case 1://�g���l�[�h����
		if (nPatternTime == 30)
		{
			for (int nCntBullet = 0; nCntBullet < 5; nCntBullet++)
			{
				pBullet = CBullet::Create(CBullet::BULLETTYPE_ENEMY, CBullet::BULLETTEX_ENEMY, 180, 1, 20.0f, 20.0f, Pos + D3DXVECTOR3(0.0f, GetSize().y / 2.0f, 0.0f),
					D3DXVECTOR3(sinf(-0.6f + 0.3f * nCntBullet) * 10.0f, cosf(-0.6f + 0.3f * nCntBullet), 0.0f));

				pBullet->SetUseGravity(-0.4f);
			}

			for (int nCnt = 0; nCnt < m_nMAX_TWINTORNADO; nCnt++)
			{
				switch (nCnt)
				{
				case 0:
					m_apTwinTornado[nCnt] = CAttackEnemy::Create(CAttack::ATTACKTYPE03_TORNADO, 1, 180,
						Pos + D3DXVECTOR3(0.0f, GetSize().y / 2.0f, 0.0f), D3DXVECTOR3(sinf(0.1f) * 5.0f, cosf(0.1f) * 5.0f, 0.0f)
						, ONE_VECTOR3 * 1.0f, NULL_VECTOR3, CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, D3DXVECTOR3(0.0f, 0.4f, 0.0f), false, 10);
					break;
				case 1:
					m_apTwinTornado[nCnt] = CAttackEnemy::Create(CAttack::ATTACKTYPE03_TORNADO, 1, 180,
						Pos + D3DXVECTOR3(0.0f, GetSize().y / 2.0f, 0.0f), D3DXVECTOR3(sinf(-0.1f) * 5.0f, cosf(-0.1f) * 5.0f, 0.0f)
						, ONE_VECTOR3 * 1.0f, NULL_VECTOR3, CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, D3DXVECTOR3(0.0f, 0.4f, 0.0f), false, 10);
					break;
				case 2:
					m_apTwinTornado[nCnt] = CAttackEnemy::Create(CAttack::ATTACKTYPE03_TORNADO, 1, 180,
						Pos + D3DXVECTOR3(0.0f, GetSize().y / 2.0f, 0.0f), D3DXVECTOR3(sinf(0.1f + D3DX_PI) * 5.0f, cosf(0.1f + D3DX_PI) * 5.0f, 0.0f)
						, ONE_VECTOR3 * 1.0f, NULL_VECTOR3, CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, D3DXVECTOR3(0.0f, 0.4f, 0.0f), false, 10);
					break;
				case 3:
					m_apTwinTornado[nCnt] = CAttackEnemy::Create(CAttack::ATTACKTYPE03_TORNADO, 1, 180,
						Pos + D3DXVECTOR3(0.0f, GetSize().y / 2.0f, 0.0f), D3DXVECTOR3(sinf(-0.1f + D3DX_PI) * 5.0f, cosf(-0.1f + D3DX_PI) * 5.0f, 0.0f)
						, ONE_VECTOR3 * 1.0f, NULL_VECTOR3, CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, D3DXVECTOR3(0.0f, 0.4f, 0.0f), false, 10);
					break;
				case 4:
					m_apTwinTornado[nCnt] = CAttackEnemy::Create(CAttack::ATTACKTYPE03_TORNADO, 1, 180,
						Pos + D3DXVECTOR3(0.0f, GetSize().y / 2.0f, 0.0f), D3DXVECTOR3(sinf(0.1f) * 10.0f, cosf(0.1f) * 10.0f, 0.0f)
						, ONE_VECTOR3 * 1.0f, NULL_VECTOR3, CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, D3DXVECTOR3(0.0f, 0.4f, 0.0f), false, 10);
					break;
				case 5:
					m_apTwinTornado[nCnt] = CAttackEnemy::Create(CAttack::ATTACKTYPE03_TORNADO, 1, 180,
						Pos + D3DXVECTOR3(0.0f, GetSize().y / 2.0f, 0.0f), D3DXVECTOR3(sinf(-0.1f) * 10.0f, cosf(-0.1f) * 10.0f, 0.0f)
						, ONE_VECTOR3 * 1.0f, NULL_VECTOR3, CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, D3DXVECTOR3(0.0f, 0.4f, 0.0f), false, 10);
					break;
				case 6:
					m_apTwinTornado[nCnt] = CAttackEnemy::Create(CAttack::ATTACKTYPE03_TORNADO, 1, 180,
						Pos + D3DXVECTOR3(0.0f, GetSize().y / 2.0f, 0.0f), D3DXVECTOR3(sinf(0.1f + D3DX_PI) * 10.0f, cosf(0.1f + D3DX_PI) * 10.0f, 0.0f)
						, ONE_VECTOR3 * 1.0f, NULL_VECTOR3, CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, D3DXVECTOR3(0.0f, 0.4f, 0.0f), false, 10);
					break;
				case 7:
					m_apTwinTornado[nCnt] = CAttackEnemy::Create(CAttack::ATTACKTYPE03_TORNADO, 1, 180,
						Pos + D3DXVECTOR3(0.0f, GetSize().y / 2.0f, 0.0f), D3DXVECTOR3(sinf(-0.1f + D3DX_PI) * 10.0f, cosf(-0.1f + D3DX_PI) * 10.0f, 0.0f)
						, ONE_VECTOR3 * 1.0f, NULL_VECTOR3, CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, D3DXVECTOR3(0.0f, 0.4f, 0.0f), false, 10);
					break;
				default:
					break;
				}

				m_apTwinTornado[nCnt]->SetUseDeath(false);//���S�t���O���I�t�ɂ���
			}
		}

		if (nPatternTime == 50)
		{//�����J�n
			for (int nCnt = 0; nCnt < m_nMAX_TWINTORNADO; nCnt++)
			{
				if (nCnt % 2 == 0)
				{
					m_apTwinTornado[nCnt]->SetMove(D3DXVECTOR3(-1.0f,0.0f, 0.0f));
				}
				else
				{
					m_apTwinTornado[nCnt]->SetMove(D3DXVECTOR3(1.0f,0.0f, 0.0f));
				}

				m_apTwinTornado[nCnt]->SetUseMultiSpeed(true,ONE_VECTOR3 * 1.06f);
			}

			nPatternTime = 0;
			nPattern++;
		}
		break;
	case 2://�p�^�[���I��
		if (nPatternTime == 40)
		{
			for (int nCnt = 0; nCnt < m_nMAX_TWINTORNADO; nCnt++)
			{
				m_apTwinTornado[nCnt]->SetUseDeath(true);//�g���l�[�h���g���I������̂ŁA���S�t���O���I���ɂ���
				m_apTwinTornado[nCnt] = nullptr;
			}
			ResetPattern();
		}
		break;
	default:
		break;
	}
}
//================================================================================================================================

//========================================================
//�s���R
//========================================================
void CDeathEnaga::Action3()
{
	int& nPatternTime = GetPatternTime();         //�p�^�[�����Ԃ��擾����
	bool& bAction = GetAction();                  //�s�����Ă��邩�ǂ������擾
	int& nPattern = GetPattern();                 //���݂̃p�^�[�����擾
	int& nActionNum = GetActionNum();             //�A�N�V�����񐔂��J�E���g

	D3DXVECTOR3& Pos = GetPos();                  //�ʒu���擾
	D3DXVECTOR3& PurposePos = GetPurposePos();    //�ړI�̈ʒu�擾
	D3DXVECTOR3& Move = GetMove();                //�ړ��ʂ��擾
	CBullet* pBullet = nullptr;                   //�o���b�g�ւ̃|�C���^

	float fVXaim = 0.0f;                          //X�����x�N�g��
	float fVYaim = 0.0f;                          //Y�����x�N�g��
	float fVLaim = 0.0f;                          //�ړI�ւ̊p�x

	float fRandRot = 0.0f;                        //��������	
	CAttackEnemy* pAttackEnemy = nullptr;         //�G�U���ւ̃|�C���^

	fVXaim = PurposePos.x - Pos.x;
	fVYaim = PurposePos.y - Pos.y;
	fVLaim = atan2f(fVXaim, fVYaim);
	switch (nPattern)
	{
	case 0://���߂�
		if (nPatternTime == 30)
		{//���F�`���[�W
			CParticle::SummonChargeParticle(CParticle::TYPE00_NORMAL, 180, 120, 5.0f, 30.0f, 30.0f, 300, 100, 10, false,
				Pos, D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), true);

			nPatternTime = 0;
			nPattern++;
		}
		break;
	case 1:
		if (nPatternTime == 1)
		{//�ړI�n�����݂̂��̎��_�ł̃v���C���[�̈ʒu�ɐݒ�
			PurposePos = CGame::GetPlayer()->GetPos();
		}
		Move = D3DXVECTOR3(sinf(fVLaim) * 5.0f, cosf(fVLaim) * 5.0f, 0.0f);

		if (Pos.x > PurposePos.x - 10.0f && Pos.x < PurposePos.x + 10.0f &&
			Pos.y > PurposePos.y - 10.0f && Pos.y < PurposePos.y + 10.0f)
		{//�ړI�n�ɒB������
			Move = NULL_VECTOR3;
			nPatternTime = 0;
			nPattern++;
		}
		break;
	case 2:
		if (nPatternTime == 30)
		{//�ԃ`���[�W
			CParticle::SummonChargeParticle(CParticle::TYPE00_NORMAL, 180, 120, 5.0f, 30.0f, 30.0f, 300, 100, 10,true,
				Pos, D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), true);
			nPatternTime = 0;
			nPattern++;
		}
		break;
	case 3://��������
		if (nPatternTime % 10 == 0)
		{
			if (nActionNum % 2 == 0)
			{
				fRandRot = float(rand() % 156 - 78) / 100 + D3DX_PI * 0.5f;
			}
			else
			{
				fRandRot = float(rand() % 156 - 78) / 100 + D3DX_PI * -0.5f;
			}
			pAttackEnemy = CAttackEnemy::Create(CAttack::ATTACKTYPE03_TORNADO, 1, 220,
				Pos, D3DXVECTOR3(sinf(fRandRot) * 10.0f,cosf(fRandRot) * 10.0f,0.0f), ONE_VECTOR3, NULL_VECTOR3,
					CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, D3DXVECTOR3(0.0f, 0.5f, 0.0f), false, 100);

			if (nActionNum % 2 == 0)
			{
				pAttackEnemy->SetUseAddSpeed(true,D3DXVECTOR3(-0.2f, 0.0f, 0.0f));
				pAttackEnemy->SetColor(CObjectX::COLORTYPE_BRUE, 300);
			}
			else
			{
				pAttackEnemy->SetUseAddSpeed(true, D3DXVECTOR3(0.2f, 0.0f, 0.0f));
				pAttackEnemy->SetColor(CObjectX::COLORTYPE_GREEN, 300);
			}
			nActionNum++;
		}

		if (nActionNum == 20)
		{
			ResetPattern();
		}
		break;
	}
}
//================================================================================================================================

//========================================================
//�s���S
//========================================================
void CDeathEnaga::Action4()
{
	int& nPatternTime = GetPatternTime();         //�p�^�[�����Ԃ��擾����
	bool& bAction = GetAction();                  //�s�����Ă��邩�ǂ������擾
	int& nPattern = GetPattern();                 //���݂̃p�^�[�����擾
	int& nActionNum = GetActionNum();             //�A�N�V�����񐔂��J�E���g

	D3DXVECTOR3& Pos = GetPos();                  //�ʒu���擾
	D3DXVECTOR3& PurposePos = GetPurposePos();    //�ړI�̈ʒu�擾
	D3DXVECTOR3& Move = GetMove();                //�ړ��ʂ��擾

	D3DXVECTOR3 PlayerPos = CGame::GetPlayer()->GetPos();//�v���C���[�̈ʒu
	D3DXVECTOR3 PlayerSize = CGame::GetPlayer()->GetSize();//�v���C���[�̃T�C�Y
	D3DXVECTOR3 PlayerSenterPos = PlayerPos + D3DXVECTOR3(0.0f, PlayerSize.y * 0.5f, 0.0f);

	float fVXaim = 0.0f;                          //X�����x�N�g��
	float fVYaim = 0.0f;                          //Y�����x�N�g��
	float fVLaim = 0.0f;                          //�ړI�ւ̊p�x

	float fRandRot = 0.0f;                        //��������	

	CParticle* pParticle = nullptr;               //�p�[�e�B�N���ւ̃|�C���^
	CBullet* pBullet = nullptr;                   //�o���b�g�ւ̃|�C���^
	CAttackEnemy* pAttackEnemy = nullptr;         //�G�U���ւ̃|�C���^

	fVXaim = PurposePos.x - Pos.x;
	fVYaim = PurposePos.z - Pos.z;
	fVLaim = atan2f(fVXaim, fVYaim);

	//�������I�t�ɂ���
	SetUseInteria(false);

	switch (nPattern)
	{
	case 0://���߂�
		if (nPatternTime == 30)
		{//���F�`���[�W
			CParticle::SummonChargeParticle(CParticle::TYPE00_NORMAL, 180, 120, 5.0f, 30.0f, 30.0f, 300, 100, 10, false,
				Pos, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), true);

			nPatternTime = 0;
			nPattern++;
		}
		break;
	case 1://�J�����̎��_�ɓːi
		if (nPatternTime == 1)
		{
			PurposePos = CManager::GetCamera()->GetPosV();
		}

		if (nPatternTime == 20)
		{
			Move = D3DXVECTOR3(sinf(fVLaim) * 15.0f,0.0f,cosf(fVLaim) * 15.0f);
			nPatternTime = 0;
			nPattern++;
		}
		break;
	case 2://�J�����̌�둤�ɒ�������
		if (Pos.z < CGame::GetPlayer()->GetPos().z - 650.0f)
		{
			nPatternTime = 0;
			nPattern++;
			Move = NULL_VECTOR3;
		}
		break;
	case 3:
		Pos.x = CGame::GetPlayer()->GetPos().x;
		Pos.y = CGame::GetPlayer()->GetPos().y;

		if (nPatternTime < 100)
		{//�ːi�\��ʒu�Ƀp�[�e�B�N��������
			fRandRot = float(rand() % 628) / 100;
			pParticle = CParticle::Create(CParticle::TYPE00_NORMAL, 40, 40.0f, 40.0f,
				CGame::GetPlayer()->GetPos() + D3DXVECTOR3(PlayerSenterPos.x + sinf(fRandRot) * 100.0f,
					PlayerSenterPos.y + cosf(fRandRot) * 100.0f, 0.0f),
				D3DXVECTOR3(sinf(fRandRot) * 15.0f, cosf(fRandRot) * 15.0f, 0.0f), D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), true);

			pParticle->SetUseAddSpeed(true, true, 1.03f);
		}

		if (nPatternTime == 120)
		{
			Move.z = 10.0f;//�O���ɒ�����
			nPatternTime = 0;
			nPattern++;
		}
		break;
	case 4://�ړI�n�ɒ�������

		if (Pos.z > -20.0f && Pos.z < 20.0f && nActionNum < 1)
		{//Z�����_�̋߂��ɗ�����
			nActionNum++;
			for (int nCnt = 0; nCnt < 16; nCnt++)
			{
				fRandRot = (2.0f / 16.0f) * float(nCnt);
				CBullet::Create(CBullet::BULLETTYPE_ENEMY, CBullet::BULLETTEX_ENEMY, 140, 10, 40.0f, 40.0f,
					D3DXVECTOR3(Pos.x, Pos.y, 0.0f),
					D3DXVECTOR3(sinf(D3DX_PI * fRandRot) * 10.0f, cosf(D3DX_PI * fRandRot) * 10.0f, 0.0f));
			}
		}

		if (Pos.z > 600.0f && Pos.z < 640.0f)
		{
			Move = NULL_VECTOR3;
			nPatternTime = 0;
			nActionNum = 0;
			nPattern++;
		}
		break;
	case 5://�K�E�Z�`���[�W
		if (nPatternTime <= 120)
		{//���F�`���[�W
			CParticle::SummonChargeParticle(CParticle::TYPE00_NORMAL, 10, 120, 10.0f, 30.0f, 30.0f, 400, 100, 10, true,
				Pos + D3DXVECTOR3(0.0f, GetSize().y * 0.5f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), true);
		}
		else
		{
			nPatternTime = 0;
			nPattern++;
		}
		break;
	case 6://�K�E�Z
		if (nPatternTime % 30 == 0)
		{
			for (int nCnt = 0; nCnt < 8; nCnt++)
			{
				if (nActionNum % 2 == 0)
				{//���ʂɌ���
					fRandRot = (2.0f / 8.0f) * float(nCnt);
				}
				else
				{//�������炵�Č���
					fRandRot = (2.0f / 8.0f) * float(nCnt) + ((2.0f / 8.0f) / 2.0f);
				}
				CAttackEnemy::Create(CAttack::ATTACKTYPE03_TORNADO, 12, 300,
					Pos - D3DXVECTOR3(0.0f,300.0f,0.0f), D3DXVECTOR3(sinf(D3DX_PI * fRandRot) * 15.0f, 0.0f, cosf(D3DX_PI * fRandRot) * 15.0f), D3DXVECTOR3(2.0f, 10.0f, 2.0f),
					NULL_VECTOR3, CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, D3DXVECTOR3(0.0f, 0.6f, 0.0f), false, 120);
			}
			nActionNum++;//�U���񐔂𑝂₷
		}

		if (nActionNum == 7)
		{
			nPatternTime = 0;
			nActionNum = 0;
			nPattern++;
		}
		break;
	case 7://���W��߂�
		Move.z = -20.0f;

		if (Pos.z > -20.0f && Pos.z < 20.0f)
		{
			SetInsanity(true);
			Move = NULL_VECTOR3;
			SetUseInteria(true);
			ResetPattern();
		}
		break;
	default:
		break;
	}
}
//===================================================================================================================================================

//============================================<<<�~�X�^�[�E�X�g�����O�N���X>>>=============================================================================

//==================================================================
//�R���X�g���N�^
//==================================================================
CMrStrong::CMrStrong()
{

}
//======================================================================================================================================

//==================================================================
//�f�X�g���N�^
//==================================================================
CMrStrong::~CMrStrong()
{

}
//======================================================================================================================================

//==================================================================
//����������
//==================================================================
HRESULT CMrStrong::Init()
{
	CBoss::Init();
	m_pBurstMeteo = nullptr;//�o�[�X�ƃ��e�I�ւ̃|�C���^
	return S_OK;
}
//======================================================================================================================================

//==================================================================
//�I������
//==================================================================
void CMrStrong::Uninit()
{
	CBoss::Uninit();
}
//======================================================================================================================================

//==================================================================
//�X�V����
//==================================================================
void CMrStrong::Update()
{
	D3DXVECTOR3 Pos = GetPos();
	CBoss::Update();
	CManager::GetDebugText()->PrintDebugText("�{�X�̈ʒu�F%f %f %f\n", Pos.x, Pos.y, Pos.z);
}
//======================================================================================================================================

//==================================================================
//�`�揈��
//==================================================================
void CMrStrong::Draw()
{
	int nCntTime = GetCntTime();
	CBoss::Draw();
}
//======================================================================================================================================

//==================================================================
//���S�t���O�ݒ菈��
//==================================================================
void CMrStrong::SetDeath()
{
	if (m_pBurstMeteo != nullptr)
	{
		m_pBurstMeteo->SetUseDeath(true);
		m_pBurstMeteo->SetDeath();
		m_pBurstMeteo = nullptr;
	}
	CBoss::SetDeath();
}
//======================================================================================================================================

//==================================================================
//��������
//==================================================================
CMrStrong* CMrStrong::Create(BOSSTYPE type, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 Scale)
{
	CMrStrong* pMrStrong = DBG_NEW CMrStrong;   //�{�X�𐶐�
	bool bSuccess = pMrStrong->CObject::GetCreateSuccess();
	int nIdx = 0;//���f���̃C���f�b�N�X
	if (bSuccess == true)
	{
		if (pMrStrong != nullptr)
		{
			pMrStrong->Init();                             //����������
			pMrStrong->SetUseDeath(true);                  //���S�t���O�𔭓����邩�ǂ�����ݒ肷��
			pMrStrong->SetType(type);                      //�{�X�̎��
			pMrStrong->SetLife(nLife);                     //�̗�
			pMrStrong->SetMaxLife(nLife);//�ő�̗�
			pMrStrong->SetAutoSubLife(false);              //�����I�ɑ̗͂����炷���ǂ���
			pMrStrong->CObjectX::SetPos(pos);                                                                                  //�I�u�W�F�N�g�Q�c�̈ʒu��ݒ�
			pMrStrong->SetPosOld(pos);                                                                                         //1f�O�̈ʒu��ݒ�
			pMrStrong->SetSupportPos(pos);                                                                                     //�ݒu�ʒu
			pMrStrong->CObject::SetType(CObject::TYPE_BOSS);                                                                   //�I�u�W�F�N�g�̎�ނ����߂�
			pMrStrong->CObjectX::SetScale(Scale);                                                                              //�I�u�W�F�N�g�̊g�嗦
			pMrStrong->SetFormarScale(Scale);                                                                                  //���̊g�嗦��ݒ肷��
			nIdx = CManager::GetObjectXInfo()->Regist(CObjectX::OBJECTXTYPE_BOSS, m_BOSS_FILENAME[(int)(type)]);
			pMrStrong->CObjectX::BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(CObjectX::OBJECTXTYPE_BOSS, nIdx),
				CManager::GetObjectXInfo()->GetBuffMat(CObjectX::OBJECTXTYPE_BOSS, nIdx),
				CManager::GetObjectXInfo()->GetdwNumMat(CObjectX::OBJECTXTYPE_BOSS, nIdx),
				CManager::GetObjectXInfo()->GetTexture(CObjectX::OBJECTXTYPE_BOSS, nIdx),
				CManager::GetObjectXInfo()->GetColorValue(CObjectX::OBJECTXTYPE_BOSS, nIdx));                                   //���f���������蓖�Ă�
			pMrStrong->SetBossHpGauge(nLife);                                                                                  //�̗̓Q�[�W��ݒ肷��
			pMrStrong->CObjectX::SetObjXType(CObjectX::OBJECTXTYPE_BOSS);                                                      //�I�u�W�F�N�gX�̃^�C�v��ݒ�
			pMrStrong->CObjectX::SetTypeNum((int)(type));                                                                      //�I�u�W�F�N�gX���Ƃ̃^�C�v�ԍ���ݒ�
			pMrStrong->SetSize();                                                                                              //X�I�u�W�F�N�g�̃T�C�Y��ݒ肷��
			pMrStrong->CObjectX::SetRotType(CObjectX::ROTTYPE::ROTTYPE_NORMAL);                                                //�����̎�ނ�ݒ�
			pMrStrong->SetManagerType((int)(CStageManager::MANAGEROBJECT_BOSS));                                               //�}�l�[�W���[�ŌĂяo�����̎�ނ�ݒ�
			pMrStrong->AdjustSize();      //����T�C�Y�������s��
		}
		else
		{//�I�u�W�F�N�g�ɋ󂫂��Ȃ������̂Ŕj������
			delete pMrStrong;
			pMrStrong = nullptr;
			return nullptr;
		}
	}
	return pMrStrong;
}
//======================================================================================================================================

//==================================================================
//�ړ��p�^�[��
//==================================================================
void CMrStrong::MovePattern()
{
	int& nMovePatternFrame = GetMovePatternFrame();
	int& nMaxMovePatternFrame = GetMaxMovePatternFrame();
	bool& bUseMovePattern = GetUseMovePattern();//�ړ��p�^�[�����g�p���Ă��邩�ǂ���
	int& nMovePatternNum = GetMovePatternNum();//�ړ��p�^�[���ԍ����擾����
	D3DXVECTOR3& Move = GetMove();
	D3DXVECTOR3 Pos = GetPos();
	float fLength = 0.0f;

	CBoss::MovePattern();//�ړ��p�^�[���̋��ʏ���

	DecideMovePattern();//�ړ��p�^�[�����菈��

	//�ړ��p�^�[�����J�n����
	switch (nMovePatternNum)
	{
	case 0:
		MovePattern0();//�ړ��p�^�[���O
		break;
	case 1:
		MovePattern1();//�ړ��p�^�[���P
		break;
	default:
		break;
	}

	//�ړ��p�^�[�����I��点��
	if (nMovePatternFrame > nMaxMovePatternFrame)
	{
		bUseMovePattern = false;//�A�N�V�����p�^�[���ֈڍs
		nMovePatternFrame = 0;
		Move = NULL_VECTOR3;
	}

}
//======================================================================================================================================

//==================================================================
//�ړ��p�^�[�����菈��
//==================================================================
void CMrStrong::DecideMovePattern()
{
	int& nMovePatternFrame = GetMovePatternFrame();
	int& nMaxMovePatternFrame = GetMaxMovePatternFrame();
	bool& bUseMovePattern = GetUseMovePattern();//�ړ��p�^�[�����g�p���Ă��邩�ǂ���
	int& nMovePatternNum = GetMovePatternNum();//�ړ��p�^�[���ԍ����擾����
	D3DXVECTOR3 Pos = GetPos();

	if (nMovePatternFrame == 1)
	{//�ŏ��Ɉړ����@�����߂�
		nMovePatternNum = rand() % 2;

		if (Pos.x > 250.0f && CGame::GetPlayer()->GetPos().x > Pos.x)
		{//�ǍۂɃv���C���[���ǂ����܂�Ă�����E�E�E
			nMovePatternNum = 1;
		}
		else if (Pos.x < -250.0f && CGame::GetPlayer()->GetPos().x < Pos.x)
		{//�ǍۂɃv���C���[���ǂ����܂�Ă�����E�E�E
			nMovePatternNum = 1;
		}

		//���ꂼ��̈ړ��p�^�[���̍ő�t���[������ݒ肷��
		switch (nMovePatternNum)
		{
		case 0:
			nMaxMovePatternFrame = 100;
			break;
		case 1:
			nMaxMovePatternFrame = 90;
			break;
		default:
			break;
		}
	}

}
//======================================================================================================================================

//==================================================================
//�s���p�^�[��
//==================================================================
void CMrStrong::ActionPattern()
{
	bool& bAction = GetAction();    //�s�����Ă��邩�ǂ���
	int& nSkill = GetSkill();       //�X�L��
	bool& bInsanity = GetInsanity();//�������Ă��邩�ǂ���
	int nLife = GetLife();          //�̗�
	int& nCntTurn = GetTurnCnt();   //�^�[�������J�E���g
	int nMaxLife = GetMaxLife();    //�ő�̗�
	float fRatioLife = 0.0f;        //�̗͂̊���

	fRatioLife = float(nLife) / float(nMaxLife);

	if (fRatioLife < 0.5f)
	{//�̗͂��T�O������������甭����Ԃɂ���
		bInsanity = true;
	}

	if (bAction == false)
	{
		if (bInsanity == false)
		{//������Ԃ���Ȃ����
			nSkill = rand() % 4;
			//nSkill = 2;
		}
		else
		{//������ԂȂ�
			nCntTurn++;//��������^�[�������J�E���g
			nSkill = rand() % 6;
			if (nCntTurn == 1)
			{//�������Ă���ŏ��̃^�[���ɕK�E�Z�𔭓��I
				nSkill = 5;
			}
		}
		bAction = true;
	}

	if (bAction == true)
	{
		CBoss::ActionPattern();
		switch (nSkill)
		{
		case 0:
			Action0();
			break;
		case 1:
			Action1();
			break;
		case 2:
			Action2();
			break;
		case 3:
			Action4();
			break;
		case 4:
			Action3();
			break;
		case 5:
			Action5();
			break;
		default:
			break;
		}
	}

}
//======================================================================================================================================

//==================================================================
//�ړ��O�i�������ڋ߁j
//==================================================================
void CMrStrong::MovePattern0()
{

	D3DXVECTOR3& Move = GetMove();
	D3DXVECTOR3& Pos = GetPos();
	D3DXVECTOR3 PlayerPos = CGame::GetPlayer()->GetPos();

	if (Pos.x < PlayerPos.x)
	{//�v���C���[���E���ɂ�����
		Move.x = 1.0f;
	}
	else
	{//�v���C���[�������ɂ�����
		Move.x = -1.0f;
	}
	
}
//======================================================================================================================================

//==================================================================
//�T�C�Y�𒲐�����
//==================================================================
void CMrStrong::AdjustSize()
{
	D3DXVECTOR3& VtxMax = GetOriginVtxMax();
	D3DXVECTOR3& VtxMin = GetOriginVtxMin();

	VtxMax.x -= 45.0f;
	VtxMin.x += 45.0f;
}
//======================================================================================================================================

//==================================================================
//�ړ��P�i�W�����v���݂��j
//==================================================================
void CMrStrong::MovePattern1()
{
	D3DXVECTOR3& Move = GetMove();
	D3DXVECTOR3& Pos = GetPos();
	D3DXVECTOR3 PlayerPos = CGame::GetPlayer()->GetPos();
	int nMovePatternFrame = GetMovePatternFrame();//�ړ��p�^�[���t���[��
	SetUseInteria(false);//�����I�t
	if (nMovePatternFrame == 1)
	{
		if (Pos.x < PlayerPos.x)
		{//�v���C���[���E���ɂ�����
			Move.x = 5.0f;
			Move.y = 25.0f;
		}
		else
		{//�v���C���[�������ɂ�����
			Move.x = -5.0f;
			Move.y = 25.0f;
		}
	}
	if (nMovePatternFrame > 2)
	{
		if (Pos.y < 0.1f)
		{
			Move = NULL_VECTOR3;
		}
	}

	if (nMovePatternFrame == 89)
	{
		SetUseInteria(true);//�����I��
	}

}
//======================================================================================================================================

//==================================================================
//�s���O�i�Ⓤ���U���j
//==================================================================
void CMrStrong::Action0()
{
	int& nPatternTime = GetPatternTime();         //�p�^�[�����Ԃ��擾����
	bool& bAction = GetAction();                  //�s�����Ă��邩�ǂ������擾
	int& nPattern = GetPattern();                 //���݂̃p�^�[�����擾
	int& nActionNum = GetActionNum();             //�A�N�V�����񐔂��J�E���g

	D3DXVECTOR3& Pos = GetPos();                  //�ʒu���擾
	D3DXVECTOR3 PlayerPos = CGame::GetPlayer()->GetPos();//�v���C���[�̈ʒu
	CBullet* pBullet = nullptr;                   //�o���b�g�ւ̃|�C���^
	float fRandRot = 0.0f;                        //��������	
	float fRandSpeed = 0.0f;                      //�������x
	CAttackEnemy* pAttackEnemy = nullptr;         //�G�U���ւ̃|�C���^
	switch (nPattern)
	{
	case 0://���߂�
		if (nPatternTime == 30)
		{//�ԃ`���[�W
			CParticle::SummonChargeParticle(CParticle::TYPE00_NORMAL, 180, 120, 5.0f, 30.0f, 30.0f, 300, 100, 10, false,
				Pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), true);

			nPatternTime = 0;
			nPattern++;
		}
		break;
	case 1://覐Δ���
		if (nPatternTime == 40)
		{
			for (int nCnt = 0; nCnt < 6; nCnt++)
			{
				if (Pos.x < PlayerPos.x)
				{//�v���C���[���E���ɂ�����E�E�E
					fRandRot = float(rand() % 157) / -100 + D3DX_PI * 0.5f;
				}
				else
				{//�v���C���[�������ɂ�����E�E�E
					fRandRot = float(rand() % 157) / 100 - D3DX_PI * 0.5f;
				}
				fRandSpeed = 5.0f + float(rand() % 100) / 10;
				pAttackEnemy = CAttackEnemy::Create(CAttack::ATTACKTYPE01_METEO, 10, 120, Pos, D3DXVECTOR3(sinf(fRandRot) * fRandSpeed, cosf(fRandRot) * fRandSpeed, 0.0f), ONE_VECTOR3,
					NULL_VECTOR3, CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, ONE_VECTOR3 * 0.05f, true, 30);

				pAttackEnemy->SetUseGravity(-0.4f);//�d�͂�������
			}

			nPatternTime = 0;
			nPattern++;
		}
		break;
	case 2://�N�[���^�C��
		if (nPatternTime == 20)
		{
			ResetPattern();
		}
		break;
	default:
		break;

	}
}
//======================================================================================================================================

//==================================================================
//�s���P�i�����c�[�Ռ��g�j
//==================================================================
void CMrStrong::Action1()
{
	int& nPatternTime = GetPatternTime();         //�p�^�[�����Ԃ��擾����
	bool& bAction = GetAction();                  //�s�����Ă��邩�ǂ������擾
	int& nPattern = GetPattern();                 //���݂̃p�^�[�����擾
	int& nActionNum = GetActionNum();             //�A�N�V�����񐔂��J�E���g

	D3DXVECTOR3& Pos = GetPos();                  //�ʒu���擾
	D3DXVECTOR3 PlayerPos = CGame::GetPlayer()->GetPos();//�v���C���[�̈ʒu
	CBullet* pBullet = nullptr;                   //�o���b�g�ւ̃|�C���^
	float fRandRot = 0.0f;                        //��������	
	float fRandSpeed = 0.0f;                      //�������x
	CAttackEnemy* pAttackEnemy = nullptr;         //�G�U���ւ̃|�C���^
	switch (nPattern)
	{
	case 0://���߂�
		if (nPatternTime == 45)
		{//�΃`���[�W
			CParticle::SummonChargeParticle(CParticle::TYPE00_NORMAL, 180, 120, 5.0f, 30.0f, 30.0f, 300, 100, 10, false,
				Pos, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), true);

			nPatternTime = 0;
			nPattern++;
		}
		break;
	case 1:
		if (nPatternTime == 60)
		{
			nPatternTime = 0;
			nPattern++;
		}
		break;
	case 2:
		if (nPatternTime % 20 == 0)
		{
			if (nActionNum % 2 == 0)
			{
				pAttackEnemy = CAttackEnemy::Create(CAttack::ATTACKTYPE10_SHOCKWAVE, 20, 150, Pos, D3DXVECTOR3(1.0f, 0.0f, 0.0f), ONE_VECTOR3 * 4.0f, NULL_VECTOR3,
					CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, NULL_VECTOR3, false, 60);

				pAttackEnemy->SetUseMultiSpeed(true, D3DXVECTOR3(1.1f, 0.0f, 0.0f));
			}
			else
			{
				pAttackEnemy = CAttackEnemy::Create(CAttack::ATTACKTYPE10_SHOCKWAVE, 20, 150, Pos, D3DXVECTOR3(-1.0f, 0.0f, 0.0f), ONE_VECTOR3 * 4.0f,D3DXVECTOR3(0.0f,D3DX_PI,0.0f),
					CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, NULL_VECTOR3, false, 60);

				pAttackEnemy->SetUseMultiSpeed(true, D3DXVECTOR3(1.1f, 0.0f, 0.0f));
			}
			nActionNum++;
		}

		if (nActionNum == 12)
		{
			ResetPattern();
		}
		break;
	default:
		break;
	}
}
//======================================================================================================================================

//==================================================================
//�s���Q�i�������v���X�j
//==================================================================
void CMrStrong::Action2()
{
	int& nPatternTime = GetPatternTime();         //�p�^�[�����Ԃ��擾����
	bool& bAction = GetAction();                  //�s�����Ă��邩�ǂ������擾
	int& nPattern = GetPattern();                 //���݂̃p�^�[�����擾
	int& nActionNum = GetActionNum();             //�A�N�V�����񐔂��J�E���g

	D3DXVECTOR3& Pos = GetPos();                  //�ʒu���擾
	D3DXVECTOR3& Move = GetMove();                //�ړ��ʂ��擾
	D3DXVECTOR3 PlayerPos = CGame::GetPlayer()->GetPos();//�v���C���[�̈ʒu
	CBullet* pBullet = nullptr;                   //�o���b�g�ւ̃|�C���^
	float fRandRot = 0.0f;                        //��������	
	float fRandSpeed = 0.0f;                      //�������x
	float fLength = 0.0f;                         //����

	//�v���C���[�ƃ{�X�̋��������߂�
	fLength = sqrtf(powf(PlayerPos.x - Pos.x, 2) + powf(PlayerPos.y - Pos.y, 2));

	CAttackEnemy* pAttackEnemy = nullptr;         //�G�U���ւ̃|�C���^
	switch (nPattern)
	{
	case 0://���߂�
		if (nPatternTime == 45)
		{//�`���[�W
			CParticle::SummonChargeParticle(CParticle::TYPE00_NORMAL, 180, 120, 5.0f, 30.0f, 30.0f, 300, 100, 10, false,
				Pos, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), true);

			nPatternTime = 0;
			nPattern++;
		}
		break;
	case 1://�������v���X
		if (nPatternTime == 5)
		{
			SetUseInteria(false);             //�������g�p���Ȃ�
			fRandRot = CCalculation::CalculationParabola(CCalculation::CalculationLength(Pos, PlayerPos),GetNormalGravity(), 25.0f, Pos, PlayerPos);
			Move = D3DXVECTOR3(sinf(fRandRot) * 25.0f, cosf(fRandRot) * 25.0f, 0.0f);
		}

		if (nPatternTime > 5)
		{
			if (Pos.y < 0.1f)
			{
				pAttackEnemy = CAttackEnemy::Create(CAttack::ATTACKTYPE10_SHOCKWAVE, 20, 150, Pos, D3DXVECTOR3(1.0f, 0.0f, 0.0f),
					ONE_VECTOR3 * 3.5f + (ONE_VECTOR3 * 0.2f) * float(nActionNum), NULL_VECTOR3,
					CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, NULL_VECTOR3, false, 60);
				pAttackEnemy->SetUseMultiSpeed(true, D3DXVECTOR3(1.1f, 0.0f, 0.0f));

				pAttackEnemy = CAttackEnemy::Create(CAttack::ATTACKTYPE10_SHOCKWAVE, 20, 150, Pos, D3DXVECTOR3(-1.0f, 0.0f, 0.0f),
					ONE_VECTOR3 * 3.5f + (ONE_VECTOR3 * 0.2f) * float(nActionNum), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
					CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, NULL_VECTOR3, false, 60);

				pAttackEnemy->SetUseMultiSpeed(true, D3DXVECTOR3(1.1f, 0.0f, 0.0f));

				nPatternTime = 0;
				nActionNum++;
				Move = NULL_VECTOR3;
			}
		}

		if (nActionNum == 5)
		{//5�񒵂˂�
			SetUseInteria(true);//�������g�p����
			Move = NULL_VECTOR3;
			nPatternTime = 0;
			nPattern++;
			nActionNum = 0;
		}
		break;
	case 2://�N�[���^�C��
		if (nPatternTime == 45)
		{
			ResetPattern();
		}
		break;
	default:
		break;
	}

}
//======================================================================================================================================

//==================================================================
//�s���R�i�o�[�X�g���e�I�j
//==================================================================
void CMrStrong::Action3()
{
	int& nPatternTime = GetPatternTime();         //�p�^�[�����Ԃ��擾����
	bool& bAction = GetAction();                  //�s�����Ă��邩�ǂ������擾
	int& nPattern = GetPattern();                 //���݂̃p�^�[�����擾
	int& nActionNum = GetActionNum();             //�A�N�V�����񐔂��J�E���g

	D3DXVECTOR3& Pos = GetPos();                  //�ʒu���擾
	D3DXVECTOR3 AttackMove = NULL_VECTOR3;        //�U���̈ړ���
	D3DXVECTOR3 PlayerPos = CGame::GetPlayer()->GetPos();//�v���C���[�̈ʒu
	CBullet* pBullet = nullptr;                   //�o���b�g�ւ̃|�C���^
	float fRandRot = 0.0f;                        //��������	
	float fRandSpeed = 0.0f;                      //�������x
	float fLength = 0.0f;                         //����

	//�v���C���[�ƃ{�X�̋��������߂�
	fLength = sqrtf(powf(PlayerPos.x - Pos.x, 2) + powf(PlayerPos.y - Pos.y, 2));

	//�������ړ��֌W
	float fXG = fLength * GetNormalGravity();      //�i�����A�d�́j
	float fV = 35.0f;                             //�ړ����x
	float fV2 = powf(fV, 2);                      //�ړ����x�̂Q��
	float fResult = 0.0f;                         //�v�Z����
	CAttackEnemy* pAttackEnemy = nullptr;         //�G�U���ւ̃|�C���^

	switch (nPattern)
	{
	case 0://���߂�
		if (nPatternTime == 15)
		{//���`���[�W
			CParticle::SummonChargeParticle(CParticle::TYPE00_NORMAL, 180, 120, 5.0f, 30.0f, 30.0f, 300, 100, 10, false,
				Pos, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), true);
		}

		if (nPatternTime == 60)
		{
			nPatternTime = 0;
			nPattern++;
		}
		break;
	case 1:
		fResult = asinf(fXG / fV2);       //��������t�Z���Ĕ��ˊp�x������o��
		fRandRot = fResult / 2.0f;
		if (PlayerPos.x < Pos.x)
		{
			AttackMove = D3DXVECTOR3(sinf(-fRandRot) * fV, cosf(-fRandRot) * fV, 0.0f);
		}
		else
		{
			AttackMove = D3DXVECTOR3(sinf(fRandRot) * fV, cosf(fRandRot) * fV, 0.0f);
		}

		m_pBurstMeteo = CAttackEnemy::Create(CAttack::ATTACKTYPE01_METEO, 10, 150, Pos, AttackMove, ONE_VECTOR3 * 2.0f, NULL_VECTOR3,
			CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, D3DXVECTOR3(0.0f, 1.1f, 0.5f), false, 45);
		m_pBurstMeteo->SetUseGravity(-GetNormalGravity());
		m_pBurstMeteo->SetUseDeath(false);//���S�t���O�𔭓������Ȃ��B

		nPatternTime = 0;
		nPattern++;
		break;
	case 2://���e���ɕ��􂳂���
		if (m_pBurstMeteo->GetPos().y < 0.1f)
		{
			for (int nCnt = 0; nCnt < 4; nCnt++)
			{
				fRandRot = float(rand() % 100 - 50) / 100;
				fRandSpeed = float(rand() % 100) / 10.0f + 7.0f;
				pAttackEnemy = CAttackEnemy::Create(CAttack::ATTACKTYPE01_METEO, 10, 120, m_pBurstMeteo->GetPos(),
					D3DXVECTOR3(sinf(fRandRot) * fRandSpeed, cosf(fRandRot) * fRandSpeed, 0.0f), ONE_VECTOR3 * 0.5f, NULL_VECTOR3,
					CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, NULL_VECTOR3, true, 50);

				pAttackEnemy->SetUseGravity(-0.5f);
				pAttackEnemy->SetUseEffect(CEffect::EFFECTTYPE00_NORMAL, true, D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), 40, D3DXVECTOR2(30.0f, 30.0f));//�G�t�F�N�g���g�p
			}

			if (m_pBurstMeteo != nullptr)
			{
				m_pBurstMeteo->SetUseDeath(true);//���S�t���O���g�p����
				m_pBurstMeteo->SetDeath();       //���S�t���O��ݒ肷��
				m_pBurstMeteo = nullptr;         //�|�C���^��������
			}

			if (nActionNum == 4)
			{
				nActionNum = 0;
				nPattern++;
			}
			else
			{
				nActionNum++;//�A�N�V�����񐔂𑝂₷
				nPattern = 1;//�p�^�[����߂�
			}
			nPatternTime = 0;//�p�^�[�����Ԃ�߂�
		}
		break;
	case 3://�N�[���^�C��
		if (nPatternTime == 60)
		{
			ResetPattern();
		}
		break;
	default:
		break;
	}
}
//======================================================================================================================================

//==================================================================
//�s���S�i�ǂ��l�߃o���b�g�j	
//==================================================================
void CMrStrong::Action4()
{
	int& nPatternTime = GetPatternTime();         //�p�^�[�����Ԃ��擾����
	bool& bAction = GetAction();                  //�s�����Ă��邩�ǂ������擾
	int& nPattern = GetPattern();                 //���݂̃p�^�[�����擾
	int& nActionNum = GetActionNum();             //�A�N�V�����񐔂��J�E���g

	D3DXVECTOR3& Pos = GetPos();                  //�ʒu���擾
	D3DXVECTOR3 SummonPos = NULL_VECTOR3;         //�����ʒu
	D3DXVECTOR3& Scale = GetScale();              //�g�嗦
	D3DXVECTOR3 PlayerPos = CGame::GetPlayer()->GetPos();//�v���C���[�̈ʒu
	CBullet* pBullet = nullptr;                   //�o���b�g�ւ̃|�C���^
	float fRandRot = 0.0f;                        //��������	
	float fRandSpeed = 0.0f;                      //�������x
	float fLength = 0.0f;                         //����

	float fVXaim = CGame::GetPlayer()->GetPos().x - Pos.x;
	float fVYaim = CGame::GetPlayer()->GetPos().y - Pos.y;
	float fVLaim = atan2f(fVXaim,fVYaim);//�ړI�n�ւ̊p�x
	switch (nPattern)
	{
	case 0://���߂�
		if (nPatternTime == 15)
		{//���F�`���[�W
			CParticle::SummonChargeParticle(CParticle::TYPE00_NORMAL, 180, 120, 5.0f, 30.0f, 30.0f, 300, 100, 10, false,
				Pos, D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), true);
		}

		if (nPatternTime == 60)
		{
			nPatternTime = 0;
			nPattern++;
		}
		break;
	case 1://���剻
		if (nPatternTime < 50)
		{
			Scale += ONE_VECTOR3 * 0.1f;
		}
		else
		{
			nPatternTime = 0;
			nPattern++;
		}
		break;
	case 2:
		if (nPatternTime < 45)
		{
			CParticle::SummonChargeParticle(CParticle::TYPE00_NORMAL, 5, 60, 5.0f, 30.0f, 30.0f, 300, 100, 10, false,
				Pos, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), true);
		}

		if (nPatternTime == 60)
		{
			nPatternTime = 0;
			nPattern++;
		}
		break;
	case 3://�o���b�g����
		if (nPatternTime < 120)
		{
			if (nPatternTime % 10 == 0)
			{
				SummonPos = D3DXVECTOR3(Pos.x, Pos.y + GetVtxMax().y, Pos.z);
				fVXaim = PlayerPos.x - SummonPos.x;
				fVYaim = (CGame::GetPlayer()->GetSize().y / 2.0f + PlayerPos.y) - SummonPos.y;
				fVLaim = atan2f(fVXaim, fVYaim);//�ړI�n�ւ̊p�x

				for (int nCnt = 0; nCnt < 5; nCnt++)
				{
					pBullet =  CBullet::Create(CBullet::BULLETTYPE_ENEMY, CBullet::BULLETTEX_ENEMY, 100, 1, 30.0f,30.0f, SummonPos,
						D3DXVECTOR3(sinf(fVLaim - 0.6f + 0.3f * float(nCnt)) * 12.0f, cosf(fVLaim - 0.6f + 0.3f * float(nCnt)) * 12.0f, 0.0f));
					pBullet->SetUseEffect(20, D3DXVECTOR2(30.0f, 30.0f), D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
				}
			}
		}
		else
		{
			nPattern++;
			nPatternTime = 0;
		}
		break;
	case 4://�g�嗦�����ɖ߂�
		if (nPatternTime < 50)
		{
			Scale -= ONE_VECTOR3 * 0.1f;
		}
		else
		{
			Scale = GetFormarScale();
			ResetPattern();
		}
		break;
	default:
		break;
	}
}
//======================================================================================================================================

//==================================================================
//�s���T�i�w���t�H�[���j	
//==================================================================
void CMrStrong::Action5()
{
	int& nPatternTime = GetPatternTime();         //�p�^�[�����Ԃ��擾����
	bool& bAction = GetAction();                  //�s�����Ă��邩�ǂ������擾
	int& nPattern = GetPattern();                 //���݂̃p�^�[�����擾
	int& nActionNum = GetActionNum();             //�A�N�V�����񐔂��J�E���g
	int& nSubPattern = GetSubPattern();           //�T�u�p�^�[�����擾
	D3DXVECTOR3& PurposePos = GetPurposePos();    //�ړI�n��ݒ肷��

	D3DXVECTOR3& Pos = GetPos();                  //�ʒu���擾
	D3DXVECTOR3& Move = GetMove();                //�ړ��ʂ��擾
	D3DXVECTOR3 PlayerPos = CGame::GetPlayer()->GetPos();//�v���C���[�̈ʒu

	CBullet* pBullet = nullptr;                   //�o���b�g�ւ̃|�C���^
	CAttackEnemy* pAttackEnemy = nullptr;         //�G�U���ւ̃|�C���^
	CMrStrongAttack* pMrStrongAttack = nullptr;   //�~�X�^�[�X�g�����O�̍U��
	float fVXaim = 0.0f;//X�x�N�g��
	float fVYaim = 0.0f;//Y�x�N�g��
	float fVLaim = 0.0f;//�ړI�n�ւ̊p�x
	switch (nPattern)
	{
	case 0://���߂�
		if (nPatternTime == 15)
		{//���F�`���[�W�I
			SetUseInteria(false);
			CParticle::SummonChargeParticle(CParticle::TYPE00_NORMAL, 180, 120, 5.0f, 30.0f, 30.0f, 300, 100, 10, false,
				Pos, D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), true);
		}
		SetUseInteria(false);
		if (nPatternTime == 60)
		{
			nPatternTime = 0;
			nPattern++;
		}
		break;
	case 1:
		if (nPatternTime == 1)
		{
			PurposePos = D3DXVECTOR3(0.0f,120.0f,0.0f);
			nPatternTime = 0;
			nPattern++;
		}
		break;
	case 2://�ړI�n�ֈړ�
		fVXaim = PurposePos.x - Pos.x;
		fVYaim = PurposePos.y - Pos.y;
		fVLaim = atan2f(fVXaim, fVYaim);

		SetUseGravity(false);

		Move = D3DXVECTOR3(sinf(fVLaim) * 5.0f, cosf(fVLaim) * 5.0f, 0.0f);//���S�ֈړ�

		if (Pos.x > PurposePos.x - 5.0f && Pos.x < PurposePos.x + 5.0f &&
			Pos.y > PurposePos.y - 5.0f && Pos.y < PurposePos.y + 5.0f)
		{//�ړI�n�ւ��ǂ蒅����
			Move = NULL_VECTOR3;
			nPatternTime = 0;
			nPattern++;
		}
		break;
	case 3://������x�`���[�W
		if (nPatternTime < 60)
		{//�ԐF�`���[�W�I
			CParticle::SummonChargeParticle(CParticle::TYPE00_NORMAL, 2, 120, 5.0f, 30.0f, 30.0f, 300, 100, 10, false,
				Pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), true);
		}
		else
		{
			nPatternTime = 0;
			nPattern++;
		}
		break;
	case 4:
		if (nSubPattern % 2 == 0)
		{
			if (nPatternTime % 2 == 0)
			{
				pAttackEnemy = CAttackEnemy::Create(CAttack::ATTACKTYPE01_METEO, 10, 100, Pos,
					D3DXVECTOR3(sinf(D3DX_PI * 0.5f - 0.1f * float(nActionNum)) * 15.0f, cosf(D3DX_PI * 0.5f - 0.1f * float(nActionNum)) * 15.0f, 0.0f),
					ONE_VECTOR3 * 2.0f, NULL_VECTOR3, CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, NULL_VECTOR3, true, 60);

				pAttackEnemy->SetUseGravity(-0.4f);

				nActionNum++;
			}

			if (nActionNum % 30 == 0 && nActionNum > 0)
			{
				nPatternTime = 0;
				nSubPattern++;
				nActionNum = 0;
			}
		}
		else
		{
			if (nPatternTime % 2 == 0)
			{
				pAttackEnemy = CAttackEnemy::Create(CAttack::ATTACKTYPE01_METEO, 10, 100, Pos,
					D3DXVECTOR3(sinf(-D3DX_PI * 0.5f + 0.1f * float(nActionNum)) * 15.0f, cosf(-D3DX_PI * 0.5f + 0.1f * float(nActionNum)) * 15.0f, 0.0f),
					ONE_VECTOR3 * 2.0f, NULL_VECTOR3, CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, NULL_VECTOR3, true, 60);

				pAttackEnemy->SetUseGravity(-0.4f);

				nActionNum++;
			}

			if (nActionNum % 30 == 0 && nActionNum > 0)
			{
				nPatternTime = 0;
				nSubPattern++;
				nActionNum = 0;
			}
		}

		if (nSubPattern == 5)
		{
			nPatternTime = 0;
			nSubPattern = 0;
			nActionNum = 0;
			nPattern++;
		}
		break;
	case 5://���ֈړ�
		Move.z = 10.0f;

		if (Pos.z > 500.0f && Pos.z < 520.0f)
		{
			Move = NULL_VECTOR3;
			nPatternTime = 0;
			nPattern++;
		}
		break;
	case 6:
		if (nPatternTime % 120 == 0)
		{
			float fSummonWidth = BOSSSTAGE_WIDTH / 10;
			int nRand = rand() % 10;
			for (int nCnt = 0; nCnt < 10; nCnt++)
			{
				if (nRand != nCnt)
				{
					pMrStrongAttack = CMrStrongAttack::Create(CAttack::ATTACKTYPE17_BLOCK,10,600,Pos,NULL_VECTOR3,ONE_VECTOR3 * 2,NULL_VECTOR3,
						CAttack::COLLISIONTYPE02_TRUERECTANGLEXY,NULL_VECTOR3,false,20);

					pMrStrongAttack->SetPurposePos(D3DXVECTOR3(-BOSSSTAGE_HALFWIDTH + fSummonWidth * nCnt + fSummonWidth / 2, 300.0f,Pos.z));//�u���b�N������
				}
			}
		}

		if (nPatternTime == 700)
		{
			nPattern++;
			nPatternTime = 0;
		}
		break;
	case 7:
		Move.z = -10.0f;

		if (Pos.z > -10.0f && Pos.z < 10.0f)
		{
			nPattern++;
			nPatternTime = 0;
			Move = NULL_VECTOR3;
		}
		break;
	case 8:
		if (nPatternTime == 90)
		{
			SetUseInteria(true);
			SetUseGravity(true);
			ResetPattern();
		}
		break;
	default:
		break;	      
	}
}
//======================================================================================================================================

//============================================<<<���C���[�N���X>>>=============================================================================

//==================================================================
//�R���X�g���N�^
//==================================================================
CRiley::CRiley()
{

}
//======================================================================================================================================

//==================================================================
//�f�X�g���N�^
//==================================================================
CRiley::~CRiley()
{

}
//======================================================================================================================================

//==================================================================
//����������
//==================================================================
HRESULT CRiley::Init()
{
	CBoss::Init();
	m_pSnowRamming = nullptr;//�̓�����̎��Ɏg���U��

	for (int nCnt = 0; nCnt < m_nMAX_REFLECTIONSNOWBURST; nCnt++)
	{
		m_pReflectionSnowBurst[nCnt] = nullptr;
	}
	m_bSwitchScale = false;
	return S_OK;
}
//======================================================================================================================================

//==================================================================
//�I������
//==================================================================
void CRiley::Uninit()
{
	CBoss::Uninit();
}
//======================================================================================================================================

//==================================================================
//�X�V����
//==================================================================
void CRiley::Update()
{
	CBoss::Update();
}
//======================================================================================================================================

//==================================================================
//�`�揈��
//==================================================================
void CRiley::Draw()
{
	int nCntTime = GetCntTime();
	CBoss::Draw();
}
//======================================================================================================================================

//==================================================================
//���S�t���O�ݒ菈��
//==================================================================
void CRiley::SetDeath()
{
	if (m_pSnowRamming != nullptr)
	{
		m_pSnowRamming->SetUseDeath(true);
		m_pSnowRamming->SetDeath();
		m_pSnowRamming = nullptr;
	}

	for (int nCnt = 0; nCnt < m_nMAX_REFLECTIONSNOWBURST; nCnt++)
	{
		if (m_pReflectionSnowBurst[nCnt] != nullptr)
		{
			m_pReflectionSnowBurst[nCnt]->SetUseDeath(true);
			m_pReflectionSnowBurst[nCnt]->SetDeath();
			m_pReflectionSnowBurst[nCnt] = nullptr;
		}
	}

	CBoss::SetDeath();
}
//======================================================================================================================================

//==================================================================
//��������
//==================================================================
CRiley* CRiley::Create(BOSSTYPE type, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 Scale)
{
	CRiley* pRiley = DBG_NEW CRiley;   //�{�X�𐶐�
	bool bSuccess = pRiley->GetCreateSuccess();
	int nIdx = 0;//���f���̃C���f�b�N�X
	if (bSuccess == true)
	{
		if (pRiley != nullptr)
		{
			pRiley->Init();                             //����������
			pRiley->SetUseDeath(true);                  //���S�t���O�𔭓����邩�ǂ�����ݒ肷��
			pRiley->SetType(type);                      //�{�X�̎��
			pRiley->SetLife(nLife);                     //�̗�
			pRiley->SetMaxLife(nLife);//�ő�̗�
			pRiley->SetAutoSubLife(false);              //�����I�ɑ̗͂����炷���ǂ���
			pRiley->CObjectX::SetPos(pos);                                                                                  //�I�u�W�F�N�g�Q�c�̈ʒu��ݒ�
			pRiley->SetPosOld(pos);                                                                                         //1f�O�̈ʒu��ݒ�
			pRiley->SetSupportPos(pos);                                                                                     //�ݒu�ʒu
			pRiley->CObject::SetType(CObject::TYPE_BOSS);                                                                   //�I�u�W�F�N�g�̎�ނ����߂�
			pRiley->CObjectX::SetScale(Scale);                                                                              //�I�u�W�F�N�g�̊g�嗦
			pRiley->SetFormarScale(Scale);                                                                                  //���̊g�嗦��ݒ肷��
			nIdx = CManager::GetObjectXInfo()->Regist(CObjectX::OBJECTXTYPE_BOSS, m_BOSS_FILENAME[(int)(type)]);
			pRiley->CObjectX::BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(CObjectX::OBJECTXTYPE_BOSS, nIdx),
				CManager::GetObjectXInfo()->GetBuffMat(CObjectX::OBJECTXTYPE_BOSS, nIdx),
				CManager::GetObjectXInfo()->GetdwNumMat(CObjectX::OBJECTXTYPE_BOSS, nIdx),
				CManager::GetObjectXInfo()->GetTexture(CObjectX::OBJECTXTYPE_BOSS, nIdx),
				CManager::GetObjectXInfo()->GetColorValue(CObjectX::OBJECTXTYPE_BOSS, nIdx));                                   //���f���������蓖�Ă�
			pRiley->SetBossHpGauge(nLife);                                                                                  //�̗̓Q�[�W��ݒ肷��
			pRiley->CObjectX::SetObjXType(CObjectX::OBJECTXTYPE_BOSS);                                                      //�I�u�W�F�N�gX�̃^�C�v��ݒ�
			pRiley->CObjectX::SetTypeNum((int)(type));                                                                      //�I�u�W�F�N�gX���Ƃ̃^�C�v�ԍ���ݒ�
			pRiley->SetSize();                                                                                              //X�I�u�W�F�N�g�̃T�C�Y��ݒ肷��
			pRiley->CObjectX::SetRotType(CObjectX::ROTTYPE::ROTTYPE_NORMAL);                                                //�����̎�ނ�ݒ�
			pRiley->SetManagerType((int)(CStageManager::MANAGEROBJECT_BOSS));                                               //�}�l�[�W���[�ŌĂяo�����̎�ނ�ݒ�
			pRiley->AdjustSize();      //����T�C�Y�������s��
		}
		else
		{//�I�u�W�F�N�g�ɋ󂫂��Ȃ������̂Ŕj������
			delete pRiley;
			pRiley = nullptr;
			return nullptr;
		}
	}
	return pRiley;
}
//======================================================================================================================================

//==================================================================
//�ړ��p�^�[��
//==================================================================
void CRiley::MovePattern()
{
	int& nMovePatternFrame = GetMovePatternFrame();
	int& nMaxMovePatternFrame = GetMaxMovePatternFrame();
	bool& bUseMovePattern = GetUseMovePattern();//�ړ��p�^�[�����g�p���Ă��邩�ǂ���
	int& nMovePatternNum = GetMovePatternNum();//�ړ��p�^�[���ԍ����擾����
	D3DXVECTOR3& Move = GetMove();
	D3DXVECTOR3 Pos = GetPos();

	CBoss::MovePattern();//�ړ��p�^�[���̋��ʏ���

	DecideMovePattern();//�ړ��p�^�[�����菈��

	//�ړ��p�^�[�����J�n����
	switch (nMovePatternNum)
	{
	case 0:
		MovePattern0();//�ړ��p�^�[���O
		break;
	case 1:
		MovePattern1();//�ړ��p�^�[���P
		break;
	default:
		break;
	}

	//�ړ��p�^�[�����I��点��
	if (nMovePatternFrame > nMaxMovePatternFrame)
	{
		bUseMovePattern = false;//�A�N�V�����p�^�[���ֈڍs
		nMovePatternFrame = 0;
		Move = NULL_VECTOR3;
	}

}
//======================================================================================================================================

//==================================================================
//�ړ��p�^�[�������߂鏈��
//==================================================================
void CRiley::DecideMovePattern()
{
	int& nMovePatternFrame = GetMovePatternFrame();
	int& nMaxMovePatternFrame = GetMaxMovePatternFrame();
	bool& bUseMovePattern = GetUseMovePattern();//�ړ��p�^�[�����g�p���Ă��邩�ǂ���
	int& nMovePatternNum = GetMovePatternNum();//�ړ��p�^�[���ԍ����擾����
	D3DXVECTOR3 Pos = GetPos();
	D3DXVECTOR3 PlayerPos = CGame::GetPlayer()->GetPos();

	//���������
	float fLength = CCalculation::CalculationLength(Pos,PlayerPos);

	if (nMovePatternFrame == 1)
	{//�ŏ��Ɉړ����@�����߂�

		if (fLength < m_fMOVEPATTERNLENGTH)
		{//�������߂����
			nMovePatternNum = 0;
		}
		else
		{//�������������
			nMovePatternNum = 1;
		}

		//���ꂼ��̈ړ��p�^�[���̍ő�t���[������ݒ肷��
		switch (nMovePatternNum)
		{
		case 0:
			nMaxMovePatternFrame = m_nMAX_MOVEPATTERN_0_FRAME;
			break;
		case 1:
			nMaxMovePatternFrame = m_nMAX_MOVEPATTERN_1_FRAME;
			break;
		default:
			break;
		}
	}
}
//======================================================================================================================================

//==================================================================
//�ړ��O�i���������j
//==================================================================
void CRiley::MovePattern0()
{
	D3DXVECTOR3& Move = GetMove();
	D3DXVECTOR3& Pos = GetPos();
	D3DXVECTOR3 PlayerPos = CGame::GetPlayer()->GetPos();
	int nMovePatternFrame = GetMovePatternFrame();

	SetInertia(0.05f);//����������������
	if (nMovePatternFrame == 30)
	{
		if (Pos.x > PlayerPos.x)
		{
			Move.x = 10.0f;
		}
		else
		{
			Move.x = -10.0f;
		}
	}

	if (nMovePatternFrame == m_nMAX_MOVEPATTERN_0_FRAME)
	{
		SetUndoInertia();//���������ɖ߂�
		nMovePatternFrame = 0;
	}
}
//======================================================================================================================================


//==================================================================
//�ړ��P�i�߂Â��j
//==================================================================
void CRiley::MovePattern1()
{
	D3DXVECTOR3& Move = GetMove();
	D3DXVECTOR3& Pos = GetPos();
	D3DXVECTOR3 PlayerPos = CGame::GetPlayer()->GetPos();
	int nMovePatternFrame = GetMovePatternFrame();

	SetInertia(0.05f);//����������������
	if (nMovePatternFrame == 30)
	{
		if (Pos.x > PlayerPos.x)
		{
			Move.x = -10.0f;
		}
		else
		{
			Move.x = 10.0f;
		}
	}

	if (nMovePatternFrame == m_nMAX_MOVEPATTERN_0_FRAME)
	{
		nMovePatternFrame = 0;
		SetUndoInertia();//���������ɖ߂�
	}
}
//======================================================================================================================================

//==================================================================
//�s���p�^�[��
//==================================================================
void CRiley::ActionPattern()
{
	bool& bAction = GetAction();    //�s�����Ă��邩�ǂ���
	int& nSkill = GetSkill();       //�X�L��
	bool& bInsanity = GetInsanity();//�������Ă��邩�ǂ���
	int nLife = GetLife();          //�̗�
	int& nCntTurn = GetTurnCnt();   //�^�[�������J�E���g
	int nMaxLife = GetMaxLife();    //�ő�̗�
	float fRatioLife = 0.0f;        //�̗͂̊���

	fRatioLife = float(nLife) / float(nMaxLife);

	if (fRatioLife < 0.5f)
	{//�̗͂��T�O������������甭����Ԃɂ���
		bInsanity = true;
	}

	if (bAction == false)
	{
		if (bInsanity == false)
		{//������Ԃ���Ȃ����
			nSkill = rand() % 4;
			//nSkill = 5;//�K�E�Z�𔭓�
		}
		else
		{//������ԂȂ�
			nCntTurn++;//��������^�[�������J�E���g
			nSkill = rand() % 6;
			if (nCntTurn == 1)
			{
				nSkill = 5;//�K�E�Z�𔭓�
			}
		}
		bAction = true;
	}

	if (bAction == true)
	{
		CBoss::ActionPattern();
		switch (nSkill)
		{
		case 0:
			Action0();
			break;
		case 1:
			Action1();
			break;
		case 2:
			Action2();
			break;
		case 3:
			Action4();
			break;
		case 4:
			Action3();
			break;
		case 5:
			Action5();
			break;
		default:
			break;
		}
	}
}
//======================================================================================================================================

//==================================================================
//�T�C�Y�𒲐�����
//==================================================================
void CRiley::AdjustSize()
{
	D3DXVECTOR3& VtxMax = GetOriginVtxMax();
	D3DXVECTOR3& VtxMin = GetOriginVtxMin();

	VtxMax.x -= 20.0f;
	VtxMin.x += 20.0f;
	VtxMax.y -= 10.0f;

}
//======================================================================================================================================

//==================================================================
//�s���O
//==================================================================
void CRiley::Action0()
{
	int& nPatternTime = GetPatternTime();         //�p�^�[�����Ԃ��擾����
	bool& bAction = GetAction();                  //�s�����Ă��邩�ǂ������擾
	int& nPattern = GetPattern();                 //���݂̃p�^�[�����擾
	int& nActionNum = GetActionNum();             //�A�N�V�����񐔂��J�E���g

	D3DXVECTOR3& Pos = GetPos();                  //�ʒu���擾
	D3DXVECTOR3& Rot = GetRot();                  //�������擾
	D3DXVECTOR3& Move = GetMove();                //�ړ��ʂ��擾
	D3DXVECTOR3 PlayerPos = CGame::GetPlayer()->GetPos();//�v���C���[�̈ʒu
	CBullet* pBullet = nullptr;                   //�o���b�g�ւ̃|�C���^
	float fRandRot = 0.0f;                        //��������	
	float fRandSpeed = 0.0f;                      //�������x
	float fLength = 0.0f;                         //����

	//�������ړ��֌W
	switch (nPattern)
	{
	case 0://���߂�
		if (nPatternTime == 45)
		{//�`���[�W
			CParticle::SummonChargeParticle(CParticle::TYPE00_NORMAL, 120, 120, 5.0f, 30.0f, 30.0f, 300, 100, 10, false,
				Pos, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), true);

			nPatternTime = 0;
			nPattern++;
			m_pSnowRamming = CRileyAttack::Create(CAttack::ATTACKTYPE11_EXPLOSION, 20, 360, Pos, NULL_VECTOR3, ONE_VECTOR3, NULL_VECTOR3,
				CAttack::COLLISIONTYPE03_BALL, ONE_VECTOR3 * 0.1f, false, 10);
			m_pSnowRamming->SetUseDeath(false);
		}
		break;
	case 1:
		SetUseInteria(false);
		m_pSnowRamming->SetPos(Pos);
		if (Pos.x > PlayerPos.x)
		{
			Move.x -= 0.1f;
			Rot.z += 0.2f;
		}
		else
		{
			Move.x += 0.1f;
			Rot.z -= 0.2f;
		}
		if (nPatternTime % 45 == 0)
		{
			m_bSwitchScale = m_bSwitchScale ? false : true;
			nActionNum++;
		}

		if (m_bSwitchScale == false)
		{
			m_pSnowRamming->SetUseAddScale(ONE_VECTOR3 * 0.25f, true);
		}
		else
		{
			m_pSnowRamming->SetUseAddScale(ONE_VECTOR3 * -0.25f, true);
		}

		if (nActionNum == 6)
		{
			m_pSnowRamming->SetUseDeath(true);
			m_pSnowRamming->SetDeath();
			m_pSnowRamming = nullptr;
			Move = NULL_VECTOR3;
			Rot = NULL_VECTOR3;
			SetUseInteria(true);
			ResetPattern();
		}

		break;
	default:
		break;
	}
}
//=====================================================================================================================================

//==================================================================
//�s���P�i�N���X�̓�����j
//==================================================================
void CRiley::Action1()
{
	int& nPatternTime = GetPatternTime();         //�p�^�[�����Ԃ��擾����
	bool& bAction = GetAction();                  //�s�����Ă��邩�ǂ������擾
	int& nPattern = GetPattern();                 //���݂̃p�^�[�����擾
	int& nActionNum = GetActionNum();             //�A�N�V�����񐔂��J�E���g
	int& nSubPattern = GetSubPattern();           //�T�u�p�^�[�����擾
	bool bIsLanding = GetLanding();               //�n�ʂɂ��邩�ǂ������擾
	D3DXVECTOR3& Pos = GetPos();                  //�ʒu���擾
	D3DXVECTOR3& Rot = GetRot();                  //�������擾
	D3DXVECTOR3& Move = GetMove();                //�ړ��ʂ��擾
	D3DXVECTOR3 PlayerPos = CGame::GetPlayer()->GetPos();//�v���C���[�̈ʒu
	CBullet* pBullet = nullptr;                   //�o���b�g�ւ̃|�C���^

	if (m_pSnowRamming != nullptr)
	{
		m_pSnowRamming->SetPos(Pos);//��Ƀ{�X�̈ʒu�ɌŒ�
	}
	SetUseInteria(false);
	switch (nPattern)
	{
	case 0://���߂�
		if (nPatternTime < 15)
		{//�ԃ`���[�W
			CParticle::SummonChargeParticle(CParticle::TYPE00_NORMAL, 10, 120, 5.0f, 30.0f, 30.0f, 300, 100, 10, false,
				Pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), true);

			if (nPatternTime == 1)
			{
				m_pSnowRamming = CRileyAttack::Create(CAttack::ATTACKTYPE11_EXPLOSION, 20, 360, Pos, NULL_VECTOR3, ONE_VECTOR3, NULL_VECTOR3,
					CAttack::COLLISIONTYPE03_BALL, ONE_VECTOR3 * 0.1f, false, 10);
				m_pSnowRamming->SetUseDeath(false);
				m_pSnowRamming->SetUseAddScale(ONE_VECTOR3 * 0.12f, true);
			}
		}
		
		if (nPatternTime == 60)
		{
			m_pSnowRamming->SetUseAddScale(NULL_VECTOR3, false);
			nPattern++;
			nPatternTime = 0;
		}
		break;
	case 1:
		if (nPatternTime == 1)
		{
			if (Pos.x < PlayerPos.x)
			{//�v���C���[���E���ɂ���
				Move.x = 12.0f + 1.5f * float(nActionNum);
			}
			else
			{//�v���C���[�������ɂ���
				Move.x = -12.0f - 1.5f * float(nActionNum);
			}
		}

		if (nPatternTime > 5)
		{
			if (Pos.x + GetVtxMax().x > BOSSSTAGE_HALFWIDTH || Pos.x + GetVtxMin().x < -BOSSSTAGE_HALFWIDTH)//��ʒ[�ɓ���������E�E�E
			{
				CParticle::SummonChargeParticle(CParticle::TYPE00_NORMAL, 120, 120, 5.0f, 30.0f, 30.0f, 300, 100, 10, false,
					Pos, NORMAL_COL, true);//���݂̈ʒu�Ƀp�[�e�B�N��������
				Pos = PlayerPos + D3DXVECTOR3(0.0f, 400.0f, 0.0f);//�v���C���[�̓���Ƀ��[�v
				Move = NULL_VECTOR3;//�������~�߂�
				CParticle::SummonParticle(CParticle::TYPE00_NORMAL, 100, 60, 30.0f, 30.0f, 100, 10, false, Pos, NORMAL_COL, true);//���[�v��Ƀp�[�e�B�N��������

				nPattern++;
				nPatternTime = 0;
			}
		}
		break;
	case 2:
		if (bIsLanding == true)
		{
			if (nActionNum < 6)
			{
				nActionNum++;
				nPatternTime = 0;
				nPattern = 1;
			}
			else
			{
				m_pSnowRamming->SetUseDeath(true);
				m_pSnowRamming->SetDeath();
				m_pSnowRamming = nullptr;
				SetUseInteria(true);
				ResetPattern();
			}
		}
		break;
	default:
		break;
	}

}
//=====================================================================================================================================

//==================================================================
//�s���Q�i���˃o�[�X�g�j
//==================================================================
void CRiley::Action2()
{
	int& nPatternTime = GetPatternTime();         //�p�^�[�����Ԃ��擾����
	bool& bAction = GetAction();                  //�s�����Ă��邩�ǂ������擾
	int& nPattern = GetPattern();                 //���݂̃p�^�[�����擾
	int& nActionNum = GetActionNum();             //�A�N�V�����񐔂��J�E���g
	int& nSubPattern = GetSubPattern();           //�T�u�p�^�[�����擾
	bool bIsLanding = GetLanding();               //�n�ʂɂ��邩�ǂ������擾
	D3DXVECTOR3& Pos = GetPos();                  //�ʒu���擾
	D3DXVECTOR3& Rot = GetRot();                  //�������擾
	D3DXVECTOR3& Move = GetMove();                //�ړ��ʂ��擾
	D3DXVECTOR3 PlayerPos = CGame::GetPlayer()->GetPos();//�v���C���[�̈ʒu
	float fRandRot = 0.0f;
	bool bNextPattern = false;
	if (m_pSnowRamming != nullptr)
	{
		m_pSnowRamming->SetPos(Pos);//��Ƀ{�X�̈ʒu�ɌŒ�
	}
	switch (nPattern)
	{
	case 0://���߂�
		if (nPatternTime == 30)
		{//�΃`���[�W
			CParticle::SummonChargeParticle(CParticle::TYPE00_NORMAL, 100, 120, 5.0f, 30.0f, 30.0f, 300, 100, 10, false,
				Pos, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), true);
		}

		if (nPatternTime == 80)
		{
			nPattern++;
			nPatternTime = 0;
		}
		break;
	case 1:
		for (int nCnt = 0; nCnt < m_nMAX_REFLECTIONSNOWBURST; nCnt++)
		{
			fRandRot = CCalculation::CalculationRandVecXY();
			m_pReflectionSnowBurst[nCnt] = CRileyAttack::Create(CAttack::ATTACKTYPE11_EXPLOSION, 12, 240, GetSenterPos(),
				D3DXVECTOR3(sinf(fRandRot) * 10.0f, cosf(fRandRot) * 10.0f, 0.0f), ONE_VECTOR3 * 2, NULL_VECTOR3, CAttack::COLLISIONTYPE03_BALL, NULL_VECTOR3, false, 10);
			m_pReflectionSnowBurst[nCnt]->SetUseDeath(false);
			m_pReflectionSnowBurst[nCnt]->SetUseMultiSpeed(true, ONE_VECTOR3 * 0.99f);
			m_pReflectionSnowBurst[nCnt]->SetUseReflection(true);
			m_pReflectionSnowBurst[nCnt]->SetUseBlockCollision(true);
		}

		nPattern++;
		nPatternTime = 0;
		break;
	case 2:
		bNextPattern = true;
		for (int nCntSnow = 0; nCntSnow < m_nMAX_REFLECTIONSNOWBURST; nCntSnow++)
		{
			if (m_pReflectionSnowBurst[nCntSnow] != nullptr)
			{
				if (m_pReflectionSnowBurst[nCntSnow]->GetLife() < 2)
				{
					for (int nCnt = 0; nCnt < m_nMAX_ACTION2BULLETNUM; nCnt++)
					{
						fRandRot = (2.0f / m_nMAX_ACTION2BULLETNUM) * nCnt;
						CBullet::Create(CBullet::BULLETTYPE_ENEMY, CBullet::BULLETTEX_ENEMY, 120, 10, 24.0f, 24.0f,
							m_pReflectionSnowBurst[nCntSnow]->GetSenterPos(), D3DXVECTOR3(sinf(D3DX_PI * fRandRot) * 12.0f, cosf(D3DX_PI * fRandRot) * 12.0f, 0.0f));
					}

					m_pReflectionSnowBurst[nCntSnow]->SetUseDeath(true);
					m_pReflectionSnowBurst[nCntSnow]->SetDeath();
					m_pReflectionSnowBurst[nCntSnow] = nullptr;
				}
			}

			if (m_pReflectionSnowBurst[nCntSnow] != nullptr)
			{
				bNextPattern = false;
			}
		}
		if (bNextPattern == true)
		{
			ResetPattern();
		}
		break;
	default:
		break;
	}
}
//=====================================================================================================================================

//==================================================================
//�s���R�i�g���C�A�^�b�N�j
//==================================================================
void CRiley::Action3()
{
	int& nPatternTime = GetPatternTime();         //�p�^�[�����Ԃ��擾����
	bool& bAction = GetAction();                  //�s�����Ă��邩�ǂ������擾
	int& nPattern = GetPattern();                 //���݂̃p�^�[�����擾
	int& nActionNum = GetActionNum();             //�A�N�V�����񐔂��J�E���g
	int& nSubPattern = GetSubPattern();           //�T�u�p�^�[�����擾
	D3DXVECTOR3& Pos = GetPos();                  //�ʒu���擾
	D3DXVECTOR3& Rot = GetRot();                  //�������擾
	D3DXVECTOR3& Move = GetMove();                //�ړ��ʂ��擾
	D3DXVECTOR3 PlayerPos = CGame::GetPlayer()->GetPos();//�v���C���[�̈ʒu
	float fRandRot = 0.0f;
	CRileyAttack* pRileyAttack = nullptr;
	switch (nPattern)
	{
	case 0://���߂�
		if (nPatternTime == 30)
		{//�΃`���[�W
			CParticle::SummonChargeParticle(CParticle::TYPE00_NORMAL, 100, 120, 5.0f, 30.0f, 30.0f, 300, 100, 10, false,
				Pos, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), true);
		}

		if (nPatternTime == 80)
		{
			nPattern++;
			nPatternTime = 0;
		}
		break;
	case 1:
		if (nPatternTime == 1)
		{
			for (int nCnt = 0; nCnt < m_nMAX_ACTION03_ATTACKNUM; nCnt++)
			{
				pRileyAttack = CRileyAttack::Create(CAttack::ATTACKTYPE11_EXPLOSION, 10, 240,
					D3DXVECTOR3(-BOSSSTAGE_HALFWIDTH, (BOSSSTAGE_HEIGHT / m_nMAX_ACTION03_ATTACKNUM) * nCnt + 30.0f, 0.0f)
					, NULL_VECTOR3, ONE_VECTOR3 * 2, NULL_VECTOR3, CAttack::COLLISIONTYPE03_BALL, NULL_VECTOR3, false, 30);
				pRileyAttack->SetSubType(rand() % CRileyAttack::m_nMAX_ACTION03_SUBTYPE);
				pRileyAttack->SetAttackMove(CRileyAttack::ATTACKMOVE_RILEY_01_ACTION03);
			}

			for (int nCnt = 0; nCnt < m_nMAX_ACTION03_ATTACKNUM; nCnt++)
			{
				pRileyAttack = CRileyAttack::Create(CAttack::ATTACKTYPE11_EXPLOSION, 10, 240,
					D3DXVECTOR3(BOSSSTAGE_HALFWIDTH, (BOSSSTAGE_HEIGHT / m_nMAX_ACTION03_ATTACKNUM) * nCnt + 30.0f, 0.0f)
					, NULL_VECTOR3, ONE_VECTOR3 * 2, NULL_VECTOR3, CAttack::COLLISIONTYPE03_BALL, NULL_VECTOR3, false, 30);
				pRileyAttack->SetSubType(rand() % CRileyAttack::m_nMAX_ACTION03_SUBTYPE);
				pRileyAttack->SetAttackMove(CRileyAttack::ATTACKMOVE_RILEY_01_ACTION03);
			}
		}

		if (nPatternTime == 100)
		{
			nPatternTime = 0;
			nPattern++;
		}
		break;
	case 2:
		if (nPatternTime == 1)
		{
			for (int nCnt = 0; nCnt < m_nMAX_ACTION03_ATTACKNUM * 2; nCnt++)
			{
				pRileyAttack = CRileyAttack::Create(CAttack::ATTACKTYPE11_EXPLOSION, 10, 240,
					D3DXVECTOR3(-BOSSSTAGE_HALFWIDTH + (BOSSSTAGE_WIDTH / (m_nMAX_ACTION03_ATTACKNUM * 2)) * nCnt,BOSSSTAGE_HEIGHT - 20.0f, 0.0f)
					, NULL_VECTOR3, ONE_VECTOR3 * 2, NULL_VECTOR3, CAttack::COLLISIONTYPE03_BALL, NULL_VECTOR3, false, 30);
				pRileyAttack->SetSubType(rand() % CRileyAttack::m_nMAX_ACTION03_SUBTYPE);
				pRileyAttack->SetAttackMove(CRileyAttack::ATTACKMOVE_RILEY_01_ACTION03);
			}

		}

		if (nPatternTime == 140)
		{
			ResetPattern();
		}
		break;
	default:
		break;
	}
}
//=====================================================================================================================================

//==================================================================
//�s���S�i�A�C�X�G�C�W�j
//==================================================================
void CRiley::Action4()
{
	int& nPatternTime = GetPatternTime();         //�p�^�[�����Ԃ��擾����
	bool& bAction = GetAction();                  //�s�����Ă��邩�ǂ������擾
	int& nPattern = GetPattern();                 //���݂̃p�^�[�����擾
	int& nActionNum = GetActionNum();             //�A�N�V�����񐔂��J�E���g
	int& nSubPattern = GetSubPattern();           //�T�u�p�^�[�����擾
	D3DXVECTOR3& Pos = GetPos();                  //�ʒu���擾
	D3DXVECTOR3& Rot = GetRot();                  //�������擾
	D3DXVECTOR3& Move = GetMove();                //�ړ��ʂ��擾
	D3DXVECTOR3 PlayerPos = CGame::GetPlayer()->GetPos();//�v���C���[�̈ʒu
	bool& bIsLanding = GetLanding();
	float fRandRot = 0.0f;
	CRileyAttack* pRileyAttack = nullptr;
	switch (nPattern)
	{
	case 0://���߂�
		if (nPatternTime == 30)
		{//���F�`���[�W
			CParticle::SummonChargeParticle(CParticle::TYPE00_NORMAL, 100, 120, 5.0f, 30.0f, 30.0f, 300, 100, 10, false,
				Pos, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), true);
		}

		if (nPatternTime == 80)
		{
			nPattern++;
			nPatternTime = 0;
		}
		break;
	case 1:
		SetUseInteria(false);
		if (GetSenterPos().x < CGame::GetPlayer()->GetPos().x)
		{
			Move.x = 3.0f;
		}
		else
		{
			Move.x = -3.0f;
		}

		if (bIsLanding == true)
		{
			nActionNum++;
			Move.y = 10.0f * nActionNum;
		}

		if (nActionNum == 3)
		{
			nPattern++;
			nPatternTime = 0;
			nActionNum = 0;
		}
		break;
	case 2:
		if (bIsLanding == true)
		{
			if (nPatternTime % 5 == 0)
			{
				CManager::GetCamera()->SetShake(3, 0.4f);
				nActionNum++;
				if (nActionNum < m_nMAX_ICE_EIGE)
				{
					pRileyAttack = CRileyAttack::Create(CAttack::ATTACKTYPE05_ICE, 30, 15, Pos + D3DXVECTOR3(50.0f * nActionNum, -40.0f, 0.0f), NULL_VECTOR3,
						ONE_VECTOR3, NULL_VECTOR3, CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, NULL_VECTOR3, false, 100);
					pRileyAttack->SetUseAddScale(D3DXVECTOR3(0.0f, 0.2f * nActionNum, 0.0f), true);

					pRileyAttack = CRileyAttack::Create(CAttack::ATTACKTYPE05_ICE, 30, 15, Pos - D3DXVECTOR3(50.0f * nActionNum,40.0f, 0.0f), NULL_VECTOR3,
						ONE_VECTOR3, NULL_VECTOR3, CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, NULL_VECTOR3, false, 100);
					pRileyAttack->SetUseAddScale(D3DXVECTOR3(0.0f, 0.2f * nActionNum, 0.0f), true);
				}
				else
				{
					SetUseInteria(true);
					ResetPattern();
				}
			}
		}
		else
		{//�n�ʂɒ����܂Ńv���C���[��ǂ�������
			if (nPatternTime < 30)
			{
				if (GetSenterPos().x < CGame::GetPlayer()->GetPos().x)
				{
					Move.x = 10.0f;
				}
				else
				{
					Move.x = -10.0f;
				}
			}
			else
			{
				Move.x = 0.0f;
			}
		}
		break;
	default:
		break;
	}
}
//=====================================================================================================================================

//==================================================================
//�s���T
//==================================================================
void CRiley::Action5()
{
	int& nPatternTime = GetPatternTime();         //�p�^�[�����Ԃ��擾����
	bool& bAction = GetAction();                  //�s�����Ă��邩�ǂ������擾
	int& nPattern = GetPattern();                 //���݂̃p�^�[�����擾
	int& nActionNum = GetActionNum();             //�A�N�V�����񐔂��J�E���g
	int& nSubPattern = GetSubPattern();           //�T�u�p�^�[�����擾
	D3DXVECTOR3& Pos = GetPos();                  //�ʒu���擾
	D3DXVECTOR3& Rot = GetRot();                  //�������擾
	D3DXVECTOR3& Move = GetMove();                //�ړ��ʂ��擾
	D3DXVECTOR3 PlayerPos = CGame::GetPlayer()->GetPos();//�v���C���[�̈ʒu

	D3DXVECTOR3& PurposePos = GetPurposePos();//�ړI�n���g�p
	bool& bIsLanding = GetLanding();
	bool& bIsWalling = GetWalling();
	float fRandRot = 0.0f;
	CRileyAttack* pRileyAttack = nullptr;
	if (m_pSnowRamming != nullptr)
	{
		m_pSnowRamming->SetPos(Pos);
	}


	SetUseInteria(false);
	SetUseGravity(false);
	switch (nPattern)
	{
	case 0://���߂�
		if (nPatternTime == 30)
		{//���F�`���[�W
			CParticle::SummonChargeParticle(CParticle::TYPE00_NORMAL, 100, 120, 5.0f, 30.0f, 30.0f, 300, 100, 10,true,
				Pos, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), true);

			m_pSnowRamming = CRileyAttack::Create(CAttack::ATTACKTYPE11_EXPLOSION, 20, 360, Pos, NULL_VECTOR3, ONE_VECTOR3, NULL_VECTOR3,
				CAttack::COLLISIONTYPE03_BALL, ONE_VECTOR3 * 0.1f, false, 10);
			m_pSnowRamming->SetUseDeath(false);
			m_pSnowRamming->SetUseAddScale(ONE_VECTOR3 * 0.2f, true);

		}

		if (nPatternTime == 80)
		{
			m_pSnowRamming->SetUseAddScale(NULL_VECTOR3, false);
			nPattern++;
			nPatternTime = 0;
		}
		break;
	case 1:
		PurposePos = D3DXVECTOR3(0.0f, 300.0f, 0.0f);
		fRandRot = CCalculation::CalculationXYaim(GetSenterPos(),PurposePos);
		Move.x = sinf(fRandRot) * 10.0f;
		Move.y = cosf(fRandRot) * 10.0f;

		if (GetSenterPos().x > PurposePos.x - 20.0f && GetSenterPos().x < PurposePos.x + 20.0f &&
			GetSenterPos().y > PurposePos.y - 20.0f && GetSenterPos().y < PurposePos.y + 20.0f)
		{
			Pos = PurposePos;
			Move = NULL_VECTOR3;
			nPattern++;
			nPatternTime = 0;
		}
		break;
	case 2:
		if (nPatternTime < 60)
		{
			CParticle::SummonChargeParticle(CParticle::TYPE00_NORMAL, 5, 60, 10.0f, 30.0f, 30.0f, 500, 100, 10, false,
				Pos, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), true);
		}

		if (nPatternTime == 120)
		{
			m_pSnowRamming->SetUseAttackEffect(true, 10, D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f));
			nPattern++;
			nPatternTime = 0;
		}
		break;
	case 3://�U���J�n
		Move.x = -20.0f;
		if (bIsWalling == true)
		{
			Pos.x = BOSSSTAGE_HALFWIDTH;
			nActionNum++;
		}
		fRandRot = CCalculation::CalculationRandVecXY();

		if (nPatternTime % 5 == 0)
		{
			CBullet::Create(CBullet::BULLETTYPE_ENEMY, CBullet::BULLETTEX_ENEMY, 120, 10, 20.0f, 20.0f, Pos, D3DXVECTOR3(sinf(fRandRot) * 5.0f, cosf(fRandRot) * 5.0f, 0.0f));
		}

		if (nActionNum == 10)
		{//10�T�����玟�̃p�^�[����
			nPattern++;
			nPatternTime = 0;
			nActionNum = 0;
		}
		break;
	case 4:
		if (nPatternTime <= 1)
		{
			PurposePos.x = float(rand() % 720 - 360);
		}

		if (GetSenterPos().x > PurposePos.x - 20.0f && GetSenterPos().x < PurposePos.x + 20.0f &&
			GetSenterPos().y > PurposePos.y - 20.0f && GetSenterPos().y < PurposePos.y + 20.0f)
		{
			Move = NULL_VECTOR3;
			nPattern++;
			nPatternTime = 0;
		}
		break;
	case 5:
		if (nPatternTime < 30)
		{
			CParticle::SummonChargeParticle(CParticle::TYPE00_NORMAL, 5, 60, 10.0f, 30.0f, 30.0f, 500, 100, 10, false,
				Pos, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), true);
			m_pSnowRamming->SetUseAddScale(ONE_VECTOR3 * 0.2f, true);
		}

		if (nPatternTime == 70)
		{
			SetUseAdjustOriginPos(false);
			m_pSnowRamming->SetUseAddScale(NULL_VECTOR3,false);
			fRandRot = CCalculation::CalculationXYaim(Pos, PlayerPos);
			PurposePos = CGame::GetPlayer()->GetPos();
			Move.x = sinf(fRandRot) * 3.0f;
			Move.y = cosf(fRandRot) * 3.0f;
		}

		if (nPatternTime > 70)
		{
			if (bIsLanding == true)
			{
				SetUseAdjustOriginPos(true);
				Move = NULL_VECTOR3;
				SetUseGravity(true);
				SetUseInteria(true);
				m_pSnowRamming->SetUseDeath(true);
				m_pSnowRamming->SetDeath();
				m_pSnowRamming = nullptr;
				CParticle::SummonParticle(CParticle::TYPE02_ICE, 500, 60, 120.0f, 120.0f, 200, 10, true, Pos, NORMAL_COL, true);
				ResetPattern();
			}
		}
		break;
	default:
		break;
	}
}
//=====================================================================================================================================

//============================================<<<�h���C���N���X>>>=============================================================================	

//==================================================================
//�R���X�g���N�^
//==================================================================
CDraiya::CDraiya() : m_nNumThunder(0),m_pSpecialThunder()
{

}
//=====================================================================================================================================

//==================================================================
//�f�X�g���N�^
//==================================================================
CDraiya::~CDraiya()
{

}
//=====================================================================================================================================

//==================================================================
//����������
//==================================================================
HRESULT CDraiya::Init()
{
	CBoss::Init();//�{�X������
	m_pAnnihilationThunder = nullptr;

	for (int nCnt = 0; nCnt < m_nMAX_ACTION2THUNDERNUM; nCnt++)
	{
		m_bAnnihilationThunderShot[nCnt] = false;
	}

	for (int nCnt = 0; nCnt < m_nMAX_ACTION03_RAMMING_NUM; nCnt++)
	{
		m_RammingSavePos[nCnt] = NULL_VECTOR3;
		m_fSaveRammingRot[nCnt] = 0.0f;
	}

	return S_OK;
}
//=====================================================================================================================================

//==================================================================
//�I������
//==================================================================
void CDraiya::Uninit()
{
	CBoss::Uninit();//�{�X�I������
}
//=====================================================================================================================================

//==================================================================
//�X�V����
//==================================================================
void CDraiya::Update()
{
	CBoss::Update();//�{�X�X�V����
}
//=====================================================================================================================================

//==================================================================
//�`�揈��
//==================================================================
void CDraiya::Draw()
{
	int nCntTime = GetCntTime();
	CBoss::Draw();//�{�X�`�揈��
}
//=====================================================================================================================================

//==================================================================
//���S�t���O�ݒ菈��
//==================================================================
void CDraiya::SetDeath()
{
	if (m_pAnnihilationThunder != nullptr)
	{
		m_pAnnihilationThunder->SetUseDeath(true);
		m_pAnnihilationThunder->SetDeath();
		m_pAnnihilationThunder = nullptr;
	}

	for (int nCnt = 0; nCnt < m_nNUMSPECIALATTACK; nCnt++)
	{
		if (m_pSpecialThunder[nCnt] != nullptr)
		{
			m_pSpecialThunder[nCnt]->SetUseDeath(true);
			m_pSpecialThunder[nCnt]->SetDeath();
			m_pSpecialThunder[nCnt] = nullptr;
		}
	}
	CBoss::SetDeath();//�{�X���S�t���O�ݒ菈��
}
//=====================================================================================================================================

//==================================================================
//��������
//==================================================================
CDraiya* CDraiya::Create(BOSSTYPE type, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 Scale)
{
	CDraiya* pDraiya = DBG_NEW CDraiya;   //�{�X�𐶐�
	bool bSuccess = pDraiya->GetCreateSuccess();
	int nIdx = 0;//���f���̃C���f�b�N�X
	if (bSuccess == true)
	{
		if (pDraiya != nullptr)
		{
			pDraiya->Init();                             //����������
			pDraiya->SetUseDeath(true);                  //���S�t���O�𔭓����邩�ǂ�����ݒ肷��
			pDraiya->SetType(type);                      //�{�X�̎��
			pDraiya->SetLife(nLife);                     //�̗�
			pDraiya->SetMaxLife(nLife);//�ő�̗�
			pDraiya->SetAutoSubLife(false);              //�����I�ɑ̗͂����炷���ǂ���
			pDraiya->CObjectX::SetPos(pos);                                                                                  //�I�u�W�F�N�g�Q�c�̈ʒu��ݒ�
			pDraiya->SetPosOld(pos);                                                                                         //1f�O�̈ʒu��ݒ�
			pDraiya->SetSupportPos(pos);                                                                                     //�ݒu�ʒu
			pDraiya->CObject::SetType(CObject::TYPE_BOSS);                                                                   //�I�u�W�F�N�g�̎�ނ����߂�
			pDraiya->CObjectX::SetScale(Scale);                                                                              //�I�u�W�F�N�g�̊g�嗦
			pDraiya->SetFormarScale(Scale);                                                                                  //���̊g�嗦��ݒ肷��
			nIdx = CManager::GetObjectXInfo()->Regist(CObjectX::OBJECTXTYPE_BOSS, m_BOSS_FILENAME[(int)(type)]);
			pDraiya->CObjectX::BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(CObjectX::OBJECTXTYPE_BOSS, nIdx),
				CManager::GetObjectXInfo()->GetBuffMat(CObjectX::OBJECTXTYPE_BOSS, nIdx),
				CManager::GetObjectXInfo()->GetdwNumMat(CObjectX::OBJECTXTYPE_BOSS, nIdx),
				CManager::GetObjectXInfo()->GetTexture(CObjectX::OBJECTXTYPE_BOSS, nIdx),
				CManager::GetObjectXInfo()->GetColorValue(CObjectX::OBJECTXTYPE_BOSS, nIdx));                                   //���f���������蓖�Ă�
			pDraiya->SetBossHpGauge(nLife);                                                                                  //�̗̓Q�[�W��ݒ肷��
			pDraiya->CObjectX::SetObjXType(CObjectX::OBJECTXTYPE_BOSS);                                                      //�I�u�W�F�N�gX�̃^�C�v��ݒ�
			pDraiya->CObjectX::SetTypeNum((int)(type));                                                                      //�I�u�W�F�N�gX���Ƃ̃^�C�v�ԍ���ݒ�
			pDraiya->SetSize();                                                                                              //X�I�u�W�F�N�g�̃T�C�Y��ݒ肷��
			pDraiya->CObjectX::SetRotType(CObjectX::ROTTYPE::ROTTYPE_NORMAL);                                                //�����̎�ނ�ݒ�
			pDraiya->SetManagerType((int)(CStageManager::MANAGEROBJECT_BOSS));                                               //�}�l�[�W���[�ŌĂяo�����̎�ނ�ݒ�
			pDraiya->AdjustSize();      //����T�C�Y�������s��
		}
		else
		{//�I�u�W�F�N�g�ɋ󂫂��Ȃ������̂Ŕj������
			delete pDraiya;
			pDraiya = nullptr;
			return nullptr;
		}
	}
	return pDraiya;
}
//=====================================================================================================================================

//==================================================================
//�ړ��p�^�[�����s����
//==================================================================
void CDraiya::MovePattern()
{
	int& nMovePatternFrame = GetMovePatternFrame();
	int& nMaxMovePatternFrame = GetMaxMovePatternFrame();
	bool& bUseMovePattern = GetUseMovePattern();//�ړ��p�^�[�����g�p���Ă��邩�ǂ���
	int& nMovePatternNum = GetMovePatternNum();//�ړ��p�^�[���ԍ����擾����
	D3DXVECTOR3& Move = GetMove();
	D3DXVECTOR3 Pos = GetPos();

	CBoss::MovePattern();//�ړ��p�^�[���̋��ʏ���

	DecideMovePattern();//�ړ��p�^�[�����菈��

	//�ړ��p�^�[�����J�n����
	switch (nMovePatternNum)
	{
	case 0:
		MovePattern0();//�ړ��p�^�[���O
		break;
	default:
		break;
	}

	//�ړ��p�^�[�����I��点��
	if (nMovePatternFrame > nMaxMovePatternFrame)
	{
		bUseMovePattern = false;//�A�N�V�����p�^�[���ֈڍs
		nMovePatternFrame = 0;
		Move = NULL_VECTOR3;
	}

}
//=====================================================================================================================================

//==================================================================
//�ړ��p�^�[���O
//==================================================================
void CDraiya::MovePattern0()
{
	D3DXVECTOR3& Move = GetMove();
	D3DXVECTOR3& Pos = GetPos();
	D3DXVECTOR3 PlayerPos = CGame::GetPlayer()->GetPos();
	int nMovePatternFrame = GetMovePatternFrame();
	float fLength = CCalculation::CalculationLength(Pos, PlayerPos);
	if (nMovePatternFrame > 15)
	{
		if (fLength < 200.0f)
		{//�������P�Q�O���߂���΁i�߂��̂ŉ�������j
			if (Pos.x < PlayerPos.x)
			{//�v���C���[���E���ɂ�����E�E�E
				Move.x = -2.0f;
			}
			else
			{//�v���C���[�������ɂ�����E�E�E
				Move.x = 2.0f;
			}
		}
		else
		{//�������P�Q�O��艓����΁i�����̂ŋ߂Â��j
			if (Pos.x < PlayerPos.x)
			{//�v���C���[���E���ɂ�����E�E�E
				Move.x = 2.0f;
			}
			else
			{//�v���C���[�������ɂ�����E�E�E
				Move.x = -2.0f;
			}
		}

		if (fLength < 210.0f && fLength > 190.0f)
		{
			Move.x = 0.0f;
		}

		if (CGame::GetPlayer()->GetPos().y > 5.0f && GetLanding() == true)
		{
			Move.y = 20.0f;
		}
	}

	if (nMovePatternFrame == m_nMAX_MOVEPATTERN_0_FRAME)
	{
		SetUndoInertia();//���������ɖ߂�
		nMovePatternFrame = 0;
	}

}
//=====================================================================================================================================

//==================================================================
//�ړ��p�^�[�����菈��
//==================================================================
void CDraiya::DecideMovePattern()
{
	int& nMovePatternFrame = GetMovePatternFrame();
	int& nMaxMovePatternFrame = GetMaxMovePatternFrame();
	bool& bUseMovePattern = GetUseMovePattern();//�ړ��p�^�[�����g�p���Ă��邩�ǂ���
	int& nMovePatternNum = GetMovePatternNum();//�ړ��p�^�[���ԍ����擾����
	D3DXVECTOR3 Pos = GetPos();
	D3DXVECTOR3 PlayerPos = CGame::GetPlayer()->GetPos();

	if (nMovePatternFrame == 1)
	{//�ŏ��Ɉړ����@�����߂�

		nMovePatternNum = 0;

		//���ꂼ��̈ړ��p�^�[���̍ő�t���[������ݒ肷��
		switch (nMovePatternNum)
		{
		case 0:
			nMaxMovePatternFrame = m_nMAX_MOVEPATTERN_0_FRAME;
			break;
		default:
			break;
		}
	}

}
//=====================================================================================================================================

//==================================================================
//�T�C�Y��������
//==================================================================
void CDraiya::AdjustSize()
{

}
//=====================================================================================================================================

//==================================================================
//�s���p�^�[�����s����
//==================================================================
void CDraiya::ActionPattern()
{
	bool& bAction = GetAction();    //�s�����Ă��邩�ǂ���
	int& nSkill = GetSkill();       //�X�L��
	bool& bInsanity = GetInsanity();//�������Ă��邩�ǂ���
	int nLife = GetLife();          //�̗�
	int& nCntTurn = GetTurnCnt();   //�^�[�������J�E���g
	int nMaxLife = GetMaxLife();    //�ő�̗�
	float fRatioLife = 0.0f;        //�̗͂̊���

	fRatioLife = float(nLife) / float(nMaxLife);

	if (fRatioLife < 0.5f)
	{//�̗͂��T�O������������甭����Ԃɂ���
		bInsanity = true;
	}

	if (bAction == false)
	{
		if (bInsanity == false)
		{//������Ԃ���Ȃ����
			nSkill = rand() % 4;
			//nSkill = 4;
		}
		else
		{//������ԂȂ�
			nCntTurn++;//��������^�[�������J�E���g
			nSkill = rand() % 5;
			if (nCntTurn == 1)
			{
				nSkill = 4;
			}
		}
		bAction = true;
	}

	if (bAction == true)
	{
		CBoss::ActionPattern();
		switch (nSkill)
		{
		case 0:
			Action0();
			break;
		case 1:
			Action1();
			break;
		case 2:
			Action2();
			break;
		case 3:
			Action3();
			break;
		case 4:
			Action4();
			break;
		default:
			break;
		}
	}

}
//=====================================================================================================================================

//==================================================================
//�s���p�^�[���O
//==================================================================
void CDraiya::Action0()
{
	int& nPatternTime = GetPatternTime();         //�p�^�[�����Ԃ��擾����
	bool& bAction = GetAction();                  //�s�����Ă��邩�ǂ������擾
	int& nPattern = GetPattern();                 //���݂̃p�^�[�����擾
	int& nActionNum = GetActionNum();             //�A�N�V�����񐔂��J�E���g
	int& nSubPattern = GetSubPattern();           //�T�u�p�^�[�����擾
	D3DXVECTOR3& Pos = GetPos();                  //�ʒu���擾
	D3DXVECTOR3& Rot = GetRot();                  //�������擾
	D3DXVECTOR3& Move = GetMove();                //�ړ��ʂ��擾
	D3DXVECTOR3 PlayerPos = CGame::GetPlayer()->GetPos();//�v���C���[�̈ʒu

	D3DXVECTOR3& PurposePos = GetPurposePos();//�ړI�n���g�p
	bool& bIsLanding = GetLanding();
	bool& bIsWalling = GetWalling();
	float fRandRot = 0.0f;
	CDraiyaAttack* pRileyAttack = nullptr;

	switch (nPattern)
	{
	case 0://���߂�
		if (nPatternTime == 30)
		{//�ԐF�`���[�W
			CParticle::SummonChargeParticle(CParticle::TYPE00_NORMAL, 100, 120, 5.0f, 30.0f, 30.0f, 300, 100, 10, true,
				Pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), true);
		}

		if (nPatternTime == 90)
		{
			SetUseGravity(false);//�d�͂��g�p���Ȃ�
			SetUseInteria(false);//�������g�p���Ȃ�
			nPattern++;
			nPatternTime = 0;
		}
		break;
	case 1://�ړI�n�܂ňړ�
		if (nPatternTime == 20)
		{
			PurposePos = PlayerPos + D3DXVECTOR3(0.0f, 200.0f, 0.0f);

			if (PurposePos.x + GetVtxMin().x < -BOSSSTAGE_HALFWIDTH)
			{
				PurposePos.x = -BOSSSTAGE_HALFWIDTH - GetVtxMin().x;
			}

			if (PurposePos.x + GetVtxMax().x > BOSSSTAGE_HALFWIDTH)
			{
				PurposePos.x = BOSSSTAGE_HALFWIDTH - GetVtxMax().x;
			}

			fRandRot = CCalculation::CalculationXYaim(Pos,PurposePos);
			Move.x = sinf(fRandRot) * 20.0f;
			Move.y = cosf(fRandRot) * 20.0f;
			nPattern++;
			nPatternTime = 0;
		}
		break;
	case 2://�ړI�n�ɂ��ǂ蒅��
		if (Pos.x > PurposePos.x - 20.0f && Pos.x < PurposePos.x + 20.0f &&
			Pos.y > PurposePos.y - 20.0f && Pos.y < PurposePos.y + 20.0f)
		{
			Move = NULL_VECTOR3;
			SetUseGravity(true);
			nPattern++;
			nPatternTime = 0;
		}
		break;
	case 3://�n�ʂɒ�������U������
		if (bIsLanding == true)
		{
			if (nPatternTime % 10 == 0)
			{
				nActionNum++;
				if (nActionNum == 1)
				{
					CUi3D::Create(CUi3D::UI3DTYPE01_WARNING, 60, 30.0f, 30.0f, Pos, NULL_VECTOR3);
					CDraiyaAttack::Create(CAttack::ATTACKTYPE06_THUNDER, 10, 250, Pos + D3DXVECTOR3(0.0f, 200.0f, 0.0f), D3DXVECTOR3(0.0f, -m_fACTION0_ATTACKSPEED, 0.0f),
						D3DXVECTOR3(1.0f, 5.0f, 1.0f), NULL_VECTOR3, CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, D3DXVECTOR3(0.0f, 0.6f, 0.0f), false, 20);
				}
				if (nActionNum <= 5 && nActionNum > 1)
				{
					CUi3D::Create(CUi3D::UI3DTYPE01_WARNING, 60, 30.0f, 30.0f, Pos + D3DXVECTOR3(40.0f * nActionNum, 30.0f, 0.0f), NULL_VECTOR3);
					CDraiyaAttack::Create(CAttack::ATTACKTYPE06_THUNDER, 10, 250, Pos + D3DXVECTOR3(40.0f * nActionNum, 200.0f, 0.0f), D3DXVECTOR3(0.0f, -m_fACTION0_ATTACKSPEED, 0.0f),
						D3DXVECTOR3(1.0f, 5.0f, 1.0f), NULL_VECTOR3, CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, D3DXVECTOR3(0.0f, 0.6f, 0.0f), false, 20);

					CUi3D::Create(CUi3D::UI3DTYPE01_WARNING, 60, 30.0f, 30.0f, Pos + D3DXVECTOR3(-40.0f * nActionNum, 30.0f, 0.0f), NULL_VECTOR3);
					CDraiyaAttack::Create(CAttack::ATTACKTYPE06_THUNDER, 10, 250, Pos + D3DXVECTOR3(-40.0f * nActionNum, 200.0f, 0.0f), D3DXVECTOR3(0.0f, -m_fACTION0_ATTACKSPEED, 0.0f),
						D3DXVECTOR3(1.0f, 5.0f, 1.0f), NULL_VECTOR3, CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, D3DXVECTOR3(0.0f, 0.6f, 0.0f), false, 20);

				}

				if (nActionNum == 5)
				{
					nActionNum = 0;
					nPattern++;
					nPatternTime = 0;
				}
			}
		}
		break;
	case 4:
		if (nPatternTime % 7 == 0)
		{
			nActionNum++;

			CUi3D::Create(CUi3D::UI3DTYPE01_WARNING, 60, 30.0f, 30.0f, Pos + D3DXVECTOR3(-700.0f + 40.0f * nActionNum, 30.0f, 0.0f), NULL_VECTOR3);
			CDraiyaAttack::Create(CAttack::ATTACKTYPE06_THUNDER, 10, 250, Pos + D3DXVECTOR3(-700.0f + 40.0f * nActionNum, 200.0f, 0.0f), D3DXVECTOR3(0.0f, -10.0f, 0.0f),
				D3DXVECTOR3(1.0f, 5.0f, 1.0f), NULL_VECTOR3, CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, D3DXVECTOR3(0.0f, 0.6f, 0.0f), false, 20);

			CUi3D::Create(CUi3D::UI3DTYPE01_WARNING, 60, 30.0f, 30.0f, Pos + D3DXVECTOR3(700.0f - 40.0f * nActionNum, 30.0f, 0.0f), NULL_VECTOR3);
			CDraiyaAttack::Create(CAttack::ATTACKTYPE06_THUNDER, 10, 250, Pos + D3DXVECTOR3(700.0f - 40.0f * nActionNum, 200.0f, 0.0f), D3DXVECTOR3(0.0f, -10.0f, 0.0f),
				D3DXVECTOR3(1.0f, 5.0f, 1.0f), NULL_VECTOR3, CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, D3DXVECTOR3(0.0f, 0.6f, 0.0f), false, 20);

			if (nActionNum == 12)
			{
				ResetPattern();
				SetUseInteria(true);//�������g�p����
			}
		}
		break;
	default:
		break;
	}
}
//=====================================================================================================================================

//==================================================================
//�s���p�^�[���P
//==================================================================
void CDraiya::Action1()
{
	int& nPatternTime = GetPatternTime();         //�p�^�[�����Ԃ��擾����
	bool& bAction = GetAction();                  //�s�����Ă��邩�ǂ������擾
	int& nPattern = GetPattern();                 //���݂̃p�^�[�����擾
	int& nActionNum = GetActionNum();             //�A�N�V�����񐔂��J�E���g
	int& nSubPattern = GetSubPattern();           //�T�u�p�^�[�����擾
	D3DXVECTOR3& Pos = GetPos();                  //�ʒu���擾
	D3DXVECTOR3& Rot = GetRot();                  //�������擾
	D3DXVECTOR3& Move = GetMove();                //�ړ��ʂ��擾
	D3DXVECTOR3 PlayerPos = CGame::GetPlayer()->GetPos();//�v���C���[�̈ʒu

	CBullet* pBullet = nullptr;//�e�ւ̃|�C���^
	D3DXVECTOR3& PurposePos = GetPurposePos();//�ړI�n���g�p
	bool& bIsLanding = GetLanding();
	bool& bIsWalling = GetWalling();
	float fRandRot = 0.0f;
	float& fSaveRot = GetSaveRot();//�ۑ�����������擾����
	CDraiyaAttack* pRileyAttack = nullptr;

	switch (nPattern)
	{
	case 0://���߂�
		if (nPatternTime == 30)
		{//�F�`���[�W
			CParticle::SummonChargeParticle(CParticle::TYPE00_NORMAL, 100, 120, 5.0f, 30.0f, 30.0f, 300, 100, 10,false,
				Pos, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), true);
		}

		if (nPatternTime == 90)
		{
			SetUseInteria(false);//�������g�p���Ȃ�
			nPattern++;
			nPatternTime = 0;
		}
		break;
	case 1:
		if (Pos.x < PlayerPos.x)
		{
			SetUseAddRot(true, D3DXVECTOR3(0.0f, 0.0f, 0.2f));
		}
		else
		{
			SetUseAddRot(true, D3DXVECTOR3(0.0f, 0.0f, -0.2f));
		}
		fRandRot = CCalculation::CalculationParabola(CCalculation::CalculationLength(Pos, PlayerPos), GetNormalGravity(), m_fACTION1_PARABORAJUMPSPEED, Pos, PlayerPos);

		Move.x = sinf(fRandRot) * m_fACTION1_PARABORAJUMPSPEED;
		Move.y = cosf(fRandRot) * m_fACTION1_PARABORAJUMPSPEED;

		nPattern++;
		nPatternTime = 0;
		break;
	case 2:
		CParticle::Create(CParticle::TYPE03_THUNDER, 30, 100.0f, 100.0f, GetSenterPos(), NULL_VECTOR3, NORMAL_COL, true);
		if (nPatternTime % 4 == 0)
		{
			pBullet = CBullet::Create(CBullet::BULLETTYPE_ENEMY, CBullet::BULLETTEX_ENEMY, 120, 10, 40.0f, 40.0f, Pos, D3DXVECTOR3(sinf(GetRot().z) * 1.0f, cosf(GetRot().z) * 1.0f, 0.0f));
			pBullet->SetUseAddSpeed(true, true, 1.05f);
			pBullet->SetUseEffect(10, D3DXVECTOR2(60.0f, 60.0f), D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
		}
		if (bIsLanding == true)
		{
			nPattern++;
			nPatternTime = 0;
			Move = NULL_VECTOR3;
			Rot = NULL_VECTOR3;
			SetUseAddRot(false, NULL_VECTOR3);
		}
		break;
	case 3:
		CParticle::Create(CParticle::TYPE03_THUNDER, 30, 100.0f, 100.0f, GetSenterPos(), NULL_VECTOR3, NORMAL_COL, true);

		if (nPatternTime == 1)
		{
			Move.y = 30.0f;
		}

		if (nPatternTime >= 10)
		{
			if (Move.y < 0.0f)
			{
				nPatternTime = 0;
				nPattern++;
			}
		}
		break;
	case 4:
		if (nPatternTime == 1)
		{
			SetUseGravity(false);
			fSaveRot = CCalculation::CalculationXYaim(Pos, PlayerPos);
			Move.x = sinf(fSaveRot) * 15.0f;
			Move.y = cosf(fSaveRot) * 15.0f;
		}

		if (nPatternTime % 12 == 0)
		{
			pBullet = CBullet::Create(CBullet::BULLETTYPE_ENEMY, CBullet::BULLETTEX_ENEMY, 120, 10, 40.0f, 40.0f, Pos, D3DXVECTOR3(sinf(fSaveRot + 1.0f) * 1.0f, cosf(fSaveRot + 1.0f) * 1.0f, 0.0f));
			pBullet->SetUseAddSpeed(true, true, 1.1f);
			pBullet->SetUseEffect(10, D3DXVECTOR2(60.0f, 60.0f), D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));

			pBullet = CBullet::Create(CBullet::BULLETTYPE_ENEMY, CBullet::BULLETTEX_ENEMY, 120, 10, 40.0f, 40.0f, Pos, D3DXVECTOR3(sinf(fSaveRot - 1.0f) * 1.0f, cosf(fSaveRot - 1.0f) * 1.0f, 0.0f));
			pBullet->SetUseAddSpeed(true, true, 1.1f);
			pBullet->SetUseEffect(10, D3DXVECTOR2(60.0f, 60.0f), D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
		}

		if (Pos.y + GetVtxMin().y <= 0.0f)
		{
			Move = NULL_VECTOR3;
			SetUseGravity(true);
			SetUseInteria(true);
			ResetPattern();
		}
		break;
	default:
		break;
	}

}
//=====================================================================================================================================

//==================================================================
//�s���p�^�[���Q�i�r�Ń��[�U�[�j
//==================================================================
void CDraiya::Action2()
{
	int& nPatternTime = GetPatternTime();         //�p�^�[�����Ԃ��擾����
	bool& bAction = GetAction();                  //�s�����Ă��邩�ǂ������擾
	int& nPattern = GetPattern();                 //���݂̃p�^�[�����擾
	int& nActionNum = GetActionNum();             //�A�N�V�����񐔂��J�E���g
	int& nSubPattern = GetSubPattern();           //�T�u�p�^�[�����擾
	D3DXVECTOR3& Pos = GetPos();                  //�ʒu���擾
	D3DXVECTOR3& Rot = GetRot();                  //�������擾
	D3DXVECTOR3& Move = GetMove();                //�ړ��ʂ��擾
	D3DXVECTOR3 PlayerPos = CGame::GetPlayer()->GetPos();//�v���C���[�̈ʒu

	int nRand = 0;//�����p
	CBullet* pBullet = nullptr;//�e�ւ̃|�C���^
	D3DXVECTOR3& PurposePos = GetPurposePos();//�ړI�n���g�p
	bool& bIsLanding = GetLanding();
	bool& bIsWalling = GetWalling();
	float fRandRot = 0.0f;
	float& fSaveRot = GetSaveRot();//�ۑ�����������擾����
	CDraiyaAttack* pRileyAttack = nullptr;

	switch (nPattern)
	{
	case 0://���߂�
		if (nPatternTime == 30)
		{//�ΐF�`���[�W
			CParticle::SummonChargeParticle(CParticle::TYPE00_NORMAL, 100, 120, 5.0f, 30.0f, 30.0f, 300, 100, 10, false,
				Pos, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), true);
		}

		if (nPatternTime == 90)
		{
			SetUseInteria(false);//�������g�p���Ȃ�
			SetUseGravity(false);//�d�͂��g�p���Ȃ�
			nPattern++;
			nPatternTime = 0;
		}
		break;
	case 1:
		fRandRot = CCalculation::CalculationXYaim(Pos, NULL_VECTOR3);
		Move.x = sinf(fRandRot) * 10.0f;
		Move.y = cosf(fRandRot) * 10.0f;

		if (Pos.x > -20.0f && Pos.x < 20.0f)
		{
			Move = NULL_VECTOR3;
			nPattern++;
			nPatternTime = 0;
		}
		break;
	case 2:
		if (nPatternTime % 30 == 0 && nActionNum < m_nMAX_ACTION2THUNDERNUM)
		{
			nRand = rand() % 2;

			if (nActionNum < 0 || nActionNum >= m_nMAX_ACTION2THUNDERNUM)
			{
				assert("�h���C���̋Z�Q���z��O�A�N�Z�X");
			}

			if (nRand == 0)
			{
				m_bAnnihilationThunderShot[nActionNum] = false;
				CParticle::SummonParticle(CParticle::TYPE00_NORMAL, 120, 50, 50.0f, 50.0f, 200, 10, false, Pos, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), true);
				CUi3D::Create(CUi3D::UI3DTYPE01_WARNING, 15, 100.0f, 100.0f, D3DXVECTOR3(-BOSSSTAGE_HALFWIDTH * 0.5f, 200.0f, 0.0f), NULL_VECTOR3);
			}
			else
			{
				m_bAnnihilationThunderShot[nActionNum] = true;
				CUi3D::Create(CUi3D::UI3DTYPE01_WARNING, 15, 100.0f, 100.0f, D3DXVECTOR3(BOSSSTAGE_HALFWIDTH * 0.5f, 200.0f, 0.0f), NULL_VECTOR3);
				CParticle::SummonParticle(CParticle::TYPE00_NORMAL, 120, 50, 50.0f, 50.0f, 200, 10, false, Pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), true);
			}

			nActionNum++;
		}

		if (nActionNum == m_nMAX_ACTION2THUNDERNUM)
		{
			nPattern++;
			nActionNum = 0;
			nPatternTime = 0;
		}
		break;
	case 3:

		if (nPatternTime % m_nMAX_ACTION2THUNDER_SPAN == 0)
		{
			m_pAnnihilationThunder = CDraiyaAttack::Create(CAttack::ATTACKTYPE06_THUNDER, 10, 20,NULL_VECTOR3, NULL_VECTOR3, D3DXVECTOR3(2.0f, 10.0f, 2.0f), NULL_VECTOR3,
				CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, D3DXVECTOR3(0.0f, 0.5f, 0.0f), false, 60);
			m_pAnnihilationThunder->SetUseDeath(false);
			m_pAnnihilationThunder->SetUseAttackEffect(true, 20, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			m_pAnnihilationThunder->SetParentInfo(Pos, NULL_VECTOR3, ONE_VECTOR3, true);
		}

		if (m_pAnnihilationThunder != nullptr)
		{
			D3DXVECTOR3& ParentRot = m_pAnnihilationThunder->GetParentRot();

			if (nActionNum < 0 || nActionNum >= m_nMAX_ACTION2THUNDERNUM)
			{
				assert("�h���C���̋Z�Q���z��O�A�N�Z�X");
			}
			if (m_bAnnihilationThunderShot[nActionNum] == false)
			{
				ParentRot.z += 0.1f;
			}
			else
			{
				ParentRot.z += -0.1f;
			}

			if (m_pAnnihilationThunder->GetLife() < 2)
			{
				m_pAnnihilationThunder->SetUseDeath(true);
				m_pAnnihilationThunder->SetDeath();
				m_pAnnihilationThunder = nullptr;
				nActionNum++;
				nPatternTime = 0;
			}
		}

		if (nActionNum == m_nMAX_ACTION2THUNDERNUM)
		{
			ResetPattern();
			SetUseInteria(true);//���������ɖ߂�
			SetUseGravity(true);//�d�͂����ɖ߂�
		}
		break;
	default:
		break;
	}

}
//=====================================================================================================================================

//==================================================================
//�s���p�^�[���R�i�M���̑̓�����j
//==================================================================
void CDraiya::Action3()
{
	int& nPatternTime = GetPatternTime();         //�p�^�[�����Ԃ��擾����
	bool& bAction = GetAction();                  //�s�����Ă��邩�ǂ������擾
	int& nPattern = GetPattern();                 //���݂̃p�^�[�����擾
	int& nActionNum = GetActionNum();             //�A�N�V�����񐔂��J�E���g
	int& nSubPattern = GetSubPattern();           //�T�u�p�^�[�����擾
	D3DXVECTOR3& Pos = GetPos();                  //�ʒu���擾
	D3DXVECTOR3& Rot = GetRot();                  //�������擾
	D3DXVECTOR3& Move = GetMove();                //�ړ��ʂ��擾
	D3DXVECTOR3 PlayerPos = CGame::GetPlayer()->GetPos();//�v���C���[�̈ʒu

	int nRand = 0;//�����p
	CBullet* pBullet = nullptr;//�e�ւ̃|�C���^
	D3DXVECTOR3& PurposePos = GetPurposePos();//�ړI�n���g�p
	bool& bIsLanding = GetLanding();
	bool& bIsWalling = GetWalling();
	float fRandRot = 0.0f;
	float& fSaveRot = GetSaveRot();//�ۑ�����������擾����
	CDraiyaAttack* pDraiyaAttack = nullptr;
	float fRandPosY = 0.0f;//�����ʒuY
	float fRandPosX = 0.0f;//�����ʒuX

	switch (nPattern)
	{
	case 0://���߂�
		if (nPatternTime == 30)
		{//���F�`���[�W
			CParticle::SummonChargeParticle(CParticle::TYPE00_NORMAL, 100, 120, 5.0f, 30.0f, 30.0f, 300, 100, 10, false,
				Pos, D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), true);
		}

		if (nPatternTime == 90)
		{
			SetUseInteria(false);//�������g�p���Ȃ�
			SetUseGravity(false);//�d�͂��g�p���Ȃ�
			nPattern++;
			nPatternTime = 0;
		}
		break;
	case 1:
		if (nPatternTime % 30 == 0 && nActionNum < m_nMAX_ACTION03_RAMMING_NUM)
		{
			if (nActionNum < 4)
			{//������

				if (nActionNum < 0 || nActionNum >= m_nMAX_ACTION03_RAMMING_NUM)
				{
					assert("�h���C���̋Z�R���z��O�A�N�Z�X");
				}
				m_fSaveRammingRot[nActionNum] = float(rand() % 40 - 20) / 100 + D3DX_PI * -0.5f;
				fRandPosY = float(rand() % int(BOSSSTAGE_HEIGHT));
				m_RammingSavePos[nActionNum] = D3DXVECTOR3(BOSSSTAGE_HALFWIDTH, fRandPosY, 0.0f);

				for (int nCnt = 0; nCnt < 20; nCnt++)
				{
					CUi3D::Create(CUi3D::UI3DTYPE01_WARNING, 40, 30.0f, 30.0f,
						m_RammingSavePos[nActionNum] + D3DXVECTOR3(sinf(m_fSaveRammingRot[nActionNum]) * (60.0f * nCnt), cosf(m_fSaveRammingRot[nActionNum]) * (60.0f * nCnt), 0.0f), NULL_VECTOR3);
				}
			}
			else
			{//�ォ��
				if (nActionNum < 0 || nActionNum >= m_nMAX_ACTION03_RAMMING_NUM)
				{
					assert("�h���C���̋Z�R���z��O�A�N�Z�X");
				}
				m_fSaveRammingRot[nActionNum] = float(rand() % 40 - 20) / 100 + D3DX_PI * 1.0f;
				fRandPosX = float(rand() % int(BOSSSTAGE_WIDTH)) - BOSSSTAGE_HALFWIDTH;
				m_RammingSavePos[nActionNum] = D3DXVECTOR3(fRandPosX, BOSSSTAGE_HEIGHT, 0.0f);
				for (int nCnt = 0; nCnt < 20; nCnt++)
				{
					CUi3D::Create(CUi3D::UI3DTYPE01_WARNING, 40, 30.0f, 30.0f,
						m_RammingSavePos[nActionNum] + D3DXVECTOR3(sinf(m_fSaveRammingRot[nActionNum]) * (60.0f * nCnt), cosf(m_fSaveRammingRot[nActionNum]) * (60.0f * nCnt), 0.0f), NULL_VECTOR3);
				}
			}
			nActionNum++;
		}

		if (nActionNum == m_nMAX_ACTION03_RAMMING_NUM)
		{
			nPatternTime = 0;
			nPattern++;
			nActionNum = 0;
		}
		break;
	case 2:
		if (nPatternTime < 50)
		{
			CParticle::SummonChargeParticle(CParticle::TYPE00_NORMAL, 10, 60, 5.0f, 30.0f, 30.0f, 300, 100, 10, false,
				Pos, D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), true);
		}
		else
		{
			nPattern++;
			nPatternTime = 0;
		}
		break;
	case 3:
		if (nPatternTime == 1)
		{
			CParticle::SummonParticle(CParticle::TYPE00_NORMAL, 120, 50, 50.0f, 50.0f, 200, 10, false,GetSenterPos(),D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), true);
			Pos = m_RammingSavePos[nActionNum];
		}
		CParticle::Create(CParticle::TYPE03_THUNDER, 30, 100.0f, 100.0f, GetSenterPos(), NULL_VECTOR3, NORMAL_COL, true);
		Move.x = sinf(m_fSaveRammingRot[nActionNum]) * 20.0f;
		Move.y = cosf(m_fSaveRammingRot[nActionNum]) * 20.0f;

		if (nActionNum < m_nMAX_ACTION03_RAMMING_NUM)
		{
			if (nActionNum < 4)
			{
				if (bIsWalling == true)
				{
					nActionNum++;
					Pos = m_RammingSavePos[nActionNum];
				}
			}
			else
			{
				if (bIsLanding == true)
				{
 					nActionNum++;
					if (nActionNum < 0 || nActionNum >= m_nMAX_ACTION03_RAMMING_NUM)
					{
						assert("�h���C���̋Z�R���z��O�A�N�Z�X");
					}
					Pos = m_RammingSavePos[nActionNum];
					pDraiyaAttack = CDraiyaAttack::Create(CAttack::ATTACKTYPE11_EXPLOSION, 20, 120, GetSenterPos(), NULL_VECTOR3, ONE_VECTOR3, NULL_VECTOR3, CAttack::COLLISIONTYPE03_BALL,
						ONE_VECTOR3 * 0.1f, false, 20);

					CManager::GetCamera()->SetShake(20, 0.2f);
					pDraiyaAttack->SetUseRatioLifeAlpha(true);
					pDraiyaAttack->SetUseAddScale(ONE_VECTOR3 * 0.1f, true);
				}
			}
		}

		if (nActionNum == m_nMAX_ACTION03_RAMMING_NUM)
		{//�Ōゾ���s������
			for (int nCnt = 0; nCnt < 7; nCnt++)
			{//�o���b�g����
				pBullet = CBullet::Create(CBullet::BULLETTYPE_ENEMY, CBullet::BULLETTEX_ENEMY, 120, 10, 30.0f, 30.0f, GetSenterPos(), NULL_VECTOR3);
				fRandRot = (2.0f / 7.0f) * nCnt;
				pBullet->SetUseLengthCurve(true, D3DX_PI* fRandRot, 0.02f, 5.0f);
				pBullet->SetUseEffect(40,D3DXVECTOR2(30.0f,30.0f),NORMAL_COL);
			}
			for (int nCnt = 0; nCnt < 7; nCnt++)
			{//�o���b�g����
				pBullet = CBullet::Create(CBullet::BULLETTYPE_ENEMY, CBullet::BULLETTEX_ENEMY, 120, 10, 30.0f, 30.0f, GetSenterPos(), NULL_VECTOR3);
				fRandRot = (2.0f / 7.0f) * nCnt;
				pBullet->SetUseLengthCurve(true, D3DX_PI * fRandRot, -0.02f, 5.0f);
				pBullet->SetUseEffect(40, D3DXVECTOR2(30.0f, 30.0f), NORMAL_COL);
			}

			Pos = GetPosOld();
			Move = NULL_VECTOR3;
			ResetPattern();
			SetUseGravity(true);
			SetUseInteria(true);
		}
		break;
	default:
		break;
	}

}
//=====================================================================================================================================

//==================================================================
//�s���p�^�[���S�i�K�E�Z�j
//==================================================================
void CDraiya::Action4()
{
	int& nPatternTime = GetPatternTime();         //�p�^�[�����Ԃ��擾����
	bool& bAction = GetAction();                  //�s�����Ă��邩�ǂ������擾
	int& nPattern = GetPattern();                 //���݂̃p�^�[�����擾
	int& nActionNum = GetActionNum();             //�A�N�V�����񐔂��J�E���g
	int& nSubPattern = GetSubPattern();           //�T�u�p�^�[�����擾
	D3DXVECTOR3& Pos = GetPos();                  //�ʒu���擾
	D3DXVECTOR3& Rot = GetRot();                  //�������擾
	D3DXVECTOR3& Move = GetMove();                //�ړ��ʂ��擾
	D3DXVECTOR3 PlayerPos = CGame::GetPlayer()->GetPos();//�v���C���[�̈ʒu
	D3DXVECTOR3 PlayerSenterPos = CGame::GetPlayer()->GetSenterPos();
	int nRand = 0;//�����p
	CBullet* pBullet = nullptr;//�e�ւ̃|�C���^
	D3DXVECTOR3& PurposePos = GetPurposePos();//�ړI�n���g�p
	bool& bIsLanding = GetLanding();
	bool& bIsWalling = GetWalling();
	float fRandRot = 0.0f;
	float& fSaveRot = GetSaveRot();//�ۑ�����������擾����
	CDraiyaAttack* pDraiyaAttack = nullptr;
	CUi3D* pUi3D = nullptr;
	float fRandPosY = 0.0f;//�����ʒuY
	float fRandPosX = 0.0f;//�����ʒuX
	switch (nPattern)
	{
	case 0://���߂�
		if (nPatternTime == 30)
		{//���F�`���[�W
			CParticle::SummonChargeParticle(CParticle::TYPE00_NORMAL, 100, 120, 5.0f, 30.0f, 30.0f, 300, 100, 10, true,
				Pos, D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), true);
		}

		if (nPatternTime == 90)
		{
			SetUseInteria(false);//�������g�p���Ȃ�
			SetUseGravity(false);//�d�͂��g�p���Ȃ�
			nPattern++;
			nPatternTime = 0;
		}
		break;
	case 1:
		if (nPatternTime == 1)
		{
			fRandRot = CCalculation::CalculationXYaim(Pos, NULL_VECTOR3);
			Move.x = sinf(fRandRot) * 10.0f;
			Move.y = cosf(fRandRot) * 10.0f;
		}

		if (Pos.x >= -20.0f && Pos.x <= 20.0f &&
			Pos.y >= -20.0f && Pos.y <= 20.0f)
		{
			Pos = NULL_VECTOR3;
			Move = NULL_VECTOR3;
			nPattern++;
			nPatternTime = 0;
		}
		break;
	case 2:
		Move.z = 10.0f;

		if (Pos.z > m_fACTION4_BACKMOCELENGTH - 20.0f && Pos.z < m_fACTION4_BACKMOCELENGTH + 20.0f)
		{
			Move = NULL_VECTOR3;
			nPattern++;
			nPatternTime = 0;
		}
		break;
	case 3:
		if (nPatternTime % m_nACTION4_THUNDERSUMMONSPAN == 0 && nPatternTime >= m_nACTION4_THUNDERSUMMONSPAN)
		{
			CUi3D::Create(CUi3D::UI3DTYPE01_WARNING, 120, 30.0f, 30.0f, PlayerSenterPos + D3DXVECTOR3(sinf(D3DX_PI * 0.25f) * 300.0f, cosf(D3DX_PI * 0.25f) * 300.0f, 0.0f),
				D3DXVECTOR3(sinf(D3DX_PI * 1.25f) * 30.0f, cosf(D3DX_PI * 1.25f) * 30.0f, 0.0f));
			CUi3D::Create(CUi3D::UI3DTYPE01_WARNING, 120, 30.0f, 30.0f, PlayerSenterPos + D3DXVECTOR3(sinf(D3DX_PI * -0.25f) * 300.0f, cosf(D3DX_PI * -0.25f) * 300.0f, 0.0f),
				D3DXVECTOR3(sinf(D3DX_PI * -1.25f) * 30.0f, cosf(D3DX_PI * -1.25f) * 30.0f, 0.0f));

			pDraiyaAttack = CDraiyaAttack::Create(CAttack::ATTACKTYPE06_THUNDER, 10, 120,
				NULL_VECTOR3,
				D3DXVECTOR3(0.0f,-m_fACTION4_THUNDERSPEED,0.0f),
				D3DXVECTOR3(1.0f, m_fACTION4_THUNDERSCALE_Y, 1.0f), NULL_VECTOR3,CAttack::COLLISIONTYPE02_TRUERECTANGLEXY,D3DXVECTOR3(0.0f,0.6f,0.0f), false, 30);

			pDraiyaAttack->SetParentInfo(PlayerSenterPos + D3DXVECTOR3(sinf(D3DX_PI * 0.25f) * m_fACTION4_SUMMONTHUNDERLENGTH, cosf(D3DX_PI * 0.25f) * m_fACTION4_SUMMONTHUNDERLENGTH, 0.0f),
				D3DXVECTOR3(0.0f, 0.0f, D3DX_PI * 1.75f), ONE_VECTOR3, true);

			pDraiyaAttack = CDraiyaAttack::Create(CAttack::ATTACKTYPE06_THUNDER, 10, 120,
				NULL_VECTOR3,
				D3DXVECTOR3(0.0f,-m_fACTION4_THUNDERSPEED,0.0f),
				D3DXVECTOR3(1.0f, m_fACTION4_THUNDERSCALE_Y, 1.0f), NULL_VECTOR3,CAttack::COLLISIONTYPE02_TRUERECTANGLEXY,D3DXVECTOR3(0.0f,0.6f,0.0f), false, 30);

			pDraiyaAttack->SetParentInfo(PlayerSenterPos + D3DXVECTOR3(sinf(D3DX_PI * -0.25f) * m_fACTION4_SUMMONTHUNDERLENGTH, cosf(D3DX_PI * -0.25f) * m_fACTION4_SUMMONTHUNDERLENGTH, 0.0f),
				D3DXVECTOR3(0.0f, 0.0f, -D3DX_PI * 1.75f), ONE_VECTOR3, true);
			nActionNum++;
		}

		if (nActionNum == m_nACTION4_THUNDERSHOTNUM)
		{
			nPatternTime = 0;
			nPattern++;
			nActionNum = 0;
		}
		break;
	case 4:
		if (nPatternTime % m_nACTION4_THUNDERSUMMONSPAN == 0)
		{
			CUi3D::Create(CUi3D::UI3DTYPE01_WARNING, 120, 30.0f, 30.0f, PlayerSenterPos + D3DXVECTOR3(sinf(D3DX_PI * 0.5f) * 300.0f, cosf(D3DX_PI * 0.5f) * 300.0f, 0.0f),
				D3DXVECTOR3(sinf(D3DX_PI * 1.5f) * 30.0f, cosf(D3DX_PI * 1.5f) * 30.0f, 0.0f));
			CUi3D::Create(CUi3D::UI3DTYPE01_WARNING, 120, 30.0f, 30.0f, PlayerSenterPos + D3DXVECTOR3(sinf(D3DX_PI * 0.0f) * 300.0f, cosf(D3DX_PI * 0.0f) * 300.0f, 0.0f),
				D3DXVECTOR3(sinf(D3DX_PI * 1.0f) * 30.0f, cosf(D3DX_PI * 1.0f) * 30.0f, 0.0f));

			pDraiyaAttack = CDraiyaAttack::Create(CAttack::ATTACKTYPE06_THUNDER, 10, 120,
				NULL_VECTOR3,
				D3DXVECTOR3(0.0f, -m_fACTION4_THUNDERSPEED, 0.0f),
				D3DXVECTOR3(1.0f, m_fACTION4_THUNDERSCALE_Y, 1.0f), NULL_VECTOR3, CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, D3DXVECTOR3(0.0f, 0.6f, 0.0f), false, 30);

			pDraiyaAttack->SetParentInfo(PlayerSenterPos + D3DXVECTOR3(sinf(D3DX_PI * 0.0f) * m_fACTION4_SUMMONTHUNDERLENGTH, cosf(D3DX_PI * 0.0f) * m_fACTION4_SUMMONTHUNDERLENGTH, 0.0f),
				D3DXVECTOR3(0.0f, 0.0f, D3DX_PI * 0.0f), ONE_VECTOR3, true);

			pDraiyaAttack = CDraiyaAttack::Create(CAttack::ATTACKTYPE06_THUNDER, 10, 120,
				NULL_VECTOR3,
				D3DXVECTOR3(0.0f, -m_fACTION4_THUNDERSPEED, 0.0f),
				D3DXVECTOR3(1.0f, m_fACTION4_THUNDERSCALE_Y, 1.0f), NULL_VECTOR3, CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, D3DXVECTOR3(0.0f, 0.6f, 0.0f), false, 30);

			pDraiyaAttack->SetParentInfo(PlayerSenterPos + D3DXVECTOR3(sinf(D3DX_PI * 0.5f) * m_fACTION4_SUMMONTHUNDERLENGTH, cosf(D3DX_PI * 0.5f) * m_fACTION4_SUMMONTHUNDERLENGTH, 0.0f),
				D3DXVECTOR3(0.0f, 0.0f, -D3DX_PI * 0.5f), ONE_VECTOR3, true);

			nActionNum++;
		}

		if (nActionNum == m_nACTION4_THUNDERSHOTNUM)
		{
			nActionNum = 0;
			nPatternTime = 0;
			nPattern++;
		}
		break;
	case 5:
		if (nPatternTime % 60 == 0)
		{
			nRand = rand() % 2;

			if (nRand == 0)
			{
				CUi3D::Create(CUi3D::UI3DTYPE01_WARNING, 120, 30.0f, 30.0f, PlayerSenterPos + D3DXVECTOR3(sinf(D3DX_PI * 0.25f) * 300.0f, cosf(D3DX_PI * 0.25f) * 300.0f, 0.0f),
					D3DXVECTOR3(sinf(D3DX_PI * 1.25f) * 30.0f, cosf(D3DX_PI * 1.25f) * 30.0f, 0.0f));
				CUi3D::Create(CUi3D::UI3DTYPE01_WARNING, 120, 30.0f, 30.0f, PlayerSenterPos + D3DXVECTOR3(sinf(D3DX_PI * -0.25f) * 300.0f, cosf(D3DX_PI * -0.25f) * 300.0f, 0.0f),
					D3DXVECTOR3(sinf(D3DX_PI * -1.25f) * 30.0f, cosf(D3DX_PI * -1.25f) * 30.0f, 0.0f));

				pDraiyaAttack = CDraiyaAttack::Create(CAttack::ATTACKTYPE06_THUNDER, 10, 120,
					NULL_VECTOR3,
					D3DXVECTOR3(0.0f, -m_fACTION4_THUNDERSPEED, 0.0f),
					D3DXVECTOR3(1.0f, m_fACTION4_THUNDERSCALE_Y, 1.0f), NULL_VECTOR3, CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, D3DXVECTOR3(0.0f, 0.6f, 0.0f), false, 30);

				pDraiyaAttack->SetParentInfo(PlayerSenterPos + D3DXVECTOR3(sinf(D3DX_PI * 0.25f) * m_fACTION4_SUMMONTHUNDERLENGTH, cosf(D3DX_PI * 0.25f) * m_fACTION4_SUMMONTHUNDERLENGTH, 0.0f),
					D3DXVECTOR3(0.0f, 0.0f, D3DX_PI * 1.75f), ONE_VECTOR3, true);

				pDraiyaAttack = CDraiyaAttack::Create(CAttack::ATTACKTYPE06_THUNDER, 10, 120,
					NULL_VECTOR3,
					D3DXVECTOR3(0.0f, -m_fACTION4_THUNDERSPEED, 0.0f),
					D3DXVECTOR3(1.0f, m_fACTION4_THUNDERSCALE_Y, 1.0f), NULL_VECTOR3, CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, D3DXVECTOR3(0.0f, 0.6f, 0.0f), false, 30);

				pDraiyaAttack->SetParentInfo(PlayerSenterPos + D3DXVECTOR3(sinf(D3DX_PI * -0.25f) * m_fACTION4_SUMMONTHUNDERLENGTH, cosf(D3DX_PI * -0.25f) * m_fACTION4_SUMMONTHUNDERLENGTH, 0.0f),
					D3DXVECTOR3(0.0f, 0.0f, -D3DX_PI * 1.75f), ONE_VECTOR3, true);
				nActionNum++;
			}
			else
			{
				CUi3D::Create(CUi3D::UI3DTYPE01_WARNING, 120, 30.0f, 30.0f, PlayerSenterPos + D3DXVECTOR3(sinf(D3DX_PI * 0.5f) * 300.0f, cosf(D3DX_PI * 0.5f) * 300.0f, 0.0f),
					D3DXVECTOR3(sinf(D3DX_PI * 1.5f) * 30.0f, cosf(D3DX_PI * 1.5f) * 30.0f, 0.0f));
				CUi3D::Create(CUi3D::UI3DTYPE01_WARNING, 120, 30.0f, 30.0f, PlayerSenterPos + D3DXVECTOR3(sinf(D3DX_PI * 0.0f) * 300.0f, cosf(D3DX_PI * 0.0f) * 300.0f, 0.0f),
					D3DXVECTOR3(sinf(D3DX_PI * 1.0f) * 30.0f, cosf(D3DX_PI * 1.0f) * 30.0f, 0.0f));

				pDraiyaAttack = CDraiyaAttack::Create(CAttack::ATTACKTYPE06_THUNDER, 10, 120,
					NULL_VECTOR3,
					D3DXVECTOR3(0.0f, -m_fACTION4_THUNDERSPEED, 0.0f),
					D3DXVECTOR3(1.0f, m_fACTION4_THUNDERSCALE_Y, 1.0f), NULL_VECTOR3, CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, D3DXVECTOR3(0.0f, 0.6f, 0.0f), false, 30);

				pDraiyaAttack->SetParentInfo(PlayerSenterPos + D3DXVECTOR3(sinf(D3DX_PI * 0.0f) * m_fACTION4_SUMMONTHUNDERLENGTH, cosf(D3DX_PI * 0.0f) * m_fACTION4_SUMMONTHUNDERLENGTH, 0.0f),
					D3DXVECTOR3(0.0f, 0.0f, D3DX_PI * 0.0f), ONE_VECTOR3, true);

				pDraiyaAttack = CDraiyaAttack::Create(CAttack::ATTACKTYPE06_THUNDER, 10, 120,
					NULL_VECTOR3,
					D3DXVECTOR3(0.0f, -m_fACTION4_THUNDERSPEED, 0.0f),
					D3DXVECTOR3(1.0f, m_fACTION4_THUNDERSCALE_Y, 1.0f), NULL_VECTOR3, CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, D3DXVECTOR3(0.0f, 0.6f, 0.0f), false, 30);

				pDraiyaAttack->SetParentInfo(PlayerSenterPos + D3DXVECTOR3(sinf(D3DX_PI * 0.5f) * m_fACTION4_SUMMONTHUNDERLENGTH, cosf(D3DX_PI * 0.5f) * m_fACTION4_SUMMONTHUNDERLENGTH, 0.0f),
					D3DXVECTOR3(0.0f, 0.0f, -D3DX_PI * 0.5f), ONE_VECTOR3, true);

				nActionNum++;
			}
		 
			if (nActionNum == m_nACTION4_THUNDERSHOTNUM * 3)
			{
				nPattern++;
				nPatternTime = 0;
				nActionNum = 0;
			}
		}
		break;
	case 6:
		if (nPatternTime == 1)
		{

			for (int nCnt = 0; nCnt < (nActionNum + 1) * 2; nCnt++)
			{
				if (nCnt < 0 || nCnt >= m_nNUMSPECIALATTACK)
				{
					assert("�h���C���̋Z�S���z��O�A�N�Z�X");
				}
				float fAddRot = float(rand() % 100 - 50) / 200;
				m_pSpecialThunder[nCnt] = CDraiyaAttack::Create(CAttack::ATTACKTYPE19_SENTERTHUNDER, 10, 9999, GetSenterPos(), NULL_VECTOR3,
					D3DXVECTOR3(2.0f, 10.0f, 2.0f), NULL_VECTOR3, CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, D3DXVECTOR3(0.0f, 0.0f, fAddRot), false, 10);
			}
		}

		for (int nCnt = 0; nCnt < (nActionNum + 1) * 2; nCnt++)
		{
			float& fAddRotThunder = m_pSpecialThunder[nCnt]->GetAddRot().z;
			fAddRotThunder *= 0.98f;
		}

		if (nPatternTime == 120)
		{
			for (int nCnt = 0; nCnt < (nActionNum + 1) * 2; nCnt++)
			{
				if (nCnt < 0 || nCnt >= m_nNUMSPECIALATTACK)
				{
					assert("�h���C���̋Z�S���z��O�A�N�Z�X");
				}
				m_pSpecialThunder[nCnt]->SetMove(D3DXVECTOR3(0.0f, 0.0f, -30.0f));
			}
		}

		if (nPatternTime == 180)
		{
			for (int nCnt = 0; nCnt < m_nNUMSPECIALATTACK; nCnt++)
			{
				if (m_pSpecialThunder[nCnt] != nullptr)
				{
					m_pSpecialThunder[nCnt]->SetUseDeath(true);
					m_pSpecialThunder[nCnt]->SetDeath();
					m_pSpecialThunder[nCnt] = nullptr;
				}
			}

			nActionNum++;
			nPatternTime = 0;

			if (nActionNum == 4)
			{
				nPattern++;
				nPatternTime = 0;
				nActionNum = 0;
			}
		}
		break;
	case 7:
		Move.z = -10.0f;
		if (Pos.z < 20.0f && Pos.z > -20.0f)
		{
			Move = NULL_VECTOR3;
			Pos.z = 0.0f;
			ResetPattern();
			SetUseInteria(true);
			SetUseGravity(true);
		}
		break;
	default:
		break;
	}

}
//=====================================================================================================================================

//============================================<<<�K�u���G���N���X>>>=============================================================================	

//====================================================================
//�R���X�g���N�^
//====================================================================
CGabriel::CGabriel() : m_fMoveLength(0.0f),m_fMoveRot(0.0f),m_bMoveAim(false),
m_SaveMoveFulcrumPos(NULL_VECTOR3),m_pCircleBullet(),m_pSubAiModel(nullptr)
{

}
//=====================================================================================================================================

//====================================================================
//�f�X�g���N�^
//====================================================================
CGabriel::~CGabriel()
{

}
//=====================================================================================================================================

//====================================================================
//����������
//====================================================================
HRESULT CGabriel::Init()
{
	CBoss::Init();

	return S_OK;
}
//=====================================================================================================================================

//====================================================================
//�I������
//====================================================================
void CGabriel::Uninit()
{
	CBoss::Uninit();
}
//======================================================================================================================================

//====================================================================
//�X�V����
//====================================================================
void CGabriel::Update()
{
	CBoss::Update();
}
//======================================================================================================================================

//====================================================================
//�`�揈��
//====================================================================
void CGabriel::Draw()
{
	CBoss::Draw();
}
//=====================================================================================================================================

//====================================================================
//���S�t���O�ݒ菈��
//====================================================================
void CGabriel::SetDeath()
{
	CBoss::SetDeath();

	for (int nCnt = 0; nCnt < m_nMAX_ACTION0BULLETNUM; nCnt++)
	{
		if (m_pCircleBullet[nCnt] != nullptr)
		{
			m_pCircleBullet[nCnt]->SetUseDeath(true);
			m_pCircleBullet[nCnt]->SetDeath();
			m_pCircleBullet[nCnt] = nullptr;
		}
	}

	if (m_pSubAiModel != nullptr)
	{
		m_pSubAiModel->SetUseDeath(true);
		m_pSubAiModel->SetDeath();
		m_pSubAiModel = nullptr;
	}
}
//====================================================================================================================================

//==================================================================
//��������
//==================================================================
CGabriel* CGabriel::Create(BOSSTYPE type, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 Scale)
{
	CGabriel* pGabriel = DBG_NEW CGabriel;   //�{�X�𐶐�
	bool bSuccess = pGabriel->GetCreateSuccess();
	int nIdx = 0;//���f���̃C���f�b�N�X
	if (bSuccess == true)
	{
		if (pGabriel != nullptr)
		{
			pGabriel->Init();                             //����������
			pGabriel->SetUseDeath(true);                  //���S�t���O�𔭓����邩�ǂ�����ݒ肷��
			pGabriel->SetType(type);                      //�{�X�̎��
			pGabriel->SetLife(nLife);                     //�̗�
			pGabriel->SetMaxLife(nLife);//�ő�̗�
			pGabriel->SetAutoSubLife(false);              //�����I�ɑ̗͂����炷���ǂ���
			pGabriel->CObjectX::SetPos(pos);                                                                                  //�I�u�W�F�N�g�Q�c�̈ʒu��ݒ�
			pGabriel->SetPosOld(pos);                                                                                         //1f�O�̈ʒu��ݒ�
			pGabriel->SetSupportPos(pos);                                                                                     //�ݒu�ʒu
			pGabriel->CObject::SetType(CObject::TYPE_BOSS);                                                                   //�I�u�W�F�N�g�̎�ނ����߂�
			pGabriel->CObjectX::SetScale(Scale);                                                                              //�I�u�W�F�N�g�̊g�嗦
			pGabriel->SetFormarScale(Scale);                                                                                  //���̊g�嗦��ݒ肷��
			nIdx = CManager::GetObjectXInfo()->Regist(CObjectX::OBJECTXTYPE_BOSS, m_BOSS_FILENAME[(int)(type)]);
			pGabriel->CObjectX::BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(CObjectX::OBJECTXTYPE_BOSS, nIdx),
				CManager::GetObjectXInfo()->GetBuffMat(CObjectX::OBJECTXTYPE_BOSS, nIdx),
				CManager::GetObjectXInfo()->GetdwNumMat(CObjectX::OBJECTXTYPE_BOSS, nIdx),
				CManager::GetObjectXInfo()->GetTexture(CObjectX::OBJECTXTYPE_BOSS, nIdx),
				CManager::GetObjectXInfo()->GetColorValue(CObjectX::OBJECTXTYPE_BOSS, nIdx));                                   //���f���������蓖�Ă�
			pGabriel->SetBossHpGauge(nLife);                                                                                  //�̗̓Q�[�W��ݒ肷��
			pGabriel->CObjectX::SetObjXType(CObjectX::OBJECTXTYPE_BOSS);                                                      //�I�u�W�F�N�gX�̃^�C�v��ݒ�
			pGabriel->CObjectX::SetTypeNum((int)(type));                                                                      //�I�u�W�F�N�gX���Ƃ̃^�C�v�ԍ���ݒ�
			pGabriel->SetSize();                                                                                              //X�I�u�W�F�N�g�̃T�C�Y��ݒ肷��
			pGabriel->CObjectX::SetRotType(CObjectX::ROTTYPE::ROTTYPE_NORMAL);                                                //�����̎�ނ�ݒ�
			pGabriel->SetManagerType((int)(CStageManager::MANAGEROBJECT_BOSS));                                               //�}�l�[�W���[�ŌĂяo�����̎�ނ�ݒ�
			pGabriel->AdjustSize();      //����T�C�Y�������s��
		}
		else
		{//�I�u�W�F�N�g�ɋ󂫂��Ȃ������̂Ŕj������
			delete pGabriel;
			pGabriel = nullptr;
			return nullptr;
		}
	}
	return pGabriel;
}
//=====================================================================================================================================

//==================================================================
//�ړ��p�^�[��
//==================================================================
void CGabriel::MovePattern()
{
	int& nMovePatternFrame = GetMovePatternFrame();
	int& nMaxMovePatternFrame = GetMaxMovePatternFrame();
	bool& bUseMovePattern = GetUseMovePattern();//�ړ��p�^�[�����g�p���Ă��邩�ǂ���
	int& nMovePatternNum = GetMovePatternNum();//�ړ��p�^�[���ԍ����擾����
	D3DXVECTOR3& Move = GetMove();

	CBoss::MovePattern();//�ړ��p�^�[���̋��ʏ���

	DecideMovePattern();//�ړ��p�^�[�����菈��

	//�ړ��p�^�[�����J�n����
	switch (nMovePatternNum)
	{
	case 0:
		MovePattern0();//�ړ��p�^�[���O
		break;
	default:
		break;
	}

	//�ړ��p�^�[�����I��点��
	if (nMovePatternFrame > nMaxMovePatternFrame)
	{
		bUseMovePattern = false;//�A�N�V�����p�^�[���ֈڍs
		nMovePatternFrame = 0;
		Move = NULL_VECTOR3;
	}

}
//=====================================================================================================================================

//==================================================================
//�ړ��p�^�[���O
//==================================================================
void CGabriel::MovePattern0()
{
	D3DXVECTOR3& Move = GetMove();
	D3DXVECTOR3& Pos = GetPos();
	D3DXVECTOR3 PlayerPos = CGame::GetPlayer()->GetPos();
	int nMovePatternFrame = GetMovePatternFrame();
	float fLength = CCalculation::CalculationLength(Pos, PlayerPos);
	bool bMove = true;
	if (nMovePatternFrame == 15)
	{
		CParticle::SummonChargeParticle(CParticle::TYPE03_THUNDER, 100, 60, 10.0f, 30.0f, 30.0f, 200, 100, 10, false, GetSenterPos(), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), true);
	}

	if (nMovePatternFrame == 30)
	{
		m_fMoveLength = CCalculation::CalculationLength(Pos, PlayerPos);
		if (Pos.x < PlayerPos.x)
		{//�E�ɂ���
			m_bMoveAim = true;
			m_fMoveRot = -D3DX_PI * 0.5f;
		}
		else
		{//���ɂ���
			m_bMoveAim = false;
			m_fMoveRot = D3DX_PI * 0.5f;
		}

		m_SaveMoveFulcrumPos = PlayerPos;
	}

	if (nMovePatternFrame > 40 && Pos.z >= -20.0f && Pos.z <= 20.0f)
	{
		bMove = false;
	}

	if (nMovePatternFrame > 30 && bMove == true)
	{
		if (m_bMoveAim == true)
		{
			m_fMoveRot += 0.05f;
		}
		else
		{
			m_fMoveRot -= 0.05f;
		}

		Pos.x = m_SaveMoveFulcrumPos.x + sinf(m_fMoveRot) * m_fMoveLength;
		Pos.z = m_SaveMoveFulcrumPos.z + cosf(m_fMoveRot) * m_fMoveLength;

		if (Pos.x + GetVtxMin().x < -BOSSSTAGE_HALFWIDTH)
		{
			Pos.x = -BOSSSTAGE_HALFWIDTH - GetVtxMin().x;
		}
		else if (Pos.x + GetVtxMax().x > BOSSSTAGE_HALFWIDTH)
		{
			Pos.x = BOSSSTAGE_HALFWIDTH - GetVtxMax().x;
		}
	}

	if (nMovePatternFrame == m_nMAX_MOVEPATTERN0_FRAME)
	{
		SetUndoInertia();//���������ɖ߂�
		m_fMoveRot = 0.0f;
		m_bMoveAim = false;
		m_SaveMoveFulcrumPos = NULL_VECTOR3;
		m_fMoveLength = 0.0f;
		nMovePatternFrame = 0;
	}

}
//=====================================================================================================================================

//==================================================================
//�ړ��p�^�[�����菈��
//==================================================================
void CGabriel::DecideMovePattern()
{
	int& nMovePatternFrame = GetMovePatternFrame();
	int& nMaxMovePatternFrame = GetMaxMovePatternFrame();
	bool& bUseMovePattern = GetUseMovePattern();//�ړ��p�^�[�����g�p���Ă��邩�ǂ���
	int& nMovePatternNum = GetMovePatternNum();//�ړ��p�^�[���ԍ����擾����
	D3DXVECTOR3 Pos = GetPos();
	D3DXVECTOR3 PlayerPos = CGame::GetPlayer()->GetPos();

	//���������
	float fLength = CCalculation::CalculationLength(Pos, PlayerPos);

	if (nMovePatternFrame == 1)
	{//�ŏ��Ɉړ����@�����߂�

		nMovePatternNum = 0;
		//���ꂼ��̈ړ��p�^�[���̍ő�t���[������ݒ肷��
		switch (nMovePatternNum)
		{
		case 0:
			nMaxMovePatternFrame = m_nMAX_MOVEPATTERN0_FRAME;
			break;
		default:
			break;
		}
	}
}
//=====================================================================================================================================

//==================================================================
//�T�C�Y��������
//==================================================================
void CGabriel::AdjustSize()
{
	D3DXVECTOR3& VtxMax = GetOriginVtxMax();
	D3DXVECTOR3& VtxMin = GetOriginVtxMin();

	VtxMax.y -= 25.0f;

	VtxMax.x -= 20.0f;
	VtxMin.x += 20.0f;

}
//=====================================================================================================================================

//==================================================================
//�s���p�^�[��
//==================================================================
void CGabriel::ActionPattern()
{
	bool& bAction = GetAction();    //�s�����Ă��邩�ǂ���
	int& nSkill = GetSkill();       //�X�L��
	bool& bInsanity = GetInsanity();//�������Ă��邩�ǂ���
	int nLife = GetLife();          //�̗�
	int& nCntTurn = GetTurnCnt();   //�^�[�������J�E���g
	int nMaxLife = GetMaxLife();    //�ő�̗�
	float fRatioLife = 0.0f;        //�̗͂̊���

	fRatioLife = float(nLife) / float(nMaxLife);

	if (fRatioLife < 0.5f)
	{//�̗͂��T�O������������甭����Ԃɂ���
		bInsanity = true;
	}

	if (bAction == false)
	{
		if (bInsanity == false)
		{//������Ԃ���Ȃ����
			nSkill = rand() % 1;
			//nSkill = 4;
		}
		else
		{//������ԂȂ�
			nCntTurn++;//��������^�[�������J�E���g
			nSkill = rand() % 1;
			if (nCntTurn == 1)
			{
				nSkill = 0;
			}
		}
		bAction = true;
	}

	if (bAction == true)
	{
		CBoss::ActionPattern();
		switch (nSkill)
		{
		case 0:
			Action0();
			break;
		default:
			break;
		}
	}

}
//=====================================================================================================================================

//==================================================================
//�s���p�^�[���O
//==================================================================
void CGabriel::Action0()
{
	int& nPatternTime = GetPatternTime();         //�p�^�[�����Ԃ��擾����
	bool& bAction = GetAction();                  //�s�����Ă��邩�ǂ������擾
	int& nPattern = GetPattern();                 //���݂̃p�^�[�����擾
	int& nActionNum = GetActionNum();             //�A�N�V�����񐔂��J�E���g
	int& nSubPattern = GetSubPattern();           //�T�u�p�^�[�����擾
	D3DXVECTOR3& Pos = GetPos();                  //�ʒu���擾
	D3DXVECTOR3 SenterPos = GetSenterPos();       //���S�_���擾
	D3DXVECTOR3& Rot = GetRot();                  //�������擾
	D3DXVECTOR3& Move = GetMove();                //�ړ��ʂ��擾
	D3DXVECTOR3 PlayerPos = CGame::GetPlayer()->GetPos();//�v���C���[�̈ʒu
	D3DXVECTOR3 PlayerSenterPos = CGame::GetPlayer()->GetSenterPos();//�v���C���[�̒��S�̈ʒu
	D3DXVECTOR3& PurposePos = GetPurposePos();//�ړI�n���g�p
	D3DXVECTOR3 AiRot = NULL_VECTOR3;

	bool& bIsLanding = GetLanding();
	bool bUseGravity = GetUseGravity();
	bool& bIsWalling = GetWalling();
	float fRandRot = 0.0f;
	float fRandRot2 = 0.0f;
	CGabrielAttack* pGabrielAttack = nullptr;

	switch (nPattern)
	{
	case 0://���߂�
		if (nPatternTime == 30)
		{//�ԐF�`���[�W
			CParticle::SummonChargeParticle(CParticle::TYPE00_NORMAL, 100, 120, 5.0f, 30.0f, 30.0f, 300, 100, 10, true,
				Pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), true);
		}

		if (nPatternTime == 60)
		{
			SetUseGravity(false);//�d�͂��g�p���Ȃ�
			nPattern++;
			nPatternTime = 0;
			Move = NULL_VECTOR3;
		}
		break;
	case 1:
		if (nPatternTime == 1)
		{
			m_fMoveLength = 200.0f;
			m_pSubAiModel = CAIModel::Create(CAIModel::AIMODELTYPE_BALL, PlayerPos, NULL_VECTOR3, ONE_VECTOR3);
			for (int nCnt = 0; nCnt < m_nMAX_ACTION0BULLETNUM; nCnt++)
			{
				if (m_pCircleBullet[nCnt] == nullptr)
				{
					fRandRot = (2.0f / m_nMAX_ACTION0BULLETNUM) * nCnt;
					m_pCircleBullet[nCnt] = CBullet::Create(CBullet::BULLETTYPE_ENEMY, CBullet::BULLETTEX_ENEMY, 9999, 10, 30.0f, 30.0f,
						D3DXVECTOR3(sinf(D3DX_PI * fRandRot) * m_fMoveLength, cosf(D3DX_PI * fRandRot) * m_fMoveLength, 0.0f), NULL_VECTOR3);
					m_pCircleBullet[nCnt]->SetUseDeath(false);
					m_pCircleBullet[nCnt]->SetUseMtxChild(&m_pSubAiModel->GetMatrixWorld(), true);
				}
			}

			nPatternTime = 0;
			nPattern++;
		}
		break;
	case 2:

		//============================================
		//�e�̓���
		//============================================
		if (nPatternTime == 1)
		{
			m_pSubAiModel->SetUseAddRot(true, D3DXVECTOR3(0.0f, 0.0f, 0.01f));
			m_pSubAiModel->SetUseDraw(false);
		}
		if (nPatternTime % 30 == 0)
		{
			for (int nCnt = 0; nCnt < m_nMAX_ACTION0BULLETNUM; nCnt++)
			{
				fRandRot = CCalculation::CalculationRandVecXY();
				if (m_pCircleBullet[nCnt]->GetPos().y > 0.0f)
				{
					CBullet::Create(CBullet::BULLETTYPE_ENEMY, CBullet::BULLETTEX_ENEMY, 200, 10, 10.0f, 10.0f, m_pCircleBullet[nCnt]->GetMtxTransPos(),
						D3DXVECTOR3(sinf(fRandRot) * 1.0f, cosf(fRandRot) * 1.0f, 0.0f));
				}
			}
		}

		if (nPatternTime >= 400)
		{
			m_pSubAiModel->SetUseAddRot(true, NULL_VECTOR3);
			for (int nCnt = 0; nCnt < m_nMAX_ACTION0BULLETNUM; nCnt++)
			{
				fRandRot = (2.0f / m_nMAX_ACTION0BULLETNUM) * nCnt;
				m_pCircleBullet[nCnt]->SetPos(D3DXVECTOR3(sinf(D3DX_PI * fRandRot) * m_fMoveLength, cosf(D3DX_PI * fRandRot) * m_fMoveLength, 0.0f));

			}
			m_fMoveLength -= 8.0f;
		}
		else
		{
			m_pSubAiModel->SetPos(PlayerSenterPos);
		}

		if (nPatternTime == 500)
		{
			nPattern++;
			nPatternTime = 0;

			for (int nCnt = 0; nCnt < m_nMAX_ACTION0BULLETNUM; nCnt++)
			{
				m_pCircleBullet[nCnt]->SetUseDeath(true);
				m_pCircleBullet[nCnt]->SetDeath();
				m_pCircleBullet[nCnt] = nullptr;
			}
			m_pSubAiModel->SetUseDeath(true);
			m_pSubAiModel->SetDeath();
			m_pSubAiModel = nullptr;

			m_fMoveLength = 0.0f;

			SetUseGravity(true);
			SetUseInteria(true);

			ResetPattern();
		}
		//=======================================================================================================

		//=====================================
		//�{�X�̓���
		//=====================================

		if (nPatternTime % 50 == 0)
		{
			CParticle::SummonChargeParticle(CParticle::TYPE00_NORMAL, 100, 60, 10, 30.0f, 30.0f, 200, 100, 10, false, GetSenterPos(), NORMAL_COL, true);
			
			fRandRot2 = CCalculation::CalculationXYaim(SenterPos, PlayerSenterPos);
			pGabrielAttack = CGabrielAttack::Create(CAttack::ATTACKTYPE01_METEO, 20, 180,
				SenterPos, D3DXVECTOR3(sinf(fRandRot2) * 1.0f, cosf(fRandRot2) * 1.0f, 0.0f), ONE_VECTOR3 * 2, D3DXVECTOR3(0.0f, 0.0f, fRandRot2 - D3DX_PI * 0.5f),
				CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, NULL_VECTOR3, true, 100);
			pGabrielAttack->SetUseMultiSpeed(true, ONE_VECTOR3 * 1.05f);
			pGabrielAttack->SetUseAttackEffect(true, 5, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
			pGabrielAttack->SetUseAddRot(true, ONE_VECTOR3 * 0.1f);
				

			fRandRot2 = float(rand() % 314 - 157) / 100;
			float fRandLength = float(rand() % 200 + 200);
			Pos = PlayerSenterPos + D3DXVECTOR3(sinf(fRandRot2) * fRandLength, cosf(fRandRot2) * fRandLength, 0.0f);
			if (Pos.x + GetVtxMin().x < -BOSSSTAGE_HALFWIDTH)
			{
				Pos.x = -BOSSSTAGE_HALFWIDTH - GetVtxMin().x;
			}
			if (Pos.x + GetVtxMax().x > BOSSSTAGE_HALFWIDTH)
			{
				Pos.x = BOSSSTAGE_HALFWIDTH - GetVtxMax().x;
			}

			if (Pos.y < 0.0f)
			{
				Pos.y = 0.0f;
			}
			CParticle::SummonParticle(CParticle::TYPE00_NORMAL, 100, 120, 30.0f, 30.0f, 200, 10, false, Pos, NORMAL_COL, true);
		}
		break;
	default:
		break;
	}
}
//=====================================================================================================================================

//==================================================================
//�s���p�^�[���P
//==================================================================
void CGabriel::Action1()
{
	int& nPatternTime = GetPatternTime();         //�p�^�[�����Ԃ��擾����
	bool& bAction = GetAction();                  //�s�����Ă��邩�ǂ������擾
	int& nPattern = GetPattern();                 //���݂̃p�^�[�����擾
	int& nActionNum = GetActionNum();             //�A�N�V�����񐔂��J�E���g
	int& nSubPattern = GetSubPattern();           //�T�u�p�^�[�����擾
	D3DXVECTOR3& Pos = GetPos();                  //�ʒu���擾
	D3DXVECTOR3 SenterPos = GetSenterPos();       //���S�_���擾
	D3DXVECTOR3& Rot = GetRot();                  //�������擾
	D3DXVECTOR3& Move = GetMove();                //�ړ��ʂ��擾
	D3DXVECTOR3 PlayerPos = CGame::GetPlayer()->GetPos();//�v���C���[�̈ʒu
	D3DXVECTOR3 PlayerSenterPos = CGame::GetPlayer()->GetSenterPos();//�v���C���[�̒��S�̈ʒu
	D3DXVECTOR3& PurposePos = GetPurposePos();//�ړI�n���g�p
	D3DXVECTOR3 AiRot = NULL_VECTOR3;
	bool& bIsLanding = GetLanding();
	bool bUseGravity = GetUseGravity();
	bool& bIsWalling = GetWalling();
	float fRandRot = 0.0f;
	float fRandRot2 = 0.0f;
	CGabrielAttack* pGabrielAttack = nullptr;


	switch (nPattern)
	{
	case 0://���߂�
		if (nPatternTime == 30)
		{//�ԐF�`���[�W
			CParticle::SummonChargeParticle(CParticle::TYPE00_NORMAL, 100, 120, 5.0f, 30.0f, 30.0f, 300, 100, 10, true,
				Pos, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), true);
		}

		if (nPatternTime == 60)
		{
			SetUseGravity(false);//�d�͂��g�p���Ȃ�
			nPattern++;
			nPatternTime = 0;
			Move = NULL_VECTOR3;
		}
		break;
	case 1://�g���l�[�h���e�I

		break;
	default:
		break;
	}
}
//=====================================================================================================================================