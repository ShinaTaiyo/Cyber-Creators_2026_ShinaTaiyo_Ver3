//========================================================
//
//�G�̏���[Enemy.cpp]
//Author:ShinaTaiyo
//
//=========================================================
#include "enemy.h"
#include "main.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "shadow.h"
#include "player.h"
#include <stdio.h>
#include <string.h>
#include "model.h"
#include "score.h"
#include "bullet.h"
#include "item.h"
#include "particle.h"
#include "scrap.h"
#include "game.h"
#include "bossgauge.h"
#include "enemygauge.h"
#include "battery.h"
#include "effect.h"
#include "attackmodel.h"
#include "mission.h"
#include "field.h"
#include "Rader.h"
#include "debugproc.h"
#include "camera.h"
#include "timer.h"
#include "boss.h"
#include "combo.h"
#define TEST_MOTION	0

//==============================
//�O���[�o���ϐ�
//==============================
EnemyKey g_aEnemyKey[NUM_ENEMY][MAX_ENEMYPARTS];//�e�L�[���
EnemyKeySet g_aEnemyKeySet[NUM_ENEMY][MAX_ENEMYKEY][MAX_ENEMYMOTION];//�e�L�[�Z�b�g���
EnemyMotionSet g_aEnemyMotionSet[NUM_ENEMY][MAX_ENEMYMOTION];//���[�V�������
EnemyParts g_aEnemyParts[NUM_ENEMY][MAX_ENEMYPARTS];//�G�̊e�p�[�c���
Enemy g_aEnemy[MAX_ENEMY];//���f�����̍\����
int g_nDelayEnemy = 0;
int g_nldxShadowEnemy = -1;//�Ώۂ̉e�̃C���f�b�N�X�i�ԍ��j
int g_nLastDamageEnemy;//�Ō�ɍU�������Ƃ��̃_���[�W��4

//==============================
//����������
//==============================
void InitEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�ւ̃|�C���^�Ǝ擾
	BYTE* pVtxBuff;							//���_�o�b�t�@�ւ̃|�C���^
	D3DXMATERIAL* pMat;						//�}�e���A���ւ̃|�C���^

	//�ϐ�
	g_nDelayEnemy = 0;
	g_nLastDamageEnemy = 0;//�Ō�ɗ^�����_���[�W��

	for (int nCntType = 0; nCntType < NUM_ENEMY; nCntType++)
	{
		for (int nCntParts = 0; nCntParts < ENEMY_NUMPARTS[nCntType]; nCntParts++)
		{
			g_aEnemyParts[nCntType][nCntParts].pBuffMat = {};
			g_aEnemyParts[nCntType][nCntParts].dwNumMat = {};
			g_aEnemyParts[nCntType][nCntParts].pMesh = {};
			g_aEnemyParts[nCntType][nCntParts].mtxWorld = {};
			g_aEnemyParts[nCntType][nCntParts].nIdxModelParent = -1;

			for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
			{
				g_aEnemy[nCntEnemy].parts[nCntType][nCntParts].pBuffMat = {};
				g_aEnemy[nCntEnemy].parts[nCntType][nCntParts].dwNumMat = {};
				g_aEnemy[nCntEnemy].parts[nCntType][nCntParts].pMesh = {};
				g_aEnemy[nCntEnemy].parts[nCntType][nCntParts].mtxWorld = {};
				g_aEnemy[nCntEnemy].parts[nCntType][nCntParts].nIdxModelParent = -1;

			}
		}
	}
	
	//X�t�@�C���̓ǂݍ���
	for (int nCntType = 0; nCntType < NUM_ENEMY; nCntType++)
	{
		for (int nCntParts = 0; nCntParts < ENEMY_NUMPARTS[nCntType]; nCntParts++)
		{
			//�^�C�v0
			if (nCntType == 0)
			{
				D3DXLoadMeshFromX(ENEMY000_FILENAME[nCntParts], D3DXMESH_SYSTEMMEM, pDevice, NULL,
					&g_aEnemyParts[nCntType][nCntParts].pBuffMat,
					NULL,
					&g_aEnemyParts[nCntType][nCntParts].dwNumMat,
					&g_aEnemyParts[nCntType][nCntParts].pMesh);
			}
			//�^�C�v1
			else if (nCntType == 1)
			{
				D3DXLoadMeshFromX(ENEMY001_FILENAME[nCntParts], D3DXMESH_SYSTEMMEM, pDevice, NULL,
					&g_aEnemyParts[nCntType][nCntParts].pBuffMat,
					NULL,
					&g_aEnemyParts[nCntType][nCntParts].dwNumMat,
					&g_aEnemyParts[nCntType][nCntParts].pMesh);
			}
			//�^�C�v2
			else if (nCntType == 2)
			{
				D3DXLoadMeshFromX(ENEMY002_FILENAME[nCntParts], D3DXMESH_SYSTEMMEM, pDevice, NULL,
					&g_aEnemyParts[nCntType][nCntParts].pBuffMat,
					NULL,
					&g_aEnemyParts[nCntType][nCntParts].dwNumMat,
					&g_aEnemyParts[nCntType][nCntParts].pMesh);
			}
			//�^�C�v3
			else if (nCntType == 3)
			{
				D3DXLoadMeshFromX(ENEMY003_FILENAME[nCntParts], D3DXMESH_SYSTEMMEM, pDevice, NULL,
					&g_aEnemyParts[nCntType][nCntParts].pBuffMat,
					NULL,
					&g_aEnemyParts[nCntType][nCntParts].dwNumMat,
					&g_aEnemyParts[nCntType][nCntParts].pMesh);
			}
			//�^�C�v4
			else if (nCntType == 4)
			{
				D3DXLoadMeshFromX(ENEMY004_FILENAME[nCntParts], D3DXMESH_SYSTEMMEM, pDevice, NULL,
					&g_aEnemyParts[nCntType][nCntParts].pBuffMat,
					NULL,
					&g_aEnemyParts[nCntType][nCntParts].dwNumMat,
					&g_aEnemyParts[nCntType][nCntParts].pMesh);
			}
	
			//�}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_aEnemyParts[nCntType][nCntParts].pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aEnemyParts[nCntType][nCntParts].dwNumMat; nCntMat++)
			{
				//�e�N�X�`���t�@�C�������݂���
				if (pMat[nCntMat].pTextureFilename != NULL)
				{
					//�e�N�X�`���̓ǂݍ���
					D3DXCreateTextureFromFile(pDevice,
						pMat[nCntMat].pTextureFilename,
						&g_aEnemyParts[nCntType][nCntParts].pTexture[nCntMat]);
				}

				for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
				{
					g_aEnemy[nCntEnemy].parts[nCntType][nCntParts].Diffuse[nCntMat] = pMat[nCntMat].MatD3D.Diffuse;//�F����
					g_aEnemy[nCntEnemy].parts[nCntType][nCntParts].FormarDiffuse[nCntMat] = pMat[nCntMat].MatD3D.Diffuse;//���̐F����
				}
			}
		}

		//���[�V�������̓ǂݍ���
		LoadEnemyMotion(nCntType);
	}

	//�\���̂̏��
	for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++)
	{
		g_aEnemy[nCnt].bUse = false;//�g�p���Ă��邩�ǂ���
		g_aEnemy[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�ʒu
		g_aEnemy[nCnt].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//1F�O�̈ʒu
		g_aEnemy[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�ړ���
		g_aEnemy[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//����
		g_aEnemy[nCnt].nIdxShadow = -1;//�e�̃C���f�b�N�X
		g_aEnemy[nCnt].nIdxRaderDisplay = -1;//���[�_�[�f�B�X�v���C�̃C���f�b�N�X
		g_aEnemy[nCnt].nType = -1;//���
		g_aEnemy[nCnt].vtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//���ꂼ��̍��W�̃��f���̈ʒu�̍ŏ��l
		g_aEnemy[nCnt].vtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//���ꂼ��̍��W�̃��f���̈ʒu�̍ő�l
		g_aEnemy[nCnt].Size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�G�̑傫��
		g_aEnemy[nCnt].bIsLanding = false;//�n�ʂɂ��Ă��邩�ǂ���
		g_aEnemy[nCnt].fVaimY1 = 0.0f;//��������Y�����̃x�N�g��
		g_aEnemy[nCnt].fVXaim = 0.0f;
		g_aEnemy[nCnt].fVYaim = 0.0f;
		g_aEnemy[nCnt].fVZaim = 0.0f;
		g_aEnemy[nCnt].fVaim = 0.0f;
		g_aEnemy[nCnt].fPenduram = 0.0f;//�U��q�̂悤�ɒe�𔭎˂����鏈��
		g_aEnemy[nCnt].bPenduramFlag = false;
		g_aEnemy[nCnt].fLength = 0.0f;//�������Ƃ�
		g_aEnemy[nCnt].state = ENEMYSTATE_NEUTRAL;//���
		g_aEnemy[nCnt].nAttackTiming = 0;//�U������̃^�C�~���O�����߂�
		g_aEnemy[nCnt].bHitFlag = false;//�U�����q�b�g�������ǂ��������߂�ϐ�
		g_aEnemy[nCnt].fMoveStartLength = 0.0f;//�G���ړ����J�n���鋗��
		g_aEnemy[nCnt].fAttackStartLength = 0.0f;//�G���U�����J�n���鋗��
		g_aEnemy[nCnt].bUnderAttack = false;//�G���U�������ǂ��������߂�
		g_aEnemy[nCnt].nNumLandingModel = 0;//�ǂ̃��f���̏�ɏ���Ă��邩
		g_aEnemy[nCnt].bDefeatDropScrap = false;//�|���ꂽ�Ƃ��ɃX�N���b�v���h���b�v���邩�ǂ���
		g_aEnemy[nCnt].bSummonByBoss = false;//�{�X�ɂ���ď������ꂽ���ǂ���
		//========================================
		//�G�̕\���I�v�V�����̕\���̕ϐ�
		//========================================
		g_aEnemy[nCnt].fOptionDispLength = 0.0f;//�G�̃I�v�V�����̕\�����J�n���鋗��
		g_aEnemy[nCnt].bUseEnemyDisplay = false;//�G�̃I�v�V�����\�����g�p���邩�ǂ���

		//================================
		//��Ԃ̋���
		//================================
		g_aEnemy[nCnt].bCompulsionAttack = false;//�����G�ɍU����Ԃɂ���
		g_aEnemy[nCnt].bCompulsionNeutoral = false;//�����I�Ƀj���[�g������Ԃɂ���
		
		//============================
		//�O�������@�_������
		//============================
		g_aEnemy[nCnt].fVXaim3D = 0.0f;
		g_aEnemy[nCnt].fVYaim3D = 0.0f;
		g_aEnemy[nCnt].fVZaim3D = 0.0f;
		g_aEnemy[nCnt].fVLaim3D = 0.0f;

		for (int nCntAttackModel = 0; nCntAttackModel < MAX_ATTACKMODEL; nCntAttackModel++)
		{
			g_aEnemy[nCnt].bHitStopBoomerang[nCntAttackModel] = false;//�u�[�������ɑ΂���q�b�g�X�g�b�v���������
			g_aEnemy[nCnt].nCoolTimeBoomerang[nCntAttackModel] = 0;//�u�[�������ɑ΂���q�b�g�X�g�b�v�̎��Ԃ��J�E���g����
		}

		//==========================
		//�U���󂯕t������
		//==========================
		for (int nCntHitStop = 0; nCntHitStop < PLAYERATTACK_MAX; nCntHitStop++)
		{
			g_aEnemy[nCnt].bHitStop[nCntHitStop] = false;//���̕ϐ���true�̊ԁA�U�����󂯕t���Ȃ�
			g_aEnemy[nCnt].nCoolTime[nCntHitStop] = 0;   //bHitStop��false�ɂ���܂ł̃J�E���g
			g_aEnemy[nCnt].bWeakWeapon[nCntHitStop] = false;//�ǂ̕���Ɏア�������߂�B
		}

		//�X�e�[�^�X�ݒ�
		g_aEnemy[nCnt].nHp = 0;//�̗�
		g_aEnemy[nCnt].nAttack = 0;//�U����
		g_aEnemy[nCnt].nDefeatScore = 0;//�|�����Ƃ��ɂ��炦��X�R�A
		g_aEnemy[nCnt].fSpeed = 0.0f;//�G�̑���
		g_aEnemy[nCnt].nMaxHp = 0;//�G�̗̑͂̍ő�l

		//===========================
		//���{�X�ϐ�
		//===========================
		g_aEnemy[nCnt].nLotteryAttack = 0;//�ǂ̍U�������邩�̒��I���s���B

		//�e��ނ̃T�C�Y�����߂�
		for (int nCntType = 0; nCntType < NUM_ENEMY; nCntType++)
		{
			for (int nCntParts = 0; nCntParts < ENEMY_NUMPARTS[nCntType]; nCntParts++)
			{
				//���_���̎擾
				int nNumVtx = g_aEnemyParts[nCntType][nCntParts].pMesh->GetNumVertices();

				//���_�t�H�[�}�b�g�̃T�C�Y���擾
				DWORD sizeFVF = D3DXGetFVFVertexSize(g_aEnemyParts[nCntType][nCntParts].pMesh->GetFVF());

				//���_�o�b�t�@�̃��b�N
				g_aEnemyParts[nCntType][nCntParts].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

				for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
				{
					//���_���W�̑��
					D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

					//���f���̍ŏ��l�ƍő�l���擾
					if (vtx.x < g_aEnemy[nCnt].vtxMin.x)
					{//X���W�̍ŏ��l
						g_aEnemy[nCnt].vtxMin.x = vtx.x;
					}
					if (vtx.y < g_aEnemy[nCnt].vtxMin.y)
					{//Y���W�̍ŏ��l
						g_aEnemy[nCnt].vtxMin.y = vtx.y;
					}
					if (vtx.z < g_aEnemy[nCnt].vtxMin.z)
					{//Z���W�̍ŏ��l
						g_aEnemy[nCnt].vtxMin.z = vtx.z;
					}
					if (vtx.x > g_aEnemy[nCnt].vtxMax.x)
					{//X���W�̍ő�l
						g_aEnemy[nCnt].vtxMax.x = vtx.x;
					}
					if (vtx.y > g_aEnemy[nCnt].vtxMax.y)
					{//Y���W�̍ő�l
						g_aEnemy[nCnt].vtxMax.y = vtx.y;
					}
					if (vtx.z > g_aEnemy[nCnt].vtxMax.z)
					{//Z���W�̍ő�l
						g_aEnemy[nCnt].vtxMax.z = vtx.z;
					}

					//���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
					pVtxBuff += sizeFVF;
				}

				//�G�̑傫�������߂�B�i�Ŗ��I���W�i������)
				g_aEnemy[nCnt].Size = g_aEnemy[nCnt].vtxMax - g_aEnemy[nCnt].vtxMin;

				//���_�o�b�t�@�̃A�����b�N
				g_aEnemyParts[nCntType][nCntParts].pMesh->UnlockVertexBuffer();
			}
		}
	}
}

//==============================
//�I������
//==============================
void UninitEnemy(void)
{
	for (int nCntType = 0; nCntType < NUM_ENEMY; nCntType++)
	{
		for (int nCntParts = 0; nCntParts < ENEMY_NUMPARTS[nCntType]; nCntParts++)
		{
			//���b�V���̔j��
			if (g_aEnemyParts[nCntType][nCntParts].pMesh != NULL)
			{
				g_aEnemyParts[nCntType][nCntParts].pMesh->Release();
				g_aEnemyParts[nCntType][nCntParts].pMesh = NULL;
			}
			//�}�e���A���̔j��
			if (g_aEnemyParts[nCntType][nCntParts].pBuffMat != NULL)
			{
				g_aEnemyParts[nCntType][nCntParts].pBuffMat->Release();
				g_aEnemyParts[nCntType][nCntParts].pBuffMat = NULL;
			}

			//�e�N�X�`���̔j��
			for (int nCntMat = 0; nCntMat < MAX_ENEMYMAT; nCntMat++)
			{
				if (g_aEnemyParts[nCntType][nCntParts].pTexture[nCntMat] != NULL)
				{
					g_aEnemyParts[nCntType][nCntParts].pTexture[nCntMat]->Release();
					g_aEnemyParts[nCntType][nCntParts].pTexture[nCntMat] = NULL;
					
				}
			}

			for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
			{
				//���b�V���̔j��
				if (g_aEnemy[nCntEnemy].parts[nCntType][nCntParts].pMesh != NULL)
				{
					g_aEnemy[nCntEnemy].parts[nCntType][nCntParts].pMesh->Release();
					g_aEnemy[nCntEnemy].parts[nCntType][nCntParts].pMesh = NULL;

				}
				
				//���_�o�b�t�@�̔j��
				if (g_aEnemy[nCntEnemy].parts[nCntType][nCntParts].pBuffMat != NULL)
				{
					g_aEnemy[nCntEnemy].parts[nCntType][nCntParts].pBuffMat->Release();
					g_aEnemy[nCntEnemy].parts[nCntType][nCntParts].pBuffMat = NULL;

				}

				//�e�N�X�`���̔j��
				for (int nCntMat = 0; nCntMat < MAX_ENEMYMAT; nCntMat++)
				{
					if (g_aEnemy[nCntEnemy].parts[nCntType][nCntParts].pTexture[nCntMat] != NULL)
					{
						g_aEnemy[nCntEnemy].parts[nCntType][nCntParts].pTexture[nCntMat]->Release();
						g_aEnemy[nCntEnemy].parts[nCntType][nCntParts].pTexture[nCntMat] = NULL;

					}
				}

			}
		}
	}
}

//============================
//�X�V����
//============================
void UpdateEnemy(void)
{
	Player* pPlayer = GetPlayer();		//�v���C���[�̏����擾
	Field* pField = GetField();			//�t�B�[���h�̏����擾
	EnemyHP* pEnemyHP = GetEnemyGauge();//�G�̗̑̓Q�[�W�����擾
	EnemyWeakDisp* pEnemyWeakDisp = GetEnemyWeakDisp();//�G�̎�_�\�L�̏����擾
	Boss* pBoss = GetBoss();//�{�X�̏����擾
	g_nDelayEnemy++;

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemyHP++, pEnemyWeakDisp++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{//�g�p���Ă���ꍇ
			if (g_aEnemy[nCntEnemy].pos.y <= 0.0f)
			{//�n�ʂɐG��Ă���ꍇ
				g_aEnemy[nCntEnemy].bIsLanding = true;//�n�ʂɗ����Ă���
			}

			if (g_aEnemy[nCntEnemy].bIsLanding == false)
			{//�n�ʂɗ����Ă��Ȃ��ꍇ
				g_aEnemy[nCntEnemy].move.y += -0.4f;//�d�͏���
			}
			else
			{
				Model* pModel = GetModel();
				g_aEnemy[nCntEnemy].move.y = 0.0f;

				for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++, pModel++)
				{
					if (pModel->bUse == true && nCntModel == g_aEnemy[nCntEnemy].nNumLandingModel)
					{
						g_aEnemy[nCntEnemy].pos.y = pModel->pos.y + pModel->vtxMax.y;
						break;
					}
				}
			}

			//==============================================================
			//�{�X���o�������u�ԂɃ{�X�ɂ���ď�������ĂȂ��G������
			//==============================================================
			if (pBoss->bUse == true && g_aEnemy[nCntEnemy].bSummonByBoss == false)
			{
				g_aEnemy[nCntEnemy].bUse = false;
				g_aEnemy[nCntEnemy].bDefeatDropScrap = false;//�|���ꂽ����ɂȂ��Ă��A�X�N���b�v���h���b�v���Ȃ��B
			}

			//===========================================
			//�ėp�I�Ɏg�������l
			//===========================================
			float fPurposeRandRot = float(rand() % 200 + 1) / 100.0f;
			float fPurposeRandSpeed = float(rand() % 20 + 1);
			float fPurposeRandSpeedY = float(rand() % 20 + 1);

			float fPurposeoPosX = float(rand() % 400 - 200);
			float fPurposeoPosY = float(rand() % 400 - 200);
			float fPurposeoPosZ = float(rand() % 400 - 200);

			CalculateMatrixEnemy();//�}�g���b�N�X���v�Z����

			//======================================
			//�U���󂯕t������
			//======================================
			AttackModel* pAttackModel = GetAttackModel();

			for (int nCntHitStop = 0; nCntHitStop < PLAYERATTACK_MAX; nCntHitStop++)
			{
				if (g_aEnemy[nCntEnemy].bHitStop[nCntHitStop] == true)
				{
					g_aEnemy[nCntEnemy].nCoolTime[nCntHitStop]++;

					switch (nCntHitStop)
					{
					case PLAYERATTACK00_SWORD:

						if (g_aEnemy[nCntEnemy].nCoolTime[nCntHitStop] >= 20)
						{
							g_aEnemy[nCntEnemy].bHitStop[nCntHitStop] = false;
							g_aEnemy[nCntEnemy].nCoolTime[nCntHitStop] = 0;
						}
						break;

					case PLAYERATTACK02_SPEAKER:

						if (g_aEnemy[nCntEnemy].nCoolTime[nCntHitStop] >= 2)
						{
							g_aEnemy[nCntEnemy].bHitStop[nCntHitStop] = false;
							g_aEnemy[nCntEnemy].nCoolTime[nCntHitStop] = 0;
						}
						break;
					}
				}
			}

			//==========================================
			//�u�[�������ɑ΂���q�b�g�X�g�b�v
			//==========================================
			for (int nCntAttackModel = 0; nCntAttackModel < MAX_ATTACKMODEL; nCntAttackModel++, pAttackModel++)
			{
				if (g_aEnemy[nCntEnemy].bHitStopBoomerang[nCntAttackModel] == true)
				{
					g_aEnemy[nCntEnemy].nCoolTimeBoomerang[nCntAttackModel]++;
					if (g_aEnemy[nCntEnemy].nCoolTimeBoomerang[nCntAttackModel] >= 20)
					{
						g_aEnemy[nCntEnemy].bHitStopBoomerang[nCntAttackModel] = false;
						g_aEnemy[nCntEnemy].nCoolTimeBoomerang[nCntAttackModel] = 0;
					}
				}
			}

			//=========================================
			// �G�ƃv���C���[�̋��������B
			//=========================================
			float fLength = sqrtf((g_aEnemy[nCntEnemy].pos.x - pPlayer->pos.x) * (g_aEnemy[nCntEnemy].pos.x - pPlayer->pos.x) +
				(g_aEnemy[nCntEnemy].pos.z - pPlayer->pos.z) * (g_aEnemy[nCntEnemy].pos.z - pPlayer->pos.z)) / 2;

			g_aEnemy[nCntEnemy].fVXaim = pPlayer->pos.x - g_aEnemy[nCntEnemy].pos.x;
			g_aEnemy[nCntEnemy].fVYaim = pPlayer->pos.y - g_aEnemy[nCntEnemy].pos.y;
			g_aEnemy[nCntEnemy].fVZaim = pPlayer->pos.z - g_aEnemy[nCntEnemy].pos.z;

			g_aEnemy[nCntEnemy].fVaim = atan2f(g_aEnemy[nCntEnemy].fVXaim, g_aEnemy[nCntEnemy].fVZaim);
			g_aEnemy[nCntEnemy].fVaimY1 = atan2f(g_aEnemy[nCntEnemy].fVYaim, g_aEnemy[nCntEnemy].fVZaim);



			//===============================
			//�U�������A�ړ�����
			//===============================
			switch (g_aEnemy[nCntEnemy].nType)
			{
			case 0://���b�h�u��

				//=========================
				//�s�������߂�
				//=========================
				if (fLength < g_aEnemy[nCntEnemy].fAttackStartLength)
				{
					g_aEnemy[nCntEnemy].state = ENEMYSTATE_ATTACK;//�U�����
					g_aEnemy[nCntEnemy].bCompulsionAttack = true;
				}
				//�͈͊O
				else
				{
					g_aEnemy[nCntEnemy].rot.y = g_aEnemy[nCntEnemy].fVaim + D3DX_PI;//�v���C���[�̕���������
					g_aEnemy[nCntEnemy].state = ENEMYSTATE_MOVE;//�ړ����
				}

				if (g_nDelayEnemy % 180 == 0)
				{
					g_aEnemy[nCntEnemy].bCompulsionNeutoral = false;
					int nLottery = rand() % 5;

					if (nLottery == 0)
					{
						g_aEnemy[nCntEnemy].bCompulsionNeutoral = true;
					}
				}

				//===========================
				//��Ԃ̋���
				//===========================
				if (g_aEnemy[nCntEnemy].bCompulsionNeutoral == true)
				{
					g_aEnemy[nCntEnemy].state = ENEMYSTATE_NEUTRAL;
				}

				if (g_aEnemy[nCntEnemy].bCompulsionAttack == true)
				{
					g_aEnemy[nCntEnemy].state = ENEMYSTATE_ATTACK;//�j���[�g�������
				}

				if (g_aEnemy[nCntEnemy].state == ENEMYSTATE_ATTACK)
				{
					g_aEnemy[nCntEnemy].bUnderAttack = true;//�U�����n�܂����̂ŁA�U�����̔��������t���O��true�ɂ���

					float fLengthAttack = sqrtf((g_aEnemy[nCntEnemy].parts[0][0].PartsPos.x - pPlayer->pos.x) * (g_aEnemy[nCntEnemy].parts[0][0].PartsPos.x - pPlayer->pos.x) +
						(g_aEnemy[nCntEnemy].parts[0][0].PartsPos.z - pPlayer->pos.z) * (g_aEnemy[nCntEnemy].parts[0][0].PartsPos.z - pPlayer->pos.z) +
						(g_aEnemy[nCntEnemy].parts[0][0].PartsPos.y - pPlayer->pos.y) * (g_aEnemy[nCntEnemy].parts[0][0].PartsPos.y - pPlayer->pos.y)) / 3;

					g_aEnemy[nCntEnemy].nAttackTiming++;

					if (g_aEnemy[nCntEnemy].nAttackTiming == 1)
					{//�U�����n�܂������Ƀv���C���[�̈ʒu�Ɍ���
						g_aEnemy[nCntEnemy].rot.y = g_aEnemy[nCntEnemy].fVaim + D3DX_PI;//�v���C���[�̕����������i�U�����Ԃ��ƂɌ�����ς���)
					}

					if (fLengthAttack < 20.0f && g_aEnemy[nCntEnemy].bHitFlag == false)
					{
						if (g_aEnemy[nCntEnemy].nAttackTiming >= 34 && g_aEnemy[nCntEnemy].nAttackTiming <= 50)
						{
							AttackHitPlayer(35);//�U�����q�b�g�������Ƀ_���[�W��Ԃɂ���

							g_aEnemy[nCntEnemy].bHitFlag = true;
						}
					}
				}
				break;

			case 1://�~���^���N

				//===========================================
				//�Q�b���Ƃɍs�������߂�
				//===========================================
				if (g_nDelayEnemy % 120 == 0)
				{
					int nLottery = rand() % 4;
					if (nLottery == 0)
					{
						g_aEnemy[nCntEnemy].bUnderAttack = false;
						g_aEnemy[nCntEnemy].bCompulsionNeutoral = false;

					}
					else if (nLottery >= 1 && nLottery <= 2)
					{
						g_aEnemy[nCntEnemy].bUnderAttack = true;
						g_aEnemy[nCntEnemy].bCompulsionNeutoral = false;

					}
					else if (nLottery == 3)
					{
						g_aEnemy[nCntEnemy].bCompulsionNeutoral = true;
					}
				}

				if (g_aEnemy[nCntEnemy].bUnderAttack == false)
				{
					g_aEnemy[nCntEnemy].rot.y = g_aEnemy[nCntEnemy].fVaim + D3DX_PI;//�v���C���[�̕���������
					g_aEnemy[nCntEnemy].state = ENEMYSTATE_MOVE;//�ړ����
				}
				else
				{
					g_aEnemy[nCntEnemy].state = ENEMYSTATE_ATTACK;
				}

				if (g_aEnemy[nCntEnemy].state == ENEMYSTATE_ATTACK)
				{
					g_aEnemy[nCntEnemy].nAttackTiming++;

					if (g_aEnemy[nCntEnemy].nAttackTiming == 5)
					{
						for (int nCntBullet = 0; nCntBullet < 3; nCntBullet++)
						{
							SetBullet(BULLETTYPE_ENEMY, 120, 10.0f, 10.0f,
								D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y + 30.0f, g_aEnemy[nCntEnemy].pos.z), D3DXVECTOR3(sinf(g_aEnemy[nCntEnemy].rot.y + D3DX_PI) * (5.0f + 3.0f * nCntBullet), 0.0f, cosf(g_aEnemy[nCntEnemy].rot.y + D3DX_PI) * (5.0f + 3.0f * nCntBullet)),
								NULL_VECTOR3, D3DXCOLOR(0.545f, 0.27f, 0.074f, 1.0f), nCntEnemy, 0.0f);
						}
					}
				}
				break;

			case 2://�~�b�N�X�W���[�`��

				//===========================================
				//�Q�b���Ƃɍs�������߂�
				//===========================================
				if (g_nDelayEnemy % 120 == 0)
				{
					int nLottery = rand() % 4;
					if (nLottery == 0)
					{
						g_aEnemy[nCntEnemy].bUnderAttack = false;
						g_aEnemy[nCntEnemy].bCompulsionNeutoral = false;

					}
					else if (nLottery >= 1 && nLottery <= 2)
					{
						g_aEnemy[nCntEnemy].bUnderAttack = true;
						g_aEnemy[nCntEnemy].bCompulsionNeutoral = false;

					}
					else if (nLottery == 3)
					{
						g_aEnemy[nCntEnemy].bCompulsionNeutoral = true;
					}
				}

				if (g_aEnemy[nCntEnemy].bUnderAttack == false)
				{
					g_aEnemy[nCntEnemy].rot.y = g_aEnemy[nCntEnemy].fVaim + D3DX_PI;//�v���C���[�̕���������
					g_aEnemy[nCntEnemy].state = ENEMYSTATE_MOVE;//�ړ����
				}
				else
				{
					g_aEnemy[nCntEnemy].state = ENEMYSTATE_ATTACK;
				}
				
				if (g_aEnemy[nCntEnemy].state == ENEMYSTATE_ATTACK)
				{
					g_aEnemy[nCntEnemy].nAttackTiming++;

					if (g_aEnemy[nCntEnemy].nAttackTiming == 1)
					{//�U�����n�܂����u�Ԃ�
						g_aEnemy[nCntEnemy].rot.y = g_aEnemy[nCntEnemy].fVaim + D3DX_PI;//�v���C���[�̕���������
					}

					if (g_aEnemy[nCntEnemy].nAttackTiming == 120)
					{
						SetBullet(BULLETTYPE_RING, 90, 20.0f, 20.0f,
							D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y + 20.0f, g_aEnemy[nCntEnemy].pos.z), D3DXVECTOR3(sinf(g_aEnemy[nCntEnemy].rot.y + D3DX_PI) * 8.0f, 0.0f, cosf(g_aEnemy[nCntEnemy].rot.y + D3DX_PI) * 8.0f),
							NULL_VECTOR3, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), nCntEnemy, 0.1f);
					}
				}
				break;

			case 3://�����X�^�[

				//=========================
				//�s�������߂�
				//=========================
				if (fLength < g_aEnemy[nCntEnemy].fAttackStartLength)
				{//�͈͓�
					g_aEnemy[nCntEnemy].state = ENEMYSTATE_ATTACK;//�U�����
					g_aEnemy[nCntEnemy].bCompulsionAttack = true;
				}
				else
				{//�͈͊O
					g_aEnemy[nCntEnemy].rot.y = g_aEnemy[nCntEnemy].fVaim + D3DX_PI;//�v���C���[�̕���������
					g_aEnemy[nCntEnemy].state = ENEMYSTATE_MOVE;//�ړ����
				}

				if (g_nDelayEnemy % 180 == 0)
				{
					g_aEnemy[nCntEnemy].bCompulsionNeutoral = false;
					int nLottery = rand() % 5;

					if (nLottery == 0)
					{
						g_aEnemy[nCntEnemy].bCompulsionNeutoral = true;
					}
				}

				//===========================
				//��Ԃ̋���
				//===========================
				if (g_aEnemy[nCntEnemy].bCompulsionNeutoral == true)
				{
					g_aEnemy[nCntEnemy].state = ENEMYSTATE_NEUTRAL;
				}

				if (g_aEnemy[nCntEnemy].bCompulsionAttack == true)
				{
					g_aEnemy[nCntEnemy].state = ENEMYSTATE_ATTACK;//�j���[�g�������

				}

				if (g_aEnemy[nCntEnemy].state == ENEMYSTATE_ATTACK)
				{
					g_aEnemy[nCntEnemy].bUnderAttack = true;//�U�����n�܂����̂ŁA�U�����̔��������t���O��true�ɂ���

					//�E��ƃv���C���[�̋���
					float fLengthAttack = sqrtf((g_aEnemy[nCntEnemy].parts[3][2].PartsPos.x - pPlayer->pos.x) * (g_aEnemy[nCntEnemy].parts[3][2].PartsPos.x - pPlayer->pos.x) +
						(g_aEnemy[nCntEnemy].parts[3][2].PartsPos.z - pPlayer->pos.z) * (g_aEnemy[nCntEnemy].parts[3][2].PartsPos.z - pPlayer->pos.z) +
						(g_aEnemy[nCntEnemy].parts[3][2].PartsPos.y - pPlayer->pos.y) * (g_aEnemy[nCntEnemy].parts[3][2].PartsPos.y - pPlayer->pos.y)) / 3;

					g_aEnemy[nCntEnemy].nAttackTiming++;

					if (g_aEnemy[nCntEnemy].nAttackTiming == 1)
					{//�U�����n�܂������Ƀv���C���[�̈ʒu�Ɍ���
						g_aEnemy[nCntEnemy].rot.y = g_aEnemy[nCntEnemy].fVaim + D3DX_PI;//�v���C���[�̕����������i�U�����Ԃ��ƂɌ�����ς���)
					}

					SetEffect(EFFECTTYPE_NORMAL, 200, 20.0f, 20.0f, 1.0f, g_aEnemy[nCntEnemy].parts[3][2].PartsPos, NULL_VECTOR3, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 0.0f, 0);

					if (fLengthAttack < 20.0f && g_aEnemy[nCntEnemy].bHitFlag == false)
					{
						if (g_aEnemy[nCntEnemy].nAttackTiming >= 28 && g_aEnemy[nCntEnemy].nAttackTiming <= 40)
						{
							AttackHitPlayer(40);//�U�����q�b�g�������Ƀ_���[�W��Ԃɂ���

							g_aEnemy[nCntEnemy].bHitFlag = true;
						}
					}
				}
				break;

			case 4://�J�b�v��

				if (fLength < g_aEnemy[nCntEnemy].fMoveStartLength)
				{//�͈͓�
					//===========================================
					//�Q�b���ƂɍU�����ړ��������߂�
					//===========================================
					if (g_nDelayEnemy % 120 == 0)
					{
						int nLottery = rand() % 2;
						if (nLottery == 0)
						{
							g_aEnemy[nCntEnemy].bUnderAttack = false;
						}
						else
						{
							g_aEnemy[nCntEnemy].bUnderAttack = true;
						}
					}

					if (g_aEnemy[nCntEnemy].bUnderAttack == false)
					{
						g_aEnemy[nCntEnemy].rot.y = g_aEnemy[nCntEnemy].fVaim + D3DX_PI;//�v���C���[�̕���������
						g_aEnemy[nCntEnemy].state = ENEMYSTATE_MOVE;//�ړ����
					}
					else
					{
						g_aEnemy[nCntEnemy].state = ENEMYSTATE_ATTACK;
					}
				}
				else
				{//�͈͊O
					if (g_aEnemy[nCntEnemy].bUnderAttack == false)
					{
						g_aEnemy[nCntEnemy].state = ENEMYSTATE_NEUTRAL;//�j���[�g�������
					}
				}

				if (g_aEnemy[nCntEnemy].state == ENEMYSTATE_ATTACK)
				{
					g_aEnemy[nCntEnemy].nAttackTiming++;

					if (g_aEnemy[nCntEnemy].nAttackTiming == 1)
					{//�U�����n�܂����u�ԂɃv���C���[�̕���������
						g_aEnemy[nCntEnemy].nLotteryAttack = rand() % 4;
						g_aEnemy[nCntEnemy].rot.y = g_aEnemy[nCntEnemy].fVaim + D3DX_PI;//�v���C���[�̕���������
					}

					if (g_aEnemy[nCntEnemy].nLotteryAttack == 0)
					{//�U���O�F�`���΂�
						if (g_aEnemy[nCntEnemy].nAttackTiming % 5 == 0)
						{
							SetAttackModel(ATTACKMODEL08_SOUP, 100, g_aEnemy[nCntEnemy].parts[4][1].PartsPos,
								D3DXVECTOR3(sinf(D3DX_PI * fPurposeRandRot) * fPurposeRandSpeed, fPurposeRandSpeedY / 2.0f, cosf(D3DX_PI * fPurposeRandRot) * fPurposeRandSpeed), NULL_VECTOR3, ONE_VECTOR3, 0.0f, 0.0f, 0.1f, NULL_VECTOR3, ATTACKMODELBEHAVIOR01_ANOTHER);
						}
					}
					else if (g_aEnemy[nCntEnemy].nLotteryAttack == 1)
					{//�U���P�F�X�t�B�A�e
						if (g_aEnemy[nCntEnemy].nAttackTiming % 30 == 0)
						{
							SetAttackModel(ATTACKMODEL02_SPHERE, 200, g_aEnemy[nCntEnemy].pos, NULL_VECTOR3, NULL_VECTOR3, ONE_VECTOR3, 0.0f, 0.0f, 0.03f, NULL_VECTOR3, ATTACKMODELBEHAVIOR01_ANOTHER);
						}
					}
					else if (g_aEnemy[nCntEnemy].nLotteryAttack == 2)
					{//�U���Q�F�T���_�[�o���A

						//============================
						//�_�ŏ���
						//============================
						if (g_aEnemy[nCntEnemy].nAttackTiming >= 0 && g_aEnemy[nCntEnemy].nAttackTiming <= 40)
						{
							if (g_aEnemy[nCntEnemy].nAttackTiming % 2 == 0)
							{
								for (int nCntParts = 0; nCntParts < ENEMY_NUMPARTS[g_aEnemy[nCntEnemy].nType]; nCntParts++)
								{
									for (int nCntMat = 0; nCntMat < (int)(g_aEnemyParts[g_aEnemy[nCntEnemy].nType][nCntParts].dwNumMat); nCntMat++)
									{
										g_aEnemy[nCntEnemy].parts[g_aEnemy[nCntEnemy].nType][nCntParts].Diffuse[nCntMat].b = 1.0f;
										g_aEnemy[nCntEnemy].parts[g_aEnemy[nCntEnemy].nType][nCntParts].Diffuse[nCntMat].r = 0.0f;
										g_aEnemy[nCntEnemy].parts[g_aEnemy[nCntEnemy].nType][nCntParts].Diffuse[nCntMat].g = 0.0f;
									}
								}
							}
							else
							{
								for (int nCntParts = 0; nCntParts < ENEMY_NUMPARTS[g_aEnemy[nCntEnemy].nType]; nCntParts++)
								{
									for (int nCntMat = 0; nCntMat < (int)(g_aEnemyParts[g_aEnemy[nCntEnemy].nType][nCntParts].dwNumMat); nCntMat++)
									{
										g_aEnemy[nCntEnemy].parts[g_aEnemy[nCntEnemy].nType][nCntParts].Diffuse[nCntMat] =
											g_aEnemy[nCntEnemy].parts[g_aEnemy[nCntEnemy].nType][nCntParts].FormarDiffuse[nCntMat];
									}
								}
							}

							if (g_aEnemy[nCntEnemy].nAttackTiming == 40)
							{
								for (int nCntParts = 0; nCntParts < ENEMY_NUMPARTS[g_aEnemy[nCntEnemy].nType]; nCntParts++)
								{
									for (int nCntMat = 0; nCntMat < (int)(g_aEnemyParts[g_aEnemy[nCntEnemy].nType][nCntParts].dwNumMat); nCntMat++)
									{
										g_aEnemy[nCntEnemy].parts[g_aEnemy[nCntEnemy].nType][nCntParts].Diffuse[nCntMat] =
											g_aEnemy[nCntEnemy].parts[g_aEnemy[nCntEnemy].nType][nCntParts].FormarDiffuse[nCntMat];
									}
								}
							}
						}

						//=============================
						//�U���J�n
						//=============================
						if (g_aEnemy[nCntEnemy].nAttackTiming == 80)
						{
							PlaySound(SOUND_LABEL_SE_THUNDER_000);


							for (int nCntModel = 0; nCntModel < ENEMY004_NUM_ATTACK2; nCntModel++)
							{
								float fRot = (2.0f / (float)(ENEMY004_NUM_ATTACK2)) * (float)(nCntModel);
								SetAttackModel(ATTACKMODEL06_THUNDER, 180, D3DXVECTOR3(sinf(D3DX_PI * fRot) * 200.0f + g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y, cosf(D3DX_PI * fRot) * 200.0f + g_aEnemy[nCntEnemy].pos.z), NULL_VECTOR3, NULL_VECTOR3, D3DXVECTOR3(1.5f, 0.5f, 0.5f), 0.0f, 0.0f, 0.05f, NULL_VECTOR3, ATTACKMODELBEHAVIOR01_ANOTHER);
							}
						}
					}
					else if (g_aEnemy[nCntEnemy].nLotteryAttack == 3)
					{//���r�b�g�V���b�g
						//==========================
						//�R�������@�_������
						//==========================
						g_aEnemy[nCntEnemy].fVXaim3D = g_aEnemy[nCntEnemy].pos.x - (g_aEnemy[nCntEnemy].pos.x + fPurposeoPosX);
						g_aEnemy[nCntEnemy].fVYaim3D = g_aEnemy[nCntEnemy].pos.y - (g_aEnemy[nCntEnemy].pos.y + fPurposeoPosY);
						g_aEnemy[nCntEnemy].fVZaim3D = g_aEnemy[nCntEnemy].pos.z - (g_aEnemy[nCntEnemy].pos.z + fPurposeoPosZ);

						D3DXVECTOR3 BulletMove = NULL_VECTOR3;

						g_aEnemy[nCntEnemy].fVLaim3D = sqrtf((g_aEnemy[nCntEnemy].fVXaim3D * g_aEnemy[nCntEnemy].fVXaim3D) + (g_aEnemy[nCntEnemy].fVYaim3D * g_aEnemy[nCntEnemy].fVYaim3D)
							+ (g_aEnemy[nCntEnemy].fVZaim3D * g_aEnemy[nCntEnemy].fVZaim3D));

						BulletMove.x = g_aEnemy[nCntEnemy].fVXaim3D / g_aEnemy[nCntEnemy].fVLaim3D * 5.0f;
						BulletMove.y = g_aEnemy[nCntEnemy].fVYaim3D / g_aEnemy[nCntEnemy].fVLaim3D * 5.0f;
						BulletMove.z = g_aEnemy[nCntEnemy].fVZaim3D / g_aEnemy[nCntEnemy].fVLaim3D * 5.0f;

						SetBullet(BULLETTYPE_ENEMY, 300, 10.0f, 10.0f, g_aEnemy[nCntEnemy].pos, BulletMove, NULL_VECTOR3, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), nCntEnemy, 0.0f);
					}
				}

				break;
			}

			//======================================================
			//�ړ���Ԃ�������A�ړ�������
			//======================================================
			if (g_aEnemy[nCntEnemy].state == ENEMYSTATE_MOVE)
			{

				if (g_aEnemy[nCntEnemy].nType == 0 || g_aEnemy[nCntEnemy].nType == 3)
				{
					if (fLength < 60.0f)
					{//������15.0f�ȉ��Ȃ�A�ړ����Ȃ�
						g_aEnemy[nCntEnemy].fSpeed = 0.0f;
					}
					else
					{
						g_aEnemy[nCntEnemy].fSpeed = g_aEnemy[nCntEnemy].fFormarSpeed;
					}
				}
				else if (g_aEnemy[nCntEnemy].nType == 1 || g_aEnemy[nCntEnemy].nType == 2)
				{
					if (fLength < 200.0f)
					{//������15.0f�ȉ��Ȃ�A�ړ����Ȃ�
						g_aEnemy[nCntEnemy].fSpeed = 0.0f;
					}
					else
					{
						g_aEnemy[nCntEnemy].fSpeed = g_aEnemy[nCntEnemy].fFormarSpeed;
					}
				}

				//�m�b�N�o�b�N��move�Ŏg�������̂ŁA�ړ���Ԃ̎��ɁA�G�̃X�s�[�h���A���ڈʒu�����炷�B
				g_aEnemy[nCntEnemy].pos.x += sinf(g_aEnemy[nCntEnemy].fVaim) * g_aEnemy[nCntEnemy].fSpeed;
				g_aEnemy[nCntEnemy].pos.z += cosf(g_aEnemy[nCntEnemy].fVaim) * g_aEnemy[nCntEnemy].fSpeed;
			}

			g_aEnemy[nCntEnemy].posOld = g_aEnemy[nCntEnemy].pos;

			g_aEnemy[nCntEnemy].move.x += (0.0f - g_aEnemy[nCntEnemy].move.x) * 0.1f;
			g_aEnemy[nCntEnemy].move.z += (0.0f - g_aEnemy[nCntEnemy].move.z) * 0.1f;

			g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move;

			//===========================================================================
			//���f���̏�ɏ���Ă��邩�ǂ����̔���ƁA����Ă��郂�f���̔ԍ��̎擾
			//===========================================================================
			g_aEnemy[nCntEnemy].nNumLandingModel = -1;
			g_aEnemy[nCntEnemy].bIsLanding = bCollisionModel(&g_aEnemy[nCntEnemy].pos, &g_aEnemy[nCntEnemy].posOld, g_aEnemy[nCntEnemy].Size,
				g_aEnemy[nCntEnemy].nNumLandingModel);

			//========================================================
			//�I�v�V�����̕\�������邩�ǂ���
			//========================================================

			if (fLength < g_aEnemy[nCntEnemy].fOptionDispLength)
			{
				g_aEnemy[nCntEnemy].bUseEnemyDisplay = true;
			}

			if (g_aEnemy[nCntEnemy].bUseEnemyDisplay == false)
			{
				pEnemyHP->bUse = false;//�̗͕\�������Ȃ�

				if (g_aEnemy[nCntEnemy].nType != 4)
				{
					pEnemyWeakDisp->bUse = false;//��_�\�������Ȃ�
				}
			}
			else
			{
				pEnemyHP->bUse = true;//�̗͕\�����J�n

				if (g_aEnemy[nCntEnemy].nType != 4)
				{
					pEnemyWeakDisp->bUse = true;//��_�\�������Ȃ�
				}
			}

			//======================================
			// �̗͂��O�ȉ��̏ꍇ
			//======================================
			if (g_aEnemy[nCntEnemy].nHp <= 0)
			{
				Mission* pMission = GetMission();//�~�b�V���������擾

				/*PlaySound(SOUND_LABEL_SE_DEFEATENEMY_000);*/

				pMission->aMission1.nDefeatEnemyCount++;//�G���j���ɃJ�E���g���v���X

				g_aEnemy[nCntEnemy].bUse = false;//�̗͂O�ȉ��Ȃ̂ŁA�|���
				AddScore(ENEMY_SCORE[g_aEnemy[nCntEnemy].nType]);//�X�R�A�̉��Z
			}

			//�e�̈ʒu���X�V
			SetPositionShadow(g_aEnemy[nCntEnemy].nIdxShadow,
				D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y - g_aEnemy[nCntEnemy].Size.y / 2, g_aEnemy[nCntEnemy].pos.z));

			SetPositionRaderDisPlay(g_aEnemy[nCntEnemy].nIdxRaderDisplay, g_aEnemy[nCntEnemy].pos, 0.0f);

			//���[�V��������
			EnemyMotion(nCntEnemy);

			//=======================================================
			//�G���t�B�[���h�͈͊O�ɏo���Ƃ��́A�ʒu�̒���
			//=======================================================

			if (g_aEnemy[nCntEnemy].pos.x + (g_aEnemy[nCntEnemy].Size.x / 2.0f) > pField->fWidth || g_aEnemy[nCntEnemy].pos.x - (g_aEnemy[nCntEnemy].Size.x / 2.0f) < -pField->fWidth ||
				g_aEnemy[nCntEnemy].pos.z + (g_aEnemy[nCntEnemy].Size.z / 2.0f) > pField->fHeight || g_aEnemy[nCntEnemy].pos.z - (g_aEnemy[nCntEnemy].Size.z / 2.0f) < -pField->fHeight)
			{
				g_aEnemy[nCntEnemy].pos = g_aEnemy[nCntEnemy].posOld;
			}

			//======================================
			// �G���g�p����Ă��Ȃ��ꍇ
			//======================================
			if (g_aEnemy[nCntEnemy].bUse == false)
			{
				pPlayer->nTotalDefeatEnemy++;//���v�G���j�����v���X

				if (g_aEnemy[nCntEnemy].bSummonByBoss == true)
				{
					pBoss->nCntSummonEnemy--;
				}

				//�G�̊e�p�[�c�̐ݒ�
				for (int nCntParts = 0; nCntParts < ENEMY_NUMPARTS[g_aEnemy[nCntEnemy].nType]; nCntParts++)
				{
					g_aEnemy[nCntEnemy].parts[g_aEnemy[nCntEnemy].nType][nCntParts].bUse = false;//�g�p����
				}

				//============================
				// �X�N���b�v�̃h���b�v����
				//============================
				if (g_aEnemy[nCntEnemy].bDefeatDropScrap == true)
				{
					//�v���C���[�̃R���{����J�E���g��0�ł͂Ȃ��ꍇ
					if (pPlayer->nCntCombo > 0)
					{
						//�R���{�{�������Z
						pPlayer->fCombo += 0.1f;

						//�R���{�{�����ő�{�����������ꍇ
						if (pPlayer->fCombo >= MAX_COMBO)
						{
							//�ő�{���ɒ���
							pPlayer->fCombo = MAX_COMBO;
						}
					}
					
					//�v���C���[�̃R���{�J�E���g�����Z�b�g
					pPlayer->nCntCombo = COMBO_COUNT;

					for (int nCntDrop = 0; nCntDrop < 10; nCntDrop++)
					{
						float fRandumRot = float(rand() % 200 + 1) / 100;
						float fRandumSpeed = float(rand() % 100 - 50) / 10;

						switch (g_aEnemy[nCntEnemy].nType)
						{
						case 0:
							SetScrap(SCRAPTYPE00_TIBIROBO, GENERALLYSCRAPLIFE, SCRAPRADIUSSIZE, SCRAPRADIUSSIZE,
								D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y + SCRAPRADIUSSIZE, g_aEnemy[nCntEnemy].pos.z)
								, g_aEnemy[nCntEnemy].rot,
								D3DXVECTOR3(sinf(-D3DX_PI * fRandumRot) * fRandumSpeed, 0.0f, cosf(-D3DX_PI * fRandumRot) * fRandumSpeed));
							break;
						case 1:
							SetScrap(SCRAPTYPE03_SPEAKER, GENERALLYSCRAPLIFE, SCRAPRADIUSSIZE, SCRAPRADIUSSIZE,
								D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y + SCRAPRADIUSSIZE, g_aEnemy[nCntEnemy].pos.z)
								, g_aEnemy[nCntEnemy].rot,
								D3DXVECTOR3(sinf(-D3DX_PI * fRandumRot) * fRandumSpeed, 0.0f, cosf(-D3DX_PI * fRandumRot) * fRandumSpeed));
							break;
						case 2:
							SetScrap(SCRAPTYPE02_BOOMERANG, GENERALLYSCRAPLIFE, SCRAPRADIUSSIZE, SCRAPRADIUSSIZE,
								D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y + SCRAPRADIUSSIZE, g_aEnemy[nCntEnemy].pos.z)
								, g_aEnemy[nCntEnemy].rot,
								D3DXVECTOR3(sinf(-D3DX_PI * fRandumRot) * fRandumSpeed, 0.0f, cosf(-D3DX_PI * fRandumRot) * fRandumSpeed));
							break;
						case 3:
							SetScrap(SCRAPTYPE01_SWORD, GENERALLYSCRAPLIFE, SCRAPRADIUSSIZE, SCRAPRADIUSSIZE,
								D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y + SCRAPRADIUSSIZE, g_aEnemy[nCntEnemy].pos.z)
								, g_aEnemy[nCntEnemy].rot,
								D3DXVECTOR3(sinf(-D3DX_PI * fRandumRot) * fRandumSpeed, 0.0f, cosf(-D3DX_PI * fRandumRot) * fRandumSpeed));
							break;
						case 4:
							SetScrap(SCRAPTYPE04_ALL, GENERALLYSCRAPLIFE, SCRAPRADIUSSIZE, SCRAPRADIUSSIZE,
								D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y + SCRAPRADIUSSIZE, g_aEnemy[nCntEnemy].pos.z)
								, g_aEnemy[nCntEnemy].rot,
								D3DXVECTOR3(sinf(-D3DX_PI * fRandumRot) * fRandumSpeed, 0.0f, cosf(-D3DX_PI * fRandumRot) * fRandumSpeed));
							break;
						}
					}
				}

				KillShadow(g_aEnemy[nCntEnemy].nIdxShadow);//�e�̔j��
				KillRaderDisplay(g_aEnemy[nCntEnemy].nIdxRaderDisplay);
			}
		}
	}

	PrintDebugProc("�Ō�ɗ^�����_���[�W�ʁF%d\n", g_nLastDamageEnemy);
}

//================================
// �`�揈��
//================================
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�ւ̃|�C���^�Ǝ擾
	D3DXMATRIX mtxRot, mtxTrans, mtxParent;//�v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL* pMat;//�}�e���A���f�[�^�ւ̃|�C���^

	//���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		//�g�p���Ă���ꍇ
		if (g_aEnemy[nCntEnemy].bUse == true)
		{
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aEnemy[nCntEnemy].mtxWorld);

			//�����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aEnemy[nCntEnemy].rot.y, g_aEnemy[nCntEnemy].rot.x, g_aEnemy[nCntEnemy].rot.z);
			D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].mtxWorld, &g_aEnemy[nCntEnemy].mtxWorld, &mtxRot);

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y, g_aEnemy[nCntEnemy].pos.z);
			D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].mtxWorld, &g_aEnemy[nCntEnemy].mtxWorld, &mtxTrans);

			//��ޕۑ�
			int nType = g_aEnemy[nCntEnemy].nType;

			for (int nCntParts = 0; nCntParts < ENEMY_NUMPARTS[nType]; nCntParts++)
			{
				//�e�p�[�c���f���̃��[���h�}�g���b�N�X�̏�����
				D3DXMatrixIdentity(&g_aEnemy[nCntEnemy].parts[nType][nCntParts].mtxWorld);

				//�e�p�[�c���f���̌����𔽉f
				D3DXMatrixRotationYawPitchRoll(&mtxRot, 
					g_aEnemy[nCntEnemy].parts[nType][nCntParts].rot.y,
					g_aEnemy[nCntEnemy].parts[nType][nCntParts].rot.x,
					g_aEnemy[nCntEnemy].parts[nType][nCntParts].rot.z);

				D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].parts[nType][nCntParts].mtxWorld, &g_aEnemy[nCntEnemy].parts[nType][nCntParts].mtxWorld, &mtxRot);

				//�e�p�[�c���f���̈ʒu�𔽉f
				D3DXMatrixTranslation(&mtxTrans, 
					g_aEnemy[nCntEnemy].parts[nType][nCntParts].pos.x, 
					g_aEnemy[nCntEnemy].parts[nType][nCntParts].pos.y, 
					g_aEnemy[nCntEnemy].parts[nType][nCntParts].pos.z);

				D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].parts[nType][nCntParts].mtxWorld, &g_aEnemy[nCntEnemy].parts[nType][nCntParts].mtxWorld, &mtxTrans);

				//�e�ԍ��̕ۑ�
				int nParent = g_aEnemy[nCntEnemy].parts[nType][nCntParts].nIdxModelParent;

				//�e�̃}�g���b�N�X�𔽉f
				if (nParent != -1)
				{//�e�̔ԍ������ꍇ
					mtxParent = g_aEnemy[nCntEnemy].parts[nType][nParent].mtxWorld;//�e���f���̃}�g���b�N�X
				}
				else
				{//�e�̔ԍ��������Ȃ��ꍇ
					mtxParent = g_aEnemy[nCntEnemy].mtxWorld;//�v���C���[�̃}�g���b�N�X
				}

				//�e�̃}�g���b�N�X���|�����킹��
				D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].parts[nType][nCntParts].mtxWorld, &g_aEnemy[nCntEnemy].parts[nType][nCntParts].mtxWorld, &mtxParent);

				//���[���h�}�g���b�N�X�̐ݒ�
				pDevice->SetTransform(D3DTS_WORLD, &g_aEnemy[nCntEnemy].parts[nType][nCntParts].mtxWorld);

				//���݂̃}�e���A�����擾
				pDevice->GetMaterial(&matDef);

				//�}�e���A���f�[�^�ւ̃|�C���^���擾
				pMat = (D3DXMATERIAL*)g_aEnemyParts[nType][nCntParts].pBuffMat->GetBufferPointer();

				for (int nCntMat = 0; nCntMat < (int)g_aEnemyParts[nType][nCntParts].dwNumMat; nCntMat++)
				{
					pMat[nCntMat].MatD3D.Diffuse = g_aEnemy[nCntEnemy].parts[nType][nCntParts].Diffuse[nCntMat];

					//�}�e���A���̐ݒ�
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

					//�e�N�X�`���̐ݒ�
					pDevice->SetTexture(0,g_aEnemyParts[g_aEnemy[nCntEnemy].nType][nCntParts].pTexture[nCntMat]);

					//���f��(�p�[�c)�̕`��
					g_aEnemyParts[nType][nCntParts].pMesh->DrawSubset(nCntMat);
				}

				//�ۑ����Ă����}�e���A����߂�
				pDevice->SetMaterial(&matDef);
			}
		}
	}
}

//================================
//�ݒ菈��
//================================
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, bool bSummonByBoss)//�ݒ菈��
{
	Field* pField = GetField();
	Camera* pCamera = GetCamera();
	Boss* pBoss = GetBoss();
	int nTimer = GetTimer();
	float fStatusBuff = 0.0f;

	//================================
	//�G�̃X�e�[�^�X�{��
	//================================
	for (int nCnt = 0; nCnt < 12; nCnt++)
	{
		if (nTimer >= 220 - (20 * nCnt) && nTimer <= MAX_TIME - (20 * nCnt))
		{
			fStatusBuff = 1.0f + (float)(nCnt) * 0.15f;//20�b���Ƃ�15%���G�̃X�e�[�^�X�̔{�����グ��
			break;
		}
	}

	if (pCamera->nMode == CAMERAMODE_REVOLUTION)
	{
		for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
		{
			if (g_aEnemy[nCntEnemy].bUse == false)
			{
				g_aEnemy[nCntEnemy].bUse = true;//�g�p���Ă��邩�ǂ���
				g_aEnemy[nCntEnemy].pos = pos;//�ʒu
				g_aEnemy[nCntEnemy].posOld = pos;//1F�O�̈ʒu
				g_aEnemy[nCntEnemy].rot = rot;//����
				g_aEnemy[nCntEnemy].nType = nType;//���
				g_aEnemy[nCntEnemy].fPenduram = 0.0f;//�U��q�̂悤�ɒe�𔭎˂���ϐ�
				g_aEnemy[nCntEnemy].bPenduramFlag = false;
				g_aEnemy[nCntEnemy].fLength = 0.0f;//�������Ƃ�
				g_aEnemy[nCntEnemy].bDefeatDropScrap = true;//�|���ꂽ�Ƃ��ɃX�N���b�v���h���b�v���邩�ǂ���

				//==================================
				//�G�̃X�e�[�^�X�ݒ�
				//==================================
				g_aEnemy[nCntEnemy].nHp =(int)(ENEMY_HP[nType] * fStatusBuff);     //�̗�
				g_aEnemy[nCntEnemy].nMaxHp = g_aEnemy[nCntEnemy].nHp;               //�G�̗̑͂̍ő�l
				g_aEnemy[nCntEnemy].nAttack = ENEMY_ATTACK[nType];                  //�U����
				g_aEnemy[nCntEnemy].nDefeatScore = ENEMY_SCORE[nType];              //�|�������ɂ��炦��X�R�A
				g_aEnemy[nCntEnemy].fSpeed = ENEMY_SPEED[nType] * fStatusBuff;      //����
				g_aEnemy[nCntEnemy].fFormarSpeed = ENEMY_SPEED[nType] * fStatusBuff;//���̑���
				g_aEnemy[nCntEnemy].fMoveStartLength = ENEMY_MOVESTART[nType];//�ړ����J�n���鋗��
				g_aEnemy[nCntEnemy].fAttackStartLength = ENEMY_ATTACKSTART[nType];//�U�����J�n���鋗��
				g_aEnemy[nCntEnemy].nAttackTiming = 0;//�U������̃^�C�~���O�����߂�
				g_aEnemy[nCntEnemy].bHitFlag = false;//�U�����q�b�g�������ǂ��������߂�ϐ�
				g_aEnemy[nCntEnemy].bUnderAttack = false;//�G���U�������ǂ��������߂�
				g_aEnemy[nCntEnemy].nNumLandingModel = -1;//�ǂ̃��f���̏�ɏ���Ă��邩

				if (g_aEnemy[nCntEnemy].pos.x > pField->fWidth)
				{
					g_aEnemy[nCntEnemy].pos.x = pField->fWidth;
				}
				if (g_aEnemy[nCntEnemy].pos.x < -pField->fWidth)
				{
					g_aEnemy[nCntEnemy].pos.x = -pField->fWidth;
				}
				if (g_aEnemy[nCntEnemy].pos.z > pField->fHeight)
				{
					g_aEnemy[nCntEnemy].pos.z = pField->fHeight;
				}
				if (g_aEnemy[nCntEnemy].pos.z < -pField->fHeight)
				{
					g_aEnemy[nCntEnemy].pos.z = -pField->fHeight;
				}

				for (int nCntAttackModel = 0; nCntAttackModel < MAX_ATTACKMODEL; nCntAttackModel++)
				{
					g_aEnemy[nCntEnemy].bHitStopBoomerang[nCntAttackModel] = false;//�u�[�������ɑ΂���q�b�g�X�g�b�v���������
					g_aEnemy[nCntEnemy].nCoolTimeBoomerang[nCntAttackModel] = 0;//�u�[�������ɑ΂���q�b�g�X�g�b�v�̎��Ԃ��J�E���g����
				}

				//============================
				//�U���󂯕t������
				//============================
				for (int nCntHitStop = 0; nCntHitStop < PLAYERATTACK_MAX; nCntHitStop++)
				{
					g_aEnemy[nCntEnemy].bHitStop[nCntHitStop] = false;//���̕ϐ���true�̊ԁA�U�����󂯕t���Ȃ�
					g_aEnemy[nCntEnemy].nCoolTime[nCntHitStop] = 0;   //bHitStop��false�ɂ���܂ł̃J�E���g
				}

				//========================================
				//�G�̕\���I�v�V�����̕\���̕ϐ�
				//========================================
				g_aEnemy[nCntEnemy].fOptionDispLength = ENEMYDISPLENGTH[nType];//�G�̃I�v�V�����̕\�����J�n���鋗��
				g_aEnemy[nCntEnemy].bUseEnemyDisplay = false;//�G�̃I�v�V�����\�����g�p���邩�ǂ���

				//=================================
				//����ւ̑ϐ�����
				//=================================
				switch (nType)
				{
				case 0://���b�h�u��
					g_aEnemy[nCntEnemy].bWeakWeapon[0] = true;//���i�ア�j
					g_aEnemy[nCntEnemy].bWeakWeapon[1] = false;//�u�[������
					g_aEnemy[nCntEnemy].bWeakWeapon[2] = false;//�X�s�[�J�[
					break;
				case 1://�~���^���N
					g_aEnemy[nCntEnemy].bWeakWeapon[0] = false;//��
					g_aEnemy[nCntEnemy].bWeakWeapon[1] = true;//�u�[�������i�ア�j
					g_aEnemy[nCntEnemy].bWeakWeapon[2] = false;//�X�s�[�J�[
					break;
				case 2://�~�b�N�`���W���[�`��
					g_aEnemy[nCntEnemy].bWeakWeapon[0] = false;//��
					g_aEnemy[nCntEnemy].bWeakWeapon[1] = true;//�u�[�������i�ア�j
					g_aEnemy[nCntEnemy].bWeakWeapon[2] = false;//�X�s�[�J�[
					break;
				case 3://�����X�^�[
					g_aEnemy[nCntEnemy].bWeakWeapon[0] = true;//���i�ア�j
					g_aEnemy[nCntEnemy].bWeakWeapon[1] = false;//�u�[������
					g_aEnemy[nCntEnemy].bWeakWeapon[2] = false;//�X�s�[�J�[
					break;
				case 4://�J�b�v��
					g_aEnemy[nCntEnemy].bWeakWeapon[0] = false;//��
					g_aEnemy[nCntEnemy].bWeakWeapon[1] = false;//�u�[������
					g_aEnemy[nCntEnemy].bWeakWeapon[2] = true;//�X�s�[�J�[
					break;

				default:
					break;
				}
				
				//===================================================
				//���bUse��false�ɂȂ�Ȃ�������A���������s
				//===================================================
				if (g_aEnemy[nCntEnemy].bUse == true)
				{
					g_aEnemy[nCntEnemy].nIdxShadow = SetShadow();//�e�̐ݒ�
					g_aEnemy[nCntEnemy].nIdxRaderDisplay = SetRaderDisplay(RADERDISPLAY01_ENEMY);//���[�_�[�f�B�X�v���C�̃C���f�b�N�X
					g_aEnemy[nCntEnemy].bSummonByBoss = bSummonByBoss;//�{�X�ɂ���ď������ꂽ���ǂ���

					if (g_aEnemy[nCntEnemy].bSummonByBoss == true)
					{
						pBoss->nCntSummonEnemy++;//�{�X�ɂ���ď������ꂽ�G���J�E���g����
					}

					//HP�Q�[�W�̐ݒ�
					SetEnemyGauge(nCntEnemy, D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].vtxMax.y, g_aEnemy[nCntEnemy].pos.z));

					for (int nCntWeak = 0; nCntWeak < 3; nCntWeak++)
					{
						if (g_aEnemy[nCntEnemy].bWeakWeapon[nCntWeak] == true)
						{
							//��_�\�L�̐ݒ�
							SetEnemyWeakDisp(nCntWeak, nCntEnemy, NULL_VECTOR3);
						}
					}

					//�G�̊e�p�[�c�̐ݒ�
					for (int nCntParts = 0; nCntParts < ENEMY_NUMPARTS[nType]; nCntParts++)
					{
						g_aEnemy[nCntEnemy].parts[nType][nCntParts].bUse = true;//�g�p����
						g_aEnemy[nCntEnemy].parts[nType][nCntParts].pos = g_aEnemyParts[nType][nCntParts].pos;//�ʒu
						g_aEnemy[nCntEnemy].parts[nType][nCntParts].PartsPos = NULL_VECTOR3;//�p�[�c���Ƃ̈ʒu
						g_aEnemy[nCntEnemy].parts[nType][nCntParts].rot = g_aEnemyParts[nType][nCntParts].rot;//����
						g_aEnemy[nCntEnemy].parts[nType][nCntParts].nIdxModelParent = g_aEnemyParts[nType][nCntParts].nIdxModelParent;//�e�̔ԍ�
						//g_aEnemy[nCntEnemy].parts[nType][nCntParts].pBuffMat = g_aEnemyParts[nType][nCntParts].pBuffMat;//�}�e���A���ւ̃|�C���^
						//g_aEnemy[nCntEnemy].parts[nType][nCntParts].dwNumMat = g_aEnemyParts[nType][nCntParts].dwNumMat;//�}�e���A���̐�
						//g_aEnemy[nCntEnemy].parts[nType][nCntParts].pMesh = g_aEnemyParts[nType][nCntParts].pMesh;//���b�V���i���_���j
					}

					//======================================
					//�e��ނ̃T�C�Y�����߂�
					//======================================
					BYTE* pVtxBuff;	//���_�o�b�t�@�ւ̃|�C���^

					for (int nCntParts = 0; nCntParts < ENEMY_NUMPARTS[nType]; nCntParts++)
					{
						//���_���̎擾
						int nNumVtx = g_aEnemyParts[nType][nCntParts].pMesh->GetNumVertices();

						//���_�t�H�[�}�b�g�̃T�C�Y���擾
						DWORD sizeFVF = D3DXGetFVFVertexSize(g_aEnemyParts[nType][nCntParts].pMesh->GetFVF());

						//���_�o�b�t�@�̃��b�N
						g_aEnemyParts[nType][nCntParts].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

						for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
						{
							//���_���W�̑��
							D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

							//���f���̍ŏ��l�ƍő�l���擾
							if (vtx.x < g_aEnemy[nCntEnemy].vtxMin.x)
							{//X���W�̍ŏ��l
								g_aEnemy[nCntEnemy].vtxMin.x = vtx.x;
							}
							if (vtx.y < g_aEnemy[nCntEnemy].vtxMin.y)
							{//Y���W�̍ŏ��l
								g_aEnemy[nCntEnemy].vtxMin.y = vtx.y;
							}
							if (vtx.z < g_aEnemy[nCntEnemy].vtxMin.z)
							{//Z���W�̍ŏ��l
								g_aEnemy[nCntEnemy].vtxMin.z = vtx.z;
							}
							if (vtx.x > g_aEnemy[nCntEnemy].vtxMax.x)
							{//X���W�̍ő�l
								g_aEnemy[nCntEnemy].vtxMax.x = vtx.x;
							}
							if (vtx.y > g_aEnemy[nCntEnemy].vtxMax.y)
							{//Y���W�̍ő�l
								g_aEnemy[nCntEnemy].vtxMax.y = vtx.y;
							}
							if (vtx.z > g_aEnemy[nCntEnemy].vtxMax.z)
							{//Z���W�̍ő�l
								g_aEnemy[nCntEnemy].vtxMax.z = vtx.z;
							}

							//���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
							pVtxBuff += sizeFVF;
						}

						//�G�̑傫�������߂�B�i�Ŗ��I���W�i������)
						g_aEnemy[nCntEnemy].Size = g_aEnemy[nCntEnemy].vtxMax - g_aEnemy[nCntEnemy].vtxMin;

						//���_�o�b�t�@�̃A�����b�N
						g_aEnemyParts[nType][nCntParts].pMesh->UnlockVertexBuffer();
					}
				}

				break;
			}
		}
	}
}

//=======================================================================================================================================================================================================
// �G�̃p�[�c���̓ǂݍ���
//=======================================================================================================================================================================================================
void LoadEnemy(void)
{
	char aDataSearch[MAX_TEXT];
	memset(&aDataSearch[0], NULL, sizeof(aDataSearch));

	for (int nCntEnemy = 0; nCntEnemy < NUM_ENEMY; nCntEnemy++)
	{
		int nCntParts = 0;

		//�t�@�C�����J��
		FILE* pFile = fopen(ENEMY_FILE[nCntEnemy], "r");

		//�t�@�C�����J���Ȃ��ꍇ
		if (pFile == NULL)
		{
			return;
		}

		//END��������܂œǂݍ��݂��J��Ԃ�
		while (1)
		{
			fscanf(pFile, "%s", aDataSearch);	//����

			//END_PARTS�����������ꍇ
			if (!strcmp(aDataSearch, "END_PARTS"))
			{//�ǂݍ��ݏI��
				fclose(pFile);
				break;
			}

			//�R�����g�A�E�g�����������ꍇ
			if (aDataSearch[0] == '#')
			{//�X�L�b�v���ēǂݍ��݂𑱂���
				continue;
			}

			//���f���Z�b�g�����������ꍇ
			if (!strcmp(aDataSearch, "PARTSSET"))
			{
				//���ڂ��Ƃ̃f�[�^����
				while (1)
				{
					fscanf(pFile, "%s", aDataSearch);	//����

					//ENDMODEL�Z�b�g�����������ꍇ
					if (!strcmp(aDataSearch, "END_PARTSSET"))
					{//���f���f�[�^�̓ǂݍ��݂��I��
						break;
					}
					//�f�[�^�̓ǂݍ���
					else if (!strcmp(aDataSearch, "POSx"))
					{//�ʒu��X���W
						fscanf(pFile, "%f", &g_aEnemyParts[nCntEnemy][nCntParts].pos.x);
					}
					else if (!strcmp(aDataSearch, "POSy"))
					{//�ʒu��Y���W
						fscanf(pFile, "%f", &g_aEnemyParts[nCntEnemy][nCntParts].pos.y);
					}
					else if (!strcmp(aDataSearch, "POSz"))
					{//�ʒu��Z���W
						fscanf(pFile, "%f", &g_aEnemyParts[nCntEnemy][nCntParts].pos.z);
					}
					else if (!strcmp(aDataSearch, "ROTx"))
					{//������X���W
						fscanf(pFile, "%f", &g_aEnemyParts[nCntEnemy][nCntParts].rot.x);
					}
					else if (!strcmp(aDataSearch, "ROTy"))
					{//������Y���W
						fscanf(pFile, "%f", &g_aEnemyParts[nCntEnemy][nCntParts].rot.y);
					}
					else if (!strcmp(aDataSearch, "ROTz"))
					{//������Z���W
						fscanf(pFile, "%f", &g_aEnemyParts[nCntEnemy][nCntParts].rot.z);
					}
					else if (!strcmp(aDataSearch, "TYPE"))
					{//���
						fscanf(pFile, "%d", &g_aEnemyParts[nCntEnemy][nCntParts].nType);
					}
					else if (!strcmp(aDataSearch, "PARENT"))
					{//�e�̔ԍ�
						fscanf(pFile, "%d", &g_aEnemyParts[nCntEnemy][nCntParts].nIdxModelParent);
					}
				}

				nCntParts++;//�p�[�c�������Z
			}
		}
	}
}

//============================================================================================================================================================================================================
// ���[�V�������̓ǂݍ��ݏ���
//============================================================================================================================================================================================================
void LoadEnemyMotion(int nType)
{
	int nMotion = 0;						//���[�V�����̎�ޔԍ�
	int nCntParts = 0;						//�p�[�c��
	int nCntKey[NUM_ENEMY][MAX_ENEMYMOTION];//�e���[�V�����L�[��
	char aDataSearch[MAX_TEXT];				//�f�[�^�����p

	//�e���[�V�����L�[���̏�����
	for (int nCnt = 0; nCnt < MAX_ENEMYMOTION; nCnt++)
	{
		nCntKey[nType][nCnt] = 0;
	}

	//�t�@�C�����J��
	FILE* pFile = fopen(ENEMY_FILE[nType], "r");

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
			//���[�V�����̎�ނ�ǂݍ���
			fscanf(pFile, "%d", &nMotion);

			//END_MOTIONSET��������܂œǂݍ��݂��J��Ԃ�
			while (1)
			{
				fscanf(pFile, "%s", aDataSearch);	//����

				//END_MOTIONSET�����������ꍇ
				if (!strcmp(aDataSearch, "END_MOTIONSET"))
				{//���[�V�����ǂݍ��ݏI��
					break;
				}

				//�R�����g�A�E�g�����������ꍇ
				if (aDataSearch[0] == '#')
				{//�X�L�b�v���ēǂݍ��݂𑱂���
					continue;
				}

				//���[�v���肪���������ꍇ
				if (!strcmp(aDataSearch, "LOOP"))
				{//���[�v���邩�ǂ������擾
					fscanf(pFile, "%d", &g_aEnemyMotionSet[nType][nMotion].nMotionLoop);
				}

				//�L�[�Z�b�g�����������ꍇ
				if (!strcmp(aDataSearch, "KEYSET"))
				{
					while (1)
					{
						fscanf(pFile, "%s", aDataSearch);	//����

						//�R�����g�A�E�g�����������ꍇ
						if (aDataSearch[0] == '#')
						{//�X�L�b�v���ēǂݍ��݂𑱂���
							continue;
						}

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
									fscanf(pFile, "%d", &g_aEnemyKeySet[nType][nCntKey[nType][nMotion]][nMotion].nMaxFrame);
								}

								//�f�[�^�̓ǂݍ���
								else if (!strcmp(aDataSearch, "POSx"))
								{//�ʒu��X���W
									fscanf(pFile, "%f", &g_aEnemyKeySet[nType][nCntKey[nType][nMotion]][nMotion].key[nCntParts].pos.x);
								}
								else if (!strcmp(aDataSearch, "POSy"))
								{//�ʒu��Y���W
									fscanf(pFile, "%f", &g_aEnemyKeySet[nType][nCntKey[nType][nMotion]][nMotion].key[nCntParts].pos.y);
								}
								else if (!strcmp(aDataSearch, "POSz"))
								{//�ʒu��Z���W
									fscanf(pFile, "%f", &g_aEnemyKeySet[nType][nCntKey[nType][nMotion]][nMotion].key[nCntParts].pos.z);
								}
								else if (!strcmp(aDataSearch, "ROTx"))
								{//������X���W
									fscanf(pFile, "%f", &g_aEnemyKeySet[nType][nCntKey[nType][nMotion]][nMotion].key[nCntParts].rot.x);
								}
								else if (!strcmp(aDataSearch, "ROTy"))
								{//������Y���W
									fscanf(pFile, "%f", &g_aEnemyKeySet[nType][nCntKey[nType][nMotion]][nMotion].key[nCntParts].rot.y);
								}
								else if (!strcmp(aDataSearch, "ROTz"))
								{//������Z���W
									fscanf(pFile, "%f", &g_aEnemyKeySet[nType][nCntKey[nType][nMotion]][nMotion].key[nCntParts].rot.z);
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
						if (!strcmp(aDataSearch, "END_KEYSET"))
						{
							nCntParts = 0;
							nCntKey[nType][nMotion]++;//�L�[�������Z
							g_aEnemyMotionSet[nType][nMotion].nMaxEnemyKey = nCntKey[nType][nMotion];//���v�L�[����ۑ�
							break;
						}
					}
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
void EnemyMotion(int nCnt)
{
	//g_aEnemy[nCnt].state = ENEMYSTATE_ATTACK;

	int nType = g_aEnemy[nCnt].nType;		//�G�̎�ނ�ۑ�
	int nState = (int)(g_aEnemy[nCnt].state);	//�G�̏�Ԃ�ۑ�
	int nCntKey = g_aEnemy[nCnt].motionset[nType][nState].nCntEnemyKey;															//���݂̃L�[��ۑ�
	int nNextKey = (nCntKey + 1) % g_aEnemyMotionSet[nType][nState].nMaxEnemyKey;												//���̃L�[�����߂�
	float fFrame = (float)g_aEnemy[nCnt].motionset[nType][nState].nCntFrame / g_aEnemyKeySet[nType][nCntKey][nState].nMaxFrame;	//���݂̃t���[���ƑS�̃t���[���̊���

	//�p�[�c���Ƃɏ����X�V
	for (int nCntParts = 0; nCntParts < ENEMY_NUMPARTS[nType]; nCntParts++)
	{
		float EnemyPosX = g_aEnemyKeySet[nType][nCntKey][nState].key[nCntParts].pos.x;
		float EnemyPosY = g_aEnemyKeySet[nType][nCntKey][nState].key[nCntParts].pos.y;
		float EnemyPosZ = g_aEnemyKeySet[nType][nCntKey][nState].key[nCntParts].pos.z;
		float EnemyRotX = g_aEnemyKeySet[nType][nCntKey][nState].key[nCntParts].rot.x;
		float EnemyRotY = g_aEnemyKeySet[nType][nCntKey][nState].key[nCntParts].rot.y;
		float EnemyRotZ = g_aEnemyKeySet[nType][nCntKey][nState].key[nCntParts].rot.z;

		float NextEnemyPosX = g_aEnemyKeySet[nType][nNextKey][nState].key[nCntParts].pos.x;
		float NextEnemyPosY = g_aEnemyKeySet[nType][nNextKey][nState].key[nCntParts].pos.y;
		float NextEnemyPosZ = g_aEnemyKeySet[nType][nNextKey][nState].key[nCntParts].pos.z;
		float NextEnemyRotX = g_aEnemyKeySet[nType][nNextKey][nState].key[nCntParts].rot.x;
		float NextEnemyRotY = g_aEnemyKeySet[nType][nNextKey][nState].key[nCntParts].rot.y;
		float NextEnemyRotZ = g_aEnemyKeySet[nType][nNextKey][nState].key[nCntParts].rot.z;

		//���̃L�[�܂ł̍��������߂�
		float fPosx = NextEnemyPosX - EnemyPosX;	// X���W
		float fPosy = NextEnemyPosY - EnemyPosY;	// Y���W
		float fPosz = NextEnemyPosZ - EnemyPosZ;	// Z���W
		float fRotx = NextEnemyRotX - EnemyRotX;	// X���̌���
		float fRoty = NextEnemyRotY - EnemyRotY;	// Y���̌���
		float fRotz = NextEnemyRotZ - EnemyRotZ;	// Z���̌���

		//�������X�V
		g_aEnemy[nCnt].parts[nType][nCntParts].rot.x = g_aEnemyParts[nType][nCntParts].rot.x + EnemyRotX + fRotx * fFrame;//X��
		g_aEnemy[nCnt].parts[nType][nCntParts].rot.y = g_aEnemyParts[nType][nCntParts].rot.y + EnemyRotY + fRoty * fFrame;//Y��
		g_aEnemy[nCnt].parts[nType][nCntParts].rot.z = g_aEnemyParts[nType][nCntParts].rot.z + EnemyRotZ + fRotz * fFrame;//Z��

		//�ʒu���X�V
		g_aEnemy[nCnt].parts[nType][nCntParts].pos.x = g_aEnemyParts[nType][nCntParts].pos.x + EnemyPosX + fPosx * fFrame;//X���W
		g_aEnemy[nCnt].parts[nType][nCntParts].pos.y = g_aEnemyParts[nType][nCntParts].pos.y + EnemyPosY + fPosy * fFrame;//Y���W
		g_aEnemy[nCnt].parts[nType][nCntParts].pos.z = g_aEnemyParts[nType][nCntParts].pos.z + EnemyPosZ + fPosz * fFrame;//Z���W
	}

	//�L�[���̍X�V
	if (g_aEnemy[nCnt].motionset[nType][nState].nCntFrame == 0 || g_aEnemy[nCnt].motionset[nType][nState].nCntFrame >= g_aEnemyKeySet[nType][nCntKey][nState].nMaxFrame)
	{
		if (g_aEnemy[nCnt].motionset[nType][nState].nCntFrame >= g_aEnemyKeySet[nType][nCntKey][nState].nMaxFrame)
		{
			//�t���[�����J�E���^�����Z�b�g
			g_aEnemy[nCnt].motionset[nType][nState].nCntFrame = 0;
		}

		//�L�[�������Z
		g_aEnemy[nCnt].motionset[nType][nState].nCntEnemyKey++;

		//�L�[�����ő吔�ɂȂ���
		if (g_aEnemy[nCnt].motionset[nType][nState].nCntEnemyKey >= g_aEnemyMotionSet[nType][nState].nMaxEnemyKey)
		{
			//���[�v���Ȃ��ꍇ
			if (g_aEnemyMotionSet[nType][nState].nMotionLoop == 1)
			{
				g_aEnemy[nCnt].nAttackTiming = 0;//�U�����I����������܂܂��̂ŁA�U���J�E���g���O�ɂ���
				g_aEnemy[nCnt].bHitFlag = false;//�U������������̂ŁA�q�b�g��������Z�b�g
				g_aEnemy[nCnt].bUnderAttack = false;//�U������������̂ŁA�U�����̔�������Z�b�g
				g_aEnemy[nCnt].bCompulsionAttack = false;//�����I�ɍU����Ԃɂ���
				//�j���[�g�������
				g_aEnemy[nCnt].state = ENEMYSTATE_NEUTRAL;
			}
			
			//�L�[����������
			g_aEnemy[nCnt].motionset[nType][nState].nCntEnemyKey = 0;
		}
	}

	//�t���[�����X�V
	g_aEnemy[nCnt].motionset[nType][nState].nCntFrame++;
}

//======================================
// �\���̂̏����擾
//======================================
Enemy* GetEnemy(void)
{
	return &g_aEnemy[0];
}

//======================================
// �_���[�W����
//======================================
void SetDamageEnemy(int nDamage, int nNumEnemy, PLAYERATTACK PlayerAttackNum, int nNumAttackModel)
{
	Player* pPlayer = GetPlayer();
	float fDamageResult = (float)(nDamage);
	bool bCritical = false;

	if (g_aEnemy[nNumEnemy].bWeakWeapon[PlayerAttackNum] == true)
	{
		fDamageResult = (float)(nDamage) * 2.5f;
		bCritical = true;

		PlaySound(SOUND_LABEL_SE_CRITICAL_000);
	}

	if (g_aEnemy[nNumEnemy].bHitStop[PlayerAttackNum] == false)//�q�b�g�X�g�b�v��Ԃ���Ȃ����
	{
		if (PlayerAttackNum == PLAYERATTACK00_SWORD || PlayerAttackNum == PLAYERATTACK02_SPEAKER)
		{
			if (bCritical == false)
			{
				g_aEnemy[nNumEnemy].move.x += sinf(pPlayer->rot.y + D3DX_PI) * 10.0f;
				g_aEnemy[nNumEnemy].move.z += cosf(pPlayer->rot.y + D3DX_PI) * 10.0f;
			}

			g_aEnemy[nNumEnemy].nHp -= (int)(fDamageResult);//�̗͂����炷
			g_aEnemy[nNumEnemy].bHitStop[PlayerAttackNum] = true;  //�_���[�W��^�����U���ɑ΂��ăq�b�g�X�g�b�v��Ԃɂ���

			for (int nCntParticle = 0; nCntParticle < 100; nCntParticle++)
			{
				float fRot = float(rand() % 200 + 1) / 50;
				float fYmove = float(rand() % 200 - 100) / 10;
				float fSpeed = float(rand() % 200 - 100) / 10;

				SetParticle(PARTICLETYPE_NORMAL, 300, 10.0f, 0.6f, g_aEnemy[nNumEnemy].pos,
					D3DXVECTOR3(sinf(-D3DX_PI * fRot) * fSpeed, fYmove, cosf(-D3DX_PI * fRot) * fSpeed),
					D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 0.0f, PARTICLETEX_NORMAL,0);
			}

			//�N���e�B�J���Ȃ�
			if (bCritical == true)
			{
				for (int nCntParticle = 0; nCntParticle < 50; nCntParticle++)
				{
					float fRot = float(rand() % 200 + 1) / 100;
					float fYmove = float(rand() % 200 - 100) / 10;
					float fSpeed = float(rand() % 200 - 100) / 10;

					SetParticle(PARTICLETYPE_NORMAL, 300, 10.0f, 0.9f, g_aEnemy[nNumEnemy].pos,
						D3DXVECTOR3(sinf(-D3DX_PI * fRot) * (fSpeed * 2.0f), fYmove, cosf(-D3DX_PI * fRot) * (fSpeed * 2.0f)),
						D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), 0.0f, PARTICLETEX_NORMAL, 0);
				}

				if (PlayerAttackNum == PLAYERATTACK00_SWORD)
				{
					g_aEnemy[nNumEnemy].move.x += sinf(pPlayer->rot.y + D3DX_PI) * 20.0f;
					g_aEnemy[nNumEnemy].move.z += cosf(pPlayer->rot.y + D3DX_PI) * 20.0f;
				}
				else
				{
					g_aEnemy[nNumEnemy].move.x += sinf(pPlayer->rot.y + D3DX_PI) * 3.0f;
					g_aEnemy[nNumEnemy].move.z += cosf(pPlayer->rot.y + D3DX_PI) * 3.0f;

				}
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
	if (g_aEnemy[nNumEnemy].bHitStopBoomerang[nNumAttackModel] == false && PlayerAttackNum == PLAYERATTACK01_BOOMERANG)
	{
	    g_aEnemy[nNumEnemy].nHp -= (int)(fDamageResult);//�̗͂����炷
	    g_aEnemy[nNumEnemy].bHitStopBoomerang[nNumAttackModel] = true;  //�_���[�W��^�����U���ɑ΂��ăq�b�g�X�g�b�v��Ԃɂ���
	    
	    for (int nCntParticle = 0; nCntParticle < 50; nCntParticle++)
	    {
	    	float fRot = float(rand() % 200 + 1) / 100;
	    	float fYmove = float(rand() % 200 - 100) / 10;
	    	float fSpeed = float(rand() % 200 - 100) / 10;
	    
	    	SetParticle(PARTICLETYPE_NORMAL, 300, 10.0f, 0.6f, g_aEnemy[nNumEnemy].pos,
	    		D3DXVECTOR3(sinf(-D3DX_PI * fRot) * fSpeed, fYmove, cosf(-D3DX_PI * fRot) * fSpeed),
	    		D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 0.0f, PARTICLETEX_NORMAL,0);
	    }

		
		//�N���e�B�J���Ȃ�
		if (bCritical == true)
		{
			for (int nCntParticle = 0; nCntParticle < 50; nCntParticle++)
			{
				float fRot = float(rand() % 200 + 1) / 100;
				float fYmove = float(rand() % 200 - 100) / 10;
				float fSpeed = float(rand() % 200 - 100) / 10;

				SetParticle(PARTICLETYPE_NORMAL, 300, 10.0f, 0.6f, g_aEnemy[nNumEnemy].pos,
					D3DXVECTOR3(sinf(-D3DX_PI * fRot) * (fSpeed * 2.0f), fYmove, cosf(-D3DX_PI * fRot) * (fSpeed * 2.0f)),
					D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), 0.0f, PARTICLETEX_NORMAL, 0);
			}
		}
	    	PlaySound(SOUND_LABEL_SE_HITBOOMERANG_000);
	}

	g_nLastDamageEnemy = (int)(fDamageResult);
}

//======================================
// �L������
//======================================
void KillEnemy(void)
{
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{
			g_aEnemy[nCntEnemy].bUse = false;
			KillShadow(g_aEnemy[nCntEnemy].nIdxShadow);
		}
	}
}

//=======================================
//�G�̃}�g���b�N�X���v�Z���鏈��
//=======================================
void CalculateMatrixEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�ւ̃|�C���^�Ǝ擾
	D3DXMATRIX mtxRot, mtxTrans, mtxParent;//�v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;//���݂̃}�e���A���ۑ��p
	//D3DXMATERIAL* pMat;//�}�e���A���f�[�^�ւ̃|�C���^

	//���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		//�g�p���Ă���ꍇ
		if (g_aEnemy[nCntEnemy].bUse == true)
		{
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aEnemy[nCntEnemy].mtxWorld);

			//�����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aEnemy[nCntEnemy].rot.y, g_aEnemy[nCntEnemy].rot.x, g_aEnemy[nCntEnemy].rot.z);
			D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].mtxWorld, &g_aEnemy[nCntEnemy].mtxWorld, &mtxRot);

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y, g_aEnemy[nCntEnemy].pos.z);
			D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].mtxWorld, &g_aEnemy[nCntEnemy].mtxWorld, &mtxTrans);

			//��ޕۑ�
			int nType = g_aEnemy[nCntEnemy].nType;

			for (int nCntParts = 0; nCntParts < ENEMY_NUMPARTS[nType]; nCntParts++)
			{
				//�e�p�[�c���f���̃��[���h�}�g���b�N�X�̏�����
				D3DXMatrixIdentity(&g_aEnemy[nCntEnemy].parts[nType][nCntParts].mtxWorld);

				//�e�p�[�c���f���̌����𔽉f
				D3DXMatrixRotationYawPitchRoll(&mtxRot,
					g_aEnemy[nCntEnemy].parts[nType][nCntParts].rot.y,
					g_aEnemy[nCntEnemy].parts[nType][nCntParts].rot.x,
					g_aEnemy[nCntEnemy].parts[nType][nCntParts].rot.z);

				D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].parts[nType][nCntParts].mtxWorld, &g_aEnemy[nCntEnemy].parts[nType][nCntParts].mtxWorld, &mtxRot);

				//�e�p�[�c���f���̈ʒu�𔽉f
				D3DXMatrixTranslation(&mtxTrans,
					g_aEnemy[nCntEnemy].parts[nType][nCntParts].pos.x,
					g_aEnemy[nCntEnemy].parts[nType][nCntParts].pos.y,
					g_aEnemy[nCntEnemy].parts[nType][nCntParts].pos.z);

				D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].parts[nType][nCntParts].mtxWorld, &g_aEnemy[nCntEnemy].parts[nType][nCntParts].mtxWorld, &mtxTrans);

				//�e�ԍ��̕ۑ�
				int nParent = g_aEnemy[nCntEnemy].parts[nType][nCntParts].nIdxModelParent;

				//�e�̃}�g���b�N�X�𔽉f
				if (nParent != -1)
				{//�e�̔ԍ������ꍇ
					mtxParent = g_aEnemy[nCntEnemy].parts[nType][nParent].mtxWorld;//�e���f���̃}�g���b�N�X
				}
				else
				{//�e�̔ԍ��������Ȃ��ꍇ
					mtxParent = g_aEnemy[nCntEnemy].mtxWorld;//�v���C���[�̃}�g���b�N�X
				}

				//�e�̃}�g���b�N�X���|�����킹��
				D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].parts[nType][nCntParts].mtxWorld, &g_aEnemy[nCntEnemy].parts[nType][nCntParts].mtxWorld, &mtxParent);

				//���[���h�}�g���b�N�X�̐ݒ�
				pDevice->SetTransform(D3DTS_WORLD, &g_aEnemy[nCntEnemy].parts[nType][nCntParts].mtxWorld);

				D3DXVECTOR3 posZero = D3DXVECTOR3(0.0f,0.0f,0.0f);

				D3DXVec3TransformCoord(&g_aEnemy[nCntEnemy].parts[nType][nCntParts].PartsPos, &posZero,&g_aEnemy[nCntEnemy].parts[nType][nCntParts].mtxWorld);
			}
		}
	}
}
