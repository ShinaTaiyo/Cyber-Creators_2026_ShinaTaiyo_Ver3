//================================================================================================================================================================================================================================
//
// �r���{�[�h�̏���
// Author : Atsumu Kuboichi
//
//================================================================================================================================================================================================================================
#include "billboard.h"
#include "debugproc.h"
#include "player.h"

//================================================================================================================================================================================================================================
// �O���[�o���ϐ�
//================================================================================================================================================================================================================================
PDIRECT3DVERTEXBUFFER9 g_pVtxBuffBillboard = NULL;							//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureBillboard[(int)USE_MAX][NUM_BILLBOARD] = {};	//�e�N�X�`���ւ̃|�C���^
Billboard g_aBillboard[MAX_BILLBOARD];										//�\���̏��

//================================================================================================================================================================================================================================
// ����������
//================================================================================================================================================================================================================================
void InitBillboard(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̎擾
	VERTEX_3D* pVtx;						//���_���ւ̃|�C���^

	//�e�N�X�`���̓ǂݍ���
	for (int nCntTex = 0; nCntTex < (int)USE_MAX; nCntTex++)
	{
		if (nCntTex == 0)
		{//�Q�[���p
			for (int nCntTex2 = 0; nCntTex2 < GAME_BILLBOARD; nCntTex2++)
			{
				D3DXCreateTextureFromFile(pDevice,
					GAME_BILLBOARDFILE[nCntTex2],
					&g_pTextureBillboard[nCntTex][nCntTex2]);
			}
		}
		else if (nCntTex == 1)
		{//�`���[�g���A���p
			for (int nCntTex2 = 0; nCntTex2 < TUTORIAL_BILLBOARD; nCntTex2++)
			{
				D3DXCreateTextureFromFile(pDevice,
					TUTORIAL_BILLBOARDFILE[nCntTex2],
					&g_pTextureBillboard[nCntTex][nCntTex2]);
			}
		}
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_BILLBOARD,//���_�̐�
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBillboard,
		NULL);

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBillboard->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBillboard = 0; nCntBillboard < MAX_BILLBOARD; nCntBillboard++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-g_aBillboard[nCntBillboard].fWidth / 2.0f, g_aBillboard[nCntBillboard].fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aBillboard[nCntBillboard].fWidth / 2.0f, g_aBillboard[nCntBillboard].fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-g_aBillboard[nCntBillboard].fWidth / 2.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aBillboard[nCntBillboard].fWidth / 2.0f, 0.0f, 0.0f);

		for (int nCntVtx = 0; nCntVtx < 4; nCntVtx++)
		{
			//nor�̐ݒ�
			pVtx[nCntVtx].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			//���_�J���[�̐ݒ�
			pVtx[nCntVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBillboard->Unlock();
}

//================================================================================================================================================================================================================================
// �I������
//================================================================================================================================================================================================================================
void UninitBillboard(void)
{
	//�e�N�X�`���̔j��
	for (int nCntTex = 0; nCntTex < (int)USE_MAX; nCntTex++)
	{
		for (int nCntTex2 = 0; nCntTex2 < NUM_BILLBOARD; nCntTex2++)
		{
			if (g_pTextureBillboard[nCntTex][nCntTex2] != NULL)
			{//�e�N�X�`�������݂���ꍇ
				g_pTextureBillboard[nCntTex][nCntTex2]->Release();
				g_pTextureBillboard[nCntTex][nCntTex2] = NULL;
			}
		}
	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffBillboard != NULL)
	{//���_��񂪑��݂���ꍇ
		g_pVtxBuffBillboard->Release();
		g_pVtxBuffBillboard = NULL;
	}
}

//================================================================================================================================================================================================================================
// �X�V����
//================================================================================================================================================================================================================================
void UpdateBillboard(void)
{
	for (int nCntBillboard = 0; nCntBillboard < MAX_BILLBOARD; nCntBillboard++)
	{
		if ((GetMode() == MODE_TUTORIAL && g_aBillboard[nCntBillboard].nUse == (int)USE_TUTORIAL)
			|| (GetMode() == MODE_GAME && g_aBillboard[nCntBillboard].nUse == (int)USE_GAME))
		{//���݂̉�ʂƎg�p��ʂ���v����ꍇ
			g_aBillboard[nCntBillboard].bUse = true;//�g�p����
		}
		else
		{//��v���Ȃ��ꍇ
			g_aBillboard[nCntBillboard].bUse = false;//�g�p���Ȃ�
		}
	}
}

//================================================================================================================================================================================================================================
// �`�揈��
//================================================================================================================================================================================================================================
void DrawBillboard(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̎擾
	D3DXMATRIX mtxTrans;					//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxView;						//�r���[�}�g���b�N�X�擾�p

	for (int nCntBillboard = 0; nCntBillboard < MAX_BILLBOARD; nCntBillboard++)
	{
		if (g_aBillboard[nCntBillboard].bUse)
		{//�g�p���Ă���ꍇ
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aBillboard[nCntBillboard].mtxWorld);

			//�r���[�}�g���b�N�X���擾
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//�|���S�����J�����ɑ΂��Đ��ʂɌ�����
			D3DXMatrixInverse(&g_aBillboard[nCntBillboard].mtxWorld, NULL, &mtxView);

			//�t�s������߂�
			g_aBillboard[nCntBillboard].mtxWorld._41 = 0.0f;
			g_aBillboard[nCntBillboard].mtxWorld._42 = 0.0f;
			g_aBillboard[nCntBillboard].mtxWorld._43 = 0.0f;

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aBillboard[nCntBillboard].pos.x, g_aBillboard[nCntBillboard].pos.y, g_aBillboard[nCntBillboard].pos.z);
			D3DXMatrixMultiply(&g_aBillboard[nCntBillboard].mtxWorld, &g_aBillboard[nCntBillboard].mtxWorld, &mtxTrans);

			//���C�g�𖳌��ɂ���
			pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

			//Z�̔�r���@�ύX�B
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

			//Z�o�b�t�@�ɏ������܂Ȃ�
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aBillboard[nCntBillboard].mtxWorld);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffBillboard, 0, sizeof(VERTEX_3D));//���_�o�b�t�@�ւ̃|�C���^�ƒ��_���̍\���̂̃T�C�Y

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�g�p��ʂƎ�ނ�ۑ�
			int nUse = g_aBillboard[nCntBillboard].nUse;
			int nType = g_aBillboard[nCntBillboard].nType;

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureBillboard[nUse][nType]);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBillboard * 4, 2);

			//���C�g��L���ɖ߂�
			pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

			//Z�̔�r���@�ύX�B
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

			//Z�o�b�t�@�ɏ�������
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		}
	}
}

//================================================================================================================================================================================================================================
// �ݒ菈��
//================================================================================================================================================================================================================================
void SetBillboard(D3DXVECTOR3 pos, int nUse, int nType, float fWidth, float fHeight)
{
	VERTEX_3D* pVtx;//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBillboard->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBillboard = 0; nCntBillboard < MAX_BILLBOARD; nCntBillboard++)
	{
		if (!g_aBillboard[nCntBillboard].bUse)
		{//�g�p����Ă��Ȃ��ꍇ
			g_aBillboard[nCntBillboard].pos = pos;										//�ʒu
			g_aBillboard[nCntBillboard].nUse = nUse;									//�g�p���
			g_aBillboard[nCntBillboard].nType = nType;									//���
			g_aBillboard[nCntBillboard].fWidth = fWidth;								//����
			g_aBillboard[nCntBillboard].fHeight = fHeight;								//����
			g_aBillboard[nCntBillboard].fWidthScale = (fWidth / 2.0f) / DIVISION_SCALE;	//�����g�k��
			g_aBillboard[nCntBillboard].fHeightScale = fHeight / DIVISION_SCALE;		//�����g�k��

			//���_���W�̐ݒ�
			if (nUse == 0 && nType == 0)
			{//�uA�ŉ񕜁v�̏ꍇ
				for (int nCntVtx = 0; nCntVtx < 4; nCntVtx++)
				{
					pVtx[nCntVtx].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				}
			}
			else
			{
				pVtx[0].pos = D3DXVECTOR3(-g_aBillboard[nCntBillboard].fWidth / 2.0f, g_aBillboard[nCntBillboard].fHeight, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aBillboard[nCntBillboard].fWidth / 2.0f, g_aBillboard[nCntBillboard].fHeight, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(-g_aBillboard[nCntBillboard].fWidth / 2.0f, 0.0f, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aBillboard[nCntBillboard].fWidth / 2.0f, 0.0f, 0.0f);
			}

			break;
		}

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBillboard->Unlock();
}