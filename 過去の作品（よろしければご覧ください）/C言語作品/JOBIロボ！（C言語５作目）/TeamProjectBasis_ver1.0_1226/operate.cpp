//=======================================================================================================================================================================================================================
//
// �`���[�g���A��UI�̏���
// Author : Atsumu Kuboichi
//
//=======================================================================================================================================================================================================================
#include "operate.h"
#include "check.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "game.h"

//=======================================================================================================================================================================================================================
// �O���[�o���ϐ�
//=======================================================================================================================================================================================================================
PDIRECT3DVERTEXBUFFER9 g_pVtxBuffOperate = NULL;		//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureOperate[MAX_OPERATE] = {};	//�e�N�X�`���ւ̃|�C���^
Operate g_aOperate[MAX_OPERATE];						//���̍\����

//=======================================================================================================================================================================================================================
// ����������
//=======================================================================================================================================================================================================================
void InitOperate(void)
{
	VERTEX_2D* pVtx;						//���_���ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̎擾

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_OPERATE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffOperate,
		NULL);

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffOperate->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntOperate = 0; nCntOperate < MAX_OPERATE; nCntOperate++)
	{
		//�e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			OPERATE_FILENAME[nCntOperate],
			&g_pTextureOperate[nCntOperate]);

		
		//�ϐ�
		switch (nCntOperate)
		{
		case 0://�u������m�F���悤�I�v�̕\��
			g_aOperate[nCntOperate].bUse = true;							//�g�p����
			g_aOperate[nCntOperate].pos = D3DXVECTOR3(210.0f, 40.0f, 0.0f);//�ʒu
			g_aOperate[nCntOperate].fWidth = 400.0f;						//����
			g_aOperate[nCntOperate].fHeight = 80.0f;						//����
			break;

		case 1://������@�ꗗ
			g_aOperate[nCntOperate].bUse = true;							//�g�p����
			g_aOperate[nCntOperate].pos = D3DXVECTOR3(300.0f, 560.0f, 0.0f);//�ʒu
			g_aOperate[nCntOperate].fWidth = 500.0f;						//����
			g_aOperate[nCntOperate].fHeight = 300.0f;						//����
			break;

		case 2://�Q�[���X�^�[�g�̕\��
			g_aOperate[nCntOperate].bUse = true;							//�g�p���Ȃ�
			g_aOperate[nCntOperate].pos = D3DXVECTOR3(940.0f, 640.0f, 0.0f);//�ʒu
			g_aOperate[nCntOperate].fWidth = 600.0f;						//����
			g_aOperate[nCntOperate].fHeight = 100.0f;						//����
			break;

		default:
			break;
		}

		for (int nCntVtx = 0; nCntVtx < 4; nCntVtx++)
		{
			//rhw�̐ݒ�
			pVtx[nCntVtx].rhw = 1.0f;

			//���_�J���[�̐ݒ�
			pVtx[nCntVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aOperate[nCntOperate].pos.x - g_aOperate[nCntOperate].fWidth * 0.5f, g_aOperate[nCntOperate].pos.y - g_aOperate[nCntOperate].fHeight * 0.5f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aOperate[nCntOperate].pos.x + g_aOperate[nCntOperate].fWidth * 0.5f, g_aOperate[nCntOperate].pos.y - g_aOperate[nCntOperate].fHeight * 0.5f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aOperate[nCntOperate].pos.x - g_aOperate[nCntOperate].fWidth * 0.5f, g_aOperate[nCntOperate].pos.y + g_aOperate[nCntOperate].fHeight * 0.5f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aOperate[nCntOperate].pos.x + g_aOperate[nCntOperate].fWidth * 0.5f, g_aOperate[nCntOperate].pos.y + g_aOperate[nCntOperate].fHeight * 0.5f, 0.0f);

		//�e�N�X�`�����W�̐ݒ�i�����ݒ�j
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffOperate->Unlock();
}

//=======================================================================================================================================================================================================================
// �I������
//=======================================================================================================================================================================================================================
void UninitOperate(void)
{
	//�e�N�X�`���̔j��
	for (int nCntTex = 0; nCntTex < MAX_OPERATE; nCntTex++)
	{
		if (g_pTextureOperate[nCntTex] != NULL)
		{//�e�N�X�`����񂪑��݂���ꍇ
			g_pTextureOperate[nCntTex]->Release();
			g_pTextureOperate[nCntTex] = NULL;
		}
	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffOperate != NULL)
	{//���_��񂪑��݂���ꍇ
		g_pVtxBuffOperate->Release();
		g_pVtxBuffOperate = NULL;
	}
}

//=======================================================================================================================================================================================================================
// �X�V����
//=======================================================================================================================================================================================================================
void UpdateOperate(void)
{

}

//=======================================================================================================================================================================================================================
// �`�揈��
//=======================================================================================================================================================================================================================
void DrawOperate(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();								//�f�o�C�X�̎擾
	pDevice->SetStreamSource(0, g_pVtxBuffOperate, 0, sizeof(VERTEX_2D));	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);											//���_�t�H�[�}�b�g�̐ݒ�

	for (int nCntOperate = 0; nCntOperate < MAX_OPERATE; nCntOperate++)
	{
		if (g_aOperate[nCntOperate].bUse == true)
		{//�g�p����Ă���ꍇ
			pDevice->SetTexture(0, g_pTextureOperate[nCntOperate]);			//�e�N�X�`���̐ݒ�
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntOperate * 4, 2);//�|���S���̕`��
		}
	}
}
