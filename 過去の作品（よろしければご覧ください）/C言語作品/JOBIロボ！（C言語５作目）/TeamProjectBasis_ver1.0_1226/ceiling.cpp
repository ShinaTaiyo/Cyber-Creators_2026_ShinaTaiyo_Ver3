//=======================================================================================================================================================================================================================
//
// �V��̏���
// Author : Atsumu Kuboichi
//
//=======================================================================================================================================================================================================================
#include "ceiling.h"
#include "field.h"

//=======================================================================================================================================================================================================================
// �O���[�o���ϐ�
//=======================================================================================================================================================================================================================
Ceiling g_aCeiling;	//�\���̂̏��

//=======================================================================================================================================================================================================================
// ����������
//=======================================================================================================================================================================================================================
void InitCeiling(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̎擾
	VERTEX_3D* pVtx;						//���_���ւ̃|�C���^

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Ceiling000.png",
		&g_aCeiling.pTexture);

	//�ϐ�
	g_aCeiling.pos = D3DXVECTOR3(0.0f, 1500.0f, 0.0f);	//�ʒu
	g_aCeiling.rot = D3DXVECTOR3(0.0f, 0.0f, D3DX_PI);	//����
	g_aCeiling.fWidth = FIELD_WIDTH;					//����
	g_aCeiling.fHeight = FIELD_HEIGHT;					//�c��

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_aCeiling.pVtxBuff,
		NULL);

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_aCeiling.pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-g_aCeiling.fWidth, 0.0f, g_aCeiling.fHeight);
	pVtx[1].pos = D3DXVECTOR3(g_aCeiling.fWidth, 0.0f, g_aCeiling.fHeight);
	pVtx[2].pos = D3DXVECTOR3(-g_aCeiling.fWidth, 0.0f, -g_aCeiling.fHeight);
	pVtx[3].pos = D3DXVECTOR3(g_aCeiling.fWidth, 0.0f, -g_aCeiling.fHeight);

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
	g_aCeiling.pVtxBuff->Unlock();
}

//=======================================================================================================================================================================================================================
// �I������
//=======================================================================================================================================================================================================================
void UninitCeiling(void)
{
	//�e�N�X�`���̔j��
	if (g_aCeiling.pTexture != NULL)
	{//�e�N�X�`����񂪑��݂���ꍇ
		g_aCeiling.pTexture->Release();
		g_aCeiling.pTexture = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_aCeiling.pVtxBuff != NULL)
	{//���_��񂪑��݂���ꍇ
		g_aCeiling.pVtxBuff->Release();
		g_aCeiling.pVtxBuff = NULL;
	}
}

//=======================================================================================================================================================================================================================
// �`�揈��
//=======================================================================================================================================================================================================================
void DrawCeiling(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans;			//�v�Z�p�}�g���b�N�X

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_aCeiling.mtxworld);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aCeiling.rot.y, g_aCeiling.rot.x, g_aCeiling.rot.z);
	D3DXMatrixMultiply(&g_aCeiling.mtxworld, &g_aCeiling.mtxworld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, g_aCeiling.pos.x, g_aCeiling.pos.y, g_aCeiling.pos.z);
	D3DXMatrixMultiply(&g_aCeiling.mtxworld, &g_aCeiling.mtxworld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_aCeiling.mtxworld);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_aCeiling.pVtxBuff, 0, sizeof(VERTEX_3D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_aCeiling.pTexture);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}
