//================================================================================================================================================================================================================================
//
// �U�����f���̏���
// Author : Taiyo Shina
//
//================================================================================================================================================================================================================================
#include "attackModel.h"
#include "game.h"
#include "input.h"
#include "enemy.h"
#include "player.h"
#include "particle.h"
#include "effect.h"
#include "battery.h"
#include "boss.h"
#include <math.h>
#include "model.h"
#include "emitter.h"
#include "dummy.h"
#include "field.h"
#include "shadow.h"
#include "sound.h"

//================================================================================================================================================================================================================================
// �O���[�o���ϐ�
//================================================================================================================================================================================================================================
LPDIRECT3DTEXTURE9 g_apTextureAttackModel[NUM_ATTACKMODEL][MAX_ATTACKMODELMAT] = {};//�e�N�X�`���ւ̃|�C���^
AttackModel g_aAttackModel[MAX_ATTACKMODEL];										//�I�u�W�F�N�g���
bool g_bIsLandingAttackModel;														//���f���̏�ɂ��邩�ǂ����𔻒肷��
D3DXVECTOR3 g_PlayerPosLength;														//�v���C���[�̈ʒu�̐�Βl�����߂�
int g_nDelayAttackModel;															//�f�B���C

AttackModelInfo g_aAttackModelInfo[NUM_ATTACKMODEL];                                //�U�����f���̏��

//================================================================================================================================================================================================================================
// ����������
//================================================================================================================================================================================================================================
void InitAttackModel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̎擾
	D3DXMATERIAL* pMat;						//�}�e���A���ւ̃|�C���^

	//���f�����̏�����
	for (int nCntModelInfo = 0; nCntModelInfo < NUM_ATTACKMODEL; nCntModelInfo++)
	{
		g_aAttackModelInfo[nCntModelInfo].dwNumMat = {};
		g_aAttackModelInfo[nCntModelInfo].pBuffMat = {};
		g_aAttackModelInfo[nCntModelInfo].pMesh = {};

		for (int nCntMat = 0; nCntMat < MAX_ATTACKMODELMAT; nCntMat++)
		{
			g_aAttackModelInfo[nCntModelInfo].pTexture[nCntMat] = {};
		}
	}

	//================================
	//�\���̂̏��̏�����
	//================================
	for (int nCntAttackModel = 0; nCntAttackModel < MAX_ATTACKMODEL; nCntAttackModel++)
	{
		g_aAttackModel[nCntAttackModel].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�ʒu
		g_aAttackModel[nCntAttackModel].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�ړ���
		g_aAttackModel[nCntAttackModel].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//����
		g_aAttackModel[nCntAttackModel].Vrot = NULL_VECTOR3;                    //�x�N�g�����l����������
		g_aAttackModel[nCntAttackModel].Scale = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  //�g�嗦
		g_aAttackModel[nCntAttackModel].fRot = 0.0f;                            //�������������߂�
		g_aAttackModel[nCntAttackModel].nType = 0;								//���
		g_aAttackModel[nCntAttackModel].bUse = false;							//�g�p���Ă���
		g_aAttackModel[nCntAttackModel].nLife = 0;                              //�̗�
		g_aAttackModel[nCntAttackModel].nMaxLife = 0;                           //�ő�̗�
		g_aAttackModel[nCntAttackModel].SupportPos = NULL_VECTOR3;              //�x�_�ƂȂ�ʒu
		g_aAttackModel[nCntAttackModel].fSaveRot = 0.0f;                        //�v���C���[���u�[�������𓊂����������L�^����
		g_aAttackModel[nCntAttackModel].nCntTime = 0;                           //�U�����o�����Ă���̎��Ԃ��J�E���g����
		g_aAttackModel[nCntAttackModel].fTotalRotMove = 0.0f;                   //�v���C���[���u�[�������𓊂��Ă����]�ړ����������v��
		g_aAttackModel[nCntAttackModel].SupportPosPlayer = NULL_VECTOR3;        //�v���C���[���猩���x�_�ƂȂ�ʒu
		g_aAttackModel[nCntAttackModel].nBehaviorType = 0;                      //�U���̋����̎��
		g_aAttackModel[nCntAttackModel].bTurnFlag = false;                      //�u�[���������^�[��������t���O
		g_aAttackModel[nCntAttackModel].PosLength = NULL_VECTOR3;               //���镨�̂Ƃ̋��������߂鎞�Ɏg��
		g_aAttackModel[nCntAttackModel].fVaim = 0.0f;                           //�x�N�g��
		g_aAttackModel[nCntAttackModel].fVXaim = 0.0f;                          //X�����̃x�N�g��
		g_aAttackModel[nCntAttackModel].fVZaim = 0.0f;                          //Z�����̃x�N�g��
		g_aAttackModel[nCntAttackModel].Size = NULL_VECTOR3;                    //�傫��
		g_aAttackModel[nCntAttackModel].bIsLanding = false;                     //�n�ʂɂ��邩�ǂ���
		g_aAttackModel[nCntAttackModel].PosOld = NULL_VECTOR3;                  //1f�O�̈ʒu
		g_aAttackModel[nCntAttackModel].fSpeed = 0.0f;                          //�X�s�[�h
		g_aAttackModel[nCntAttackModel].nNumLandingModel = 0;                   //�ǂ̃��f���̏�ɏ���Ă��邩
		g_aAttackModel[nCntAttackModel].nTurnCnt = 0;                           //�u�[���������^�[�����J�n������A�J�E���g���J�n
		g_aAttackModel[nCntAttackModel].nIdxShadow = -1;                        //�e�̃C���f�b�N�X
		g_aAttackModel[nCntAttackModel].bCurveLaser = false;                    //���[�U�[���J�[�u��������������߂�
		g_aAttackModel[nCntAttackModel].fBoomerangRange = 0.0f;                 //�u�[���������������ꂽ���_�ł̎˒���ۑ�����
		for (int nCntAim = 0; nCntAim < 4; nCntAim++)
		{
			g_aAttackModel[nCntAttackModel].fSupportVaim[nCntAim] = 0.0f;       //�x�_�ƂȂ�x�N�g��
		}
		//=============================
		//�X�t�B�A�ϐ�
		//=============================
		g_aAttackModel[nCntAttackModel].bSphereShot = false;                    //���C�t�������ɂȂ����玩�@�_���œ�����

		//=============================
		//�O�������@�_���ϐ�
		//=============================
		g_aAttackModel[nCntAttackModel].fVXaim3D = 0.0f;
		g_aAttackModel[nCntAttackModel].fVYaim3D = 0.0f;
		g_aAttackModel[nCntAttackModel].fVZaim3D = 0.0f;
		g_aAttackModel[nCntAttackModel].fVLaim3D = 0.0f;
	}

	//�ϐ�
	g_bIsLandingAttackModel = false;
	g_PlayerPosLength = NULL_VECTOR3;
	g_nDelayAttackModel = 0;//�f�B���C

	for (int nCntType = 0; nCntType < NUM_ATTACKMODEL; nCntType++)
	{
		//X�t�@�C���̓ǂݍ���
		D3DXLoadMeshFromX(ATTACKMODEL_FILENAME[nCntType], D3DXMESH_SYSTEMMEM, pDevice, NULL,
			&g_aAttackModelInfo[nCntType].pBuffMat, NULL, &g_aAttackModelInfo[nCntType].dwNumMat, &g_aAttackModelInfo[nCntType].pMesh);

		//�}�e���A���f�[�^�ւ̃|�C���^���擾
		pMat = (D3DXMATERIAL*)g_aAttackModelInfo[nCntType].pBuffMat->GetBufferPointer();

		//�}�e���A���f�[�^��ۑ�
		for (int nCntMat = 0; nCntMat < (int)g_aAttackModelInfo[nCntType].dwNumMat; nCntMat++)
		{
			for (int nCntAttackModel = 0; nCntAttackModel < MAX_ATTACKMODEL; nCntAttackModel++)
			{
				g_aAttackModel[nCntAttackModel].DiffUse[nCntType][nCntMat] = pMat[nCntMat].MatD3D.Diffuse;
			}
		}

		for (int nCntMat = 0; nCntMat < (int)g_aAttackModelInfo[nCntType].dwNumMat; nCntMat++)
		{
			//�e�N�X�`���t�@�C�������݂���
			if (pMat[nCntMat].pTextureFilename != NULL)
			{
				//�e�N�X�`���̓ǂݍ���
				D3DXCreateTextureFromFile(pDevice,
					pMat[nCntMat].pTextureFilename,
					&g_aAttackModelInfo[nCntType].pTexture[nCntMat]);
			}
		}
	}
}

//================================================================================================================================================================================================================================
// �I������
//================================================================================================================================================================================================================================
void UninitAttackModel(void)
{
	for (int nCntAttackModel = 0; nCntAttackModel < NUM_ATTACKMODEL; nCntAttackModel++)
	{
		//���b�V���̔j��
		if (g_aAttackModelInfo[nCntAttackModel].pMesh != NULL)
		{
			g_aAttackModelInfo[nCntAttackModel].pMesh->Release();
			g_aAttackModelInfo[nCntAttackModel].pMesh = NULL;
		}
		//�}�e���A���̔j��
		if (g_aAttackModelInfo[nCntAttackModel].pBuffMat != NULL)
		{
			g_aAttackModelInfo[nCntAttackModel].pBuffMat->Release();
			g_aAttackModelInfo[nCntAttackModel].pBuffMat = NULL;
		}

		//�e�N�X�`���̔j��
		for (int nCntMat = 0; nCntMat < ATTACKMODEL_MAT; nCntMat++)
		{
			if (g_aAttackModelInfo[nCntAttackModel].pTexture[nCntMat] != NULL)
			{
				g_aAttackModelInfo[nCntAttackModel].pTexture[nCntMat]->Release();
				g_aAttackModelInfo[nCntAttackModel].pTexture[nCntMat] = NULL;
			}
		}
	}
}

//================================================================================================================================================================================================================================
// �X�V����
//================================================================================================================================================================================================================================
void UpdateAttackModel(void)
{
	for (int nCntAttackModel = 0; nCntAttackModel < MAX_ATTACKMODEL; nCntAttackModel++)
	{
		if (g_aAttackModel[nCntAttackModel].bUse)
		{
			Player* pPlayer = GetPlayer();	//�v���C���[�̏����擾
			Enemy* pEnemy = GetEnemy();		//�G�̏����擾
			Boss* pBoss = GetBoss();		//�{�X�̏����擾
			Dummy* pDummy = GetDummy();		//�_�~�[�̏����擾

			g_aAttackModel[nCntAttackModel].nCntTime++;//�o�����Ă���̎��Ԃ��v������
			g_nDelayAttackModel++;//�f�B���C

			//===============================================
			//���f���̊g�嗦���Q�Ƃ������f���̍ő�ŏ�
			//===============================================
			g_aAttackModel[nCntAttackModel].vtxMin.x = g_aAttackModel[nCntAttackModel].Scale.y * g_aAttackModel[nCntAttackModel].OriginallyVtxMin.x;
			g_aAttackModel[nCntAttackModel].vtxMin.y = g_aAttackModel[nCntAttackModel].Scale.x * g_aAttackModel[nCntAttackModel].OriginallyVtxMin.y;
			g_aAttackModel[nCntAttackModel].vtxMin.z = g_aAttackModel[nCntAttackModel].Scale.z * g_aAttackModel[nCntAttackModel].OriginallyVtxMin.z;
			g_aAttackModel[nCntAttackModel].vtxMax.x = g_aAttackModel[nCntAttackModel].Scale.y * g_aAttackModel[nCntAttackModel].OriginallyVtxMax.x;
			g_aAttackModel[nCntAttackModel].vtxMax.y = g_aAttackModel[nCntAttackModel].Scale.x * g_aAttackModel[nCntAttackModel].OriginallyVtxMax.y;
			g_aAttackModel[nCntAttackModel].vtxMax.z = g_aAttackModel[nCntAttackModel].Scale.z * g_aAttackModel[nCntAttackModel].OriginallyVtxMax.z;
			g_aAttackModel[nCntAttackModel].Size = g_aAttackModel[nCntAttackModel].vtxMax - g_aAttackModel[nCntAttackModel].vtxMin;

			//===============================================
			//�v���C���[�ƍU�����f���̃x�N�g��
			//===============================================
			g_aAttackModel[nCntAttackModel].fVXaim = pPlayer->pos.x - g_aAttackModel[nCntAttackModel].pos.x;
			g_aAttackModel[nCntAttackModel].fVZaim = pPlayer->pos.z - g_aAttackModel[nCntAttackModel].pos.z;

			g_aAttackModel[nCntAttackModel].fVaim = atan2f(g_aAttackModel[nCntAttackModel].fVXaim, g_aAttackModel[nCntAttackModel].fVZaim);

			//===============================================
			//�v���C���[�ƍU�����f���̋���
			//===============================================
			float fLineLength = fRotCollisionAModel(pPlayer->pos.x, g_aAttackModel[nCntAttackModel].pos.x, g_aAttackModel[nCntAttackModel].pos.x +
				sinf(g_aAttackModel[nCntAttackModel].rot.y) * g_aAttackModel[nCntAttackModel].vtxMax.z,
				pPlayer->pos.z, g_aAttackModel[nCntAttackModel].pos.z, g_aAttackModel[nCntAttackModel].pos.z +
				cosf(g_aAttackModel[nCntAttackModel].rot.y) * g_aAttackModel[nCntAttackModel].vtxMax.z);

			//===============================================
			//�����S�̂Ŏg���\���̂��闐���l
			//===============================================
			float fPurposeRandRot = float(rand() % 200 + 1) / 100.0f;
			float fPurposeRandSpeed = float(rand() % 100 + 1) / 10.0f;
			float fPurposeRandSpeedY = float(rand() % 200 + 1) / 10.0f;

			float fPurposePosX = float(rand() % 400 - 200);
			float fPurposePosY = float(rand() % 400 - 200);
			float fPurposePosZ = float(rand() % 400 - 200);


			switch (g_aAttackModel[nCntAttackModel].nType)
			{
			case ATTACKMODEL00_BOOMERANG:

				if (g_aAttackModel[nCntAttackModel].nBehaviorType == ATTACKMODELBEHAVIOR00_NORMAL)
				{
					g_aAttackModel[nCntAttackModel].fSaveRot += 0.2f;//�v���C���[�������������̌����ɍX�ɉ��Z���Ă���

					g_aAttackModel[nCntAttackModel].rot.y += 0.15f;//�u�[����������]������

					g_aAttackModel[nCntAttackModel].fTotalRotMove += 0.2f;//�o�����Ă����]�ړ����������v��

					//=========================
					//���������߂�	
					//=========================
					if (g_aAttackModel[nCntAttackModel].fTotalRotMove <= D3DX_PI * 5)
					{
						g_aAttackModel[nCntAttackModel].pos =
							D3DXVECTOR3(sinf(g_aAttackModel[nCntAttackModel].fSaveRot + D3DX_PI) * (pPlayer->aBoomerangInfo.fBoomerangRenge) + g_aAttackModel[nCntAttackModel].SupportPos.x + pPlayer->pos.x,
								0.0f,
								cosf(g_aAttackModel[nCntAttackModel].fSaveRot + D3DX_PI) * (pPlayer->aBoomerangInfo.fBoomerangRenge) + g_aAttackModel[nCntAttackModel].SupportPos.z + pPlayer->pos.z);
					}
					else
					{
						float fVXaim = pPlayer->pos.x - g_aAttackModel[nCntAttackModel].pos.x;
						float fVZaim = pPlayer->pos.z - g_aAttackModel[nCntAttackModel].pos.z;
						float fVaim = atan2f(fVXaim, fVZaim);

						g_aAttackModel[nCntAttackModel].move = D3DXVECTOR3(sinf(fVaim) * 10.0f, 0.0f, cosf(fVaim) * 10.0f);
					}

					SetEffect(EFFECTTYPE_NORMAL, 300, 90.0f, 15.0f, 1.0f, D3DXVECTOR3(g_aAttackModel[nCntAttackModel].pos.x, g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMax.y / 2,
						g_aAttackModel[nCntAttackModel].pos.z), NULL_VECTOR3, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, 0);

					//�v���C���[�ɓ���������
					if (g_aAttackModel[nCntAttackModel].pos.x + g_aAttackModel[nCntAttackModel].vtxMax.x >= pPlayer->pos.x - pPlayer->Size.x / 2 &&
						g_aAttackModel[nCntAttackModel].pos.x + g_aAttackModel[nCntAttackModel].vtxMin.x <= pPlayer->pos.x + pPlayer->Size.x / 2 &&
						g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMax.y >= pPlayer->pos.y &&
						g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMin.y <= pPlayer->pos.y + pPlayer->Size.y &&
						g_aAttackModel[nCntAttackModel].pos.z + g_aAttackModel[nCntAttackModel].vtxMax.z >= pPlayer->pos.z - pPlayer->Size.z / 2 &&
						g_aAttackModel[nCntAttackModel].pos.z + g_aAttackModel[nCntAttackModel].vtxMin.z <= pPlayer->pos.z + pPlayer->Size.z / 2 &&
						g_aAttackModel[nCntAttackModel].fTotalRotMove >= (D3DX_PI * 0.8f))
					{
						g_aAttackModel[nCntAttackModel].bUse = false;

						if (pPlayer->aBoomerangInfo.nBoomerangPiece < pPlayer->aBoomerangInfo.nMaxBoomerangPiece)
						{//�v���C���[�̎����Ă���u�[�������̌����ő�l�������Ȃ�������
							pPlayer->aBoomerangInfo.nBoomerangPiece++;
						}
					}

					//�G�Ƃ̓����蔻��
					for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
					{
						if (pEnemy->bUse)
						{
							if (g_aAttackModel[nCntAttackModel].pos.x + g_aAttackModel[nCntAttackModel].vtxMax.x >= pEnemy->pos.x - pEnemy->Size.x / 2 &&
								g_aAttackModel[nCntAttackModel].pos.x + g_aAttackModel[nCntAttackModel].vtxMin.x <= pEnemy->pos.x + pEnemy->Size.x / 2 &&
								g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMax.y >= pEnemy->pos.y &&
								g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMin.y <= pEnemy->pos.y + pEnemy->Size.y &&
								g_aAttackModel[nCntAttackModel].pos.z + g_aAttackModel[nCntAttackModel].vtxMax.z >= pEnemy->pos.z - pEnemy->Size.z / 2 &&
								g_aAttackModel[nCntAttackModel].pos.z + g_aAttackModel[nCntAttackModel].vtxMin.z <= pEnemy->pos.z + pEnemy->Size.z / 2)
							{
								SetDamageEnemy(pPlayer->aBoomerangInfo.nBoomerangPower, nCntEnemy, PLAYERATTACK01_BOOMERANG, nCntAttackModel);
							}
						}
					}

					//�{�X�Ƃ̓����蔻��
					if (pBoss->bUse)
					{
						if (g_aAttackModel[nCntAttackModel].pos.x + g_aAttackModel[nCntAttackModel].vtxMax.x >= pBoss->pos.x - pBoss->Size.x / 2 &&
							g_aAttackModel[nCntAttackModel].pos.x + g_aAttackModel[nCntAttackModel].vtxMin.x <= pBoss->pos.x + pBoss->Size.x / 2 &&
							g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMax.y >= pBoss->pos.y &&
							g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMin.y <= pBoss->pos.y + pBoss->Size.y &&
							g_aAttackModel[nCntAttackModel].pos.z + g_aAttackModel[nCntAttackModel].vtxMax.z >= pBoss->pos.z - pBoss->Size.z / 2 &&
							g_aAttackModel[nCntAttackModel].pos.z + g_aAttackModel[nCntAttackModel].vtxMin.z <= pBoss->pos.z + pBoss->Size.z / 2)
						{
							SetDamageBoss(pPlayer->aBoomerangInfo.nBoomerangPower, PLAYERATTACK01_BOOMERANG, nCntAttackModel);
						}
					}
				}
				else
				{
					//===============================================
					//�U���J�n�ʒu�ƍU�����f���̋���
					//===============================================
					float fLength = sqrtf((g_aAttackModel[nCntAttackModel].pos.x - g_aAttackModel[nCntAttackModel].SupportPos.x) * (g_aAttackModel[nCntAttackModel].pos.x - g_aAttackModel[nCntAttackModel].SupportPos.x) +
						(g_aAttackModel[nCntAttackModel].pos.z - g_aAttackModel[nCntAttackModel].SupportPos.z) * (g_aAttackModel[nCntAttackModel].pos.z - g_aAttackModel[nCntAttackModel].SupportPos.z)) / 2;

					g_aAttackModel[nCntAttackModel].rot.y += 0.2f;

					if (g_aAttackModel[nCntAttackModel].bTurnFlag == false)
					{
						if (fLength > g_aAttackModel[nCntAttackModel].fBoomerangRange)
						{
							g_aAttackModel[nCntAttackModel].move.x += (0.0f - g_aAttackModel[nCntAttackModel].move.x) * 0.3f;
							g_aAttackModel[nCntAttackModel].move.z += (0.0f - g_aAttackModel[nCntAttackModel].move.z) * 0.3f;
						}


						if ((g_aAttackModel[nCntAttackModel].move.x >= -0.1f && g_aAttackModel[nCntAttackModel].move.x <= 0.1f) &&
							(g_aAttackModel[nCntAttackModel].move.z >= -0.1f && g_aAttackModel[nCntAttackModel].move.z <= 0.1f))
						{
							g_aAttackModel[nCntAttackModel].bTurnFlag = true;
						}
					}

					Field* pField = GetField();	//�t�B�[���h�̏����擾

					if (g_aAttackModel[nCntAttackModel].pos.x > pField->fWidth || g_aAttackModel[nCntAttackModel].pos.x < -pField->fWidth
						|| g_aAttackModel[nCntAttackModel].pos.z > pField->fHeight || g_aAttackModel[nCntAttackModel].pos.z < -pField->fHeight)
					{
						g_aAttackModel[nCntAttackModel].bTurnFlag = true;
					}

					if (g_aAttackModel[nCntAttackModel].bTurnFlag == true)
					{
						g_aAttackModel[nCntAttackModel].nTurnCnt++;

						if (g_nDelayAttackModel % 3 == 0 || g_aAttackModel[nCntAttackModel].nTurnCnt == 1)
						{
							//==========================
							//�R�������@�_������
							//==========================
							g_aAttackModel[nCntAttackModel].fVXaim3D = pPlayer->pos.x - g_aAttackModel[nCntAttackModel].pos.x;
							g_aAttackModel[nCntAttackModel].fVYaim3D = pPlayer->pos.y - g_aAttackModel[nCntAttackModel].pos.y;
							g_aAttackModel[nCntAttackModel].fVZaim3D = pPlayer->pos.z - g_aAttackModel[nCntAttackModel].pos.z;

							D3DXVECTOR3 BulletMove = NULL_VECTOR3;

							g_aAttackModel[nCntAttackModel].fVLaim3D = sqrtf((g_aAttackModel[nCntAttackModel].fVXaim3D * g_aAttackModel[nCntAttackModel].fVXaim3D) +
								(g_aAttackModel[nCntAttackModel].fVYaim3D * g_aAttackModel[nCntAttackModel].fVYaim3D)
								+ (g_aAttackModel[nCntAttackModel].fVZaim3D * g_aAttackModel[nCntAttackModel].fVZaim3D));

							BulletMove.x = g_aAttackModel[nCntAttackModel].fVXaim3D / g_aAttackModel[nCntAttackModel].fVLaim3D * 30.0f;
							BulletMove.y = g_aAttackModel[nCntAttackModel].fVYaim3D / g_aAttackModel[nCntAttackModel].fVLaim3D * 30.0f;
							BulletMove.z = g_aAttackModel[nCntAttackModel].fVZaim3D / g_aAttackModel[nCntAttackModel].fVLaim3D * 30.0f;

							g_aAttackModel[nCntAttackModel].move = BulletMove;
						}
					}

					//=========================
					//���������߂�	
					//=========================
					SetEffect(EFFECTTYPE_NORMAL, 300, g_aAttackModel[nCntAttackModel].Size.x * 1.5f, g_aAttackModel[nCntAttackModel].Size.y * 1.5f,
						1.0f, D3DXVECTOR3(g_aAttackModel[nCntAttackModel].pos.x, g_aAttackModel[nCntAttackModel].pos.y,
							g_aAttackModel[nCntAttackModel].pos.z), NULL_VECTOR3, D3DXCOLOR(1.0f, 0.3f, 0.0f, 1.0f), 0.0f, 0);

					//�v���C���[�ɓ���������
					if (g_aAttackModel[nCntAttackModel].pos.x + g_aAttackModel[nCntAttackModel].vtxMax.x >= pPlayer->pos.x - pPlayer->Size.x / 2 &&
						g_aAttackModel[nCntAttackModel].pos.x + g_aAttackModel[nCntAttackModel].vtxMin.x <= pPlayer->pos.x + pPlayer->Size.x / 2 &&
						g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMax.y >= pPlayer->pos.y &&
						g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMin.y <= pPlayer->pos.y + pPlayer->Size.y &&
						g_aAttackModel[nCntAttackModel].pos.z + g_aAttackModel[nCntAttackModel].vtxMax.z >= pPlayer->pos.z - pPlayer->Size.z / 2 &&
						g_aAttackModel[nCntAttackModel].pos.z + g_aAttackModel[nCntAttackModel].vtxMin.z <= pPlayer->pos.z + pPlayer->Size.z / 2 &&
						g_aAttackModel[nCntAttackModel].nCntTime >= 30)
					{
						g_aAttackModel[nCntAttackModel].bUse = false;

						if (pPlayer->aBoomerangInfo.nBoomerangPiece < pPlayer->aBoomerangInfo.nMaxBoomerangPiece)
						{//�v���C���[�̎����Ă���u�[�������̌����ő�l�������Ȃ�������
							pPlayer->aBoomerangInfo.nBoomerangPiece++;
						}
					}

					//�G�Ƃ̓����蔻��
					for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
					{
						if (pEnemy->bUse == true)
						{//�G���g�p����Ă���ꍇ
							if (g_aAttackModel[nCntAttackModel].pos.x + g_aAttackModel[nCntAttackModel].vtxMax.x >= pEnemy->pos.x - pEnemy->Size.x / 2 &&
								g_aAttackModel[nCntAttackModel].pos.x + g_aAttackModel[nCntAttackModel].vtxMin.x <= pEnemy->pos.x + pEnemy->Size.x / 2 &&
								g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMax.y >= pEnemy->pos.y &&
								g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMin.y <= pEnemy->pos.y + pEnemy->Size.y &&
								g_aAttackModel[nCntAttackModel].pos.z + g_aAttackModel[nCntAttackModel].vtxMax.z >= pEnemy->pos.z - pEnemy->Size.z / 2 &&
								g_aAttackModel[nCntAttackModel].pos.z + g_aAttackModel[nCntAttackModel].vtxMin.z <= pEnemy->pos.z + pEnemy->Size.z / 2)
							{
								SetDamageEnemy(pPlayer->aBoomerangInfo.nBoomerangPower, nCntEnemy, PLAYERATTACK01_BOOMERANG, nCntAttackModel);
							}
						}
					}

					//�{�X�Ƃ̓����蔻��
					if (pBoss->bUse)
					{//�{�X���g�p����Ă���ꍇ
						if (g_aAttackModel[nCntAttackModel].pos.x + g_aAttackModel[nCntAttackModel].vtxMax.x >= pBoss->pos.x - pBoss->Size.x / 2 &&
							g_aAttackModel[nCntAttackModel].pos.x + g_aAttackModel[nCntAttackModel].vtxMin.x <= pBoss->pos.x + pBoss->Size.x / 2 &&
							g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMax.y >= pBoss->pos.y &&
							g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMin.y <= pBoss->pos.y + pBoss->Size.y &&
							g_aAttackModel[nCntAttackModel].pos.z + g_aAttackModel[nCntAttackModel].vtxMax.z >= pBoss->pos.z - pBoss->Size.z / 2 &&
							g_aAttackModel[nCntAttackModel].pos.z + g_aAttackModel[nCntAttackModel].vtxMin.z <= pBoss->pos.z + pBoss->Size.z / 2)
						{
							SetDamageBoss(pPlayer->aBoomerangInfo.nBoomerangPower, PLAYERATTACK01_BOOMERANG, nCntAttackModel);
						}
					}

					if (pDummy->bUse)
					{//�_�~�[���g�p����Ă���ꍇ
						if (g_aAttackModel[nCntAttackModel].pos.x + g_aAttackModel[nCntAttackModel].vtxMax.x >= pDummy->pos.x - pDummy->Size.x / 2 &&
							g_aAttackModel[nCntAttackModel].pos.x + g_aAttackModel[nCntAttackModel].vtxMin.x <= pDummy->pos.x + pDummy->Size.x / 2 &&
							g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMax.y >= pDummy->pos.y &&
							g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMin.y <= pDummy->pos.y + pDummy->Size.y &&
							g_aAttackModel[nCntAttackModel].pos.z + g_aAttackModel[nCntAttackModel].vtxMax.z >= pDummy->pos.z - pDummy->Size.z / 2 &&
							g_aAttackModel[nCntAttackModel].pos.z + g_aAttackModel[nCntAttackModel].vtxMin.z <= pDummy->pos.z + pDummy->Size.z / 2)
						{
							DamageDummy(pPlayer->aBoomerangInfo.nBoomerangPower, PLAYERATTACK01_BOOMERANG);
						}
					}
				}

				break;

			case ATTACKMODEL01_WAVE:
				if (g_aAttackModel[nCntAttackModel].nBehaviorType == ATTACKMODELBEHAVIOR00_NORMAL)
				{
					g_aAttackModel[nCntAttackModel].Scale += D3DXVECTOR3(g_aAttackModel[nCntAttackModel].fExpansion,
						g_aAttackModel[nCntAttackModel].fExpansion, g_aAttackModel[nCntAttackModel].fExpansion);

					g_aAttackModel[nCntAttackModel].DiffUse[g_aAttackModel[nCntAttackModel].nType]->a = (float)(g_aAttackModel[nCntAttackModel].nLife) / (float)(g_aAttackModel[nCntAttackModel].nMaxLife);

					//====================
					//�����蔻��
					//====================
					for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
					{
						if (pEnemy->bUse)
						{//�G���g�p����Ă���ꍇ
							if (g_aAttackModel[nCntAttackModel].pos.x + g_aAttackModel[nCntAttackModel].vtxMax.x >= pEnemy->pos.x - pEnemy->Size.x / 2 &&
								g_aAttackModel[nCntAttackModel].pos.x + g_aAttackModel[nCntAttackModel].vtxMin.x <= pEnemy->pos.x + pEnemy->Size.x / 2 &&
								g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMax.y >= pEnemy->pos.y &&
								g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMin.y <= pEnemy->pos.y + pEnemy->Size.y &&
								g_aAttackModel[nCntAttackModel].pos.z + g_aAttackModel[nCntAttackModel].vtxMax.z >= pEnemy->pos.z - pEnemy->Size.z / 2 &&
								g_aAttackModel[nCntAttackModel].pos.z + g_aAttackModel[nCntAttackModel].vtxMin.z <= pEnemy->pos.z + pEnemy->Size.z / 2)
							{
								SetDamageEnemy((int)(pPlayer->aSpeakerInfo.fSpeakerPower), nCntEnemy, PLAYERATTACK02_SPEAKER, nCntAttackModel);
							}
						}
					}

					//�{�X�̓����蔻��
					if (pBoss->bUse)
					{//�{�X���g�p����Ă���ꍇ
						if (g_aAttackModel[nCntAttackModel].pos.x + g_aAttackModel[nCntAttackModel].vtxMax.x >= pBoss->pos.x - pBoss->Size.x / 2 &&
							g_aAttackModel[nCntAttackModel].pos.x + g_aAttackModel[nCntAttackModel].vtxMin.x <= pBoss->pos.x + pBoss->Size.x / 2 &&
							g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMax.y >= pBoss->pos.y &&
							g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMin.y <= pBoss->pos.y + pBoss->Size.y &&
							g_aAttackModel[nCntAttackModel].pos.z + g_aAttackModel[nCntAttackModel].vtxMax.z >= pBoss->pos.z - pBoss->Size.z / 2 &&
							g_aAttackModel[nCntAttackModel].pos.z + g_aAttackModel[nCntAttackModel].vtxMin.z <= pBoss->pos.z + pBoss->Size.z / 2)
						{
							SetDamageBoss(pPlayer->aBoomerangInfo.nBoomerangPower, PLAYERATTACK02_SPEAKER, nCntAttackModel);
						}
					}

					//�_�~�[�̓����蔻��
					if (pDummy->bUse)
					{//�_�~�[���g�p����Ă���ꍇ
						if (g_aAttackModel[nCntAttackModel].pos.x + g_aAttackModel[nCntAttackModel].vtxMax.x >= pDummy->pos.x - pDummy->Size.x / 2 &&
							g_aAttackModel[nCntAttackModel].pos.x + g_aAttackModel[nCntAttackModel].vtxMin.x <= pDummy->pos.x + pDummy->Size.x / 2 &&
							g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMax.y >= pDummy->pos.y &&
							g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMin.y <= pDummy->pos.y + pDummy->Size.y &&
							g_aAttackModel[nCntAttackModel].pos.z + g_aAttackModel[nCntAttackModel].vtxMax.z >= pDummy->pos.z - pDummy->Size.z / 2 &&
							g_aAttackModel[nCntAttackModel].pos.z + g_aAttackModel[nCntAttackModel].vtxMin.z <= pDummy->pos.z + pDummy->Size.z / 2)
						{
							DamageDummy(pPlayer->aBoomerangInfo.nBoomerangPower, PLAYERATTACK02_SPEAKER);
						}
					}
				}
				else
				{
					g_aAttackModel[nCntAttackModel].Scale += D3DXVECTOR3(g_aAttackModel[nCntAttackModel].fExpansion,
						g_aAttackModel[nCntAttackModel].fExpansion, g_aAttackModel[nCntAttackModel].fExpansion);

					//====================
					//�����蔻��
					//====================
					for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
					{
						if (pEnemy->bUse)
						{//�G���g�p����Ă���ꍇ
							if (g_aAttackModel[nCntAttackModel].pos.x + g_aAttackModel[nCntAttackModel].vtxMax.x >= pEnemy->pos.x - pEnemy->Size.x / 2 &&
								g_aAttackModel[nCntAttackModel].pos.x + g_aAttackModel[nCntAttackModel].vtxMin.x <= pEnemy->pos.x + pEnemy->Size.x / 2 &&
								g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMax.y >= pEnemy->pos.y &&
								g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMin.y <= pEnemy->pos.y + pEnemy->Size.y &&
								g_aAttackModel[nCntAttackModel].pos.z + g_aAttackModel[nCntAttackModel].vtxMax.z >= pEnemy->pos.z - pEnemy->Size.z / 2 &&
								g_aAttackModel[nCntAttackModel].pos.z + g_aAttackModel[nCntAttackModel].vtxMin.z <= pEnemy->pos.z + pEnemy->Size.z / 2)
							{
								SetDamageEnemy((int)(pPlayer->aSpeakerInfo.fSpeakerPower), nCntEnemy, PLAYERATTACK02_SPEAKER, nCntAttackModel);
							}
						}
					}

					//�{�X�̓����蔻��
					if (pBoss->bUse)
					{//�{�X���g�p����Ă���ꍇ
						if (g_aAttackModel[nCntAttackModel].pos.x + g_aAttackModel[nCntAttackModel].vtxMax.x >= pBoss->pos.x - pBoss->Size.x / 2 &&
							g_aAttackModel[nCntAttackModel].pos.x + g_aAttackModel[nCntAttackModel].vtxMin.x <= pBoss->pos.x + pBoss->Size.x / 2 &&
							g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMax.y >= pBoss->pos.y &&
							g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMin.y <= pBoss->pos.y + pBoss->Size.y &&
							g_aAttackModel[nCntAttackModel].pos.z + g_aAttackModel[nCntAttackModel].vtxMax.z >= pBoss->pos.z - pBoss->Size.z / 2 &&
							g_aAttackModel[nCntAttackModel].pos.z + g_aAttackModel[nCntAttackModel].vtxMin.z <= pBoss->pos.z + pBoss->Size.z / 2)
						{
							SetDamageBoss(pPlayer->aBoomerangInfo.nBoomerangPower, PLAYERATTACK02_SPEAKER, nCntAttackModel);
						}
					}

					//�_�~�[�̓����蔻��
					if (pDummy->bUse)
					{//�_�~�[���g�p����Ă���ꍇ
						if (g_aAttackModel[nCntAttackModel].pos.x + g_aAttackModel[nCntAttackModel].vtxMax.x >= pDummy->pos.x - pDummy->Size.x / 2 &&
							g_aAttackModel[nCntAttackModel].pos.x + g_aAttackModel[nCntAttackModel].vtxMin.x <= pDummy->pos.x + pDummy->Size.x / 2 &&
							g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMax.y >= pDummy->pos.y &&
							g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMin.y <= pDummy->pos.y + pDummy->Size.y &&
							g_aAttackModel[nCntAttackModel].pos.z + g_aAttackModel[nCntAttackModel].vtxMax.z >= pDummy->pos.z - pDummy->Size.z / 2 &&
							g_aAttackModel[nCntAttackModel].pos.z + g_aAttackModel[nCntAttackModel].vtxMin.z <= pDummy->pos.z + pDummy->Size.z / 2)
						{
							DamageDummy(pPlayer->aBoomerangInfo.nBoomerangPower, PLAYERATTACK02_SPEAKER);
						}
					}
				}

				break;

			case ATTACKMODEL02_SPHERE:
				if (g_aAttackModel[nCntAttackModel].nBehaviorType == ATTACKMODELBEHAVIOR00_NORMAL)
				{
					g_aAttackModel[nCntAttackModel].Scale += D3DXVECTOR3(g_aAttackModel[nCntAttackModel].fExpansion,
						g_aAttackModel[nCntAttackModel].fExpansion, g_aAttackModel[nCntAttackModel].fExpansion);

					g_aAttackModel[nCntAttackModel].DiffUse[g_aAttackModel[nCntAttackModel].nType]->a = (float)(g_aAttackModel[nCntAttackModel].nLife) / (float)(g_aAttackModel[nCntAttackModel].nMaxLife);

					//====================
					//�����蔻��
					//====================
					if (g_aAttackModel[nCntAttackModel].pos.x + g_aAttackModel[nCntAttackModel].vtxMax.x >= pPlayer->pos.x - pPlayer->Size.x / 2 &&
						g_aAttackModel[nCntAttackModel].pos.x + g_aAttackModel[nCntAttackModel].vtxMin.x <= pPlayer->pos.x + pPlayer->Size.x / 2 &&
						g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMax.y >= pPlayer->pos.y &&
						g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMin.y <= pPlayer->pos.y + pPlayer->Size.y &&
						g_aAttackModel[nCntAttackModel].pos.z + g_aAttackModel[nCntAttackModel].vtxMax.z >= pPlayer->pos.z - pPlayer->Size.z / 2 &&
						g_aAttackModel[nCntAttackModel].pos.z + g_aAttackModel[nCntAttackModel].vtxMin.z <= pPlayer->pos.z + pPlayer->Size.z / 2)
					{
						AttackHitPlayer(20);
					}

				}
				else
				{
					g_aAttackModel[nCntAttackModel].Scale += D3DXVECTOR3(g_aAttackModel[nCntAttackModel].fExpansion,
						g_aAttackModel[nCntAttackModel].fExpansion, g_aAttackModel[nCntAttackModel].fExpansion);


					g_aAttackModel[nCntAttackModel].DiffUse[g_aAttackModel[nCntAttackModel].nType]->a = (float)(g_aAttackModel[nCntAttackModel].nLife) / (float)(g_aAttackModel[nCntAttackModel].nMaxLife);

					if (g_aAttackModel[nCntAttackModel].nLife < (g_aAttackModel[nCntAttackModel].nMaxLife / 2.0f) && g_aAttackModel[nCntAttackModel].bSphereShot == false)
					{

						g_aAttackModel[nCntAttackModel].move.x = sinf(g_aAttackModel[nCntAttackModel].fVaim) * 10.0f;
						g_aAttackModel[nCntAttackModel].move.z = cosf(g_aAttackModel[nCntAttackModel].fVaim) * 10.0f;

						g_aAttackModel[nCntAttackModel].bSphereShot = true;//��x�������߂��̂�OK
					}
					if (g_aAttackModel[nCntAttackModel].nLife > (g_aAttackModel[nCntAttackModel].nMaxLife / 2.0f) && g_aAttackModel[nCntAttackModel].bSphereShot == false)
					{
						SetParticle(PARTICLETYPE_CHARGE, 200, 20.0f, 0.7f, D3DXVECTOR3(g_aAttackModel[nCntAttackModel].pos.x + fPurposePosX,
							g_aAttackModel[nCntAttackModel].pos.y + fPurposePosY,
							g_aAttackModel[nCntAttackModel].pos.z + fPurposePosZ),
							NULL_VECTOR3, D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), 0.0f, PARTICLETEX_NORMAL, nCntAttackModel);
					}

					if (g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMin.y < 0.0f)
					{
						g_aAttackModel[nCntAttackModel].pos.y = g_aAttackModel[nCntAttackModel].Size.y / 2.0f;
					}

					//====================
					//�����蔻��
					//====================
					if (g_aAttackModel[nCntAttackModel].pos.x + g_aAttackModel[nCntAttackModel].vtxMax.x >= pPlayer->pos.x - pPlayer->Size.x / 2 &&
						g_aAttackModel[nCntAttackModel].pos.x + g_aAttackModel[nCntAttackModel].vtxMin.x <= pPlayer->pos.x + pPlayer->Size.x / 2 &&
						g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMax.y >= pPlayer->pos.y &&
						g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMin.y <= pPlayer->pos.y + pPlayer->Size.y &&
						g_aAttackModel[nCntAttackModel].pos.z + g_aAttackModel[nCntAttackModel].vtxMax.z >= pPlayer->pos.z - pPlayer->Size.z / 2 &&
						g_aAttackModel[nCntAttackModel].pos.z + g_aAttackModel[nCntAttackModel].vtxMin.z <= pPlayer->pos.z + pPlayer->Size.z / 2)
					{
						AttackHitPlayer(20);
					}
				}

				break;

			case ATTACKMODEL03_BOMB:
				if (g_aAttackModel[nCntAttackModel].nBehaviorType == ATTACKMODELBEHAVIOR00_NORMAL)
				{
					g_aAttackModel[nCntAttackModel].bIsLanding = bCollisionModel(&g_aAttackModel[nCntAttackModel].pos,
						&g_aAttackModel[nCntAttackModel].PosOld,
						g_aAttackModel[nCntAttackModel].Size, g_aAttackModel[nCntAttackModel].nNumLandingModel);

					if (!g_aAttackModel[nCntAttackModel].bIsLanding)
					{
						g_aAttackModel[nCntAttackModel].move.y -= 0.2f;
					}
					else
					{
						g_aAttackModel[nCntAttackModel].move.y = 0.0f;
					}

					if (g_aAttackModel[nCntAttackModel].pos.y <= 0.0f)
					{
						g_aAttackModel[nCntAttackModel].pos.y = 0.0f;
						g_aAttackModel[nCntAttackModel].move.y = 0.0f;
					}
				}
				else
				{
					g_aAttackModel[nCntAttackModel].bIsLanding = bCollisionModel(&g_aAttackModel[nCntAttackModel].pos,
						&g_aAttackModel[nCntAttackModel].PosOld,
						g_aAttackModel[nCntAttackModel].Size, g_aAttackModel[nCntAttackModel].nNumLandingModel);

					if (!g_aAttackModel[nCntAttackModel].bIsLanding)
					{
						g_aAttackModel[nCntAttackModel].move.y -= 0.2f;
					}
					else
					{
						g_aAttackModel[nCntAttackModel].move.y = 0.0f;
					}

					if (g_aAttackModel[nCntAttackModel].pos.y <= 0.0f)
					{
						g_aAttackModel[nCntAttackModel].pos.y = 0.0f;
						g_aAttackModel[nCntAttackModel].move = NULL_VECTOR3;
					}

					if (g_aAttackModel[nCntAttackModel].bIsLanding)
					{
						g_aAttackModel[nCntAttackModel].move = NULL_VECTOR3;
					}
				}

				break;

			case ATTACKMODEL04_SHOCKWAVE:

				if (g_aAttackModel[nCntAttackModel].nBehaviorType == ATTACKMODELBEHAVIOR00_NORMAL)
				{
					g_aAttackModel[nCntAttackModel].rot.y += 0.25f;


					SetParticle(PARTICLETYPE_GRAVITY, 80, 5.0f, 0.05f, g_aAttackModel[nCntAttackModel].pos,
						D3DXVECTOR3(sinf(D3DX_PI * fPurposeRandRot) * fPurposeRandSpeed, fPurposeRandSpeedY, cosf(D3DX_PI * fPurposeRandRot) * fPurposeRandSpeed),
						D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), 0.0f, PARTICLETEX_NORMAL, 0);

					//====================
					//�����蔻��
					//====================
					if (g_aAttackModel[nCntAttackModel].pos.x + g_aAttackModel[nCntAttackModel].vtxMax.x >= pPlayer->pos.x - pPlayer->Size.x / 2 &&
						g_aAttackModel[nCntAttackModel].pos.x + g_aAttackModel[nCntAttackModel].vtxMin.x <= pPlayer->pos.x + pPlayer->Size.x / 2 &&
						g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMax.y >= pPlayer->pos.y &&
						g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMin.y <= pPlayer->pos.y + pPlayer->Size.y &&
						g_aAttackModel[nCntAttackModel].pos.z + g_aAttackModel[nCntAttackModel].vtxMax.z >= pPlayer->pos.z - pPlayer->Size.z / 2 &&
						g_aAttackModel[nCntAttackModel].pos.z + g_aAttackModel[nCntAttackModel].vtxMin.z <= pPlayer->pos.z + pPlayer->Size.z / 2)
					{
						AttackHitPlayer(30);

						for (int nCntParticle = 0; nCntParticle < 10; nCntParticle++)
						{
							float fRandRot = float(rand() % 200 + 1) / 10;
							float fRandSpeed = float(rand() % 20 + 1);
							float fRandMove = float(rand() % 40 - 20) / 2;

							SetParticle(PARTICLETYPE_NORMAL, 80, 5.0f, 0.4f, pPlayer->pos,
								D3DXVECTOR3(sinf(-D3DX_PI * fRandRot) * fRandSpeed, fRandMove, cosf(-D3DX_PI * fRandRot) * fRandSpeed),
								D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 0.0f, PARTICLETEX_NORMAL, 0);
						}
					}
				}
				else
				{
					g_aAttackModel[nCntAttackModel].rot.y -= 0.25f;


					g_aAttackModel[nCntAttackModel].nCntTime++;//�o�����Ă���̎��Ԃ𑪂�	

					//========================================================
					//�o�����Ă���̎��Ԃ��Ƃ̏���
					//========================================================
					if (g_aAttackModel[nCntAttackModel].nCntTime == 2)
					{
						g_aAttackModel[nCntAttackModel].fSpeed = g_aAttackModel[nCntAttackModel].fExpansion * 1.1f;
					}

					if (g_aAttackModel[nCntAttackModel].nCntTime == 70)
					{
						g_aAttackModel[nCntAttackModel].rot.y = g_aAttackModel[nCntAttackModel].fVaim + D3DX_PI;
						g_aAttackModel[nCntAttackModel].move.x = sinf(g_aAttackModel[nCntAttackModel].fVaim) * g_aAttackModel[nCntAttackModel].fSpeed;
						g_aAttackModel[nCntAttackModel].move.z = cosf(g_aAttackModel[nCntAttackModel].fVaim) * g_aAttackModel[nCntAttackModel].fSpeed;

					}

					SetParticle(PARTICLETYPE_GRAVITY, 120, 5.0f, 0.05f, g_aAttackModel[nCntAttackModel].pos,
						D3DXVECTOR3(sinf(D3DX_PI * fPurposeRandRot) * fPurposeRandSpeed, fPurposeRandSpeedY, cosf(D3DX_PI * fPurposeRandRot) * fPurposeRandSpeed),
						D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), 0.0f, PARTICLETEX_NORMAL, 0);

					//====================
					//�����蔻��
					//====================
					if (g_aAttackModel[nCntAttackModel].pos.x + g_aAttackModel[nCntAttackModel].vtxMax.x >= pPlayer->pos.x - pPlayer->Size.x / 2 &&
						g_aAttackModel[nCntAttackModel].pos.x + g_aAttackModel[nCntAttackModel].vtxMin.x <= pPlayer->pos.x + pPlayer->Size.x / 2 &&
						g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMax.y >= pPlayer->pos.y &&
						g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMin.y <= pPlayer->pos.y + pPlayer->Size.y &&
						g_aAttackModel[nCntAttackModel].pos.z + g_aAttackModel[nCntAttackModel].vtxMax.z >= pPlayer->pos.z - pPlayer->Size.z / 2 &&
						g_aAttackModel[nCntAttackModel].pos.z + g_aAttackModel[nCntAttackModel].vtxMin.z <= pPlayer->pos.z + pPlayer->Size.z / 2)
					{
						AttackHitPlayer(30);
					}
				}

				break;

			case ATTACKMODEL05_TORNADO:

				SetEffect(EFFECTTYPE_NORMAL, 120, 10.0f, 10.0f, 0.1f, g_aAttackModel[nCntAttackModel].pos, NULL_VECTOR3, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), 0.0f, 0);

				if (g_nDelayAttackModel % 10 == 0)
				{
					SetEmitterEffect(EMITTERTYPE_TORNADO, EMITTERTARGET_ATTACKMODEL, 180, 3, 10.0f, 0.02f, g_aAttackModel[nCntAttackModel].pos, D3DXVECTOR3(0.0f, 2.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), 0.0f, nCntAttackModel, 0.0f, NULL_VECTOR3);
				}

				if (g_aAttackModel[nCntAttackModel].nBehaviorType == ATTACKMODELBEHAVIOR00_NORMAL)
				{
					g_aAttackModel[nCntAttackModel].rot.y += 0.17f;

					g_aAttackModel[nCntAttackModel].fExpansion *= 1.05f;
					g_aAttackModel[nCntAttackModel].fRot += 0.005f + g_aAttackModel[nCntAttackModel].fSaveRot;

					g_aAttackModel[nCntAttackModel].pos = D3DXVECTOR3(sinf(D3DX_PI * g_aAttackModel[nCntAttackModel].fRot) * g_aAttackModel[nCntAttackModel].fExpansion + g_aAttackModel[nCntAttackModel].SupportPos.x,
						0.0f, cosf(D3DX_PI * g_aAttackModel[nCntAttackModel].fRot) * g_aAttackModel[nCntAttackModel].fExpansion + g_aAttackModel[nCntAttackModel].SupportPos.z);

					//====================
					//�����蔻��
					//====================
					if (g_aAttackModel[nCntAttackModel].pos.x + g_aAttackModel[nCntAttackModel].vtxMax.x >= pPlayer->pos.x - pPlayer->Size.x / 2 &&
						g_aAttackModel[nCntAttackModel].pos.x + g_aAttackModel[nCntAttackModel].vtxMin.x <= pPlayer->pos.x + pPlayer->Size.x / 2 &&
						g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMax.y >= pPlayer->pos.y &&
						g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMin.y <= pPlayer->pos.y + pPlayer->Size.y &&
						g_aAttackModel[nCntAttackModel].pos.z + g_aAttackModel[nCntAttackModel].vtxMax.z >= pPlayer->pos.z - pPlayer->Size.z / 2 &&
						g_aAttackModel[nCntAttackModel].pos.z + g_aAttackModel[nCntAttackModel].vtxMin.z <= pPlayer->pos.z + pPlayer->Size.z / 2)
					{
						AttackHitPlayer(30);
					}
				}
				else
				{
					g_aAttackModel[nCntAttackModel].move.x *= 1.025f;
					g_aAttackModel[nCntAttackModel].move.z *= 1.025f;

					g_aAttackModel[nCntAttackModel].rot.y += 0.1f;

					//====================
					//�����蔻��
					//====================
					if (g_aAttackModel[nCntAttackModel].pos.x + g_aAttackModel[nCntAttackModel].vtxMax.x >= pPlayer->pos.x - pPlayer->Size.x / 2 &&
						g_aAttackModel[nCntAttackModel].pos.x + g_aAttackModel[nCntAttackModel].vtxMin.x <= pPlayer->pos.x + pPlayer->Size.x / 2 &&
						g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMax.y >= pPlayer->pos.y &&
						g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMin.y <= pPlayer->pos.y + pPlayer->Size.y &&
						g_aAttackModel[nCntAttackModel].pos.z + g_aAttackModel[nCntAttackModel].vtxMax.z >= pPlayer->pos.z - pPlayer->Size.z / 2 &&
						g_aAttackModel[nCntAttackModel].pos.z + g_aAttackModel[nCntAttackModel].vtxMin.z <= pPlayer->pos.z + pPlayer->Size.z / 2)
					{
						AttackHitPlayer(30);
					}
				}

				break;

			case ATTACKMODEL06_THUNDER:

				g_aAttackModel[nCntAttackModel].rot.y += 0.22f;

				//====================
				//�����蔻��
				//====================
				if (g_aAttackModel[nCntAttackModel].pos.x + g_aAttackModel[nCntAttackModel].vtxMax.x >= pPlayer->pos.x - pPlayer->Size.x / 2 &&
					g_aAttackModel[nCntAttackModel].pos.x + g_aAttackModel[nCntAttackModel].vtxMin.x <= pPlayer->pos.x + pPlayer->Size.x / 2 &&
					g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMax.y >= pPlayer->pos.y &&
					g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMin.y <= pPlayer->pos.y + pPlayer->Size.y &&
					g_aAttackModel[nCntAttackModel].pos.z + g_aAttackModel[nCntAttackModel].vtxMax.z >= pPlayer->pos.z - pPlayer->Size.z / 2 &&
					g_aAttackModel[nCntAttackModel].pos.z + g_aAttackModel[nCntAttackModel].vtxMin.z <= pPlayer->pos.z + pPlayer->Size.z / 2)
				{
					AttackHitPlayer(40);
				}

				break;

			case ATTACKMODEL07_METEO:

				if (g_aAttackModel[nCntAttackModel].nBehaviorType == ATTACKMODELBEHAVIOR00_NORMAL)
				{
					if (g_aAttackModel[nCntAttackModel].nCntTime == 1)
					{
						SetEmitterEffect(EMITTERTYPE_ROTATION, EMITTERTARGET_ATTACKMODEL, 1000, 50, 15, 0.1f, NULL_VECTOR3, NULL_VECTOR3, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 0.0f, nCntAttackModel, g_aAttackModel[nCntAttackModel].Size.x / 2.0f, g_aAttackModel[nCntAttackModel].SupportPos);
						SetEmitterEffect(EMITTERTYPE_ROTATION, EMITTERTARGET_ATTACKMODEL, 1000, 50, 15, 0.1f, NULL_VECTOR3, NULL_VECTOR3, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), D3DX_PI, nCntAttackModel, g_aAttackModel[nCntAttackModel].Size.x / 2.0f, g_aAttackModel[nCntAttackModel].SupportPos);
					}

					//====================
					//�����蔻��
					//====================

					if (g_aAttackModel[nCntAttackModel].pos.x + g_aAttackModel[nCntAttackModel].vtxMax.x >= pPlayer->pos.x - pPlayer->Size.x / 2 &&
						g_aAttackModel[nCntAttackModel].pos.x + g_aAttackModel[nCntAttackModel].vtxMin.x <= pPlayer->pos.x + pPlayer->Size.x / 2 &&
						g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMax.y >= pPlayer->pos.y &&
						g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMin.y <= pPlayer->pos.y + pPlayer->Size.y &&
						g_aAttackModel[nCntAttackModel].pos.z + g_aAttackModel[nCntAttackModel].vtxMax.z >= pPlayer->pos.z - pPlayer->Size.z / 2 &&
						g_aAttackModel[nCntAttackModel].pos.z + g_aAttackModel[nCntAttackModel].vtxMin.z <= pPlayer->pos.z + pPlayer->Size.z / 2)
					{
						AttackHitPlayer(100);
					}

					if (g_aAttackModel[nCntAttackModel].pos.y <= 0.0f)
					{
						g_aAttackModel[nCntAttackModel].bUse = false;
						PlaySound(SOUND_LABEL_SE_BURSTMETEO_000);
					}
				}

				break;

			case ATTACKMODEL08_SOUP:

				//====================
				//�����蔻��
				//====================
				g_aAttackModel[nCntAttackModel].move.y += -0.3f;

				for (int nCntParticle = 0; nCntParticle < 3; nCntParticle++)
				{
					float fRandRot = float(rand() % 200 + 1) / 10;
					float fRandSpeed = float(rand() % 40 + 1);
					float fRandMove = float(rand() % 400) / 10;

					SetParticle(PARTICLETYPE_NORMAL, 10, 10.0f, 1.0f, g_aAttackModel[nCntAttackModel].pos,
						D3DXVECTOR3(sinf(D3DX_PI * fRandRot) * fRandSpeed, fRandMove, cosf(D3DX_PI * fRandRot) * fRandSpeed),
						D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f), 0.0f, PARTICLETEX_NORMAL, 0);
				}

				if (g_aAttackModel[nCntAttackModel].pos.y <= 0.0f)
				{
					g_aAttackModel[nCntAttackModel].pos.y = 0.0f;
					g_aAttackModel[nCntAttackModel].move.y = 0.0f;
				}

				if (g_aAttackModel[nCntAttackModel].pos.x + g_aAttackModel[nCntAttackModel].vtxMax.x >= pPlayer->pos.x - pPlayer->Size.x / 2 &&
					g_aAttackModel[nCntAttackModel].pos.x + g_aAttackModel[nCntAttackModel].vtxMin.x <= pPlayer->pos.x + pPlayer->Size.x / 2 &&
					g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMax.y >= pPlayer->pos.y &&
					g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMin.y <= pPlayer->pos.y + pPlayer->Size.y &&
					g_aAttackModel[nCntAttackModel].pos.z + g_aAttackModel[nCntAttackModel].vtxMax.z >= pPlayer->pos.z - pPlayer->Size.z / 2 &&
					g_aAttackModel[nCntAttackModel].pos.z + g_aAttackModel[nCntAttackModel].vtxMin.z <= pPlayer->pos.z + pPlayer->Size.z / 2)
				{
					AttackHitPlayer(10);
					g_aAttackModel[nCntAttackModel].bUse = false;
				}

				break;

			case ATTACKMODEL09_LASER:
				g_aAttackModel[nCntAttackModel].rot.z += 0.17f;


				if (g_aAttackModel[nCntAttackModel].nCntTime == 1)
				{
					g_aAttackModel[nCntAttackModel].fRot = 0.01f;

					if (g_aAttackModel[nCntAttackModel].rot.y >= g_aAttackModel[nCntAttackModel].fVaim)
					{
						g_aAttackModel[nCntAttackModel].bCurveLaser = false;
					}
					else
					{
						g_aAttackModel[nCntAttackModel].bCurveLaser = true;
					}
				}

				if (g_aAttackModel[nCntAttackModel].fRot <= 0.061f)
				{
					g_aAttackModel[nCntAttackModel].fRot *= 1.003f;
				}

				if (g_aAttackModel[nCntAttackModel].bCurveLaser == true)
				{
					g_aAttackModel[nCntAttackModel].rot.y += g_aAttackModel[nCntAttackModel].fRot;
				}
				else
				{
					g_aAttackModel[nCntAttackModel].rot.y -= g_aAttackModel[nCntAttackModel].fRot;
				}

				g_aAttackModel[nCntAttackModel].SupportPos = D3DXVECTOR3(sinf(g_aAttackModel[nCntAttackModel].rot.y) * (g_aAttackModel[nCntAttackModel].vtxMax.z * 2.0f), g_aAttackModel[nCntAttackModel].pos.y,
					cosf(g_aAttackModel[nCntAttackModel].rot.y) * (g_aAttackModel[nCntAttackModel].vtxMax.z * 2.0f));

				//==========================
			    //�����_���ȕ����ɃG�~�b�^�[���΂�
			    //==========================
					float fRandPosX = float(rand() % 500 - 250);
					float fRandPosY = float(rand() % 500 - 250);
					float fRandPosZ = float(rand() % 500 - 250);
					float fColR = float(rand() % 100 + 1) / 100.0f;
					float fColG = float(rand() % 100 + 1) / 100.0f;
					float fColB = float(rand() % 100 + 1) / 100.0f;
					g_aAttackModel[nCntAttackModel].fVXaim3D = (g_aAttackModel[nCntAttackModel].pos.x + fRandPosX) - g_aAttackModel[nCntAttackModel].pos.x;
					g_aAttackModel[nCntAttackModel].fVYaim3D = (g_aAttackModel[nCntAttackModel].pos.y + fRandPosY) - g_aAttackModel[nCntAttackModel].pos.y;
					g_aAttackModel[nCntAttackModel].fVZaim3D = (g_aAttackModel[nCntAttackModel].pos.z + fRandPosZ) - g_aAttackModel[nCntAttackModel].pos.z;

					D3DXVECTOR3 BulletMove = NULL_VECTOR3;

					g_aAttackModel[nCntAttackModel].fVLaim3D = sqrtf((g_aAttackModel[nCntAttackModel].fVXaim3D * g_aAttackModel[nCntAttackModel].fVXaim3D) +
						(g_aAttackModel[nCntAttackModel].fVYaim3D * g_aAttackModel[nCntAttackModel].fVYaim3D)
						+ (g_aAttackModel[nCntAttackModel].fVZaim3D * g_aAttackModel[nCntAttackModel].fVZaim3D));

					BulletMove.x = g_aAttackModel[nCntAttackModel].fVXaim3D / g_aAttackModel[nCntAttackModel].fVLaim3D * 20.0f;
					BulletMove.y = g_aAttackModel[nCntAttackModel].fVYaim3D / g_aAttackModel[nCntAttackModel].fVLaim3D * 20.0f;
					BulletMove.z = g_aAttackModel[nCntAttackModel].fVZaim3D / g_aAttackModel[nCntAttackModel].fVLaim3D * 20.0f;

					if (g_nDelayAttackModel % 2 == 0)
					{
						SetEmitterEffect(EMITTERTYPE_ACCELEHORMING, 0, 300, 30, 30.0f, 0.8f, g_aAttackModel[nCntAttackModel].pos, BulletMove, D3DXCOLOR(fColR, fColG, fColB, 1.0f), 0.0f, nCntAttackModel, 0.0f,
							NULL_VECTOR3);
					}

				//==============================================================================================================================================
			
				if (fLineLength <= 5000.0f &&
					g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMax.y >= pPlayer->pos.y &&
					g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMin.y <= pPlayer->pos.y + pPlayer->Size.y)
				{
					AttackHitPlayer(100);
				}

				if (pBoss->bUse == false)
				{
					g_aAttackModel[nCntAttackModel].bUse = false;
				}


				//==================================================================================================================================

				break;
			}

			//=====================
			//�ړ�
			//=====================
			g_aAttackModel[nCntAttackModel].PosOld = g_aAttackModel[nCntAttackModel].pos;
			g_aAttackModel[nCntAttackModel].pos += g_aAttackModel[nCntAttackModel].move;

			//=====================
			//�̗͂����炷
			//=====================
			g_aAttackModel[nCntAttackModel].nLife -= 1;

			if (g_aAttackModel[nCntAttackModel].nLife <= 0)
			{
				g_aAttackModel[nCntAttackModel].bUse = false;
			}

			if (!g_aAttackModel[nCntAttackModel].bUse)
			{
				if (g_aAttackModel[nCntAttackModel].nType == ATTACKMODEL03_BOMB)
				{
					if (g_aAttackModel[nCntAttackModel].nBehaviorType == ATTACKMODELBEHAVIOR00_NORMAL)
					{
						SetAttackModel(ATTACKMODEL02_SPHERE, 60, g_aAttackModel[nCntAttackModel].pos, NULL_VECTOR3, NULL_VECTOR3, NULL_VECTOR3,
							0.0f, 0.0f, 0.1f, NULL_VECTOR3, ATTACKMODELBEHAVIOR00_NORMAL);
					}
					else
					{
						SetAttackModel(ATTACKMODEL02_SPHERE, 60, g_aAttackModel[nCntAttackModel].pos, NULL_VECTOR3, NULL_VECTOR3, NULL_VECTOR3,
							0.0f, 0.0f, 0.2f, NULL_VECTOR3, ATTACKMODELBEHAVIOR00_NORMAL);
					}
				}
				else if (g_aAttackModel[nCntAttackModel].nType == ATTACKMODEL07_METEO)
				{
					for (int nCntParticle = 0; nCntParticle < 100; nCntParticle++)
					{
						float fRandRot = float(rand() % 200 + 1) / 10;
						float fRandSpeed = float(rand() % 40 + 1);
						float fRandMove = float(rand() % 400) / 10;
						float fRandumColorR = float(rand() % 100 + 1) / 100;
						float fRandumColorG = float(rand() % 100 + 1) / 100;
						float fRandumColorB = float(rand() % 100 + 1) / 100;

						SetParticle(PARTICLETYPE_GRAVITY, 25, 10.0f, 0.2f, g_aAttackModel[nCntAttackModel].pos,
							D3DXVECTOR3(sinf(D3DX_PI * fRandRot) * fRandSpeed, fRandMove, cosf(D3DX_PI * fRandRot) * fRandSpeed),
							D3DXCOLOR(fRandumColorR, fRandumColorG, fRandumColorB, 1.0f), 0.0f, PARTICLETEX_NORMAL, 0);
					}
				}
				else if (g_aAttackModel[nCntAttackModel].nType == ATTACKMODEL09_LASER)
				{
					StopSound(SOUND_LABEL_SE_RASER_000); 
				}
			}


			SetPositionShadow(g_aAttackModel[nCntAttackModel].nIdxShadow, D3DXVECTOR3(g_aAttackModel[nCntAttackModel].pos.x, g_aAttackModel[nCntAttackModel].pos.y, g_aAttackModel[nCntAttackModel].pos.z));
			if (g_aAttackModel[nCntAttackModel].bUse == false)
			{
				KillShadow(g_aAttackModel[nCntAttackModel].nIdxShadow);
			}

		}
	}

}

//================================================================================================================================================================================================================================
// �`�揈��
//================================================================================================================================================================================================================================
void DrawAttackModel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans, mtxScale;	//�v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;					//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL* pMat;						//�}�e���A���̃f�[�^�ւ̃|�C���^

	//���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	for (int nCntAttackModel = 0; nCntAttackModel < MAX_ATTACKMODEL; nCntAttackModel++)
	{
		if (g_aAttackModel[nCntAttackModel].bUse)
		{//�g�p����Ă���ꍇ
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aAttackModel[nCntAttackModel].mtxWorld);

			//�傫���𔽉f
			D3DXMatrixScaling(&mtxScale, g_aAttackModel[nCntAttackModel].Scale.y, g_aAttackModel[nCntAttackModel].Scale.x, g_aAttackModel[nCntAttackModel].Scale.z);
			D3DXMatrixMultiply(&g_aAttackModel[nCntAttackModel].mtxWorld, &g_aAttackModel[nCntAttackModel].mtxWorld, &mtxScale);

			//�����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aAttackModel[nCntAttackModel].rot.y, g_aAttackModel[nCntAttackModel].rot.x, g_aAttackModel[nCntAttackModel].rot.z);
			D3DXMatrixMultiply(&g_aAttackModel[nCntAttackModel].mtxWorld, &g_aAttackModel[nCntAttackModel].mtxWorld, &mtxRot);

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aAttackModel[nCntAttackModel].pos.x, g_aAttackModel[nCntAttackModel].pos.y, g_aAttackModel[nCntAttackModel].pos.z);
			D3DXMatrixMultiply(&g_aAttackModel[nCntAttackModel].mtxWorld, &g_aAttackModel[nCntAttackModel].mtxWorld, &mtxTrans);

			//===============================================================
			//�`��̒���
			//===============================================================
			
			//�@���̒������P�ɂ���B�i�X�P�[���Ȃǂ��g�������́A�K���g���B)
			pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aAttackModel[nCntAttackModel].mtxWorld);

			//�}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_aAttackModelInfo[g_aAttackModel[nCntAttackModel].nType].pBuffMat->GetBufferPointer();

			//============================
			//�`��̒���
			//============================
			//�A���t�@�e�X�g��L��
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
			pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

			//==================================================================

			for (int nCntAttackModel2 = 0; nCntAttackModel2 < (int)g_aAttackModelInfo[g_aAttackModel[nCntAttackModel].nType].dwNumMat; nCntAttackModel2++)
			{
				pMat[nCntAttackModel2].MatD3D.Diffuse.a = g_aAttackModel[nCntAttackModel].DiffUse[g_aAttackModel[nCntAttackModel].nType]->a;
				//�}�e���A���̐ݒ�
				pDevice->SetMaterial(&pMat[nCntAttackModel2].MatD3D);

				//�e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_aAttackModelInfo[g_aAttackModel[nCntAttackModel].nType].pTexture[nCntAttackModel2]);

				//���f��(�p�[�c)�̕`��
				g_aAttackModelInfo[g_aAttackModel[nCntAttackModel].nType].pMesh->DrawSubset(nCntAttackModel2);
			}

			//============================
			//�`��̒��������Ƃɖ߂�
			//============================
			//�A���t�@�e�X�g�𖳌��ɖ߂�
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

			//=====================================================================

			//����cpp�ŃX�P�[�����g���Ƃ͌���Ȃ��i�@���̏����l��1.0f�Ȃ̂Łj����A�@���̃m�[�}���C�Y�i1.0f)�ɂ���̂��I�t�ɂ���B
			pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
		}
	}

	//�ۑ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}

//================================================================================================================================================================================================================================
// �ݒ菈��
//================================================================================================================================================================================================================================
void SetAttackModel(int nType, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot, D3DXVECTOR3 Scale,
	float fRot, float fSaveRot, float fExpansion, D3DXVECTOR3 SupportPos, int nBehaviorType)
{
	BYTE* pVtxBuff;	//���_�o�b�t�@�ւ̃|�C���^

	Player* pPlayer = GetPlayer();

	for (int nCntAttackModel = 0; nCntAttackModel < MAX_ATTACKMODEL; nCntAttackModel++)
	{
		if (!g_aAttackModel[nCntAttackModel].bUse)
		{//�g�p����Ă��Ȃ��ꍇ
			g_aAttackModel[nCntAttackModel].pos = pos;		                //�ʒu
			g_aAttackModel[nCntAttackModel].move = move;                    //�ړ���
			g_aAttackModel[nCntAttackModel].rot = rot;		                //����
			g_aAttackModel[nCntAttackModel].Vrot = rot - D3DXVECTOR3(D3DX_PI,D3DX_PI,D3DX_PI);//�x�N�g�����l����������
			g_aAttackModel[nCntAttackModel].Scale = Scale;                  //�傫��
			g_aAttackModel[nCntAttackModel].nType = nType;	                //���
			g_aAttackModel[nCntAttackModel].nBehaviorType = nBehaviorType;	//�����̎��
			g_aAttackModel[nCntAttackModel].bUse = true;	                //�g�p����
			g_aAttackModel[nCntAttackModel].fRot = fRot;                    //�������������߂�
			g_aAttackModel[nCntAttackModel].nLife = nLife;                  //�̗�
			g_aAttackModel[nCntAttackModel].fSaveRot = fSaveRot;            //�v���C���[���u�[�������𓊂����������L�^����
			g_aAttackModel[nCntAttackModel].fTotalRotMove = 0.0f;           //�v���C���[���u�[�������𓊂��Ă����]�ړ����������v��
			g_aAttackModel[nCntAttackModel].bTurnFlag = false;              //�u�[���������^�[��������t���O
			g_aAttackModel[nCntAttackModel].nCntTime = 0;                   //�o�����Ă���̎��Ԃ��J�E���g���� 
			g_aAttackModel[nCntAttackModel].fExpansion = fExpansion;        //�g���
			g_aAttackModel[nCntAttackModel].nMaxLife = nLife;               //�ő�̗�
			g_aAttackModel[nCntAttackModel].PosLength = NULL_VECTOR3;       //���镨�̂Ƃ̋��������߂鎞�Ɏg��
			g_aAttackModel[nCntAttackModel].fVaim = 0.0f;                   //�x�N�g��
			g_aAttackModel[nCntAttackModel].fVXaim = 0.0f;                  //X�����̃x�N�g��
			g_aAttackModel[nCntAttackModel].fVZaim = 0.0f;                  //Z�����̃x�N�g��
			g_aAttackModel[nCntAttackModel].PosOld = NULL_VECTOR3;          //1f�O�̈ʒu
			g_aAttackModel[nCntAttackModel].DiffUse[nType]->a = 1.0f;
			g_aAttackModel[nCntAttackModel].DiffUse[nType]->r = 1.0f;
			g_aAttackModel[nCntAttackModel].DiffUse[nType]->g = 1.0f;
			g_aAttackModel[nCntAttackModel].DiffUse[nType]->b = 1.0f;
			g_aAttackModel[nCntAttackModel].OriginallyVtxMin = NULL_VECTOR3;
			g_aAttackModel[nCntAttackModel].OriginallyVtxMax = NULL_VECTOR3;
			g_aAttackModel[nCntAttackModel].vtxMin = NULL_VECTOR3;
			g_aAttackModel[nCntAttackModel].vtxMax = NULL_VECTOR3;
			g_aAttackModel[nCntAttackModel].Size = NULL_VECTOR3;	//�傫��
			g_aAttackModel[nCntAttackModel].bIsLanding = false;		//�n�ʂɂ��邩�ǂ���
			g_aAttackModel[nCntAttackModel].fSpeed = 0.0f;			//�X�s�[�h
			g_aAttackModel[nCntAttackModel].nNumLandingModel = 0;	//�ǂ̃��f���̏�ɏ���Ă��邩
			g_aAttackModel[nCntAttackModel].nTurnCnt = 0;            //�u�[���������^�[�����J�n������A�J�E���g���J�n
			g_aAttackModel[nCntAttackModel].bCurveLaser = false;                    //���[�U�[���J�[�u��������������߂�
			g_aAttackModel[nCntAttackModel].SupportPos = SupportPos;        //�x�_�ƂȂ�ʒu
			g_aAttackModel[nCntAttackModel].nIdxShadow = SetShadow();//�e�̃C���f�b�N�X
			g_aAttackModel[nCntAttackModel].fBoomerangRange = pPlayer->aBoomerangInfo.fBoomerangRenge;//�������ꂽ���_�ł̎˒���ۑ�
			//=============================
		    //�O�������@�_���ϐ�
		    //=============================
			g_aAttackModel[nCntAttackModel].fVXaim3D = 0.0f;
			g_aAttackModel[nCntAttackModel].fVYaim3D = 0.0f;
			g_aAttackModel[nCntAttackModel].fVZaim3D = 0.0f;
			g_aAttackModel[nCntAttackModel].fVLaim3D = 0.0f;
			
			//=============================
			//�X�t�B�A�ϐ�
			//=============================
			g_aAttackModel[nCntAttackModel].bSphereShot = false;//���C�t�������ɂȂ����玩�@�_���œ�����

			//���_���̎擾
			int nNumVtx = g_aAttackModelInfo[nType].pMesh->GetNumVertices();

			//���_�t�H�[�}�b�g�̃T�C�Y���擾
			DWORD sizeFVF = D3DXGetFVFVertexSize(g_aAttackModelInfo[nType].pMesh->GetFVF());

			//���_�o�b�t�@�̃��b�N
			g_aAttackModelInfo[nType].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

			for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
			{
				//���_���W�̑��
				D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

				//���f���̍ŏ��l�ƍő�l���擾
				if (vtx.x < g_aAttackModel[nCntAttackModel].OriginallyVtxMin.x)
				{//X���W�̍ŏ��l
					g_aAttackModel[nCntAttackModel].OriginallyVtxMin.x = vtx.x;
				}
				if (vtx.y < g_aAttackModel[nCntAttackModel].OriginallyVtxMin.y)
				{//Y���W�̍ŏ��l
					g_aAttackModel[nCntAttackModel].OriginallyVtxMin.y = vtx.y;
				}
				if (vtx.z < g_aAttackModel[nCntAttackModel].OriginallyVtxMin.z)
				{//Z���W�̍ŏ��l
					g_aAttackModel[nCntAttackModel].OriginallyVtxMin.z = vtx.z;
				}
				if (vtx.x > g_aAttackModel[nCntAttackModel].OriginallyVtxMax.x)
				{//X���W�̍ő�l
					g_aAttackModel[nCntAttackModel].OriginallyVtxMax.x = vtx.x;
				}
				if (vtx.y > g_aAttackModel[nCntAttackModel].OriginallyVtxMax.y)
				{//Y���W�̍ő�l
					g_aAttackModel[nCntAttackModel].OriginallyVtxMax.y = vtx.y;
				}
				if (vtx.z > g_aAttackModel[nCntAttackModel].OriginallyVtxMax.z)
				{//Z���W�̍ő�l
					g_aAttackModel[nCntAttackModel].OriginallyVtxMax.z = vtx.z;
				}


				//���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
				pVtxBuff += sizeFVF;
			}

			//���_�o�b�t�@�̃A�����b�N
			g_aAttackModelInfo[nType].pMesh->UnlockVertexBuffer();

			break;
		}
	}
}

//================================================================================================================================================================================================================================
// ���̎擾
//================================================================================================================================================================================================================================
AttackModel* GetAttackModel(void)
{
	return &g_aAttackModel[0];
}

float fRotCollisionAModel(float x0, float x1, float x2, float z0, float z1, float z2)
{
	float a = x2 - x1;
	float b = z2 - z1;
	float a2 = a * a;
	float b2 = b * b;
	float r2 = a2 + b2;
	float tt = -(a * (x1 - x0) + b * (z1 - z0));
	if (tt < 0) {
		return (x1 - x0) * (x1 - x0) + (z1 - z0) * (z1 - z0);
	}
	if (tt > r2) {
		return (x2 - x0) * (x2 - x0) + (z2 - z0) * (z2 - z0);
	}
	float f1 = a * (z1 - z0) - b * (x1 - x0);
	return (f1 * f1) / r2;
}
