//=======================================================================================================================================================================================================================
//
// �G�f�B�^�̏���
// Author : Atsumu Kuboichi
//
//=======================================================================================================================================================================================================================
#include "edit.h"
#include <stdio.h>
#include "input.h"
#include "debugproc.h"
#include "game.h"
#include "editcamera.h"

//=======================================================================================================================================================================================================================
// �O���[�o���ϐ�
//=======================================================================================================================================================================================================================
Edit g_edit[MAX_MODEL];									//�z�u�I�u�W�F�N�g�̏��
LPD3DXMESH g_EditMesh[NUM_EDITTYPE][NUM_CLASS];			//���b�V����(���_���)�ւ̃|�C���^
LPD3DXBUFFER  g_EditBuffMat[NUM_EDITTYPE][NUM_CLASS];	//�}�e���A���̃|�C���^
DWORD  g_EditdwNumMat[NUM_EDITTYPE][NUM_CLASS];			//�}�e���A���̐�
int g_nCntEdit;	//�z�u���̃��f���̔ԍ�
int g_nEditUse;	//���݂̉��

//=======================================================================================================================================================================================================================
// ����������
//=======================================================================================================================================================================================================================
void InitEdit(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̎擾
	D3DXMATERIAL* pMat;						//�}�e���A���̃f�[�^�ւ̃|�C���^

	//���
	for (int nCntEdit = 0; nCntEdit < MAX_MODEL; nCntEdit++)
	{
		g_edit[nCntEdit].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�ʒu
		g_edit[nCntEdit].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//����
		g_edit[nCntEdit].nType = 0;								//���
		g_edit[nCntEdit].nClass = 0;							//�z�u���̎��
		g_edit[nCntEdit].nUse = -1;								//�g�p�t�B�[���h
		g_edit[nCntEdit].bBuild = false;						//�z�u�ς݂��ǂ���

		//�ŏ������g�p����
		if (nCntEdit == 0)
		{//�g�p����
			g_edit[nCntEdit].bUse = true;
		}
		else
		{//�g�p���Ȃ�
			g_edit[nCntEdit].bUse = false;
		}
	}

	//=====================================================================================================
	// X�t�@�C���̓ǂݍ���
	//=====================================================================================================
	for (int nCntClass = 0; nCntClass < NUM_CLASS; nCntClass++)
	{
		//���f��
		if (nCntClass == 0)
		{
			for (int nCntModel = 0; nCntModel < NUM_MODEL; nCntModel++)
			{
				//X�t�@�C���̓ǂݍ���
				D3DXLoadMeshFromX(MODEL_FILENAME[nCntModel], D3DXMESH_SYSTEMMEM, pDevice, NULL,
					&g_EditBuffMat[nCntModel][nCntClass], NULL, &g_EditdwNumMat[nCntModel][nCntClass], &g_EditMesh[nCntModel][nCntClass]);

				//�}�e���A���f�[�^�ւ̃|�C���^���擾
				pMat = (D3DXMATERIAL*)g_EditBuffMat[nCntModel][nCntClass]->GetBufferPointer();

				//�}�e���A���f�[�^��ۑ�
				for (int nCntMat = 0; nCntMat < (int)g_EditdwNumMat[nCntModel][nCntClass]; nCntMat++)
				{
					g_edit[nCntModel].Diffuse[nCntMat] = pMat[nCntMat].MatD3D.Diffuse;
				}
			}
		}
	}

	//�ϐ�
	g_nCntEdit = 0;//�z�u���̃��f���̔ԍ�
	g_nEditUse = 0;//���݂̉��
}

//=======================================================================================================================================================================================================================
// �I������
//=======================================================================================================================================================================================================================
void UninitEdit(void)
{
	for (int nCntClass = 0; nCntClass < NUM_CLASS; nCntClass++)
	{
		for (int nCntEdit = 0; nCntEdit < NUM_EDITTYPE; nCntEdit++)
		{
			//���b�V���̔j��
			if (g_EditMesh[nCntEdit][nCntClass] != NULL)
			{
				g_EditMesh[nCntEdit][nCntClass]->Release();
				g_EditMesh[nCntEdit][nCntClass] = NULL;
			}
			//�}�e���A���̔j��
			if (g_EditBuffMat[nCntEdit][nCntClass] != NULL)
			{
				g_EditBuffMat[nCntEdit][nCntClass]->Release();
				g_EditBuffMat[nCntEdit][nCntClass] = NULL;
			}
		}
	}
}

//=======================================================================================================================================================================================================================
// �X�V����
//=======================================================================================================================================================================================================================
void UpdateEdit(void)
{
	//������@�\��
	PrintDebugProc("==============================\n ���� \n==============================\n");
	PrintDebugProc("�ړ�\nX���ړ�:T,G�@Y���ړ�:Y,H�@Z���ړ�:U,J\n\n");
	PrintDebugProc("%f���ړ�\nX��:1,2�@Y��:3,4�@Z��:5,6\n\n", EDIT_SPEED);
	PrintDebugProc("��]\n����]:Z�@�E��]:C\n\n");
	PrintDebugProc("�ʒu�ƌ��������Z�b�g:R\n\n");
	PrintDebugProc("�J����\n�ړ�:WASD�@��]:��������\n\n");
	PrintDebugProc("��ނ�ύX:Q,E\n");
	PrintDebugProc("�z�u:B\n");
	PrintDebugProc("�ҏW�ΏەύX:N,M\n");
	PrintDebugProc("�X�e�[�W�؂�ւ�:F6\n");
	PrintDebugProc("�폜:BACK SPACE\n");
	PrintDebugProc("�S���Z�b�g:0\n");
	PrintDebugProc("�����o��:F3\n\n");

	//�z�u�����\��
	PrintDebugProc("==============================\n �z�u����� \n==============================\n");
	PrintDebugProc("POS\nX:%f�@Y:%f�@Z:%f\n\n", g_edit[g_nCntEdit].pos.x, g_edit[g_nCntEdit].pos.y, g_edit[g_nCntEdit].pos.z);	//�ʒu
	PrintDebugProc("ROT\nX:%f�@Y:%f  Z:%f\n\n", g_edit[g_nCntEdit].rot.x, g_edit[g_nCntEdit].rot.y, g_edit[g_nCntEdit].rot.z);	//����
	PrintDebugProc("TYPE:%d\n\n", g_edit[g_nCntEdit].nType);																	//���
	PrintDebugProc("�z�u��:%d/%d\n", g_nCntEdit, MAX_MODEL);																	//�z�u��

	if (g_edit[g_nCntEdit].bUse)
	{//�z�u�����g�p����Ă���ꍇ
		//=======================================================================
		// �ړ�(�v���X)
		//======================================================================= 
		if (GetKeyboardPress(DIK_T)) //T
		{
			g_edit[g_nCntEdit].pos.x -= EDIT_SPEED;
		}
		else if (GetKeyboardPress(DIK_G)) //G
		{
			g_edit[g_nCntEdit].pos.x += EDIT_SPEED;
		}
		else if (GetKeyboardPress(DIK_Y)) //Y
		{
			g_edit[g_nCntEdit].pos.y += EDIT_SPEED;
		}
		else if (GetKeyboardPress(DIK_H)) //H
		{
			g_edit[g_nCntEdit].pos.y -= EDIT_SPEED;
		}
		else if (GetKeyboardPress(DIK_U)) //U
		{
			g_edit[g_nCntEdit].pos.z += EDIT_SPEED;
		}
		else if (GetKeyboardPress(DIK_J)) //J
		{
			g_edit[g_nCntEdit].pos.z -= EDIT_SPEED;
		}

		//=======================================================================
		// �ړ�(�g���K�[)
		//=======================================================================
		if (GetKeyboardTrigger(DIK_1)) //1
		{
			g_edit[g_nCntEdit].pos.x -= EDIT_SPEED;
		}
		else if (GetKeyboardTrigger(DIK_2)) //2
		{
			g_edit[g_nCntEdit].pos.x += EDIT_SPEED;
		}
		else if (GetKeyboardTrigger(DIK_3)) //3
		{
			g_edit[g_nCntEdit].pos.y += EDIT_SPEED;
		}
		else if (GetKeyboardTrigger(DIK_4)) //4
		{
			g_edit[g_nCntEdit].pos.y -= EDIT_SPEED;
		}
		else if (GetKeyboardTrigger(DIK_5)) //5
		{
			g_edit[g_nCntEdit].pos.z += EDIT_SPEED;
		}
		else if (GetKeyboardTrigger(DIK_6)) //6
		{
			g_edit[g_nCntEdit].pos.z -= EDIT_SPEED;
		}

		//=======================================================================
		// ��]
		//=======================================================================
		if (GetKeyboardTrigger(DIK_C))
		{//C�L�[�������ꂽ�ꍇ
			g_edit[g_nCntEdit].rot.y -= EDIT_TURN;//����]

			//�p�x�̕␳
			if (g_edit[g_nCntEdit].rot.y <= -D3DX_PI)
			{
				g_edit[g_nCntEdit].rot.y = D3DX_PI;
			}
		}
		else if (GetKeyboardTrigger(DIK_V))
		{//V�L�[�������ꂽ�ꍇ
			g_edit[g_nCntEdit].rot.y += EDIT_TURN;//�E��]

			//�p�x�̕␳
			if (g_edit[g_nCntEdit].rot.y >= D3DX_PI)
			{
				g_edit[g_nCntEdit].rot.y = -D3DX_PI;
			}
		}

		//=======================================================================
		// �ʒu�ƌ��������Z�b�g
		//=======================================================================
		if (GetKeyboardTrigger(DIK_R))
		{//R�L�[�������ꂽ�ꍇ
			g_edit[g_nCntEdit].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_edit[g_nCntEdit].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}

		//=======================================================================
		// ��ޕύX
		//=======================================================================
		if (GetKeyboardTrigger(DIK_Q))
		{//Q�L�[�������ꂽ�ꍇ
			if (g_edit[g_nCntEdit].nType > 0)
			{
				g_edit[g_nCntEdit].nType--;
			}
		}
		else if (GetKeyboardTrigger(DIK_E))
		{//E�L�[�������ꂽ�ꍇ
			if (g_edit[g_nCntEdit].nType < NUM_EDITTYPE - 1)
			{
				g_edit[g_nCntEdit].nType++;
			}
		}

		//=======================================================================
		// �z�u
		//=======================================================================
		if (GetKeyboardTrigger(DIK_B))
		{//B�L�[�������ꂽ�ꍇ
			g_edit[g_nCntEdit].bBuild = true;						//�z�u����
			g_edit[g_nCntEdit + 1].bUse = true;						//���̃f�[�^���g�p����
			g_edit[g_nCntEdit + 1].pos = g_edit[g_nCntEdit].pos;	//���̃f�[�^�Ɍ��݂̃f�[�^�̈ʒu�𔽉f
			g_edit[g_nCntEdit + 1].rot = g_edit[g_nCntEdit].rot;	//���̃f�[�^�Ɍ��݂̃f�[�^�̈ʒu�𔽉f
			g_edit[g_nCntEdit + 1].nType = g_edit[g_nCntEdit].nType;//���̃f�[�^�Ɍ��݂̃f�[�^�̃^�C�v�𔽉f

			g_nCntEdit++;	//�z�u�������Z
		}
	}

	//==================================================
	// �ҏW�ΏەύX
	//==================================================
	if (GetKeyboardTrigger(DIK_N))
	{//N�L�[�������ꂽ�ꍇ
		while (1)
		{
			g_nCntEdit--;//���̃I�u�W�F�N�g��Ώۂɂ���

			if (g_nCntEdit < 0)
			{//�J�E���g���I�u�W�F�N�g�̍ő吔�ɓ��B�����ꍇ
				g_nCntEdit = MAX_MODEL;
			}

			if (g_edit[g_nCntEdit].bUse)
			{//���̃I�u�W�F�N�g���g�p����Ă���ꍇ
				break;
			}
		}

		SetPosEditCamera(g_nCntEdit);//�J�����̈ʒu��Ώۂ̃I�u�W�F�N�g�ɍ��킹�ĕύX
	}
	else if (GetKeyboardTrigger(DIK_M))
	{//M�L�[�������ꂽ�ꍇ
		while (1)
		{
			g_nCntEdit++;//���̃I�u�W�F�N�g��Ώۂɂ���

			if (g_nCntEdit > MAX_MODEL)
			{//�J�E���g���I�u�W�F�N�g�̍ő吔�ɓ��B�����ꍇ
				g_nCntEdit = 0;
			}

			if (g_edit[g_nCntEdit].bUse)
			{//���̃I�u�W�F�N�g���g�p����Ă���ꍇ
				break;
			}
		}

		//�J�����̈ʒu��Ώۂ̃I�u�W�F�N�g�ɍ��킹�ĕύX
		SetPosEditCamera(g_nCntEdit);
	}

	//=======================================================================
	// �폜
	//=======================================================================
	if (GetKeyboardTrigger(DIK_BACK))
	{//BACK SPACE�������ꂽ�ꍇ
		g_edit[g_nCntEdit].bUse = false;	//���݂̃f�[�^���g�p���Ȃ�
		g_edit[g_nCntEdit].bBuild = false;	//�z�u����Ă��Ȃ�
		g_nCntEdit--;

		//�J�����̈ʒu��Ώۂ̃I�u�W�F�N�g�ɍ��킹�ĕύX
		SetPosEditCamera(g_nCntEdit);
	}

	//=======================================================================
	// �S�f�[�^���Z�b�g
	//=======================================================================
	if (GetKeyboardTrigger(DIK_0))
	{//0�L�[�������ꂽ�ꍇ
		//�S��񏉊���
		for (int nCntEdit = 0; nCntEdit < MAX_MODEL; nCntEdit++)
		{
			g_edit[nCntEdit].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�ʒu
			g_edit[nCntEdit].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//����
			g_edit[nCntEdit].nType = 0;								//���
			g_edit[nCntEdit].nClass = 0;							//�z�u���̎��
			g_edit[nCntEdit].nUse = -1;								//�g�p�t�B�[���h
			g_edit[nCntEdit].bBuild = false;						//�z�u�ς݂��ǂ���

			//�ŏ������g�p����
			if (nCntEdit == 0)
			{//�g�p����
				g_edit[nCntEdit].bUse = true;
			}
			else
			{//�g�p���Ȃ�
				g_edit[nCntEdit].bUse = false;
			}
		}

		g_nCntEdit = 0;//�z�u����0�ɏ�����
	}

	//=======================================================================
	// �Q�[��or�`���[�g���A���̔���
	//=======================================================================
	if (!GetHouse())
	{//�Q�[��
		g_edit[g_nCntEdit].nUse = 0;
		g_nEditUse = 0;
	}
	else
	{//�`���[�g���A��
		g_edit[g_nCntEdit].nUse = 1;
		g_nEditUse = 1;
	}

	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{
		if (g_edit[nCntModel].bBuild)
		{
			//�g�p�X�e�[�W����v����ꍇ
			if (g_edit[nCntModel].nUse == g_nEditUse)
			{//�g�p����
				g_edit[nCntModel].bUse = true;
			}
			//��v���Ȃ��ꍇ
			else
			{//�g�p���Ȃ�
				g_edit[nCntModel].bUse = false;
			}
		}
	}

	//=======================================================================
	// ���̏����o��
	//=======================================================================
	WriteModel();//�����o������
}

//=======================================================================================================================================================================================================================
// �`�揈��
//=======================================================================================================================================================================================================================
void DrawEdit(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans;			//�v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;					//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL* pMat;						//�}�e���A���̃f�[�^�ւ̃|�C���^

	for (int nCntEdit = 0; nCntEdit < MAX_MODEL; nCntEdit++)
	{
		//�g�p����Ă���ꍇ
		if (g_edit[nCntEdit].bUse)
		{
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_edit[nCntEdit].mtxWorld);

			//�����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_edit[nCntEdit].rot.y, g_edit[nCntEdit].rot.x, g_edit[nCntEdit].rot.z);
			D3DXMatrixMultiply(&g_edit[nCntEdit].mtxWorld, &g_edit[nCntEdit].mtxWorld, &mtxRot);

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_edit[nCntEdit].pos.x, g_edit[nCntEdit].pos.y, g_edit[nCntEdit].pos.z);
			D3DXMatrixMultiply(&g_edit[nCntEdit].mtxWorld, &g_edit[nCntEdit].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_edit[nCntEdit].mtxWorld);

			//���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			//�}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_EditBuffMat[g_edit[nCntEdit].nType][g_edit[nCntEdit].nClass]->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_EditdwNumMat[g_edit[nCntEdit].nType][g_edit[nCntEdit].nClass]; nCntMat++)
			{
				//�ҏW�Ώۂ̃I�u�W�F�N�g�̏ꍇ
				if (nCntEdit == g_nCntEdit)
				{
					//���f���̐F��ω�
					pMat[nCntMat].MatD3D.Diffuse.a = 0.5f;
				}
				//�ҏW�Ώۂ̃I�u�W�F�N�g�ł͂Ȃ��ꍇ
				else if(nCntEdit != g_nCntEdit)
				{
					//���f���̐F��߂�
					pMat[nCntMat].MatD3D.Diffuse.a = 1.0f;
				}

				//�}�e���A���̐ݒ�
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//�e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, NULL);

				//���f��(�p�[�c)�̕`��
				g_EditMesh[g_edit[nCntEdit].nType][g_edit[nCntEdit].nClass]->DrawSubset(nCntMat);
			}

			//�ۑ����Ă����}�e���A����߂�
			pDevice->SetMaterial(&matDef);
		}
	}
}

//=======================================================================================================================================================================================================================
// ���f�����̏����o������
//=======================================================================================================================================================================================================================
void WriteModel(void)
{
	FILE* pFile{};
	int nCntModel = 0;

	//====================================================================================================
	// ���������o��
	//====================================================================================================
	if (GetKeyboardTrigger(DIK_F3))	//F3�L�[�������ꂽ�ꍇ
	{
		//================================================================================
		// txt�ւ̏����o��
		//================================================================================

		//�t�@�C�����J��
		pFile = fopen(MODEL_TXT, "w");

		if (pFile != NULL)
		{
			//�t�@�C���̒��g�����Z�b�g
			for (int nCnt = 0; nCnt < NUM_MODEL; nCnt++)
			{
				fprintf(pFile, "# TYPE %d : %s\n", nCnt, MODEL_FILENAME[nCnt]);
			}
			fprintf(pFile, "\n");

			for (int nCnt = 0; nCnt < MAX_MODEL; nCnt++)
			{
				//�g�p����Ă���ꍇ
				if (g_edit[nCnt].bBuild)
				{
					//�f�[�^�̋L�^
					fprintf(pFile, "# model%d\n", nCntModel);//�f�[�^�̔ԍ�
					fprintf(pFile, "MODELSET\n");
					fprintf(pFile, "	POSx %f\n", g_edit[nCnt].pos.x);//X���W
					fprintf(pFile, "	POSy %f\n", g_edit[nCnt].pos.y);//Y���W
					fprintf(pFile, "	POSz %f\n", g_edit[nCnt].pos.z);//Z���W
					fprintf(pFile, "	ROTx %f\n", g_edit[nCnt].rot.x);//X���̌���
					fprintf(pFile, "	ROTy %f\n", g_edit[nCnt].rot.y);//Y���̌���
					fprintf(pFile, "	ROTz %f\n", g_edit[nCnt].rot.z);//Z���̌���
					fprintf(pFile, "	TYPE %d\n", g_edit[nCnt].nType);//���
					fprintf(pFile, "	nUSE %d\n", g_edit[nCnt].nUse);	//�g�p�t�B�[���h
					fprintf(pFile, "END_MODELSET\n\n");

					nCntModel++;
				}
			}

			//�t�@�C�������
			fclose(pFile);
		}

		//================================================================================
		// bin�ւ̏����o��
		//================================================================================

		//�t�@�C�����J��
		pFile = fopen(MODEL_BIN, "wb");

		if (pFile != NULL)
		{
			//�z�u���������o��
			fwrite(&g_nCntEdit, sizeof(int), 1, pFile);

			for (int nCnt = 0; nCnt < MAX_MODEL; nCnt++)
			{
				//�g�p����Ă���ꍇ
				if (g_edit[nCnt].bBuild)
				{
					//�f�[�^�̋L�^
					fwrite(&g_edit[nCnt].pos, sizeof(D3DXVECTOR3), 1, pFile);	//�ʒu
					fwrite(&g_edit[nCnt].rot, sizeof(D3DXVECTOR3), 1, pFile);	//����
					fwrite(&g_edit[nCnt].nType, sizeof(int), 1, pFile);			//���
					fwrite(&g_edit[nCnt].nUse, sizeof(int), 1, pFile);			//�g�p�t�B�[���h
				}
			}

			//�t�@�C�������
			fclose(pFile);
		}
	}
}

//================================================================================================================================================================================================================================
// �ݒ菈��
//================================================================================================================================================================================================================================
void SetEdit(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nUse)
{
	//�ǂݍ��񂾃f�[�^��ݒ�
	g_edit[g_nCntEdit].pos = pos;		//�ʒu
	g_edit[g_nCntEdit].rot = rot;		//����
	g_edit[g_nCntEdit].nType = nType;	//���
	g_edit[g_nCntEdit].nUse = nUse;		//�g�p�t�B�[���h
	g_edit[g_nCntEdit].bUse = true;		//�g�p����
	g_edit[g_nCntEdit].bBuild = true;	//�z�u�ς�

	//���̃f�[�^�����������ėL���ɂ���
	g_edit[g_nCntEdit + 1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�ʒu
	g_edit[g_nCntEdit + 1].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//����
	g_edit[g_nCntEdit + 1].nType = 0;	//���
	g_edit[g_nCntEdit + 1].nUse = -1;	//�g�p�t�B�[���h
	g_edit[g_nCntEdit + 1].bUse = true;	//�g�p����

	g_nCntEdit++;//�ǂݍ��ݐ������Z
}

//================================================================================================================================================================================================================================
// �\���̂̏��̎擾
//================================================================================================================================================================================================================================
Edit* GetEdit(void)
{
	return &g_edit[0];
}
