//=======================================================================================================================================================================================================================
//
// �ǂ̏���
// Author : Atsumu Kuboichi
//
//=======================================================================================================================================================================================================================
#include <stdio.h>
#include <string.h>
#include "wall.h"
#include "main.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "field.h"
#include "game.h"

//=======================================================================================================================================================================================================================
// �O���[�o���ϐ�
//=======================================================================================================================================================================================================================
PDIRECT3DVERTEXBUFFER9 g_pVtxBuffWall = NULL;		//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureWall[NUM_WALL] = {};	//�e�N�X�`���ւ̃|�C���^
D3DXMATRIX g_mtxWorldWall;							//���[���h�}�g���b�N�X
Wall g_aWall[MAX_WALL];								//�ǂ̏��

//=======================================================================================================================================================================================================================
// ����������
//=======================================================================================================================================================================================================================
void InitWall(void)
{
	VERTEX_3D* pVtx;						//���_���ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̎擾

	//�e�N�X�`���̓ǂݍ���
	for (int nCntTex = 0; nCntTex < NUM_WALL; nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice,
			WALL_FILENAME[nCntTex],
			&g_pTextureWall[nCntTex]);
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_WALL,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffWall,
		NULL);

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		//�ϐ�
		g_aWall[nCntWall].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aWall[nCntWall].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aWall[nCntWall].fWidth = 0.0f;
		g_aWall[nCntWall].fHeight = 0.0f;
		g_aWall[nCntWall].bUse = true;

		//���_���W�̐ݒ�i�����ʒu�j
		pVtx[0].pos = D3DXVECTOR3(0.0f,0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//nor�̐ݒ�i�����ݒ�j
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//�e�N�X�`�����W�̐ݒ�i�����ݒ�j
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffWall->Unlock();
}

//=======================================================================================================================================================================================================================
// �I������
//=======================================================================================================================================================================================================================
void UninitWall(void)
{
	//�e�N�X�`���̔j��
	for (int nCntTex = 0; nCntTex < NUM_WALL; nCntTex++)
	{
		if (g_pTextureWall[nCntTex] != NULL)
		{//�e�N�X�`����񂪑��݂���ꍇ
			g_pTextureWall[nCntTex]->Release();
			g_pTextureWall[nCntTex] = NULL;
		}
	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffWall != NULL)
	{
		g_pVtxBuffWall->Release();
		g_pVtxBuffWall = NULL;
	}
}

//=======================================================================================================================================================================================================================
// �X�V����
//=======================================================================================================================================================================================================================
void UpdateWall(void)
{
	VERTEX_3D* pVtx;			//���_���ւ̃|�C���^
	Field* pField = GetField();	//�t�B�[���h�̏��̍\���̂��擾

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);

	//�ʒu
	g_aWall[0].pos = D3DXVECTOR3(pField->pos.x, pField->pos.y, pField->pos.z + pField->fHeight);
	g_aWall[1].pos = D3DXVECTOR3(pField->pos.x + pField->fWidth, pField->pos.y, pField->pos.z);
	g_aWall[2].pos = D3DXVECTOR3(pField->pos.x, pField->pos.y, pField->pos.z - pField->fHeight);
	g_aWall[3].pos = D3DXVECTOR3(pField->pos.x - pField->fWidth, pField->pos.y, pField->pos.z);

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		//����
		g_aWall[nCntWall].rot = D3DXVECTOR3(0.0f, nCntWall * (D3DX_PI / 2.0f), 0.0f);

		if (g_aWall[nCntWall].bUse == true)
		{//�g�p����Ă���ꍇ
			if (!GetEditMode())
			{//�Q�[����
				if (GetMode() != MODE_TUTORIAL)
				{//�`���[�g���A����ʈȊO�̏ꍇ
					g_aWall[nCntWall].nType = 1;
				}
				else
				{//�`���[�g���A����ʂ̏ꍇ
					g_aWall[nCntWall].nType = 0;
				}
			}
			else
			{//�G�f�B�^��
				if (!GetHouse())
				{//�`���[�g���A����ʂ̏ꍇ
					g_aWall[nCntWall].nType = 1;
				}
				else
				{//�Q�[����ʂ̏ꍇ
					g_aWall[nCntWall].nType = 0;
				}
			}

			//���_���W�̐ݒ�
			if (nCntWall % 2 == 0)
			{//�O��̕�
				pVtx[0].pos = D3DXVECTOR3(-pField->fWidth, pField->fHeight, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(pField->fWidth, pField->fHeight, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(-pField->fWidth, 0.0f, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(pField->fWidth, 0.0f, 0.0f);
			}
			else
			{//���E�̕�
				pVtx[0].pos = D3DXVECTOR3(-pField->fHeight, pField->fHeight, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(pField->fHeight, pField->fHeight, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(-pField->fHeight, 0.0f, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(pField->fHeight, 0.0f, 0.0f);
			}
		}

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffWall->Unlock();
}

//=======================================================================================================================================================================================================================
// �`�揈��
//=======================================================================================================================================================================================================================
void DrawWall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans;			//�v�Z�p�}�g���b�N�X

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		if (g_aWall[nCntWall].bUse == true)
		{//�g�p����Ă���ꍇ
	        //���[���h�}�g���b�N�X�̏�����
	        D3DXMatrixIdentity(&g_mtxWorldWall);
	        
	        //�����𔽉f
	        D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aWall[nCntWall].rot.y, g_aWall[nCntWall].rot.x, g_aWall[nCntWall].rot.z);
	        
	        D3DXMatrixMultiply(&g_mtxWorldWall, &g_mtxWorldWall, &mtxRot);
	        
	        //�ʒu�𔽉f
	        D3DXMatrixTranslation(&mtxTrans, g_aWall[nCntWall].pos.x, g_aWall[nCntWall].pos.y, g_aWall[nCntWall].pos.z);
	        D3DXMatrixMultiply(&g_mtxWorldWall, &g_mtxWorldWall, &mtxTrans);
	        
	        //���[���h�}�g���b�N�X�̐ݒ�
	        pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldWall);
	        
	        //���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	        pDevice->SetStreamSource(0, g_pVtxBuffWall, 0, sizeof(VERTEX_3D));
	        
	        //���_�t�H�[�}�b�g�̐ݒ�
	        pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureWall[g_aWall[nCntWall].nType]);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntWall * 4, 2);
		}
	}	
}

//=======================================================================================================================================================================================================================
// ���̍\���̂��擾
//=======================================================================================================================================================================================================================
Wall* GetWall(void)
{
	return &g_aWall[0];
}
