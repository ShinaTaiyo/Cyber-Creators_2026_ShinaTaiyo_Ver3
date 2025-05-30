//=======================================================================================================================================================================================================================
//
// ���[��������ʂ̏���
// Author : Atsumu Kuboichi
//
//=======================================================================================================================================================================================================================
#include "rule.h"
#include "input.h"
#include "tutorial.h"

//=======================================================================================================================================================================================================================
// �O���[�o���ϐ�
//=======================================================================================================================================================================================================================
LPDIRECT3DTEXTURE9 g_pTextureRule[NUM_PAGE] = {};	//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRule = NULL;		//���_�o�b�t�@�ւ̃|�C���^
Rule g_aRule;										//�\���̏��

//=======================================================================================================================================================================================================================
// ����������
//=======================================================================================================================================================================================================================
void InitRule(void)
{
	VERTEX_2D* pVtx;						//���_���ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̎擾

	//�e�N�X�`���̓ǂݍ���
	for (int nCntBack = 0; nCntBack < NUM_PAGE; nCntBack++)
	{
		D3DXCreateTextureFromFile(pDevice, RULESCREEN_FILENAME[nCntBack], &g_pTextureRule[nCntBack]);
	}

	//�\���̂̏��
	g_aRule.nPage = 0;											//�y�[�W��
	g_aRule.fWidthScale = SCREEN_WIDTH / (float)SPLIT_SCALE;	//X���g�k��
	g_aRule.fHeightScale = SCREEN_HEIGHT / (float)SPLIT_SCALE;	//Y���g�k��
	g_aRule.bScale = true;										//�g�k�̐؂�ւ�
	g_aRule.bUse = true;										//�g�p���

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRule,
		NULL);

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffRule->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntVtx = 0; nCntVtx < 4; nCntVtx++)
	{
		pVtx[nCntVtx].rhw = 1.0f;								//rhw�̐ݒ�
		pVtx[nCntVtx].col = D3DCOLOR_RGBA(255, 255, 255, 255);	//���_�J���[�̐ݒ�
	}

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffRule->Unlock();
}

//=======================================================================================================================================================================================================================
// �I������
//=======================================================================================================================================================================================================================
void UninitRule(void)
{
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffRule != NULL)
	{
		g_pVtxBuffRule->Release();
		g_pVtxBuffRule = NULL;
	}
	//�e�N�X�`���̔j��
	for (int nCntRule = 0; nCntRule < NUM_PAGE; nCntRule++)
	{
		if (g_pTextureRule[nCntRule] != NULL)
		{
			g_pTextureRule[nCntRule]->Release();
			g_pTextureRule[nCntRule] = NULL;
		}
	}
}

//=======================================================================================================================================================================================================================
// �X�V����
//=======================================================================================================================================================================================================================
void UpdateRule(void)
{
	VERTEX_2D* pVtx;//���_���ւ̃|�C���^

	if (GetJoypadTrigger(JOYKEY_A))
	{//A�{�^���������ꂽ�ꍇ
		if (g_aRule.nPage < NUM_PAGE - 1)
		{//�Ō�̃y�[�W�ł͂Ȃ��ꍇ
			g_aRule.nPage++;//�y�[�W��i�߂�
		}
		else
		{//�Ō�̃y�[�W�̏ꍇ
			g_aRule.bScale = false;	//��ʂ��k������
		}
	}
	else if (GetJoypadTrigger(JOYKEY_B))
	{//B�{�^���������ꂽ�ꍇ
		if (g_aRule.nPage != 0)
		{//�ŏ��̃y�[�W�ł͂Ȃ��ꍇ
			g_aRule.nPage--;//�y�[�W��߂�
		}
	}

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffRule->Lock(0, 0, (void**)&pVtx, 0);

	if (g_aRule.bScale)
	{//�g�咆�̏ꍇ
		if (pVtx[0].pos != D3DXVECTOR3(0.0f, 0.0f, 0.0f))
		{//���_���ő�܂ŒB���Ă��Ȃ��ꍇ
			//�g��
			pVtx[0].pos.x -= g_aRule.fWidthScale;
			pVtx[0].pos.y -= g_aRule.fHeightScale;
			pVtx[1].pos.x += g_aRule.fWidthScale;
			pVtx[1].pos.y -= g_aRule.fHeightScale;
			pVtx[2].pos.x -= g_aRule.fWidthScale;
			pVtx[2].pos.y += g_aRule.fHeightScale;
			pVtx[3].pos.x += g_aRule.fWidthScale;
			pVtx[3].pos.y += g_aRule.fHeightScale;
		}
	}
	else
	{//�k�����̏ꍇ
		if (pVtx[0].pos != D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f))
		{//���_���ŏ��̈ʒu�܂ŒB���Ă��Ȃ��ꍇ
			//�k��
			pVtx[0].pos.x += g_aRule.fWidthScale;
			pVtx[0].pos.y += g_aRule.fHeightScale;
			pVtx[1].pos.x -= g_aRule.fWidthScale;
			pVtx[1].pos.y += g_aRule.fHeightScale;
			pVtx[2].pos.x += g_aRule.fWidthScale;
			pVtx[2].pos.y -= g_aRule.fHeightScale;
			pVtx[3].pos.x -= g_aRule.fWidthScale;
			pVtx[3].pos.y -= g_aRule.fHeightScale;
		}
		else
		{//���_���ŏ��̈ʒu�܂ŒB���Ă���ꍇ
			g_aRule.bUse = false;	//���[��������ʂ����
		}
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffRule->Unlock();
}

//=======================================================================================================================================================================================================================
// �`�揈��
//=======================================================================================================================================================================================================================
void DrawRule(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();							//�f�o�C�X�̎擾
	pDevice->SetStreamSource(0, g_pVtxBuffRule, 0, sizeof(VERTEX_2D));	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);										//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetTexture(0, g_pTextureRule[g_aRule.nPage]);				//�e�N�X�`���̐ݒ�
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);					//�|���S���̕`��
}

//=======================================================================================================================================================================================================================
// �\���̂̏��̎擾
//=======================================================================================================================================================================================================================
Rule* GetRuleScreen(void)
{
	return &g_aRule;
}
