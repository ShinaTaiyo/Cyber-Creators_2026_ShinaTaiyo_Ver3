//=======================================================================================================================================================================================================
//
// �{�X�̏���
// Author : Shina Taiyo
//
//=======================================================================================================================================================================================================
#include "boss.h"
#include <stdio.h>
#include "bossgauge.h"
#include "sound.h"
#include "model.h"
#include "debugproc.h"
#include "attackmodel.h"
#include "bullet.h"
#include "battery.h"
#include "particle.h"
#include"attackbillboard.h"
#include "timer.h"
#include "mission.h"
#include "field.h"
#include "shadow.h"
#include "camera.h"
#include "Rader.h"
#include "emitter.h"
#include "enemy.h"
#include "Warning.h"
//=======================================================================================================================================================================================================
// �O���[�o���ϐ�
//=======================================================================================================================================================================================================
BossKey g_aBossKey[MAX_BOSSPARTS];							//�e�L�[���
BossKeySet g_aBossKeySet[MAX_BOSSKEYSET][MAX_BOSSMOTION];	//�e�L�[�Z�b�g���
BossMotionSet g_aBossMotionSet[MAX_BOSSMOTION];				//���[�V�������
BossParts g_aBossParts[MAX_BOSSPARTS];						//�e�p�[�c���
Boss g_aBoss;												//�\���̂̏��
int g_nldxShadowBoss = -1;									//�Ώۂ̉e�̃C���f�b�N�X�i�ԍ��j
int g_nDelayBoss;     
bool g_bPurposeFlag[MAX_PURPOSEFLAG];						//�ėp�t���O
int g_nPurposeCnt;//�ėp�J�E���g

#define GRASPBOSS_SIZE (100.0f)
LPDIRECT3DTEXTURE9 g_pTextureGraspBoss = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGraspBoss = NULL;
GraspBoss g_aGraspBoss;


//=======================================================================================================================================================================================================
// ����������
//=======================================================================================================================================================================================================
void InitBoss(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾
	D3DXMATERIAL* pMat;							//�}�e���A���̃f�[�^�ւ̃|�C���^

	//�\���̂̏��
	g_aBoss.bUse = false;//�g�p���Ă��邩�ǂ���
	g_aBoss.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�ʒu
	g_aBoss.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//1F�O�̈ʒu
	g_aBoss.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�ړ���
	g_aBoss.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//����
	g_aBoss.nIdxShadow = -1;
	g_aBoss.vtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//���ꂼ��̍��W�̃��f���̈ʒu�̍ŏ��l
	g_aBoss.vtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//���ꂼ��̍��W�̃��f���̈ʒu�̍ő�l
	g_aBoss.Size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�G�̑傫��
	g_aBoss.bIsLanding = false;//�n�ʂɂ��Ă��邩�ǂ���
	g_aBoss.fVaimY1 = 0.0f;//��������Y�����̃x�N�g��
	g_aBoss.fVXaim = 0.0f;
	g_aBoss.fVYaim = 0.0f;
	g_aBoss.fVZaim = 0.0f;
	g_aBoss.fVaim = 0.0f;
	g_aBoss.fLength = 0.0f;//�������Ƃ�
	g_aBoss.nAttackTiming = 0;//�U������̃^�C�~���O�����߂�
	g_aBoss.bHitFlag = false;//�U�����q�b�g�������ǂ��������߂�ϐ�
	g_aBoss.fMoveStartLength = 0.0f;//�G���ړ����J�n���鋗��
	g_aBoss.fAttackStartLength = 0.0f;//�G���U�����J�n���鋗��
	g_aBoss.bUnderAttack = false;//�G���U�������ǂ��������߂�
	g_aBoss.state = BOSSSTATE_NEUTRAL;//�{�X�̏��
	g_aBoss.nCntAttackTime = 0;//�U�������Ă��鎞�Ԃ𑪂�
	g_aBoss.nLotteryAttack = 0;//�ǂ̍U�������邩���I������
	g_aBoss.nPattern = 0;//�p�^�[��
	g_aBoss.nAttackCoolTime = 0;//�U���̃N�[���^�C��	
	g_aBoss.fSaveAim = 0.0f;//�x�N�g�����Z�[�u����
	g_aBoss.bBossSummon = false;//�{�X�����t���O
	g_aBoss.fRotMove = 0.0f;//�ꕔ�̏����ł��̕ϐ��̒l�̕����Ɉړ�������
	g_aBoss.nNumLandingModel = 0;//�ǂ̃��f���̏�ɏ���Ă��邩
	g_aBoss.nIdxRaderDisplay = -1;//���[�_�[�f�B�X�v���C�̃C���f�b�N�X
	g_aBoss.nHp = 0;//�̗�
	g_aBoss.nCntAppearTime = 0;//�o�����Ă���̎���
	g_aBoss.bBossAppear = false;//�{�X���o�����o�����ǂ���
	g_aBoss.bCurveLaser = false;//���[�U�[���J�[�u��������������߂�
	g_aBoss.nRunAwayLottery = 0;//��m���Ń{�X�𓦂������闐��
	g_aBoss.bRunAwayBoss = false;//�t���O�������Ƀ{�X�𓦂�������t���O
	g_aBoss.nCntSummonEnemy = 0;//�{�X�����������G�̐����J�E���g����
	g_aBoss.nTotalDamage = 0;//�^�����_���[�W���v
	g_aBoss.nDamageCnt = 0;
	g_aBoss.nCntUseAttack = 0;//�U���������񐔂��J�E���g����
	//==================================
	//�R�����x�N�g���Ŏg���ϐ�
	//==================================
	g_aBoss.fVXaim3D = 0.0f;
	g_aBoss.fVYaim3D = 0.0f;
	g_aBoss.fVZaim3D = 0.0f;
	g_aBoss.fVLaim3D = 0.0f;
	g_aBoss.SaveMove3D = NULL_VECTOR3;
	g_nPurposeCnt = 0;//�ėp�J�E���g

	for (int nCntHit = 0; nCntHit < PLAYERATTACK_MAX; nCntHit++)
	{
		g_aBoss.bHitStop[nCntHit] = false;
		g_aBoss.nCoolTime[nCntHit] = 0;
	}

	for (int nCntAttackModel = 0; nCntAttackModel < MAX_ATTACKMODEL; nCntAttackModel++)
	{
		g_aBoss.bHitStopBoomerang[nCntAttackModel] = false;//�u�[�������ɑ΂���q�b�g�X�g�b�v�̔��������
		g_aBoss.nCoolTimeBoomerang[nCntAttackModel] = 0;//�u�[�������ɑ΂���q�b�g�X�g�b�v�̃J�E���g������
	}

	for (int nCntFlag = 0; nCntFlag < MAX_PURPOSEFLAG; nCntFlag++)
	{
		g_bPurposeFlag[nCntFlag] = false;
	}

	g_nDelayBoss = 0;//�f�B���C

	//�p�[�c���̓ǂݍ���
	LoadBossParts();

	//���[�V�������̓ǂݍ���
	LoadBossMotion();

	//�e�p�[�c���f�����
	for (int nCntParts = 0; nCntParts < g_aBoss.nNumParts; nCntParts++)
	{
		g_aBoss.aParts[nCntParts].vtxMax = NULL_VECTOR3;
		g_aBoss.aParts[nCntParts].vtxMin = NULL_VECTOR3;
		g_aBoss.aParts[nCntParts].PartsPos = NULL_VECTOR3;
		//X�t�@�C���̓ǂݍ���
		D3DXLoadMeshFromX(BOSSPARTS_FILENAME[nCntParts], D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_aBoss.aParts[nCntParts].pBuffMat, NULL, &g_aBoss.aParts[nCntParts].dwNumMat, &g_aBoss.aParts[nCntParts].pMesh);

		//�}�e���A���f�[�^�ւ̃|�C���^���擾
		pMat = (D3DXMATERIAL*)g_aBoss.aParts[nCntParts].pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_aBoss.aParts[nCntParts].dwNumMat; nCntMat++)
		{
			g_aBoss.aParts[nCntParts].Diffuse[nCntMat] = pMat[nCntMat].MatD3D.Diffuse;
		}
	}
}

//=====================================
//�{�X�ʒu�c���\���̍X�V
//=====================================
void InitGraspBoss(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�ւ̃|�C���^

	pDevice = GetDevice();

	VERTEX_2D* pVtx;

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\GraspBoss_000.png",
		&g_pTextureGraspBoss);


	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffGraspBoss,
		NULL);

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffGraspBoss->Lock(0, 0, (void**)&pVtx, 0);

	g_aGraspBoss.pos = NULL_VECTOR3;
	g_aGraspBoss.rot = NULL_VECTOR3;
	g_aGraspBoss.bUse = false;
	g_aGraspBoss.fAnimationPattern = 0.0f;
	g_aGraspBoss.fAnimationCnt = 0.0f;

	pVtx[0].pos = NULL_VECTOR3;
	pVtx[1].pos = NULL_VECTOR3;
	pVtx[2].pos = NULL_VECTOR3;
	pVtx[3].pos = NULL_VECTOR3;

	//rhw�̐ݒ�i�����ݒ�j
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[�̐ݒ�i�����ݒ�j
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//�e�N�X�`�����W�̐ݒ�i�����ݒ�j
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffGraspBoss->Unlock();

}

//=======================================================================================================================================================================================================
// �I������
//=======================================================================================================================================================================================================
void UninitBoss(void)
{
	for (int nCntParts = 0; nCntParts < g_aBoss.nNumParts; nCntParts++)
	{
		//���b�V���̔j��
		if (g_aBossParts[nCntParts].pMesh != NULL)
		{//���b�V����񂪑��݂���ꍇ
			g_aBossParts[nCntParts].pMesh->Release();
			g_aBossParts[nCntParts].pMesh = NULL;
		}
		//�}�e���A���̔j��
		if (g_aBossParts[nCntParts].pBuffMat != NULL)
		{//�}�e���A����񂪑��݂���ꍇ
			g_aBossParts[nCntParts].pBuffMat->Release();
			g_aBossParts[nCntParts].pBuffMat = NULL;
		}
	}
}

//==================================
//�{�X�ʒu�c���\���̏I������
//==================================
void UninitGraspBoss(void)
{
	if (g_pTextureGraspBoss != NULL)
	{
		g_pTextureGraspBoss->Release();
		g_pTextureGraspBoss = NULL;
	}

	if (g_pVtxBuffGraspBoss != NULL)
	{
		g_pVtxBuffGraspBoss->Release();
		g_pVtxBuffGraspBoss = NULL;
	}
}

//=======================================================================================================================================================================================================
// �X�V����
//=======================================================================================================================================================================================================
void UpdateBoss(void)
{
	Player* pPlayer = GetPlayer();	//�v���C���[�̏����擾
	Field* pField = GetField();		//�t�B�[���h�̏����擾
	Camera* pCamera = GetCamera();  //�J�����̏����擾
	g_nDelayBoss++;

	if (GetTimer() == BOSS_SAMMON_TIME && !g_aBoss.bBossSummon)
	{//�c�莞�Ԃ�120�b�ɂȂ����ꍇ
		SetBoss(D3DXVECTOR3(0.0f, 0.0f, 1800.0f), 1200);//�ݒ菈��
		g_aBoss.bBossSummon = true;//�{�X�����ς�
		StopSound(SOUND_LABEL_BGM013);//�Q�[��BGM
		PlaySound(SOUND_LABEL_BGM015);//�{�XBGM
		SetGraspBoss();//�{�X�ʒu�c���\����ݒ�
		g_aBoss.state = BOSSSTATE_NEUTRAL;
	}

	if (g_aBoss.bUse && g_aBoss.bBossAppear == false)
	{//�g�p����Ă���ꍇ���A�o�����o���I�t�̏ꍇ
		CaluculateMatrixBoss();
		g_aBoss.nCntAppearTime++;//�o�����Ă���̎���
		g_aBoss.move.y += -2.0f;//�d�͏���

		if (g_aBoss.pos.y <= 0.0f)
		{//Y���W��0�ȉ��ɂȂ�Ȃ��悤�ɒ���
			g_aBoss.pos.y = 0.0f;
			g_aBoss.move.y = 0.0f;
		}

		//===============================================
		//���_���[�W���ƂɃX�N���b�v���h���b�v
		//===============================================
		if (g_aBoss.nDamageCnt > 0)
		{
			g_aBoss.nDamageCnt--;
			g_aBoss.nTotalDamage++;

			if (g_aBoss.nTotalDamage % 5 == 0)
			{

				float fPurposeoPosX = float(rand() % 400 - 200);
				float fPurposeoPosY = float(rand() % 400 - 200);
				float fPurposeoPosZ = float(rand() % 400 - 200);

				float fVXaim3D = g_aBoss.pos.x - (g_aBoss.pos.x + fPurposeoPosX);
				float fVYaim3D = g_aBoss.pos.y - (g_aBoss.pos.y + fPurposeoPosY);
				float fVZaim3D = g_aBoss.pos.z - (g_aBoss.pos.z + fPurposeoPosZ);

				D3DXVECTOR3 BulletMove = NULL_VECTOR3;

				float fVLaim3D = sqrtf((fVXaim3D * fVXaim3D) + (fVYaim3D * fVYaim3D)
					+ (fVZaim3D * fVZaim3D));

				BulletMove.x = fVXaim3D / fVLaim3D * 10.0f;
				BulletMove.y = 0.0f;
				BulletMove.z = fVZaim3D / fVLaim3D * 10.0f;

				int nLottery = rand() % 5;

				SetScrap(nLottery, 1000, SCRAPRADIUSSIZE, SCRAPRADIUSSIZE, D3DXVECTOR3(g_aBoss.pos.x, g_aBoss.pos.y + SCRAPRADIUSSIZE, g_aBoss.pos.z),
					NULL_VECTOR3, BulletMove);
			}

		}

		HitStopBoss();//�q�b�g�X�g�b�v����

		if (pCamera->nMode == CAMERAMODE_REVOLUTION)
		{
			//====================================================
			// �G�ƃv���C���[�̋��������AXZ�̃x�N�g�������߂�
			//====================================================
			float fLength = sqrtf((g_aBoss.pos.x - pPlayer->pos.x) * (g_aBoss.pos.x - pPlayer->pos.x) +
				(g_aBoss.pos.z - pPlayer->pos.z) * (g_aBoss.pos.z - pPlayer->pos.z)) / 2;

			g_aBoss.fVXaim = pPlayer->pos.x - g_aBoss.pos.x;
			g_aBoss.fVYaim = pPlayer->pos.y - g_aBoss.pos.y;
			g_aBoss.fVZaim = pPlayer->pos.z - g_aBoss.pos.z;

			g_aBoss.fVaim = atan2f(g_aBoss.fVXaim, g_aBoss.fVZaim);
			g_aBoss.fVaimY1 = atan2f(g_aBoss.fVYaim, g_aBoss.fVZaim);

			//=======================
			//�ʏ�s��
			//=======================
			if (g_nDelayBoss % 120 == 0)
			{
				g_aBoss.nPattern = rand() % 2;
			}

			if (g_aBoss.nPattern == 0)
			{//�U�������Ă��Ȃ����A������700.0f���߂�������
				if (!g_aBoss.bUnderAttack)
				{
					g_aBoss.rot.y = g_aBoss.fVaim + D3DX_PI;//�v���C���[�̕���������
					g_aBoss.state = BOSSSTATE_MOVE;//�ړ����

					g_aBoss.move.x = sinf(g_aBoss.fVaim) * g_aBoss.fSpeed;
					g_aBoss.move.z = cosf(g_aBoss.fVaim) * g_aBoss.fSpeed;
				}
			}
			else
			{//�͈͊O
				if (!g_aBoss.bUnderAttack)
				{
					g_aBoss.state = BOSSSTATE_NEUTRAL;//�j���[�g�������

					g_aBoss.move.x = 0.0f;
					g_aBoss.move.z = 0.0f;
				}
			}

			//============================================
			//�{�X�̋Z��������
			//============================================
			if (g_aBoss.bUnderAttack == false)
			{
				g_aBoss.nAttackCoolTime++;
			}

			if (g_aBoss.nAttackCoolTime == 230)
			{
				SetWarning(100, 20.0f, 60.0f, g_aBoss.pos);
			}

			if (g_aBoss.nAttackCoolTime == 300)
			{//�N�[���^�C���ɓ����Ă���T�b�o������A���̍U���̒��I���J�n���A�U����Ԃɂ���
				float fRatioHp = float(g_aBoss.nHp) / float(g_aBoss.nMaxHp);
				g_aBoss.nRunAwayLottery = rand() % 10;

				if (fRatioHp >= 0.5f)
				{//�̗͂��T�O���ȏ�Ȃ�
					g_aBoss.nLotteryAttack = rand() % (BOSSATTACK_MAX - 3);
				}
				else
				{//�̗͂��T�O���ȉ��Ȃ�i�����Z���g�p����悤�ɂȂ�j
					g_aBoss.nCntUseAttack++;//�U���������񐔂��J�E���g����

					g_aBoss.nLotteryAttack = rand() % (BOSSATTACK_MAX - 3) + 3;

					if (g_aBoss.nCntUseAttack == 1)
					{
						g_aBoss.nLotteryAttack = 9;
					}
				}
				//g_aBoss.nLotteryAttack = 5;

				g_aBoss.bUnderAttack = true;

				g_aBoss.nAttackCoolTime = 0;//�U�����������̂ŃN�[���^�C�����O��

				if (g_aBoss.nRunAwayLottery >= 0 && g_aBoss.nRunAwayLottery <= 1)
				{
					//g_aBoss.bRunAwayBoss = true;
					for (int nCntEnemy = 0; nCntEnemy < 10; nCntEnemy++)
					{
						float fLengthrand = float(rand() % 500);
						float fRot = float(rand() % 200 + 1) / 100.0f;
						int nType = rand() % 4;
						SetEnemy(D3DXVECTOR3(g_aBoss.pos.x + sinf(D3DX_PI * fRot) * fLengthrand, 0.0f, g_aBoss.pos.z + cosf(D3DX_PI * fRot) * fLengthrand),
							NULL_VECTOR3,
							nType, true);//�G���Z�b�g���������A�֐��̐�ŁA�{�X���������������J�E���g���Ă���
					}
				}
			}

			if (g_aBoss.bUnderAttack == true)
			{
				if (g_aBoss.nLotteryAttack == BOSSATTACK00_ATOMICPRESS)
				{
					BossAttack0_AtomicPress();//�U���O�F�A�g�~�b�N�v���X
				}
				else if (g_aBoss.nLotteryAttack == BOSSATTACK01_FLASHPUNCH)
				{
					BossAttack1_FlashPunch();//�U���P�F�t���b�V���p���`
				}
				else if (g_aBoss.nLotteryAttack == BOSSATTACK02_THRUSTSTUB)
				{
					BossAttack2_ThrustStub();//�U���Q�F�X���X�g�X�^�u
				}
				else if (g_aBoss.nLotteryAttack == BOSSATTACK03_BIRDSTRIKE)
				{
					BossAttack3_BirdStrike();//�U���R�F�o�[�h�X�g���C�N
				}
				else if (g_aBoss.nLotteryAttack == BOSSATTACK04_BOMBRAIN)
				{
					BossAttack4_BombRain();//�U���S�F�{�����C��
				}
				else if (g_aBoss.nLotteryAttack == BOSSATTACK05_QUELLTHEEARTH)
				{
					BossAttack5_QuellTheEarth();//�U���T�F�N�E�F���W�A�[�X
				}
				else if (g_aBoss.nLotteryAttack == BOSSATTACK06_BURNINGCHICKEN)
				{
					BossAttack6_BurningChicken();//�{�X�U���U�F�o�[�j���O�`�L��
				}
				else if (g_aBoss.nLotteryAttack == BOSSATTACK07_SLANDERTORNADO)
				{
					BossAttack7_SlanderTornado();//�U���V�F�X�����_�[�g���l�[�h
				}
				else if (g_aBoss.nLotteryAttack == BOSSATTACK08_STARDUSTMETEO)
				{
					BossAttack8_StardustMeteo();//�U���W�F�X�^�[�_�X�g���e�I
				}
				else if (g_aBoss.nLotteryAttack == BOSSATTACK09_COLLAPSELASER)
				{
					BossAttack9_CollapseLaser();//�U���X�F�R���v�X���[�U�[
				}

				//else
				//{//�{�X���������
				//	RunAwayBoss();//�{�X��������
				//}
			}

			//=================================================
			//�v���C���[�ɐڋ߂���͈͂̒���
			//=================================================
			if (g_aBoss.bUnderAttack == false)
			{
				if (fLength <= 50.0f)
				{
					g_aBoss.move.x = 0.0f;
					g_aBoss.move.z = 0.0f;
				}
			}

			g_aBoss.posOld = g_aBoss.pos;//1F�O�̈ʒu��ۑ�
			g_aBoss.pos += g_aBoss.move;//�ړ��ʂ����Z

			g_aBoss.nNumLandingModel = -1;

			if (!g_aBoss.bUnderAttack)
			{
				g_aBoss.bIsLanding = bCollisionModel(&g_aBoss.pos, &g_aBoss.posOld, g_aBoss.Size, g_aBoss.nNumLandingModel);
			}

			PrintDebugProc("�{�X�̈ʒu�F%f %f %f\n", g_aBoss.pos.x, g_aBoss.pos.y, g_aBoss.pos.z);
			PrintDebugProc("�{�X�̍U�����ԁF%d\n", g_aBoss.nCntAttackTime);

			//=========================================
			//�{�X�̍s���͈͐���
			//=========================================
			if (g_aBoss.pos.x + (g_aBoss.Size.x / 2.0f) > pField->fWidth || g_aBoss.pos.x - (g_aBoss.Size.x / 2.0f) < -pField->fWidth ||
				g_aBoss.pos.z + (g_aBoss.Size.z / 2.0f) > pField->fHeight || g_aBoss.pos.z - (g_aBoss.Size.z / 2.0f) < -pField->fHeight)
			{
				g_aBoss.pos = g_aBoss.posOld;
			}

			//�e�̐ݒ�
			SetPositionShadow(g_aBoss.nIdxShadow, g_aBoss.pos);

			//���[�_�[�f�B�X�v���C�̐ݒ�
			SetPositionRaderDisPlay(g_aBoss.nIdxRaderDisplay, g_aBoss.pos, 0.0f);

			//==============================================
			//�{�X���|���ꂽ��
			//==============================================
			if (g_aBoss.nHp <= 0)
			{
				g_aBoss.bUse = false;
			}

			if (g_aBoss.bUse == false)
			{
				StopSound(SOUND_LABEL_BGM015);
				PlaySound(SOUND_LABEL_BGM013);

				for (int nCntScrap = 0; nCntScrap < 100; nCntScrap++)
				{

					float fRandPos = float(rand() % 100 + 1);
					float fRandRot = float(rand() % 200) / 100;

					SetScrap(4, 300, 30.0f, 30.0f, D3DXVECTOR3(sinf(D3DX_PI * fRandRot) * fRandPos + pPlayer->pos.x, 0.0f, cosf(D3DX_PI * fRandRot) * fRandPos + pPlayer->pos.z), NULL_VECTOR3, NULL_VECTOR3);
				}

				AddScore(5000);
				KillShadow(g_aBoss.nIdxShadow);
				KillRaderDisplay(g_aBoss.nIdxRaderDisplay);
			}
		}
		
		if (g_aBoss.nCntAppearTime == 1)
		{
			g_aBoss.state = BOSSSTATE_NEUTRAL;
		}
	}

	//���[�V��������
	BossMotion();

	PrintDebugProc("�{�X�̃x�N�g���F%f\n", g_aBoss.fVaim);
	PrintDebugProc("�{�X�̌����F%f\n", g_aBoss.rot.y - D3DX_PI);
	PrintDebugProc("���v�_���[�W�F%d\n",g_aBoss.nTotalDamage);

}

void UpdateGraspBoss(void)
{
	VERTEX_2D* pVtx;

	Player* pPlayer = GetPlayer();
	Camera* pCamera = GetCamera();
	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffGraspBoss->Lock(0, 0, (void**)&pVtx, 0);

	if (g_aGraspBoss.bUse == true)
	{
		g_aGraspBoss.pos.x = sinf(pPlayer->fVaim - pCamera->rot.y) * 1500.0f + SCREEN_WIDTH / 2.0f;
		g_aGraspBoss.pos.y = cosf(pPlayer->fVaim - pCamera->rot.y + D3DX_PI) * 1500.0f + SCREEN_HEIGHT / 2.0f;

		g_aGraspBoss.pos += g_aGraspBoss.move;

		if (g_aGraspBoss.pos.x <= 0.0f + GRASPBOSS_SIZE / 2.0f)
		{//��
			g_aGraspBoss.pos.x = 0.0f + GRASPBOSS_SIZE / 2.0f;
			g_aGraspBoss.move = NULL_VECTOR3;
			g_aGraspBoss.fAnimationPattern = 0.0f;
		}
		if (g_aGraspBoss.pos.x >= SCREEN_WIDTH - GRASPBOSS_SIZE / 2.0f)
		{//�E
			g_aGraspBoss.pos.x = SCREEN_WIDTH - GRASPBOSS_SIZE / 2.0f;
			g_aGraspBoss.move = NULL_VECTOR3;

			g_aGraspBoss.fAnimationPattern = 2.0f;

		}

		if (g_aGraspBoss.pos.y <= 0.0f + GRASPBOSS_SIZE / 2.0f)
		{//��
			g_aGraspBoss.pos.y = 0.0f + GRASPBOSS_SIZE / 2.0f;
			g_aGraspBoss.move = NULL_VECTOR3;
			g_aGraspBoss.fAnimationPattern = 1.0f;

		}
		if (g_aGraspBoss.pos.y >= SCREEN_HEIGHT - GRASPBOSS_SIZE / 2.0f)
		{//��
			g_aGraspBoss.pos.y = SCREEN_HEIGHT - GRASPBOSS_SIZE / 2.0f;
			g_aGraspBoss.move = NULL_VECTOR3;

			g_aGraspBoss.fAnimationPattern = 3.0f;


		}

		pVtx[0].tex = D3DXVECTOR2(0.0f + 0.25f * g_aGraspBoss.fAnimationPattern, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.25f + 0.25f * g_aGraspBoss.fAnimationPattern, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + 0.25f * g_aGraspBoss.fAnimationPattern, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.25f + 0.25f * g_aGraspBoss.fAnimationPattern,1.0f);

		pVtx[0].pos = D3DXVECTOR3(g_aGraspBoss.pos.x - (GRASPBOSS_SIZE / 2.0f), g_aGraspBoss.pos.y - (GRASPBOSS_SIZE / 2.0f), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aGraspBoss.pos.x + (GRASPBOSS_SIZE / 2.0f), g_aGraspBoss.pos.y - (GRASPBOSS_SIZE / 2.0f), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aGraspBoss.pos.x - (GRASPBOSS_SIZE / 2.0f), g_aGraspBoss.pos.y + (GRASPBOSS_SIZE / 2.0f), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aGraspBoss.pos.x + (GRASPBOSS_SIZE / 2.0f), g_aGraspBoss.pos.y + (GRASPBOSS_SIZE / 2.0f), 0.0f);

		if (g_aBoss.bUse == false)
		{
			g_aGraspBoss.bUse = false;
		}

	}

	g_pVtxBuffGraspBoss->Unlock();
}

//=======================================================================================================================================================================================================
// �`�揈��
//=======================================================================================================================================================================================================
void DrawBoss(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�ւ̃|�C���^�Ǝ擾
	D3DXMATRIX mtxRot, mtxTrans, mtxParent;//�v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL* pMat;//�}�e���A���f�[�^�ւ̃|�C���^

	//���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	//�g�p���Ă���ꍇ
	if (g_aBoss.bUse == true)
	{
		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_aBoss.mtxWorld);

		//�����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aBoss.rot.y, g_aBoss.rot.x, g_aBoss.rot.z);
		D3DXMatrixMultiply(&g_aBoss.mtxWorld, &g_aBoss.mtxWorld, &mtxRot);

		//�ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, g_aBoss.pos.x, g_aBoss.pos.y, g_aBoss.pos.z);
		D3DXMatrixMultiply(&g_aBoss.mtxWorld, &g_aBoss.mtxWorld, &mtxTrans);

		for (int nCntParts = 0; nCntParts < g_aBoss.nNumParts; nCntParts++)
		{
			//�e�p�[�c���f���̃��[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aBoss.aParts[nCntParts].mtxWorld);

			//�e�p�[�c���f���̌����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot,
				g_aBoss.aParts[nCntParts].rot.y,
				g_aBoss.aParts[nCntParts].rot.x,
				g_aBoss.aParts[nCntParts].rot.z);

			D3DXMatrixMultiply(&g_aBoss.aParts[nCntParts].mtxWorld, &g_aBoss.aParts[nCntParts].mtxWorld, &mtxRot);

			//�e�p�[�c���f���̈ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans,
				g_aBoss.aParts[nCntParts].pos.x,
				g_aBoss.aParts[nCntParts].pos.y,
				g_aBoss.aParts[nCntParts].pos.z);

			D3DXMatrixMultiply(&g_aBoss.aParts[nCntParts].mtxWorld, &g_aBoss.aParts[nCntParts].mtxWorld, &mtxTrans);

			//�e�ԍ��̕ۑ�
			int nParent = g_aBoss.aParts[nCntParts].nParent;

			//�e�̃}�g���b�N�X�𔽉f
			if (nParent != -1)
			{//�e�̔ԍ������ꍇ
				mtxParent = g_aBoss.aParts[nParent].mtxWorld;//�e���f���̃}�g���b�N�X
			}
			else
			{//�e�̔ԍ��������Ȃ��ꍇ
				mtxParent = g_aBoss.mtxWorld;//�v���C���[�̃}�g���b�N�X
			}

			//�e�̃}�g���b�N�X���|�����킹��
			D3DXMatrixMultiply(&g_aBoss.aParts[nCntParts].mtxWorld, &g_aBoss.aParts[nCntParts].mtxWorld, &mtxParent);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aBoss.aParts[nCntParts].mtxWorld);

			//���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			//�}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_aBoss.aParts[nCntParts].pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aBoss.aParts[nCntParts].dwNumMat; nCntMat++)
			{
				//�}�e���A���̐ݒ�
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//�e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, NULL);

				//���f��(�p�[�c)�̕`��
				g_aBoss.aParts[nCntParts].pMesh->DrawSubset(nCntMat);
			}

			//�ۑ����Ă����}�e���A����߂�
			pDevice->SetMaterial(&matDef);
		}
	}
}

void DrawGraspBoss(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffGraspBoss, 0, sizeof(VERTEX_2D));//���_�o�b�t�@�ւ̃|�C���^�ƒ��_���̍\���̂̃T�C�Y

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (g_aGraspBoss.bUse == true)
	{
		pDevice->SetTexture(0, g_pTextureGraspBoss);
		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);//0�́A�`�悷��ŏ��̒��_�C���f�b�N�X�ŁA2�́A�`�悷��v���~�e�B�u��
	}
}

//=======================================================================================================================================================================================================
// �G�̃p�[�c���̓ǂݍ���
//=======================================================================================================================================================================================================
void LoadBossParts(void)
{
	int nIndex = 0;				//�p�[�c�ԍ�
	int nCntParts = 0;			//�p�[�c��
	char aDataSearch[MAX_TEXT];	//�����񌟍��p

	//�t�@�C�����J��
	FILE* pFile = fopen(BOSSSET, "r");

	//�t�@�C�����J���Ȃ��ꍇ
	if (pFile == NULL)
	{
		return;
	}

	//END_PARTS��������܂œǂݍ��݂��J��Ԃ�
	while (1)
	{
		fscanf(pFile, "%s", aDataSearch);	//����

		//END_PARTS�����������ꍇ
		if (!strcmp(aDataSearch, "END_PARTS"))
		{//�ǂݍ��ݏI��
			fclose(pFile);	//�t�@�C�������
			break;
		}

		//PARTSSET�����������ꍇ
		if (!strcmp(aDataSearch, "PARTSSET"))
		{
			//���ڂ��Ƃ̃f�[�^����
			while (1)
			{
				fscanf(pFile, "%s", aDataSearch);	//����

				//END_PARTSSET�����������ꍇ
				if (!strcmp(aDataSearch, "END_PARTSSET"))
				{//�p�[�c���̓ǂݍ��݂��I��
					g_aBoss.aParts[nIndex].pos = g_aBossParts[nIndex].pos;
					g_aBoss.aParts[nIndex].rot = g_aBossParts[nIndex].rot;
					g_aBoss.aParts[nIndex].nParent = g_aBossParts[nIndex].nParent;
					break;
				}

				//INDEX�����������ꍇ
				else if (!strcmp(aDataSearch, "INDEX"))
				{
					//�ԍ���ۑ�
					fscanf(pFile, "%d", &nIndex);

					//�p�[�c�̔ԍ����p�[�c���𒴂��Ă���ꍇ
					if (nIndex > g_aBoss.nNumParts)
					{//�e�p�[�c���Ƃ̃f�[�^�������I��
						break;
					}
				}

				//PARENT�����������ꍇ
				else if (!strcmp(aDataSearch, "PARENT"))
				{//�e�̔ԍ���ۑ�
					fscanf(pFile, "%d", &g_aBossParts[nIndex].nParent);
				}

				//POS�����������ꍇ
				else if (!strcmp(aDataSearch, "POS"))
				{//�ʒu��ۑ�
					fscanf(pFile, "%f", &g_aBossParts[nIndex].pos.x);//X���W
					fscanf(pFile, "%f", &g_aBossParts[nIndex].pos.y);//Y���W
					fscanf(pFile, "%f", &g_aBossParts[nIndex].pos.z);//Z���W
				}

				//ROT�����������ꍇ
				else if (!strcmp(aDataSearch, "ROT"))
				{//������ۑ�
					fscanf(pFile, "%f", &g_aBossParts[nIndex].rot.x);//X��
					fscanf(pFile, "%f", &g_aBossParts[nIndex].rot.y);//Y��
					fscanf(pFile, "%f", &g_aBossParts[nIndex].rot.z);//Z��
				}
			}

			nCntParts++;					//�p�[�c�������Z
			g_aBoss.nNumParts = nCntParts;	//���v�p�[�c����ۑ�
		}

		//��L�ȊO�����������ꍇ
		else
		{//�X�L�b�v���ēǂݍ��݂𑱂���
			continue;
		}
	}
}

//============================================================================================================================================================================================================
// ���[�V�������̓ǂݍ��ݏ���
//============================================================================================================================================================================================================
void LoadBossMotion(void)
{
	int nMotion = -1;			//���[�V�����̎��
	int nCntParts = 0;			//�p�[�c��
	int nCntKey[MAX_BOSSMOTION];//�e���[�V�����L�[��
	char aDataSearch[MAX_TEXT];	//�f�[�^�����p

	//�e���[�V�����L�[���̏�����
	for (int nCnt = 0; nCnt < MAX_BOSSMOTION; nCnt++)
	{
		nCntKey[nCnt] = 0;
	}

	//�t�@�C�����J��
	FILE* pFile = fopen(BOSSSET, "r");

	//�t�@�C�����J���Ȃ��ꍇ
	if (pFile == NULL)
	{
		return;
	}

	while (1)
	{
		int nResult = fscanf(pFile, "%s", aDataSearch);	//����

		//���[�V�����Z�b�g�����������ꍇ
		if (!strcmp(aDataSearch, "MOTIONSET"))
		{
			//���[�V�����̎�ސ������Z
			nMotion++;

			//END_MOTIONSET��������܂œǂݍ��݂��J��Ԃ�
			while (1)
			{
				fscanf(pFile, "%s", aDataSearch);	//����

				//END_MOTIONSET�����������ꍇ
				if (!strcmp(aDataSearch, "END_MOTIONSET"))
				{//���[�V�����ǂݍ��ݏI��
					break;
				}

				//���[�v���肪���������ꍇ
				else if (!strcmp(aDataSearch, "LOOP"))
				{//���[�v���邩�ǂ������擾
					fscanf(pFile, "%d", &g_aBossMotionSet[nMotion].nLoop);
				}

				//�L�[�Z�b�g�����������ꍇ
				else if (!strcmp(aDataSearch, "KEYSET"))
				{
					while (1)
					{
						fscanf(pFile, "%s", aDataSearch);	//����

						//KEY�����������ꍇ
						if (!strcmp(aDataSearch, "KEY"))
						{
							//���ڂ��Ƃ̃f�[�^����
							while (1)
							{
								fscanf(pFile, "%s", aDataSearch);	//����

								//FRAME�����������ꍇ
								if (!strcmp(aDataSearch, "FRAME"))
								{//�t���[������ǂݍ���
									fscanf(pFile, "%d", &g_aBossKeySet[nCntKey[nMotion]][nMotion].nMaxFrame);
								}

								//POS�����������ꍇ
								else if (!strcmp(aDataSearch, "POS"))
								{//�ʒu
									fscanf(pFile, "%f", &g_aBossKeySet[nCntKey[nMotion]][nMotion].key[nCntParts].pos.x);//X���W
									fscanf(pFile, "%f", &g_aBossKeySet[nCntKey[nMotion]][nMotion].key[nCntParts].pos.y);//Y���W
									fscanf(pFile, "%f", &g_aBossKeySet[nCntKey[nMotion]][nMotion].key[nCntParts].pos.z);//Z���W
								}
								
								//ROT�����������ꍇ
								else if (!strcmp(aDataSearch, "ROT"))
								{//����
									fscanf(pFile, "%f", &g_aBossKeySet[nCntKey[nMotion]][nMotion].key[nCntParts].rot.x);//X��
									fscanf(pFile, "%f", &g_aBossKeySet[nCntKey[nMotion]][nMotion].key[nCntParts].rot.y);//Y��
									fscanf(pFile, "%f", &g_aBossKeySet[nCntKey[nMotion]][nMotion].key[nCntParts].rot.z);//Z��
								}

								//END_KEY�Z�b�g�����������ꍇ
								else if (!strcmp(aDataSearch, "END_KEY"))
								{//���f���f�[�^�̓ǂݍ��݂��I��
									nCntParts++;	//�f�[�^�������Z
									break;
								}
							}
						}

						//END_KEYSET�����������ꍇ
						else if (!strcmp(aDataSearch, "END_KEYSET"))
						{
							nCntParts = 0;
							nCntKey[nMotion]++;//�L�[�������Z
							g_aBossMotionSet[nMotion].nMaxKey = nCntKey[nMotion];//���v�L�[����ۑ�
							break;
						}

						//�R�����g�A�E�g�����������ꍇ
						else
						{//�X�L�b�v���ēǂݍ��݂𑱂���
							continue;
						}
					}
				}

				//�R�����g�A�E�g�����������ꍇ
				else
				{//�X�L�b�v���ēǂݍ��݂𑱂���
					continue;
				}
			}
		}

		//EOF�����������ꍇ
		else if (nResult == EOF)
		{//�t�@�C�������
			fclose(pFile);
			break;
		}

		//���[�V�����Z�b�g�ȊO�����������ꍇ
		else
		{//�����𑱂���
			continue;
		}
	}
}

//============================================================================================================================================================================================================
// ���[�V��������
//============================================================================================================================================================================================================
void BossMotion(void)
{
	int nState = (int)(g_aBoss.state);																		//��Ԃ�ۑ�
	int nCntKey = g_aBoss.aMotionset[nState].nCntKey;														//���݂̃L�[��ۑ�
	int nNextKey = (nCntKey + 1) % g_aBossMotionSet[nState].nMaxKey;										//���̃L�[�����߂�
	float fFrame = (float)g_aBoss.aMotionset[nState].nCntFrame / g_aBossKeySet[nCntKey][nState].nMaxFrame;	//���݂̃t���[���ƑS�̃t���[���̊���

	//�p�[�c���Ƃɏ����X�V
	for (int nCntParts = 0; nCntParts < g_aBoss.nNumParts; nCntParts++)
	{
		float BossPosX = g_aBossKeySet[nCntKey][nState].key[nCntParts].pos.x;
		float BossPosY = g_aBossKeySet[nCntKey][nState].key[nCntParts].pos.y;
		float BossPosZ = g_aBossKeySet[nCntKey][nState].key[nCntParts].pos.z;
		float BossRotX = g_aBossKeySet[nCntKey][nState].key[nCntParts].rot.x;
		float BossRotY = g_aBossKeySet[nCntKey][nState].key[nCntParts].rot.y;
		float BossRotZ = g_aBossKeySet[nCntKey][nState].key[nCntParts].rot.z;

		float NextBossPosX = g_aBossKeySet[nNextKey][nState].key[nCntParts].pos.x;
		float NextBossPosY = g_aBossKeySet[nNextKey][nState].key[nCntParts].pos.y;
		float NextBossPosZ = g_aBossKeySet[nNextKey][nState].key[nCntParts].pos.z;
		float NextBossRotX = g_aBossKeySet[nNextKey][nState].key[nCntParts].rot.x;
		float NextBossRotY = g_aBossKeySet[nNextKey][nState].key[nCntParts].rot.y;
		float NextBossRotZ = g_aBossKeySet[nNextKey][nState].key[nCntParts].rot.z;

		//���̃L�[�܂ł̍��������߂�
		float fPosx = NextBossPosX - BossPosX;	// X���W
		float fPosy = NextBossPosY - BossPosY;	// Y���W
		float fPosz = NextBossPosZ - BossPosZ;	// Z���W
		float fRotx = NextBossRotX - BossRotX;	// X���̌���
		float fRoty = NextBossRotY - BossRotY;	// Y���̌���
		float fRotz = NextBossRotZ - BossRotZ;	// Z���̌���

		//�������X�V
		g_aBoss.aParts[nCntParts].rot.x = g_aBossParts[nCntParts].rot.x + BossRotX + fRotx * fFrame;//X��
		g_aBoss.aParts[nCntParts].rot.y = g_aBossParts[nCntParts].rot.y + BossRotY + fRoty * fFrame;//Y��
		g_aBoss.aParts[nCntParts].rot.z = g_aBossParts[nCntParts].rot.z + BossRotZ + fRotz * fFrame;//Z��

		//�ʒu���X�V
		g_aBoss.aParts[nCntParts].pos.x = g_aBossParts[nCntParts].pos.x + BossPosX + fPosx * fFrame;//X���W
		g_aBoss.aParts[nCntParts].pos.y = g_aBossParts[nCntParts].pos.y + BossPosY + fPosy * fFrame;//Y���W
		g_aBoss.aParts[nCntParts].pos.z = g_aBossParts[nCntParts].pos.z + BossPosZ + fPosz * fFrame;//Z���W
	}

	//�L�[���̍X�V
	if (g_aBoss.aMotionset[nState].nCntFrame == 0 || g_aBoss.aMotionset[nState].nCntFrame >= g_aBossKeySet[nCntKey][nState].nMaxFrame)
	{
		if (g_aBoss.aMotionset[nState].nCntFrame >= g_aBossKeySet[nCntKey][nState].nMaxFrame)
		{
			//�t���[�����J�E���^�����Z�b�g
			g_aBoss.aMotionset[nState].nCntFrame = 0;
		}

		//�L�[�������Z
		g_aBoss.aMotionset[nState].nCntKey++;

		//�L�[�����ő吔�ɂȂ���
		if (g_aBoss.aMotionset[nState].nCntKey >= g_aBossMotionSet[nState].nMaxKey)
		{
			//���[�v���Ȃ��ꍇ
			if (g_aBossMotionSet[nState].nLoop == 1)
			{
				g_aBoss.nAttackTiming = 0;//�U�����I����������܂܂��̂ŁA�U���J�E���g���O�ɂ���
				g_aBoss.bHitFlag = false;//�U������������̂ŁA�q�b�g��������Z�b�g
			
				//�j���[�g�������
				g_aBoss.state = BOSSSTATE_NEUTRAL;
			}

			//�L�[����������
			g_aBoss.aMotionset[nState].nCntKey = 0;
		}
	}

	//�t���[�����X�V
	g_aBoss.aMotionset[nState].nCntFrame++;
}

//============================================================================================================================================================================================================
//�{�X����������
//============================================================================================================================================================================================================
void SetBoss(D3DXVECTOR3 pos, int nHp)
{
	D3DXMATERIAL* pMat;//�}�e���A���̃f�[�^�ւ̃|�C���^
	BYTE* pVtxBuff;//���_�o�b�t�@�ւ̃|�C���^

	if (g_aBoss.bUse == false)
	{
		g_aBoss.pos = pos;
		g_aBoss.bUse = true;
		g_aBoss.nHp = nHp;
		g_aBoss.nMaxHp = nHp;
		g_aBoss.fSpeed = 3.0f;
		g_aBoss.fRotMove = 0.0f;//�ꕔ�̏����ł��̕ϐ��̒l�̕����Ɉړ�������
		g_aBoss.nNumLandingModel = 0;//�ǂ̃��f���̏�ɏ���Ă��邩
		g_aBoss.nIdxShadow = SetShadow();//�e�̃C���f�b�N�X��ݒ�
		g_aBoss.nIdxRaderDisplay = SetRaderDisplay(RADERDISPLAY02_BOSS);//���[�_�[�f�B�X�v���C��ݒ�
		g_aBoss.fVXaim3D = 0.0f;
		g_aBoss.fVYaim3D = 0.0f;
		g_aBoss.fVZaim3D = 0.0f;
		g_aBoss.fVLaim3D = 0.0f;
		g_aBoss.SaveMove3D = NULL_VECTOR3;
		g_aBoss.nCntAppearTime = 0;
		g_aBoss.bBossAppear = true;//�{�X�o�����o�����ǂ���
		g_aBoss.bCurveLaser = false;//���[�U�[���J�[�u��������������߂�
		g_aBoss.nRunAwayLottery = 0;//��m���Ń{�X�𓦂������闐��
		g_aBoss.bRunAwayBoss = false;//�t���O�������Ƀ{�X�𓦂�������t���O
		g_aBoss.nCntSummonEnemy = 0;//�{�X�����������G�̐����J�E���g����
		g_aBoss.nTotalDamage = 0;//�^�����_���[�W���v
		g_aBoss.nDamageCnt = 0;
		g_aBoss.nCntUseAttack = 0;//�U���������񐔂��J�E���g����

		SetBossGauge(0);

		for (int nCntParts = 0; nCntParts < g_aBoss.nNumParts; nCntParts++)
		{
			//�}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_aBoss.aParts[nCntParts].pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aBoss.aParts[nCntParts].dwNumMat; nCntMat++)
			{
				g_aBoss.aParts[nCntParts].Diffuse[nCntMat] = pMat[nCntMat].MatD3D.Diffuse;
			}

			//���_���̎擾
			int nNumVtx = g_aBoss.aParts[nCntParts].pMesh->GetNumVertices();

			//���_�t�H�[�}�b�g�̃T�C�Y���擾
			DWORD sizeFVF = D3DXGetFVFVertexSize(g_aBoss.aParts[nCntParts].pMesh->GetFVF());

			//���_�o�b�t�@�̃��b�N
			g_aBoss.aParts[nCntParts].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

			for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
			{
				//���_���W�̑��
				D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;


				//================================
				// �v���C���[�̍ő�ŏ������߂�
				//================================

				//���f���̒��_���W�̍ŏ��l�ƍő�l���擾
				if (vtx.x < g_aBoss.vtxMin.x)
				{//X���W�̍ŏ��l
					g_aBoss.vtxMin.x = vtx.x;
				}
				if (vtx.y < g_aBoss.vtxMin.y)
				{//Y���W�̍ŏ��l
					g_aBoss.vtxMin.y = vtx.y;
				}
				if (vtx.z < g_aBoss.vtxMin.z)
				{//Z���W�̍ŏ��l
					g_aBoss.vtxMin.z = vtx.z;
				}
				if (vtx.x > g_aBoss.vtxMax.x)
				{//X���W�̍ő�l
					g_aBoss.vtxMax.x = vtx.x;
				}
				if (vtx.y > g_aBoss.vtxMax.y)
				{//Y���W�̍ő�l
					g_aBoss.vtxMax.y = vtx.y;
				}
				if (vtx.z > g_aBoss.vtxMax.z)
				{//Z���W�̍ő�l
					g_aBoss.vtxMax.z = vtx.z;
				}

				//���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
				pVtxBuff += sizeFVF;
			}

			//��ނ��Ƃ̃T�C�Y�����߂�
			g_aBoss.Size =
			{
				g_aBoss.vtxMax.x - g_aBoss.vtxMin.x,
				g_aBoss.vtxMax.y - g_aBoss.vtxMin.y,
				g_aBoss.vtxMax.z - g_aBoss.vtxMin.z
			};

			//���_�o�b�t�@�̃A�����b�N
			g_aBoss.aParts[nCntParts].pMesh->UnlockVertexBuffer();
		}
	}
}

void SetGraspBoss(void)
{
	if (g_aGraspBoss.bUse == false)
	{
		g_aGraspBoss.pos = NULL_VECTOR3;
		g_aGraspBoss.rot = NULL_VECTOR3;
		g_aGraspBoss.bUse = true;
	}
}

//============================================================================================================================================================================================================
//�{�X�̏����擾����
//============================================================================================================================================================================================================
Boss * GetBoss(void)
{
	return &g_aBoss;
}

//============================================================================================================================================================================================================
//�{�X�Ƀ_���[�W��^����
//============================================================================================================================================================================================================
void SetDamageBoss(int nDamage, PLAYERATTACK PlayerAttackNum, int nNumAttackModel)
{
	if (g_aBoss.bHitStop[PlayerAttackNum] == false)//�q�b�g�X�g�b�v��Ԃ���Ȃ����
	{
		if (PlayerAttackNum == PLAYERATTACK00_SWORD || PlayerAttackNum == PLAYERATTACK02_SPEAKER)
		{
			g_aBoss.nDamageCnt += nDamage;
			g_aBoss.nHp -= nDamage;//�̗͂����炷
			g_aBoss.bHitStop[PlayerAttackNum] = true;  //�_���[�W��^�����U���ɑ΂��ăq�b�g�X�g�b�v��Ԃɂ���

			for (int nCntParticle = 0; nCntParticle < 100; nCntParticle++)
			{
				float fRot = float(rand() % 200 + 1) / 100;
				float fYmove = float(rand() % 200 - 100) / 10;
				float fSpeed = float(rand() % 200 - 100) / 10;

				SetParticle(PARTICLETYPE_NORMAL, 300, 10.0f, 0.6f, g_aBoss.pos,
					D3DXVECTOR3(sinf(-D3DX_PI * fRot) * fSpeed, fYmove, cosf(-D3DX_PI * fRot) * fSpeed),
					D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 0.0f, PARTICLETEX_NORMAL,0);
			}

			//========================
			//���ʉ�����
			//========================
			if (PlayerAttackNum == PLAYERATTACK00_SWORD)
			{
				PlaySound(SOUND_LABEL_SE_SWORDSLASH_000);
			}
			if (PlayerAttackNum == PLAYERATTACK02_SPEAKER)
			{
				PlaySound(SOUND_LABEL_SE_SPEAKERWAVEHIT_000);
			}
		}
	}
		if (g_aBoss.bHitStopBoomerang[nNumAttackModel] == false && PlayerAttackNum == PLAYERATTACK01_BOOMERANG)
		{
			g_aBoss.nDamageCnt += nDamage;
			g_aBoss.nHp -= nDamage;//�̗͂����炷
			g_aBoss.bHitStopBoomerang[nNumAttackModel] = true;

			for (int nCntParticle = 0; nCntParticle < 100; nCntParticle++)
			{
				float fRot = float(rand() % 200 + 1) / 100;
				float fYmove = float(rand() % 200 - 100) / 10;
				float fSpeed = float(rand() % 200 - 100) / 10;

				SetParticle(PARTICLETYPE_NORMAL, 300, 10.0f, 0.6f, g_aBoss.pos,
					D3DXVECTOR3(sinf(-D3DX_PI * fRot) * fSpeed, fYmove, cosf(-D3DX_PI * fRot) * fSpeed),
					D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 0.0f, PARTICLETEX_NORMAL,0);
			}

		    if (PlayerAttackNum == PLAYERATTACK01_BOOMERANG)
		    {
		    	PlaySound(SOUND_LABEL_SE_HITBOOMERANG_000);
		    }

		}
}

//============================================================================================================================================================================================================
//�q�b�g�X�g�b�v����
//============================================================================================================================================================================================================
void HitStopBoss(void)
{
	//======================================
	//�U���󂯕t������
	//======================================

	for (int nCntHitStop = 0; nCntHitStop < PLAYERATTACK_MAX; nCntHitStop++)
	{
		if (g_aBoss.bHitStop[nCntHitStop] == true)
		{
			g_aBoss.nCoolTime[nCntHitStop]++;

			switch (nCntHitStop)
			{
			case PLAYERATTACK00_SWORD:

				if (g_aBoss.nCoolTime[nCntHitStop] >= 20)
				{
					g_aBoss.bHitStop[nCntHitStop] = false;
					g_aBoss.nCoolTime[nCntHitStop] = 0;
				}
				break;
			case PLAYERATTACK02_SPEAKER:

				if (g_aBoss.nCoolTime[nCntHitStop] >= 2)
				{
					g_aBoss.bHitStop[nCntHitStop] = false;
					g_aBoss.nCoolTime[nCntHitStop] = 0;
				}
				break;
			}
		}
	}

	for (int nCntAttackModel = 0; nCntAttackModel < MAX_ATTACKMODEL; nCntAttackModel++)
	{
		if (g_aBoss.bHitStopBoomerang[nCntAttackModel] == true)
		{
			g_aBoss.nCoolTimeBoomerang[nCntAttackModel]++;

			if (g_aBoss.nCoolTimeBoomerang[nCntAttackModel] >= 20)
			{
				g_aBoss.bHitStopBoomerang[nCntAttackModel] = false;
				g_aBoss.nCoolTimeBoomerang[nCntAttackModel] = 0;
			}
		}
	}
}

//============================================================================================================================================================================================================
//�{�X�U���O�F�A�g�~�b�N�v���X
//============================================================================================================================================================================================================
void BossAttack0_AtomicPress(void)
{
	Player* pPlayer = GetPlayer();	//�v���C���[�̏����擾

	g_aBoss.nCntAttackTime++;

	if (g_aBoss.nCntAttackTime == 1)
	{
		g_bPurposeFlag[0] = false;
		g_bPurposeFlag[1] = false;
	}
	else if (g_aBoss.nCntAttackTime == 20)
	{
		g_aBoss.state = BOSSSTATE_ATTACK;
	}

	if (g_aBoss.nCntAttackTime >= 30 && g_aBoss.nCntAttackTime <= 120)
	{
		g_aBoss.move.x = sinf(g_aBoss.fVaim) * 30.0f;
		g_aBoss.move.z = cosf(g_aBoss.fVaim) * 30.0f;

		g_aBoss.move.y = 16.0f;
	}

	if (g_aBoss.nCntAttackTime >= 90 && g_aBoss.nCntAttackTime <= 150)
	{
		if (g_aBoss.pos.x >= pPlayer->pos.x - 40.0f &&
			g_aBoss.pos.x <= pPlayer->pos.x + 40.0f &&
			g_aBoss.pos.z >= pPlayer->pos.z - 40.0f &&
			g_aBoss.pos.z <= pPlayer->pos.z + 40.0f && g_bPurposeFlag[0] == false)
		{
			g_bPurposeFlag[0] = true;
		}

		if (g_bPurposeFlag[0] == true)
		{
			g_aBoss.move = NULL_VECTOR3;
			g_aBoss.move.y = -20.0f;
		}

		if (g_aBoss.pos.y <= 0.0f && g_bPurposeFlag[1] == false)
		{
			SetAttackModel(ATTACKMODEL02_SPHERE, 180, g_aBoss.pos, NULL_VECTOR3, NULL_VECTOR3, NULL_VECTOR3, 0.0f, 0.0f, 0.2f, NULL_VECTOR3,0);
			g_bPurposeFlag[1] = true;

			PlaySound(SOUND_LABEL_SE_BIRDSTRIKE_000);

			for (int nCntBullet = 0; nCntBullet < 8; nCntBullet++)
			{
				float fRot = (2.0f / 12.0f) * (float)(nCntBullet);

				SetBullet(BULLETTYPE_CURVE, 180, 10.0f, 10.0f,D3DXVECTOR3(g_aBoss.pos.x,g_aBoss.pos.y + 10.0f,g_aBoss.pos.z),
					NULL_VECTOR3,D3DXVECTOR3(5.0f,fRot,5.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0, 0.0f);

				SetBullet(BULLETTYPE_CURVE, 180, 10.0f, 10.0f, D3DXVECTOR3(g_aBoss.pos.x, g_aBoss.pos.y + 10.0f, g_aBoss.pos.z),
					NULL_VECTOR3, D3DXVECTOR3(5.0f, fRot, 5.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 1, 0.0f);
			}
		}
	}

	if (g_aBoss.nCntAttackTime >= 130 && g_aBoss.nCntAttackTime <= 230)
	{
		g_aBoss.state = BOSSSTATE_ATTACK;

		if (g_aBoss.nCntAttackTime == 230)
		{
			g_aBoss.bUnderAttack = false;
			g_aBoss.nCntAttackTime = 0;
		}
	}
}

//============================================================================================================================================================================================================
//�{�X�U���P�F�t���b�V���p���`
//============================================================================================================================================================================================================
void BossAttack1_FlashPunch(void)
{
	Player* pPlayer = GetPlayer();	//�v���C���[�̏����擾

	g_aBoss.nCntAttackTime++;

	if (g_aBoss.nCntAttackTime == 1)
	{
		g_aBoss.move = NULL_VECTOR3;
		g_bPurposeFlag[0] = false;
	}

	if (g_aBoss.nCntAttackTime >= 20 && g_aBoss.nCntAttackTime <= 60)
	{
		g_aBoss.rot.y = g_aBoss.fVaim + D3DX_PI;//�v���C���[�̕���������
	}

	if (g_aBoss.nCntAttackTime == 90)
	{
		g_aBoss.fSaveAim = g_aBoss.fVaim;
	}

	if (g_aBoss.nCntAttackTime >= 96 && g_aBoss.nCntAttackTime <= 136)
	{
		if (g_aBoss.nCntAttackTime == 96)
		{
			PlaySound(SOUND_LABEL_SE_FASTMOVE_000);
		}

		g_aBoss.move.x = sinf(g_aBoss.fSaveAim) * 20.0f;
		g_aBoss.move.z = cosf(g_aBoss.fSaveAim) * 20.0f;

		g_aBoss.state = BOSSSTATE_ATTACK;

		//�E��ƃv���C���[�̋���
		float fLengthAttack = sqrtf((g_aBoss.aParts[3].PartsPos.x - pPlayer->pos.x) * (g_aBoss.aParts[3].PartsPos.x - pPlayer->pos.x) +
			(g_aBoss.aParts[3].PartsPos.z - pPlayer->pos.z) * (g_aBoss.aParts[3].PartsPos.z - pPlayer->pos.z) +
			(g_aBoss.aParts[3].PartsPos.y - pPlayer->pos.y) * (g_aBoss.aParts[3].PartsPos.y - pPlayer->pos.y)) / 3;

		if (fLengthAttack < 40.0f && g_bPurposeFlag[0] == false)
		{
			AttackHitPlayer(60);
			g_bPurposeFlag[0] = true;

			PlaySound(SOUND_LABEL_SE_FLUSHPUNCH_000);

			for (int nCntParticle = 0; nCntParticle < 40; nCntParticle++)
			{
				float fRandRot = float(rand() % 200 + 1) / 100.0f;
				float fRandSpeed = float(rand() % 20 + 1 - 10);
				float fRandYMove = float(rand() % 20 + 1 - 10);
				SetParticle(PARTICLETEX_NORMAL, 60, 10.0f, 0.4f, pPlayer->pos, D3DXVECTOR3(sinf(D3DX_PI * fRandRot) * fRandSpeed, fRandYMove, cosf(D3DX_PI * fRandRot) * fRandSpeed),
					D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 0.0f, PARTICLETEX_NORMAL,0);
			}
		}
	}

	if (g_aBoss.nCntAttackTime >= 150 && g_aBoss.nCntAttackTime <= 180)
	{
		g_aBoss.rot.y = g_aBoss.fVaim + D3DX_PI;//�v���C���[�̕���������
	}

	if (g_aBoss.nCntAttackTime == 181)
	{
		g_aBoss.fSaveAim = g_aBoss.fVaim;
		g_bPurposeFlag[0] = false;
	}

	if (g_aBoss.nCntAttackTime >= 200 && g_aBoss.nCntAttackTime <= 235)
	{
		if (g_aBoss.nCntAttackTime == 200)
		{
			PlaySound(SOUND_LABEL_SE_FASTMOVE_000);
		}

		g_aBoss.move.x = sinf(g_aBoss.fSaveAim) * 20.0f;
		g_aBoss.move.z = cosf(g_aBoss.fSaveAim) * 20.0f;

		g_aBoss.state = BOSSSTATE_ATTACK;

		//�E��ƃv���C���[�̋���
		float fLengthAttack = sqrtf((g_aBoss.aParts[3].PartsPos.x - pPlayer->pos.x) * (g_aBoss.aParts[3].PartsPos.x - pPlayer->pos.x) +
			(g_aBoss.aParts[3].PartsPos.z - pPlayer->pos.z) * (g_aBoss.aParts[3].PartsPos.z - pPlayer->pos.z) +
			(g_aBoss.aParts[3].PartsPos.y - pPlayer->pos.y) * (g_aBoss.aParts[3].PartsPos.y - pPlayer->pos.y)) / 3;

		if (fLengthAttack < 40.0f && g_bPurposeFlag[0] == false)
		{
			AttackHitPlayer(70);
			g_bPurposeFlag[0] = true;

			PlaySound(SOUND_LABEL_SE_FLUSHPUNCH_000);


			for (int nCntParticle = 0; nCntParticle < 50; nCntParticle++)
			{
				float fRandRot = float(rand() % 200 + 1) / 100.0f;
				float fRandSpeed = float(rand() % 20 + 1 - 10);
				float fRandYMove = float(rand() % 20 + 1 - 10);
				SetParticle(PARTICLETEX_NORMAL, 60, 20.0f, 0.3f, pPlayer->pos, D3DXVECTOR3(sinf(D3DX_PI * fRandRot) * fRandSpeed, fRandYMove, cosf(D3DX_PI * fRandRot) * fRandSpeed),
					D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 0.0f, PARTICLETEX_NORMAL,0);
			}
		}
	}

	if (g_aBoss.nCntAttackTime >= 250 && g_aBoss.nCntAttackTime <= 270)
	{
		g_aBoss.rot.y = g_aBoss.fVaim + D3DX_PI;//�v���C���[�̕���������
	}

	if (g_aBoss.nCntAttackTime == 271)
	{
		g_aBoss.fSaveAim = g_aBoss.fVaim;

		g_bPurposeFlag[0] = false;
	}

	if (g_aBoss.nCntAttackTime >= 300 && g_aBoss.nCntAttackTime <= 340)
	{
		if (g_aBoss.nCntAttackTime == 300)
		{
			PlaySound(SOUND_LABEL_SE_FASTMOVE_000);
		}

		g_aBoss.move.x = sinf(g_aBoss.fSaveAim) * 25.0f;
		g_aBoss.move.z = cosf(g_aBoss.fSaveAim) * 25.0f;

		g_aBoss.state = BOSSSTATE_ATTACK;

		//�E��ƃv���C���[�̋���
		float fLengthAttack = sqrtf((g_aBoss.aParts[3].PartsPos.x - pPlayer->pos.x) * (g_aBoss.aParts[3].PartsPos.x - pPlayer->pos.x) +
			(g_aBoss.aParts[3].PartsPos.z - pPlayer->pos.z) * (g_aBoss.aParts[3].PartsPos.z - pPlayer->pos.z) +
			(g_aBoss.aParts[3].PartsPos.y - pPlayer->pos.y) * (g_aBoss.aParts[3].PartsPos.y - pPlayer->pos.y)) / 3;

		if (fLengthAttack < 40.0f && g_bPurposeFlag[0] == false)
		{
			AttackHitPlayer(70);
			g_bPurposeFlag[0] = true;

			PlaySound(SOUND_LABEL_SE_FLUSHPUNCH_000);


			for (int nCntParticle = 0; nCntParticle < 50; nCntParticle++)
			{
				float fRandRot = float(rand() % 200 + 1) / 100.0f;
				float fRandSpeed = float(rand() % 20 + 1 - 10);
				float fRandYMove = float(rand() % 20 + 1 - 10);
				SetParticle(PARTICLETEX_NORMAL, 60, 20.0f, 0.3f, pPlayer->pos, D3DXVECTOR3(sinf(D3DX_PI * fRandRot) * fRandSpeed, fRandYMove, cosf(D3DX_PI * fRandRot) * fRandSpeed),
					D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 0.0f, PARTICLETEX_NORMAL,0);
			}
		}
	}

	if (g_aBoss.nCntAttackTime == 350)
	{
		g_aBoss.bUnderAttack = false;
		g_aBoss.nCntAttackTime = 0;
	}

	g_aBoss.move.x += (0.0f - g_aBoss.move.x) * 0.1f;
	g_aBoss.move.z += (0.0f - g_aBoss.move.z) * 0.1f;
}

//============================================================================================================================================================================================================
//�{�X�U���Q�F�X���X�g�X�^�u
//============================================================================================================================================================================================================
void BossAttack2_ThrustStub(void)
{
	Player* pPlayer = GetPlayer();	//�v���C���[�̏����擾

	g_aBoss.nCntAttackTime++;

	if (g_aBoss.nCntAttackTime == 1)
	{
		g_aBoss.move = NULL_VECTOR3;
		g_bPurposeFlag[0] = false;
	}

	if (g_aBoss.nCntAttackTime == 15)
	{
		g_aBoss.state = BOSSSTATE_ATTACK;
		g_aBoss.fSaveAim = g_aBoss.fVaim;
		g_aBoss.rot.y = g_aBoss.fVaim + D3DX_PI;
	}

	if (g_aBoss.nCntAttackTime >= 81 && g_aBoss.nCntAttackTime <= 135)
	{
		float fRandPos = float(rand() % 500 + 1);
		float fRandY = float(rand() % 100 * 1);
		float fRandRot = float(rand() % 150 - 75) / 100;
		SetAttackbillboard(ATTACKBILLBOARD00_STUB, 60, 40.0f, 40.0f,
		D3DXVECTOR3(sinf(g_aBoss.fSaveAim + fRandRot) * fRandPos + g_aBoss.pos.x, fRandY + g_aBoss.pos.y, cosf(g_aBoss.fSaveAim + fRandRot) * fRandPos + g_aBoss.pos.z), NULL_VECTOR3, NULL_VECTOR3,
		0, 0.0f);
	}

	if (g_aBoss.nCntAttackTime == 170)
	{
		g_aBoss.fSaveAim = g_aBoss.fVaim;
	}

	if (g_aBoss.nCntAttackTime >= 200 && g_aBoss.nCntAttackTime <= 220)
	{
		g_aBoss.move = D3DXVECTOR3(sinf(g_aBoss.fSaveAim) * 50.0f,0.0f,cosf(g_aBoss.fSaveAim) * 50.0f);
	}

	if (g_aBoss.nCntAttackTime >= 200 && g_aBoss.nCntAttackTime <= 250)
	{
		g_aBoss.rot.y = g_aBoss.fVaim + D3DX_PI;

		if (g_aBoss.pos.x >= pPlayer->pos.x - 150.0f &&
			g_aBoss.pos.x <= pPlayer->pos.x + 150.0f &&
			g_aBoss.pos.z >= pPlayer->pos.z - 150.0f &&
			g_aBoss.pos.z <= pPlayer->pos.z + 150.0f)
		{
			g_aBoss.move = NULL_VECTOR3;
			g_bPurposeFlag[0] = true;
		}
		if (g_bPurposeFlag[0] == true)
		{
			g_aBoss.move = NULL_VECTOR3;
			g_aBoss.pos = D3DXVECTOR3(sinf(pPlayer->rot.y) * 100.0f + pPlayer->pos.x, 0.0f, cosf(pPlayer->rot.y) * 100.0f + pPlayer->pos.z);
		}
	}

	if (g_aBoss.nCntAttackTime == 255)
	{
		g_aBoss.state = BOSSSTATE_ATTACK;
		g_aBoss.fSaveAim = g_aBoss.fVaim;
		g_aBoss.rot.y = g_aBoss.fVaim + D3DX_PI;
	}

	if (g_aBoss.nCntAttackTime >= 291 && g_aBoss.nCntAttackTime <= 345)
	{
		float fRandPos = float(rand() % 500 + 1);
		float fRandY = float(rand() % 100 * 1);
		float fRandRot = float(rand() % 150 - 75) / 100;

		SetAttackbillboard(ATTACKBILLBOARD00_STUB, 60, 40.0f, 40.0f,
			D3DXVECTOR3(sinf(g_aBoss.fSaveAim + fRandRot) * fRandPos + g_aBoss.pos.x, fRandY + g_aBoss.pos.y, cosf(g_aBoss.fSaveAim + fRandRot) * fRandPos + g_aBoss.pos.z), NULL_VECTOR3, NULL_VECTOR3,
			0, 0.0f);
	}

	if (g_aBoss.nCntAttackTime == 340)
	{
		g_aBoss.bUnderAttack = false;
		g_aBoss.nCntAttackTime = 0;
	}

	g_aBoss.move.x += (0.0f - g_aBoss.move.x) * 0.1f;
	g_aBoss.move.z += (0.0f - g_aBoss.move.z) * 0.1f;
}

//============================================================================================================================================================================================================
//�{�X�̍U���R�F�o�[�h�X�g���C�N
//============================================================================================================================================================================================================
void BossAttack3_BirdStrike(void)
{
	Player* pPlayer = GetPlayer();	//�v���C���[�̏����擾

	g_aBoss.nCntAttackTime++;

	if (g_aBoss.nCntAttackTime == 1)
	{
		g_aBoss.move = NULL_VECTOR3;
		g_bPurposeFlag[0] = false;
	}

	//==========================
	//�R�������@�_������
	//==========================
	g_aBoss.fVXaim3D = pPlayer->pos.x - g_aBoss.pos.x;
	g_aBoss.fVYaim3D = pPlayer->pos.y - g_aBoss.pos.y;
	g_aBoss.fVZaim3D = pPlayer->pos.z - g_aBoss.pos.z;

	D3DXVECTOR3 BulletMove = NULL_VECTOR3;

	g_aBoss.fVLaim3D = sqrtf((g_aBoss.fVXaim3D * g_aBoss.fVXaim3D) + (g_aBoss.fVYaim3D * g_aBoss.fVYaim3D)
		+ (g_aBoss.fVZaim3D * g_aBoss.fVZaim3D));

	BulletMove.x = g_aBoss.fVXaim3D / g_aBoss.fVLaim3D * 30.0f;
	BulletMove.y = g_aBoss.fVYaim3D / g_aBoss.fVLaim3D * 30.0f;
	BulletMove.z = g_aBoss.fVZaim3D / g_aBoss.fVLaim3D * 30.0f;

	if (g_aBoss.nCntAttackTime == 30)
	{
		g_aBoss.move.y = 30.0f;
	}

	if (g_aBoss.nCntAttackTime >= 50 && g_aBoss.nCntAttackTime <= 200)
	{
		g_aBoss.move = NULL_VECTOR3;

		if (g_nDelayBoss % 3 == 0)
		{
			float fRandRot = float(rand() % 200 + 1) / 100.0f;
			float fRandSpeed = float(rand() % 20 - 10);
		    
			SetBullet(BULLETTYPE_BOUND, 200, 20.0f, 20.0f, g_aBoss.pos,
				D3DXVECTOR3(sinf(D3DX_PI * fRandRot) * fRandSpeed, 5.0f, cosf(D3DX_PI * fRandRot) * fRandSpeed),
				NULL_VECTOR3, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0, 0.0f);
		}
	}

	if (g_aBoss.nCntAttackTime >= 201 && g_aBoss.nCntAttackTime <= 320)
	{
		if (g_aBoss.nCntAttackTime == 231)
		{
			g_aBoss.SaveMove3D = BulletMove;//���̎��_�ł̂R�����x�N�g����ۑ�
		}

		g_aBoss.rot.y = g_aBoss.fVaim + D3DX_PI;
		g_aBoss.rot.x += 0.1f;
		g_aBoss.rot.z += 0.1f;


		g_aBoss.move = NULL_VECTOR3;
	}

	if (g_aBoss.nCntAttackTime >= 301 && g_aBoss.nCntAttackTime <= 321)
	{
		g_aBoss.move = g_aBoss.SaveMove3D;//�v���C���[�Ɍ������ē˂�����
	}

	if (g_aBoss.pos.y <= 0.0f && g_aBoss.nCntAttackTime >= 150 && g_bPurposeFlag[0] == false)
	{
		g_aBoss.move = NULL_VECTOR3;

		g_bPurposeFlag[0] = true;

		PlaySound(SOUND_LABEL_SE_BIRDSTRIKE_000);
		PlaySound(SOUND_LABEL_SE_THUNDER_000);
		for (int nCntThunder = 0; nCntThunder < 16; nCntThunder++)
		{
			float fRot = (2.0f / 16.0f) * nCntThunder;
			SetAttackModel(ATTACKMODEL06_THUNDER, 500, g_aBoss.pos,
				D3DXVECTOR3(sinf(D3DX_PI * fRot) * 10.0f, 0.0f, cosf(D3DX_PI * fRot) * 10.0f),
				NULL_VECTOR3,D3DXVECTOR3(1.5f,0.5f,0.5f), fRot, 0.0f, 0.0f, NULL_VECTOR3, ATTACKMODELBEHAVIOR00_NORMAL);
		}
	}

	if (g_bPurposeFlag[0] == true)
	{
		g_aBoss.move = NULL_VECTOR3;
	}

	if (g_aBoss.nCntAttackTime == 400)
	{
		g_aBoss.bUnderAttack = false;
		g_aBoss.nCntAttackTime = 0;
		g_aBoss.rot = NULL_VECTOR3;
	}
}


//============================================================================================================================================================================================================
//�{�X�U���S�F�{�����C��
//============================================================================================================================================================================================================
void BossAttack4_BombRain(void)
{
	Player* pPlayer = GetPlayer();	//�v���C���[�̏����擾

	g_aBoss.nCntAttackTime++;

	if (g_aBoss.nCntAttackTime == 1)
	{
		g_aBoss.move = NULL_VECTOR3;//�ړ��ʂ��O��
		g_bPurposeFlag[0] = false;//�ėp�I�Ɏg���t���O
		g_aBoss.state = BOSSSTATE_ATTACK;//�U�����
	}


	if (g_aBoss.nCntAttackTime >= 150)
	{
		g_aBoss.state = BOSSSTATE_MOVE;
		g_aBoss.rot.y = g_aBoss.fVaim + D3DX_PI;
		
		if (g_aBoss.nCntAttackTime == 151)
		{
			g_aBoss.fRotMove = g_aBoss.fVaim;
		}

		if (g_aBoss.fRotMove < g_aBoss.fVaim)
		{
			g_aBoss.fRotMove += 0.05f;
		}
		else
		{
			g_aBoss.fRotMove -= 0.05f;

		}

		g_aBoss.move.x = sinf(g_aBoss.fRotMove) * 5.0f;
		g_aBoss.move.z = cosf(g_aBoss.fRotMove) * 5.0f;
		
		if (g_aBoss.fRotMove >= g_aBoss.fVaim - 0.3f && g_aBoss.fRotMove <= g_aBoss.fVaim + 0.3f)
		{
			g_aBoss.move.x = sinf(g_aBoss.fRotMove) * 10.0f;
			g_aBoss.move.z = cosf(g_aBoss.fRotMove) * 10.0f;
		}
	}

	float fRandLength = float(rand() % 1000 + 1);
	float fRandRot = float(rand() % 200 + 1) / 100;
	float fRandPosY = float(rand() % 500 + 400);

	if (g_aBoss.nCntAttackTime % 25 == 0)
	{
		SetAttackModel(ATTACKMODEL03_BOMB,300, D3DXVECTOR3(pPlayer->pos.x,300.0f, pPlayer->pos.z), NULL_VECTOR3, NULL_VECTOR3,D3DXVECTOR3(1.0f, 1.0f, 1.0f),0.0f,0.0f,0.0f,NULL_VECTOR3,0);

		SetAttackModel(ATTACKMODEL03_BOMB,300, D3DXVECTOR3(pPlayer->pos.x + sinf(D3DX_PI * fRandRot) * fRandLength, fRandPosY, pPlayer->pos.z + cosf(D3DX_PI * fRandRot) * fRandLength), NULL_VECTOR3, NULL_VECTOR3, D3DXVECTOR3(1.0f, 1.0f, 1.0f), 0.0f, 0.0f, 0.0f, NULL_VECTOR3, 0);
	}

	if (g_aBoss.nCntAttackTime == 1000)
	{
		g_bPurposeFlag[0] = false;//�ėp�I�Ɏg���t���O
		g_aBoss.bUnderAttack = false;//�U�����Ă��邩�ǂ���
		g_aBoss.nCntAttackTime = 0;
	}
}

//============================================================================================================================================================================================================
//�{�X�U���T�F�N�E�F���W�A�[�X
//============================================================================================================================================================================================================
void BossAttack5_QuellTheEarth(void)
{
	g_aBoss.nCntAttackTime++;


	if (g_aBoss.nCntAttackTime == 1)
	{
		g_aBoss.move = NULL_VECTOR3;//�ړ��ʂ��O��
		g_bPurposeFlag[0] = false;//�ėp�I�Ɏg���t���O
		g_nPurposeCnt = 0;//�ėp�J�E���g
	}

	if (g_aBoss.nCntAttackTime == 100)
	{
		g_aBoss.state = BOSSSTATE_ATTACK;
	}

	if (g_aBoss.nCntAttackTime == 180)
	{
		g_aBoss.rot.y = g_aBoss.fVaim + D3DX_PI;

		PlaySound(SOUND_LABEL_SE_SHOCKWAVE_000);

		SetAttackModel(ATTACKMODEL04_SHOCKWAVE,400,g_aBoss.pos,D3DXVECTOR3(sinf(g_aBoss.fVaim) * 12.0f,0.0f,cosf(g_aBoss.fVaim) * 12.0f),D3DXVECTOR3(g_aBoss.rot.x,g_aBoss.rot.y,g_aBoss.rot.z),D3DXVECTOR3(2.0f,2.0f,2.0f),0.0f,0.0f,0.0f,NULL_VECTOR3,ATTACKMODELBEHAVIOR00_NORMAL);
		SetAttackModel(ATTACKMODEL04_SHOCKWAVE, 400, g_aBoss.pos, D3DXVECTOR3(sinf(g_aBoss.fVaim + 0.2f) * 10.0f, 0.0f, cosf(g_aBoss.fVaim + 0.2f) * 10.0f), D3DXVECTOR3(g_aBoss.rot.x,g_aBoss.rot.y + 0.2f,g_aBoss.rot.z), D3DXVECTOR3(2.0f, 2.0f, 2.0f), 0.0f, 0.0f, 0.0f, NULL_VECTOR3, ATTACKMODELBEHAVIOR00_NORMAL);
		SetAttackModel(ATTACKMODEL04_SHOCKWAVE, 400, g_aBoss.pos, D3DXVECTOR3(sinf(g_aBoss.fVaim - 0.2f) * 10.0f, 0.0f, cosf(g_aBoss.fVaim - 0.2f) * 10.0f), D3DXVECTOR3(g_aBoss.rot.x,g_aBoss.rot.y - 0.2f,g_aBoss.rot.z), D3DXVECTOR3(2.0f, 2.0f, 2.0f), 0.0f, 0.0f, 0.0f, NULL_VECTOR3, ATTACKMODELBEHAVIOR00_NORMAL);
		SetAttackModel(ATTACKMODEL04_SHOCKWAVE, 400, g_aBoss.pos, D3DXVECTOR3(sinf(g_aBoss.fVaim + 0.4f) * 8.0f, 0.0f, cosf(g_aBoss.fVaim + 0.4f) * 8.0f), D3DXVECTOR3(g_aBoss.rot.x,g_aBoss.rot.y + 0.4f,g_aBoss.rot.z), D3DXVECTOR3(2.0f, 2.0f, 2.0f), 0.0f, 0.0f, 0.0f, NULL_VECTOR3, ATTACKMODELBEHAVIOR00_NORMAL);
		SetAttackModel(ATTACKMODEL04_SHOCKWAVE, 400, g_aBoss.pos, D3DXVECTOR3(sinf(g_aBoss.fVaim - 0.4f) * 8.0f, 0.0f, cosf(g_aBoss.fVaim - 0.4f) * 8.0f), D3DXVECTOR3(g_aBoss.rot.x,g_aBoss.rot.y - 0.4f,g_aBoss.rot.z), D3DXVECTOR3(2.0f, 2.0f, 2.0f), 0.0f, 0.0f, 0.0f, NULL_VECTOR3, ATTACKMODELBEHAVIOR00_NORMAL);
		SetAttackModel(ATTACKMODEL04_SHOCKWAVE, 400, g_aBoss.pos, D3DXVECTOR3(sinf(g_aBoss.fVaim + 0.6f) * 6.0f, 0.0f, cosf(g_aBoss.fVaim + 0.6f) * 6.0f), D3DXVECTOR3(g_aBoss.rot.x,g_aBoss.rot.y + 0.6f,g_aBoss.rot.z), D3DXVECTOR3(2.0f, 2.0f, 2.0f), 0.0f, 0.0f, 0.0f, NULL_VECTOR3, ATTACKMODELBEHAVIOR00_NORMAL);
		SetAttackModel(ATTACKMODEL04_SHOCKWAVE, 400, g_aBoss.pos, D3DXVECTOR3(sinf(g_aBoss.fVaim - 0.6f) * 6.0f, 0.0f, cosf(g_aBoss.fVaim - 0.6f) * 6.0f), D3DXVECTOR3(g_aBoss.rot.x,g_aBoss.rot.y - 0.6f,g_aBoss.rot.z), D3DXVECTOR3(2.0f, 2.0f, 2.0f), 0.0f, 0.0f, 0.0f, NULL_VECTOR3, ATTACKMODELBEHAVIOR00_NORMAL);
	}

	if (g_aBoss.nCntAttackTime == 270)
	{
		g_aBoss.state = BOSSSTATE_ATTACK;
		g_aBoss.rot.y = g_aBoss.fVaim + D3DX_PI;

		PlaySound(SOUND_LABEL_SE_SHOCKWAVE_000);

		SetAttackModel(ATTACKMODEL04_SHOCKWAVE, 400, g_aBoss.pos, D3DXVECTOR3(sinf(g_aBoss.fVaim) * 12.0f, 0.0f, cosf(g_aBoss.fVaim) * 212.0f), g_aBoss.rot, D3DXVECTOR3(2.0f, 2.0f, 2.0f), 0.0f, 0.0f, 0.0f, NULL_VECTOR3, ATTACKMODELBEHAVIOR00_NORMAL);
		SetAttackModel(ATTACKMODEL04_SHOCKWAVE, 400, g_aBoss.pos, D3DXVECTOR3(sinf(g_aBoss.fVaim + 0.1f) * 10.0f, 0.0f, cosf(g_aBoss.fVaim + 0.1f) * 10.0f), g_aBoss.rot, D3DXVECTOR3(2.0f, 2.0f, 2.0f), 0.0f, 0.0f, 0.0f, NULL_VECTOR3, ATTACKMODELBEHAVIOR00_NORMAL);
		SetAttackModel(ATTACKMODEL04_SHOCKWAVE, 400, g_aBoss.pos, D3DXVECTOR3(sinf(g_aBoss.fVaim - 0.1f) * 10.0f, 0.0f, cosf(g_aBoss.fVaim - 0.1f) * 10.0f), g_aBoss.rot, D3DXVECTOR3(2.0f, 2.0f, 2.0f), 0.0f, 0.0f, 0.0f, NULL_VECTOR3, ATTACKMODELBEHAVIOR00_NORMAL);
		SetAttackModel(ATTACKMODEL04_SHOCKWAVE, 400, g_aBoss.pos, D3DXVECTOR3(sinf(g_aBoss.fVaim + 0.3f) * 8.0f, 0.0f, cosf(g_aBoss.fVaim + 0.3f) * 8.0f), g_aBoss.rot, D3DXVECTOR3(2.0f, 2.0f, 2.0f), 0.0f, 0.0f, 0.0f, NULL_VECTOR3, ATTACKMODELBEHAVIOR00_NORMAL);
		SetAttackModel(ATTACKMODEL04_SHOCKWAVE, 400, g_aBoss.pos, D3DXVECTOR3(sinf(g_aBoss.fVaim - 0.3f) * 8.0f, 0.0f, cosf(g_aBoss.fVaim - 0.3f) * 8.0f), g_aBoss.rot, D3DXVECTOR3(2.0f, 2.0f, 2.0f), 0.0f, 0.0f, 0.0f, NULL_VECTOR3, ATTACKMODELBEHAVIOR00_NORMAL);
		SetAttackModel(ATTACKMODEL04_SHOCKWAVE, 400, g_aBoss.pos, D3DXVECTOR3(sinf(g_aBoss.fVaim + 0.5f) * 6.0f, 0.0f, cosf(g_aBoss.fVaim + 0.5f) * 6.0f), g_aBoss.rot, D3DXVECTOR3(2.0f, 2.0f, 2.0f), 0.0f, 0.0f, 0.0f, NULL_VECTOR3, ATTACKMODELBEHAVIOR00_NORMAL);
		SetAttackModel(ATTACKMODEL04_SHOCKWAVE, 400, g_aBoss.pos, D3DXVECTOR3(sinf(g_aBoss.fVaim - 0.5f) * 6.0f, 0.0f, cosf(g_aBoss.fVaim - 0.5f) * 6.0f), g_aBoss.rot, D3DXVECTOR3(2.0f, 2.0f, 2.0f), 0.0f, 0.0f, 0.0f, NULL_VECTOR3, ATTACKMODELBEHAVIOR00_NORMAL);
	}

	if (g_aBoss.nCntAttackTime == 360)
	{
		g_aBoss.state = BOSSSTATE_ATTACK;
		g_aBoss.rot.y = g_aBoss.fVaim + D3DX_PI;

		PlaySound(SOUND_LABEL_SE_SHOCKWAVE_000);

		SetAttackModel(ATTACKMODEL04_SHOCKWAVE, 400, g_aBoss.pos, D3DXVECTOR3(sinf(g_aBoss.fVaim) * 12.0f, 0.0f, cosf(g_aBoss.fVaim) * 12.0f), D3DXVECTOR3(g_aBoss.rot.x, g_aBoss.rot.y, g_aBoss.rot.z), D3DXVECTOR3(2.0f, 2.0f, 2.0f), 0.0f, 0.0f, 20.0f, NULL_VECTOR3, ATTACKMODELBEHAVIOR01_ANOTHER);
		SetAttackModel(ATTACKMODEL04_SHOCKWAVE, 400, g_aBoss.pos, D3DXVECTOR3(sinf(g_aBoss.fVaim + 0.2f) * 10.0f, 0.0f, cosf(g_aBoss.fVaim + 0.2f) * 10.0f), D3DXVECTOR3(g_aBoss.rot.x, g_aBoss.rot.y + 0.2f, g_aBoss.rot.z), D3DXVECTOR3(2.0f, 2.0f, 2.0f), 0.0f, 0.0f, 16.0f, NULL_VECTOR3, ATTACKMODELBEHAVIOR01_ANOTHER);
		SetAttackModel(ATTACKMODEL04_SHOCKWAVE, 400, g_aBoss.pos, D3DXVECTOR3(sinf(g_aBoss.fVaim - 0.2f) * 10.0f, 0.0f, cosf(g_aBoss.fVaim - 0.2f) * 10.0f), D3DXVECTOR3(g_aBoss.rot.x, g_aBoss.rot.y - 0.2f, g_aBoss.rot.z), D3DXVECTOR3(2.0f, 2.0f, 2.0f), 0.0f, 0.0f, 16.0f, NULL_VECTOR3, ATTACKMODELBEHAVIOR01_ANOTHER);
		SetAttackModel(ATTACKMODEL04_SHOCKWAVE, 400, g_aBoss.pos, D3DXVECTOR3(sinf(g_aBoss.fVaim + 0.4f) * 8.0f, 0.0f, cosf(g_aBoss.fVaim + 0.4f) * 8.0f), D3DXVECTOR3(g_aBoss.rot.x, g_aBoss.rot.y + 0.4f, g_aBoss.rot.z), D3DXVECTOR3(2.0f, 2.0f, 2.0f), 0.0f, 0.0f, 12.0f, NULL_VECTOR3, ATTACKMODELBEHAVIOR01_ANOTHER);
		SetAttackModel(ATTACKMODEL04_SHOCKWAVE, 400, g_aBoss.pos, D3DXVECTOR3(sinf(g_aBoss.fVaim - 0.4f) * 8.0f, 0.0f, cosf(g_aBoss.fVaim - 0.4f) * 8.0f), D3DXVECTOR3(g_aBoss.rot.x, g_aBoss.rot.y - 0.4f, g_aBoss.rot.z), D3DXVECTOR3(2.0f, 2.0f, 2.0f), 0.0f, 0.0f, 12.0f, NULL_VECTOR3, ATTACKMODELBEHAVIOR01_ANOTHER);
		SetAttackModel(ATTACKMODEL04_SHOCKWAVE, 400, g_aBoss.pos, D3DXVECTOR3(sinf(g_aBoss.fVaim + 0.6f) * 6.0f, 0.0f, cosf(g_aBoss.fVaim + 0.6f) * 6.0f), D3DXVECTOR3(g_aBoss.rot.x, g_aBoss.rot.y + 0.6f, g_aBoss.rot.z), D3DXVECTOR3(2.0f, 2.0f, 2.0f), 0.0f, 0.0f, 8.0f, NULL_VECTOR3, ATTACKMODELBEHAVIOR01_ANOTHER);
		SetAttackModel(ATTACKMODEL04_SHOCKWAVE, 400, g_aBoss.pos, D3DXVECTOR3(sinf(g_aBoss.fVaim - 0.6f) * 6.0f, 0.0f, cosf(g_aBoss.fVaim - 0.6f) * 6.0f), D3DXVECTOR3(g_aBoss.rot.x, g_aBoss.rot.y - 0.6f, g_aBoss.rot.z), D3DXVECTOR3(2.0f, 2.0f, 2.0f), 0.0f, 0.0f, 8.0f, NULL_VECTOR3, ATTACKMODELBEHAVIOR01_ANOTHER);
	}

	if (g_aBoss.nCntAttackTime == 450)
	{
		g_aBoss.state = BOSSSTATE_ATTACK;
		g_aBoss.rot.y = g_aBoss.fVaim + D3DX_PI;

		PlaySound(SOUND_LABEL_SE_SHOCKWAVE_000);

		SetAttackModel(ATTACKMODEL04_SHOCKWAVE, 400, g_aBoss.pos, D3DXVECTOR3(sinf(g_aBoss.fVaim) * 12.0f, 0.0f, cosf(g_aBoss.fVaim) * 12.0f), D3DXVECTOR3(g_aBoss.rot.x, g_aBoss.rot.y, g_aBoss.rot.z), D3DXVECTOR3(2.0f, 2.0f, 2.0f), 0.0f, 0.0f, 20.0f, NULL_VECTOR3, ATTACKMODELBEHAVIOR00_NORMAL);
		SetAttackModel(ATTACKMODEL04_SHOCKWAVE, 400, g_aBoss.pos, D3DXVECTOR3(sinf(g_aBoss.fVaim + 0.2f) * 12.0f, 0.0f, cosf(g_aBoss.fVaim + 0.2f) * 12.0f), D3DXVECTOR3(g_aBoss.rot.x, g_aBoss.rot.y + 0.2f, g_aBoss.rot.z), D3DXVECTOR3(2.0f, 2.0f, 2.0f), 0.0f, 0.0f, 20.0f, NULL_VECTOR3, ATTACKMODELBEHAVIOR01_ANOTHER);
		SetAttackModel(ATTACKMODEL04_SHOCKWAVE, 400, g_aBoss.pos, D3DXVECTOR3(sinf(g_aBoss.fVaim - 0.2f) * 12.0f, 0.0f, cosf(g_aBoss.fVaim - 0.2f) * 12.0f), D3DXVECTOR3(g_aBoss.rot.x, g_aBoss.rot.y - 0.2f, g_aBoss.rot.z), D3DXVECTOR3(2.0f, 2.0f, 2.0f), 0.0f, 0.0f, 20.0f, NULL_VECTOR3, ATTACKMODELBEHAVIOR01_ANOTHER);
		SetAttackModel(ATTACKMODEL04_SHOCKWAVE, 400, g_aBoss.pos, D3DXVECTOR3(sinf(g_aBoss.fVaim + 0.4f) * 12.0f, 0.0f, cosf(g_aBoss.fVaim + 0.4f) * 12.0f), D3DXVECTOR3(g_aBoss.rot.x, g_aBoss.rot.y + 0.4f, g_aBoss.rot.z), D3DXVECTOR3(2.0f, 2.0f, 2.0f), 0.0f, 0.0f, 20.0f, NULL_VECTOR3, ATTACKMODELBEHAVIOR00_NORMAL);
		SetAttackModel(ATTACKMODEL04_SHOCKWAVE, 400, g_aBoss.pos, D3DXVECTOR3(sinf(g_aBoss.fVaim - 0.4f) * 12.0f, 0.0f, cosf(g_aBoss.fVaim - 0.4f) * 12.0f), D3DXVECTOR3(g_aBoss.rot.x, g_aBoss.rot.y - 0.4f, g_aBoss.rot.z), D3DXVECTOR3(2.0f, 2.0f, 2.0f), 0.0f, 0.0f, 20.0f, NULL_VECTOR3, ATTACKMODELBEHAVIOR00_NORMAL);
		SetAttackModel(ATTACKMODEL04_SHOCKWAVE, 400, g_aBoss.pos, D3DXVECTOR3(sinf(g_aBoss.fVaim + 0.6f) * 12.0f, 0.0f, cosf(g_aBoss.fVaim + 0.6f) * 12.0f), D3DXVECTOR3(g_aBoss.rot.x, g_aBoss.rot.y + 0.6f, g_aBoss.rot.z), D3DXVECTOR3(2.0f, 2.0f, 2.0f), 0.0f, 0.0f, 20.0f, NULL_VECTOR3, ATTACKMODELBEHAVIOR01_ANOTHER);
		SetAttackModel(ATTACKMODEL04_SHOCKWAVE, 400, g_aBoss.pos, D3DXVECTOR3(sinf(g_aBoss.fVaim - 0.6f) * 12.0f, 0.0f, cosf(g_aBoss.fVaim - 0.6f) * 12.0f), D3DXVECTOR3(g_aBoss.rot.x, g_aBoss.rot.y - 0.6f, g_aBoss.rot.z), D3DXVECTOR3(2.0f, 2.0f, 2.0f), 0.0f, 0.0f, 20.0f, NULL_VECTOR3, ATTACKMODELBEHAVIOR01_ANOTHER);
	}

	if (g_aBoss.nCntAttackTime >= 530 && g_aBoss.nCntAttackTime <= 560)
	{
		g_aBoss.state = BOSSSTATE_MOVE;
		g_aBoss.move.x = sinf(g_aBoss.fVaim) * 20.0f;
		g_aBoss.move.z = cosf(g_aBoss.fVaim) * 20.0f;
	}

	if (g_aBoss.nCntAttackTime == 530)
	{
		g_aBoss.move.y = 50.0f;
	}

	if (g_aBoss.nCntAttackTime >= 531 && g_aBoss.nCntAttackTime <= 700 && g_aBoss.pos.y <= 0.0f)
	{
		g_bPurposeFlag[0] = true;
	}

	if (g_bPurposeFlag[0] == true)
	{
		g_nPurposeCnt++;
	}

	if (g_nPurposeCnt == 50)
	{
		PlaySound(SOUND_LABEL_SE_BIRDSTRIKE_000);
		
		SetAttackModel(ATTACKMODEL02_SPHERE, 100, D3DXVECTOR3(sinf(D3DX_PI * 0.0f) * 200.0f + g_aBoss.pos.x, 0.0f, cosf(D3DX_PI * 0.0f) * 200.0f + g_aBoss.pos.z), NULL_VECTOR3, NULL_VECTOR3,
			NULL_VECTOR3, 0.0f, 0.0f, 0.1f, NULL_VECTOR3, ATTACKMODELBEHAVIOR00_NORMAL);
		SetAttackModel(ATTACKMODEL02_SPHERE, 100, D3DXVECTOR3(sinf(D3DX_PI * 0.5f) * 200.0f + g_aBoss.pos.x, 0.0f, cosf(D3DX_PI * 0.5f) * 200.0f + g_aBoss.pos.z), NULL_VECTOR3, NULL_VECTOR3,
			NULL_VECTOR3, 0.0f, 0.0f, 0.1f, NULL_VECTOR3, ATTACKMODELBEHAVIOR00_NORMAL);
		SetAttackModel(ATTACKMODEL02_SPHERE, 100, D3DXVECTOR3(sinf(D3DX_PI * 1.0f) * 200.0f + g_aBoss.pos.x, 0.0f, cosf(D3DX_PI * 1.0f) * 200.0f + g_aBoss.pos.z), NULL_VECTOR3, NULL_VECTOR3,
			NULL_VECTOR3, 0.0f, 0.0f, 0.1f, NULL_VECTOR3, ATTACKMODELBEHAVIOR00_NORMAL);
		SetAttackModel(ATTACKMODEL02_SPHERE, 100, D3DXVECTOR3(sinf(D3DX_PI * 1.5f) * 200.0f + g_aBoss.pos.x, 0.0f, cosf(D3DX_PI * 1.5f) * 200.0f + g_aBoss.pos.z), NULL_VECTOR3, NULL_VECTOR3,
			NULL_VECTOR3, 0.0f, 0.0f, 0.1f, NULL_VECTOR3, ATTACKMODELBEHAVIOR00_NORMAL);
	}
	if (g_nPurposeCnt == 120)
	{
		PlaySound(SOUND_LABEL_SE_BIRDSTRIKE_000);

		SetAttackModel(ATTACKMODEL02_SPHERE, 100, D3DXVECTOR3(sinf(D3DX_PI * 0.25f) * 400.0f + g_aBoss.pos.x, 0.0f, cosf(D3DX_PI * 0.25f) * 400.0f + g_aBoss.pos.z), NULL_VECTOR3, NULL_VECTOR3,
			NULL_VECTOR3, 0.0f, 0.0f, 0.2f, NULL_VECTOR3, ATTACKMODELBEHAVIOR00_NORMAL);
		SetAttackModel(ATTACKMODEL02_SPHERE, 100, D3DXVECTOR3(sinf(D3DX_PI * 0.75f) * 400.0f + g_aBoss.pos.x, 0.0f, cosf(D3DX_PI * 0.75f) * 400.0f + g_aBoss.pos.z), NULL_VECTOR3, NULL_VECTOR3,
			NULL_VECTOR3, 0.0f, 0.0f, 0.2f, NULL_VECTOR3, ATTACKMODELBEHAVIOR00_NORMAL);
		SetAttackModel(ATTACKMODEL02_SPHERE, 100, D3DXVECTOR3(sinf(D3DX_PI * 1.25f) * 400.0f + g_aBoss.pos.x, 0.0f, cosf(D3DX_PI * 1.25f) * 400.0f + g_aBoss.pos.z), NULL_VECTOR3, NULL_VECTOR3,
			NULL_VECTOR3, 0.0f, 0.0f, 0.2f, NULL_VECTOR3, ATTACKMODELBEHAVIOR00_NORMAL);
		SetAttackModel(ATTACKMODEL02_SPHERE, 100, D3DXVECTOR3(sinf(D3DX_PI * 1.75f) * 400.0f + g_aBoss.pos.x, 0.0f, cosf(D3DX_PI * 1.75f) * 400.0f + g_aBoss.pos.z), NULL_VECTOR3, NULL_VECTOR3,
			NULL_VECTOR3, 0.0f, 0.0f, 0.2f, NULL_VECTOR3, ATTACKMODELBEHAVIOR00_NORMAL);
	}
	if (g_nPurposeCnt == 190)
	{
		PlaySound(SOUND_LABEL_SE_BIRDSTRIKE_000);

		SetAttackModel(ATTACKMODEL02_SPHERE, 100, D3DXVECTOR3(sinf(D3DX_PI * 0.0f) * 600.0f + g_aBoss.pos.x, 0.0f, cosf(D3DX_PI * 0.0f) * 600.0f + g_aBoss.pos.z), NULL_VECTOR3, NULL_VECTOR3,
			NULL_VECTOR3, 0.0f, 0.0f, 0.3f, NULL_VECTOR3, ATTACKMODELBEHAVIOR00_NORMAL);
		SetAttackModel(ATTACKMODEL02_SPHERE, 100, D3DXVECTOR3(sinf(D3DX_PI * 0.5f) * 600.0f + g_aBoss.pos.x, 0.0f, cosf(D3DX_PI * 0.5f) * 600.0f + g_aBoss.pos.z), NULL_VECTOR3, NULL_VECTOR3,
			NULL_VECTOR3, 0.0f, 0.0f, 0.3f, NULL_VECTOR3, ATTACKMODELBEHAVIOR00_NORMAL);
		SetAttackModel(ATTACKMODEL02_SPHERE, 100, D3DXVECTOR3(sinf(D3DX_PI * 1.0f) * 600.0f + g_aBoss.pos.x, 0.0f, cosf(D3DX_PI * 1.0f) * 600.0f + g_aBoss.pos.z), NULL_VECTOR3, NULL_VECTOR3,
			NULL_VECTOR3, 0.0f, 0.0f, 0.3f, NULL_VECTOR3, ATTACKMODELBEHAVIOR00_NORMAL);
		SetAttackModel(ATTACKMODEL02_SPHERE, 100, D3DXVECTOR3(sinf(D3DX_PI * 1.5f) * 600.0f + g_aBoss.pos.x, 0.0f, cosf(D3DX_PI * 1.5f) * 600.0f + g_aBoss.pos.z), NULL_VECTOR3, NULL_VECTOR3,
			NULL_VECTOR3, 0.0f, 0.0f, 0.3f, NULL_VECTOR3, ATTACKMODELBEHAVIOR00_NORMAL);
	}
	if (g_nPurposeCnt == 260)
	{
		PlaySound(SOUND_LABEL_SE_BIRDSTRIKE_000);

		SetAttackModel(ATTACKMODEL02_SPHERE, 100, D3DXVECTOR3(sinf(D3DX_PI * 0.25f) * 800.0f + g_aBoss.pos.x, 0.0f, cosf(D3DX_PI * 0.25f) * 800.0f + g_aBoss.pos.z), NULL_VECTOR3, NULL_VECTOR3,
			NULL_VECTOR3, 0.0f, 0.0f, 0.4f, NULL_VECTOR3, ATTACKMODELBEHAVIOR00_NORMAL);
		SetAttackModel(ATTACKMODEL02_SPHERE, 100, D3DXVECTOR3(sinf(D3DX_PI * 0.75f) * 800.0f + g_aBoss.pos.x, 0.0f, cosf(D3DX_PI * 0.75f) * 800.0f + g_aBoss.pos.z), NULL_VECTOR3, NULL_VECTOR3,
			NULL_VECTOR3, 0.0f, 0.0f, 0.4f, NULL_VECTOR3, ATTACKMODELBEHAVIOR00_NORMAL);
		SetAttackModel(ATTACKMODEL02_SPHERE, 100, D3DXVECTOR3(sinf(D3DX_PI * 1.25f) * 800.0f + g_aBoss.pos.x, 0.0f, cosf(D3DX_PI * 1.25f) * 800.0f + g_aBoss.pos.z), NULL_VECTOR3, NULL_VECTOR3,
			NULL_VECTOR3, 0.0f, 0.0f, 0.4f, NULL_VECTOR3, ATTACKMODELBEHAVIOR00_NORMAL);
		SetAttackModel(ATTACKMODEL02_SPHERE, 100, D3DXVECTOR3(sinf(D3DX_PI * 1.75f) * 800.0f + g_aBoss.pos.x, 0.0f, cosf(D3DX_PI * 1.75f) * 800.0f + g_aBoss.pos.z), NULL_VECTOR3, NULL_VECTOR3,
			NULL_VECTOR3, 0.0f, 0.0f, 0.4f, NULL_VECTOR3, ATTACKMODELBEHAVIOR00_NORMAL);
	}

	if (g_aBoss.nCntAttackTime == 1100)
	{
		g_bPurposeFlag[0] = false;//�ėp�I�Ɏg���t���O
		g_aBoss.bUnderAttack = false;//�U�����Ă��邩�ǂ���
		g_aBoss.nCntAttackTime = 0;
		g_nPurposeCnt = 0;//�ėp�J�E���g
	}

	g_aBoss.move.x += (0.0f - g_aBoss.move.x) * 0.1f;
	g_aBoss.move.z += (0.0f - g_aBoss.move.z) * 0.1f;
}

//============================================================================================================================================================================================================
//�{�X�U���V�F�X�����_�[�g���l�[�h
//============================================================================================================================================================================================================
void BossAttack7_SlanderTornado(void)
{
	Player* pPlayer = GetPlayer();	//�v���C���[�̏����擾
	int nLottery = rand() % 2;

	g_aBoss.nCntAttackTime++;

	if (g_aBoss.nCntAttackTime == 1)
	{
		g_aBoss.move = NULL_VECTOR3;//�ړ��ʂ��O��
		g_bPurposeFlag[0] = false;//�ėp�I�Ɏg���t���O�O
		g_bPurposeFlag[1] = false;//�ėp�I�Ɏg���t���O�P
		g_bPurposeFlag[2] = false;//�ėp�I�Ɏg���t���O�Q
 		g_nPurposeCnt = 0;//�ėp�J�E���g
	}

	if (g_aBoss.nCntAttackTime == 100)
	{
		g_aBoss.state = BOSSSTATE_ATTACK;

		PlaySound(SOUND_LABEL_SE_WIND_000);

		for (int nCntAttack = 0; nCntAttack < 8; nCntAttack++)
		{
			float fRot = (2.0f / 8.0f) * float(nCntAttack);
			SetAttackModel(ATTACKMODEL05_TORNADO, 300, g_aBoss.pos, NULL_VECTOR3, NULL_VECTOR3, D3DXVECTOR3(2.0f, 2.0f, 2.0f),
				fRot,0.002f * float(nCntAttack),0.1f, g_aBoss.pos, ATTACKMODELBEHAVIOR00_NORMAL);
		}
	}

	if (g_aBoss.nCntAttackTime == 200)
	{
		if(nLottery == 0)
		{
			g_bPurposeFlag[0] = false;
		}
		else
		{
			g_bPurposeFlag[0] = true;
		}
	}

	float fXVaim = (pPlayer->pos.x + 500.0f) - g_aBoss.pos.x;
	float fZVaim = pPlayer->pos.z - g_aBoss.pos.z;
	float fVaim = atan2f(fXVaim, fZVaim);

	float fXVaim2 = (pPlayer->pos.x - 500.0f) - g_aBoss.pos.x;
	float fZVaim2 = pPlayer->pos.z - g_aBoss.pos.z;
	float fVaim2 = atan2f(fXVaim2, fZVaim2);

	if (g_aBoss.nCntAttackTime >= 200 && g_aBoss.nCntAttackTime <= 400)
	{
		g_aBoss.state = BOSSSTATE_MOVE;

		if (g_bPurposeFlag[0] == false)
		{
		
			g_aBoss.move.x = sinf(fVaim) * 30.0f;
			g_aBoss.move.z = cosf(fVaim) * 30.0f;
		}
		else
		{
			g_aBoss.move.x = sinf(fVaim2) * 30.0f;
			g_aBoss.move.z = cosf(fVaim2) * 30.0f;
		}
	}

	if (g_aBoss.nCntAttackTime >= 350 && g_aBoss.nCntAttackTime <= 400)
	{
		if (g_bPurposeFlag[0] == false)
		{
			g_aBoss.rot.y = g_aBoss.fVaim + D3DX_PI;
			g_aBoss.state = BOSSSTATE_ATTACK;

			if (g_aBoss.pos.x >= pPlayer->pos.x + 500.0f - 50.0f && g_aBoss.pos.x <= pPlayer->pos.x + 500.0f + 50.0f &&
				g_aBoss.pos.z >= pPlayer->pos.z - 50.0f && g_aBoss.pos.z <= pPlayer->pos.z + 50.0f)
			{
				g_aBoss.move = NULL_VECTOR3;
				g_bPurposeFlag[1] = true;
			}

			if (g_bPurposeFlag[1] == true && g_bPurposeFlag[2] == false)
			{
				PlaySound(SOUND_LABEL_SE_WIND_000);


				SetAttackModel(ATTACKMODEL05_TORNADO, 300, D3DXVECTOR3(g_aBoss.pos.x, g_aBoss.pos.y, g_aBoss.pos.z),
					D3DXVECTOR3(sinf(D3DX_PI * 1.5f) * 4.0f, 0.0f, cosf(D3DX_PI * 1.5f) * 4.0f), NULL_VECTOR3, D3DXVECTOR3(2.0f,1.0f, 1.0f),
					0.0f, 0.0f, 0.1f, g_aBoss.pos, ATTACKMODELBEHAVIOR01_ANOTHER);

				for (int nCntAttack = 0; nCntAttack < 8; nCntAttack++)
				{
					if (nCntAttack % 3 == 0)
					{
						SetAttackModel(ATTACKMODEL05_TORNADO, 300, D3DXVECTOR3(g_aBoss.pos.x, g_aBoss.pos.y, g_aBoss.pos.z + 100.0f + (100.0f * nCntAttack)),
							D3DXVECTOR3(sinf(D3DX_PI * 1.5f) * 4.0f, 0.0f, cosf(D3DX_PI * 1.5f) * 4.0f), NULL_VECTOR3, D3DXVECTOR3(2.0f, 1.0f, 1.0f),
							0.0f, 0.002f * float(nCntAttack), 0.1f, g_aBoss.pos, ATTACKMODELBEHAVIOR01_ANOTHER);
					}
					else
					{
						SetAttackModel(ATTACKMODEL05_TORNADO, 300, D3DXVECTOR3(g_aBoss.pos.x, g_aBoss.pos.y, g_aBoss.pos.z - 100.0f - (100.0f * nCntAttack)),
							D3DXVECTOR3(sinf(D3DX_PI * 1.5f) * 4.0f, 0.0f, cosf(D3DX_PI * 1.5f) * 4.0f), NULL_VECTOR3, D3DXVECTOR3(2.0f, 1.0f, 1.0f),
							0.0f, 0.002f * float(nCntAttack), 0.1f, g_aBoss.pos, ATTACKMODELBEHAVIOR01_ANOTHER);
					}
				}

				g_bPurposeFlag[2] = true;
			}
		}
		else
		{
			g_aBoss.rot.y = g_aBoss.fVaim + D3DX_PI;

			g_aBoss.state = BOSSSTATE_ATTACK;

			if (g_aBoss.pos.x >= pPlayer->pos.x - 500.0f - 50.0f && g_aBoss.pos.x <= pPlayer->pos.x - 500.0f + 50.0f &&
				g_aBoss.pos.z >= pPlayer->pos.z - 50.0f && g_aBoss.pos.z <= pPlayer->pos.z + 50.0f)
			{
				g_aBoss.move = NULL_VECTOR3;
				g_bPurposeFlag[1] = true;
			}

			if (g_bPurposeFlag[1] == true && g_bPurposeFlag[2] == false)
			{
				PlaySound(SOUND_LABEL_SE_WIND_000);

				SetAttackModel(ATTACKMODEL05_TORNADO, 300, D3DXVECTOR3(g_aBoss.pos.x, g_aBoss.pos.y, g_aBoss.pos.z),
					D3DXVECTOR3(sinf(D3DX_PI * 0.5f) * 4.0f, 0.0f, cosf(D3DX_PI * 0.5f) * 4.0f), NULL_VECTOR3, D3DXVECTOR3(2.0f, 1.0f, 1.0f),
					0.0f, 0.0f, 0.1f, g_aBoss.pos, ATTACKMODELBEHAVIOR01_ANOTHER);

				for (int nCntAttack = 0; nCntAttack < 8; nCntAttack++)
				{
					if (nCntAttack % 3 == 0)
					{
						SetAttackModel(ATTACKMODEL05_TORNADO, 300, D3DXVECTOR3(g_aBoss.pos.x, g_aBoss.pos.y, g_aBoss.pos.z + 100.0f + (100.0f * nCntAttack)),
							D3DXVECTOR3(sinf(D3DX_PI * 0.5f) * 4.0f, 0.0f, cosf(D3DX_PI * 0.5f) * 4.0f), NULL_VECTOR3, D3DXVECTOR3(2.0f, 1.0f, 1.0f),
							0.0f, 0.002f * float(nCntAttack), 0.1f, g_aBoss.pos, ATTACKMODELBEHAVIOR01_ANOTHER);
					}
					else
					{
						SetAttackModel(ATTACKMODEL05_TORNADO, 300, D3DXVECTOR3(g_aBoss.pos.x, g_aBoss.pos.y, g_aBoss.pos.z - 100.0f - (100.0f * nCntAttack)),
							D3DXVECTOR3(sinf(D3DX_PI * 0.5f) * 4.0f, 0.0f, cosf(D3DX_PI * 0.5f) * 4.0f), NULL_VECTOR3, D3DXVECTOR3(2.0f, 1.0f, 1.0f),
							0.0f, 0.002f * float(nCntAttack), 0.1f, g_aBoss.pos, ATTACKMODELBEHAVIOR01_ANOTHER);
					}
				}

				g_bPurposeFlag[2] = true;
			}
		}
	}

	if (g_aBoss.nCntAttackTime == 400)
	{
		g_bPurposeFlag[0] = false;//�ėp�I�Ɏg���t���O
		g_bPurposeFlag[1] = false;//�ėp�I�Ɏg���t���O�P
		g_bPurposeFlag[2] = false;//�ėp�I�Ɏg���t���O�Q
	}

	float fZVaim3 = (pPlayer->pos.z + 500.0f) - g_aBoss.pos.z;
	float fXVaim3 = pPlayer->pos.x - g_aBoss.pos.x;
	float fVaim3 = atan2f(fXVaim3, fZVaim3);

	float fZVaim4 = (pPlayer->pos.z - 500.0f) - g_aBoss.pos.z;
	float fXVaim4 = pPlayer->pos.x - g_aBoss.pos.x;
	float fVaim4 = atan2f(fXVaim4, fZVaim4);

	if (g_aBoss.nCntAttackTime == 470)
	{
		if (nLottery == 0)
		{
			g_bPurposeFlag[0] = false;
		}
		else
		{
			g_bPurposeFlag[0] = true;
		}
	}

	if (g_aBoss.nCntAttackTime >= 470 && g_aBoss.nCntAttackTime <= 670)
	{
		g_aBoss.state = BOSSSTATE_MOVE;

		if (g_bPurposeFlag[0] == false)
		{

			g_aBoss.move.x = sinf(fVaim3) * 40.0f;
			g_aBoss.move.z = cosf(fVaim3) * 40.0f;
		}
		else
		{
			g_aBoss.move.x = sinf(fVaim4) * 40.0f;
			g_aBoss.move.z = cosf(fVaim4) * 40.0f;
		}
	}

	if (g_aBoss.nCntAttackTime >= 470 && g_aBoss.nCntAttackTime <= 670)
	{
		if (g_bPurposeFlag[0] == false)
		{
			g_aBoss.rot.y = g_aBoss.fVaim + D3DX_PI;
			g_aBoss.state = BOSSSTATE_ATTACK;

			if (g_aBoss.pos.x >= pPlayer->pos.x - 50.0f && g_aBoss.pos.x <= pPlayer->pos.x + 50.0f &&
				g_aBoss.pos.z >= pPlayer->pos.z + 500.0f - 50.0f && g_aBoss.pos.z <= pPlayer->pos.z + 500.0f + 50.0f)
			{
				g_aBoss.move = NULL_VECTOR3;
				g_bPurposeFlag[1] = true;
			}

			if (g_bPurposeFlag[1] == true && g_bPurposeFlag[2] == false)
			{
				PlaySound(SOUND_LABEL_SE_WIND_000);

				SetAttackModel(ATTACKMODEL05_TORNADO, 300, D3DXVECTOR3(g_aBoss.pos.x, g_aBoss.pos.y, g_aBoss.pos.z),
					D3DXVECTOR3(sinf(D3DX_PI) * 4.0f, 0.0f, cosf(D3DX_PI) * 4.0f), NULL_VECTOR3, D3DXVECTOR3(2.0f, 1.0f, 1.0f),
					0.0f, 0.0f, 0.1f, g_aBoss.pos, ATTACKMODELBEHAVIOR01_ANOTHER);

				for (int nCntAttack = 0; nCntAttack < 8; nCntAttack++)
				{
					if (nCntAttack % 3 == 0)
					{
						SetAttackModel(ATTACKMODEL05_TORNADO, 300, D3DXVECTOR3(g_aBoss.pos.x + 100.0f + (100.0f * nCntAttack), g_aBoss.pos.y, g_aBoss.pos.z),
							D3DXVECTOR3(sinf(D3DX_PI + 0.2f) * 4.0f, 0.0f, cosf(D3DX_PI + 0.2f) * 4.0f), NULL_VECTOR3, D3DXVECTOR3(2.0f, 1.0f, 1.0f),
							0.0f, 0.002f * float(nCntAttack), 0.1f, g_aBoss.pos, ATTACKMODELBEHAVIOR01_ANOTHER);
					}
					else
					{
						SetAttackModel(ATTACKMODEL05_TORNADO, 300, D3DXVECTOR3(g_aBoss.pos.x - 100.0f - (100.0f * nCntAttack), g_aBoss.pos.y, g_aBoss.pos.z),
							D3DXVECTOR3(sinf(D3DX_PI - 0.2f) * 4.0f, 0.0f, cosf(D3DX_PI - 0.2f) * 4.0f), NULL_VECTOR3, D3DXVECTOR3(2.0f, 1.0f, 1.0f),
							0.0f, 0.002f * float(nCntAttack), 0.1f, g_aBoss.pos, ATTACKMODELBEHAVIOR01_ANOTHER);
					}
				}

				g_bPurposeFlag[2] = true;
			}
		}
		else
		{
			g_aBoss.rot.y = g_aBoss.fVaim + D3DX_PI;

			g_aBoss.state = BOSSSTATE_ATTACK;

			if (g_aBoss.pos.x >= pPlayer->pos.x - 50.0f && g_aBoss.pos.x <= pPlayer->pos.x + 50.0f &&
				g_aBoss.pos.z >= pPlayer->pos.z - 500.0f - 50.0f && g_aBoss.pos.z <= pPlayer->pos.z - 500.0f + 50.0f)
			{
				g_aBoss.move = NULL_VECTOR3;
				g_bPurposeFlag[1] = true;
			}

			if (g_bPurposeFlag[1] == true && g_bPurposeFlag[2] == false)
			{
				PlaySound(SOUND_LABEL_SE_WIND_000);

				SetAttackModel(ATTACKMODEL05_TORNADO, 300, D3DXVECTOR3(g_aBoss.pos.x, g_aBoss.pos.y, g_aBoss.pos.z),
					D3DXVECTOR3(sinf(0.0f) * 4.0f, 0.0f, cosf(0.0f) * 4.0f), NULL_VECTOR3, D3DXVECTOR3(2.0f, 1.0f, 1.0f),
					0.0f, 0.0f, 0.1f, g_aBoss.pos, ATTACKMODELBEHAVIOR01_ANOTHER);

				for (int nCntAttack = 0; nCntAttack < 8; nCntAttack++)
				{
					if (nCntAttack % 3 == 0)
					{
						SetAttackModel(ATTACKMODEL05_TORNADO, 300, D3DXVECTOR3(g_aBoss.pos.x + 100.0f + (100.0f * nCntAttack), g_aBoss.pos.y, g_aBoss.pos.z),
							D3DXVECTOR3(sinf(0.0f + 0.2f) * 4.0f, 0.0f, cosf(0.0f + 0.2f) * 4.0f), NULL_VECTOR3, D3DXVECTOR3(2.0f, 1.0f, 1.0f),
							0.0f, 0.002f * float(nCntAttack), 0.1f, g_aBoss.pos, ATTACKMODELBEHAVIOR01_ANOTHER);
					}
					else
					{
						SetAttackModel(ATTACKMODEL05_TORNADO, 300, D3DXVECTOR3(g_aBoss.pos.x - 100.0f - (100.0f * nCntAttack), g_aBoss.pos.y, g_aBoss.pos.z),
							D3DXVECTOR3(sinf(0.0f - 0.2f) * 4.0f, 0.0f, cosf(0.0f - 0.2f) * 4.0f), NULL_VECTOR3, D3DXVECTOR3(2.0f, 1.0f, 1.0f),
							0.0f, 0.002f * float(nCntAttack), 0.1f, g_aBoss.pos, ATTACKMODELBEHAVIOR01_ANOTHER);
					}
				}

				g_bPurposeFlag[2] = true;
			}
		}
	}

	if (g_aBoss.nCntAttackTime == 680)
	{
		g_bPurposeFlag[0] = false;//�ėp�I�Ɏg���t���O
		g_bPurposeFlag[1] = false;//�ėp�I�Ɏg���t���O�P
		g_bPurposeFlag[2] = false;//�ėp�I�Ɏg���t���O�Q

		g_aBoss.bUnderAttack = false;//�U�����Ă��邩�ǂ���
		g_aBoss.nCntAttackTime = 0;
		g_nPurposeCnt = 0;//�ėp�J�E���g
	}

	g_aBoss.move.x += (0.0f - g_aBoss.move.x) * 0.1f;
	g_aBoss.move.z += (0.0f - g_aBoss.move.z) * 0.1f;
}

//============================================================================================================================================================================================================
//�{�X�U���U�F�o�[�j���O�`�L��
//============================================================================================================================================================================================================
void BossAttack6_BurningChicken(void)//�{�X�U���U�F�o�[�j���O�`�L��
{
	g_aBoss.nCntAttackTime++;

	if (g_aBoss.nCntAttackTime == 1)
	{
		g_aBoss.move = NULL_VECTOR3;//�ړ��ʂ��O��
		g_bPurposeFlag[0] = false;//�ėp�I�Ɏg���t���O�O
		g_bPurposeFlag[1] = false;//�ėp�I�Ɏg���t���O�P
		g_bPurposeFlag[2] = false;//�ėp�I�Ɏg���t���O�Q
		g_nPurposeCnt = 0;//�ėp�J�E���g
		g_aBoss.state = BOSSSTATE_ATTACK;
	}

	if (g_aBoss.nCntAttackTime % 15 == 0 && g_aBoss.nCntAttackTime <= 200)
	{
		PlaySound(SOUND_LABEL_SE_FIRE_000);

		SetAttackbillboard(ATTACKBILLBORAD02_FIRE, 300, 40.0f, 100.0f,
			D3DXVECTOR3(sinf(g_aBoss.fVaim + 0.4f) * (100.0f * float(g_nPurposeCnt)) + g_aBoss.pos.x,50.0f, cosf(g_aBoss.fVaim + 0.4f) * (100.0f * float(g_nPurposeCnt)) + g_aBoss.pos.z), NULL_VECTOR3, NULL_VECTOR3, 0, 0.0f);

		SetAttackbillboard(ATTACKBILLBORAD02_FIRE, 300, 40.0f, 100.0f,
			D3DXVECTOR3(sinf(g_aBoss.fVaim - 0.4f) * (100.0f * float(g_nPurposeCnt)) + g_aBoss.pos.x, 50.0f, cosf(g_aBoss.fVaim - 0.4f) * (100.0f * float(g_nPurposeCnt)) + g_aBoss.pos.z), NULL_VECTOR3, NULL_VECTOR3, 0, 0.0f);

		g_nPurposeCnt++;
	}

	if (g_aBoss.nCntAttackTime >= 220 && g_aBoss.nCntAttackTime <= 320)
	{
		g_aBoss.rot.y = g_aBoss.fVaim + D3DX_PI;
		g_aBoss.state = BOSSSTATE_MOVE;
	}

	if (g_aBoss.nCntAttackTime >= 350 && g_aBoss.nCntAttackTime <= 500)
	{
		g_aBoss.move.x = sinf(g_aBoss.rot.y + D3DX_PI) * 15.0f;
		g_aBoss.move.z = cosf(g_aBoss.rot.y + D3DX_PI) * 15.0f;

		if (g_aBoss.nCntAttackTime % 40 == 0)
		{
			SetBullet(BULLETTYPE_FIRE, 300, 0.0f, 0.0f, g_aBoss.pos, D3DXVECTOR3(sinf(g_aBoss.rot.y + (D3DX_PI * 1.5f)) * 7.0f, 0.0f, cosf(g_aBoss.rot.y + (D3DX_PI * 1.5f)) * 7.0f),
				NULL_VECTOR3, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0, 0.0f);

			SetBullet(BULLETTYPE_FIRE, 300, 0.0f, 0.0f, g_aBoss.pos, D3DXVECTOR3(sinf(g_aBoss.rot.y + (D3DX_PI * 0.5f)) * 7.0f, 0.0f, cosf(g_aBoss.rot.y + (D3DX_PI * 0.5f)) * 7.0f),
				NULL_VECTOR3, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0, 0.0f);
		}
	}

	if (g_aBoss.nCntAttackTime == 500)
	{
		g_bPurposeFlag[0] = false;//�ėp�I�Ɏg���t���O
		g_bPurposeFlag[1] = false;//�ėp�I�Ɏg���t���O�P
		g_bPurposeFlag[2] = false;//�ėp�I�Ɏg���t���O�Q

		g_aBoss.bUnderAttack = false;//�U�����Ă��邩�ǂ���
		g_aBoss.nCntAttackTime = 0;
		g_nPurposeCnt = 0;//�ėp�J�E���g

		g_aBoss.move = NULL_VECTOR3;
	}
}

//=====================================
//�{�X�U���W�F�X�^�[�_�X�g���e�I
//=====================================
void BossAttack8_StardustMeteo(void)
{
	Player* pPlayer = GetPlayer();

	g_aBoss.nCntAttackTime++;

	int nLottery = rand() % 3;

	if (g_aBoss.nCntAttackTime == 1)
	{
		g_aBoss.move = NULL_VECTOR3;//�ړ��ʂ��O��
		g_bPurposeFlag[0] = false;//�ėp�I�Ɏg���t���O�O
		g_bPurposeFlag[1] = false;//�ėp�I�Ɏg���t���O�P
		g_bPurposeFlag[2] = false;//�ėp�I�Ɏg���t���O�Q
		g_nPurposeCnt = 0;//�ėp�J�E���g
		g_aBoss.state = BOSSSTATE_ATTACK;
	}

	float fRandRot = float(rand() % 200 + 1) / 100;
	float fRandLength = float(rand() % 700 + 1);

	if (g_aBoss.nCntAttackTime >= 50 && g_aBoss.nCntAttackTime <= 150)
	{
		g_aBoss.move.y = 10.0f;
	}

	if (g_aBoss.nCntAttackTime >= 150 && g_aBoss.nCntAttackTime <= 800)
	{
		g_aBoss.move = NULL_VECTOR3;

		if (g_aBoss.nCntAttackTime % 20 == 0)
		{
			PlaySound(SOUND_LABEL_SE_FALLINGMETEO_000);
			D3DXVECTOR3 MeteoMove = NULL_VECTOR3;
			if (nLottery == 0)
			{
				//==========================
				//�R�������@�_������
				//==========================
				g_aBoss.fVXaim3D = pPlayer->pos.x - g_aBoss.pos.x;
				g_aBoss.fVYaim3D = pPlayer->pos.y - g_aBoss.pos.y;
				g_aBoss.fVZaim3D = pPlayer->pos.z - g_aBoss.pos.z;


				g_aBoss.fVLaim3D = sqrtf((g_aBoss.fVXaim3D * g_aBoss.fVXaim3D) + (g_aBoss.fVYaim3D * g_aBoss.fVYaim3D)
					+ (g_aBoss.fVZaim3D * g_aBoss.fVZaim3D));

				MeteoMove.y = g_aBoss.fVYaim3D / g_aBoss.fVLaim3D * 20.0f;
				MeteoMove.x = g_aBoss.fVXaim3D / g_aBoss.fVLaim3D * 20.0f;
				MeteoMove.z = g_aBoss.fVZaim3D / g_aBoss.fVLaim3D * 20.0f;

				//===============================
				//���e�I����
				//===============================
				SetAttackModel(ATTACKMODEL07_METEO, 1000, g_aBoss.pos, MeteoMove, NULL_VECTOR3, D3DXVECTOR3(0.2f, 0.2f, 0.2f), 0.0f, 0.0f, 0.0f,
					pPlayer->pos, ATTACKMODELBEHAVIOR00_NORMAL);
			}
			else if (nLottery == 1)
			{
				//================================================================
				//�R�������@�_�������i�v���C���[�̌����Ă��������_��)
				//================================================================
				g_aBoss.fVXaim3D = (pPlayer->pos.x + sinf(pPlayer->rot.y + D3DX_PI) * 700.0f) - g_aBoss.pos.x;
				g_aBoss.fVYaim3D = pPlayer->pos.y - g_aBoss.pos.y;
				g_aBoss.fVZaim3D = (pPlayer->pos.z + cosf(pPlayer->rot.y + D3DX_PI) * 700.0f) - g_aBoss.pos.z;

				g_aBoss.fVLaim3D = sqrtf((g_aBoss.fVXaim3D * g_aBoss.fVXaim3D) + (g_aBoss.fVYaim3D * g_aBoss.fVYaim3D)
					+ (g_aBoss.fVZaim3D * g_aBoss.fVZaim3D));

				MeteoMove.x = g_aBoss.fVXaim3D / g_aBoss.fVLaim3D * 10.0f;
				MeteoMove.y = g_aBoss.fVYaim3D / g_aBoss.fVLaim3D * 10.0f;
				MeteoMove.z = g_aBoss.fVZaim3D / g_aBoss.fVLaim3D * 10.0f;

				//===============================
				//���e�I����
				//===============================
				SetAttackModel(ATTACKMODEL07_METEO, 1000, g_aBoss.pos, MeteoMove, NULL_VECTOR3, D3DXVECTOR3(0.4f, 0.4f, 0.4f), 0.0f, 0.0f, 0.0f,
					D3DXVECTOR3((pPlayer->pos.x + sinf(pPlayer->rot.y + D3DX_PI) * 700.0f),
						pPlayer->pos.y,
						(pPlayer->pos.z + cosf(pPlayer->rot.y + D3DX_PI) * 700.0f)), ATTACKMODELBEHAVIOR00_NORMAL);
			}
			else if (nLottery == 2)
			{

				//================================================================
				//�R�������@�_�������i�v���C���[�̌����Ă��������_��)
				//================================================================
				g_aBoss.fVXaim3D = (pPlayer->pos.x + sinf(D3DX_PI * fRandRot) * fRandLength) - g_aBoss.pos.x;
				g_aBoss.fVYaim3D = pPlayer->pos.y - g_aBoss.pos.y;
				g_aBoss.fVZaim3D = (pPlayer->pos.z + cosf(D3DX_PI * fRandRot) * fRandLength) - g_aBoss.pos.z;

				g_aBoss.fVLaim3D = sqrtf((g_aBoss.fVXaim3D * g_aBoss.fVXaim3D) + (g_aBoss.fVYaim3D * g_aBoss.fVYaim3D)
					+ (g_aBoss.fVZaim3D * g_aBoss.fVZaim3D));

				MeteoMove.x = g_aBoss.fVXaim3D / g_aBoss.fVLaim3D * 15.0f;
				MeteoMove.y = g_aBoss.fVYaim3D / g_aBoss.fVLaim3D * 15.0f;
				MeteoMove.z = g_aBoss.fVZaim3D / g_aBoss.fVLaim3D * 15.0f;

				//===============================
				//���e�I����
				//===============================
				SetAttackModel(ATTACKMODEL07_METEO, 1000, g_aBoss.pos, MeteoMove, NULL_VECTOR3, D3DXVECTOR3(0.2f, 0.2f, 0.2f), 0.0f, 0.0f, 0.0f,
					D3DXVECTOR3((pPlayer->pos.x + sinf(D3DX_PI * fRandRot) * fRandLength),
						pPlayer->pos.y,
						(pPlayer->pos.z + cosf(D3DX_PI * fRandRot) * fRandLength)), ATTACKMODELBEHAVIOR00_NORMAL);
			}
		}
	}

	if (g_aBoss.nCntAttackTime == 900)
	{
		g_bPurposeFlag[0] = false;//�ėp�I�Ɏg���t���O
		g_bPurposeFlag[1] = false;//�ėp�I�Ɏg���t���O�P
		g_bPurposeFlag[2] = false;//�ėp�I�Ɏg���t���O�Q

		g_aBoss.bUnderAttack = false;//�U�����Ă��邩�ǂ���
		g_aBoss.nCntAttackTime = 0;
		g_nPurposeCnt = 0;//�ėp�J�E���g

		g_aBoss.move = NULL_VECTOR3;
	}

}

//====================================
//�{�X�U���X�F�R���v�X���[�U�[
//====================================
void BossAttack9_CollapseLaser(void)
{
	g_aBoss.nCntAttackTime++;

	if (g_aBoss.nCntAttackTime == 1)
	{
		g_aBoss.move = NULL_VECTOR3;//�ړ��ʂ��O��
		g_bPurposeFlag[0] = false;//�ėp�I�Ɏg���t���O�O
		g_bPurposeFlag[1] = false;//�ėp�I�Ɏg���t���O�P
		g_bPurposeFlag[2] = false;//�ėp�I�Ɏg���t���O�Q
		g_nPurposeCnt = 0;//�ėp�J�E���g
		g_aBoss.state = BOSSSTATE_ATTACK;
		g_aBoss.fSaveAim = g_aBoss.rot.y - D3DX_PI;
		g_aBoss.bCurveLaser = false;//���[�U�[���J�[�u��������������߂�
		g_aBoss.fRotMove = 0.0f;//��]����ړ���

		PlaySound(SOUND_LABEL_SE_POWERCHARGE_000);
	}
	
	if (g_aBoss.nCntAttackTime >= 2 && g_aBoss.nCntAttackTime <= 250)
	{
		float fRandPosX = float(rand() % 1500 - 750) + g_aBoss.pos.x;
		float fRandPosY = float(rand() % 1500 - 750) + g_aBoss.pos.y;
		float fRandPosZ = float(rand() % 1500 - 750) + g_aBoss.pos.z;

		float fColR = float(rand() % 100 + 1) / 100.0f;
		float fColG = float(rand() % 100 + 1) / 100.0f;
		float fColB = float(rand() % 100 + 1) / 100.0f;
		D3DXVECTOR3 BulletMove = NULL_VECTOR3;

		g_aBoss.fVXaim3D = g_aBoss.pos.x - fRandPosX;
		g_aBoss.fVYaim3D = g_aBoss.pos.y - fRandPosY;
		g_aBoss.fVZaim3D = g_aBoss.pos.z - fRandPosZ;

		g_aBoss.fVLaim3D = sqrtf((g_aBoss.fVXaim3D * g_aBoss.fVXaim3D) + (g_aBoss.fVYaim3D * g_aBoss.fVYaim3D)
			+ (g_aBoss.fVZaim3D * g_aBoss.fVZaim3D));


		BulletMove.x = g_aBoss.fVXaim3D / g_aBoss.fVLaim3D * 10.0f;
		BulletMove.y = g_aBoss.fVYaim3D / g_aBoss.fVLaim3D * 10.0f;
		BulletMove.z = g_aBoss.fVZaim3D / g_aBoss.fVLaim3D * 10.0f;

		SetEmitterEffect(EMITTERTYPE_GATHERING, 0, 300, 30, 30.0f, 0.6f, D3DXVECTOR3(fRandPosX, fRandPosY, fRandPosZ), BulletMove, D3DXCOLOR(fColR, fColG, fColB, 1.0f),
			0.0f, 0, 0.0f, D3DXVECTOR3(g_aBoss.pos.x, g_aBoss.pos.y + g_aBoss.Size.y / 2.0f, g_aBoss.pos.z));


	}
	

	if (g_aBoss.nCntAttackTime == 300)
	{
		StopSound(SOUND_LABEL_SE_POWERCHARGE_000);
		PlaySound(SOUND_LABEL_SE_RASER_000);
		SetAttackModel(ATTACKMODEL09_LASER, 750, g_aBoss.aParts[1].PartsPos, NULL_VECTOR3, D3DXVECTOR3(0.0f,g_aBoss.fSaveAim, 0.0f), D3DXVECTOR3(1.2f,1.2f,4.0f),
			0.0f, 0.0f, 0.0f, NULL_VECTOR3, ATTACKMODELBEHAVIOR00_NORMAL);
	}

	if (g_aBoss.nCntAttackTime == 950)
	{
		g_bPurposeFlag[0] = false;//�ėp�I�Ɏg���t���O
		g_bPurposeFlag[1] = false;//�ėp�I�Ɏg���t���O�P
		g_bPurposeFlag[2] = false;//�ėp�I�Ɏg���t���O�Q

		g_aBoss.bUnderAttack = false;//�U�����Ă��邩�ǂ���
		g_aBoss.nCntAttackTime = 0;
		g_nPurposeCnt = 0;//�ėp�J�E���g
		g_aBoss.fSaveAim = 0.0f;

		g_aBoss.move = NULL_VECTOR3;
	}
}


//======================================
//�{�X�𓦂�������
//======================================
void RunAwayBoss(void)
{
	g_aBoss.nCntAttackTime++;

	if (g_aBoss.nCntAttackTime == 1)
	{
		g_aBoss.move = NULL_VECTOR3;//�ړ��ʂ��O��
		g_bPurposeFlag[0] = false;//�ėp�I�Ɏg���t���O�O
		g_bPurposeFlag[1] = false;//�ėp�I�Ɏg���t���O�P
		g_bPurposeFlag[2] = false;//�ėp�I�Ɏg���t���O�Q
		g_nPurposeCnt = 0;//�ėp�J�E���g
		g_aBoss.state = BOSSSTATE_ATTACK;
		g_aBoss.fSaveAim = g_aBoss.rot.y - D3DX_PI;
		g_aBoss.bCurveLaser = false;//���[�U�[���J�[�u��������������߂�
		g_aBoss.fRotMove = 0.0f;//��]����ړ���
	}
  
	if (g_aBoss.nCntAttackTime >= 2 && g_aBoss.nCntAttackTime <= 300)
	{
		g_aBoss.move = NULL_VECTOR3;
		//g_aBoss.pos.y += 10.0f;
	}

	if (g_aBoss.nCntAttackTime >= 301)
	{
		if (g_aBoss.nCntAttackTime == 305)
		{
			for (int nCntEnemy = 0; nCntEnemy < 10; nCntEnemy++)
			{
				float fLength = float(rand() % 500);
				float fRot = float(rand() % 200 + 1) / 100.0f;
				int nType = rand() % 4;
				SetEnemy(D3DXVECTOR3(g_aBoss.pos.x + sinf(D3DX_PI * fRot) * fLength, 0.0f, g_aBoss.pos.z + cosf(D3DX_PI * fRot) * fLength),
					NULL_VECTOR3,
					nType, true);//�G���Z�b�g���������A�֐��̐�ŁA�{�X���������������J�E���g���Ă���
			}
		}
		g_aBoss.move = NULL_VECTOR3;

		if (g_aBoss.nCntSummonEnemy == 0 && g_aBoss.nCntAttackTime >= 310)
		{//���������G�̐����O�ɂȂ�����
			g_bPurposeFlag[0] = false;//�ėp�I�Ɏg���t���O
			g_bPurposeFlag[1] = false;//�ėp�I�Ɏg���t���O�P
			g_bPurposeFlag[2] = false;//�ėp�I�Ɏg���t���O�Q

			g_aBoss.bUnderAttack = false;//�U�����Ă��邩�ǂ���
			g_aBoss.bRunAwayBoss = false;//�{�X�̓������Ԃ�����
			g_aBoss.nCntAttackTime = 0;
			g_nPurposeCnt = 0;//�ėp�J�E���g
			g_aBoss.fSaveAim = 0.0f;
			
		}
	}

	PrintDebugProc("�{�X�����������G�̐��F%d", g_aBoss.nCntSummonEnemy);
}

//============================================================================================================================================================================================================
//�{�X�̃}�g���b�N�X���v�Z����
//============================================================================================================================================================================================================
void CaluculateMatrixBoss(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�ւ̃|�C���^�Ǝ擾
	D3DXMATRIX mtxRot, mtxTrans, mtxParent;//�v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;//���݂̃}�e���A���ۑ��p

	//���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	//�g�p���Ă���ꍇ
	if (g_aBoss.bUse == true)
	{
		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_aBoss.mtxWorld);

		//�����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aBoss.rot.y, g_aBoss.rot.x, g_aBoss.rot.z);
		D3DXMatrixMultiply(&g_aBoss.mtxWorld, &g_aBoss.mtxWorld, &mtxRot);

		//�ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, g_aBoss.pos.x, g_aBoss.pos.y, g_aBoss.pos.z);
		D3DXMatrixMultiply(&g_aBoss.mtxWorld, &g_aBoss.mtxWorld, &mtxTrans);

		for (int nCntParts = 0; nCntParts < g_aBoss.nNumParts; nCntParts++)
		{
			//�e�p�[�c���f���̃��[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aBoss.aParts[nCntParts].mtxWorld);

			//�e�p�[�c���f���̌����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot,
				g_aBoss.aParts[nCntParts].rot.y,
				g_aBoss.aParts[nCntParts].rot.x,
				g_aBoss.aParts[nCntParts].rot.z);

			D3DXMatrixMultiply(&g_aBoss.aParts[nCntParts].mtxWorld, &g_aBoss.aParts[nCntParts].mtxWorld, &mtxRot);

			//�e�p�[�c���f���̈ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans,
				g_aBoss.aParts[nCntParts].pos.x,
				g_aBoss.aParts[nCntParts].pos.y,
				g_aBoss.aParts[nCntParts].pos.z);

			D3DXMatrixMultiply(&g_aBoss.aParts[nCntParts].mtxWorld, &g_aBoss.aParts[nCntParts].mtxWorld, &mtxTrans);

			//�e�ԍ��̕ۑ�
			int nParent = g_aBoss.aParts[nCntParts].nParent;

			//�e�̃}�g���b�N�X�𔽉f
			if (nParent != -1)
			{//�e�̔ԍ������ꍇ
				mtxParent = g_aBoss.aParts[nParent].mtxWorld;//�e���f���̃}�g���b�N�X
			}
			else
			{//�e�̔ԍ��������Ȃ��ꍇ
				mtxParent = g_aBoss.mtxWorld;//�v���C���[�̃}�g���b�N�X
			}

			//�e�̃}�g���b�N�X���|�����킹��
			D3DXMatrixMultiply(&g_aBoss.aParts[nCntParts].mtxWorld, &g_aBoss.aParts[nCntParts].mtxWorld, &mtxParent);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aBoss.aParts[nCntParts].mtxWorld);

			D3DXVECTOR3 PosZero = NULL_VECTOR3;

			D3DXVec3TransformCoord(&g_aBoss.aParts[nCntParts].PartsPos, &PosZero, &g_aBoss.aParts[nCntParts].mtxWorld);
		}
	}
}
