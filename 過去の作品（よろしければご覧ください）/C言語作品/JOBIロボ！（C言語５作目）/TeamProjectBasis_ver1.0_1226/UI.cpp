//================================================================================================================================================================================================================================
//
// �Q�[��UI�̏���
// Author : Atsumu Kuboichi
//
//================================================================================================================================================================================================================================
#include "UI.h"
#include "model.h"
#include "player.h"

//================================================================================================================================================================================================================================
// �O���[�o���ϐ�
//================================================================================================================================================================================================================================
LPDIRECT3DTEXTURE9 g_pTextureUI[NUM_UI] = {};	//�e�N�X�`���ւ̃|�C���^
PDIRECT3DVERTEXBUFFER9 g_pVtxBuffUI = NULL;		//���_�o�b�t�@�ւ̃|�C���^
UI g_aUI[NUM_UI];								//�\���̂̏��

//================================================================================================================================================================================================================================
// ����������
//================================================================================================================================================================================================================================
void InitUI(void)
{
	VERTEX_2D* pVtx;						//���_���ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̎擾

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_UI,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffUI,
		NULL);

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffUI->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntUI = 0; nCntUI < NUM_UI; nCntUI++)
	{
		//�e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			UITEXTURE_FILENAME[nCntUI],
			&g_pTextureUI[nCntUI]);

		//�ʒu
		g_aUI[nCntUI].pos = UI_POS[nCntUI];
		g_aUI[nCntUI].bUse = true;

		if (nCntUI == 2)
		{
			g_aUI[nCntUI].bUse = false;
		}

		//�ʒu���̏�����
		pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - UI_WIDTH[nCntUI] / 2.0f, g_aUI[nCntUI].pos.y - UI_HEIGHT[nCntUI] / 2.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + UI_WIDTH[nCntUI] / 2.0f, g_aUI[nCntUI].pos.y - UI_HEIGHT[nCntUI] / 2.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - UI_WIDTH[nCntUI] / 2.0f, g_aUI[nCntUI].pos.y + UI_HEIGHT[nCntUI] / 2.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + UI_WIDTH[nCntUI] / 2.0f, g_aUI[nCntUI].pos.y + UI_HEIGHT[nCntUI] / 2.0f, 0.0f);

		//rhw�̐ݒ�i�����ݒ�j
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�i�����ݒ�j
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
	g_pVtxBuffUI->Unlock();
}

//================================================================================================================================================================================================================================
// �I������
//================================================================================================================================================================================================================================
void UninitUI(void)
{
	//�e�N�X�`���̔j��
	for (int nCntUI = 0; nCntUI < NUM_UI; nCntUI++)
	{
		if (g_pTextureUI[nCntUI] != NULL)
		{
			g_pTextureUI[nCntUI]->Release();
			g_pTextureUI[nCntUI] = NULL;
		}
	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffUI != NULL)
	{
		g_pVtxBuffUI->Release();
		g_pVtxBuffUI = NULL;
	}
}

//================================================================================================================================================================================================================================
// �X�V����
//================================================================================================================================================================================================================================
void UpdateUI(void)
{
	Player* pPlayer = GetPlayer();	//�v���C���[�\���̂̏����擾
	Model* pModel = GetModel();		//���f���\���̂̏����擾
	float fLength = 3000.0f;		//�e�R���Z���g�Ƃ̍ŏ�����

	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++, pModel++)
	{
		if (pModel->bUse && (pModel->nType == 19 || pModel->nType == 41))
		{//�R���Z���g���g�p����Ă���ꍇ
			//�v���C���[�Ƃ̋������v�Z
			pPlayer->fAutletLength = sqrtf((pPlayer->pos.x - pModel->pos.x) * (pPlayer->pos.x - pModel->pos.x) + (pPlayer->pos.z - pModel->pos.z) * (pPlayer->pos.z - pModel->pos.z)) / 2.0f;

			if (fLength > pPlayer->fAutletLength)
			{//�v�Z�����������ŏ��l�̏ꍇ
				fLength = pPlayer->fAutletLength;//�ŏ��������X�V
			}

			if (fLength <= 50.0f)
			{//�R���Z���g�̋߂��ɂ���ꍇ
				g_aUI[2].bUse = true;//�g�p����
			}
			else
			{//�R���Z���g�̋߂��ɂ��Ȃ��ꍇ
				g_aUI[2].bUse = false;//�g�p���Ȃ�
			}
		}
	}
}

//================================================================================================================================================================================================================================
// �`�揈��
//================================================================================================================================================================================================================================
void DrawUI(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̎擾

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffUI, 0, sizeof(VERTEX_2D));
	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntUI = 0; nCntUI < NUM_UI; nCntUI++)
	{
		if (g_aUI[nCntUI].bUse)
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureUI[nCntUI]);
			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntUI * 4, 2);
		}
	}
}
