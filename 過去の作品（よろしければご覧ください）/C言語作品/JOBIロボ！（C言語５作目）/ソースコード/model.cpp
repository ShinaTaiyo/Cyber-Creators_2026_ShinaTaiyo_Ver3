//================================================================================================================================================================================================================================
//
// �I�u�W�F�N�g�̏���
// Author : Atsumu Kuboichi
//
//================================================================================================================================================================================================================================
#include "model.h"
#include "game.h"
#include "input.h"
#include <cstdio>
#include "edit.h"
#include "billboard.h"
#include "Rader.h"
#include "camera.h"
#include "particle.h"
#include "battery.h"
//================================================================================================================================================================================================================================
// �O���[�o���ϐ�
//================================================================================================================================================================================================================================
LPDIRECT3DTEXTURE9 g_apTextureModel[NUM_MODEL][MAX_MODELMAT] = {};	//�e�N�X�`���ւ̃|�C���^
Model g_aModel[MAX_MODEL];											//�I�u�W�F�N�g���
int g_nUse;															//�Q�[���t�B�[���h�����у��{�n�E�X�����̔���(0��1)
bool g_bIsLandingModel;												//���f���̏�ɂ��邩�ǂ����𔻒肷��
int g_nMaxModel;													//�I�u�W�F�N�g�̃f�[�^
Model g_aModelInfo[MAX_TEXT];										//���f���̓ǂݍ��ݏ��
int g_nCntModel = 0;												//���f���̃f�[�^��
int g_nDelayModel = 0;												//�f�B���C

//================================================================================================================================================================================================================================
// ����������
//================================================================================================================================================================================================================================
void InitModel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̎擾
	D3DXMATERIAL* pMat;						//�}�e���A���ւ̃|�C���^

	//�\���̂̏��
	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{
		g_aModel[nCntModel].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�ʒu
		g_aModel[nCntModel].posOld = NULL_VECTOR3;                  //1f�O�̈ʒu
		g_aModel[nCntModel].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�ړ���
		g_aModel[nCntModel].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//����
		g_aModel[nCntModel].nType = 0;								//���
		g_aModel[nCntModel].bUse = false;							//�g�p���Ă���
		g_aModel[nCntModel].nUse = -1;								//�g�p�t�B�[���h(0 = �Q�[���t�B�[���h�A1 = ���у��{�n�E�X)
		g_aModel[nCntModel].nCntTime = 0;                           //�l�X�ȃ^�C�~���O�Ŏ��Ԃ𑪂�
		g_aModel[nCntModel].SavePos = NULL_VECTOR3;                 //�����ʒu��ۑ�����
		g_aModel[nCntModel].nIdxRaderDisplay = -1;                  //���[�_�[�f�B�X�v���C�̃C���f�b�N�X

		//======================================
		//���t�g���菈��
		//======================================
		g_aModel[nCntModel].bStopLift = false;	//���t�g���~�߂�
		g_aModel[nCntModel].bUpDownLift = false;//�t���O�̐؂�ւ��Ń��t�g���㉺������
	}

	//�ϐ�
	g_nUse = 0;

	//=====================================
	//��u�Q�[����ʂ��f���o�����΍�
	//=====================================
	if (GetMode() == MODE_TUTORIAL)
	{
		g_nUse = 1;
	}

	//==========================================================

	g_bIsLandingModel = false;
	g_nDelayModel = 0;//�f�B���C

	for (int nCntType = 0; nCntType < NUM_MODEL; nCntType++)
	{
		//X�t�@�C���̓ǂݍ���
		D3DXLoadMeshFromX(MODEL_FILENAME[nCntType], D3DXMESH_SYSTEMMEM, pDevice, NULL,
			&g_aModel[nCntType].pBuffMat, NULL, &g_aModel[nCntType].dwNumMat, &g_aModel[nCntType].pMesh);

		//�}�e���A���f�[�^�ւ̃|�C���^���擾
		pMat = (D3DXMATERIAL*)g_aModel[nCntType].pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_aModel[nCntType].dwNumMat; nCntMat++)
		{
			//�e�N�X�`���t�@�C�������݂���
			if (pMat[nCntMat].pTextureFilename != NULL)
			{
				//�e�N�X�`���̓ǂݍ���
				D3DXCreateTextureFromFile(pDevice,
					pMat[nCntMat].pTextureFilename,
					&g_apTextureModel[nCntType][nCntMat]);
			}
		}
	}
}

//================================================================================================================================================================================================================================
// �I������
//================================================================================================================================================================================================================================
void UninitModel(void)
{
	for (int nCntModel = 0; nCntModel < NUM_MODEL; nCntModel++)
	{
		//���b�V���̔j��
		if (g_aModel[nCntModel].pMesh != NULL)
		{
			g_aModel[nCntModel].pMesh->Release();
			g_aModel[nCntModel].pMesh = NULL;
		}
		//�}�e���A���̔j��
		if (g_aModel[nCntModel].pBuffMat != NULL)
		{
			g_aModel[nCntModel].pBuffMat->Release();
			g_aModel[nCntModel].pBuffMat = NULL;
		}

		//�e�N�X�`���̔j��
		for (int nCntMat = 0; nCntMat < MAX_MODELMAT; nCntMat++)
		{
			if (g_apTextureModel[nCntModel][nCntMat] != NULL)
			{
				g_apTextureModel[nCntModel][nCntMat]->Release();
				g_apTextureModel[nCntModel][nCntMat] = NULL;
			}
		}
	}
}

//================================================================================================================================================================================================================================
// �X�V����
//================================================================================================================================================================================================================================
void UpdateModel(void)
{
	//if (GetMode() != MODE_TUTORIAL)
	//{//�`���[�g���A����ʂł͂Ȃ��ꍇ
	//	g_nUse = 0;
	//}
	//else
	//{//�`���[�g���A����ʂ̏ꍇ
	//	g_nUse = 1;
	//}

	if (GetMode() == MODE_GAME)
	{//�Q�[����ʂ̏ꍇ
		g_nDelayModel++;//�f�B���C

		if (g_nDelayModel == 1)
		{//�Q�[�����n�܂������Ɉ�x��������
			for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
			{
				//�g�p�X�e�[�W����v����ꍇ
				if (g_aModel[nCntModel].nUse == g_nUse)
				{//�g�p����
					g_aModel[nCntModel].bUse = true;
				}
				//��v���Ȃ��ꍇ
				else
				{//�g�p���Ȃ�
					g_aModel[nCntModel].bUse = false;
				}
			}
		}
	}
	else
	{//�Q�[����ʂł͂Ȃ��ꍇ
		for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
		{
			//�g�p�X�e�[�W����v����ꍇ
			if (g_aModel[nCntModel].nUse == g_nUse)
			{//�g�p����
				g_aModel[nCntModel].bUse = true;
			}
			//��v���Ȃ��ꍇ
			else
			{//�g�p���Ȃ�
				g_aModel[nCntModel].bUse = false;
			}
		}
	}

	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{
		if (g_aModel[nCntModel].bUse)
		{
			switch (g_aModel[nCntModel].nType)
			{
			case 18://���t�g
				g_aModel[nCntModel].nCntTime++;
				
				//================================================
			    //���̍����ɒB������A���t�g�̓������~�߂�
			    //================================================
				if (g_aModel[nCntModel].bStopLift)
				{
					if (g_aModel[nCntModel].nCntTime == 100)
					{
						g_aModel[nCntModel].bStopLift = false;
						g_aModel[nCntModel].nCntTime = 0;
					}
				}
				
				//==================================
				//���t�g�̈ړ�������ς���
				//==================================
				if (g_aModel[nCntModel].pos.y > 300.0f + g_aModel[nCntModel].SavePos.y)
				{
					g_aModel[nCntModel].pos.y = 300.0f + g_aModel[nCntModel].SavePos.y;

					if (!g_aModel[nCntModel].bStopLift)
					{
						g_aModel[nCntModel].move = NULL_VECTOR3;
						g_aModel[nCntModel].bUpDownLift = g_aModel[nCntModel].bUpDownLift ? false : true;
						g_aModel[nCntModel].nCntTime = 0;
						g_aModel[nCntModel].bStopLift = true;
					}
				}
				else if (g_aModel[nCntModel].pos.y < g_aModel[nCntModel].SavePos.y)
				{
					g_aModel[nCntModel].pos.y = g_aModel[nCntModel].SavePos.y;

					if (!g_aModel[nCntModel].bStopLift)
					{
						g_aModel[nCntModel].move = NULL_VECTOR3;
						g_aModel[nCntModel].bUpDownLift = g_aModel[nCntModel].bUpDownLift ? false : true;
						g_aModel[nCntModel].nCntTime = 0;
						g_aModel[nCntModel].bStopLift = true;
					}
				}
				
				//============================
				//���t�g���㉺�ړ�������
				//============================
				if (!g_aModel[nCntModel].bStopLift)
				{
					if (!g_aModel[nCntModel].bUpDownLift)
					{
						g_aModel[nCntModel].move.y = 2.0f;
					}
					else
					{
						g_aModel[nCntModel].move.y = -2.0f;
					}
				}
				
				break;

			default:
				break;
			}

			if (g_aModel[nCntModel].nType == 19 || g_aModel[nCntModel].nType == 41)
			{
				if (g_nDelayModel == 1)
				{
					g_aModel[nCntModel].nIdxRaderDisplay = SetRaderDisplay(RADERDISPLAY03_OUTLET);
					SetPositionRaderDisPlay(g_aModel[nCntModel].nIdxRaderDisplay, g_aModel[nCntModel].pos,0.0f);
				}
			}

			//===================================
			//�ړ��ʂ̔��f
			//===================================
			g_aModel[nCntModel].posOld = g_aModel[nCntModel].pos;
			g_aModel[nCntModel].pos += g_aModel[nCntModel].move;
		}
	}
}

//================================================================================================================================================================================================================================
// �`�揈��
//================================================================================================================================================================================================================================
void DrawModel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans;			//�v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;					//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL* pMat;						//�}�e���A���̃f�[�^�ւ̃|�C���^

	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{
		//�g�p����Ă���ꍇ
		if (g_aModel[nCntModel].bUse == true)
		{
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aModel[nCntModel].mtxWorld);

			//�����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aModel[nCntModel].rot.y, g_aModel[nCntModel].rot.x, g_aModel[nCntModel].rot.z);
			D3DXMatrixMultiply(&g_aModel[nCntModel].mtxWorld, &g_aModel[nCntModel].mtxWorld, &mtxRot);

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aModel[nCntModel].pos.x, g_aModel[nCntModel].pos.y, g_aModel[nCntModel].pos.z);
			D3DXMatrixMultiply(&g_aModel[nCntModel].mtxWorld, &g_aModel[nCntModel].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aModel[nCntModel].mtxWorld);

			//���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			//�}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_aModel[g_aModel[nCntModel].nType].pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aModel[g_aModel[nCntModel].nType].dwNumMat; nCntMat++)
			{
				//�}�e���A���̐ݒ�
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//�e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_apTextureModel[g_aModel[nCntModel].nType][nCntMat]);

				//���f��(�p�[�c)�̕`��
				g_aModel[g_aModel[nCntModel].nType].pMesh->DrawSubset(nCntMat);
			}

			//�ۑ����Ă����}�e���A����߂�
			pDevice->SetMaterial(&matDef);
		}
	}
}

//================================================================================================================================================================================================================================
// �ݒ菈��
//================================================================================================================================================================================================================================
void SetModel(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nUse)
{
	BYTE* pVtxBuff;	//���_�o�b�t�@�ւ̃|�C���^

	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{
		if (!g_aModel[nCntModel].bUse)
		{//�g�p����Ă��Ȃ��ꍇ
			g_aModel[nCntModel].pos = pos;		//�ʒu
			g_aModel[nCntModel].SavePos = pos;	//�����ʒu��ۑ�����
			g_aModel[nCntModel].rot = rot;		//����
			g_aModel[nCntModel].nType = nType;	//���
			g_aModel[nCntModel].nUse = nUse;	//�g�p�t�B�[���h
			g_aModel[nCntModel].bUse = true;	//�g�p����
			g_aModel[nCntModel].nCntTime = 0;   //�l�X�ȃ^�C�~���O�Ŏ��Ԃ𑪂�

			//�����蔻��̗L��
			if (nType == 2 || nType == 6 || nType == 19 || nType == 41)
			{//�����蔻�薳��
				g_aModel[nCntModel].bCollision = false;

				if (nType == 19 || nType == 41)
				{//�[�d��Ȃ�
					SetOutletDisp(D3DXVECTOR3(pos.x,pos.y + 80.0f,pos.z), nCntModel);
				}
			}
			else
			{//�����蔻�肠��
				g_aModel[nCntModel].bCollision = true;
			}

			//======================================
			//���t�g���菈��
			//======================================
			g_aModel[nCntModel].bStopLift = false;  //���t�g���~�߂�
			g_aModel[nCntModel].bUpDownLift = false;//�t���O�̐؂�ւ��Ń��t�g���㉺������

			//���_���̎擾
			int nNumVtx = g_aModel[nType].pMesh->GetNumVertices();

			//���_�t�H�[�}�b�g�̃T�C�Y���擾
			DWORD sizeFVF = D3DXGetFVFVertexSize(g_aModel[nType].pMesh->GetFVF());

			//���_�o�b�t�@�̃��b�N
			g_aModel[nType].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

			for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
			{
				//���_���W�̑��
				D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

				//���f���̍ŏ��l�ƍő�l���擾
				if (vtx.x < g_aModel[nCntModel].vtxMin.x)
				{//X���W�̍ŏ��l
					g_aModel[nCntModel].vtxMin.x = vtx.x;
				}
				if (vtx.y < g_aModel[nCntModel].vtxMin.y)
				{//Y���W�̍ŏ��l
					g_aModel[nCntModel].vtxMin.y = vtx.y;
				}
				if (vtx.z < g_aModel[nCntModel].vtxMin.z)
				{//Z���W�̍ŏ��l
					g_aModel[nCntModel].vtxMin.z = vtx.z;
				}
				if (vtx.x > g_aModel[nCntModel].vtxMax.x)
				{//X���W�̍ő�l
					g_aModel[nCntModel].vtxMax.x = vtx.x;
				}
				if (vtx.y > g_aModel[nCntModel].vtxMax.y)
				{//Y���W�̍ő�l
					g_aModel[nCntModel].vtxMax.y = vtx.y;
				}
				if (vtx.z > g_aModel[nCntModel].vtxMax.z)
				{//Z���W�̍ő�l
					g_aModel[nCntModel].vtxMax.z = vtx.z;
				}

				//���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
				pVtxBuff += sizeFVF;
			}

			//���_�o�b�t�@�̃A�����b�N
			g_aModel[nType].pMesh->UnlockVertexBuffer();

			break;
		}
	}
}

//=======================================================================================================================================================================================================
// ���f�����̓ǂݍ���(txt)
//=======================================================================================================================================================================================================
void LoadModeltxt(void)
{
	int nCntModel = 0;
	char aDataSearch[MAX_TEXT];

	//�t�@�C�����J��
	FILE* pFile = fopen(MODEL_TXT, "r");

	//�t�@�C�����J���Ȃ��ꍇ
	if (pFile == NULL)
	{
		return;
	}

	//END��������܂œǂݍ��݂��J��Ԃ�
	while (1)
	{
		int nResult = fscanf(pFile, "%s", aDataSearch);	//����

		//EOF�����������ꍇ
		if (nResult == EOF)
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
		if (!strcmp(aDataSearch, "MODELSET"))
		{
			//���ڂ��Ƃ̃f�[�^����
			while (1)
			{
				fscanf(pFile, "%s", aDataSearch);	//����

				//ENDMODEL�Z�b�g�����������ꍇ
				if (!strcmp(aDataSearch, "END_MODELSET"))
				{//���f���f�[�^�̓ǂݍ��݂��I��
					break;
				}

				//�f�[�^�̓ǂݍ���
				else if (!strcmp(aDataSearch, "POSx"))
				{//�ʒu��X���W
					fscanf(pFile, "%f", &g_aModelInfo[nCntModel].pos.x);
				}
				else if (!strcmp(aDataSearch, "POSy"))
				{//�ʒu��Y���W
					fscanf(pFile, "%f", &g_aModelInfo[nCntModel].pos.y);
				}
				else if (!strcmp(aDataSearch, "POSz"))
				{//�ʒu��Z���W
					fscanf(pFile, "%f", &g_aModelInfo[nCntModel].pos.z);
				}
				else if (!strcmp(aDataSearch, "ROTx"))
				{//������X���W
					fscanf(pFile, "%f", &g_aModelInfo[nCntModel].rot.x);
				}
				else if (!strcmp(aDataSearch, "ROTy"))
				{//������Y���W
					fscanf(pFile, "%f", &g_aModelInfo[nCntModel].rot.y);
				}
				else if (!strcmp(aDataSearch, "ROTz"))
				{//������Z���W
					fscanf(pFile, "%f", &g_aModelInfo[nCntModel].rot.z);
				}
				else if (!strcmp(aDataSearch, "TYPE"))
				{//���
					fscanf(pFile, "%d", &g_aModelInfo[nCntModel].nType);
				}
				else if (!strcmp(aDataSearch, "nUSE"))
				{//�g�p�t�B�[���h
					fscanf(pFile, "%d", &g_aModelInfo[nCntModel].nUse);
				}
			}

			nCntModel++;				//�f�[�^�������Z
			g_nCntModel = nCntModel;	//�f�[�^����ۑ�
		}
	}

	for (int nCntSet = 0; nCntSet < g_nCntModel; nCntSet++)
	{
		//==================================================================================================================================================================================
		// �I�u�W�F�N�g�̐ݒ�
		//==================================================================================================================================================================================
		SetModel(g_aModelInfo[nCntSet].pos, g_aModelInfo[nCntSet].rot, g_aModelInfo[nCntSet].nType, g_aModelInfo[nCntSet].nUse);

		//��(���k�p)�̏ꍇ
		if (g_aModelInfo[nCntSet].nType == 0)
		{
			//���̋r��ݒ�
			SetModel(D3DXVECTOR3(g_aModelInfo[nCntSet].pos.x - CREATE_POSX_TYPE1, g_aModelInfo[nCntSet].pos.y, g_aModelInfo[nCntSet].pos.z), g_aModelInfo[nCntSet].rot, 1, g_aModelInfo[nCntSet].nUse);
			SetModel(D3DXVECTOR3(g_aModelInfo[nCntSet].pos.x + CREATE_POSX_TYPE1, g_aModelInfo[nCntSet].pos.y, g_aModelInfo[nCntSet].pos.z), g_aModelInfo[nCntSet].rot, 1, g_aModelInfo[nCntSet].nUse);
		}

		//�֎q(���k�p)�̏ꍇ
		else if (g_aModelInfo[nCntSet].nType == 2)
		{
			//�֎q�̋r��ݒ�
			SetModel(D3DXVECTOR3(g_aModelInfo[nCntSet].pos.x - CREATE_POSX_TYPE3, g_aModelInfo[nCntSet].pos.y, g_aModelInfo[nCntSet].pos.z), g_aModelInfo[nCntSet].rot, 3, g_aModelInfo[nCntSet].nUse);
			SetModel(D3DXVECTOR3(g_aModelInfo[nCntSet].pos.x + CREATE_POSX_TYPE3, g_aModelInfo[nCntSet].pos.y, g_aModelInfo[nCntSet].pos.z), g_aModelInfo[nCntSet].rot, 3, g_aModelInfo[nCntSet].nUse);
		}

		//����̏ꍇ
		else if (g_aModelInfo[nCntSet].nType == 4)
		{
			//����̋r��ݒ�
			SetModel(D3DXVECTOR3(g_aModelInfo[nCntSet].pos.x - CREATE_POSX_TYPE5, g_aModelInfo[nCntSet].pos.y, g_aModelInfo[nCntSet].pos.z), g_aModelInfo[nCntSet].rot, 5, g_aModelInfo[nCntSet].nUse);
			SetModel(D3DXVECTOR3(g_aModelInfo[nCntSet].pos.x + CREATE_POSX_TYPE5, g_aModelInfo[nCntSet].pos.y, g_aModelInfo[nCntSet].pos.z), g_aModelInfo[nCntSet].rot, 5, g_aModelInfo[nCntSet].nUse);
		}

		//�֎q(�搶�p)�̏ꍇ
		else if (g_aModelInfo[nCntSet].nType == 6)
		{
			//�֎q(�搶�p)�̋r��ݒ�
			SetModel(D3DXVECTOR3(g_aModelInfo[nCntSet].pos.x - CREATE_POSX_TYPE7, g_aModelInfo[nCntSet].pos.y, g_aModelInfo[nCntSet].pos.z), g_aModelInfo[nCntSet].rot, 7, g_aModelInfo[nCntSet].nUse);
			SetModel(D3DXVECTOR3(g_aModelInfo[nCntSet].pos.x + CREATE_POSX_TYPE7, g_aModelInfo[nCntSet].pos.y, g_aModelInfo[nCntSet].pos.z), g_aModelInfo[nCntSet].rot, 8, g_aModelInfo[nCntSet].nUse);
		}

		//==================================================================================================================================================================================
		// �G�f�B�^�̔z�u���̐ݒ�
		//==================================================================================================================================================================================
		SetEdit(g_aModelInfo[nCntSet].pos, g_aModelInfo[nCntSet].rot, g_aModelInfo[nCntSet].nType, g_aModelInfo[nCntSet].nUse);
	}
}

//=======================================================================================================================================================================================================================
// ���f�����̓ǂݍ���(bin)
//=======================================================================================================================================================================================================================
void LoadModelbin(void)
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 rot;	//����
	int nType;			//���
	int nUse;			//�g�p�t�B�[���h

	for (int nCntObj = 0; nCntObj < NUM_MODEL; nCntObj++)
	{
		//�t�@�C�����J��
		FILE* pFile = fopen(MODEL_BIN, "rb");

		//�I�u�W�F�N�g���̓ǂݍ���
		if (pFile != NULL)
		{
			//�t�@�C���̓ǂݍ���
			fread(&g_nMaxModel, sizeof(int), 1, pFile);

			for (int nCnt = 1; nCnt < g_nMaxModel; nCnt++)
			{
				//���W�̎擾
				fread(&pos, sizeof(D3DXVECTOR3), 1, pFile);

				//�����̎擾
				fread(&rot, sizeof(D3DXVECTOR3), 1, pFile);

				//�^�C�v�̎擾
				fread(&nType, sizeof(int), 1, pFile);

				//�g�p�t�B�[���h�̎擾
				fread(&nUse, sizeof(int), 1, pFile);

				//�I�u�W�F�N�g�̐ݒ�
				SetModel(pos, rot, nType, nUse);

				//��(���k�p)�̏ꍇ
				if (nType == 0)
				{
					//���̋r��ݒ�
					SetModel(D3DXVECTOR3(pos.x - CREATE_POSX_TYPE1, pos.y, pos.z), rot, 1, nUse);
					SetModel(D3DXVECTOR3(pos.x + CREATE_POSX_TYPE1, pos.y, pos.z), rot, 1, nUse);
				}

				//�֎q(���k�p)�̏ꍇ
				else if (nType == 2)
				{
					//�֎q�̋r��ݒ�
					SetModel(D3DXVECTOR3(pos.x - CREATE_POSX_TYPE3, pos.y, pos.z), rot, 3, nUse);
					SetModel(D3DXVECTOR3(pos.x + CREATE_POSX_TYPE3, pos.y, pos.z), rot, 3, nUse);
				}

				//����̏ꍇ
				else if (nType == 4)
				{
					//����̋r��ݒ�
					SetModel(D3DXVECTOR3(pos.x - CREATE_POSX_TYPE5, pos.y, pos.z), rot, 5, nUse);
					SetModel(D3DXVECTOR3(pos.x + CREATE_POSX_TYPE5, pos.y, pos.z), rot, 5, nUse);
				}

				//�֎q(�搶�p)�̏ꍇ
				else if (nType == 6)
				{
					//�֎q(�搶�p)�̋r��ݒ�
					SetModel(D3DXVECTOR3(pos.x - CREATE_POSX_TYPE7, pos.y, pos.z), rot, 7, nUse);
					SetModel(D3DXVECTOR3(pos.x + CREATE_POSX_TYPE7, pos.y, pos.z), rot, 8, nUse);
				}
			}

			//�t�@�C�������
			fclose(pFile);
		}
		else
		{
			return;
		}
	}
}

//================================================================================================================================================================================================================================
// �����蔻�菈��
//================================================================================================================================================================================================================================
bool bCollisionModel(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 Size, int& nNumLandingModel)
{
	g_bIsLandingModel = false;	//���n���Ă��邩�ǂ���

	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{
		if (g_aModel[nCntModel].bUse && g_aModel[nCntModel].bCollision)
		{//���f�����g�p���Ă��āA�����蔻�肪����ꍇ

			//��
			if (pPos->x + Size.x / 2 >= g_aModel[nCntModel].pos.x + g_aModel[nCntModel].vtxMin.x
				&& pPos->x - Size.x / 2 <= g_aModel[nCntModel].pos.x + g_aModel[nCntModel].vtxMax.x
				&& pPos->y <= g_aModel[nCntModel].pos.y + g_aModel[nCntModel].vtxMax.y
				&& pPosOld->y + 5.0f > g_aModel[nCntModel].pos.y + g_aModel[nCntModel].vtxMax.y
				&& pPos->z + Size.z / 2 >= g_aModel[nCntModel].pos.z + g_aModel[nCntModel].vtxMin.z
				&& pPos->z - Size.z / 2 <= g_aModel[nCntModel].pos.z + g_aModel[nCntModel].vtxMax.z)
			{//�Ώۂ̉��[�����f����+Y�ɓ����������̏���
				if (g_aModel[nCntModel].nType == 18 || g_aModel[nCntModel].nType == 55)
				{
					g_aModel[nCntModel].vtxMax.y = 1.1f;
				}
				pPos->y = g_aModel[nCntModel].pos.y + g_aModel[nCntModel].vtxMax.y;
				g_bIsLandingModel = true;
				nNumLandingModel = nCntModel;//������Ă��郂�f���̔ԍ��������ɕԂ�
			}

			//��
			else if (pPos->x + Size.x / 2 >= g_aModel[nCntModel].pos.x + g_aModel[nCntModel].vtxMin.x
				&& pPos->x - Size.x / 2 <= g_aModel[nCntModel].pos.x + g_aModel[nCntModel].vtxMax.x
				&& pPos->y + Size.y >= g_aModel[nCntModel].pos.y + g_aModel[nCntModel].vtxMin.y
				&& pPosOld->y + Size.y < g_aModel[nCntModel].pos.y + g_aModel[nCntModel].vtxMin.y
				&& pPos->z + Size.z / 2 >= g_aModel[nCntModel].pos.z + g_aModel[nCntModel].vtxMin.z
				&& pPos->z - Size.z / 2 <= g_aModel[nCntModel].pos.z + g_aModel[nCntModel].vtxMax.z)
			{//�Ώۂ̉��[�����f����+Y�ɓ����������̏���
				pPos->y = g_aModel[nCntModel].pos.y + g_aModel[nCntModel].vtxMin.y - Size.y;
			}

			//��
			else if (pPos->x + Size.x / 2 > g_aModel[nCntModel].pos.x + g_aModel[nCntModel].vtxMin.x//�Ώۂ̍ő�X���W���A���f���̍ŏ�X���W����
				&& pPosOld->x + Size.x / 2 <= g_aModel[nCntModel].pos.x + g_aModel[nCntModel].vtxMin.x//�Ώۂ�1F�O�̍ő�X���W���A���f���̍ŏ�X���W��艺
				&& pPos->y + Size.y >= g_aModel[nCntModel].pos.y + g_aModel[nCntModel].vtxMin.y//�Ώۂ̍ő�Y���W�����f���̍ŏ�Y���W����
				&& pPos->y <= g_aModel[nCntModel].pos.y + g_aModel[nCntModel].vtxMax.y
				&& pPos->z + Size.z / 2 >= g_aModel[nCntModel].pos.z + g_aModel[nCntModel].vtxMin.z//�Ώۂ̍ő�X���W���A���f���̍ŏ�X���W����
				&& pPos->z - Size.z / 2 <= g_aModel[nCntModel].pos.z + g_aModel[nCntModel].vtxMax.z)//�Ώۂ�1F�O�̍ő�X���W���A���f���̍ŏ�X���W��艺)//�Ώۂ̍ŏ�Y���W�����f���̍ő�Y���W��艺
			{//�Ώۂ��u���b�N��-X�ɓ����������̏���
				pPos->x = g_aModel[nCntModel].pos.x + g_aModel[nCntModel].vtxMin.x - Size.x / 2;

				if (g_aModel[nCntModel].nType == 18 || g_aModel[nCntModel].nType == 55)
				{//�����������f�����u���t�g�v��������
					pPos->y = g_aModel[nCntModel].pos.y + g_aModel[nCntModel].vtxMax.y + 1.0f;//���t�g�̏�ɏ��悤�ɂ���
					nNumLandingModel = nCntModel;
				}
			}

			//�E
			else if (pPos->x - Size.x / 2 < g_aModel[nCntModel].pos.x + g_aModel[nCntModel].vtxMax.x
				&& pPosOld->x - Size.x / 2 >= g_aModel[nCntModel].pos.x + g_aModel[nCntModel].vtxMax.x
				&& pPos->y + Size.y >= g_aModel[nCntModel].pos.y + g_aModel[nCntModel].vtxMin.y
				&& pPos->y <= g_aModel[nCntModel].pos.y + g_aModel[nCntModel].vtxMax.y
				&& pPos->z + Size.z / 2 >= g_aModel[nCntModel].pos.z + g_aModel[nCntModel].vtxMin.z
				&& pPos->z - Size.z / 2 <= g_aModel[nCntModel].pos.z + g_aModel[nCntModel].vtxMax.z)
			{//�Ώۂ��u���b�N��+X�ɓ����������̏���
				pPos->x = g_aModel[nCntModel].pos.x + g_aModel[nCntModel].vtxMax.x + Size.x / 2;

				if (g_aModel[nCntModel].nType == 18 || g_aModel[nCntModel].nType == 55)
				{//�����������f�����u���t�g�v��������
					pPos->y = g_aModel[nCntModel].pos.y + g_aModel[nCntModel].vtxMax.y + 1.0f;//���t�g�̏�ɏ��悤�ɂ���
					nNumLandingModel = nCntModel;
				}
			}

			//�O
			else if (pPos->x + Size.x / 2 >= g_aModel[nCntModel].pos.x + g_aModel[nCntModel].vtxMin.x
				&& pPos->x - Size.x / 2 <= g_aModel[nCntModel].pos.x + g_aModel[nCntModel].vtxMax.x
				&& pPos->y + Size.y >= g_aModel[nCntModel].pos.y + g_aModel[nCntModel].vtxMin.y
				&& pPos->y <= g_aModel[nCntModel].pos.y + g_aModel[nCntModel].vtxMax.y
				&& pPos->z - Size.z / 2 < g_aModel[nCntModel].pos.z + g_aModel[nCntModel].vtxMax.z
				&& pPosOld->z - Size.z / 2 >= g_aModel[nCntModel].pos.z + g_aModel[nCntModel].vtxMax.z)
			{//�Ώۂ��u���b�N��+X�ɓ����������̏���
				pPos->z = g_aModel[nCntModel].pos.z + g_aModel[nCntModel].vtxMax.z + Size.z / 2;

				if (g_aModel[nCntModel].nType == 18 || g_aModel[nCntModel].nType == 55)
				{//�����������f�����u���t�g�v��������
					pPos->y = g_aModel[nCntModel].pos.y + g_aModel[nCntModel].vtxMax.y + 1.0f;//���t�g�̏�ɏ��悤�ɂ���
					nNumLandingModel = nCntModel;
				}
			}

			//���
			else if (pPos->x + Size.x / 2 >= g_aModel[nCntModel].pos.x + g_aModel[nCntModel].vtxMin.x
				&& pPos->x - Size.x / 2 <= g_aModel[nCntModel].pos.x + g_aModel[nCntModel].vtxMax.x
				&& pPos->y + Size.y >= g_aModel[nCntModel].pos.y + g_aModel[nCntModel].vtxMin.y
				&& pPos->y <= g_aModel[nCntModel].pos.y + g_aModel[nCntModel].vtxMax.y
				&& pPos->z + Size.z / 2 > g_aModel[nCntModel].pos.z + g_aModel[nCntModel].vtxMin.z
				&& pPosOld->z + Size.z / 2 <= g_aModel[nCntModel].pos.z + g_aModel[nCntModel].vtxMin.z)
			{//�Ώۂ��u���b�N��+X�ɓ����������̏���
				pPos->z = g_aModel[nCntModel].pos.z + g_aModel[nCntModel].vtxMin.z - Size.z / 2;

				if (g_aModel[nCntModel].nType == 18 || g_aModel[nCntModel].nType == 55)
				{//�����������f�����u���t�g�v��������
					pPos->y = g_aModel[nCntModel].pos.y + g_aModel[nCntModel].vtxMax.y + 1.0f;//���t�g�̏�ɏ��悤�ɂ���
					nNumLandingModel = nCntModel;
				}
			}
		}
	}

	return g_bIsLandingModel;
}

//================================================================================================================================================================================================================================
// ���̎擾
//================================================================================================================================================================================================================================
Model* GetModel(void)
{
	return &g_aModel[0];
}
