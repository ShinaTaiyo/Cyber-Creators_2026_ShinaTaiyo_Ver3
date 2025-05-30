//=======================================================================================================================================================================================================================
//
// �R���{�{���\���̏���
// Author : Atsumu Kuboichi
//
//=======================================================================================================================================================================================================================
#include "combo.h"
#include <cstdio>
#include "player.h"

//=======================================================================================================================================================================================================================
// �O���[�o���ϐ�
//=======================================================================================================================================================================================================================
LPDIRECT3DTEXTURE9 g_pTextureComboUI = NULL;//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffComboUI = NULL;//���_�o�b�t�@�ւ̃|�C���^
ComboUI g_aComboUI;//�\���̂̏��

LPDIRECT3DTEXTURE9 g_pTextureComboNUM = NULL;//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffComboNUM = NULL;//���_�o�b�t�@�ւ̃|�C���^
ComboNum g_aComboNum[NUM_COMBO];//�\���̂̏��

//=======================================================================================================================================================================================================================
// ����������
//=======================================================================================================================================================================================================================
void InitCombo(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̎擾
	VERTEX_2D* pVtx;//���_���ւ̃|�C���^
	int aPosTexU[NUM_COMBO];//�e���̐������i�[
	int nMulti = 1;//�e���̔{��

	//=======================================================
	// �R���{UI
	//=======================================================

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\ComboUI000.png",
		&g_pTextureComboUI);	//�R���{�{������

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_COMBO,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffComboUI,
		NULL);

	//�e���̐��ݒ�
	for (int nCntCombo = 1; nCntCombo <= NUM_COMBO; nCntCombo++)
	{
		nMulti *= 10;
		aPosTexU[NUM_COMBO - nCntCombo] = (10 % nMulti) / (nMulti / 10);
	}

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffComboUI->Lock(0, 0, (void**)&pVtx, 0);
	
	//���
	g_aComboUI.pos = D3DXVECTOR3(COMBO_UI_POS.x, COMBO_UI_POS.y, COMBO_UI_POS.z);//�ʒu
	g_aComboUI.fLength = sqrtf(COMBO_UI_WIDTH * COMBO_UI_WIDTH + COMBO_UI_HEIGHT * COMBO_UI_HEIGHT) / 2.0f;//�Ίp���̒���
	g_aComboUI.fAngle = atan2f(COMBO_UI_WIDTH, COMBO_UI_HEIGHT);//�Ίp���̊p�x
	g_aComboUI.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//�F

	//���_���W�̐ݒ�
	pVtx[0].pos.x = g_aComboUI.pos.x + sinf(g_aComboUI.pos.z - (D3DX_PI - g_aComboUI.fAngle)) * g_aComboUI.fLength;
	pVtx[0].pos.y = g_aComboUI.pos.y + cosf(g_aComboUI.pos.z - (D3DX_PI - g_aComboUI.fAngle)) * g_aComboUI.fLength;
	pVtx[0].pos.z = 0.0f;
	pVtx[1].pos.x = g_aComboUI.pos.x + sinf(g_aComboUI.pos.z + (D3DX_PI - g_aComboUI.fAngle)) * g_aComboUI.fLength;
	pVtx[1].pos.y = g_aComboUI.pos.y + cosf(g_aComboUI.pos.z + (D3DX_PI - g_aComboUI.fAngle)) * g_aComboUI.fLength;
	pVtx[1].pos.z = 0.0f;
	pVtx[2].pos.x = g_aComboUI.pos.x + sinf(g_aComboUI.pos.z - g_aComboUI.fAngle) * g_aComboUI.fLength;
	pVtx[2].pos.y = g_aComboUI.pos.y + cosf(g_aComboUI.pos.z - g_aComboUI.fAngle) * g_aComboUI.fLength;
	pVtx[2].pos.z = 0.0f;
	pVtx[3].pos.x = g_aComboUI.pos.x + sinf(g_aComboUI.pos.z + g_aComboUI.fAngle) * g_aComboUI.fLength;
	pVtx[3].pos.y = g_aComboUI.pos.y + cosf(g_aComboUI.pos.z + g_aComboUI.fAngle) * g_aComboUI.fLength;
	pVtx[3].pos.z = 0.0f;

	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[�̐ݒ�
	pVtx[0].col = g_aComboUI.col;
	pVtx[1].col = g_aComboUI.col;
	pVtx[2].col = g_aComboUI.col;
	pVtx[3].col = g_aComboUI.col;

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffComboUI->Unlock();

	//=======================================================
	// �R���{�{��
	//=======================================================

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\ComboNumber000.png",
		&g_pTextureComboNUM);	//�R���{�{������

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_COMBO,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffComboNUM,
		NULL);

	//�e���̐��ݒ�
	for (int nCntCombo = 1; nCntCombo <= NUM_COMBO; nCntCombo++)
	{
		nMulti *= 10;
		aPosTexU[NUM_COMBO - nCntCombo] = (10 % nMulti) / (nMulti / 10);
	}

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffComboNUM->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntCombo = 0; nCntCombo < NUM_COMBO; nCntCombo++)
	{
		//���
		g_aComboNum[nCntCombo].pos = D3DXVECTOR3(COMBO_NUM_POS.x + (COMBO_NUM_SPACE * nCntCombo), COMBO_NUM_POS.y, COMBO_NUM_POS.z);//�ʒu
		g_aComboNum[nCntCombo].fLength = sqrtf(COMBO_NUM_WIDTH * COMBO_NUM_WIDTH + COMBO_NUM_HEIGHT * COMBO_NUM_HEIGHT) / 2.0f;//�Ίp���̒���
		g_aComboNum[nCntCombo].fAngle = atan2f(COMBO_NUM_WIDTH, COMBO_NUM_HEIGHT);//�Ίp���̊p�x
		g_aComboNum[nCntCombo].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//�F

		//���_���W�̐ݒ�
		pVtx[0].pos.x = g_aComboNum[nCntCombo].pos.x + sinf(g_aComboNum[nCntCombo].pos.z - (D3DX_PI - g_aComboNum[nCntCombo].fAngle)) * g_aComboNum[nCntCombo].fLength;
		pVtx[0].pos.y = g_aComboNum[nCntCombo].pos.y + cosf(g_aComboNum[nCntCombo].pos.z - (D3DX_PI - g_aComboNum[nCntCombo].fAngle)) * g_aComboNum[nCntCombo].fLength;
		pVtx[0].pos.z = 0.0f;
		pVtx[1].pos.x = g_aComboNum[nCntCombo].pos.x + sinf(g_aComboNum[nCntCombo].pos.z + (D3DX_PI - g_aComboNum[nCntCombo].fAngle)) * g_aComboNum[nCntCombo].fLength;
		pVtx[1].pos.y = g_aComboNum[nCntCombo].pos.y + cosf(g_aComboNum[nCntCombo].pos.z + (D3DX_PI - g_aComboNum[nCntCombo].fAngle)) * g_aComboNum[nCntCombo].fLength;
		pVtx[1].pos.z = 0.0f;
		pVtx[2].pos.x = g_aComboNum[nCntCombo].pos.x + sinf(g_aComboNum[nCntCombo].pos.z - g_aComboNum[nCntCombo].fAngle) * g_aComboNum[nCntCombo].fLength;
		pVtx[2].pos.y = g_aComboNum[nCntCombo].pos.y + cosf(g_aComboNum[nCntCombo].pos.z - g_aComboNum[nCntCombo].fAngle) * g_aComboNum[nCntCombo].fLength;
		pVtx[2].pos.z = 0.0f;
		pVtx[3].pos.x = g_aComboNum[nCntCombo].pos.x + sinf(g_aComboNum[nCntCombo].pos.z + g_aComboNum[nCntCombo].fAngle) * g_aComboNum[nCntCombo].fLength;
		pVtx[3].pos.y = g_aComboNum[nCntCombo].pos.y + cosf(g_aComboNum[nCntCombo].pos.z + g_aComboNum[nCntCombo].fAngle) * g_aComboNum[nCntCombo].fLength;
		pVtx[3].pos.z = 0.0f;

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[0].col = g_aComboNum[nCntCombo].col;
		pVtx[1].col = g_aComboNum[nCntCombo].col;
		pVtx[2].col = g_aComboNum[nCntCombo].col;
		pVtx[3].col = g_aComboNum[nCntCombo].col;

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(aPosTexU[nCntCombo] / 10.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2((aPosTexU[nCntCombo] + 1) / 10.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(aPosTexU[nCntCombo] / 10.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2((aPosTexU[nCntCombo] + 1) / 10.0f, 1.0f);

		//���_�f�[�^�̃|�C���^��i�߂�
		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffComboNUM->Unlock();
}

//=======================================================================================================================================================================================================================
// �I������
//=======================================================================================================================================================================================================================
void UninitCombo(void)
{
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffComboUI != NULL)
	{
		g_pVtxBuffComboUI->Release();
		g_pVtxBuffComboUI = NULL;
	}
	if (g_pVtxBuffComboNUM != NULL)
	{
		g_pVtxBuffComboNUM->Release();
		g_pVtxBuffComboNUM = NULL;
	}
	//�e�N�X�`���̔j��
	if (g_pTextureComboUI != NULL)
	{
		g_pTextureComboUI->Release();
		g_pTextureComboUI = NULL;
	}
	if (g_pTextureComboNUM != NULL)
	{
		g_pTextureComboNUM->Release();
		g_pTextureComboNUM = NULL;
	}
}

//=======================================================================================================================================================================================================================
// �X�V����
//=======================================================================================================================================================================================================================
void UpdateCombo(void)
{
	VERTEX_2D* pVtx;//���_���ւ̃|�C���^
	Player* pPlayer = GetPlayer();//�v���C���[�̏����擾
	int aPosTexU[NUM_COMBO];//�e���̐������i�[
	int nMulti = 1;//�e���̔{��

	//�e���̐������X�V
	for (int nCntCombo = 1; nCntCombo <= NUM_COMBO; nCntCombo++)
	{
		nMulti *= 10;
		aPosTexU[NUM_COMBO - nCntCombo] = ((int)(pPlayer->fCombo * 10) % nMulti) / (nMulti / 10);
	}

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffComboNUM->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntCombo = 0; nCntCombo < NUM_COMBO; nCntCombo++)
	{
		//�e�N�X�`�����W�̍X�V
		pVtx[0].tex = D3DXVECTOR2(aPosTexU[nCntCombo] / 10.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2((aPosTexU[nCntCombo] + 1) / 10.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(aPosTexU[nCntCombo] / 10.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2((aPosTexU[nCntCombo] + 1) / 10.0f, 1.0f);

		//�F�̍X�V
		g_aComboNum[nCntCombo].col = D3DXCOLOR(1.0f, MAX_COMBO - pPlayer->fCombo, MAX_COMBO - pPlayer->fCombo, 1.0f);

		if (pPlayer->fCombo <= 1.0f)
		{//�R���{�{����1.0�̏ꍇ�͔�\��
			g_aComboNum[nCntCombo].col.a = 0.0f;
		}

		//���_�J���[�̍X�V
		pVtx[0].col = g_aComboNum[nCntCombo].col;
		pVtx[1].col = g_aComboNum[nCntCombo].col;
		pVtx[2].col = g_aComboNum[nCntCombo].col;
		pVtx[3].col = g_aComboNum[nCntCombo].col;

		//���_�f�[�^�̃|�C���^��i�߂�
		pVtx += 4;
	}

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffComboUI->Lock(0, 0, (void**)&pVtx, 0);

	//���_�J���[�̍X�V
	pVtx[0].col = g_aComboNum[0].col;
	pVtx[1].col = g_aComboNum[0].col;
	pVtx[2].col = g_aComboNum[0].col;
	pVtx[3].col = g_aComboNum[0].col;

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffComboUI->Unlock();

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffComboNUM->Unlock();
}

//=======================================================================================================================================================================================================================
// �R���{UI�`�揈��
//=======================================================================================================================================================================================================================
void DrawComboUI(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̎擾
	pDevice->SetStreamSource(0, g_pVtxBuffComboUI, 0, sizeof(VERTEX_2D));//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetTexture(0, g_pTextureComboUI);//�e�N�X�`���̐ݒ�
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);//�|���S���̕`��
}

//=======================================================================================================================================================================================================================
// �R���{�{���`�揈��
//=======================================================================================================================================================================================================================
void DrawComboNum(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̎擾
	pDevice->SetStreamSource(0, g_pVtxBuffComboNUM, 0, sizeof(VERTEX_2D));//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetTexture(0, g_pTextureComboNUM);//�e�N�X�`���̐ݒ�

	//�|���S���̕`��
	for (int nCntCombo = 0; nCntCombo < NUM_COMBO; nCntCombo++)
	{
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntCombo * 4, 2);
	}
}
