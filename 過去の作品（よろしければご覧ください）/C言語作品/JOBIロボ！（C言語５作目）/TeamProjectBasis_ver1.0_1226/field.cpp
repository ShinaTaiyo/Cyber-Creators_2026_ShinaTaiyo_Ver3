//=======================================================================================================================================================================================================================
//
// �t�B�[���h�̏���
// Author : Atsumu Kuboichi
//
//=======================================================================================================================================================================================================================
#include "field.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "game.h"

//=======================================================================================================================================================================================================================
// �O���[�o���ϐ�
//=======================================================================================================================================================================================================================

//==================
//�t�B�[���h
//==================
Field g_aField;	//���\����

//==================
//���o�t�B�[���h
//==================
PDIRECT3DVERTEXBUFFER9 g_pVtxBuffStagingField = NULL;		        //���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureStagingField[NUM_STAGINGFIELD] = {};	//�e�N�X�`���ւ̃|�C���^
StagingField g_aStagingField[MAX_STAGINGFIELD];                 //���o�t�B�[���h�̍\����
//=======================================================================================================================================================================================================================
// ����������
//=======================================================================================================================================================================================================================
void InitField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̎擾
	VERTEX_3D* pVtx;						//���_���ւ̃|�C���^

	//�e�N�X�`���̓ǂݍ���
	for (int nCntTex = 0; nCntTex < NUM_FIELD; nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice,
			FIELD_FILENAME[nCntTex],
			&g_aField.pTexture[nCntTex]);
	}

	//�ϐ�
	g_aField.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�ʒu
	g_aField.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//����
	g_aField.fWidth = FIELD_WIDTH;					//����
	g_aField.fHeight = FIELD_HEIGHT;				//�c��

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_aField.pVtxBuff,
		NULL);

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_aField.pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-g_aField.fWidth, 0.0f, g_aField.fHeight);
	pVtx[1].pos = D3DXVECTOR3(g_aField.fWidth, 0.0f, g_aField.fHeight);
	pVtx[2].pos = D3DXVECTOR3(-g_aField.fWidth, 0.0f, -g_aField.fHeight);
	pVtx[3].pos = D3DXVECTOR3(g_aField.fWidth, 0.0f, -g_aField.fHeight);

	//nor�̐ݒ�
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	g_aField.pVtxBuff->Unlock();
}

//=======================================================================================================================================================================================================================
// �I������
//=======================================================================================================================================================================================================================
void UninitField(void)
{
	//�e�N�X�`���̔j��
	for (int nCntTex = 0; nCntTex < NUM_FIELD; nCntTex++)
	{
		if (g_aField.pTexture[nCntTex] != NULL)
		{//�e�N�X�`����񂪑��݂���ꍇ
			g_aField.pTexture[nCntTex]->Release();
			g_aField.pTexture[nCntTex] = NULL;
		}
	}
	//���_�o�b�t�@�̔j��
	if (g_aField.pVtxBuff != NULL)
	{//���_��񂪑��݂���ꍇ
		g_aField.pVtxBuff->Release();
		g_aField.pVtxBuff = NULL;
	}
}

//=======================================================================================================================================================================================================================
// �X�V����
//=======================================================================================================================================================================================================================
void UpdateField(void)
{
	VERTEX_3D* pVtx;//���_���ւ̃|�C���^

	//�Q�[����
	if (!GetEditMode())
	{
		//�Q�[���X�e�[�W
		if (GetMode() != MODE_TUTORIAL)
		{
			g_aField.nType = 1;
			g_aField.fWidth = FIELD_WIDTH;	//����
			g_aField.fHeight = FIELD_HEIGHT;//�c��
		}
		//���у��{�n�E�X��
		else
		{
			g_aField.nType = 0;
			g_aField.fWidth = TUTORIAL_SCALE;	//����
			g_aField.fHeight = TUTORIAL_SCALE;	//�c��
		}
	}
	//�G�f�B�^��
	else
	{
		//�Q�[���X�e�[�W
		if (!GetHouse())
		{
			g_aField.nType = 1;
			g_aField.fWidth = FIELD_WIDTH;	//����
			g_aField.fHeight = FIELD_HEIGHT;//�c��
		}
		//���у��{�n�E�X��
		else
		{
			g_aField.nType = 0;
			g_aField.fWidth = TUTORIAL_SCALE;	//����
			g_aField.fHeight = TUTORIAL_SCALE;	//�c��
		}
	}

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_aField.pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̍X�V
	pVtx[0].pos = D3DXVECTOR3(-g_aField.fWidth, 0.0f, g_aField.fHeight);
	pVtx[1].pos = D3DXVECTOR3(g_aField.fWidth, 0.0f, g_aField.fHeight);
	pVtx[2].pos = D3DXVECTOR3(-g_aField.fWidth, 0.0f, -g_aField.fHeight);
	pVtx[3].pos = D3DXVECTOR3(g_aField.fWidth, 0.0f, -g_aField.fHeight);

	//���_�o�b�t�@���A�����b�N����
	g_aField.pVtxBuff->Unlock();
}

//=======================================================================================================================================================================================================================
// �`�揈��
//=======================================================================================================================================================================================================================
void DrawField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans;			//�v�Z�p�}�g���b�N�X

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_aField.mtxworld);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aField.rot.y, g_aField.rot.x, g_aField.rot.z);

	D3DXMatrixMultiply(&g_aField.mtxworld, &g_aField.mtxworld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, g_aField.pos.x, g_aField.pos.y, g_aField.pos.z);
	D3DXMatrixMultiply(&g_aField.mtxworld, &g_aField.mtxworld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_aField.mtxworld);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_aField.pVtxBuff, 0, sizeof(VERTEX_3D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_aField.pTexture[g_aField.nType]);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//=======================================================================================================================================================================================================================
// ���̍\���̂̎擾
//=======================================================================================================================================================================================================================
Field* GetField(void)
{
	return &g_aField;
}

void InitStagingField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̎擾
	VERTEX_3D* pVtx;						//���_���ւ̃|�C���^

	//�e�N�X�`���̓ǂݍ���
	for (int nCntType = 0; nCntType < NUM_STAGINGFIELD; nCntType++)
	{
		D3DXCreateTextureFromFile(pDevice,
		    STAGINGFIELD_FILENAME[nCntType],
			&g_pTextureStagingField[nCntType]);
	}

	//===========================
	//������
	//===========================
	for (int nCntStagingField = 0; nCntStagingField < MAX_STAGINGFIELD; nCntStagingField++)
	{
		g_aStagingField[nCntStagingField].pos = NULL_VECTOR3;//�ʒu
		g_aStagingField[nCntStagingField].mtxworld = {};//�}�g���b�N�X���[���h
		g_aStagingField[nCntStagingField].nType = 0;//���
		g_aStagingField[nCntStagingField].rot = NULL_VECTOR3;//����
		g_aStagingField[nCntStagingField].fWidth = 0.0f;//����
		g_aStagingField[nCntStagingField].fHeight = 0.0f;//����
		g_aStagingField[nCntStagingField].bUse = false;//�g�p���
		g_aStagingField[nCntStagingField].nLife = 0;//�̗�
		g_aStagingField[nCntStagingField].fRot = 0.0f;//����
	}

	//=========================================================================================

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_STAGINGFIELD,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffStagingField,
		NULL);

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffStagingField->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntField = 0; nCntField < MAX_STAGINGFIELD; nCntField++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = NULL_VECTOR3;
		pVtx[1].pos = NULL_VECTOR3;
		pVtx[2].pos = NULL_VECTOR3;
		pVtx[3].pos = NULL_VECTOR3;
		//nor�̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffStagingField->Unlock();
}

void UninitStagingField(void)
{
	//�e�N�X�`���̔j��
	for (int nCntType = 0; nCntType < NUM_STAGINGFIELD; nCntType++)
	{
		if (g_pTextureStagingField[nCntType] != NULL)
		{
			g_pTextureStagingField[nCntType]->Release();
			g_pTextureStagingField[nCntType] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffStagingField != NULL)
	{
		g_pVtxBuffStagingField->Release();
		g_pVtxBuffStagingField = NULL;
	}
}

void UpdateStagingField(void)
{
	for (int nCntField = 0; nCntField < MAX_STAGINGFIELD; nCntField++)
	{
		if (g_aStagingField[nCntField].bUse == true)
		{
			g_aStagingField[nCntField].nLife--;

			//============================
			//���@�w�Ȃ�
			//============================
			if (g_aStagingField[nCntField].nType == STAGINGFIELD00_MAGICCIRCLE)
			{
				g_aStagingField[nCntField].rot.y += g_aStagingField[nCntField].fRot;
			}

			//===========================================================================


			//============================
			//�̗͂��O�ɂȂ�����
			//============================
			if (g_aStagingField[nCntField].nLife <= 0)
			{
				g_aStagingField[nCntField].bUse = false;
			}

			//============================================================================
		}
	}
}

void DrawStagingField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans;			//�v�Z�p�}�g���b�N�X

	for (int nCntField = 0; nCntField < MAX_STAGINGFIELD; nCntField++)
	{
		if (g_aStagingField[nCntField].bUse == true)
		{
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aStagingField[nCntField].mtxworld);

			//�����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aStagingField[nCntField].rot.y, g_aStagingField[nCntField].rot.x, g_aStagingField[nCntField].rot.z);

			D3DXMatrixMultiply(&g_aStagingField[nCntField].mtxworld, &g_aStagingField[nCntField].mtxworld, &mtxRot);

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aStagingField[nCntField].pos.x, g_aStagingField[nCntField].pos.y, g_aStagingField[nCntField].pos.z);
			D3DXMatrixMultiply(&g_aStagingField[nCntField].mtxworld, &g_aStagingField[nCntField].mtxworld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aStagingField[nCntField].mtxworld);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffStagingField, 0, sizeof(VERTEX_3D));

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureStagingField[g_aStagingField[nCntField].nType]);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,nCntField * 4, 2);
		}
	}
}

void SetStagingField(int nType, int nLife, float fWidth, float fHeight, float fRot, D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	VERTEX_3D* pVtx;
	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffStagingField->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntField = 0; nCntField < MAX_STAGINGFIELD; nCntField++)
	{
		if (g_aStagingField[nCntField].bUse == false)
		{
			g_aStagingField[nCntField].pos = pos;//�ʒu
			g_aStagingField[nCntField].mtxworld = {};//�}�g���b�N�X���[���h
			g_aStagingField[nCntField].nType = nType;//���
			g_aStagingField[nCntField].rot = rot;//����
			g_aStagingField[nCntField].fWidth = fWidth;//����
			g_aStagingField[nCntField].fHeight = fHeight;//����
			g_aStagingField[nCntField].bUse = true;//�g�p���
			g_aStagingField[nCntField].fRot = fRot;//�����̕␳
			g_aStagingField[nCntField].nLife = nLife;//�̗�

			//���_���W�̍X�V
			pVtx[0].pos = D3DXVECTOR3(-fWidth, 0.0f, fHeight);
			pVtx[1].pos = D3DXVECTOR3(fWidth, 0.0f, fHeight);
			pVtx[2].pos = D3DXVECTOR3(-fWidth, 0.0f, -fHeight);
			pVtx[3].pos = D3DXVECTOR3(fWidth, 0.0f, -fHeight);

			break;
		}
		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffStagingField->Unlock();
}

StagingField* GetStagingField(void)
{
	return &g_aStagingField[0];
}
