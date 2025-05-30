//========================================================
//
//�P�P���V���F���f���ǂݍ���[EditEnemy.cpp]
//Author:ShinaTaiyo
//
//=========================================================

#include "editEnemy.h"
#include "model.h"
#include "main.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "shadow.h"
#include "player.h"
#include <stdio.h>
#include <string.h>
#include "enemy.h"
#include "edit.h"
//�O���[�o���ϐ�
int g_nNumEditEnemy = 0;
#define EditEnemy_SCALE (10.0f)
#define EditEnemySET "data\\TEXTFILE\\EditEnemy.txt"
#define SAVEEditEnemy "data\\TEXTFILE\\SaveEditEnemy.txt"
#define SAVEENEMY "data\\TEXTFILE\\SaveEnemy.txt"


LPD3DXMESH g_pMeshEditEnemy[NUM_EDITENEMY] = {};//���b�V���i���_���j�ւ̃|�C���^�i���f���̎�ސ����j
LPD3DXBUFFER g_pBuffMatEditEnemy[NUM_EDITENEMY] = {};//�}�e���A���ւ̃|�C���^
DWORD g_dwNumMatEditEnemy[NUM_EDITENEMY] = {};//�}�e���A���̐�
LPDIRECT3DTEXTURE9 g_apTextureEditEnemy[NUM_EDITENEMY] = {};
D3DXVECTOR3 g_posEditEnemy;//�ʒu
D3DXVECTOR3 g_rotEditEnemy;//����
D3DXMATRIX g_mtxWorldEditEnemy;//���[���h�}�g���b�N�X

EditEnemy g_aEditEnemy;//���f�����̍\����

//int g_nGetNumModel = 0;//�擾�������f���̍ő吔��ۑ�����
int g_nSetNumEnemy = 0;//���f����ݒu���鎞�̃^�C�v�ԍ���ۑ�����
int g_nSaveSetEnemy = 0;//���f����ݒu��������ۑ�����
EditEnemy g_aSaveEditEnemyModel[MAX_MODEL];

//int g_nldxShadow = -1;//�Ώۂ̉e�̃C���f�b�N�X�i�ԍ��j

//�^�C�g����ʂ̏���������
void InitEditEnemy(void)
{

	//g_nGetNumModel = GetNumModel();//�擾�������f���̍ő吔��ۑ�����

	g_nSetNumEnemy = 0;//���f����ݒu���鎞�̃^�C�v�ԍ���ۑ�����

	g_nSaveSetEnemy = 0;//���f����ݒu��������ۑ�����

	//for (int nCntEditEnemy = 0; nCntEditEnemy < NUM_EDITENEMY; nCntEditEnemy++)
	//{
	//	g_dwNumMatEditEnemy[nCntEditEnemy] = 0;//�}�e���A���̐�
	//}
	//g_nldxShadow = 0;//�Ώۂ̉e�̃C���f�b�N�X

	g_nNumEditEnemy = 2;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�ւ̃|�C���^�Ǝ擾

	g_posEditEnemy = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_rotEditEnemy = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	g_nNumEditEnemy = 0;

	for (int nCntEditEnemyModel = 0; nCntEditEnemyModel < MAX_MODEL; nCntEditEnemyModel++)
	{
		g_aSaveEditEnemyModel[nCntEditEnemyModel].bUse = false;
		g_aSaveEditEnemyModel[nCntEditEnemyModel].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aSaveEditEnemyModel[nCntEditEnemyModel].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aSaveEditEnemyModel[nCntEditEnemyModel].nType = 0;
	}
	////���f���̓ǂݍ���
	//D3DXLoadMeshFromX("data\\EditEnemy\\bill.x",
	//	D3DXMESH_SYSTEMMEM,
	//	pDevice,
	//	NULL,
	//	&g_pBuffMatEditEnemy,
	//	NULL,
	//	&g_dwNumMatEditEnemy,
	//	&g_pMeshEditEnemy[EditEnemyTYPE_BILL]);

	//D3DXLoadMeshFromX("data\\EditEnemy\\BigBriddge.x",
	//	D3DXMESH_SYSTEMMEM,
	//	pDevice,
	//	NULL,
	//	&g_pBuffMatEditEnemy,
	//	NULL,
	//	&g_dwNumMatEditEnemy,
	//	&g_pMeshEditEnemy[EditEnemyTYPE_BRIDDGE]);




	for (int nCnt = 0; nCnt < MAX_EDITENEMY; nCnt++)
	{
		g_aEditEnemy.bUse = false;//�g�p���Ă��邩�ǂ���
		g_aEditEnemy.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�ʒu
		g_aEditEnemy.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//����
		g_aEditEnemy.nIdxShadow = -1;
		g_aEditEnemy.nType = 0;
	}

	g_aEditEnemy.bUse = true;


	char aString[100] = {};//�������ǂݍ��ނ��߂̕ϐ�
	int nMaxRoadEditEnemy = 0;//�ǂݍ��ރ��f���̍ő吔
	int nType = 0;//�ǂݍ��񂾃��f���̎��

	float fPosX = 0;//���f���̈ʒuX
	float fPosY = 0;//���f���̈ʒuY
	float fPosZ = 0;//���f���̈ʒuZ

	float fRotX = 0;//���f���̈ʒuX
	float fRotY = 0;//���f���̈ʒuY
	float fRotZ = 0;//���f���̈ʒuZ
	int nCnt = 0;//�ǂݍ��ރ��f���𐔂���p
	FILE* pFile;//�t�@�C���|�C���^

	char aStringPass[100] = {};

	pFile = fopen(ENEMYSET, "r");

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
			else if (strcmp(&aString[0], "NUM_ENEMY") == 0)//�ǂݍ��ރ��f���̎�ސ������߂�
			{
				fscanf(pFile, "%d", &g_nNumEditEnemy);

				for (int nCnt = 0; nCnt < 3; nCnt++)
				{
					fscanf(pFile, "%s", &aString[0]);
				}
				/*for (int nRoadEditEnemy = 0; nRoadEditEnemy < g_nNumEditEnemy; nRoadEditEnemy++)
				{*/
				/*	while (1)
					{*/
				int nRoadEditEnemy = 0;
				do
				{
					fscanf(pFile, "%s", &aString[0]);

					if (aString[0] == '#')
					{
						fgets(&aString[0], 100, pFile);
						continue;
					}
					else if (strcmp(&aString[0], "ENEMY_FILENAME") == 0)
					{
						fscanf(pFile, "%s", &aStringPass[0]);
						//���f���̓ǂݍ���
						D3DXLoadMeshFromX(&aStringPass[0],
							D3DXMESH_SYSTEMMEM,
							pDevice,
							NULL,
							&g_pBuffMatEditEnemy[nRoadEditEnemy],//���f���ł́A������z��ɂ���
							NULL,
							&g_dwNumMatEditEnemy[nRoadEditEnemy],//���f���ł́A������z��ɂ���
							&g_pMeshEditEnemy[nRoadEditEnemy]);
						fscanf(pFile, "%s", &aString[0]);
						nRoadEditEnemy++;
					}

				} while (nRoadEditEnemy < g_nNumEditEnemy);
				/*	nCnt++;
					if (nCnt == g_nNumEditEnemy)
					{
						break;
					}*/
					//}
			}
		}
	}
}

//==============================
//�^�C�g����ʂ̏I������
//==============================
void UninitEditEnemy(void)
{
	for (int nCnt = 0; nCnt < NUM_EDITENEMY; nCnt++)
	{
		//���b�V���̔j��
		if (g_pMeshEditEnemy[nCnt] != NULL)
		{
			g_pMeshEditEnemy[nCnt]->Release();
			g_pMeshEditEnemy[nCnt] = NULL;
		}
		//�}�e���A���̔j��
		if (g_pBuffMatEditEnemy[nCnt] != NULL)
		{
			g_pBuffMatEditEnemy[nCnt]->Release();
			g_pBuffMatEditEnemy[nCnt] = NULL;
		}
	}
}
//============================
//���f���̍X�V����
//============================
void UpdateEditEnemy(void)
{
	Player* pPlayer = GetPlayer();
	Edit* pEdit = GetEdit();
	if (pEdit->nEditMode == EDITTYPE_ENEMY)
	{
		if (GetKeyboardTrigger(DIK_RIGHT) == true)
		{
			g_aEditEnemy.nType++;
		}
		else if (GetKeyboardTrigger(DIK_LEFT) == true)
		{
			g_aEditEnemy.nType--;
		}

		if (g_aEditEnemy.nType <= -1)
		{
			g_aEditEnemy.nType = g_nNumEditEnemy;
		}
		else if (g_aEditEnemy.nType > g_nNumEditEnemy)
		{
			g_aEditEnemy.nType = 0;
		}

		g_aEditEnemy.pos = pPlayer->pos;
		g_aEditEnemy.rot = pPlayer->rot;
		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{
			SetEditEnemy(g_aEditEnemy.pos, g_aEditEnemy.rot, g_aEditEnemy.nType);
		}

		if (GetKeyboardTrigger(DIK_F10) == true)
		{

			FILE* pFile;//�t�@�C���|�C���^��錾
			pFile = fopen(SAVEEditEnemy, "w");//�t�@�C���ɍ���z�u�������f�����������o��

			if (pFile != NULL)//�t�@�C�����J���Ă�����
			{
				for (int nCntSaveModel = 0; nCntSaveModel < g_nSaveSetEnemy - 1; nCntSaveModel++)
				{
					fprintf(pFile, "ENEMYSET\n");
					fprintf(pFile, "TYPE %d\n", g_aSaveEditEnemyModel[nCntSaveModel].nType);//�ݒu�������f���̎�ނ������o��
					fprintf(pFile, "POS %.3f  %.3f  %.3f\n", g_aSaveEditEnemyModel[nCntSaveModel].pos.x
						, g_aSaveEditEnemyModel[nCntSaveModel].pos.y, g_aSaveEditEnemyModel[nCntSaveModel].pos.z);//�ݒu�������f���̈ʒu�������o��
					fprintf(pFile, "ROT %.3f  %.3f  %.3f\n", g_aSaveEditEnemyModel[nCntSaveModel].rot.x
						, g_aSaveEditEnemyModel[nCntSaveModel].rot.y, g_aSaveEditEnemyModel[nCntSaveModel].rot.z);//�ݒu�������f���̌����������o��
					fprintf(pFile, "ENDENEMYSET\n");
					fprintf(pFile, "\n");
				}
				fclose(pFile);
			}
		}

	}
}

//================================
//�^�C�g����ʂ̕`�揈��
//================================
void DrawEditEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();;//�f�o�C�X�ւ̃|�C���^�Ǝ擾


	/*for (int nCntEditEnemy = 0; nCntEditEnemy < MAX_EDITENEMY; nCntEditEnemy++)
	{*/
		/*	if (g_aEditEnemy.bUse == true)
			{*/
		D3DXMATRIX mtxRot, mtxTrans;//�v�Z�p�}�g���b�N�X

		D3DMATERIAL9 matDef;//���݂̃}�e���A���ۑ��p

		D3DXMATERIAL* pMat;//�}�e���A���f�[�^�ւ̃|�C���^

		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_mtxWorldEditEnemy);

		//�����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aEditEnemy.rot.y, g_aEditEnemy.rot.x, g_aEditEnemy.rot.z);

		D3DXMatrixMultiply(&g_mtxWorldEditEnemy, &g_mtxWorldEditEnemy, &mtxRot);

		//�ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, g_aEditEnemy.pos.x, g_aEditEnemy.pos.y, g_aEditEnemy.pos.z);
		D3DXMatrixMultiply(&g_mtxWorldEditEnemy, &g_mtxWorldEditEnemy, &mtxTrans);

		//���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldEditEnemy);

		//���݂̃}�e���A�����擾
		pDevice->GetMaterial(&matDef);

		for (int nCntEditEnemySET = 0; nCntEditEnemySET < g_nNumEditEnemy; nCntEditEnemySET++)
		{//�ǂݍ��ރ��f���̎�ސ���for�����񂵁A�����^�C�v�̃}�e���A���̃|�C���^��A�A
		//�p�[�c�̕`��A�}�e���A���̐ݒ������B
			if (g_aEditEnemy.nType == nCntEditEnemySET)
			{
				//�}�e���A���ւ̃|�C���^���擾�i�Z�b�g�������f���Ɠ����^�C�v�j
				pMat = (D3DXMATERIAL*)g_pBuffMatEditEnemy[nCntEditEnemySET]->GetBufferPointer();

				//�Z�b�g���ꂽ���f���ƃ^�C�v���������̂̃��f���̃}�e���A���̐����ǂݍ��ށB
				for (int nCntMat = 0; nCntMat < (int)g_dwNumMatEditEnemy[nCntEditEnemySET]; nCntMat++)
				{
					//�}�e���A���̐ݒ�
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

					//�e�N�X�`���̐ݒ�
					pDevice->SetTexture(0, NULL);

					//���f���i�p�[�c�j�̕`��
					g_pMeshEditEnemy[nCntEditEnemySET]->DrawSubset(nCntMat);
				}
				break;
			}
		}

		//�ۑ����Ă����}�e���A����߂�
		pDevice->SetMaterial(&matDef);
		//}
	//}
}

//void SetEditEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType)
//{
//	for (int nCntEditEnemy = 0; nCntEditEnemy < MAX_EDITENEMY; nCntEditEnemy++)
//	{
//		if (g_aEditEnemy.bUse == false)
//		{
//			g_aEditEnemy.bUse = true;//�g�p���Ă��邩�ǂ���
//			g_aEditEnemy.pos = pos;//�ʒu
//			g_aEditEnemy.rot = rot;//����
//			g_aEditEnemy.nType = nType;//���
//			g_aEditEnemy.nIdxShadow = SetShadow();//�e�̃C���f�b�N�X��ݒ�
//			break;
//		}
//	}
//}

void SetEditEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType)
{
	/*for (int nCntEditEnemy = 0; nCntEditEnemy < MAX_EDITENEMY; nCntEditEnemy++)
	{
		if (g_aEditEnemy.bUse == false)
		{*/
	Player* pPlayer = GetPlayer();
	g_aEditEnemy.bUse = true;//�g�p���Ă��邩�ǂ���
	g_aEditEnemy.nType = nType;
	g_aEditEnemy.pos = pos;
	g_aEditEnemy.rot = rot;

	g_aSaveEditEnemyModel[g_nSaveSetEnemy].nType = nType;
	g_aSaveEditEnemyModel[g_nSaveSetEnemy].pos = pos;
	g_aSaveEditEnemyModel[g_nSaveSetEnemy].rot = rot;
	SetEnemy(g_aEditEnemy.pos, g_aEditEnemy.rot, g_aEditEnemy.nType);

	g_nSaveSetEnemy++;//�ݒu�������f���̍ő吔���C���N�������g����
	/*while (1)
	{
		g_aEditEnemy.pos = pPlayer->pos;
		g_aEditEnemy.rot = pPlayer->rot;

		if (GetKeyboardTrigger(DIK_RIGHT) == true)
		{
			g_aEditEnemy.nType++;
		}
		else if (GetKeyboardTrigger(DIK_LEFT) == true)
		{
			g_aEditEnemy.nType--;
		}

		if (g_aEditEnemy.nType <= -1)
		{
			g_aEditEnemy.nType = g_nNumEditEnemy;
		}
		else if (g_aEditEnemy.nType > g_nNumEditEnemy)
		{
			g_aEditEnemy.nType = 0;
		}

		if (GetKeyboardTrigger(DIK_F10) == true)
		{
			SetModel(g_aEditEnemy.pos, g_aEditEnemy.rot, g_aEditEnemy.nType);
			break;
		}
	}
	break;*/
	//	}
	//}
}

