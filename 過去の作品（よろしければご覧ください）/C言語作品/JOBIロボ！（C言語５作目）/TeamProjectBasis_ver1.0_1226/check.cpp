//=======================================================================================================================================================================================================================
//
// �`�F�b�N�}�[�N�̏���
// Author : Atsumu Kuboichi
//
//=======================================================================================================================================================================================================================
#include "check.h"
#include "input.h"
#include "rule.h"

//=======================================================================================================================================================================================================================
// �O���[�o���ϐ�
//=======================================================================================================================================================================================================================
PDIRECT3DVERTEXBUFFER9 g_pVtxBuffCheck = NULL;	//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureCheck = NULL;		//�e�N�X�`���ւ̃|�C���^
Check g_aCheck[MAX_CHECK];						//���̍\����
int g_nCheck;									//���̓`�F�b�N��

//=======================================================================================================================================================================================================================
// ����������
//=======================================================================================================================================================================================================================
void InitCheck(void)
{
	VERTEX_2D* pVtx;						//���_���ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̎擾

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Check000.png",
		&g_pTextureCheck);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_CHECK,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffCheck,
		NULL);

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffCheck->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntCheck = 0; nCntCheck < MAX_CHECK; nCntCheck++)
	{
		g_aCheck[nCntCheck].pos = D3DXVECTOR3(277.5f, 24.0f + nCntCheck * 52.2f, 0.0f);	//�ʒu
		g_aCheck[nCntCheck].nNum = nCntCheck;											//�ԍ�
		g_aCheck[nCntCheck].fWidth = 37.0f;												//����
		g_aCheck[nCntCheck].fHeight = 37.0f;											//����
		g_aCheck[nCntCheck].bUse = false;												//�g�p����
		
		for (int nCntVtx = 0; nCntVtx < 4; nCntVtx++)
		{
			//rhw�̐ݒ�
			pVtx[nCntVtx].rhw = 1.0f;

			//���_�J���[�̐ݒ�
			pVtx[nCntVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aCheck[nCntCheck].pos.x - g_aCheck[nCntCheck].fWidth * 0.5f, g_aCheck[nCntCheck].pos.y - g_aCheck[nCntCheck].fHeight * 0.5f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aCheck[nCntCheck].pos.x + g_aCheck[nCntCheck].fWidth * 0.5f, g_aCheck[nCntCheck].pos.y - g_aCheck[nCntCheck].fHeight * 0.5f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aCheck[nCntCheck].pos.x - g_aCheck[nCntCheck].fWidth * 0.5f, g_aCheck[nCntCheck].pos.y + g_aCheck[nCntCheck].fHeight * 0.5f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aCheck[nCntCheck].pos.x + g_aCheck[nCntCheck].fWidth * 0.5f, g_aCheck[nCntCheck].pos.y + g_aCheck[nCntCheck].fHeight * 0.5f, 0.0f);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	//�ϐ�
	g_nCheck = 0;//�`�F�b�N��

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffCheck->Unlock();
}

//=======================================================================================================================================================================================================================
// �I������
//=======================================================================================================================================================================================================================
void UninitCheck(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureCheck != NULL)
	{//�e�N�X�`����񂪑��݂���ꍇ
		g_pTextureCheck->Release();
		g_pTextureCheck = NULL;
	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffCheck != NULL)
	{//���_��񂪑��݂���ꍇ
		g_pVtxBuffCheck->Release();
		g_pVtxBuffCheck = NULL;
	}
}

//=======================================================================================================================================================================================================================
// �X�V����
//=======================================================================================================================================================================================================================
void UpdateCheck(void)
{
	//==============================================================================================================================
	// ���͔���
	//==============================================================================================================================
	if (GetJoypadPress(JOYKEY_UP) || GetJoypadPress(JOYKEY_DOWN) || GetJoypadPress(JOYKEY_RIGHT) || GetJoypadPress(JOYKEY_LEFT))
	{//�ړ��{�^�������͂��ꂽ�ꍇ
		if (!g_aCheck[0].bUse)
		{//�`�F�b�N�ς݂ł͂Ȃ��ꍇ
			g_nCheck++;	//�`�F�b�N�������Z
		}

		g_aCheck[0].bUse = true;//�`�F�b�N�ς݂ɂ���
	}
	if (GetJoypadTrigger(JOYKEY_LB) || GetJoypadTrigger(JOYKEY_RB))
	{//LB or RB�����͂��ꂽ�ꍇ
		if (!g_aCheck[1].bUse)
		{//�`�F�b�N�ς݂ł͂Ȃ��ꍇ
			g_nCheck++;	//�`�F�b�N�������Z
		}

		g_aCheck[1].bUse = true;//�`�F�b�N�ς݂ɂ���
	}
	if (GetJoypadPress(JOYKEY_X))
	{//X�����͂��ꂽ�ꍇ
		if (!g_aCheck[2].bUse)
		{//�`�F�b�N�ς݂ł͂Ȃ��ꍇ
			g_nCheck++;	//�`�F�b�N�������Z
		}

		g_aCheck[2].bUse = true;//�`�F�b�N�ς݂ɂ���
	}
	if (GetJoypadPress(JOYKEY_Y))
	{//Y�����͂��ꂽ�ꍇ
		if (!g_aCheck[3].bUse)
		{//�`�F�b�N�ς݂ł͂Ȃ��ꍇ
			g_nCheck++;	//�`�F�b�N�������Z
		}

		g_aCheck[3].bUse = true;//�`�F�b�N�ς݂ɂ���
	}
	if (GetJoypadPress(JOYKEY_B))
	{//B�����͂��ꂽ�ꍇ
		if (!g_aCheck[4].bUse)
		{//�`�F�b�N�ς݂ł͂Ȃ��ꍇ
			g_nCheck++;	//�`�F�b�N�������Z
		}

		g_aCheck[4].bUse = true;//�`�F�b�N�ς݂ɂ���
	}
	if (GetJoypadPress(JOYKEY_A))
	{//B�����͂��ꂽ�ꍇ
		if (!g_aCheck[5].bUse)
		{//�`�F�b�N�ς݂ł͂Ȃ��ꍇ
			g_nCheck++;	//�`�F�b�N�������Z
		}

		g_aCheck[5].bUse = true;//�`�F�b�N�ς݂ɂ���
	}
}

//=======================================================================================================================================================================================================================
// �`�揈��
//=======================================================================================================================================================================================================================
void DrawCheck(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();							//�f�o�C�X�̎擾
	pDevice->SetStreamSource(0, g_pVtxBuffCheck, 0, sizeof(VERTEX_2D));	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);										//���_�t�H�[�}�b�g�̐ݒ�

	for (int nCntCheck = 0; nCntCheck < MAX_CHECK; nCntCheck++)
	{
		if (g_aCheck[nCntCheck].bUse)
		{//�g�p����Ă���ꍇ
			pDevice->SetTexture(0, g_pTextureCheck);						//�e�N�X�`���̐ݒ�
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntCheck * 4, 2);	//�|���S���̕`��
		}
	}
}

//=======================================================================================================================================================================================================================
// ���̍\���̂��擾
//=======================================================================================================================================================================================================================
int GetCheck(void)
{
	return g_nCheck;
}
