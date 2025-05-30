//========================================================
//
//�P�Q���Q�U���F�`�[������̊�ՂÂ���[Summon.cpp]
//Author:ShinaTaiyo
//
// �ڍׁF��ɓG�̏��������Ă��鏈��
//=========================================================

#include "Summon.h"
#include "main.h"
#include "sound.h"
#include "player.h"
#include <stdio.h>
#include <string.h>
#include "timer.h"
#include "enemy.h"
#include "item.h"
#include "game.h"
#include "boss.h"
#include "enemy.h"
#include "editEnemy.h"
//�O���[�o���ϐ�
int g_nDelaySummon = 0;//���t���[�����ɏ������������Ƃ��Ɏg���B
float g_fRandumRot = 0;//���S�_�������������āA���̌����̂ǂ̈ʒu�ɏ������邩���߂�B
float g_fRandumLength = 0;//���S�_����̋���������āA���̋����̈ʒu�ɏ���������
int g_nType = 0;//�G�̎�ނ����蓖�Ă�
int g_nMaxSummon;//�Z�b�g������������G�̍ő吔���L�^����
bool g_bBossSummon;//�{�X�����t���O

Summon g_aSummon[MAX_SUMMON];//�G�����p�\����
SummonModelInfo g_aSummonModelInfo[NUM_ENEMY];
//�^�C�g����ʂ̏���������
void InitSummon(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�ւ̃|�C���^�Ǝ擾


	D3DXMATERIAL* pMat;//�}�e���A���ւ̃|�C���^
	g_nDelaySummon = 0;
	g_fRandumRot = 0;
	g_fRandumLength = 0;
	g_nType = 0;
	g_nMaxSummon = 0;

	for (int nCntModelInfo = 0; nCntModelInfo < NUM_ENEMY; nCntModelInfo++)
	{
		g_aSummonModelInfo[nCntModelInfo].dwNumMat = {};
		g_aSummonModelInfo[nCntModelInfo].pBuffMat = {};
		g_aSummonModelInfo[nCntModelInfo].pMesh = {};
	}

	for (int nCntSummon = 0; nCntSummon < MAX_SUMMON; nCntSummon++)
	{
		g_aSummon[nCntSummon].bUse = false;//�g�p���Ă��邩�ǂ���
		g_aSummon[nCntSummon].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�ʒu
		g_aSummon[nCntSummon].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�ړ���
		g_aSummon[nCntSummon].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//����
		g_aSummon[nCntSummon].nType = 0;//��������G�̎��
		g_aSummon[nCntSummon].nSummonTime = 0;//�G���������鎞��
		g_aSummon[nCntSummon].mtxWorld;//�}�g���b�N�X���[���h
		g_aSummon[nCntSummon].nNumSetEdit = -1;//�G�f�B�^�[�ŃZ�b�g���ꂽ�G�̔ԍ�
		g_aSummon[nCntSummon].bRemote = false;//���u����̕ύX���󂯂邩�ǂ���
		g_aSummon[nCntSummon].nLayoutType = 0;//�z�u�������@�̎��
	}



	for (int nCntType = 0; nCntType < NUM_ENEMY; nCntType++)
	{
		//�t�@�C���̓ǂݍ���
		D3DXLoadMeshFromX(SUMMON_FILENAME[nCntType],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_aSummonModelInfo[nCntType].pBuffMat,
			NULL,
			&g_aSummonModelInfo[nCntType].dwNumMat,
			&g_aSummonModelInfo[nCntType].pMesh);

		//�}�e���A���f�[�^�ւ̃|�C���^���擾
		pMat = (D3DXMATERIAL*)g_aSummonModelInfo[nCntType].pBuffMat->GetBufferPointer();

		for (int nCntSummon = 0; nCntSummon < MAX_SUMMON; nCntSummon++)
		{
			for (int nCntMat = 0; nCntMat < (int)g_aSummonModelInfo[nCntType].dwNumMat; nCntMat++)
			{
				g_aSummon[nCntSummon].Diffuse[nCntType][nCntMat] = pMat[nCntMat].MatD3D.Diffuse;
				g_aSummon[nCntSummon].FormarDiffuse[nCntType][nCntMat] = pMat[nCntMat].MatD3D.Diffuse;//���̐F����
			}
		}
	}



	char aString[100] = {};//�������ǂݍ��ނ��߂̕ϐ�
	int nType = 0;//�ǂݍ��񂾃��f���̎��
	int nLayoutType = 0;//�z�u���@�̎��

	float fPosX = 0.0f;//���f���̈ʒuX
	float fPosY = 0.0f;//���f���̈ʒuY
	float fPosZ = 0.0f;//���f���̈ʒuZ

	float fRotX = 0.0f;//���f���̈ʒuX
	float fRotY = 0.0f;//���f���̈ʒuY
	float fRotZ = 0.0f;//���f���̈ʒuZ

	int nSummonTime = 0;//�������鎞�Ԃ�錾����
	FILE* pFile;//�t�@�C���|�C���^

	pFile = fopen(SUMMONSET, "r");

	if (pFile != NULL)//�t�@�C�����J���Ă��邩�ǂ���
	{
		while (1)
		{

			fscanf(pFile, "%s", &aString[0]);


			/*	cData = fgetc(pFile);*/

			if (aString[0] == '#')
			{
				fgets(&aString[0], 100, pFile);
				continue;
			}

			if (strcmp(&aString[0], "ENDSCRIPT") == 0)
			{
				fclose(pFile);
				break;//�s�̍Ō�ł��̕�����𔻒肳���邱�Ƃɂ��A���[�v���I��点��
			}
			else if (strcmp(&aString[0], "SUMMONSET") == 0)
			{
				g_nMaxSummon++;//�G����������ő吔���C���N�������g����
				while (1)
				{
					fscanf(pFile, "%s", &aString[0]);
					if (aString[0] == '#')
					{
						fgets(&aString[0], 100, pFile);
						continue;
					}
					else if (strcmp(&aString[0], "TYPE") == 0)
					{
						fscanf(pFile, "%d", &nType);//�E�H�[���̎�ނ����߂�
					}
					else if (strcmp(&aString[0], "LAYOUTTYPE") == 0)
					{
						fscanf(pFile, "%d", &nLayoutType);//�z�u���@�̎�ނ����߂�
					}
					else if (strcmp(&aString[0], "POS") == 0)
					{
						fscanf(pFile, "%f%f%f", &fPosX, &fPosY, &fPosZ);//�ʒu����ǂݍ���
					}
					else if (strcmp(&aString[0], "ROT") == 0)
					{
						fscanf(pFile, "%f%f%f", &fRotX, &fRotY, &fRotZ);//��������ǂݍ���
					}
					else if (strcmp(&aString[0], "TIME") == 0)
					{
						fscanf(pFile, "%d", &nSummonTime);//�������鎞�Ԃ�ǂݍ��ށB
					}
					else if (strcmp(&aString[0], "ENDSUMMONSET") == 0)
					{//�������ŃZ�b�g���ꂽ�f�[�^�́A���ɂ���f�[�^�Ȃ̂ŁA�G�f�B�^�̔z�u�ɂ̓J�E���g���Ȃ�
						SetSummon(nType, nSummonTime, -1, nLayoutType,false, D3DXVECTOR3(fPosX, fPosY, fPosZ), D3DXVECTOR3(fRotX, fRotY, fRotZ));
						break;
					}
				}
			}
		}
	}
}
//============================
//���f���̏I������
//============================
void UninitSummon(void)
{
	for (int nCntSummon = 0; nCntSummon < NUM_ENEMY; nCntSummon++)
	{
		//���b�V���̔j��
		if (g_aSummonModelInfo[nCntSummon].pMesh != NULL)
		{
			g_aSummonModelInfo[nCntSummon].pMesh->Release();
			g_aSummonModelInfo[nCntSummon].pMesh = NULL;
		}
		//�}�e���A���̔j��
		if (g_aSummonModelInfo[nCntSummon].pBuffMat != NULL)
		{
			g_aSummonModelInfo[nCntSummon].pBuffMat->Release();
			g_aSummonModelInfo[nCntSummon].pBuffMat = NULL;
		}
	}
}

//============================
//���f���̍X�V����
//============================
void UpdateSummon(void)
{
	Player* pPlayer = GetPlayer();
	Boss* pBoss = GetBoss();

	bool bEditEnemyMode = GetEditEnemyMode();

	//======================================
	//�G�G�f�B�b�g���[�h�ȊO�̎��ɂ��鏈��
	//======================================
	if (bEditEnemyMode == false)
	{
		g_nDelaySummon++;

		if (g_nDelaySummon == 600)
		{
			SetEnemy(NULL_VECTOR3, NULL_VECTOR3, 4, false);
		}

		//======================================
		//���������G���I����
		//======================================
		g_fRandumRot = float(rand() % 200 + 1) / 100;
		int nGetTimer = GetTimer();//�c�莞�Ԃ̎擾
		int nLottery = rand() % 100 + 1;//��������G�𒊑I����
		//==================================================================================================================================================================


		//======================================
		//���Ԃ��ƂɌŒ�ʒu�ɓG��������
		//======================================
		if (pBoss->bUse == false)
		{
			for (int nCntSummon = 0; nCntSummon < MAX_SUMMON; nCntSummon++)
			{
				if (g_aSummon[nCntSummon].bUse == true && g_aSummon[nCntSummon].nSummonTime == nGetTimer)//�g�p���Ă��邩�A�������Ԃ���v������
				{
					if (g_aSummon[nCntSummon].nLayoutType == ENEMYEDITORLAYOUT00_WORLD)
					{
						SetEnemy(g_aSummon[nCntSummon].pos, g_aSummon[nCntSummon].rot, g_aSummon[nCntSummon].nType, false);
					}
					else if (g_aSummon[nCntSummon].nLayoutType == ENEMYEDITORLAYOUT01_PLAYERAROUND)
					{
						SetEnemy(pPlayer->pos + g_aSummon[nCntSummon].pos, g_aSummon[nCntSummon].rot, g_aSummon[nCntSummon].nType, false);

					}
					g_aSummon[nCntSummon].bUse = false;//��񏢊������̂ŗp�ς�
				}
			}
		}

		if (pBoss->bUse == false)
		{
			if (180 <= nGetTimer && nGetTimer <= 240)//�J�n�R�O�b
			{
				if (g_nDelaySummon % 140 == 0)
				{

					float fLength = 0.0f;
					int nLottery2 = rand() % 5;

					if (1 <= nLottery && nLottery <= 25)
					{
						g_nType = 0;//���b�h�u��
					}
					else if (26 <= nLottery && nLottery <= 50)
					{
						g_nType = 1;//�~���^���N
					}
					else if (51 <= nLottery && nLottery <= 75)
					{
						g_nType = 2;//�����X�^�[
					}
					else if (76 <= nLottery && nLottery <= 100)
					{
						g_nType = 3;//�~�b�N�`���W���[�`��
					}


					//==============================
					//�G�ɂ���ďo���ʒu�𒲐�
					//==============================
					if (g_nType == 1 || g_nType == 3)
					{
						if (nLottery2 >= 0 && nLottery2 <= 2)
						{//60%�̊m���ŉ����Ƃ���ɏo��B
							fLength = float(rand() % 500 + 700);
						}
						else
						{//40%�̊m���ŋ߂����ɏo��
							fLength = float(rand() % 500 + 300);

						}
					}
					else if (g_nType == 0 || g_nType == 2)
					{
						if (nLottery2 >= 0 && nLottery2 <= 2)
						{//60%�̊m���ŋ߂��Ƃ���ɏo��B
							fLength = float(rand() % 500 + 300);
						}
						else
						{//40%�̊m���ŉ������ɏo��
							fLength = float(rand() % 500 + 700);
						}
					}
					else
					{
						fLength = float(rand() % 1000 + 500);
					}

					//===========================================================================================

					SetEnemy(D3DXVECTOR3(pPlayer->pos.x + sinf(-D3DX_PI * g_fRandumRot) * fLength, pPlayer->pos.y, pPlayer->pos.z + cosf(-D3DX_PI * g_fRandumRot) * fLength),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), g_nType,false);
				}
			}
			else if (120 <= nGetTimer && nGetTimer <= 180)
			{
				if (g_bBossSummon == false)
				{
					g_bBossSummon = true;

				}
				if (g_nDelaySummon % 120 == 0)
				{
					float fLength = 0.0f;
					int nLottery2 = rand() % 5;

					if (1 <= nLottery && nLottery <= 25)
					{
						g_nType = 0;//���b�h�u��
					}
					else if (26 <= nLottery && nLottery <= 50)
					{
						g_nType = 1;//�~���^���N
					}
					else if (51 <= nLottery && nLottery <= 75)
					{
						g_nType = 2;//�����X�^�[
					}
					else if (76 <= nLottery && nLottery <= 100)
					{
						g_nType = 3;//�~�b�N�`���W���[�`��
					}

					//==============================
					//�G�ɂ���ďo���ʒu�𒲐�
					//==============================
					if (g_nType == 1 || g_nType == 3)
					{
						if (nLottery2 >= 0 && nLottery2 <= 2)
						{//60%�̊m���ŉ����Ƃ���ɏo��B
							fLength = float(rand() % 500 + 700);
						}
						else
						{//40%�̊m���ŋ߂����ɏo��
							fLength = float(rand() % 500 + 300);

						}
					}
					else if (g_nType == 0 || g_nType == 2)
					{
						if (nLottery2 >= 0 && nLottery2 <= 2)
						{//60%�̊m���ŋ߂��Ƃ���ɏo��B
							fLength = float(rand() % 500 + 300);
						}
						else
						{//40%�̊m���ŉ������ɏo��
							fLength = float(rand() % 500 + 700);
						}
					}
					else
					{
						fLength = float(rand() % 1000 + 500);
					}

					//===========================================================================================

					SetEnemy(D3DXVECTOR3(pPlayer->pos.x + sinf(-D3DX_PI * g_fRandumRot) * fLength, pPlayer->pos.y, pPlayer->pos.z + cosf(-D3DX_PI * g_fRandumRot) * fLength),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), g_nType, false);

				}
			}
			else if (60 <= nGetTimer && nGetTimer <= 120)
			{
				if (g_nDelaySummon % 100 == 0)
				{
					float fLength = 0.0f;
					int nLottery2 = rand() % 5;

					if (1 <= nLottery && nLottery <= 25)
					{
						g_nType = 0;//���b�h�u��
					}
					else if (26 <= nLottery && nLottery <= 50)
					{
						g_nType = 1;//�~���^���N
					}
					else if (51 <= nLottery && nLottery <= 75)
					{
						g_nType = 2;//�����X�^�[
					}
					else if (76 <= nLottery && nLottery <= 100)
					{
						g_nType = 3;//�~�b�N�`���W���[�`��
					}

					//==============================
					//�G�ɂ���ďo���ʒu�𒲐�
					//==============================
					if (g_nType == 1 || g_nType == 3)
					{
						if (nLottery2 >= 0 && nLottery2 <= 2)
						{//60%�̊m���ŉ����Ƃ���ɏo��B
							fLength = float(rand() % 500 + 700);
						}
						else
						{//40%�̊m���ŋ߂����ɏo��
							fLength = float(rand() % 500 + 300);

						}
					}
					else if (g_nType == 0 || g_nType == 2)
					{
						if (nLottery2 >= 0 && nLottery2 <= 2)
						{//60%�̊m���ŋ߂��Ƃ���ɏo��B
							fLength = float(rand() % 500 + 300);
						}
						else
						{//40%�̊m���ŉ������ɏo��
							fLength = float(rand() % 500 + 700);
						}
					}
					else
					{
						fLength = float(rand() % 1000 + 500);
					}

					//===========================================================================================

					SetEnemy(D3DXVECTOR3(pPlayer->pos.x + sinf(-D3DX_PI * g_fRandumRot) * fLength, pPlayer->pos.y, pPlayer->pos.z + cosf(-D3DX_PI * g_fRandumRot) * fLength),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), g_nType, false);
				}
			}

			else if (0 <= nGetTimer && nGetTimer <= 60)
			{
				if (g_nDelaySummon % 60 == 0)
				{
					float fLength = 0.0f;
					int nLottery2 = rand() % 5;
					if (1 <= nLottery && nLottery <= 25)
					{
						g_nType = 0;//���b�h�u��
					}
					else if (26 <= nLottery && nLottery <= 50)
					{
						g_nType = 1;//�~���^���N
					}
					else if (51 <= nLottery && nLottery <= 75)
					{
						g_nType = 2;//�����X�^�[
					}
					else if (76 <= nLottery && nLottery <= 100)
					{
						g_nType = 3;//�~�b�N�`���W���[�`��
					}
					//==============================
					//�G�ɂ���ďo���ʒu�𒲐�
					//==============================
					if (g_nType == 1 || g_nType == 3)
					{
						if (nLottery2 >= 0 && nLottery2 <= 2)
						{//60%�̊m���ŉ����Ƃ���ɏo��B
							fLength = float(rand() % 500 + 700);
						}
						else
						{//40%�̊m���ŋ߂����ɏo��
							fLength = float(rand() % 500 + 300);

						}
					}
					else if (g_nType == 0 || g_nType == 2)
					{
						if (nLottery2 >= 0 && nLottery2 <= 2)
						{//60%�̊m���ŋ߂��Ƃ���ɏo��B
							fLength = float(rand() % 500 + 300);
						}
						else
						{//40%�̊m���ŉ������ɏo��
							fLength = float(rand() % 500 + 700);
						}
					}
					else
					{
						fLength = float(rand() % 1000 + 500);
					}

					//===========================================================================================

					SetEnemy(D3DXVECTOR3(pPlayer->pos.x + sinf(-D3DX_PI * g_fRandumRot) * fLength, pPlayer->pos.y, pPlayer->pos.z + cosf(-D3DX_PI * g_fRandumRot) * fLength),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), g_nType, false);
				}
			}
		}
		//=====================================================================================================================================================================

	}
	//====================================================================================================================================================================================
	
}

//==============================
//�`�揈��
//==============================
void DrawSummon(void)
{
	EnemyEditor* pEnemyEditor = GetEnemyEditor();

	LPDIRECT3DDEVICE9 pDevice = GetDevice();;//�f�o�C�X�ւ̃|�C���^�Ǝ擾

	D3DXMATRIX mtxRot, mtxTrans;//�v�Z�p�}�g���b�N�X

	D3DMATERIAL9 matDef;//���݂̃}�e���A���ۑ��p

	D3DXMATERIAL* pMat;//�}�e���A���f�[�^�ւ̃|�C���^


	//���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	for (int nCntSummon = 0; nCntSummon < MAX_SUMMON; nCntSummon++)
	{
		if (g_aSummon[nCntSummon].nSummonTime == pEnemyEditor->nSummonTime && g_aSummon[nCntSummon].bUse == true && g_aSummon[nCntSummon].nLayoutType == pEnemyEditor->nLayoutType)
		{//�g�p��Ԃł��邩�A�������Ԃ��A�G�G�f�B�^�̏������Ԃƈ�v���Ă�����`����J�n

			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aSummon[nCntSummon].mtxWorld);

			//�����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aSummon[nCntSummon].rot.y, g_aSummon[nCntSummon].rot.x, g_aSummon[nCntSummon].rot.z);

			D3DXMatrixMultiply(&g_aSummon[nCntSummon].mtxWorld, &g_aSummon[nCntSummon].mtxWorld, &mtxRot);

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aSummon[nCntSummon].pos.x, g_aSummon[nCntSummon].pos.y, g_aSummon[nCntSummon].pos.z);
			D3DXMatrixMultiply(&g_aSummon[nCntSummon].mtxWorld, &g_aSummon[nCntSummon].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aSummon[nCntSummon].mtxWorld);


			//�}�e���A���ւ̃|�C���^���擾�i�Z�b�g�������f���Ɠ����^�C�v�j
			pMat = (D3DXMATERIAL*)g_aSummonModelInfo[g_aSummon[nCntSummon].nType].pBuffMat->GetBufferPointer();

			//�Z�b�g���ꂽ���f���ƃ^�C�v���������̂̃��f���̃}�e���A���̐����ǂݍ��ށB
			for (int nCntMat = 0; nCntMat < (int)g_aSummonModelInfo[g_aSummon[nCntSummon].nType].dwNumMat; nCntMat++)
			{
				pMat[nCntMat].MatD3D.Diffuse = g_aSummon[nCntSummon].Diffuse[g_aSummon[nCntSummon].nType][nCntMat];

				//�}�e���A���̐ݒ�
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//�e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, NULL);

				//���f���i�p�[�c�j�̕`��
				g_aSummonModelInfo[g_aSummon[nCntSummon].nType].pMesh->DrawSubset(nCntMat);
			}

			//�ۑ����Ă����}�e���A����߂�
			pDevice->SetMaterial(&matDef);
		}
	}
}


//==============================
//��������G�̏��̃Z�b�g
//==============================
void SetSummon(int nType, int nTime, int nNumSetEdit, int nLayoutType, bool bRemote, D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	for (int nCntSummon = 0; nCntSummon < MAX_SUMMON; nCntSummon++)
	{
		if (g_aSummon[nCntSummon].bUse == false && nTime != 240)
		{
			g_aSummon[nCntSummon].bUse = true;
			g_aSummon[nCntSummon].pos = pos;//�ʒu
			g_aSummon[nCntSummon].rot = rot;//����
			g_aSummon[nCntSummon].nType = nType;//��������G�̎��
			g_aSummon[nCntSummon].nSummonTime = nTime;//�G���������鎞��
			g_aSummon[nCntSummon].nNumSetEdit = nNumSetEdit;//�G�f�B�^�ŏ������ꂽ�G�̔ԍ�
			g_aSummon[nCntSummon].bRemote = bRemote;//�ҏW�\���ǂ���
			g_aSummon[nCntSummon].nLayoutType = nLayoutType;//�z�u�������@�̎��

			for (int nCntMat = 0; nCntMat < (int)(g_aSummonModelInfo[nType].dwNumMat); nCntMat++)
			{
				g_aSummon[nCntSummon].Diffuse[nType][nCntMat].a = 0.5f;
			}
			break;
		}
	}
}

//===================================
//�G���������鐔���擾����
//===================================
int GetMaxSummon(void)
{
	return g_nMaxSummon;
}

//===================================
//�\���̂̏����擾
//===================================
Summon* GetSummon(void)
{
	return &g_aSummon[0];
}

